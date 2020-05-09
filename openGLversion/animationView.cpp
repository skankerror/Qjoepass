/*
 animationView.cpp
 
 Navigation in 3d-space
 move in and out
 shift left and right
 rotate around center

 created by wolfgang, 04.05.2009
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



/* includes */
#include "globals.h"
#include "myutil.h"
#include "myPrintf.h"
#include "graphics.h"

#include <math.h>
#include <string.h>
#include "globals.h"
#include "animationView.h"
#include "grafik.h"
#include "animationWin.h"
#include "jugglers.h"
#include "parseSiteswap.h"

#include "systemswitch.h"
#include "mem.h"

#include "vmath.h"
#include "vtypes.h"
#include "world.h"
#include "cameraControl.h"

enum { sideMove, forwardMove, circulate, init};

Vector3D				bend(1,0,0);

#define					DefPos  Vector3D(0,0,0)
#define					DefPoi  Vector3D(1,0,0)
#define					DefUp   Vector3D(0,1,0)
#define					DefMoveFaktor	1


Vector3D				defPos(DefPos);
Vector3D				defPoi(DefPoi);
Vector3D				defUp(DefUp);

Vector3D				pos					= DefPos;
Vector3D				poi					= DefPoi;
Vector3D				up					= DefUp;


float					angleFactor			= .5;
float					moveFaktor			= DefMoveFaktor;
float					rotation			= 0;

static int				lastMode			= init;
GLuint					animationView_lastTime			= -1;
GLboolean				animationMouseMovementActive		= GL_TRUE;

/* globals */
// states of modifier keys
extern wwwBoolean		skMode, ckMode, akMode, capsMode, cmdMode;              

static GLfloat			animationView_lastSide[2];
static GLfloat			animationView_lastDist;

static GLfloat			animationView_axis[3];
static GLfloat			animationView_side[2];
static GLfloat			animationView_dist;

static GLfloat			animationView_angle = 0.0;

GLfloat					animationView_transform[4][4];

static GLuint			animationView_width;
static GLuint			animationView_height;

static GLint			animationView_button = -1;
static GLint			animationView_tracking = GL_TRUE;
static wwwBoolean		firstCirculation = true;


static void animationViewStartMotion(int x, int y, int button, int time);
static void animationViewStopMotion(int button, unsigned time);
static void animationViewCirculate( int x, int y);
static void animationViewSide( int x, int y);
static void animationViewForward( int x, int y);
//static void animationViewHelp( void );


void animationGetMatrix(GLfloat matrix[4][4])
{
	int c,d;
	
	for( c = 0; c < 4; c++)
		for( d = 0; d < 4; d++)
			matrix[c][d] = animationView_transform[c][d] ;
	
}

void animationSetMatrix(GLfloat matrix[4][4])
{
	int c,d;
	
	for( c = 0; c < 4; c++)
		for( d = 0; d < 4; d++)
			animationView_transform[c][d] = matrix[c][d];
}


void animationViewSide( int x, int y)
{
	
	animationView_side[0] = (x - animationView_lastSide[0]) * moveFaktor;
	animationView_side[1] = -(y - animationView_lastSide[1]) * moveFaktor;
	
	animationView_lastSide[0] = x;
	animationView_lastSide[1] = y;
}



void animationViewForward( int x, int y)
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( x, y)
#endif

	// Hier stellen wir nur fest, wie weit es geht.
	// Wo vorn ist, hngt davon ab, wohin wir gucken
	// maW, wohin wir uns bisher gedreht haben.
	// wird in animationViewCirculate austgerechnet
	animationView_dist =  -(y - animationView_lastDist) * moveFaktor;
	animationView_lastDist = y;
	
	// Auch bewegung zur Seite erlaubt
	animationView_side[0] = (x - animationView_lastSide[0]) * moveFaktor;
	animationView_lastSide[0] = x;
}




void animationViewInit(GLint button)
{
	animationView_button = button;
	animationView_angle = 0.0;
	animationView_side[0] = 0.;
	animationView_side[1] = 0.;
	animationView_axis[0] = animationView_axis[1] = animationView_axis[2] = 0.;
}

void animationResetMagnification( void )
{		jpPrefs.mag = DefMag;
}

void animationViewSet( int mode )
{
	
	// stop auto movement
	setCameraMode( false );
	animationResetMagnification();
	
	jpPrefs.rotationAroundCenter = 0;
	jpPrefs.bendAngle = 0;
	
	animationViewInit(animationView_button);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	switch( mode ){
		case TOP_VIEW:
			glRotatef( 90,1,0,0);
			if( jugglerCount > 1 )
				glTranslatef(0, -700 * jpPrefs.mag,0);
			else
				glTranslatef(0, -500 * jpPrefs.mag,0);
			break;
		case BIRD_VIEW:
			glRotatef( 10,1,0,0);
			glTranslatef(0,-300* jpPrefs.mag, -700 * jpPrefs.mag);
			//                      glRotatef( -40,0,1,0);
			jpPrefs.rotationAroundCenter = 40;
			break;
		case FRONT_VIEW:
			glTranslatef(0, BASIC_POSITION*jpPrefs.mag,-300*jpPrefs.mag);
			break;
		default:
			break;
	}
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)animationView_transform);
	glPopMatrix();
}




void animationViewReshape(int width, int height)
{
	
	animationView_width  = width;
	animationView_height = height;
}



void animationMatrix()
{
	static GLfloat   avm[4][4];
	float viewAngle;
	Vector3D v;
	
	
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)avm);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	

	if( animationMouseMovementActive == GL_TRUE)	{
		if( animationView_tracking){
			glTranslatef( animationView_side[0], animationView_side[1], animationView_dist);
			glRotatef(animationView_angle, animationView_axis[0], animationView_axis[1], animationView_axis[2]);
		}
	}
	animationMouseMovementActive = GL_FALSE;
	
	glMultMatrixf((GLfloat *)animationView_transform);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)animationView_transform);
	glPopMatrix();
	glLoadIdentity();

	
	v.x = animationView_transform[0][0];
	v.y = 0;
	v.z = animationView_transform[0][2];
	
	v = v.normalised();
	
	viewAngle = (v * aX) *90;
	if( ( v * aZ) < 0 )
	{       viewAngle *= -1;
	}
	
	if( viewAngle < 0 )
		viewAngle += 180;
	if( v.x<= 0 )
	{       viewAngle += 180;
	}
	
	
	glMultMatrixf((GLfloat *)animationView_transform);
	glRotatef(jpPrefs.rotationAroundCenter,0,1,0);
	bend.y = 0;
	bend.z = cos( gradToRad(jpPrefs.rotationAroundCenter + viewAngle) );
	bend.x = -sin( gradToRad(jpPrefs.rotationAroundCenter+ viewAngle) );
	glRotatef(jpPrefs.bendAngle,bend.x, bend.y, bend.z);
	glMultMatrixf((GLfloat *)avm);

	glScalef( jpPrefs.mag,  jpPrefs.mag,  jpPrefs.mag);             // scale graphics
	
}



#if 0
void animationViewHelp( void )
{       float s;
	float h;
	
	// draw floor
	if( animationView_tracking == GL_TRUE)
	{
		setColorFromVector(red);
		s = 200 * jpPrefs.mag;
		h = 1 * jpPrefs.mag;
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glLineWidth( 1);
		glBegin(GL_QUADS);
		glVertex3f(-s, h, -s);
		glVertex3f(s, h, -s);
		glVertex3f(s, h, s);
		glVertex3f(-s, h, s);
		glEnd();
//		setupRenderMode( jpPrefs.solidWorld );
	}
}
#endif

void animationViewCirculate(int x, int y)
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( y )
#endif
	static int lastX, lastY;
	float dx,dy;
	
	if( firstCirculation )
	{       lastX = x;
		lastY = y;
		firstCirculation = false;
		return;
	}
	dx = (lastX-x)*angleFactor;
	dy = (lastY-y)*angleFactor;
	
	jpPrefs.rotationAroundCenter -= dx;
	jpPrefs.bendAngle += dy;
	
	lastX = x;
	lastY = y;
}


void animationViewStartMotion(int x, int y, int button, int time)
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( button, time)
#endif
	
	animationView_tracking = GL_TRUE;
	animationView_lastTime = time;
	firstCirculation = true;
	
	
	animationView_axis[0] = animationView_axis[1] = animationView_axis[2] = 0;
	animationView_side[0] = animationView_side[1] = 0.;
	animationView_dist =  0;
	
	animationView_lastDist = y;
	animationView_lastSide[0] = x;     
	animationView_lastSide[1] = y;
	
}

void animationViewStopMotion(int button, unsigned time)
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( button, time)
#endif
	
	glutSetCursor(GLUT_CURSOR_INHERIT);
	animationView_tracking = GL_FALSE;
}



void animationViewMouse(int button, int state, int x, int y)
{
	getModifiers();
	
	animationMouseMovementActive = GL_TRUE;
	
	button = animationView_button;
	
	if (state == GLUT_DOWN && button == animationView_button)
		animationViewStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
	else if (state == GLUT_UP && button == animationView_button)
		animationViewStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
}

void animationViewMotion(int x, int y)
{
	int thisMode;
	
	animationMouseMovementActive = GL_TRUE;
	
	if (animationView_tracking == GL_FALSE)
		return;
	
	
	moveFaktor =  DefMoveFaktor * .1f;//* jpPrefs.mag;
	
	recalcFrameRate();

	if( !jpAnimate ){
		drawScene();
	}
	
	if(akMode)
		thisMode = forwardMove;
	else if(skMode)
		thisMode = sideMove;
	else
		thisMode = circulate;

	
	if( thisMode != lastMode )	{
		lastMode = thisMode;
		animationViewStartMotion(x, y, -1, glutGet(GLUT_ELAPSED_TIME));
		return;
	}
	
	switch(thisMode)	{
		case forwardMove:
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
			animationViewForward(x,y);
			break;
		case sideMove:
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			animationViewSide(x,y);
			break;
		case circulate:
			glutSetCursor(GLUT_CURSOR_INFO);
			animationViewCirculate(x,y);
			
	}
	lastMode = thisMode;
}

// places the camera at the position defined by the routines in cameraControl.cpp
void animationCameraView( void )
{
	Vector3D d, pos,poi, up;
	float b;
	
	// move pos relative to scaling
	d = cameraPoi - cameraPos;
	b = d.betrag();
	d = d.normalised();
	
	d = d * ((1.- DefMag/jpPrefs.mag)*b);
	pos = cameraPos + d;
	poi = cameraPoi;
	up = cameraUp *( DefMag/jpPrefs.mag);
	
	
	glLoadIdentity();
	glScalef( jpPrefs.mag,  jpPrefs.mag,  jpPrefs.mag);           
	
	gluLookAt( pos.x, pos.y, pos.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);
	
}


