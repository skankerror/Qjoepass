/*
 *  pattern.cpp
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

#include "systemswitch.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "fileaccess.h"


#include "editor.h"

#include "parseSiteswap.h"
#include "world.h"
#include "styleScanner.h"
#include "pattern.h"
#include "fileIO.h"
#include "applicationFolders.h"



#include "preprocess.h"

#include "myPrintf.h"

/* globals */
char defPatternReserve[] = "<4p 3|3 4p>";
char *defPattern;


wwwBoolean selectAndStartPattern( void)
{	
	char *p;
	char i[100];
	sprintf(i, "select file");

	
	p = selectAndLoadFile("animate pattern");			

	if( p )
	{	startPattern(p);
		free( p );
	}	

	return validPattern;
}
wwwBoolean readAndStartPatternModeCheck( char *filename)
{
	bool r;
	
	char *p = readFile( filename, "r" );

	if( p )
	{
		size_t l;
		char *q;
		
		//------------------------
		// mode is part of pattern
		//------------------------
		if( strstr( p, "#s ") != NULL){
			free( p );
			return readAndStartPattern( filename );
		}
			
		l = strlen( p ) + 20;
		q = (char *)malloc( l );

		//------------------------
		// try vanilla first, 
		// then extended (if neccessary)
		//------------------------
		sprintf(q, "#s b\n%s", p );
		r = startPattern( q );
		if( r == false ){
			sprintf(q, "#s x\n%s", p );
			r = startPattern( q );
			
		}
			
		free( p );
		free( q );
		return r;
	}else
		return false;
	
}
wwwBoolean readAndStartPattern( char *filename)
{
	char *p = readFile( filename, "r" );
	if( p )
	{
//		_myPrintf("%s\n", p );		
		startPattern( p );
		free( p );
		return true;
	}else
		return false;
		
}


wwwBoolean startPattern( char *p )
{	
	size_t l;
	char *theNewPattern;
	
	theNewPattern = preprocess( p );
	if( theNewPattern == NULL )
		return validPattern = false;
	
	if( !skipAnimateCdewPattern)
#if 01
		if( theNewPattern )
		{	l = strlen(theNewPattern);
			initSiteswapPattern( theNewPattern, l);
		}
#else	
		// will call itself
	_editStartFunc(theNewPattern);
#endif		
	
	free( theNewPattern);
	return validPattern;
}

