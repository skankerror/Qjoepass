/*
 *  ansiMovie.cpp
 *  joe2
 *
 *
 * Dummy routines for movie creation.
 * if you want to add movie support for your system, make a copy of this file, and add code for your sysem
 * Replace switch UNDEFINED_VERSION (defined in systemswitch.h, more infos there)
 * See folder content of mac10mm for a working example.
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---TODAY>  Wolfgang Westerboer
 
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

#if 0
//#if UNDEFINED_VERSION


#include "movie.h"
#include "fileaccess.h"

void _initMovie( void )
{
}


void _exitMovie( void )
{
}

bool _startMovie( void )
{
    return true;
}

void _endMovie( void )
{
}


// add one frame to the movie.
bool _addFrameToMovie(char *filename )
{
	return false;
}

bool getSysdepCompressionInfo( void )
{
    return false;
}
#endif