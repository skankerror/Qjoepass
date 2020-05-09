/*
 *  myUtil.h
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

#ifndef __UTIL__
#define __UTIL__
#include "systemswitch.h"

#include "mydefs.h"
#if defined(__cplusplus)
extern "C++"
{
#endif

#ifdef MAC_CLASSIC_VERSION
extern wwwBoolean		skMacMode, ckMacMode, akMacMode, capsMacMode, cmdMacMode;		//Zustand der Sondertasten
#endif
extern wwwBoolean		skMode, ckMode, akMode, capsMode, cmdMode;
void beep( void );
void _myBeep( void );

#if MAC_CLASSIC_VERSION
void getMacModifiers();
#endif

int getModifiers( void );

void checkApp( void );
#if defined(__cplusplus)
}
#endif


#endif
