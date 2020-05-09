/*
 *  stringtoscreen.cpp
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
#include "graphics.h" 

#include <stdlib.h>
#include <string.h>


#include "grafik.h"
#include "stringtoscreen.h"
#include "prefDef.h"
#include "myPrintf.h"

#define WarningTextPointer "Pattern is not a valid siteswap. Animation may look strange!"
#define ErrorTextPointer   "Syntax Error. See red text."



int     leftMargin      = 10;
int     rightMargin     = 10;

GLint   messageWin = 0;
    
char    *theMessageString = "";
    
void    deleteMessageWindow( void );
void    messageDisplay(void);
void    messageKeyboard(unsigned char key, int x, int y);

void stringToScreen( char s[], int alignement, int offset, int h, float scale, sysColor *c)

{
    long len, w;
    int i;
    static float x;
    int windowWidth, windowHeight;
    int charWidth;
    
    windowWidth = glutGet( GLUT_WINDOW_WIDTH );
    windowHeight = glutGet( GLUT_WINDOW_HEIGHT );
    charWidth = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, ' ');

    setColorFromVector(*c);
    len = strlen(s);
    if( alignement == continuous ) {
        for (i = 0; i < len; i++)
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *(s+i));

        return;
    }
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    scale *= .1f;
    if( alignement == left ) {
        x = leftMargin;
    } else {
        w = 0;
        for (i = 0; i < len; i++)
            w += charWidth;
        w *= scale;
        if( alignement == centered )
            x = (float)windowWidth/2 - w/2;
        else // right
            x = windowWidth - w - rightMargin;
        if( x < 0 ) // force string to fit on screen
        {       x = 0;
            scale *= (float)windowWidth/w;
        }

    }
    glTranslatef(x + offset * charWidth * scale, h , 0);
    glScalef( scale, scale, scale);
    for (i = 0; i < len; i++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *(s+i));

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

void message( char *msg )
{
    glutInitWindowPosition(jpPrefs.worldWinSizeX , jpPrefs.worldWinSizeY);
    glutInitWindowSize(jpPrefs.worldWinSizeW, jpPrefs.worldWinSizeH);
    atexit( deleteMessageWindow);

    messageWin = glutCreateWindow("message:");

    theMessageString = msg;
    glutDisplayFunc(messageDisplay);
    glutKeyboardFunc(messageKeyboard);

    _myPrintf("messageWinID = %d\n", messageWin);
}

void deleteMessageWindow( void )
{
    if(messageWin == 0 )
		return;
	
    glutDestroyWindow( messageWin);
	messageWin = 0;    
	_myPrintf("message window closed\n");

}

void messageDisplay(void)
{
    glClearColor( 1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stringToScreen( theMessageString, centered, 0, atLine( 1 ), .8f, &black);
    stringToScreen( "press any key", centered, 0, atLine( 0 ), .8f, &black);

    if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
        glutSwapBuffers();
    else
        glFlush();

}

void    messageKeyboard(unsigned char key, int x, int y)
{



    deleteMessageWindow( );
}


void initOneMessage( bool valid )
{
 
 if( jpPrefs.showMessageWindow == false )
    return;
    
    
    // only one message at a time
    if( messageWin )
        return;
        
    if( valid )
        message( WarningTextPointer);
     else
        message( ErrorTextPointer);
            
}