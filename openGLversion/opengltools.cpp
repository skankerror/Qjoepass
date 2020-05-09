/*
 *  opengltools.cpp
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

/*
fixes problem with mac os x and glut
which returns local coordinates for x and y position of window (i.e. always 0 )
*/

#include "graphics.h"

void    resizeWindows();

void _getWindowPosition( int *x, int *y)
{

        *x =glutGet(GLUT_WINDOW_X);
        *y = glutGet(GLUT_WINDOW_Y);
}

void _getWindowSize( int *w, int *h)
{
        *w = glutGet(GLUT_WINDOW_WIDTH);
        *h =glutGet(GLUT_WINDOW_HEIGHT);
}

// definition of wwGlutGet must be last in file

#undef glutGet


GLint wwGlutGet( GLenum value )
{

        GLint ret = glutGet( value );

#ifdef MAC_CLASSIC_VERSION
// windows coordinates are local values on Mac ;(
        Point p = {0,0};
        int x,y;
        p.v = x = glutGet(GLUT_WINDOW_X);
        p.h = y = glutGet(GLUT_WINDOW_Y);
        LocalToGlobal(&p);

//      _myPrintf("x = %d, y = %d\n", x,y);
        switch (value )
        {

                case GLUT_WINDOW_X:
                        ret = p.h - 22;
                        break;
                case GLUT_WINDOW_Y:
                        ret = p.v;
                        break;
                default:
                        break;
        }

#endif

        return ret;

}

#undef glutSetWindow
void wwGlutSetWindow( int w)
{
	glutSetWindow( w );
}
