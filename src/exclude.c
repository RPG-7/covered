/*
 Copyright (c) 2006 Trevor Williams

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program;
 if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*!
 \file     exclude.c
 \author   Trevor Williams  (phase1geo@gmail.com)
 \date     6/22/2006
*/

#include "arc.h"
#include "assertion.h"
#include "comb.h"
#include "defines.h"
#include "exclude.h"
#include "expr.h"
#include "fsm.h"
#include "func_iter.h"
#include "instance.h"
#include "line.h"
#include "link.h"
#include "memory.h"
#include "ovl.h"
#include "profiler.h"
#include "toggle.h"
#include "vector.h"


extern db**         db_list;
extern unsigned int curr_db;
extern isuppl       info_suppl;
extern char         user_msg[USER_MSG_LENGTH];

/*!
 Name of CDD file that will be read, modified with exclusion modifications and written back.
*/
static char* exclude_cdd = NULL;

/*!
 Pointer to the head of the list of exclusion IDs to toggle exclusion/inclusion mode of.
*/
static str_link* excl_ids_head = NULL;

/*!
 Pointer to the tail of the list of exclusion IDs to toggle exclusion/inclusion mode of.
*/
static str_link* excl_ids_tail = NULL;

/*!
 If set to TRUE, causes a message prompt to be displayed for each coverage point that will
 be excluded from coverage.
*/
static bool exclude_prompt_for_msgs = FALSE;


/*!
 \return Returns TRUE if a parent expression of this expression was found to be excluded from
         coverage; otherwise, returns FALSE.
*/
static bool exclude_is_parent_excluded(
    expression* expr  /*!< Pointer to current expression to evaluate */
) {

  return( (expr != NULL) &&
          ((ESUPPL_EXCLUDED( expr->suppl ) == 1) ||
           ((ESUPPL_IS_ROOT( expr->suppl ) == 0) && exclude_is_parent_excluded( expr->parent->expr ))) );

}

/*!
 Sets the specified expression's exclude bit to the given value and recalculates all
 affected coverage information for this instance.
*/
static void exclude_expr_assign_and_recalc(
            expression* expr,      /*!< Pointer to expression that is being excluded/included */
            func_unit*  funit,     /*!< Pointer to functional unit containing this expression */
            bool        excluded,  /*!< Specifies if expression is being excluded or included */
            bool        set_line,  /*!< Set to TRUE when this function is being called for line exclusion */
  /*@out@*/ statistic*  stat       /*!< Pointer to statistics structure to update */
) { PROFILE(EXCLUDE_EXPR_ASSIGN_AND_RECALC);

  unsigned int comb_hit      = 0;  /* Total number of hit combinations within this tree */
  unsigned int comb_excluded = 0;  /* Total number of excluded combinations */
  unsigned int comb_total    = 0;  /* Total number of combinational logic coverage points within this tree */
  int          ulid          = 0;  /* Temporary value */

  /* Now recalculate the coverage information for all metrics if this module is not an OVL module */
  if( (info_suppl.part.assert_ovl == 0) || !ovl_is_assertion_module( funit ) ) {

    /* If this expression is a root expression, recalculate line coverage */
    if( ESUPPL_IS_ROOT( expr->suppl ) == 1 ) {
      if( (expr->op != EXP_OP_DELAY)   &&
          (expr->op != EXP_OP_CASE)    &&
          (expr->op != EXP_OP_CASEX)   &&
          (expr->op != EXP_OP_CASEZ)   &&
          (expr->op != EXP_OP_DEFAULT) &&
          (expr->op != EXP_OP_NB_CALL) &&
          (expr->op != EXP_OP_FORK)    &&
          (expr->op != EXP_OP_JOIN)    &&
          (expr->op != EXP_OP_NOOP)    &&
          (expr->line != 0) &&
          (expr->exec_num == 0) ) {
        if( excluded ) {
          stat->line_hit++;
          stat->line_excluded++;
        } else {
          stat->line_hit--;
          stat->line_excluded--;
        }
      }
    }

    /* Always recalculate combinational coverage */
    combination_reset_counted_expr_tree( expr );
    if( excluded ) {
      combination_get_tree_stats( expr, &ulid, 0, exclude_is_parent_excluded( expr ), &comb_hit, &comb_excluded, &comb_total );
      stat->comb_hit      += (comb_total - comb_hit);
      stat->comb_excluded += (comb_total - comb_excluded);
    } else {
      expr->suppl.part.excluded = 0;
      combination_get_tree_stats( expr, &ulid, 0, exclude_is_parent_excluded( expr ), &comb_hit, &comb_excluded, &comb_total );
      stat->comb_hit      -= (comb_total - comb_hit);
      stat->comb_excluded -= (comb_total - comb_excluded);
    }

  } else {

    /* If the expression is a coverage point, recalculate the assertion coverage */
    if( ovl_is_assertion_module( funit ) && ovl_is_coverage_point( expr ) ) {
      if( expr->exec_num == 0 ) {
        if( excluded ) {
          stat->assert_hit++;
          stat->assert_excluded++;
        } else {
          stat->assert_hit--;
          stat->assert_excluded--;
        }
      }
    }

  }

  /* Set the exclude bits in the expression supplemental field */
  expr->suppl.part.excluded = excluded ? 1 : 0;
  if( (ESUPPL_IS_ROOT( expr->suppl ) == 1) && (expr->parent->stmt != NULL) ) {
    expr->parent->stmt->suppl.part.excluded = (excluded && set_line) ? 1 : 0;
  }

  PROFILE_END;

}

/*!
 Sets the specified signal's exclude bit to the given value and recalculates all
 affected coverage information for this instance.
*/
static void exclude_sig_assign_and_recalc(
            vsignal*   sig,       /*!< Pointer to signal that is being excluded/included */
            bool       excluded,  /*!< Specifies if signal is being excluded or included */
  /*@out@*/ statistic* stat       /*!< Pointer to statistic structure to update */
) { PROFILE(EXCLUDE_SIG_ASSIGN_AND_RECALC);

  /* First, set the exclude bit in the signal supplemental field */
  sig->suppl.part.excluded = excluded ? 1 : 0;

  /* If the signal is a memory, we need to update the memory coverage numbers */
  if( sig->suppl.part.type == SSUPPL_TYPE_MEM ) {

    unsigned int wr_hit       = 0;  /* Number of addressable elements written */
    unsigned int rd_hit       = 0;  /* Number of addressable elements read */
    unsigned int ae_total     = 0;  /* Number of addressable elements in this memory */
    unsigned int tog01_hit    = 0;  /* Number of bits toggling from 0->1 */
    unsigned int tog10_hit    = 0;  /* Number of bits toggling from 1->0 */
    unsigned int tog_total    = 0;  /* Total number of toggle bits */
    unsigned int mem_excluded = 0;  /* Number of excluded memory coverage points */

    /* Get the stats for the current memory */
    memory_get_stat( sig, &wr_hit, &rd_hit, &ae_total, &tog01_hit, &tog10_hit, &tog_total, &mem_excluded, TRUE );

    /* Recalculate the total and hit values for memory coverage */
    if( excluded ) {
      stat->mem_wr_hit    += (ae_total  - wr_hit);
      stat->mem_rd_hit    += (ae_total  - rd_hit);
      stat->mem_tog01_hit += (tog_total - tog01_hit);
      stat->mem_tog10_hit += (tog_total - tog10_hit);
      stat->mem_excluded  += ((ae_total * 2) + (tog_total * 2));
    } else {
      stat->mem_wr_hit    -= (ae_total  - wr_hit);
      stat->mem_rd_hit    -= (ae_total  - rd_hit);
      stat->mem_tog01_hit -= (tog_total - tog01_hit);
      stat->mem_tog10_hit -= (tog_total - tog10_hit);
      stat->mem_excluded  -= ((ae_total * 2) + (tog_total * 2));
    }

  /* Otherwise, the toggle coverage numbers should be adjusted */
  } else {

    unsigned int hit01 = 0;  /* Number of bits transitioning from 0 -> 1 */
    unsigned int hit10 = 0;  /* Number of bits transitioning from 1 -> 0 */

    /* Get the total hit01 and hit10 information */
    vector_toggle_count( sig->value, &hit01, &hit10 );

    /* Recalculate the total and hit values for toggle coverage */
    if( excluded ) {
      stat->tog01_hit    += (sig->value->width - hit01);
      stat->tog10_hit    += (sig->value->width - hit10);
      stat->tog_excluded += (sig->value->width * 2);
    } else {
      stat->tog01_hit    -= (sig->value->width - hit01);
      stat->tog10_hit    -= (sig->value->width - hit10);
      stat->tog_excluded -= (sig->value->width * 2);
    }

  }

  PROFILE_END;

}

/*!
 Sets the specified arc entry's exclude bit to the given value and recalculates all
 affected coverage information for this instance.
*/
static void exclude_arc_assign_and_recalc(
            fsm_table* table,      /*!< Pointer FSM table */
            int        arc_index,  /*!< Specifies the index of the entry containing the transition */
            bool       exclude,    /*!< Specifies if we are excluding or including coverage */
  /*@out@*/ statistic* stat        /*!< Pointer to statistic structure to update */
) { PROFILE(EXCLUDE_ARC_ASSIGN_AND_RECALC);

  /* Set the excluded bit in the specified entry and adjust coverage numbers, if necessary */
  table->arcs[arc_index]->suppl.part.excluded = (exclude ? 1 : 0);
  if( table->arcs[arc_index]->suppl.part.hit == 0 ) {
    stat->arc_hit      += exclude ? 1 : -1;
    stat->arc_excluded += exclude ? 1 : -1;
  }

  PROFILE_END;

}

/*!
 \param funit_name  Name of functional unit to search for
 \param funit_type  Type of functional unit to search for

 \return Returns a pointer to the functional unit instance that points to the functional unit
         described in the parameter list if one is found; otherwise, returns NULL.

 Using the specified functional unit information, returns the functional unit instance that
 corresponds to this description.  If one could not be found, a value of NULL is returned.
*/
static funit_inst* exclude_find_instance_from_funit_info(
  const char* funit_name,
  int         funit_type
) {

  funit_link* funitl;         /* Found functional unit link */
  int         ignore = 0;     /* Number of functional unit instances to ignore in search */
  funit_inst* inst   = NULL;  /* Found functional unit instance */

  if( (funitl = funit_link_find( funit_name, funit_type, db_list[curr_db]->funit_head )) != NULL ) {
    inst = inst_link_find_by_funit( funitl->funit, db_list[curr_db]->inst_head, &ignore );
  }

  return( inst );

}

/*!
 \return Returns TRUE if the specified line is excluded in the given functional unit; otherwise, returns FALSE.
*/
bool exclude_is_line_excluded(
  func_unit* funit,  /*!< Pointer to functional unit to check */
  int        line    /*!< Line number of line to check */
) { PROFILE(EXCLUDE_IS_LINE_EXCLUDED);

  func_iter  fi;    /* Functional unit iterator */
  statement* stmt;  /* Pointer to current statement */

  func_iter_init( &fi, funit, TRUE, FALSE );
  while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->line != line) );
  func_iter_dealloc( &fi );

  PROFILE_END;

  return( (stmt == NULL) || (stmt->suppl.part.excluded == 1) );

}

/*!
 Finds the expression(s) and functional unit instance for the given name, type and line number and calls
 the exclude_expr_assign_and_recalc function for each matching expression, setting the excluded bit
 of the expression and recalculating the summary coverage information.
*/
void exclude_set_line_exclude(
            func_unit* funit,  /*!< Pointer to functional unit */
            int        line,   /*!< Line number of expression that needs to be set */
            int        value,  /*!< Specifies if we should exclude (1) or include (0) the specified line */
  /*@out@*/ statistic* stat    /*!< Pointer to statistics structure to update */
) { PROFILE(EXCLUDE_SET_LINE_EXCLUDE);

  func_iter  fi;    /* Functional unit iterator */
  statement* stmt;  /* Pointer to current statement */

  func_iter_init( &fi, funit, TRUE, FALSE );

  do {
    while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->line != line) );
    if( stmt != NULL ) {
      exclude_expr_assign_and_recalc( stmt->exp, funit, (value == 1), TRUE, stat );
    }
  } while( stmt != NULL );

  func_iter_dealloc( &fi );

  PROFILE_END;

}

/*!
 \return Returns TRUE if the specified signal is excluded from coverage consideration; otherwise, returns FALSE.
*/
bool exclude_is_toggle_excluded(
  func_unit* funit,    /*!< Pointer to functional unit to check */
  char*      sig_name  /*!< Name of signal to search for */
) { PROFILE(EXCLUDE_IS_TOGGLE_EXCLUDED);

  func_iter fi;   /* Functional unit iterator */
  vsignal*  sig;  /* Pointer to current signal */

  func_iter_init( &fi, funit, FALSE, TRUE );
  while( ((sig = func_iter_get_next_signal( &fi )) != NULL) && (strcmp( sig->name, sig_name ) != 0) );
  func_iter_dealloc( &fi );

  PROFILE_END;

  return( (sig == NULL) || (sig->suppl.part.excluded == 1) );

}

/*!
 Finds the signal and functional unit instance for the given name, type and sig_name and calls
 the exclude_sig_assign_and_recalc function for the matching signal, setting the excluded bit
 of the signal and recalculating the summary coverage information.
*/
void exclude_set_toggle_exclude(
            func_unit*  funit,     /*!< Pointer to functional unit */
            const char* sig_name,  /*!< Name of signal to set the toggle exclusion for */
            int         value,     /*!< Specifies if we should exclude (1) or include (0) the specified line */
  /*@out@*/ statistic*  stat       /*!< Pointer to statistics structure to update */
) { PROFILE(EXCLUDE_SET_TOGGLE_EXCLUDE);

  func_iter fi;   /* Functional unit iterator */
  vsignal*  sig;  /* Pointer to current signal */

  /* Find the signal that matches the given signal name, if it exists */
  func_iter_init( &fi, funit, FALSE, TRUE );
  while( ((sig = func_iter_get_next_signal( &fi )) != NULL) && (strcmp( sig->name, sig_name ) != 0) );
  func_iter_dealloc( &fi );

  /* Set its exclude bit if it exists */
  if( sig != NULL ) {
    exclude_sig_assign_and_recalc( sig, (value == 1), stat );
  }
      
  PROFILE_END;

}

/*!
 \return Returns TRUE if specified underlined expression is excluded from coverage; otherwise, returns FALSE.
*/
bool exclude_is_comb_excluded(
  func_unit* funit,
  int        expr_id,
  int        uline_id
) { PROFILE(EXCLUDE_IS_COMB_EXCLUDED);

  func_iter   fi;      /* Functional unit iterator */
  statement*  stmt;    /* Pointer to found statement */
  expression* subexp;  /* Pointer to found expression */

  /* Find the matching root expression */
  func_iter_init( &fi, funit, TRUE, FALSE );
  while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->id != expr_id) );
  func_iter_dealloc( &fi );

  if( stmt != NULL ) {
    subexp = expression_find_uline_id( stmt->exp, uline_id );
  }

  PROFILE_END;

  return( (stmt == NULL) || (subexp == NULL) || (subexp->suppl.part.excluded == 1) );

}

/*!
 Finds the expression and functional unit instance for the given name, type and sig_name and calls
 the exclude_expr_assign_and_recalc function for the matching expression, setting the excluded bit
 of the expression and recalculating the summary coverage information.
*/
void exclude_set_comb_exclude(
            func_unit* funit,     /*!< Pointer to functional unit */
            int        expr_id,   /*!< Expression ID of root expression to set exclude value for */
            int        uline_id,  /*!< Underline ID of expression to set exclude value for */
            int        value,     /*!< Specifies if we should exclude (1) or include (0) the specified line */
  /*@out@*/ statistic* stat       /*!< Pointer to statistic structure to update */
) { PROFILE(EXCLUDE_SET_COMB_EXCLUDE);

  func_iter  fi;    /* Functional unit iterator */
  statement* stmt;  /* Pointer to current statement */

  /* Find the root expression */
  func_iter_init( &fi, funit, TRUE, FALSE );
  while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->id != expr_id) );
  func_iter_dealloc( &fi );

  if( stmt != NULL ) {
    expression* subexp;
    if( (subexp = expression_find_uline_id( stmt->exp, uline_id )) != NULL ) {
      exclude_expr_assign_and_recalc( subexp, funit, (value == 1), FALSE, stat );
    }
  }

  PROFILE_END;

}

/*!
 \return Returns TRUE if the given FSM state transition was excluded from coverage; otherwise, returns FALSE.
*/
bool exclude_is_fsm_excluded(
  func_unit* funit,
  int        expr_id,
  char*      from_state,
  char*      to_state
) { PROFILE(EXCLUDE_IS_FSM_EXCLUDED);

  fsm_link* curr_fsm;     /* Pointer to current FSM structure */
  int       found_index;  /* Index of found state transition */

  /* Find the corresponding table */
  curr_fsm = funit->fsm_head;
  while( (curr_fsm != NULL) && (curr_fsm->table->to_state->id != expr_id) ) {
    curr_fsm = curr_fsm->next;
  }

  if( curr_fsm != NULL ) {

    vector* from_vec;
    vector* to_vec;
    int     from_base, to_base;

    /* Convert from/to state strings into vector values */
    vector_from_string( &from_state, FALSE, &from_vec, &from_base );
    vector_from_string( &to_state, FALSE, &to_vec, &to_base );

    /* Find the arc entry and perform the exclusion assignment and coverage recalculation */
    found_index = arc_find_arc( curr_fsm->table->table, arc_find_from_state( curr_fsm->table->table, from_vec ), arc_find_to_state( curr_fsm->table->table, to_vec ) );

    /* Deallocate vectors */
    vector_dealloc( from_vec );
    vector_dealloc( to_vec );

  }

  PROFILE_END;

  return( (curr_fsm == NULL) || (found_index == -1) || (curr_fsm->table->table->arcs[found_index]->suppl.part.excluded == 1) );

}

/*!
 Finds the FSM table associated with the specified expr_id and sets the include/exclude status of the given
 from_state/to_state transition appropriately.
*/
void exclude_set_fsm_exclude(
            func_unit* funit,       /*!< Pointer to functional unit */
            int        expr_id,     /*!< Expression ID of output state variable */
            char*      from_state,  /*!< String containing input state value */
            char*      to_state,    /*!< String containing output state value */
            int        value,       /*!< Specifies if we should exclude (1) or include (0) the specified line */
  /*@out@*/ statistic* stat         /*!< Pointer to statistics structure to update */
) { PROFILE(EXCLUDE_SET_FSM_EXCLUDE);

  fsm_link* curr_fsm;

  /* Find the corresponding table */
  curr_fsm = funit->fsm_head;
  while( (curr_fsm != NULL) && (curr_fsm->table->to_state->id != expr_id) ) {
    curr_fsm = curr_fsm->next;
  }

  if( curr_fsm != NULL ) {

    vector* from_vec;
    vector* to_vec;
    int     found_index;
    int     from_base, to_base;

    /* Convert from/to state strings into vector values */
    vector_from_string( &from_state, FALSE, &from_vec, &from_base );
    vector_from_string( &to_state, FALSE, &to_vec, &to_base );

    /* Find the arc entry and perform the exclusion assignment and coverage recalculation */
    if( (found_index = arc_find_arc( curr_fsm->table->table, arc_find_from_state( curr_fsm->table->table, from_vec ), arc_find_to_state( curr_fsm->table->table, to_vec ) )) != -1 ) {
      exclude_arc_assign_and_recalc( curr_fsm->table->table, found_index, (value == 1), stat );
    }

    /* Deallocate vectors */
    vector_dealloc( from_vec );
    vector_dealloc( to_vec );

  }

  PROFILE_END;

}

/*!
 \return Returns TRUE if the given assertion is excluded from coverage consideration; otherwise, returns FALSE.
*/
bool exclude_is_assert_excluded(
  func_unit* funit,
  char*      inst_name, 
  int        expr_id
) { PROFILE(EXCLUDE_IS_ASSERT_EXCLUDED);

  funit_inst* inst;        /* Pointer to found functional unit instance */
  funit_inst* curr_child;  /* Pointer to current child functional instance */
  exp_link*   expl;        /* Pointer to current expression link */
  int         ignore = 0;  /* Number of instances to ignore */
  statement*  stmt;        /* Pointer to current statement */

  /* Find the functional unit instance that matches the description */
  if( (inst = inst_link_find_by_funit( funit, db_list[curr_db]->inst_head, &ignore )) != NULL ) {

    func_iter fi;

    /* Find child instance */
    curr_child = inst->child_head;
    while( (curr_child != NULL) && (strcmp( curr_child->name, inst_name ) != 0) ) {
      curr_child = curr_child->next;
    }
    assert( curr_child != NULL );

    /* Initialize the functional unit iterator */
    func_iter_init( &fi, curr_child->funit, TRUE, FALSE );

    while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->id != expr_id) );

    /* Deallocate functional unit statement iterator */
    func_iter_dealloc( &fi );

  }

  PROFILE_END;

  return( (inst == NULL) || (stmt == NULL) || (stmt->exp->id != expr_id) || (stmt->exp->suppl.part.excluded == 1) );

}

/*!
 Finds the expression and functional unit instance for the given name, type and sig_name and calls
 the exclude_expr_assign_and_recalc function for the matching expression, setting the excluded bit
 of the expression and recalculating the summary coverage information.
*/
void exclude_set_assert_exclude(
            func_unit* funit,      /*!< Pointer to functional unit */
            char*      inst_name,  /*!< Name of child instance to find in given functional unit */
            int        expr_id,    /*!< Expression ID of expression to set exclude value for */
            int        value,      /*!< Specifies if we should exclude (1) or include (0) the specified line */
  /*@out@*/ statistic* stat        /*!< Pointer to statistic structure to update */
) { PROFILE(EXCLUDE_SET_ASSERT_EXCLUDE);

  funit_inst* inst;        /* Pointer to found functional unit instance */
  funit_inst* curr_child;  /* Pointer to current child functional instance */
  exp_link*   expl;        /* Pointer to current expression link */
  int         ignore = 0;  /* Number of instances to ignore */

  /* Find the functional unit instance that matches the description */
  if( (inst = inst_link_find_by_funit( funit, db_list[curr_db]->inst_head, &ignore )) != NULL ) {
   
    func_iter  fi;
    statement* stmt;

    /* Find child instance */
    curr_child = inst->child_head;
    while( (curr_child != NULL) && (strcmp( curr_child->name, inst_name ) != 0) ) {
      curr_child = curr_child->next;
    }
    assert( curr_child != NULL );

    /* Initialize the functional unit iterator */
    func_iter_init( &fi, curr_child->funit, TRUE, FALSE );

    while( ((stmt = func_iter_get_next_statement( &fi )) != NULL) && (stmt->exp->id != expr_id) );

    /* Find the signal that matches the given signal name and sets its excluded bit */
    if( stmt->exp->id == expr_id ) {
      exclude_expr_assign_and_recalc( stmt->exp, curr_child->funit, (value == 1), FALSE, stat );
    }

    /* Deallocate functional unit statement iterator */
    func_iter_dealloc( &fi );

  }

  PROFILE_END;

}

/********************************************************************************************/

/*!
 Outputs usage information to standard output for exclude command.
*/
static void exclude_usage() {

  printf( "\n" );
  printf( "Usage:  covered exclude (-h | ([<options>] <exclusion_ids>+ <database_file>)\n" );
  printf( "\n" );
  printf( "   -h                           Displays this help information.\n" );
  printf( "\n" );
  printf( "   Options:\n" );
  printf( "      -f <filename>             Name of file containing additional arguments to parse.\n" );
  printf( "      -m                        Allows a message to be associated with an exclusion.\n" );
  printf( "                                  The message should describe the reason why a coverage point\n" );
  printf( "                                  is being excluded.  If a coverage point is being included for\n" );
  printf( "                                  coverage (i.e., it was previously excluded from coverage), no\n" );
  printf( "                                  message prompt will be specified.\n" );
  printf( "\n" );

}

/*!
 \throws anonymous Throw Throw Throw

 Parses the exclude argument list, placing all parsed values into
 global variables.  If an argument is found that is not valid
 for the score operation, an error message is displayed to the
 user.
*/
static void exclude_parse_args(
  int          argc,      /*!< Number of arguments in argument list argv */
  int          last_arg,  /*!< Index of last parsed argument from list */
  const char** argv       /*!< Argument list passed to this program */
) {

  int i;

  i = last_arg + 1;

  while( i < argc ) {

    if( strncmp( "-h", argv[i], 2 ) == 0 ) {

      exclude_usage();
      Throw 0;

    } else if( strncmp( "-f", argv[i], 2 ) == 0 ) {

      if( check_option_value( argc, argv, i ) ) {
        char**       arg_list = NULL;
        int          arg_num  = 0;
        unsigned int j;
        i++;
        Try {
          read_command_file( argv[i], &arg_list, &arg_num );
          exclude_parse_args( arg_num, -1, (const char**)arg_list );
        } Catch_anonymous {
          for( j=0; j<arg_num; j++ ) {
            free_safe( arg_list[j], (strlen( arg_list[j] ) + 1) );
          }
          free_safe( arg_list, (sizeof( char* ) * arg_num) );
          Throw 0;
        }
        for( j=0; j<arg_num; j++ ) {
          free_safe( arg_list[j], (strlen( arg_list[j] ) + 1) );
        }
        free_safe( arg_list, (sizeof( char* ) * arg_num) );
      } else {
        Throw 0;
      }

    } else if( strncmp( "-m", argv[i], 2 ) == 0 ) {

      exclude_prompt_for_msgs = TRUE;

    } else if( strncmp( "-", argv[i], 1 ) == 0 ) {

      unsigned int rv = snprintf( user_msg, USER_MSG_LENGTH, "Unknown exclude option (%s) specified.", argv[i] );
      assert( rv < USER_MSG_LENGTH );
      print_output( user_msg, FATAL, __FILE__, __LINE__ );
      Throw 0;

    } else if( (i + 1) == argc ) {

      /* Check to make sure that the user has specified at least one exclusion ID */
      if( excl_ids_head == NULL ) {
        print_output( "At least one exclusion ID must be specified", FATAL, __FILE__, __LINE__ );
        Throw 0;
      }

      if( file_exists( argv[i] ) ) {
        exclude_cdd = strdup_safe( argv[i] );
      } else {
        unsigned int rv = snprintf( user_msg, USER_MSG_LENGTH, "Specified CDD file (%s) does not exist", argv[i] );
        assert( rv < USER_MSG_LENGTH );
        print_output( user_msg, FATAL, __FILE__, __LINE__ );
        Throw 0;
      }

    } else {

      str_link_add( strdup_safe( argv[i] ), &excl_ids_head, &excl_ids_tail );

    }

    i++;

  }

}

/*!
 \return Returns pointer to found signal if it was found; otherwise, returns NULL.
*/
vsignal* exclude_find_signal(
  int id  /*!< Exclusion ID to search for */
) { PROFILE(EXCLUDE_FIND_SIGNAL);

  inst_link* instl;  /* Pointer to current instance link */
  vsignal*   sig;    /* Pointer to found signal */

  instl = db_list[curr_db]->inst_head;
  while( (instl != NULL) && ((sig = instance_find_signal_by_exclusion_id( instl->inst, id )) == NULL) ) {
    instl = instl->next;
  }

  PROFILE_END;

  return( sig );

}

/*!
 \return Returns pointer to found expression if it was found; otherwise, returns NULL.
*/
expression* exclude_find_expression(
  int id  /*!< Exclusion ID to search for */
) { PROFILE(EXCLUDE_FIND_EXPRESSION);

  inst_link*  instl;  /* Pointer to current instance link */
  expression* exp;    /* Pointer to found expression */

  instl = db_list[curr_db]->inst_head;
  while( (instl != NULL) && ((exp = instance_find_expression_by_exclusion_id( instl->inst, id )) == NULL) ) {
    instl = instl->next;
  }

  PROFILE_END;

  return( exp );

}

/*!
 \return Returns the message specified by the user.
*/
char* exclude_get_message(
  char etype,  /*!< Exclusion ID type */
  char eid     /*!< Exclusion numerical ID */
) { PROFILE(EXCLUDED_GET_MESSAGE);

  char* msg          = NULL;  /* Pointer to the message from the user */
  int   msg_size     = 0;     /* The current size of the specified message */
  char  c;                    /* Current character */
  bool  nl_just_seen = TRUE;  /* Set to TRUE if a newline was just seen */
  int   index        = 0;     /* Current string index */
  char  str[101];

  printf( "Please specify a reason for exclusion for exclusion ID %c%d (Enter . (period) on a newline to end):\n", etype, eid );

  str[0] = '\0';

  while( ((c = (char)getchar()) != '.') || !nl_just_seen ) {
    if( c == '\n' ) {
      nl_just_seen = TRUE;
    } else {
      nl_just_seen = FALSE;
    }
    str[(index % 100) + 0] = c;
    str[(index % 100) + 1] = '\0';
    if( ((index + 1) % 100) == 0 ) {
      msg       = (char*)realloc_safe( msg, msg_size, (msg_size + 100) );
      msg_size += 100;
      strcat( msg, str );
    }
    index++;
  }

  if( strlen( str ) > 0 ) {
    msg = (char*)realloc_safe( msg, msg_size, (msg_size + strlen( str ) + 1) );
    strcat( msg, str );
  }

  printf( "\n" );

  PROFILE_END;

  return( msg );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the line that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_line_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies a line coverage point */
) { PROFILE(EXCLUDE_LINE_FROM_ID);

  expression* exp;  /* Pointer to found expression */

  if( (exp = exclude_find_expression( id )) != NULL ) {

    int prev_excluded;

    assert( ESUPPL_IS_ROOT( exp->suppl ) == 1 );

    /* Get the previously excluded value */
    prev_excluded = exp->parent->stmt->suppl.part.excluded;

    /* Set the exclude bits in the expression supplemental field */
    exp->suppl.part.excluded               = (prev_excluded ^ 1);
    exp->parent->stmt->suppl.part.excluded = (prev_excluded ^ 1);

    /* If we are excluding and the -m option was specified, get an exclusion reason from the user */
    if( (prev_excluded == 0) && exclude_prompt_for_msgs ) {
      char* str = exclude_get_message( 'L', id );
      /* TBD - What to do with the exclusion message? */
      free_safe( str, (strlen( str ) + 1) );
    }

  }

  PROFILE_END;

  return( exp != NULL );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the signal that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_toggle_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies a toggle coverage point */
) { PROFILE(EXCLUDE_TOGGLE_FROM_ID);

  bool retval = FALSE;  /* Return value for this function */

  PROFILE_END;

  return( retval );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the memory that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_memory_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies a memory coverage point */
) { PROFILE(EXCLUDE_MEMORY_FROM_ID);

  bool retval = FALSE;  /* Return value for this function */

  PROFILE_END;

  return( retval );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the expression that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_expr_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies a combinational logic coverage point */
) { PROFILE(EXCLUDE_EXPR_FROM_ID);

  bool retval = FALSE;  /* Return value for this function */

  PROFILE_END;

  return( retval );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the FSM that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_fsm_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies a FSM coverage point */
) { PROFILE(EXCLUDE_FSM_FROM_ID);

  bool retval = FALSE;  /* Return value for this function */

  PROFILE_END;

  return( retval );

}

/*!
 \return Returns TRUE if the exclusion ID was found and the exclusion applied; otherwise, returns FALSE.

 Finds the assertion that matches the given exclusion ID and toggles its exclusion value, providing a reason
 for exclusion if it is excluding the coverage point and the -m option was specified on the command-line.
*/
bool exclude_assert_from_id(
  int id  /*!< Numerical portion of the exclusion ID that identifies an assertion coverage point */
) { PROFILE(EXCLUDE_ASSERT_FROM_ID);

  bool retval = FALSE;  /* Return value for this function */

  PROFILE_END;

  return( retval );

}

/*!
 \return Returns TRUE if one or more exclusion IDs were applied; otherwise, returns FALSE.

 Applies the user-specified exclusion IDs to the currently opened database.
*/
bool exclude_apply_exclusions() { PROFILE(EXCLUDE_APPLY_EXCLUSIONS);

  bool      retval = FALSE;  /* Return value for this function */
  str_link* strl;            /* Pointer to current string link */

  strl = excl_ids_head;
  while( strl != NULL ) {
    switch( strl->str[0] ) {
      case 'L' :  retval |= exclude_line_from_id( atoi( strl->str + 1 ) );    break;
      case 'T' :  retval |= exclude_toggle_from_id( atoi( strl->str + 1 ) );  break;
      case 'M' :  retval |= exclude_memory_from_id( atoi( strl->str + 1 ) );  break;
      case 'E' :  retval |= exclude_expr_from_id( atoi( strl->str + 1 ) );    break;
      case 'F' :  retval |= exclude_fsm_from_id( atoi( strl->str + 1 ) );     break;
      case 'A' :  retval |= exclude_assert_from_id( atoi( strl->str + 1 ) );  break;
      default  :
        snprintf( user_msg, USER_MSG_LENGTH, "Illegal exclusion identifier specified (%s)", strl->str );
        print_output( user_msg, FATAL, __FILE__, __LINE__ );
        Throw 0;
        break;
    }
    strl = strl->next;
  }

  PROFILE_END;

  return( retval );

}

/*!
 Performs the exclude command.
*/
void command_exclude(
  int          argc,      /*!< Number of arguments in command-line to parse */
  int          last_arg,  /*!< Index of last parsed argument from list */
  const char** argv       /*!< List of arguments from command-line to parse */
) { PROFILE(COMMAND_EXCLUDE);

  unsigned int   rv;
  comp_cdd_cov** comp_cdds    = NULL;
  unsigned int   comp_cdd_num = 0;

  /* Output header information */
  rv = snprintf( user_msg, USER_MSG_LENGTH, COVERED_HEADER );
  assert( rv < USER_MSG_LENGTH );
  print_output( user_msg, NORMAL, __FILE__, __LINE__ );

  Try {

    unsigned int rv;

    /* Parse score command-line */
    exclude_parse_args( argc, last_arg, argv );

    /* Read in database */
    rv = snprintf( user_msg, USER_MSG_LENGTH, "Reading CDD file \"%s\"", exclude_cdd );
    assert( rv < USER_MSG_LENGTH );
    print_output( user_msg, NORMAL, __FILE__, __LINE__ );

    db_read( exclude_cdd, READ_MODE_REPORT_NO_MERGE );
    bind_perform( TRUE, 0 );

    /* Apply the specified exclusion IDs */
    if( exclude_apply_exclusions() ) {
      db_write( exclude_cdd, FALSE, TRUE );
    }

  } Catch_anonymous {}

  /* Close down the database */
  db_close();
    
  /* Deallocate other allocated variables */
  str_link_delete_list( excl_ids_head );
  free_safe( exclude_cdd, (strlen( exclude_cdd ) + 1) );
      
  PROFILE_END;
 
}   


/*
 $Log$
 Revision 1.29  2008/08/23 20:00:29  phase1geo
 Full fix for bug 2054686.  Also cleaned up Cver regressions.

 Revision 1.28  2008/08/22 20:56:35  phase1geo
 Starting to make updates for proper unnamed scope report handling (fix for bug 2054686).
 Not complete yet.  Also making updates to documentation.  Checkpointing.

 Revision 1.27  2008/08/18 23:07:26  phase1geo
 Integrating changes from development release branch to main development trunk.
 Regression passes.  Still need to update documentation directories and verify
 that the GUI stuff works properly.

 Revision 1.24.2.6  2008/08/07 23:22:49  phase1geo
 Added initial code to synchronize module and instance exclusion information.  Checkpointing.

 Revision 1.24.2.5  2008/08/07 20:51:04  phase1geo
 Fixing memory allocation/deallocation issues with GUI.  Also fixing some issues with FSM
 table output and exclusion.  Checkpointing.

 Revision 1.24.2.4  2008/08/07 18:03:51  phase1geo
 Fixing instance exclusion segfault issue with GUI.  Also cleaned up function
 documentation in link.c.

 Revision 1.24.2.3  2008/08/07 06:39:10  phase1geo
 Adding "Excluded" column to the summary listbox.

 Revision 1.24.2.2  2008/08/06 20:11:33  phase1geo
 Adding support for instance-based coverage reporting in GUI.  Everything seems to be
 working except for proper exclusion handling.  Checkpointing.

 Revision 1.24.2.1  2008/07/10 22:43:50  phase1geo
 Merging in rank-devel-branch into this branch.  Added -f options for all commands
 to allow files containing command-line arguments to be added.  A few error diagnostics
 are currently failing due to changes in the rank branch that never got fixed in that
 branch.  Checkpointing.

 Revision 1.25  2008/06/27 14:02:00  phase1geo
 Fixing splint and -Wextra warnings.  Also fixing comment formatting.

 Revision 1.24  2008/05/30 05:38:30  phase1geo
 Updating development tree with development branch.  Also attempting to fix
 bug 1965927.

 Revision 1.23.2.3  2008/05/08 23:12:41  phase1geo
 Fixing several bugs and reworking code in arc to get FSM diagnostics
 to pass.  Checkpointing.

 Revision 1.23.2.2  2008/05/08 03:56:38  phase1geo
 Updating regression files and reworking arc_find and arc_add functionality.
 Checkpointing.

 Revision 1.23.2.1  2008/05/02 22:06:10  phase1geo
 Updating arc code for new data structure.  This code is completely untested
 but does compile and has been completely rewritten.  Checkpointing.

 Revision 1.23  2008/04/15 20:37:07  phase1geo
 Fixing database array support.  Full regression passes.

 Revision 1.22  2008/04/14 23:10:14  phase1geo
 More GUI updates and a fix to the line exclusion code.

 Revision 1.21  2008/03/26 21:29:31  phase1geo
 Initial checkin of new optimizations for unknown and not_zero values in vectors.
 This attempts to speed up expression operations across the board.  Working on
 debugging regressions.  Checkpointing.

 Revision 1.20  2008/02/25 18:22:16  phase1geo
 Moved statement supplemental bits from root expression to statement and starting
 to add support for race condition checking pragmas (still some work left to do
 on this item).  Updated IV and Cver regressions per these changes.

 Revision 1.19  2008/01/30 05:51:50  phase1geo
 Fixing doxygen errors.  Updated parameter list syntax to make it more readable.

 Revision 1.18  2008/01/16 05:01:22  phase1geo
 Switched totals over from float types to int types for splint purposes.

 Revision 1.17  2008/01/10 04:59:04  phase1geo
 More splint updates.  All exportlocal cases are now taken care of.

 Revision 1.16  2008/01/07 23:59:54  phase1geo
 More splint updates.

 Revision 1.15  2007/11/20 05:28:58  phase1geo
 Updating e-mail address from trevorw@charter.net to phase1geo@gmail.com.

 Revision 1.14  2006/10/12 22:48:46  phase1geo
 Updates to remove compiler warnings.  Still some work left to go here.

 Revision 1.13  2006/10/06 22:45:57  phase1geo
 Added support for the wait() statement.  Added wait1 diagnostic to regression
 suite to verify its behavior.  Also added missing GPL license note at the top
 of several *.h and *.c files that are somewhat new.

 Revision 1.12  2006/10/02 22:41:00  phase1geo
 Lots of bug fixes to memory coverage functionality for GUI.  Memory coverage
 should now be working correctly.  We just need to update the GUI documentation
 as well as other images for the new feature add.

 Revision 1.11  2006/09/26 22:36:38  phase1geo
 Adding code for memory coverage to GUI and related files.  Lots of work to go
 here so we are checkpointing for the moment.

 Revision 1.10  2006/09/01 04:06:37  phase1geo
 Added code to support more than one instance tree.  Currently, I am seeing
 quite a few memory errors that are causing some major problems at the moment.
 Checkpointing.

 Revision 1.9  2006/06/29 20:57:24  phase1geo
 Added stmt_excluded bit to expression to allow us to individually control line
 and combinational logic exclusion.  This also allows us to exclude combinational
 logic within excluded lines.  Also fixing problem with highlighting the listbox
 (due to recent changes).

 Revision 1.8  2006/06/29 20:06:33  phase1geo
 Adding assertion exclusion code.  Things seem to be working properly with this
 now.  This concludes the initial version of code exclusion.  There are some
 things to clean up (and maybe make better looking).

 Revision 1.7  2006/06/28 04:35:47  phase1geo
 Adding support for line coverage and fixing toggle and combinational coverage
 to redisplay main textbox to reflect exclusion changes.  Also added messageBox
 for close and exit menu options when a CDD has been changed but not saved to
 allow the user to do so before continuing on.

 Revision 1.6  2006/06/27 22:06:26  phase1geo
 Fixing more code related to exclusion.  The detailed combinational expression
 window now works correctly.  I am currently working on getting the main window
 text viewer to display exclusion correctly for all coverage metrics.  Still
 have a ways to go here.

 Revision 1.5  2006/06/26 22:49:00  phase1geo
 More updates for exclusion of combinational logic.  Also updates to properly
 support CDD saving; however, this change causes regression errors, currently.

 Revision 1.4  2006/06/26 04:12:55  phase1geo
 More updates for supporting coverage exclusion.  Still a bit more to go
 before this is working properly.

 Revision 1.3  2006/06/23 19:45:27  phase1geo
 Adding full C support for excluding/including coverage points.  Fixed regression
 suite failures -- full regression now passes.  We just need to start adding support
 to the Tcl/Tk files for full user-specified exclusion support.

 Revision 1.2  2006/06/23 04:03:30  phase1geo
 Updating build files and removing syntax errors in exclude.h and exclude.c
 (though this code doesn't do anything meaningful at this point).

 Revision 1.1  2006/06/22 21:56:21  phase1geo
 Adding excluded bits to signal and arc structures and changed statistic gathering
 functions to not gather coverage for excluded structures.  Started to work on
 exclude.c file which will quickly adjust coverage information from GUI modifications.
 Regression has been updated for this change and it fully passes.

*/

