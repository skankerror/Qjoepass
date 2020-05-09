/*
 *  graphics.h
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


#ifdef MAC_OS_X_VERSION
#include <GLUT/glut.h>
#elif MAC_CLASSIC_VERSION
#include <GL/glut.h>
#elif WINDOWS_VERSION
// curently windows 
/* according to
http://social.msdn.microsoft.com/Forums/vstudio/en-US/ec161b16-fcb8-425c-8a25-e76b601290cd/stdlibh-of-vs2008-incompatible-with-opengl
the order of including glut.h and stdlib.h solves errors related with the redefinition of exit().
So I always include stdlib before opengl under windows to get rid of this.
*/

#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#else
//#error Failing "unsupported system"
#include <GL/glut.h>
#include <stdlib.h>
#include <GL/gl.h>
#endif
#include "opengltools.h"

