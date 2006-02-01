/*!
 \file     parser_misc.c
 \author   Trevor Williams  (trevorw@charter.net)
 \date     12/19/2001
*/

#include <stdio.h>

#include "defines.h"
#include "parser_misc.h"
#include "util.h"
#include "static.h"


extern char          user_msg[USER_MSG_LENGTH];
extern vector_width* curr_range;


/*!
 Counts the number of errors found during the parsing process.
*/
unsigned error_count = 0;

/*!
 Counts the number of warnings found during the parsing process.
*/
unsigned warn_count  = 0;

/*!
 \param msg  String containing error message to display to user.

 Outputs specified error message string to standard output and increments
 error count.
*/
void VLerror( char* msg ) {

  error_count += 1;
  
  snprintf( user_msg, USER_MSG_LENGTH, "%s,   file: %s, line: %d", msg, yylloc.text, yylloc.first_line );
  print_output( user_msg, FATAL, __FILE__, __LINE__ );

}

/*!
 \param msg  String containing warning message to display to user.

 Outputs specified warning message string to standard output and increments
 warning count.
*/
void VLwarn( char* msg ) {

  warn_count += 1;
  
  snprintf( user_msg, USER_MSG_LENGTH, "%s,   file: %s, line: %d", msg, yylloc.text, yylloc.first_line );
  print_output( user_msg, WARNING, __FILE__, __LINE__ );

}

/*!
 Called by parser when file wrapping is required.
*/
int VLwrap() {

  return -1;

}

/*!
 Deallocates all memory associated with the curr_range global variable.
*/
void parser_dealloc_curr_range() {

  if( curr_range != NULL ) {
    static_expr_dealloc( curr_range->left,  FALSE );
    static_expr_dealloc( curr_range->right, FALSE );
    free_safe( curr_range );
  }

}

/*!
 Creates a copy of the curr_range variable for stored usage.
*/
vector_width* parser_copy_curr_range() {

  vector_width* vw;  /* Copy of the curr_range variable */

  assert( curr_range != NULL );

  vw             = (vector_width*)malloc_safe( sizeof( vector_width ), __FILE__, __LINE__ );

  vw->left       = (static_expr*)malloc_safe( sizeof( static_expr ), __FILE__, __LINE__ );
  vw->left->num  = curr_range->left->num;
  vw->left->exp  = curr_range->left->exp;

  vw->right      = (static_expr*)malloc_safe( sizeof( static_expr ), __FILE__, __LINE__ );
  vw->right->num = curr_range->right->num;
  vw->right->exp = curr_range->right->exp;

  return( vw );

}

/*!
 \param left   Pointer to static expression of expression/value on the left side of the colon.
 \param right  Pointer to static expression of expression/value on the right side of the colon.
 \param implicit

 Deallocates and sets the curr_range variable from static expressions
*/
void parser_explicitly_set_curr_range( static_expr* left, static_expr* right ) {

  /* Deallocate any memory currently associated with the curr_range variable */
  parser_dealloc_curr_range();

  /* Allocate and set curr_range */
  curr_range           = (vector_width*)malloc_safe( sizeof( vector_width ), __FILE__, __LINE__ );
  curr_range->left     = left;
  curr_range->right    = right;
  curr_range->implicit = FALSE;

}

/*! \brief Deallocates and sets the curr_range variable from scratch values */
void parser_implicitly_set_curr_range( int left_num, int right_num ) {

  /* Deallocate any memory currently associated with the curr_range variable */
  parser_dealloc_curr_range();

  /* Allocate and set curr_range */
  curr_range             = (vector_width*)malloc_safe( sizeof( vector_width ), __FILE__, __LINE__ );
  curr_range->left       = (static_expr*)malloc_safe( sizeof( static_expr ), __FILE__, __LINE__ );
  curr_range->left->num  = left_num;
  curr_range->left->exp  = NULL;
  curr_range->right      = (static_expr*)malloc_safe( sizeof( static_expr ), __FILE__, __LINE__ );
  curr_range->right->num = right_num;
  curr_range->right->exp = NULL;
  curr_range->implicit   = TRUE;

}


/*
 $Log$
 Revision 1.4  2004/03/16 05:45:43  phase1geo
 Checkin contains a plethora of changes, bug fixes, enhancements...
 Some of which include:  new diagnostics to verify bug fixes found in field,
 test generator script for creating new diagnostics, enhancing error reporting
 output to include filename and line number of failing code (useful for error
 regression testing), support for error regression testing, bug fixes for
 segmentation fault errors found in field, additional data integrity features,
 and code support for GUI tool (this submission does not include TCL files).

 Revision 1.3  2003/08/10 03:50:10  phase1geo
 More development documentation updates.  All global variables are now
 documented correctly.  Also fixed some generated documentation warnings.
 Removed some unnecessary global variables.

 Revision 1.2  2002/12/01 06:37:52  phase1geo
 Adding appropriate error output and causing score command to proper exit
 if parser errors are found.

*/

