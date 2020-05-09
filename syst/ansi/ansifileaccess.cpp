/*
 *  ansifileaccess.cpp
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
/*  place system dependent io routines here
    three routines are needed by JoePass:
    
	void initIO( void )								{					}
        do whatever you need to set up the io-system

	char *sdSelectAndLoadFile( void )				{	return NULL;	}
        call a file selector and 
        return a buffer with the content of the selected file or NULL
         
	wwwBoolean saveAsTextFile (char *buf, char *name)	{	return false;	}
        call a files elector with default name set to "name"
        save content of buffer in selected file,
        or do nothing if user hits cancel
        return true if file was saved, false otherwise
        
  
    create a file that contains these routines
    and include it here
    don´t forget to set a precompiler switch to activate your additions
    (see dysDep.h)
    
    wolfgang westerboer
    westwolf@netcologne.de
    
*/  



#include "systemswitch.h"

#if UNDEFINED_VERSION

#include <stdio.h>
#include "fileaccess.h"
#include "mydefs.h"



void _initIO( void )					{			}
//char *selectAndLoadFile( void )				{	return NULL;	}
//wwwBoolean saveTextFileAs(char *buf, char *name)	{	return false;	}
//wwwBoolean useFileSelector( void )                 	{	return true;   	}
wwwBoolean fis( char *infoString, char *name )					{	return false;	}
wwwBoolean fio( char *infoString )						{	return false;	}


//char *callFileSelectorForPath( void)				{   return NULL;	}




#endif
