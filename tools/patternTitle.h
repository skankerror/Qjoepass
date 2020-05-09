/*
 *  patternTitle.h
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

#ifndef __PATTERN_TITLE__
#define __PATTERN_TITLE__


#if defined(__cplusplus)
extern "C"
{
#endif

 
#define MAX_PATTERN_TITLE 	256
#define MAX_FULL_FILE_NAME	2000

extern  char thePatternTitle[];
extern  char fullPatternPathName[];
extern  char fullPatternPath[];

void setPatternTitle( char *title );

#if defined(__cplusplus)
}
#endif

#endif
