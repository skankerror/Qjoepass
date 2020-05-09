/*
 *  myPrintf.h
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---today>  Wolfgang Westerboer
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------*/

/*
        replaces calls to printf by calls to my routines
        io-redirection
*/

#include "mydefs.h"

#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdio.h>

#define _debugPrintf    _myPrintf
#define printf          myPrintf

int     myPrintf( const char *format, ... );
int     _myPrintf( const char *arg, ... );

void    printResult( int r );

int     printToFile( const char *name, const char *format, ... );
void    deleteLogfile( void );

#if defined(__cplusplus)
}
#endif
