/*
 *  styleScanner.h
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
#ifndef __stylescanner__
#define __stylescanner__


#if defined(__cplusplus)
extern "C"
{
#endif

#include "systemswitch.h"
#include "headers.h"

extern char *tp;
extern size_t mySize;
extern styleEl *defStyle;

// File einlesen und Listen erzeugen
wwwBoolean scanStyleFile( char *name, style *f );
wwwBoolean readStyleFile( char *name );
void    readStyles(style *first );
wwwBoolean      readMoves(style *first );
void    releaseStyles( style *first );
void    releaseMoves( styleEl *f );
void    releaseFileMemory( void );

// Einen Eintrag abfragen
style *getStyle( char *name, style *f );

size_t  readLine(char *str, size_t s);
size_t  readWord(char *str, size_t s);

style *getStyleFromIndex( int i );
void getDefaultStyles( void );
void releaseDefaultStyles( void );



#if defined(__cplusplus)
}
#endif

#endif
