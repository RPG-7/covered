#ifndef __VCD_H__
#define __VCD_H__

/*!
 \file     vcd.h
 \author   Trevor Williams (trevorw@charter.net)
 \date     7/21/2002
 \brief    Contains VCD parser functions.
*/

/*! \brief Parses specified VCD file, storing information into database. */
void vcd_parse( char* vcd_file );


/*
 $Log$
 Revision 1.3  2002/10/31 23:14:32  phase1geo
 Fixing C compatibility problems with cc and gcc.  Found a few possible problems
 with 64-bit vs. 32-bit compilation of the tool.  Fixed bug in parser that
 lead to bus errors.  Ran full regression in 64-bit mode without error.

 Revision 1.2  2002/10/29 19:57:51  phase1geo
 Fixing problems with beginning block comments within comments which are
 produced automatically by CVS.  Should fix warning messages from compiler.

 Revision 1.1  2002/07/22 05:24:46  phase1geo
 Creating new VCD parser.  This should have performance benefits as well as
 have the ability to handle any problems that come up in parsing.
*/

#endif

