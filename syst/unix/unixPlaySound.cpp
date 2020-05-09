/*
 * unixplaysound.cpp
 *  joe2
 *
 *
 * Dummy routines for sound output.
 * if you want to add sound support for your system, make a copy of this file, and add code for your sysem
  * Replace switch UNDEFINED_VERSION (defined in systemswitch.h, more infos there)
 * See folder content of mac10mm for a working example.
 *
 * created by wolfgang 
 *//*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---2011>  Wolfgang Westerboer
 
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

#include "systemswitch.h"
#if UNIX_VERSION

#include "headers.h"
#include "playSound.h"
#include "myutil.h"
 
bool	soundToPlay;
int		soundKind;	

void 	initSounds( void ) 											{				}
void 	disposeSounds( void ) 										{				}
void 	doSound(short id, _side who, int val) 						{	_myBeep();	}
short	playSndAsync(short resID, short vol)						{	_myBeep(); return 0;	}
short	playSndAsyncPan(short resID, short l, short r, short vol)	{	_myBeep(); return 0;	}
void 	idleSounds(void)											{				}

#endif