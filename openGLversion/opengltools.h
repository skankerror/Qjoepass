/*
 *  opengltools.h
 *  joe2
 *
 * created by wolfgang on 20.04.2009
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

#ifndef __OPENGL_TOOLS__
#define __OPENGL_TOOLS__


#if defined(__cplusplus)
extern "C"
{
#endif

#define	glutGet	wwGlutGet

GLint 	wwGlutGet( GLenum value );

#define getWindowPosition(a,b) {	\
	int x,y;\
	_getWindowPosition( &x, &y);\
	*a = x;\
	*b = y;\
}	

#define getWindowSize(a,b) {	\
	int x,y;\
	_getWindowSize( &x, &y);\
	*a = x;\
	*b = y;\
}	

void	_getWindowSize( int *w, int *h);
void	_getWindowPosition( int *x, int *y);


#if defined(__cplusplus)
}
#endif


#endif


