#ifndef __MERGE_H__
#define __MERGE_H__

/*!
 \file     merge.h
 \author   Trevor Williams  (trevorw@charter.net)
 \date     11/29/2001
 \brief    Contains functions for merge command.
*/


/*! \brief Parses command-line for merge options and performs merge command. */
int command_merge( int argc, int last_arg, char** argv );


/*
 $Log$
 Revision 1.5  2002/10/31 23:13:56  phase1geo
 Fixing C compatibility problems with cc and gcc.  Found a few possible problems
 with 64-bit vs. 32-bit compilation of the tool.  Fixed bug in parser that
 lead to bus errors.  Ran full regression in 64-bit mode without error.

 Revision 1.4  2002/10/29 19:57:50  phase1geo
 Fixing problems with beginning block comments within comments which are
 produced automatically by CVS.  Should fix warning messages from compiler.

 Revision 1.3  2002/07/09 04:46:26  phase1geo
 Adding -D and -Q options to covered for outputting debug information or
 suppressing normal output entirely.  Updated generated documentation and
 modified Verilog diagnostic Makefile to use these new options.

 Revision 1.2  2002/07/03 03:31:11  phase1geo
 Adding RCS Log strings in files that were missing them so that file version
 information is contained in every source and header file.  Reordering src
 Makefile to be alphabetical.  Adding mult1.v diagnostic to regression suite.
*/

#endif

