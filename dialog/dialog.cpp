/*
 *  dialog.cpp 
 *  joe2
 *
 *  created by wolfgang on 02.12.06.
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dialog.h"
#include "dialogClass.h"
#include "stringtoscreen.h"
#include "grafik.h"
#include "animationWin.h"
#include "myutil.h"
#include "languageSupport.h"


/*	example for a dialog call
	\n is interpreted as a new line
	letters in brackets are hotkeys
	can these be displayed by underlined keys?

dialog ("what shall I do with the current editor content?\n[s]ave, [d]iscard, [a]bort action")
 
 the function returns the key pressed. Hotkeys are hardcoded in the source, not in the resourcesby now.
 
*/



#define DialogTextSize         .8f
dialog	*dialogPtr				= NULL;



void dialogDraw( void );
void dialogKey( unsigned char theKey, int x, int y );
void dialogMouse(int button, int state, int x, int y);

void dialog::redisplayDialogWindow( void ) {
	
    if (id) {
        glutSetWindow(id);
        glutPostRedisplay();
		
    }
}


void dialog::setDialogString( char *s)
{
	
	if( dialogString )
		free( dialogString );
			
	dialogString = (char *)malloc( strlen( s ) + 1 );
	
	sprintf(dialogString, "%s", s);
}

void dialog::showDialogString( void ) {
	
	int c;

	for( c = 0; c < dialogLineCount; c++){
		stringToScreen( dialogLineBuffers[c], centered, 0, atLine(dialogLineCount-c+3),DialogTextSize,&black);
	}
    stringToScreen( languageStrings[languageHintDialog], centered, 0, atLine(1),DialogTextSize,&black);
}



void dialog::clearDialogString( void ) {

	sprintf(dialogString, "");
    RedisplayAppWindows();
}

void dialogDraw( void )
{
	if( dialogPtr )
		dialogPtr->draw();
}

void dialog::draw( void ) {
	
    if(!id )
        return;
	
    glutSetWindow( id);

    glClearColor(1, 1, 1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showDialogString();
    if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
        glutSwapBuffers();
    else
        glFlush();
}



void dialog::reshapeDialog(int wth, int height) {
	
    if(!id )
        return;
	
    glutSetWindow( id);
    glViewport(0, 0, wth, height);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,1.0 *  (GLfloat)wth/(GLfloat)height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    RedisplayAppWindows();
	
}


void dialog::createDialogWindow( void ) {
    
	printf("\ncreating dialog window\n");

	if( !id ) {
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(500, 100);
		id = glutCreateWindow("warning");
		
		glutDisplayFunc( dialogDraw );
//            glutReshapeFunc(reshapeInfo);
		glutMouseFunc(dialogMouse);
//		glutMotionFunc(NULL);
		glutVisibilityFunc(NULL);
		glutKeyboardFunc(dialogKey);      // one for all

	} 
    printf("dialogWin = %d\n", id);
}

void dialogKey( unsigned char theKey, int x, int y )
{
	if( dialogPtr )
		dialogPtr->key(theKey, x,y);
}


void dialog::key(unsigned char theKey, int x, int y) {
#pragma unused (x,y)
	dialogExitKey = theKey;
	delete this;

}

void dialogMouse(int button, int state, int x, int y)

{
	if( dialogPtr )
		dialogPtr->mouse(button, state, x,y);
}



void dialog::mouse(int button, int state, int x, int y) {
#pragma unused (x,y)
	
	delete this;

}

dialog::dialog( char *m ) {
	char *s;
	
	if( dialogPtr )
		return;
	
	id = 0;
	dialogString = NULL;
	
	createDialogWindow( );
	setDialogString( m );
	dialogPtr = this;
	
	
	dialogStringBuffer = s = (char *)malloc( strlen( dialogString ) + 1 );
	sprintf( s, "%s", dialogString);
	
	dialogLineCount = 0;
	
	
	do{
		dialogLineBuffers[dialogLineCount++] = s;
		s = strpbrk( s, "\n");
		if( s != NULL )
			*s++ = '\0';
	}while( s );
	
	
}


dialog::~dialog() {
	glutDestroyWindow(id);
	dialogPtr = NULL;
	
	free( dialogStringBuffer );
}


void initDialog( void )
{
	
}

void showDialog( char *messageString ) {
	
    new dialog( messageString);
	_myBeep();
}

