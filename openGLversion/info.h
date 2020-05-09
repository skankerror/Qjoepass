/*
 *  info.h
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

#ifndef __INFO__
#define __INFO__

#include "openglmenu.h"

#if defined(__cplusplus)
extern "C++"
{
#endif

#include "headers.h"

void createInfoWindow( wwwBoolean showInfoWindow);
void reshapeInfo(int width, int height);
void redisplayInfoWindow( void );

void initInfo(void);
void disposeInfo( void );
void resetInfo(void);
void setNextObjectInfo( _object *o);

void clearInfoString( void );
void setInfoString( char *i );
void resetEventInfo( void );
void nextEventInfo( void );
void setRepeatLine( char *p, int l);

void showFrameRate( void );

void    moreFrames( int dummy );
void    lessFrames( int dummy );

extern char             *externBuf;
extern int				infoWin;
extern _menuList        *infoMenuList;
extern wwwBoolean       col;
extern bool				showPatternTimeFlag;
extern bool				showObjectColorFlag; 
extern bool				showRunaroundInfoFlag;
	
#if defined(__cplusplus)
}
#endif

#endif
