/*
 *  macio.h
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
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

#if MAC_OS_X_VERSION	

#include <Carbon/Carbon.h>
//#include <Types.h>

#include "systemswitch.h"

#include "mydefs.h"
#include "macTypeCreator.h"
#include "nav.h"

#include "macfileaccess.h"

	

#if defined(__cplusplus)
extern "C"
{
#endif

//wwwBoolean 	selectForSave ( FSSpec *theFSSpec, char *name );
#ifndef     MAC_VERSION
char 	    *ioOpenTextFSSpec (FSSpec theFile);
#endif

void 	    initIO( void );
char 	    *sdSelectAndLoadFile( OSType type );
wwwBoolean 	_saveTextFileAs (char *buf, char *name, OSType type);

#if defined(__cplusplus)
}
#endif

#endif