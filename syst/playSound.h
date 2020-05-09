/*
 *  playSound.h
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

#ifndef __PLAY_SOUND__
#define __PLAY_SOUND__

#include "pathAccess.h"

#if defined(__cplusplus)
extern "C"
{
#endif

 
// this is not ok. doSound needs different parameters
#include "headers.h"

#define mp3Sound

enum {siteswapNotes = 0, soundNoises = 1, endOfSoundKinds};

extern	bool	soundToPlay;
extern	int 	soundKind;

	
void	initSounds( void );
void 	idleSounds(void);
void	doSound( short id, _side who, int vol);
	
bool	isBackgroundSoundDefined( void );
void	initBackgroundSound( char *name );
void	resetBackgroundSound( void );
void	playBackgroundSound( void );
void	stopBackgroundSound( void );
	



#if defined(__cplusplus)
}
#endif

#endif
