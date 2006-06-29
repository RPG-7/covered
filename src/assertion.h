#ifndef __ASSERTION_H__
#define __ASSERTION_H__

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
 \file     assertion.h
 \author   Trevor Williams  (trevorw@charter.net)
 \date     4/4/2006
 \brief    Contains functions for handling assertion coverage.
*/


#include <stdio.h>

#include "defines.h"


/*! \brief Parses -A command-line option to score command */
void assertion_parse( char* arg );

/*! \brief Parses an in-line attribute for assertion coverage information */
void assertion_parse_attr( attr_param* ap, func_unit* funit );

/*! \brief Gather statistics for assertion coverage */
void assertion_get_stats( func_unit* funit, float* total, int* hit );

/*! \brief Generates report output for assertion coverage */
void assertion_report( FILE* ofile, bool verbose );

/*! \brief Retrieves the total and hit counts of assertions for the specified functional unit */
bool assertion_get_funit_summary( char* funit_name, int funit_type, int* total, int* hit );

/*! \brief Collects uncovered and covered assertion instance names for the given module */
bool assertion_collect( char* funit_name, int funit_type, char*** uncov_inst_names, int** excludes, int* uncov_inst_size,
                        char*** cov_inst_names, int* cov_inst_size );

/*! \brief Gets missed coverage point descriptions for the given assertion module */
bool assertion_get_coverage( char* funit_name, int funit_type, char* inst_name, char** assert_mod, str_link** cp_head, str_link** cp_tail );


/*
 $Log$
 Revision 1.7  2006/05/01 22:27:37  phase1geo
 More updates with assertion coverage window.  Still have a ways to go.

 Revision 1.6  2006/05/01 13:19:07  phase1geo
 Enhancing the verbose assertion window.  Still need to fix a few bugs and add
 a few more enhancements.

 Revision 1.5  2006/04/29 05:12:14  phase1geo
 Adding initial version of assertion verbose window.  This is currently working; however,
 I think that I want to enhance this window a bit more before calling it good.

 Revision 1.4  2006/04/28 17:10:19  phase1geo
 Adding GUI support for assertion coverage.  Halfway there.

 Revision 1.3  2006/04/18 21:59:54  phase1geo
 Adding support for environment variable substitution in configuration files passed
 to the score command.  Adding ovl.c/ovl.h files.  Working on support for assertion
 coverage in report command.  Still have a bit to go here yet.

 Revision 1.2  2006/04/06 22:30:03  phase1geo
 Adding VPI capability back and integrating into autoconf/automake scheme.  We
 are getting close but still have a ways to go.

 Revision 1.1  2006/04/05 15:19:18  phase1geo
 Adding support for FSM coverage output in the GUI.  Started adding components
 for assertion coverage to GUI and report functions though there is no functional
 support for this at this time.

*/

#endif

