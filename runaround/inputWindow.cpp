/*
 *  inputWindow.cpp
 *  joe2
 *
 * created by wolfgang on 30.12.2001
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
 input window for runaround editor.
 
 in opposite to the animation window, we use z for height here.
 this should be changed!
 */
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "cdewDraw.h"

#include "pathAccess.h"

#include "animationWin.h"
#include "runaround.h"
#include "runaroundMenu.h"
#include "inputWindow.h"
#include "listWindow.h"
#include "grafik.h"
#include "myPrintf.h"
#include "myutil.h"
#include "vmath.h"
#include "vtypes.h"
#include "parseSiteswap.h"

int		startingAngleOfFrame = 0;

bool	runaroundRotateAllScenesTogetherFlag = false;
bool	runaroundRotateCurrentSceneFlag = false;
bool	runaroundRotateCurrentLineFlag = false;
bool	_runaroundRotateScene2 = true;
bool	runaroundShowAnimatedSceneFlag = false;
bool	showGridFlag = true;
bool	showPosFlag = false;
bool	showPoiFlag = false;
bool	showFrameFlag = true;
bool	showSceneIndexFlag = false;

int		runaroundShowSceneIndexFlag = false;
int		runaroundShowFrameFlag;









#define                                 deltaPos                30


#define RUNAROUND_SCALE_FACTOR  1.1f
extern float    metersOnScreen;


#ifndef min
#define min( a, b) ((a)<(b)?(a):(b))
#define max( a, b) ((a)>(b)?(a):(b))
#endif

#define                                 drawOneLine2D(x1,y1,x2,y2)  {glBegin(GL_LINES);  \
glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2) ); glEnd();}

void                                    drawTriangle( float scale );
float                                   angleBetween3Points( punkt a, punkt m, punkt b);
#if 0
int										
= false;
int										drawPoiInRunaroundEditorFlag = false;
#endif
int										drawModifiersInRunaroundEditorFlag = true;

wwwBoolean                              drawFrameNowFlag;
wwwBoolean                              drawFrameInRunaroundEditorFlag = true;
int                                     cornersInRunaroundView = 6;

void                                    drawFrame( int x, int y, int w, int h );

int                                     theRunaroundModifiers;
#define                                 DELTA_MOVEMENT  10.


void                                    drawOpenGLDisk( float scale );
void                                    drawOpenGLFilledDisk( float scale );
void                                    setOpenGLPosition( float x, float y);
void                                    drawLine( float x1, float y1, float x2, float y2 );



static void                             mouse(int button, int state, int x, int y);
static void 							motion( int x, int y );
static void 							passiveMotion( int x, int y );
static void 							reshape(int w, int h);
static void 							inputDraw( void );

GLint                                   inputID = 0;

int                                     mouseX = -100, mouseY = -100;
int										rStartX;
int										rStartY;
float									deltaR = 0;
bool									rotateRunaroundScene = false;

float                                   metersOnScreen = 6.0f;
const float                             scaleInputScreenBy = .9f;
_runaroundJuggler						*theDraggedJugglerPos = NULL,
										*theDraggedJugglerPosOut = NULL,
										*theDraggedJugglerPosIn = NULL,
										*theDraggedJugglerPoi = NULL,
										*theDraggedJugglerPoiOut = NULL,
										*theDraggedJugglerPoiIn = NULL;
_runaroundJuggler						tmp;

char									*selectedRunaroundPattern;
int										selectedRunaroundIndex;


/*_runaroundJuggler             *alreadySwitchedJuggler = NULL,
 *switchedDraggedJuggler = NULL,
 *switchedDroppedJuggler = NULL;
 */
_runaroundJuggler               switchStartPos;



void							drawGrid( int x, int y, int w, int h );
_runaroundJuggler               *getJugglerWithPosAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);
_runaroundJuggler               *getJugglerWithPosAtScreenCoordExcept(_fieldContent *f, _runaroundJuggler *ex, int px, int py, int x, int y, int w, int h);
_runaroundJuggler               *getJugglerWithPosOutAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);
_runaroundJuggler               *getJugglerWithPosInAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);

_runaroundJuggler               *getJugglerWithPoiAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);
_runaroundJuggler               *getJugglerWithPoiOutAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);
_runaroundJuggler               *getJugglerWithPoiInAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h);

void                             screenToFieldCoord( int sx, int sy, int x, int y, int w, int h, int *fx, int *fy);
void                             fieldToScreenCoord( int fx, int fy, int x, int y, int w, int h, int *sx, int *sy);


void addPositionMenu( bool remove);


	
void inputWindow( void )
{
float screenW, screenH;
float ix, iy, iw, ih;

	char s[100];
	if( inputID == 0)
	{
		screenW = glutGet( GLUT_SCREEN_WIDTH );
		screenH = glutGet( GLUT_SCREEN_HEIGHT );
		
		ix = runaroundInputWinSizeX * screenW;
		iy = runaroundInputWinSizeY * screenH;
		iw = runaroundInputWinSizeW * screenW;
		ih = runaroundInputWinSizeH * screenH;
		glutInitWindowPosition( ix, iy);
		glutInitWindowSize( iw, ih);
		sprintf(s, "Positions");
		inputID= glutCreateWindow( s );
		_myPrintf("inputID = %d\n", inputID);
		
		glLineWidth(1.0f);
		
		glutReshapeFunc( reshape);
		glutDisplayFunc( inputDraw );
		glutKeyboardFunc( runaroundListKey );
		glutSpecialFunc( runaroundListSpecialKey );
		glutMouseFunc( mouse );
		glutMotionFunc( motion );
		glutPassiveMotionFunc( passiveMotion );
		
		runaroundInitMenu( );
		glutPostRedisplay();
	}
}

void closeInputWindow( void )
{
	if( inputID == 0)
		return;
	
	glutDestroyWindow( inputID );	
	inputID = 0;
	
	_myPrintf("position window closed\n");
	
}



/*	when the user makes a click close to an element (pos, poi or modifierts) the point (and, if existing, it´s subpoints) is selected
	the selected points will move with the mouse motion
	pos has highest priority, modifiers lowest
	postion moves poi and all modifiers
	poi also moves poi modifiert
	modifiers are moved by themself
	a modifiert that is being draged on it´s parent (pos or poi) will be removed
	shift, alt, cmd keys move or create modifiers 
 */
void mouse(int button, int state, int x, int y)
{
#ifdef TARGET_OS_MAC
#pragma unused (button, state)
#endif
	
	static bool rotationStartedNow;
	_fieldContent	*f;
	
	if( rotateRunaroundScene == false )
		rotationStartedNow = false;
		
	int w = glutGet( GLUT_WINDOW_WIDTH);
	int h = glutGet( GLUT_WINDOW_HEIGHT);
	
	mouseX = x;
	mouseY = y;
	
	f = getPointerToSelectedField();
	if( f == NULL ){
		rotateRunaroundScene = false;
		return;
	}
	
	setRunaroundPatternChanged( true );
	
	redrawRunaroundWindows();
	
	// done.
	// save results, if any
	if( state == GLUT_UP )
	{
		if( rotateRunaroundScene ){
			rotateRunaroundScene = false;
		}
		
#define	tooCloseCondition (1.1*deltaPos)
		
		if( theDraggedJugglerPoi ){
			Vector3D v, v1, oPoi;
			float b;
			
			v = theDraggedJugglerPoi->poi - theDraggedJugglerPoi->pos;
			v.z = 0;
			b = v.betrag();
			// check if poi is too close to pos
			// id => use old values
			if(0 || b <= 0.1f )
				*theDraggedJugglerPoi = tmp;
			// close: move a bit to the outside
			
			else if( b <= tooCloseCondition ){
				
				// save original poi
				oPoi = theDraggedJugglerPoi->poi;
				
				// move poi a bit away from pos
				v = v * tooCloseCondition * (1.3/b);	
				theDraggedJugglerPoi->poi = theDraggedJugglerPoi->pos + v;
				
				// get rel Distance from original poi to new poi
				v1 = theDraggedJugglerPoi->poi - oPoi;
//				oPoi = theDraggedJugglerPoi->poi;
				// fix poiIn and poiOut
				theDraggedJugglerPoi->poiIn = theDraggedJugglerPoi->poiIn + v1;
				theDraggedJugglerPoi->poiOut = theDraggedJugglerPoi->poiOut + v1;
				
			}			
		}
		if( theDraggedJugglerPosIn){
			if( fabs( theDraggedJugglerPosIn->pos.x - theDraggedJugglerPosIn->posIn.x ) <= DELTA_MOVEMENT &&
			   fabs( theDraggedJugglerPosIn->pos.y - theDraggedJugglerPosIn->posIn.y ) <= DELTA_MOVEMENT)
			{	theDraggedJugglerPosIn->posIn = theDraggedJugglerPosIn->pos;
				
			}
		}
		
		if( theDraggedJugglerPosOut){
			if( fabs( theDraggedJugglerPosOut->pos.x - theDraggedJugglerPosOut->posOut.x ) <= DELTA_MOVEMENT &&
			   fabs( theDraggedJugglerPosOut->pos.y - theDraggedJugglerPosOut->posOut.y ) <= DELTA_MOVEMENT)
			{ 	theDraggedJugglerPosOut->posOut = theDraggedJugglerPosOut->pos;
				
			}
		}
		
		if( theDraggedJugglerPoiIn){
			if( fabs( theDraggedJugglerPoiIn->poi.x - theDraggedJugglerPoiIn->poiIn.x ) <= DELTA_MOVEMENT &&
			   fabs( theDraggedJugglerPoiIn->poi.y - theDraggedJugglerPoiIn->poiIn.y ) <= DELTA_MOVEMENT)
			{	theDraggedJugglerPoiIn->poiIn = theDraggedJugglerPoiIn->poi;
				
			}
		}
		
		if( theDraggedJugglerPoiOut){
			if( fabs( theDraggedJugglerPoiOut->poi.x - theDraggedJugglerPoiOut->poiOut.x ) <= DELTA_MOVEMENT &&
			   fabs( theDraggedJugglerPoiOut->poi.y - theDraggedJugglerPoiOut->poiOut.y ) <= DELTA_MOVEMENT)
			{ 	theDraggedJugglerPoiOut->poiOut = theDraggedJugglerPoiOut->poi;
				
			}
		}
		
		theDraggedJugglerPos = theDraggedJugglerPosOut = theDraggedJugglerPosIn = NULL;
		theDraggedJugglerPoi = theDraggedJugglerPoiOut = theDraggedJugglerPoiIn = NULL;
		return;
	}
	
	
	theRunaroundModifiers = getModifiers();
	f= getPointerToSelectedField();
	
	if( theRunaroundModifiers == ( GLUT_ACTIVE_ALT | GLUT_ACTIVE_CTRL)){
		rotateRunaroundScene = true;
		rotationStartedNow = true;
		rStartX = x;
		rStartY = y;
		
	}
	

	// get all pointers
	theDraggedJugglerPos = getJugglerWithPosAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	theDraggedJugglerPoi = getJugglerWithPoiAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	if( theDraggedJugglerPoi )
	   tmp = *theDraggedJugglerPoi;
	
	theDraggedJugglerPosIn = getJugglerWithPosInAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	theDraggedJugglerPosOut = getJugglerWithPosOutAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	theDraggedJugglerPoiIn = getJugglerWithPoiInAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	theDraggedJugglerPoiOut = getJugglerWithPoiOutAtScreenCoord( f, mouseX, mouseY, 0,0,w,h);
	
	
	if( theDraggedJugglerPos ){
		switchStartPos = *theDraggedJugglerPos;
	}
	
	
	// deselect the pointers not wanted by the user
	// just the modifiers, not the position
	if( theRunaroundModifiers == GLUT_ACTIVE_SHIFT ){
		theDraggedJugglerPos = theDraggedJugglerPoi = NULL;
	}
	// only outgoing modifiers
	if( theRunaroundModifiers == GLUT_ACTIVE_ALT ){
		theDraggedJugglerPos = theDraggedJugglerPoi = NULL;
		theDraggedJugglerPosIn = theDraggedJugglerPoiIn = NULL;
	}
	// only incomming modifiers
	if( theRunaroundModifiers == GLUT_ACTIVE_CTRL ){
		theDraggedJugglerPos = theDraggedJugglerPoi = NULL;
		theDraggedJugglerPosOut = theDraggedJugglerPoiOut = NULL;
	}
	// if the position itself is selected, ignore modifiers
	if( theDraggedJugglerPos ){
		// move everything with pos
	}else if( theDraggedJugglerPosIn || theDraggedJugglerPosOut ){
		// just move pos modifiers, don´t care for others
		theDraggedJugglerPoiOut = theDraggedJugglerPoiIn = NULL;
		theDraggedJugglerPos = theDraggedJugglerPoi = NULL;
	}else if( theDraggedJugglerPoi ){
		// move poi and it´s modifiers, don´t care pos
		theDraggedJugglerPosOut = theDraggedJugglerPosIn = NULL;
		theDraggedJugglerPoiOut = theDraggedJugglerPoiIn = NULL;
	}else if( theDraggedJugglerPoiIn || theDraggedJugglerPoiOut ){
		// just move pos modifiers, don´t care for others
		theDraggedJugglerPosOut = theDraggedJugglerPosIn = NULL;
		theDraggedJugglerPos = theDraggedJugglerPoi = NULL;
	}	

/*
 _myPrintf("theDraggedJugglerPos    = %08x\n",theDraggedJugglerPos	  );
	_myPrintf("theDraggedJugglerPosIn  = %08x\n",theDraggedJugglerPosIn );
	_myPrintf("theDraggedJugglerPosOut = %08x\n",theDraggedJugglerPosOut);
	_myPrintf("theDraggedJugglerPoi    = %08x\n",theDraggedJugglerPoi   );
	_myPrintf("theDraggedJugglerPoiIn  = %08x\n",theDraggedJugglerPoiIn );
	_myPrintf("theDraggedJugglerPoiOut = %08x\n\n\n",theDraggedJugglerPoiOut );
*/
	glutPostRedisplay();
}

void motion( int x, int y )
{
	_fieldContent *f;
	int w = glutGet( GLUT_WINDOW_WIDTH);
	int h = glutGet( GLUT_WINDOW_HEIGHT);
	
	f = getPointerToSelectedField();
	if( f == NULL )
		return;
	
	if( mouseX == x && mouseY == y )
		return;
	
	mouseX = x;
	mouseY = y;
	
	
	// rotating the whole scene
	if( rotateRunaroundScene ){
		Vector3D a, b, c, d;
		
		float l;
		
		a.x = x; a.y = y; a.z = 0;
		b.x = rStartX; b.y = rStartY; b.z = 0;
		
		c = a-b;
		d = b - f->center;
		l = c * f->center;
		
		if( x > f->center.x)
			l *= -1;
		else if( y < f->center.y)
			l *= -1.f;
		
		_runaroundRotateScene( f, l/100.f);
		
		rStartX = x;
		rStartY = y;
	}
	
	// drag a juggler to a new position
	// switch two juggler positions by dragging one onto the other 
	// 2nd jugglers new position is the one where the drag started
	if(theDraggedJugglerPos )
	{
		_runaroundJuggler *j;

		glutSetWindow( inputID);
		

		
		j = getJugglerWithPosAtScreenCoordExcept( f, theDraggedJugglerPos, mouseX, mouseY, 0,0,w,h);
		if( j )
		{

			if( fabs(j->pos.x - theDraggedJugglerPos->pos.x) < deltaPos &&
			   fabs(j->pos.y - theDraggedJugglerPos->pos.y) < deltaPos){

				// xchange content
				*theDraggedJugglerPos = *j;
				*j = switchStartPos;
				// new pointers
				switchStartPos = *theDraggedJugglerPos;
			}
		}
		if( 01 )
		{
		
			int px,py;
			int dx, dy;
			
			screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
			dx = px - theDraggedJugglerPos->pos.x;
			dy = py - theDraggedJugglerPos->pos.y;
			theDraggedJugglerPos->pos.x = px;
			theDraggedJugglerPos->pos.y = py;
			theDraggedJugglerPos->posOut.x += dx;
			theDraggedJugglerPos->posOut.y += dy;
			theDraggedJugglerPos->posIn.x += dx;
			theDraggedJugglerPos->posIn.y += dy;
			
			theDraggedJugglerPos->poi.x += dx;
			theDraggedJugglerPos->poi.y += dy;
			theDraggedJugglerPos->poiOut.x += dx;
			theDraggedJugglerPos->poiOut.y += dy;
			theDraggedJugglerPos->poiIn.x += dx;
			theDraggedJugglerPos->poiIn.y += dy;

			// do not do anything else, just the drag / position switch
			glutPostRedisplay();
			return;
		}
	}
	
	if( theDraggedJugglerPosIn )
	{
		int px,py;
		
		
		screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
		theDraggedJugglerPosIn->posIn.x = px;
		theDraggedJugglerPosIn->posIn.y = py;
		
		if( theRunaroundModifiers == GLUT_ACTIVE_SHIFT )
		{
			int dx, dy;
			
			
			dx = theDraggedJugglerPosIn->pos.x - theDraggedJugglerPosIn->posIn.x;
			dy = theDraggedJugglerPosIn->pos.y - theDraggedJugglerPosIn->posIn.y;
			
			theDraggedJugglerPosIn->posOut.x = theDraggedJugglerPosIn->pos.x + dx;
			theDraggedJugglerPosIn->posOut.y = theDraggedJugglerPosIn->pos.y + dy;
		}
	}
	
	
	if( theDraggedJugglerPosOut )
	{
		int px,py;
		
		screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
		theDraggedJugglerPosOut->posOut.x = px;
		theDraggedJugglerPosOut->posOut.y = py;
		
		if( theRunaroundModifiers == GLUT_ACTIVE_SHIFT )
		{
			int dx, dy;
			
			dx = theDraggedJugglerPosOut->pos.x - theDraggedJugglerPosOut->posOut.x;
			dy = theDraggedJugglerPosOut->pos.y - theDraggedJugglerPosOut->posOut.y;
			
			theDraggedJugglerPosOut->posIn.x = theDraggedJugglerPosOut->pos.x + dx;
			theDraggedJugglerPosOut->posIn.y = theDraggedJugglerPosOut->pos.y + dy;
		}
		
	}
	
	// move the jugglers poi with all of it´s modifiers
	// NB: don´t allow user to drag poi too close to pos
	
	if(theDraggedJugglerPoi		){
		int px,py;
		int dx, dy;
		float b;
		Vector3D v;
		
		v = theDraggedJugglerPoi->poi - theDraggedJugglerPoi->pos;
		v.z = 0;
		b = v.betrag();
		_myPrintf("%-5.2f\n", b );
		if(1 || b > tooCloseCondition ){
			screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
			dx = px - theDraggedJugglerPoi->poi.x;
			dy = py - theDraggedJugglerPoi->poi.y;
			
			
			theDraggedJugglerPoi->poi.x += dx;
			theDraggedJugglerPoi->poi.y += dy;
			theDraggedJugglerPoi->poiOut.x += dx;
			theDraggedJugglerPoi->poiOut.y += dy;
			theDraggedJugglerPoi->poiIn.x += dx;
			theDraggedJugglerPoi->poiIn.y += dy;
		}
	}

	if( theDraggedJugglerPoiIn )
	{
		int px,py;
		
		
		screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
		theDraggedJugglerPoiIn->poiIn.x = px;
		theDraggedJugglerPoiIn->poiIn.y = py;
		
		if( theRunaroundModifiers == GLUT_ACTIVE_SHIFT )
		{
			int dx, dy;
			
			
			dx = theDraggedJugglerPoiIn->poi.x - theDraggedJugglerPoiIn->poiIn.x;
			dy = theDraggedJugglerPoiIn->poi.y - theDraggedJugglerPoiIn->poiIn.y;
			
			theDraggedJugglerPoiIn->poiOut.x = theDraggedJugglerPoiIn->poi.x + dx;
			theDraggedJugglerPoiIn->poiOut.y = theDraggedJugglerPoiIn->poi.y + dy;
			
		}
		
	}
	
	
	if( theDraggedJugglerPoiOut )
	{
		int px,py;
		
		screenToFieldCoord( mouseX,mouseY,0,0,w,h,&px,&py);
		theDraggedJugglerPoiOut->poiOut.x = px;
		theDraggedJugglerPoiOut->poiOut.y = py;
		
		if( theRunaroundModifiers == GLUT_ACTIVE_SHIFT )
		{
			int dx, dy;
			
			dx = theDraggedJugglerPoiOut->poi.x - theDraggedJugglerPoiOut->poiOut.x;
			dy = theDraggedJugglerPoiOut->poi.y - theDraggedJugglerPoiOut->poiOut.y;
			
			theDraggedJugglerPoiOut->poiIn.x = theDraggedJugglerPoiOut->poi.x + dx;
			theDraggedJugglerPoiOut->poiIn.y = theDraggedJugglerPoiOut->poi.y + dy;
			
			
		}
		
	}
	
	glutPostRedisplay();
}

void passiveMotion( int x, int y )
{
	mouseX = x;
	mouseY = y;
}


void reshape(int w, int h)
{
	
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void inputDraw( void )
{
	_fieldContent *f;
	int w;
	int h;
	
	
	if( inputID == 0)
		return;
	
	
	w = glutGet( GLUT_WINDOW_WIDTH);
	h = glutGet( GLUT_WINDOW_HEIGHT);
	
	
	glClearColor( 1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setColor( 0.6f,0.6f,0.6f);
	f = getPointerToSelectedField();
	
	if( drawFrameInRunaroundEditorFlag ||1)
		drawFrameNowFlag = true;
	
	if( f != NULL )
		drawContentInRect( f, 0,0,w,h);
	
	
	if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
		glutSwapBuffers();
	else
		glFlush();
	
}

// draw the floor grid centered on the stage
void drawGrid(int x, int y, int w, int h)
{
#pragma unused (w,h)
	
    float c;
	
	
	
	
	
	if( !showGridFlag)
		return;
	
	glPushMatrix();
	
	
	for(c = -x/2; c <= x/2; c += 100 )
		drawLine(c,-y/2, c, y/2);
	
	for(c = -y/2; c <= y/2; c += 100 )
		drawLine(-x/2, c, x/2,c);
	
	glPopMatrix();
}

// draw a frame around the graphics output

void drawFrame( int x, int y, int w, int h )
{
#pragma unused( x, y )
	punkt firstP, thisP, nextP;
	int c;
	float a;
	
	/*	if( !drawFrameNowFlag )
	 return;
	 */	
	if( !showFrameFlag)
        return;
	
	glPushMatrix();
	
	glTranslatef(w/2,h/2,0);
	
	glScalef(w / (metersOnScreen * 100), h / (metersOnScreen * 100),1.f);
	
    setColor( 0.3f,0.3f,0.3f);
	

	firstP = punkt(250,0,0).drehungZ(startingAngleOfFrame);
	
	for( c = 0; c < cornersInRunaroundView - 1; c++ )
	{
		
		a = 360.f / cornersInRunaroundView * c;
		thisP = firstP.drehungZ(a);
		
		a = 360.f / cornersInRunaroundView * (c + 1);
		nextP = firstP.drehungZ(a);
		
		drawLine( thisP.x, thisP.y, nextP.x, nextP.y);
		
	}
	
	// close the polygone
	drawLine( firstP.x, firstP.y, nextP.x, nextP.y);
	
	
    glPopMatrix();
	
    drawFrameNowFlag = false;
	
	
}



/*	draws the content of the named field in a given rect.
	this routine is called by both the input window and the list window.
*/
void drawContentInRect( _fieldContent *f,  int x, int y, int w, int h)
{
	int c;
	_runaroundJuggler *j;
	_runaroundJuggler *thePos, *thePoi, *thePosOut, *thePosIn, *thePoiOut, *thePoiIn;
	GLfloat col[4], lw;
	sysColor bodyColor;
	float a, b;
	char s[1000];
	int i;
	
	
	if( !f )
		return;
	
	// save size;
	f->center.x = x + w/2.f;
	f->center.y = y + h/2.f;
	f->center.z = 0.f;
	
	// save color
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat *)col);
	
	// translate and scale to desired pos / size
	glPushMatrix();
	/*      sprintf(s, "%d %d", thisX, thisY);
	 runaroundTextOutput( 10,10, 1.f, s );
	 */
	glTranslatef(x,y,0);
	
	// scale a bit down;
	glTranslatef(w*(1-scaleInputScreenBy)*.5f, h*(1-scaleInputScreenBy)*.5f,0.f);
	glScalef(scaleInputScreenBy,scaleInputScreenBy,scaleInputScreenBy);
	
	drawFrame(0,0,w,h);
	
	// shift positions to center
	glTranslatef( w/2, h/2, 0);
	glScalef( w / metersOnScreen /100 , h / metersOnScreen / 100, 1);
	
	
	setColor( 0.6f,0.6f,0.6f);
	if( grid2Parameters )
		drawGrid(gridX*100, gridY*100, gridX * 100, gridY * 100);
	else
		drawGrid(metersOnScreen*100,metersOnScreen*100,0,0);
	
	glGetFloatv(GL_LINE_WIDTH, &lw);
	glLineWidth( 2);
	
	if( runaroundShowSceneIndexFlag ){
		i = getIndexFromFieldPointer(fieldList, f);
		sprintf(s, "%3d", i);
		glPushMatrix();
		setOpenGLPosition( 0, 0);
		setColor( 0.f,0.f,0.f);
		glScalef( 2.f,2.f,2.f);
		runaroundTextOutput(-.3f,-.3f, 1.f, s);
		glPopMatrix();
	}
	
	
	thePos		= getJugglerWithPosAtScreenCoord( f, mouseX, mouseY, x,y,w,h);
	thePosOut	= getJugglerWithPosOutAtScreenCoord( f, mouseX, mouseY, x,y,w,h);
	thePosIn	= getJugglerWithPosInAtScreenCoord(  f, mouseX, mouseY, x,y,w,h);
	thePoi		= getJugglerWithPoiAtScreenCoord( f, mouseX, mouseY, x,y,w,h);
	thePoiOut	= getJugglerWithPoiOutAtScreenCoord( f, mouseX, mouseY, x,y,w,h);
	thePoiIn	= getJugglerWithPoiInAtScreenCoord(  f, mouseX, mouseY, x,y,w,h);
	
	
	for( c = 0; c < f->c; c++)
	{
		
		j = getRunaroundJuggler( f, c);
		
#if 01
		sprintf(s, "%d", c+1);
#else
		sprintf(s, "%d@(%3.2f/%3.2f)", c, j->pos.x , j->pos.y);
#endif
		
		
		// color
		bodyColor = getListColor( c );
		
		
		// pos
		setColor(bodyColor.x,bodyColor.y,bodyColor.z);
		if(thePos && j == thePos )
			setColor(0,0,0);
		
		
        glPushMatrix();
		setOpenGLPosition( j->pos.x, j->pos.y);
		drawOpenGLDisk( 20);
		runaroundTextOutput( -.3f, -.3f, 2.f, s);
		glPopMatrix();
		
		//poi
		setColor(bodyColor.x,bodyColor.y,bodyColor.z);
		if(thePoi && j == thePoi )
			setColor(0,0,0);
        glPushMatrix();
		setOpenGLPosition( j->poi.x, j->poi.y);
		drawOpenGLFilledDisk( 5);
		glPopMatrix();
		
		
		
		// current pos path
		if( jpPrefs.showJugglerPos)
		{
			punkt p1, p2;
			float t;
			int i, l, n;
			_fieldContent *lastPos, *nextPos;
			_runaroundJuggler *lastJ, *nextJ;
			punkt in, out;
			
			
			i = getIndexFromFieldPointer (fieldList, f );
			if( i == 0 )
				l = totalFields-1;
			else
				l = i - 1;
			
			if( i == totalFields-1)
				n = 0;
			else
				n = i + 1;
			
			
			lastPos = getField( fieldList, l);
			nextPos = getField( fieldList, n);
			
			lastJ = getRunaroundJuggler( lastPos, c);
			nextJ = getRunaroundJuggler( nextPos, c);
			
			
			glLineWidth( 2.f);
#define PATH_STEP       .01f
			for( t = 0; t < 1; t += PATH_STEP)
			{
				// last to now
				out  = makeOutgoingVector( lastJ->pos, lastJ->posOut );
				in = makeIncommingVector(  j->pos, j->posIn );
				
				
				p1 = getPointFromHermite( lastJ->pos, j->pos, out, in, t);
				p2 = getPointFromHermite( lastJ->pos, j->pos, out, in, t + PATH_STEP);
				
				drawOneLine2D(p1.x, p1.y, p2.x, p2.y);
				
				// now to next
				out  = makeOutgoingVector( j->pos, j->posOut );
				in = makeIncommingVector( nextJ->pos, nextJ->posIn );
				
				
				p1 = getPointFromHermite( j->pos, nextJ->pos, out, in, t);
				p2 = getPointFromHermite( j->pos, nextJ->pos, out, in, t + PATH_STEP);
				
				drawOneLine2D(p1.x, p1.y, p2.x, p2.y);
				
			}
		}
		// current poi path
		if( jpPrefs.showJugglerPoi){
			punkt p1, p2;
			float t;
			int i, l, n;
			_fieldContent *lastPos, *nextPos;
			_runaroundJuggler *lastJ, *nextJ;
			punkt in, out;
			
			
			i = getIndexFromFieldPointer (fieldList, f );
			if( i == 0 )
				l = totalFields-1;
			else
				l = i - 1;
			
			if( i == totalFields-1)
				n = 0;
			else
				n = i + 1;
			
			
			lastPos = getField( fieldList, l);
			nextPos = getField( fieldList, n);
			
			lastJ = getRunaroundJuggler( lastPos, c);
			nextJ = getRunaroundJuggler( nextPos, c);
			
			glLineWidth( 1.f);
			for( t = 0; t < 1; t += PATH_STEP)
			{	
				static bool  drawLine = false;
				
				// last to now
				out  = makeOutgoingVector( lastJ->poi, lastJ->poiOut );
				in = makeIncommingVector(  j->poi, j->poiIn );
				
				
				p1 = getPointFromHermite( lastJ->poi, j->poi, out, in, t);
				p2 = getPointFromHermite( lastJ->poi, j->poi, out, in, t + PATH_STEP);
				
				if( drawLine )
					drawOneLine2D(p1.x, p1.y, p2.x, p2.y);
				// now to next
				
				out  = makeOutgoingVector( j->poi, j->poiOut );
				in = makeIncommingVector( nextJ->poi, nextJ->poiIn );
				
				
				p1 = getPointFromHermite( j->poi, nextJ->poi, out, in, t);
				p2 = getPointFromHermite( j->poi, nextJ->poi, out, in, t + PATH_STEP);
				
				if( drawLine )
					drawOneLine2D(p1.x, p1.y, p2.x, p2.y);
				
				toggle( drawLine);
				
			}
			
			// arrowhead
			if ( 0 )
			{
				punkt p, t;
				Vector3D d;
				
				// now to next
				out  = makeOutgoingVector( j->pos, j->posOut );
				in = makeIncommingVector( nextJ->pos, nextJ->posIn );
				
				p = getPointFromHermite( j->pos, nextJ->pos, out, in, 0.5f);
				t = getPointFromHermite( j->pos, nextJ->pos, out, in, 0.5f + PATH_STEP);
				
				d = t - p;
				_myPrintf("|d| = %f\n", d.betrag());
				if( d.betrag() > 1 )
				{
					
					setColor(bodyColor.x,bodyColor.y,bodyColor.z);
					glPushMatrix();
					setOpenGLPosition( p.x,  p.y);
					drawOpenGLFilledDisk( 15);
					glPopMatrix();
					
					
				}
			}
			
		}
		
		
		
		if( drawModifiersInRunaroundEditorFlag )
		{
			//posIn
			if( fabs( j->pos.x - j->posIn.x ) > DELTA_MOVEMENT ||
			   fabs( j->pos.y - j->posIn.y ) > DELTA_MOVEMENT)
			{
				punkt a,m,b;
				punkt vX = punkt(1,0,0);
				float angle;
				
				// in dir
				setColor(1.f,0.8f,0.8f);
				drawOneLine2D(j->pos.x, j->pos.y, j->posIn.x, j->posIn.y);
				
				
				// draw arrowhead
				setColor(bodyColor.x,bodyColor.y,bodyColor.z);
				if(thePosIn && j == thePosIn )
					setColor(0,0,0);
				
				glPushMatrix();
				
				setOpenGLPosition( j->posIn.x, j->posIn.y);
				
				m = j->pos;
				a = j->posIn;
				b = j->pos + vX;
				
				angle = angleBetween3Points( a,m,b);
				
				glRotatef( angle + 180, 0.f, 0.f, 1.f);
				drawTriangle( 2.f);
				
				glPopMatrix();
				
				
			}
			
			//posOut
			if( fabs( j->pos.x - j->posOut.x ) > DELTA_MOVEMENT ||
			   fabs( j->pos.y - j->posOut.y ) > DELTA_MOVEMENT)
			{
				punkt a,m,b;
				punkt vX = punkt(1,0,0);
				float angle;
				
				// out dir
				setColor(1.f,0.8f,0.8f);
				drawOneLine2D(j->pos.x, j->pos.y, j->posOut.x, j->posOut.y);
				
				// draw arrowhead
				setColor(bodyColor.x,bodyColor.y,bodyColor.z);
				
				if(thePosOut && j == thePosOut )
					setColor(0,0,0);
				
				glPushMatrix();
				
				setOpenGLPosition( j->posOut.x, j->posOut.y);
				m = j->pos;
				a = j->posOut;
				b = j->pos + vX;
				
				angle = angleBetween3Points( a,m,b);
				
				glRotatef( angle, 0.f, 0.f, 1.f);
				drawTriangle( 2.f);
				
				glPopMatrix();
				
			}
			
#if 01
			//poiIn
			a = (float)fabs( j->poi.x - j->poiIn.x );
			b = (float)fabs( j->poi.y - j->poiIn.y );
			if( a > DELTA_MOVEMENT || b  > DELTA_MOVEMENT)
			{
				punkt a,m,b;
				punkt vX = punkt(1,0,0);
				float angle;
				
				// in dir
				setColor(1.f,0.8f,0.8f);
				drawOneLine2D(j->poi.x, j->poi.y, j->poiIn.x, j->poiIn.y);
				
				
				// draw arrowhead
				setColor(bodyColor.x,bodyColor.y,bodyColor.z);
				if(thePoiIn && j == thePoiIn )
					setColor(0,0,0);
				
				glPushMatrix();
				
				setOpenGLPosition( j->poiIn.x, j->poiIn.y);
				
				m = j->poi;
				a = j->poiIn;
				b = j->poi + vX;
				
				angle = angleBetween3Points( a,m,b);
				
				glRotatef( angle + 180, 0.f, 0.f, 1.f);
				drawTriangle( 2.f);
				
				glPopMatrix();
				
				
			}
			
			//poiOut
			if( fabs( j->poi.x - j->poiOut.x ) > DELTA_MOVEMENT ||
			   fabs( j->poi.y - j->poiOut.y ) > DELTA_MOVEMENT)
			{
				punkt a,m,b;
				punkt vX = punkt(1,0,0);
				float angle;
				
				// out dir
				setColor(1.f,0.8f,0.8f);
				drawOneLine2D(j->poi.x, j->poi.y, j->poiOut.x, j->poiOut.y);
				
				// draw arrowhead
				setColor(bodyColor.x,bodyColor.y,bodyColor.z);
				
				if(thePosOut && j == thePosOut )
					setColor(0,0,0);
				
				glPushMatrix();
				
				setOpenGLPosition( j->poiOut.x, j->poiOut.y);
				m = j->poi;
				a = j->poiOut;
				b = j->poi + vX;
				
				angle = angleBetween3Points( a,m,b);
				
				glRotatef( angle, 0.f, 0.f, 1.f);
				drawTriangle( 2.f);
				
				glPopMatrix();
				
			}
#endif
			
		}
		//                      showPathModifiersFlag = false;
		// view dir
		setColor(1.f,0.8f,0.8f);
		drawOneLine2D(j->pos.x, j->pos.y, j->poi.x, j->poi.y);
		
	}
	glPopMatrix();
	
	
	glLineWidth( lw );
	setColor(col[0], col[1], col[2]);
	
}

// get a juggler with it´s position at given coordinates, but not the one juggler specified

_runaroundJuggler *getJugglerWithPosAtScreenCoordExcept(_fieldContent *f, _runaroundJuggler *ex, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->pos.x ) < deltaPos && fabs( fy - p->pos.y ) < deltaPos && p != ex)
			r = p;
	}
	
	return r;
	
}

// get a juggler  with it´s position at given coordinates
_runaroundJuggler *getJugglerWithPosAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->pos.x ) < deltaPos && fabs( fy - p->pos.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
//	return getJugglerWithPosAtScreenCoordExcept( f, NULL, px, py, x, y, w, h);
}

// get a juggler  with it´s position: outgoing movement modifier - at given coordinates
_runaroundJuggler *getJugglerWithPosOutAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->posOut.x ) < deltaPos && fabs( fy - p->posOut.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
}

// get a juggler  with it´s position: incoming movement modifier - at given coordinates

_runaroundJuggler *getJugglerWithPosInAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->posIn.x ) < deltaPos && fabs( fy - p->posIn.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
}


// get a juggler  with it´s point of interestat given coordinates
_runaroundJuggler *getJugglerWithPoiAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->poi.x ) < deltaPos && fabs( fy - p->poi.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
}

// get a juggler  with it´s point of interest: outgoing poi modifier - at given coordinates
_runaroundJuggler *getJugglerWithPoiOutAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->poiOut.x ) < deltaPos && fabs( fy - p->poiOut.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
}

// get a juggler  with it´s point of interest: incoming poi modifier - at given coordinates
_runaroundJuggler *getJugglerWithPoiInAtScreenCoord(_fieldContent *f, int px, int py, int x, int y, int w, int h)
{
	_runaroundJuggler *r = NULL, *p;
	int c;
	int fx, fy;
	
	
	for( c = 0; c < f->c; c++ )
	{
		p = getRunaroundJuggler(f,c);
		screenToFieldCoord(px,py,x,y,w,h, &fx, &fy);
		if( fabs(fx - p->poiIn.x ) < deltaPos && fabs( fy - p->poiIn.y ) < deltaPos )
			r = p;
	}
	
	return r;
	
}

/*	takes the relative ccordinates of a field 
	returns the global coordinates of a field 
*/
void fieldToScreenCoord( int inX, int inY, int x, int y, int w, int h, int *fx, int *fy)
{
	float px, py;
	
	
	px = inX;
//	py = h - inY;
	py = h - inY;
	
	px -= x;
	px *= w;
	px += 1*(1+scaleInputScreenBy)*.5f;
	px *= scaleInputScreenBy;
	px -= .5f;
	px /= metersOnScreen * 100;
	px += w/2;
	
	py += y;
	py *= h;
	py += 1*(1+scaleInputScreenBy)*.5f;
	py *= scaleInputScreenBy;
	py -= .5f;
	py /= metersOnScreen * 100;
	
	*fx = (int)px;
	*fy = (int)py;
}


/*	takes the global coordinates of a field
	returns the relative ccordinates of a field 
*/
void screenToFieldCoord( int inX, int inY, int x, int y, int w, int h, int *fx, int *fy)
{
	float px, py;
	
	px = inX;
	py = h-inY;
	
	px += x;
	px /= w;
	px -= 1*(1+scaleInputScreenBy)*.5f;
	px /= scaleInputScreenBy;
	px += .5f;
	px *= metersOnScreen * 100;
	
	py -= y;
	py /= h;
	py -= 1*(1+scaleInputScreenBy)*.5f;
	py /= scaleInputScreenBy;
	py += .5f;
	py *= metersOnScreen * 100;
	
	*fx = (int)px;
	*fy = (int)py;
}


// drawing abstractors
void drawLine( float x1, float y1, float x2, float y2 )
{
    drawOneLine2D( x1, y1, x2, y2);
}


void setOpenGLPosition( float x, float y)
{
	
	
	/*      drawPositionX = x;
	 drawPositionY = y;
	 */
	glTranslatef( x,y,0);
}

void drawOpenGLDisk( float scale )
{
    GLUquadricObj *theQuadric = gluNewQuadric();
	
    glPushMatrix();
    glScalef( scale, scale, scale );
    gluDisk( theQuadric, .9, 1., 20, 20);
    glPopMatrix();
	
    gluDeleteQuadric( theQuadric);
	
}
void drawOpenGLFilledDisk( float scale )
{
    GLUquadricObj *theQuadric = gluNewQuadric();
	
    glPushMatrix();
    glScalef( scale, scale, scale );
    gluDisk( theQuadric, 0, 1., 20, 20);
    glPopMatrix();
	
    gluDeleteQuadric( theQuadric);
	
}


void drawTriangle( float scale )
{
	
    glPushMatrix();
	
	
	
    glScalef( scale, scale, scale );
	
	//glRotatef( 0, 0,0,1);
	
	//      glTranslatef(0,-5,0);
	
	glBegin(GL_QUADS);
	glVertex2f(   3,  0 );
	glVertex2f(   0, 5 );
	glVertex2f(  10,  0 );
	glVertex2f(   0,  -5 );
	glEnd();
	
    glPopMatrix();
	
}


/*
 calculates angle between 2 vectors, given by three points
 
 input: a, m, b (points)
 with
 v1 = ma
 v2 = mb
 
 output
 angle between vectors
 
 
 */
float angleBetween3Points( punkt a, punkt m, punkt b)
{
	punkt v1 = a - m;
	punkt v2 = b - m;
	
	float t, t1, t2;
	float angle;
	
	t1 = v1 * v2;
	t2 = v1.betrag() * v2.betrag ();
	
	t = t1 / t2;
	
	angle = radToGrad( (float)acos( t ));
	
	if( a.y < b.y )
		angle *= -1.f;
	
	return angle;
}



/*
 set juggler default positions
 see setJugglers() in parseSiteswap.cpp
 motion() in inputWindow.cpp
 
 
 */


#define POSITION_OFFSET 150

void runaroundLinePosition( int d){
	int c, jc;
	_fieldContent *f;
	_runaroundJuggler *j;
	
	if( (f = getPointerToSelectedField()) == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		j = getRunaroundJuggler(f, c );
		j->pos = Vector3D(POSITION_OFFSET*(c-(jc-1)/2.f),0,0);					
		j->poi = j->pos + Vector3D(-100,0,0);
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}

void runaroundDropbackLinePosition( int d){
	int c, jc;
	_fieldContent *f;
	_runaroundJuggler *j;
	
	if( (f = getPointerToSelectedField()) == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		j = getRunaroundJuggler(f, c );
		j->pos = Vector3D(POSITION_OFFSET*(c-(jc-1)/2.),0,0);					
		if( c == 0){
			j->pos = j->pos + Vector3D(-100,0,0);
			j->poi = j->pos + Vector3D(100,0,0);
		}else
			j->poi = j->pos + Vector3D(-100,0,0);
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}
void runaroundSideToSidePosition( int d){
	int c, jc;
	_fieldContent *f;
	_runaroundJuggler *j;
	
	if( (f = getPointerToSelectedField()) == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		j = getRunaroundJuggler(f, c );
		j->pos = Vector3D(POSITION_OFFSET*(c-(jc-1)/2.),0,0);					
		j->poi = j->pos + Vector3D(0,-100,0);
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}

void runaroundFeedPosition( int d ){
	int c, jc;
	int o;
	_fieldContent *f;
	_runaroundJuggler *j;
	
	if( (f = getPointerToSelectedField()) == NULL )
		return;
	
	jc = f->c;
	
	o = (jc/2 == jc/2.?-POSITION_OFFSET/2:0);
	
	
	for( c = 0; c < jc; c++ ){       
		bool evenFlag = c/2==c/2.;
		int x0;
		j = getRunaroundJuggler(f, c );
		x0 = (c+1)/2*(evenFlag?-POSITION_OFFSET:POSITION_OFFSET) 
		+ o 
		+ jc*POSITION_OFFSET/4*(evenFlag?1:-1)
		- (jc/2==jc/2.? 0 : POSITION_OFFSET/4);
		if( evenFlag)
			x0 *= -1;
		j->pos = Vector3D(x0,evenFlag?200:-200,0);
		j->poi = j->pos + Vector3D(0,evenFlag?-100:100, 0);
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}

void runaroundTerrorFeedPosition( int d ){
	int c, jc;
	_fieldContent *f;
	_runaroundJuggler *j;
	
	if((f = getPointerToSelectedField()) == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		j = getRunaroundJuggler(f, c );
		if( c == 0 ){
			j->pos = Vector3D(0,-150,0);
			j->poi = j->pos + Vector3D(0,100,0);
		}else{
			j->pos = Vector3D(POSITION_OFFSET*(c-1) - POSITION_OFFSET*(jc-2)/2. ,150,0);
			j->poi = j->pos + Vector3D(0,-100,0);
		}
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}



void runaroundCirclePosition( int dummy ){
#pragma unused(dummy)
	_fieldContent *f;
	
	if( (f = getPointerToSelectedField()) )
		_runaroundCirclePosition( f );
}

void runaroundBackToBackPosition( int dummy ){
#pragma unused(dummy)
	_fieldContent *f;
	
	if( (f = getPointerToSelectedField()) )
		_runaroundBackToBackPosition( f );
	
	setRunaroundPatternChanged( true );
}

void _runaroundCirclePosition( _fieldContent *f ){
#pragma unused(dummy)
	
	int c, jc;
	_runaroundJuggler *j;
	punkt tP, tN, t;
	
	if( f == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		punkt jP;
		
		j = getRunaroundJuggler(f, c );
		
		// for historical reasons, 
		// animation uses xz for positon(y for height)
		// runaround editor uses xy
		// => remap position
		
		if( jc == 1){
			tP = jN;
			tN = Vector3D(0,100,0);
		}else{
			tP = pGroupPositions;
			tN = pN;
		}
		jP.x = tP.x;
		jP.y = tP.z;
		jP.z = tP.y;
		tN.x = tN.x;
		tN.y = tN.z;
		tN.z = tN.y;
		
		j->pos = jP.drehungZ((c * 360.f * jugglerPositionFactor) / jc);
		j->poi = jN.drehungZ((c * 360.f * jugglerPositionFactor) / jc);
		t = j->poi-j->pos;
		j->poi = j->pos + t.normalised() * 100;
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
	
	setRunaroundPatternChanged( true );
}


void _runaroundBackToBackPosition( _fieldContent *f ){
#pragma unused(dummy)
	
	int c, jc;
	_runaroundJuggler *j;
	punkt tP, tN, t;
	
	if( f == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		punkt jP;
		
		j = getRunaroundJuggler(f, c );
		
		// for historical reasons, 
		// animation uses xz for positon(y for height)
		// runaround editor uses xy
		// => remap position
		
		if( jc == 1){
			tP = jN;
			tN = Vector3D(0,100,0);
		}else{
			tP = Vector3D(0,0,-10*jc);
			tN = pN;
		}
		jP.x = tP.x;
		jP.y = tP.z;
		jP.z = tP.y;
		tN.x = tN.x;
		tN.y = tN.z;
		tN.z = tN.y;
		
		j->pos = jP.drehungZ((c * 360.f * jugglerPositionFactor) / jc);
		j->poi = jN.drehungZ((c * 360.f * jugglerPositionFactor) / jc);
		t = j->poi-j->pos;
		j->poi = j->pos - t.normalised() * 100;
		j->posIn = j->posOut = j->pos;
		j->poiIn = j->poiOut = j->poi;
		
	}
}




		
/*
 as we sumilate radio buttons for the selection menu,
 the mwnu routines can`t handle the automated de/activation of the entries.
 so we set the marker flags manually here.
*/
void runaroundRotateCurrentScene(int dummy){
#pragma unused(dummy)
	
	runaroundRotateCurrentSceneFlag = true;
	runaroundRotateCurrentLineFlag = false;
	_runaroundRotateScene2 = false;
	reCreateMenu( listID, runaroundListMenuList);
	reCreateMenu( inputID, runaroundListMenuList);

	return;
}
void runaroundRotateCurrentLine(int dummy){
#pragma unused(dummy)
	
	runaroundRotateCurrentSceneFlag = false;
	runaroundRotateCurrentLineFlag = true;
	_runaroundRotateScene2 = false;
	reCreateMenu( listID, runaroundListMenuList);
	reCreateMenu( inputID, runaroundListMenuList);
	
	return;
}
void runaroundRotateAllScenes(int dummy){
#pragma unused(dummy)
	
	runaroundRotateCurrentSceneFlag = false;
	runaroundRotateCurrentLineFlag = false;
	_runaroundRotateScene2 = true;
	reCreateMenu( listID, runaroundListMenuList);
	reCreateMenu( inputID, runaroundListMenuList);
	
	return;
}

#if 0

/*	rotate the whole content by a given amount of degrees
 */
#define ROTATION_DEGREES	1
void runaroundRotateSceneR(int dummy){
#pragma unused(dummy)
	_fieldContent *f;
	
	
	if( runaroundRotateAllScenesTogetherFlag = false ){
		if( (f = getPointerToSelectedField()) == NULL)
			return;
		
		_runaroundRotateScene(f, ROTATION_DEGREES);
	}else {
		int c;
		
		for( c = 0; c < totalFields; c++ ){
			f = getField( fieldList, c);
			
			_runaroundRotateScene(f, ROTATION_DEGREES);
		}
	}
	
	return;
}


void runaroundRotateSceneL(int dummy){
#pragma unused(dummy)
	_fieldContent *f;
	
	if( runaroundRotateAllScenesTogetherFlag = false ){
		if( (f = getPointerToSelectedField()) == NULL)
			return;
	
		_runaroundRotateScene(f, -ROTATION_DEGREES);
	}else {
		int c;
		
		for( c = 0; c < totalFields; c++ ){
			f = getField( fieldList, c);
			
			_runaroundRotateScene(f, -ROTATION_DEGREES);
		}
	}
	return;
}
#else

/*	rotate the whole content by a given amount of degrees
 */
#define ROTATION_DEGREES	1
void runaroundRotateSceneR(int dummy){
#pragma unused(dummy)
	_fieldContent *f;
	bool found = false;	
	int c;
		
	for( c = 0; c < totalFields; c++ ){
		f = getField( fieldList, c);
		
		if( f->selected ){
			_runaroundRotateScene(f, ROTATION_DEGREES);
			found = true;
		}
	}
	if( !found ){
		c = getIndexOfSelectedField();	
		f = getField( fieldList, c);
		_runaroundRotateScene(f, ROTATION_DEGREES);
	}
	return;
}


void runaroundRotateSceneL(int dummy){
#pragma unused(dummy)
	_fieldContent *f;
	bool found = false;	
	int c;
	
	for( c = 0; c < totalFields; c++ ){
		f = getField( fieldList, c);
		
		if( f->selected ){
			_runaroundRotateScene(f, -ROTATION_DEGREES);
			found = true;
		}
	}
	if( !found ){
		c = getIndexOfSelectedField();	
		f = getField( fieldList, c);
		_runaroundRotateScene(f, -ROTATION_DEGREES);
	}
	return;
}

#endif


void _runaroundRotateScene(	_fieldContent *f, int degrees ){
#pragma unused(dummy)
	
	int c, jc;
	_runaroundJuggler *j;
	
	if( f == NULL )
		return;
	
	jc = f->c;
	
	for( c = 0; c < jc; c++ ){       
		j = getRunaroundJuggler(f, c );
		
		// for historical reasons, 
		// animation uses xz for positon(y for height)
		// runaround editor uses xy
		// => remap position
		j->pos = j->pos.drehungZ(degrees);
		j->posIn = j->posIn.drehungZ(degrees);
		j->posOut = j->posOut.drehungZ(degrees);
		j->poi = j->poi.drehungZ(degrees);
		j->poiIn = j->poiIn.drehungZ(degrees);
		j->poiOut = j->poiOut.drehungZ(degrees);
		
	}
	setRunaroundPatternChanged( true );
	
}


/* ---------------------------- */


void lessCornersInFrame( int dummy )
{
#pragma unused (dummy )
	
	if( cornersInRunaroundView > 2)
		cornersInRunaroundView--;
	setRunaroundPatternChanged( true );
}


void exchange( int dummy )
{
#pragma unused (dummy)
	int a, b;
	_fieldContent *f = getPointerToSelectedField();
	int c;
	
	
	if( f == 0 )
		return;
	
	c = f->c;
	
	_myPrintf("enter jugglers to exchange positions\n");
	
	scanf("%d %d ", &a, &b);
	
	printf("ok\n");
	glutPostRedisplay();
	setRunaroundPatternChanged( true );
	
}

void bigger( int dummy )
{
#pragma unused (dummy )
	
	metersOnScreen *= RUNAROUND_SCALE_FACTOR;
	setRunaroundPatternChanged( true );
}

void smaller( int dummy )
{
#pragma unused (dummy )
	
	metersOnScreen /= RUNAROUND_SCALE_FACTOR;
	setRunaroundPatternChanged( true );
}

void showFrame( int dummy )
{
#pragma unused (dummy )
	
	runaroundShowFrameFlag = showFrameFlag;
	setRunaroundPatternChanged( true );
}

void showGrid( int dummy )
{
#pragma unused (dummy )
	
	jpPrefs.showFloorGrid = showGridFlag;
	setRunaroundPatternChanged( true );
}

void toggleSceneIndex( int dummy )
{
#pragma unused (dummy )
	
	runaroundShowSceneIndexFlag = showSceneIndexFlag;
	setRunaroundPatternChanged( true );
}
void moreCornersInFrame( int dummy )
{
#pragma unused (dummy )
	
	cornersInRunaroundView++;
	setRunaroundPatternChanged( true );
}

void rotateFrameClockwise( int dummy )
{
#pragma unused (dummy )
	
	startingAngleOfFrame -= 1;
	setRunaroundPatternChanged( true );
}

void rotateFrameCounterclockwise( int dummy )
{
#pragma unused (dummy )
	
	startingAngleOfFrame += 1;
	setRunaroundPatternChanged( true );
}

	
void showModifiers( int dummy )
{
#pragma unused (dummy )
	
	toggle( drawModifiersInRunaroundEditorFlag );
}

// call the functions from the animation window to make the switches work the same in both windows
void showPos( int dummy )
{
	jpPrefs.showJugglerPos = showPosFlag;
//	toggleShowPosFunc( dummy );
	redrawRunaroundWindows();
}

void showPoi( int dummy )
{
#pragma unused (dummy )
	jpPrefs.showJugglerPoi = showPoiFlag;
//	toggleShowPoiFunc( dummy);
	redrawRunaroundWindows();
}

void cycleCCW( int dummy )
{
#pragma unused (dummy)
	_fieldContent *s, *d, tmp;
	int c;
	int *nlBuf;
	
	//preserve newlines
	nlBuf = (int *)malloc( totalFields);
	for( c = 0; c < totalFields-1; c++ ){
		s = getField( fieldList, c );
		nlBuf[c] = s->showInNewLine;
	}
	
	// nothing to do
	if( totalFields == 1)
		return;
	
	// add an extra position at end
	_runaroundAddPosition();
	
	// copy position 0 to extra position
	d = getField( fieldList, totalFields-1 );
	s = getField( fieldList, 0 );
	copyFieldToField(d, s);
	
	// shift all positions one place down
	// including the old nr 0
	for( c = 0; c < totalFields-1; c++ ){
		
		d = getField( fieldList, c );
		s = getField( fieldList, c+1 );
		copyFieldToField(d, s);
		
	}
	
	// remove extra position
	_runaroundRemovePositionAtEnd();
	
	//restore newlines
	for( c = 0; c < totalFields-1; c++ ){
		s = getField( fieldList, c );
		s->showInNewLine = nlBuf[c];
	}
	
	free( nlBuf );
	
	setRunaroundPatternChanged( true );
}

void cycleCW( int dummy )
{
#pragma unused (dummy)
	_fieldContent *s, *d, tmp;
	int c;
	int *nlBuf;
	
	//preserve newlines
	nlBuf = (int *)malloc( totalFields);
	for( c = 0; c < totalFields-1; c++ ){
		s = getField( fieldList, c );
		nlBuf[c] = s->showInNewLine;
	}
	
	// nothing to do
	if( totalFields == 1)
		return;
	
	// add an extra position at end
	_runaroundAddPosition();
	
	
	// shift all positions one place up
	for( c = totalFields-1; c > 0 ; c-- ){
		
		d = getField( fieldList, c );
		s = getField( fieldList, c-1 );
		copyFieldToField(d, s);
		
	}
	
	// copy last position to position 0
	d = getField( fieldList, 0 );
	s = getField( fieldList, totalFields-1 );
	copyFieldToField(d, s);
	
	// remove extra position
	_runaroundRemovePositionAtEnd();
	
	//restore newlines
	for( c = 0; c < totalFields-1; c++ ){
		s = getField( fieldList, c );
		s->showInNewLine = nlBuf[c];
	}
	
	free( nlBuf );
	
	setRunaroundPatternChanged( true );
}






void horizontal( int dummy )
{
#pragma unused (dummy )
	
	_fieldContent *theField;
	int c;
	_runaroundJuggler *j;
	
	
	if( (theField = getPointerToSelectedField()) == 0 )
		return;
	
	
	for( c = 0; c < theField->c; c++ )
	{
		j = getRunaroundJuggler(theField, c );
		j->pos.y *= -1.f;
		j->posIn.y  *= -1.f;
		j->posOut.y *= -1.f;
		j->poi.y *= -1.f;
		j->poiIn.y  *= -1.f;
		j->poiOut.y *= -1.f;
		
	}
	setRunaroundPatternChanged( true );
	
}

void vertical( int dummy )
{
#pragma unused (dummy )
	
	_fieldContent *theField;
	int c;
	_runaroundJuggler *j;
	
	
	if( (theField = getPointerToSelectedField()) == 0 )
		return;
	
	
	for( c = 0; c < theField->c; c++ )
	{
		j = getRunaroundJuggler( theField, c );
		j->pos.x *= -1.f;
		j->posIn.x *= -1.f;
		j->posOut.x *= -1.f;
		j->poi.x *= -1.f;
		j->poiIn.x *= -1.f;
		j->poiOut.x *= -1.f;
		
	}
	setRunaroundPatternChanged( true );
	
}


void editPatternInCDEW( int dummy )
{
#pragma unused (dummy )
	
	_editPatternInCDEW();
	
	
}
void editPatternInTE( int dummy )
{
#pragma unused (dummy )
	
	_editPatternInTE();
	
	
}


