/*!
 \file     signal.c
 \author   Trevor Williams  (trevorw@charter.net)
 \date     12/1/2001
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "defines.h"
#include "signal.h"
#include "expr.h"
#include "link.h"
#include "vector.h"
#include "module.h"
#include "util.h"
#include "sim.h"


extern nibble or_optab[16];


/*!
 \param sig        Pointer to signal to initialize.
 \param name       Pointer to signal name string.
 \param value      Pointer to signal value.
 
 Initializes the specified signal with the values of name and value.  This
 function is called by the signal_create routine and is also useful for
 creating temporary signals (reduces the need for dynamic memory allocation).
 for performance enhancing purposes.
*/
void signal_init( signal* sig, char* name, vector* value ) {

  sig->name     = name;
  sig->value    = value;
  sig->exp_head = NULL;
  sig->exp_tail = NULL;

}

/*!
 \param name       Full hierarchical name of this signal.
 \param width      Bit width of this signal.
 \param lsb        Bit position of least significant bit.
 \param is_static  Specifies if this signal is a static value.

 \returns Pointer to newly created signal.

 This function should be called by the Verilog parser or the
 database reading function.  It initializes all of the necessary
 values for a signal and returns a pointer to this newly created
 signal.
*/
signal* signal_create( char* name, int width, int lsb, int is_static ) {

  signal* new_sig;     /* Pointer to newly created signal */

  new_sig = (signal*)malloc_safe( sizeof( signal ) );

  signal_init( new_sig, strdup( name ), vector_create( width, lsb ) );

  return( new_sig );

}

/*!
 \param sig      Signal to write to file.
 \param file     Name of file to display signal contents to.
 \param modname  Name of module that this signal belongs to.

 Prints the signal information for the specified signal to the
 specified file.  This file will be the database coverage file
 for this design.
*/
void signal_db_write( signal* sig, FILE* file, char* modname ) {

  exp_link* curr;      /* Pointer to current expression link element */

  /* Display identification and value information first */
  fprintf( file, "%d %s %s ",
    DB_TYPE_SIGNAL,
    sig->name,
    modname
  );

  // printf( "Writing signal:  %d %s %s ", DB_TYPE_SIGNAL, sig->name, modname );

  vector_db_write( sig->value, file, FALSE );

  curr = sig->exp_head;
  while( curr != NULL ) {
    fprintf( file, " %d", expression_get_id( curr->exp ) );
    curr = curr->next;
  }

  fprintf( file, "\n" );

}

/*!
 \param line      Pointer to current line from database file to parse.
 \param curr_mod  Pointer to current module instantiating this signal.

 \return Returns TRUE if signal information read successfully; otherwise,
         returns FALSE.

 \bug 
 A signal will only look in the current module for a matching expression.  In the case
 of a hierarchical reference, it is possible that an expression outside the current module
 is referencing this signal.  We need to check for this case (hierarchical expression)
 and find the expression elsewhere.

 Creates a new signal structure, parses current file line for signal
 information and stores it to the specified signal.  If there are any problems
 in reading in the current line, returns FALSE; otherwise, returns TRUE.
*/
bool signal_db_read( char** line, module* curr_mod ) {

  bool        retval = TRUE;   /* Return value for this function                   */
  char        name[256];       /* Name of current signal                           */
  signal*     sig;             /* Pointer to the newly created signal              */
  vector*     vec;             /* Vector value for this signal                     */
  int         exp_id;          /* Expression ID                                    */
  char        tmp[2];          /* Temporary holder for semicolon                   */
  int         chars_read;      /* Number of characters read from line              */
  char        modname[4096];   /* Name of signal's module                          */
  expression  texp;            /* Temporary expression link for searching purposes */
  exp_link*   expl;            /* Temporary expression link for storage            */
  char        msg[4096];       /* Error message string                             */
  expression* curr_parent;     /* Pointer to current parent being traversed        */

  /* Get name values. */
  if( sscanf( *line, "%s %s %n", name, modname, &chars_read ) == 2 ) {

    *line = *line + chars_read;

    /* Read in vector information */
    if( vector_db_read( &vec, line ) ) {

      /* Create new signal */
      sig = signal_create( name, vec->width, vec->lsb, 0 );

      /* Copy over vector value */
      vector_dealloc( sig->value );
      sig->value = vec;

      /* Add signal to signal list */
      if( curr_mod == NULL ) {
        print_output( "Internal error:  signal in database written before its module", FATAL );
        retval = FALSE;
      } else {
        sig_link_add( sig, &(curr_mod->sig_head), &(curr_mod->sig_tail) );
      }

      /* Read in expressions */
      while( sscanf( *line, "%d%n", &exp_id, &chars_read ) == 1 ) {

        *line = *line + chars_read;

        /* Find expression in current module and add it to signal list */
	texp.id = exp_id;

        if( (expl = exp_link_find( &texp, curr_mod->exp_head )) != NULL ) {

	  exp_link_add( expl->exp, &(sig->exp_head), &(sig->exp_tail) );

	  /* 
	   If expression is a signal holder, we need to set the expression's vector to point
	   to our vector and set its signal pointer to point to us.
	  */
          if( (SUPPL_OP( expl->exp->suppl ) == EXP_OP_SIG      ) ||
              (SUPPL_OP( expl->exp->suppl ) == EXP_OP_SBIT_SEL ) ||
              (SUPPL_OP( expl->exp->suppl ) == EXP_OP_MBIT_SEL ) ) {

            if( SUPPL_OP( expl->exp->suppl ) == EXP_OP_SIG ) {
              expl->exp->value = sig->value;
              expl->exp->sig   = sig;
            } else if( SUPPL_OP( expl->exp->suppl ) == EXP_OP_SBIT_SEL ) {
              expl->exp->value->value = sig->value->value;
              expl->exp->value->width = 1;
              expl->exp->suppl        = expl->exp->suppl | ((sig->value->lsb & 0xffff) << SUPPL_LSB_SIG_LSB);
              expl->exp->sig          = sig;
            } else if( SUPPL_OP( expl->exp->suppl ) == EXP_OP_MBIT_SEL ) {
              expl->exp->value->value = sig->value->value;
              expl->exp->suppl        = expl->exp->suppl | ((sig->value->lsb & 0xffff) << SUPPL_LSB_SIG_LSB);
              expl->exp->sig          = sig;
            }

#ifdef NOT_NEEDED
            /* Traverse parent links, setting its width if it is set to 0. */
            curr_parent = expl->exp->parent->expr;
            while( (curr_parent != NULL) && (curr_parent->value->width == 0) ) {
              expression_create_value( curr_parent, sig->value->width, sig->value->lsb );
              curr_parent = curr_parent->parent->expr;
            }
#endif

          }

        } else {

          snprintf( msg, 4096, "Expression %d not found for signal %s", texp.id, sig->name );
	  print_output( msg, FATAL );
	  retval = FALSE;
          exit( 1 );

        }

      }

    } else {

      retval = FALSE;

    }

  } else {

    retval = FALSE;

  }

  return( retval );

}

/*!
 \param base  Signal to store result of merge into.
 \param line  Pointer to line of CDD file to parse.

 \return Returns TRUE if parsing successful; otherwise, returns FALSE.

 Parses specified line for signal information and performs merge 
 of the base and in signals, placing the resulting merged signal 
 into the base signal.  If the signals are found to be unalike 
 (names are different), an error message is displayed to the user.  
 If both signals are the same, perform the merge on the signal's 
 vectors.
*/
bool signal_db_merge( signal* base, char** line ) {

  bool retval;         /* Return value of this function       */
  char name[256];      /* Name of current signal              */
  char modname[4096];  /* Name of current signal's module     */
  int  chars_read;     /* Number of characters read from line */

  assert( base != NULL );
  assert( base->name != NULL );

  if( sscanf( *line, "%s %s %n", name, modname, &chars_read ) == 2 ) {

    *line = *line + chars_read;

    if( strcmp( base->name, name ) != 0 ) {

      print_output( "Attempting to merge two databases derived from different designs.  Unable to merge", FATAL );
      exit( 1 );

    } else {

      /* Read in vector information */
      retval = vector_db_merge( base->value, line );

    }

  } else {

    retval = FALSE;

  }

  return( retval );

}

/*!
 \param sig    Pointer to signal to assign VCD value to.
 \param value  String version of VCD value.

 Assigns the associated value to the specified signal's vector.  After this, it
 iterates through its expression list, setting the TRUE and FALSE bits accordingly.
 Finally, calls the simulator expr_changed function for each expression.
*/
void signal_vcd_assign( signal* sig, char* value ) {

  exp_link* curr_expr;   /* Pointer to current expression link under evaluation */

  assert( sig->value != NULL );

  /* Assign value to signal's vector value */
  vector_vcd_assign( sig->value, value );

  /* Iterate through signal's expression list */
  curr_expr = sig->exp_head;
  while( curr_expr != NULL ) {

    /* Add to simulation queue */
    sim_expr_changed( curr_expr->exp );

    curr_expr = curr_expr->next;

  }

}

/*!
 \param sig   Pointer to signal to add expression to.
 \param expr  Expression to add to list.

 Adds the specified expression to the end of this signal's expression
 list.
*/
void signal_add_expression( signal* sig, expression* expr ) {

  exp_link_add( expr, &(sig->exp_head), &(sig->exp_tail) );

}

/*!
 \param sig  Pointer to signal to display to standard output.

 Displays signal's name, width, lsb and value vector to the standard output.
*/
void signal_display( signal* sig ) {

  assert( sig != NULL );

  printf( "  Signal =>  name: %s, ", sig->name );
  
  vector_display( sig->value );

}

/*!
 \param sig  Pointer to signal to deallocate.

 Deallocates all malloc'ed memory back to the heap for the specified
 signal.
*/
void signal_dealloc( signal* sig ) {

  exp_link* curr_expl;   /* Pointer to current expression link to set to NULL */

  if( sig != NULL ) {

    if( sig->name != NULL ) {
      free_safe( sig->name );
      sig->name = NULL;
    }

    /* Free up memory for value */
    vector_dealloc( sig->value );
    sig->value = NULL;

    /* Free up memory for expression list */
    curr_expl = sig->exp_head;
    while( curr_expl != NULL ) {
      curr_expl->exp->sig = NULL;
      curr_expl = curr_expl->next;
    }

    exp_link_delete_list( sig->exp_head, FALSE );
    sig->exp_head = NULL;

    /* Finally free up the memory for this signal */
    free_safe( sig );

  }

}

/* $Log$
/* Revision 1.14  2002/08/14 04:52:48  phase1geo
/* Removing unnecessary calls to signal_dealloc function and fixing bug
/* with signal_dealloc function.
/*
/* Revision 1.13  2002/07/23 12:56:22  phase1geo
/* Fixing some memory overflow issues.  Still getting core dumps in some areas.
/*
/* Revision 1.12  2002/07/19 13:10:07  phase1geo
/* Various fixes to binding scheme.
/*
/* Revision 1.11  2002/07/18 22:02:35  phase1geo
/* In the middle of making improvements/fixes to the expression/signal
/* binding phase.
/*
/* Revision 1.10  2002/07/18 02:33:24  phase1geo
/* Fixed instantiation addition.  Multiple hierarchy instantiation trees should
/* now work.
/*
/* Revision 1.9  2002/07/17 06:27:18  phase1geo
/* Added start for fixes to bit select code starting with single bit selection.
/* Full regression passes with addition of sbit_sel1 diagnostic.
/*
/* Revision 1.8  2002/07/08 12:35:31  phase1geo
/* Added initial support for library searching.  Code seems to be broken at the
/* moment.
/*
/* Revision 1.7  2002/07/05 16:49:47  phase1geo
/* Modified a lot of code this go around.  Fixed VCD reader to handle changes in
/* the reverse order (last changes are stored instead of first for timestamp).
/* Fixed problem with AEDGE operator to handle vector value changes correctly.
/* Added casez2.v diagnostic to verify proper handling of casez with '?' characters.
/* Full regression passes; however, the recent changes seem to have impacted
/* performance -- need to look into this.
/*
/* Revision 1.6  2002/07/03 03:31:11  phase1geo
/* Adding RCS Log strings in files that were missing them so that file version
/* information is contained in every source and header file.  Reordering src
/* Makefile to be alphabetical.  Adding mult1.v diagnostic to regression suite.
/* */
