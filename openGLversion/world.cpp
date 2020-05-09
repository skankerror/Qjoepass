
/*
 *  world.cpp
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
/*
 | Weltbeschreibung von Joe Pass
 |
 |ÔøΩhrend siteswap.c die Objekte durch R3 bewegt,
 |ÔøΩinden sich hier
 |ÔøΩ Zeichnen der Jongleure und der Blle
 |   Zeichnen von Koordinatensystem und Fuboden
 | - die Verwaltung der Bildebene
 |ÔøΩ Projektionsroutinen
 |ÔøΩ Bewegung im Raum
 if( 0 )
 {       getMacModifiers();
 if( skMacMode)  v.x *= 0;
 if( ckMacMode)  v.y *= 0;
 if( akMacMode)  v.z *= 0;
 }
 |
 */
#include "graphics.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "headers.h"

#include "globals.h"
#include "cdewDraw.h"
#include"constants.h"
#include "prefDefinitionValues.h"


#include "systemswitch.h"

#include "mydefs.h"
#include "myutil.h"
#include "myPrintf.h"

#include "world.h"
#include "prefDef.h"
#include "grafik.h"
#include "vmath.h"
#include "vtypes.h"
#include "parseSiteswap.h"
#include "siteswap.h"
#include "jugglers.h"
#include "mem.h"
#include "dxf.h"
#include "runaround.h"
#include "animationWin.h"

#include "animationView.h"


void					 _testJuggler( int c);

sysColor				 shortDistanceColor(1,1,1);
void                     calcAllObjectDistances( void );
bool					 emphaseShortDistanceFlag = false;
float					 minDistanceValue;
void					 projectWarningSphere( _object *o);
void					 projectOneObject( _object *o);
void					 projectAllJugglerPaths( void );
void					 projectJugglerPath( int nr );
punkt					 getJugglerWalkPosition( int m, float f );
_jm                     *getJMFromIndex( int nr, int i );
_jm                     *getJmOfJugglerBefore( int nr,  float t);
_jm                     *getJmOfJugglerAtOrAfter( int nr,  float t);
punkt					 getPoiOfJugglerAtOrAfter( int nr,  float t);
bool					 splitInSubIntervals( float t, float low, float high, float *l1, float *h1, float *l2, float *h2);

GLuint					myObjectLists;
int						tableHeigt	= 120 - ObjectSize;
int						tableSize	= 100;

char					*objInfoStr[] = {"grey", "red", "multiple", "self: yellow - pass: red", "catcher's color", "orbit"};



#define drawOneLine3(x1,y1,z1,x2,y2,z2)  glBegin(GL_LINES);  \
glVertex3f ((x1),(y1),(z1) ); glVertex3f ((x2),(y2),(z2) ); glEnd();

int             linesOnFloor            = 7;

float           distanceBlendValue      = .1f;

/*
 GLfloat         spot_direction[]        = { 0, 0, 0, 1. };
 float           highEnv                 = 10;
 float           lowEnv                  =  -100;
 bool            shaderMode              = false;
 float           fogDensity              = 0.03f;
 float           fogEnd                  = 50;
 */

void            projectUnicycle( int i );
void            projectStage( sysColor theColor );

float           shaftLength;
float           diameter                = 2;
void            drawBall3_1( punkt *a, float radius, sysColor c, float winkel, Vector3D *v);
void            drawBall3_1_transparent( punkt *a, float radius, sysColor c, float winkel, Vector3D *v);
Vector3D		getUpOfDir( Vector3D in );

int				RingLines           = 20;       // chages possible for finer vs faster drawing
int				ClubLines           = 4;        // chages possible for finer vs faster drawing
int				FishLines           = 4;        // chages possible for finer vs faster drawing

#define         ClubPoints 8            // DONT change!
GLfloat         club[ClubPoints+1][3] = {   { 00.0f, 00.0f, 00.1f},
	{ 00.0f, 02.5f, 00.0f},
	{ 00.0f, 02.5f, 02.0f},
	{ 00.0f, 01.5f, 02.1f},
	{ 00.0f, 02.0f, 20.0f},
	{ 00.0f, 07.0f, 36.0f},
	{ 00.0f, 07.0f, 41.0f},
	{ 00.0f, 02.5f, 52.0f},
	{ 00.0f, 00.0f, 52.0f},
};


#define         FishPoints 10           // DONT change!
GLfloat         fish[FishPoints+1][3] = {
	{ 00.0f, 00.0f, 00.1f},
	{ 00.0f, 02.5f, 00.0f},
	{ 00.0f, 02.5f, 02.0f},
	{ 00.0f, 01.5f, 02.1f},
	{ 00.0f, 02.0f, 20.0f},
	{ 00.0f, 05.5f, 34.0f},
	{ 00.0f, 08.0f, 38.0f},
	{ 00.0f, 08.0f, 40.0f},
	{ 00.0f, 05.5f, 43.0f},
	{ 00.0f, 02.5f, 52.0f},
	{ 00.0f, 00.0f, 52.0f},
};

GLfloat         ring[RingPoints][3] = {     {  00.0, 00.0, RingSize2*1.0},
	{  01.5, 00.0, RingSize2*.90},
	{  01.5, 00.0, RingSize2*.70},
	{  00.0, 00.0, RingSize2*.60},
	{  -1.5, 00.0, RingSize2*.70},
	{  -1.5, 00.0, RingSize2*.90},
	{  00.0, 00.0, RingSize2*1.0},
};

GLint           ringList, clubList, fishList, ballList;
float           *clubPoints             = NULL;
float           *fishPoints             = NULL;
float           *ringPoints             = NULL;

void            generateClubs( void );
void            generateFishes( void );
void            generateRings( void );
void            renderRing( void );
void            renderClub( void );
void            renderFish( void );

void            projectSky( sysColor theColor );
void            projectFloor( sysColor theColor );
void            projectFloorGrid( sysColor theColor );
void            projectModel( void );
void            projectNiveaus( wwwBoolean mode );
void            projectAxis( float scale );
void            projectAllObjects( void );
void            projectAllJugglers( void );
void            projectBall( _object *o, sysColor c);
void            projectClub( _object *o, sysColor theColor);
void            projectPancake( _object *o, sysColor c);
void            projectRing( _object *o, sysColor c);
void            projectLogo(void);
void			projectTable(void);
void            projectCameraPath( void );
static void normal3f( GLfloat x, GLfloat y, GLfloat z );

void            projectSun( sysColor theColor );
#define         SunSize                         100
sysColor        sunColor                        = sysColor(1.f,1.f,0.f);
bool            projectSunFlag					= true;
// Koordinatensystem
#define         AXIS_C                          3
typedef  struct {
	punkt P1, P2;
	char  *n1, *n2;
	sysColor color;
}
_achse;

_achse achse[AXIS_C] 
#if 0
// versteht einer meiner alten compiler nicht. ist der, den ich zur Zeit noch gebrauche. In initWorld initialisiert.
= {
	{ punkt( -1,  0, 0), punkt(  1, 0, 0), "- x", "+ x", DefRed},
	{ punkt(  0, -1, 0), punkt(  0, 1, 0), "- y", "+ y", DefGreen},
	{ punkt(   0,  0,-1), punkt(  0, 0, 1), "- z", "+ z", DefBlue}
}
#endif
;
//punkt           aX                                      = punkt(1,0,0);
//punkt           aY                                      = punkt(0,1,0);
//punkt           aZ                                      = punkt(0,0,1);
punkt           center                          = NullVector;
#define         AXIS_LENGTH                     100
// -------------------------------------------------------------------------------

void projectBallVectors( _object *o);



void projectBallVectors(_object *o) {
	Vector3D orientation;
	GLfloat w;
	Vector3D n,r,a, u;
	float d;
	
	glDisable(GL_TEXTURE_2D);
	glGetFloatv( GL_LINE_WIDTH, &w);
	glLineWidth( 2 );
	
	d = o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	a = o->a + Vector3D( 0, 0, -10);
	r = o->r;
	n = o->n.rotateAroundVektor( d, o->r);	
	
	u = n.vektorProdukt( r );
	
	// rotation vector
	orientation = r * 50.f + a;
	drawLine3(&a,&orientation,2,green);
	
	// orientation vector
	orientation = n * 50.f + a;
	drawLine3(&a,&orientation,2,blue);
	
	// calculated "up" vector
	orientation = u * 50.f + a;
	drawLine3(&a,&orientation,2,white);
	
	
	glLineWidth( w );
}




// --------------------- init module -----------------------
void initMyWorld( void ) {
	if( jpPrefs.levelOfDetail == 0)
		jpPrefs.levelOfDetail = DefLevelOfDetail;
	RingLines  = 7; // chages possible for finer vs faster drawing
	ClubLines  = 4; // chages possible for finer vs faster drawing
	generateObjects();
	
	achse[0].P1 = punkt( -1,  0, 0);
	achse[0].P2 = punkt(  1, 0, 0);
	achse[0].n1 = "- x";
	achse[0].n2 = "+ x";
	achse[0].color = DefRed;
	
	achse[1].P1 = punkt(  0, -1, 0);
	achse[1].P2 = punkt(  0,  1, 0);
	achse[1].n1 = "- y";
	achse[1].n2 = "+ y";
	achse[1].color = DefGreen;
	
	achse[2].P1 = punkt(  0, 0, -1);
	achse[2].P2 = punkt(  0, 0,  1);
	achse[2].n1 = "- z";
	achse[2].n2 = "+ z";
	achse[2].color = DefBlue;
	
	
}
void disposeMyWorld( void ) {}


void generateRings( void ) {
	int c,d;
	ringPoints = (float *)malloc(sizeof(ring)  * RingLines);
	// fill in 1 line
	for( c = 0; c < RingPoints; c++ ) {
		*(ringPoints + c + 0) = ring[c][0];
		*(ringPoints + c + 1) = ring[c][1];
		*(ringPoints + c + 2) = ring[c][2];
	}
	
	// rotate it
	for( c = 0; c < RingPoints; c++ )       // all RingPoints
	{       for( d = 0; d < RingLines; d++) // number of RingLines
	{
		*(ringPoints + (c + d * RingPoints) * 3 + 0) = ring[c][0];
		*(ringPoints + (c + d * RingPoints) * 3 + 1) = ring[c][2] * sin(gradToRad(d * 360./RingLines));
		*(ringPoints + (c + d * RingPoints) * 3 + 2) = ring[c][2] * cos(gradToRad(d * 360./RingLines));
	}
	}
	// ------------------------- generate ring list ---------------------------
	ringList = myObjectLists++;
	glNewList (ringList, GL_COMPILE);
	renderRing();
	glEndList ();
}
void generateClubs( void ) {
	int c,d;
	clubPoints = (float *)malloc(sizeof(club)  * ClubLines);
	// fill in 1 line
	for( c = 0; c < ClubPoints; c++ ) {
		*(clubPoints + c + 0) = club[c][0];
		*(clubPoints + c + 1) = club[c][1];
		*(clubPoints + c + 2) = club[c][2];
	}
	
	// rotate it
	for( c = 0; c < ClubPoints; c++ )       // all ClubPoints
	{       for( d = 0; d < ClubLines; d++) // number of ClubLines
	{
		*(clubPoints + (c + d * ClubPoints) * 3 + 0) = club[c][1]* sin(gradToRad(d * 360./ClubLines));
		*(clubPoints + (c + d * ClubPoints) * 3 + 1) = club[c][1]* cos(gradToRad(d * 360./ClubLines));
		*(clubPoints + (c + d * ClubPoints) * 3 + 2) = club[c][2];
	}
	}
	// ------------------------- generate club list ---------------------------
	clubList = myObjectLists++;
	glNewList (clubList, GL_COMPILE);
	renderClub();
	glEndList ();
}

void generateFishes( void ) {
	int c,d;
	fishPoints = (float *)malloc(sizeof(fish)  * FishLines);
	// fill in 1 line
	for( c = 0; c < FishPoints; c++ ) {
		*(fishPoints + c + 0) = fish[c][0];
		*(fishPoints + c + 1) = fish[c][1];
		*(fishPoints + c + 2) = fish[c][2];
	}
	
	// rotate it
	for( c = 0; c < FishPoints; c++ )       // all ClubPoints
	{       for( d = 0; d < FishLines; d++) // number of ClubLines
	{
		*(fishPoints + (c + d * FishPoints) * 3 + 0) = fish[c][1]* sin(gradToRad(d * 360./ClubLines));
		*(fishPoints + (c + d * FishPoints) * 3 + 1) = fish[c][1]* cos(gradToRad(d * 360./ClubLines));
		*(fishPoints + (c + d * FishPoints) * 3 + 2) = fish[c][2];
	}
	}
	// ------------------------- generate fishClub list ---------------------------
	fishList = myObjectLists++;
	glNewList (fishList, GL_COMPILE);
	renderFish();
	glEndList ();
}

void generateObjects( void ) {
	
	myObjectLists = glGenLists( 3 );
	generateRings();
	generateClubs();
	generateFishes();
	
}

// my logo (hidden)
#define LogoSize        1

/*
 * Call glNormal3f after scaling normal to unit length.
 */
static void normal3f( GLfloat x, GLfloat y, GLfloat z ) {
	GLdouble mag;
	mag = sqrt( x*x + y*y + z*z );
	if (mag>0.00001F) {
		x /= mag;
		y /= mag;
		z /= mag;
	}
	glNormal3f( x, y, z );
}
// --------------------- draw stuff -----------------------
void drawBall3_1( punkt *a, float radius, sysColor c, float winkel, Vector3D *v) {
	punkt p1;
	GLfloat w;
	GLUquadricObj*theQuadric;
	GLboolean b;
	
	theQuadric = gluNewQuadric();
	glGetBooleanv( GL_TEXTURE_2D, &b);
	gluQuadricTexture(theQuadric, b);
	
	glGetFloatv( GL_LINE_WIDTH, &w);
	p1 = *a;
	
	setColorFromVector (c);
	glLineWidth( 1 );
	
	glPushMatrix();
	glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(180,1,0,0);
	glRotatef(90,0,1,0);
	glRotatef( winkel , v->x, v->y, v->z);  // rotate desired angle
	gluSphere(theQuadric, radius, jpPrefs.levelOfDetail,jpPrefs.levelOfDetail);
	glPopMatrix();
	glLineWidth( w );
	gluDeleteQuadric( theQuadric);
}
void drawBall3_1_transparent( punkt *a, float radius, sysColor c, float winkel, Vector3D *v) {
	punkt p1;
	GLfloat w;
	GLUquadricObj*theQuadric;
	GLboolean b;
	
	theQuadric = gluNewQuadric();
	glGetBooleanv( GL_TEXTURE_2D, &b);
	gluQuadricTexture(theQuadric, b);
	
	
	glGetFloatv( GL_LINE_WIDTH, &w);
	p1 = *a;
	
	
	glColor4f (c.x, c.y, c.z, .3f);
	glLineWidth( 1 );
	
	
	glPushMatrix();
	glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(180,1,0,0);
	glRotatef(90,0,1,0);
	glRotatef( winkel , v->x, v->y, v->z);  // rotate desired angle
//	gluSphere(theQuadric, radius, jpPrefs.levelOfDetail,jpPrefs.levelOfDetail);
	gluSphere(theQuadric, radius, 20,20);
	
	glPopMatrix();
	glLineWidth( w );
	gluDeleteQuadric( theQuadric);
}

// Alles in der Welt darstellen
wwwBoolean projectWorld( void ) {
	// basically, a color is a 3d vektor
	//      sysColor f = floorColor;
	//vektorMult( &green, jpPrefs.fog&&0?.8:.3);
	glEnable(GL_AUTO_NORMAL);
	
	if( jpPrefs.floor)
		projectFloor( jpPrefs.floorColor);
	
	if( jpPrefs.showFloorGrid)
		projectFloorGrid( jpPrefs.floor||jpPrefs.sky? jpPrefs.floorGridColorDark : jpPrefs.floorGridColorLight );
	
	if(projectSunFlag)
		projectSun( sunColor);
	
	if( jpPrefs.sky)
		projectSky( jpPrefs.skyColor);
	if(jpPrefs.showStage)
		projectStage( jpPrefs.stageColor);
	if( jpPrefs.showLogo )
		projectLogo();
	if( jpPrefs.a == true)
		projectAxis( 1.f);
	if( jpPrefs.showCameraPath)
		projectCameraPath();
	
	if(validPattern) {
		projectAllJugglers();
		projectAllObjects();
	}
	if( tableJugglingMode )	
		projectTable( );
	if( jpPrefs.showScene )
		projectModel();
	
	if( jpPrefs.showNiveaus == true)
		projectNiveaus(jpPrefs.solidNiveaus);
	
	projectAllJugglerPaths();
	//      glCallList( arm_list );
	//	_testJuggler( 0 );
	
	return true;
}

void projectAxis( float scale ) {
	int c;
	line3 s;
	GLfloat w;
	sysColor theCol;
	
	glPushMatrix();
	glGetFloatv( GL_LINE_WIDTH, &w);
	glDisable(GL_TEXTURE_2D);
	glLineWidth( 1 );
	
	// for some reason, x axis +/-coordinates don´t reflect world coordinate system
	// hack: scale x by negative value
	glTranslatef( 0, 2, 0);
	glScalef(-scale, scale, scale);
	
	for( c = 0; c < AXIS_C; c++ ) {
		s.a = achse[c].P1;
		s.b = achse[c].P2;
		
		s.a = s.a * AXIS_LENGTH;
		s.b = s.b * AXIS_LENGTH;
		theCol = white * .5;
		setColorFromVector(  achse[c].color);
		stroke_output( s.a.x,s.a.y,s.a.z,"%s", achse[c].n1);
		stroke_output( s.b.x,s.b.y,s.b.z,"%s", achse[c].n2);
		drawCylinderLine3( s.a, s.b, 2, 2, achse[c].color );
	}
	glLineWidth( w );
	glPopMatrix();
}

#undef  ROTATIONCENTER
#undef  CATCHPOS
#define ROTATIONCENTER  0
#define CATCHPOS                0
void projectNiveaus( wwwBoolean mode ) {
	float g2, t;
	punkt   p, n = NullVector;
	float o;
	float d;
	float go;
	line3   sx, sy;
	int     c;
	sysColor theColor ;
	
	theColor = jpPrefs.niveauColor;
	glDisable(GL_TEXTURE_2D);
	if( mode == false) {
		_object tmp;
		theColor = theColor * .5f;
		setColorFromVector(theColor);
		for( d = max( 3, ssMinVal); d <= ssMaxVal;d ++ ) {
			g2 = jpPrefs.niveauSize / 2;
			go = jpPrefs.niveauSize / jpPrefs.niveauLines ;
			
			if( hurriedMode )
				t = d - hurriedValue;
			else
				t = d - 2 * handDwell;
			
			tmp.s = tmp.d = n;
			tmp.rTime = tmp.vTime = t/2;
			p = throwObject( &tmp);
			o = p.y + BASIC_HEIGHT;
			
			sx.a.y = sx.b.y = sy.a.y = sy.b.y = o;
			
			
			// parallelen zur X-Achse und zur Y-Achse
			t =  -g2;
			for( c = 0; c <= jpPrefs.niveauLines; c++ ) {
				sy.a.z = sx.a.x =  -g2;
				sy.a.x = sx.a.z =  t;
				
				sy.b.z = sx.b.x = g2;
				sy.b.x = sx.b.z =  t;
				drawLine3( &sx.a, &sx.b, .1f, theColor );
				drawLine3( &sy.a, &sy.b, .1f, theColor );
				
				t += go;
			}
		}
	} else {
		_object tmp;
		glColor4f(theColor.x, theColor.y, theColor.z,jpPrefs.niveauBlendValue);
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		for( d = max( 3, ssMinVal); d <= ssMaxVal;d ++ ) {
			g2 = jpPrefs.niveauSize / 2.;
			if( hurriedMode )
				t = d - hurriedValue;
			else
				t = d - 2 * handDwell;
			
			tmp.s = tmp.d = n;
			tmp.rTime = tmp.vTime = t/2;
			p = throwObject( &tmp );
			o = p.y + BASIC_HEIGHT;
			glPushMatrix();
			glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-g2, o, g2);
			glVertex3f(g2, o, g2);
			glVertex3f(g2, o, -g2);
			glVertex3f(-g2, o, -g2);
			glEnd();
			glPopMatrix();
		}
		glDisable( GL_BLEND);
	}
}
void projectCameraPath( void ) {
	float f;
	punkt p1, p2,t;
	float steps = 10.;
	extern int posSplineCount;
	int c,d;
	sysColor theColor;
	
	glDisable(GL_TEXTURE_2D);
	theColor = red * .5f;
	if( posSplineCount ) {
		f = 0.;
		for( c = 0; c <= posSplineCount; c++) {
			drawBall3( &posSplineList[c%posSplineCount], 3, 5, theColor );
			for( d = 0; d<steps; d++) {
				p1 = getPointFromSpline(posSplineList, posSplineCount, f );
				f += 1./steps;
				p2 = getPointFromSpline(posSplineList, posSplineCount, f );
				drawLine3( &p1, &p2, 1, theColor );
			}
		}
	}
	theColor = green * .5f;
	if( upSplineCount ) {
		f = 0.;
		for( c = 0; c <= upSplineCount; c++) {
			drawBall3( &upSplineList[c%upSplineCount], 3, 5, theColor );
			for( d = 0; d<steps; d++) {
				p1 = getPointFromSpline(upSplineList, upSplineCount, f );
				f += 1./steps;
				p2 = getPointFromSpline(upSplineList, upSplineCount, f );
				drawLine3( &p1, &p2, 1, theColor );
			}
		}
	}
	theColor = blue * .5;
	if( poiSplineCount ) {
		f = 0.;
		for( c = 0; c < poiSplineCount; c++) {
			drawBall3( &poiSplineList[c], 3, 5, theColor );
			f = c;
			for( d = 0; d<steps; d++) {
				p1 = getPointFromSpline(poiSplineList, poiSplineCount, f );
				f += 1./steps;
				p2 = getPointFromSpline(poiSplineList, poiSplineCount, f );
				drawLine3( &p1, &p2, 1, theColor );
			}
		}
	}
	theColor = green * .3f;
	drawBall3( &cameraPoi, 5,5,theColor  );
	
	if( 0 ) {
		extern float upSplineTime;
		t = getPointFromSpline(upSplineList, upSplineCount, upSplineTime);
		drawLine3( &t, &cameraPoi, 1, white );
	}
}

void projectAllObjects( void ) {
	int c = 0;
	sysColor ballColor;
	_object *o;
	
	calcAllObjectDistances();
	static int traceCounter, highestTraceUntilNow = 0;
	
#define TraceMax 200
#define PicturesBetweenTraces 20
	
	bool makeObjectSnapshot = false;
	static _object oldObjectList[BALL_MAX][TraceMax];
	static int oldObjectSlot = 0;
	
	
	traceCounter++;
	highestTraceUntilNow = max(traceCounter, highestTraceUntilNow);
	
	if( traceCounter == PicturesBetweenTraces )
	{
		traceCounter = 0;
		
		oldObjectSlot++;
		oldObjectSlot%=TraceMax;    
		
		makeObjectSnapshot = true;
	}else
		makeObjectSnapshot = false;    
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, objMaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, objMaterialShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,	objMaterialEmission	);
	
	// punkt in Bildschirm-Koordinaten umrechnen
	for( c = 0; c < ballCount; c++ ) {
		o = getObject(c);
		
		// don´t show the viewing object in vomitMode
		if(viewMode == VOMIT_VIEW && (getVomitNumber() == c) )
			continue;
		
		if( o->catcher.juggler == NO_JUGGLER || o->dropped || o->invisible)
			continue;
		
		
		
		switch( jpPrefs.objectColorMode ) {
			case orbitColorObj:
				ballColor = _getListColor(o->orbitNr, orbitColorList, OrbitColorMax);
				break;
			case multiColorObj:
				ballColor = getListColor( c );
				break;
			case redObj:
				ballColor = jpPrefs.objectColor;
				break;
			case greyObj:
				ballColor = lightGray;
				break;
			case destObj:
				if( o->hasSelfColor )
					ballColor = selfColor;
				else
					ballColor = passColor;
				break;
			case destJugglerObj:
//30.12.17, see below
#if true
				Juggler *j= getJuggler(o->catchingJugglerIndex);
				if( j->colorDefined)
					ballColor = j->color; // set by moveOneObject
				else
					ballColor = getListColor(o->catchingJugglerIndex);
#else
				ballColor = getListColor(o->catchingJugglerIndex); // set by moveOneObject
#endif
				break;
				
/* 			default:
				ballColor = green; */
				break;
		}
		
		if( o->colorDefined )
			ballColor =  o->color;
		

//30.12.17, see above
#if false		
		if( j->colorDefined)
			ballColor = j->color; // set by moveOneObject
		else
			ballColor = getListColor(o->catchingJugglerIndex);
#endif
		
		
		setColorFromVector(ballColor);
		if( jpPrefs.showObjectNumbers ){
			glLineWidth(2);
			glColor3f(1.f, 1.f, 1.f); 
			stroke_output(o->a.x, o->a.y, o->a.z,"%2d", getObjectIndex(o)+1);
		}
		setColorFromVector(ballColor);
		if( jpPrefs.showSiteswapValue )
			stroke_output(o->a.x, o->a.y+20, o->a.z,"%2.2f", o->nextSiteswapValue);
		
		if( o->shortDistanceFlag ) {
			Vector3D vn = Vector3D(1,1,1), r;
			float f;
			
			// revert color if too close
			r = ballColor - shortDistanceColor;
			f = r.betrag();
			if(f < .5)
				ballColor = vn - shortDistanceColor;
			else
				ballColor = shortDistanceColor;
		}
		
		o->usedColor = ballColor;
		
		if( makeObjectSnapshot )
			oldObjectList[c][oldObjectSlot] = *o;
		
		projectOneObject( o ); 
	}
	
}


void projectOneObject( _object *o)
{
	switch( o->kind ) {
		case balls:
			projectBall( o, o->usedColor);
			break;
		case clubs:
		case fishes:
			projectClub(o, o->usedColor);
			break;
		case rings:
			projectRing(o, o->usedColor);
			break;
		case pancakes:
			projectPancake(o, o->usedColor);
			break;
		case nothing:
			break;
		default:
			break;
	}
	
	
	if( jpPrefs.emphaseShortDistanceFlag )
		projectWarningSphere( o );
	// has to be in object rendering routine
	if(jpPrefs.showObjectVektors) 
		projectBallVectors( o );
	
	
}
void projectSky( sysColor theColor ) {
	punkt z = NullVector;
	float skySize = SkySize;
	sysColor c;
	
	if( hasSkyTexture && jpPrefs.texturedSky ) {
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, hasSkyTextureIndex);
		c = white;
	} else {
		glDisable(GL_TEXTURE_2D);
		c = theColor;
	}
	
	glPushMatrix();
	glRotatef( 90,0,0,1);
	glNormal3f( 0, 1,0);
	drawBall3( &z, skySize , 10, c);
	glPopMatrix();
	
}

void projectSun( sysColor theColor ) {
	float sunSize = SunSize;
	GLfloat sunEmission[] = { .5f, .5f, .5f, 1.0f };
	
	Vector3D l = Vector3D( light_position[0],light_position[1],light_position[2]);
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_EMISSION,	sunEmission	);
	glPushMatrix();
	glTranslatef(l.y, l.y, l.z);
	glNormal3f( 0, 1,0);
	drawBall3( &z, sunSize , 10, theColor);
	glPopMatrix();
}

void drawGrid( int x, int y, int w, int h );

void projectFloorGrid( sysColor theColor ) {
	extern float metersOnScreen;
	
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	
	glRotatef( -90,1,0,0);
	glTranslatef( 0,0,2);
	//	glNormal3f(0,0,-1);
	glLineWidth( 1.f );
	
	setColorFromVector (theColor);
	
	if( grid2Parameters )
		drawGrid(gridX*100, gridY*100, 0,0);
	else
		drawGrid( metersOnScreen * 100,metersOnScreen * 100,0,0);
	
	glPopMatrix();
	
}



void projectFloor( sysColor theColor ) {
	float s;
	float   textureSize = jpPrefs.repeatFloor;
	GLboolean cullFaceMode;
	
	glGetBooleanv( GL_CULL_FACE, &cullFaceMode );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	if( jpPrefs.lookThroughFloor )
		glEnable(GL_CULL_FACE); // remove backside from floor
	else
		glDisable(GL_CULL_FACE); // do not remove backsides
	
	if( hasFloorTexture && jpPrefs.texturedFloor) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hasFloorTextureIndex);
		setColorFromVector (punkt(1., 1., 1.));
	} else {
		glDisable(GL_TEXTURE_2D);
		setColorFromVector (theColor);
		
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0.0);
	s = jpPrefs.floorSize;
	
	if(hasFloorTexture) {
		glTexCoord2d(0.0, 0.0);
		glVertex3f(-s, 0, s);
		glTexCoord2d(textureSize, 0.0);
		glVertex3f(s, 0, s);
		glTexCoord2d(textureSize, textureSize);
		glVertex3f(s, 0, -s);
		glTexCoord2d(0.0, textureSize );
		glVertex3f(-s, 0, -s);
		
	} else {
		
		glVertex3f(-s, 0, s);
		glVertex3f(s, 0, s);
		glVertex3f(s, 0, -s);
		glVertex3f(-s, 0, -s);
		
		
	}
	glEnd();
	glPopMatrix();
	
	if( cullFaceMode )
		glEnable(GL_CULL_FACE); // remove backside from floor
	else
		glDisable(GL_CULL_FACE); // do not remove backsides
}

void projectStage( sysColor theColor ) {
	Vector3D a,b,c,d;
	float   textureSize = 1;
	
	a = b = jpPrefs.lowerLeftStage;
	c = d = jpPrefs.upperRightStage;
	
	b.y = c.y;
	d.y = a.y;
	setColorFromVector (theColor);
	if( hasStageTexture) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hasStageTextureIndex);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	} else {
		glDisable(GL_TEXTURE_2D);
		
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	//	glNormal3f(0, 0, -1.0);
	
	if(hasStageTexture) {
		glTexCoord2d(textureSize, textureSize);
		glVertex3f(c.x, c.y, c.z);
		glTexCoord2d(textureSize, 0.0);
		glVertex3f(d.x, d.y, d.z);
		glTexCoord2d(0.0, 0.0);
		glVertex3f(a.x, a.y, a.z);
		glTexCoord2d(0.0, textureSize);
		glVertex3f(b.x, b.y, b.z);
		
	} else {
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(d.x, d.y, d.z);
	}
	glEnd();
	glPopMatrix();
	
}

void projectBall( _object *o, sysColor c) {
	
	float           winkel;
	float           f, angle;
	Vector3D          v, z = Vector3D(0,0,1);
	punkt           a, h, n, r;
	Vector3D          rot = Vector3D(1,1,0);
	
	
	winkel = o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	n = o->n.normalised();
	r = o->r.normalised();
	a = o->a;
	if( o->thrower.h == leftHand )
		winkel *= -1;
	
	h = o->n * (  ( ROTATIONCENTER - CATCHPOS) * ClubSize * jpPrefs.objectFaktor * o->size *  o->rTime / (o->vTime + o->rTime));
	a = a + h;
	h =  o->nDest *( (ROTATIONCENTER - CATCHPOS) * ClubSize * jpPrefs.objectFaktor * o->size  *  o->vTime / (o->vTime + o->rTime));
	a = a + h;
	// angle of rotation
	f = n * z ;
	angle = radToGrad( acos( f      )  );
	// axis of rotation (cross product)
	v = z.vektorProdukt(n).normalised();
	
	if( hasObjectTexture  && jpPrefs.texturedBall) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hasObjectTextureIndex);
	}
	
	
	
	glPushMatrix();
	drawBall3_1( &a, ObjectSize * jpPrefs.objectFaktor * o->size, c, winkel, &rot);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


void projectTable(void) {
	
	glPushMatrix();         // push global
	
	glDisable(GL_TEXTURE_2D);
	glTranslatef( 0, tableHeigt, 0);
	glRotatef( -90,0,0,1);          // rotate to y dir (was z)
	drawDisk3( &pN, 0, tableSize, jpPrefs.levelOfDetail*2, &green);
	glPopMatrix();          // pop global
}

void projectLogo(void) {
	static float spin = 0;
	float    scale;
	scale = LogoSize;
	spin += 5;
	
	// global transforms
	glPushMatrix();         // push global
	//      glScalef(scale, scale, scale);
	glTranslatef( 0,1000,0);        // shift to position (10 meter above center)
	glRotatef(spin, 0,1,0); // spin around y
	
	// local transforms 1
	glPushMatrix();                 // push local
	glRotatef( -90,1,0,0);          // rotate to y dir (was z)
	renderClub();
	
	glPopMatrix();                          // pop local
	
	// local transforms 2
	glPushMatrix();                         // push local
	glTranslatef(0,50,0);
	glRotatef(90,0,0,1);
	renderRing();
	
	glPopMatrix();                          // pop local
	glPopMatrix();          // pop global
}

// draw environment model
void projectModel( void ) {
	float           scale = jpPrefs.modelScale;
	if( hasEnvModel == false)
		return;
	
	glDisable(GL_TEXTURE_2D);
	setColorFromVector(white);
	glPushMatrix();
	glScalef(scale, scale, scale);                                  // scale to desired size
	//      glRotatef( -90,1,0,0);
	
	glCallList( demo_list );
	glPopMatrix();
	//====================
	return;
	//====================
}

void projectUnicycle( int i ) {
	GLUquadricObj*theQuadric;
	Juggler *j;
	Vector3D x = Vector3D(1,0,0);
	float angle;
	Vector3D v;
	float r;
	float level;
	wwwBoolean giraffe = false;
	float zahnradDiameter = 8, chainLength;
	
	j = getJuggler( i );
	if( (level = j->position.y - BASIC_HEIGHT) > 0 )
		giraffe = true;
	chainLength = j->idlePosition.y- BASIC_HEIGHT;
	
	
	theQuadric = gluNewQuadric();
	gluQuadricTexture(theQuadric, GL_FALSE);
	gluQuadricOrientation(theQuadric,GLU_OUTSIDE);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	if(!jpPrefs.newGiraffeModel && level < 0 ) {
		shaftLength = - (BASIC_POSITION - PELVIC_POSITION) - 2*wheelSize-5;
		glTranslatef(j->idlePosition.x,wheelSize + level,j->idlePosition.z);
		glRotatef(90,0,1,0);
		
	} else {
		shaftLength = j->lb.y - 2*wheelSize-5;
		glTranslatef(j->idlePosition.x,wheelSize,j->idlePosition.z);
	}
	buildRotationVektor( center, j->viewdirection, &angle, &v);
	glRotatef ( angle + 90, v.x,v.y,v.z);
	
	setColorFromVector(white);
	glPushMatrix();
	glTranslatef(0, wheelSize, 0);                          // shaft
	glRotatef(-90,1,0,0);
	gluCylinder(theQuadric,diameter ,diameter,shaftLength , 5,2);
	glPopMatrix();
	glPushMatrix();
	// wheel (rotating)
	{
		float vz;
		
		vz = j->viewdirection *  x;
		r = j->rotation;
		if( vz < 0 )
			r = -r-180;
		
		glRotatef( r, 0,0,1);
	}
	glPushMatrix();
	setColorFromVector(white);
	glutSolidTorus(diameter*1,wheelSize*.9f,5,20);  // band
	setColorFromVector(punkt(0.1f,0.1f,0.1f));
	glutSolidTorus(diameter*1.2f,wheelSize,5,20);   // band
	glPopMatrix();
	if(  giraffe || jpPrefs.newGiraffeModel) {
		setColorFromVector(punkt(.8f,.8f,.8f));
		glPushMatrix();
		glTranslatef(0, 0, axisLength/4);                               // zahnrad unten
		gluDisk( theQuadric, 0,zahnradDiameter,10,10);
		glPopMatrix();
	}
	glPopMatrix();
	if( giraffe ) {
		glPushMatrix();
		setColorFromVector(punkt(.2f,.2f,.2f));
		glTranslatef(zahnradDiameter,0,  axisLength/4); // kette
		glRotatef( -90,1,0,0);
		gluCylinder(theQuadric,1 , 1, chainLength , 5,2);
		glPopMatrix();
		
		glPushMatrix();
		setColorFromVector(punkt(.2f,.2f,.2f));
		glTranslatef(-zahnradDiameter,0,  axisLength/4);        // kette
		glRotatef( -90,1,0,0);
		gluCylinder(theQuadric, 1, 1, chainLength , 5,2);
		glPopMatrix();
	}
	// tretlager
	if( level >= 0 || jpPrefs.newGiraffeModel)
		glTranslatef( 0,j->idlePosition.y-BASIC_HEIGHT,0);
	glRotatef( r, 0,0,1);
	
	glPushMatrix();
	setColorFromVector(punkt(.5,.5,.5));
	glTranslatef(0, 0, -axisLength/2);                      // axis
	gluCylinder(theQuadric,diameter/2 ,diameter/2, axisLength , 5,2);
	glPopMatrix();
	if(  giraffe || jpPrefs.newGiraffeModel) {
		setColorFromVector(punkt(.8f,.8f,.8f));
		glPushMatrix();
		glTranslatef(0, 0, axisLength/4);                               // zahnrad oben
		gluDisk( theQuadric, 0,zahnradDiameter,10,10);
		glPopMatrix();
	}
	
	glPushMatrix();
	glTranslatef(0, 0, axisLength/2);                       // left kurbel
	glRotatef(90,1,0,0);
	gluCylinder(theQuadric,diameter/2 ,diameter/2,kurbelLength , 5,2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, 0, -axisLength/2);                      // right kurbel
	glRotatef(-90,1,0,0);
	gluCylinder(theQuadric,diameter/2 ,diameter/2,kurbelLength , 5,2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, -wheelSize/2 - diameter, axisLength/2);                         // left pedal
	gluCylinder(theQuadric,diameter/2 ,diameter/2,pedalSize , 5,2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, wheelSize/2 +diameter , -axisLength/2-pedalSize );      // right pedal
	gluCylinder(theQuadric,diameter/2 ,diameter/2,pedalSize , 5,2);
	glPopMatrix();
	glPopMatrix();
	
#if 0
	//normals
	{ vektor n;
		n = vektorMult( &j->n, 50);
		n = vektorSumme( &n, &j->p);
		drawCylinderLine3(&j->idlePosition , &j->p, lineSize * jpPrefs.bodyFaktor, lineSize * jpPrefs.bodyFaktor, &white );
	}
#endif
	if(jpPrefs.newGiraffeModel && level < 0 ) {
		Vector3D  centerOfWheel;
		Vector3D  centerOfFeet;
		Vector3D  backsideOfTool;
		Vector3D t;
		float diameter = 3;
		
		centerOfWheel = j->idlePosition;
		centerOfWheel.y = wheelSize ;
		centerOfFeet = j->idlePosition;
		centerOfFeet.y +=  (BASIC_POSITION - PELVIC_POSITION)+5;
		t = centerOfWheel - centerOfFeet;
		t = t * -.5f;
		backsideOfTool = centerOfWheel + t;
		
		t = j->viewdirection * 50.f;
		backsideOfTool = backsideOfTool - t;
		
		drawCylinderLine3(centerOfWheel, backsideOfTool, diameter, diameter, white );
		drawCylinderLine3(backsideOfTool, centerOfFeet, diameter, diameter, white );
		drawCylinderLine3(j->lb, centerOfFeet, diameter, diameter, white );
		drawCylinderLine3(j->lb, backsideOfTool, diameter, diameter, white );
		
	}
	
	gluDeleteQuadric(theQuadric);
	
}

#undef  ROTATIONCENTER
#undef  CATCHPOS
#define ROTATIONCENTER  (28./52.)
#define CATCHPOS                .3
//------------------------
// club:
// objects are drawn on z axis,
// with handle starting on center
// and positive orientation
//------------------------
void projectClub( _object *o, sysColor c ) {
	float angle;
	Vector3D n;
	Vector3D h,a;
	
	
	float           mode;
	if( hasClubTexture && jpPrefs.texturedClub ) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hasClubTextureIndex);
		glGetTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mode );
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
	
	angle =  -1* o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	
	
	a = o->a;
	n = o->n;
	
	
	glPushMatrix();
	
	if(o->rTime <= 0 ) {
		
		
		// hold - bend
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		// object rotation
		glRotatef( angle, o->r.x, o->r.y, o->r.z );
		
		if(0 && jpPrefs.a == true)
			projectAxis(0.3f);
		
		// transform to orientation
		n.translateZObjectOnVektor();
		
		
		// a club that is catched reversed is being hold different 
		if( 0 && o->halfSpinFlag )
			glTranslatef(0,0, (CATCHPOS-1) * ClubSize* jpPrefs.objectFaktor * o->size  );
		else
			glTranslatef(0,0, -CATCHPOS * ClubSize* jpPrefs.objectFaktor * o->size   );
		
	} else {       // airborn - rotation
		//        angle =  -1* b->spins * (  b->vTime / (b->vTime + b->rTime)) * 360;
		h = n * (  (  CATCHPOS) * ClubSize * jpPrefs.objectFaktor *  o->rTime / (o->vTime + o->rTime));
		a = a + h;
		h = o->nDest * ( ( CATCHPOS) * ClubSize * jpPrefs.objectFaktor *  o->vTime / (o->vTime + o->rTime));
		a = a + h;
		
		
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		
		// object rotation
		glRotatef( angle,o->r.x, o->r.y, o->r.z );
		
		
		if(0 && jpPrefs.a == true)
			projectAxis(0.3f);
		
		// transform to orientation
		n.translateZObjectOnVektor();
		
		
		glTranslatef(0,0, -ROTATIONCENTER * ClubSize * jpPrefs.objectFaktor * o->size  );
	}
	
	
	glScalef(jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size);                                       // scale to desired size	
	
	setColorFromVector( c);
	
	
	glPushMatrix();
	if(o->halfSpinFlag){
		glTranslatef(0,0,ClubSize);
		glRotatef(180, 0,1,0);
	}
	if( o->kind == clubs)
		renderClub( );
	else
		renderFish();
	
	glPopMatrix();	
	
	glPopMatrix();
	if( hasClubTexture && jpPrefs.texturedClub) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
	}
	
	glDisable(GL_TEXTURE_2D);
}


#undef  ROTATIONCENTER
#undef  CATCHPOS
#define ROTATIONCENTER  .0
#define CATCHPOS                .5
void projectRing( _object *o, sysColor c ) {
	float angle;
	Juggler *j;
	Vector3D n;
	Vector3D h,a;
	glDisable(GL_TEXTURE_2D);
	setColorFromVector( c);
	n = o->n;
	a = o->a;
	j = o->catcher.juggler;
	
	glPushMatrix();
	angle =  -1* o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	n = o->n;
	if( o->rTime <= 0 ) {       // hold
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		
		// object rotation
		glRotatef( angle,o->r.x, o->r.y, o->r.z );
		
		// transform to orientation
		n.translateZObjectOnVektor();
		glRotatef( 180, 1,0,0);
		glTranslatef(0,0, -CATCHPOS * RingSize  * jpPrefs.objectFaktor * o->size );
	} else {       // airborn
		h = o->n * (  (  CATCHPOS) * RingSize * jpPrefs.objectFaktor *  o->rTime / (o->vTime + o->rTime));
		a = a + h;
		h = o->nDest * ( ( CATCHPOS) * RingSize * jpPrefs.objectFaktor *  o->vTime / (o->vTime + o->rTime));
		a = a + h;
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		
		// object rotation
		glRotatef( angle,o->r.x, o->r.y, o->r.z );
		
		// transform to orientation
		n.translateZObjectOnVektor();
		glRotatef( 180, 1,0,0);
		glTranslatef(0, 0, -ROTATIONCENTER * RingSize  * jpPrefs.objectFaktor * o->size );
		
	}
	glScalef(jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size);                                       // scale to desired size
	glRotatef( 90,1,0,0);
	//    glRotatef(90,0,1,0);
	// call rendering routines
	renderRing();
	glPopMatrix();
}

#undef  ROTATIONCENTER
#undef  CATCHPOS
#define ROTATIONCENTER  .0
#define CATCHPOS                .5
void projectPancake( _object *o, sysColor c ) {
	float angle;
	Juggler *j;
	Vector3D n;
	Vector3D h,a;
	glDisable(GL_TEXTURE_2D);
	setColorFromVector( c);
	n = o->n;
	a = o->a;
	j = o->catcher.juggler;
	
	glPushMatrix();
	angle =  -1* o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	n = o->n;
	if( o->rTime <= 0 ) {       // hold
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		
		// object rotation
		glRotatef( angle,o->r.x, o->r.y, o->r.z );
		
		// transform to orientation
		n.translateZObjectOnVektor();
		glRotatef( 180, 1,0,0);
		glTranslatef(0,0, -CATCHPOS * RingSize  * jpPrefs.objectFaktor * o->size );
	} else {       // airborn
		h = o->n * (  (  CATCHPOS) * RingSize * jpPrefs.objectFaktor *  o->rTime / (o->vTime + o->rTime));
		a = a + h;
		h = o->nDest * ( ( CATCHPOS) * RingSize * jpPrefs.objectFaktor *  o->vTime / (o->vTime + o->rTime));
		a = a + h;
		// shift to actual position
		glTranslatef(  a.x ,  a.y, a.z );
		
		// object rotation
		glRotatef( angle,o->r.x, o->r.y, o->r.z );
		
		// transform to orientation
		n.translateZObjectOnVektor();
		glRotatef( 180, 1,0,0);
		glTranslatef(0, 0, -ROTATIONCENTER * RingSize  * jpPrefs.objectFaktor * o->size );
		
	}
	glScalef(jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size,jpPrefs.objectFaktor*o->size);                                       // scale to desired size
	glRotatef( 90,1,0,0);
	glRotatef(90,0,1,0);
	// call rendering routines
	renderRing();
	glPopMatrix();
}
// draw all jugglers
void projectAllJugglers( void ) {
	int                     c;
	Juggler         *j;
	sysColor        bodyColor;
	GLfloat mode;
	
	if( jpPrefs.jugglerColor == noJugglers)
		return;
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, jugglerMaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, jugglerMaterialShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,	jugglerMaterialEmission	);
	
	for( c = 0; c < jugglerCount; c++ ) {
		
		j = getJuggler( c );
		if( j->invisible )
			continue;
		
		bodyColor = jpPrefs.bd;
		if( jpPrefs.jugglerColor== multiColorJugglers )
			bodyColor = getListColor( c );
		if( j->colorDefined ) {
			bodyColor = j->color;
		}
		
		if( jpPrefs.showHandPath)
			j->showArmCurve();
		
		if( j->isSittingOnUnicycle())
			projectUnicycle(c);
		
		if( hasBodyTexture ) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, hasBodyTextureIndex);
			glGetTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mode );
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		} else {
			glDisable(GL_TEXTURE_2D);
		}
		
		setColorFromVector( bodyColor );
		
		if( jpPrefs.showJugglerNumbers) {
			
			char number[100], *p;
			char *n = j->getName();
			Vector3D v1, v2, v3;
			float s = 0;
			glLineWidth(2);
			if( n == NULL )
				sprintf(number, "%d", c+1);
			else
				sprintf(number, "%s", n);
			
			if( 0 ){
				glPushMatrix();
				glTranslatef(j->head.x, j->head.y+30, j->head.z );
				v1 = j->viewdirection;
				v2 = Vector3D( 0, 0,1);
				s = v1 * v2;
				
				glRotatef(s,0,1,0);
				
				
#define S .1f
				
				glScalef(S, S , S );
				for (p = number; *p; p++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
				
				glPopMatrix();
			}else
				stroke_output(j->head.x, j->head.y+30, j->head.z,"%s", number);
			
		}
		
		// left upper arm
		drawCylinderLine3( j->ls, j->le, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// right upper arm
		drawCylinderLine3( j->le, j->lh, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// left lower arm
		drawCylinderLine3( j->rs, j->re, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor);
		// right lower arm
		drawCylinderLine3( j->re, j->rh, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// left upper leg
		drawCylinderLine3( j->lb, j->lk, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// left lower leg
		drawCylinderLine3( j->lk, j->lf, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// light lupper leg
		drawCylinderLine3( j->lb, j->rk, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		// light lower leg
		drawCylinderLine3( j->rk, j->rf, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
		
#if 1
		
		if( jpPrefs.jointObjects) {
			float f = .95f;
			
			// draw some balls to hide the gaps at all enkles
			drawBall3( &j->ls, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
			drawBall3( &j->rs, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
			drawBall3( &j->le, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
			drawBall3( &j->re, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
			drawBall3( &j->lk, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
			drawBall3( &j->rk, (j->lineSize * f) * jpPrefs.bodyFaktor, jpPrefs.levelOfDetail, bodyColor);
		}
#endif
		
#if 0
		// show normal vektor
		// for debugging only
		{
			vektor pos,n;
			
			pos = j->hb;
			n = j->n;
			
			n = vektorMult( &n, 100);
			n = vektorSumme( &pos, &n);
			
			drawCylinderLine3( &pos, &n, 1.f, 1.f, bodyColor );
		}
#endif
		
		
		if( hasBodyModel ) {
			float f;
			
			
			Vector3D a = j->viewdirection, b = aZ;
			/* angle = a * b;
			 angle = max( -1,angle);
			 angle = min(1,angle);
			 f =  radToGrad(acos(angle)); */
			f = getAngle(a,b);
			if( (j->viewdirection * aX) > 0 )
				f *= -1;
			glPushMatrix();
			glTranslatef(j->lb.x, j->lb.y, j->lb.z);
			aY.translateZObjectOnVektor();
			glRotatef(90-f, 0,0,1);
			
			glCallList( body_list );
			
			glPopMatrix();
			
		}
		if( hasArmModel) {
			Vector3D v;
			float f;
			Vector3D a = j->viewdirection, b = aZ;
			static int r;
			/*angle = a * b;
			 angle = max( -1,angle);
			 angle = min(1,angle);
			 f =  radToGrad(acos(angle)); */
			f = getAngle(a,b);
			if( (j->viewdirection * aX)> 0 )
				f *= -1;
			
			v = j->ls - j->le;
			v.normalised();
			glPushMatrix();
			glTranslatef(j->le.x, j->le.y,j->le.z);
			v.translateZObjectOnVektor();
			glRotatef(r++, 0,0,1);
			glCallList( arm_list );
			glPopMatrix();
			{
				punkt p;
				p = getUpOfDir( v );
				p = p * 50.f;
				p = p + j->le;
				drawCylinderLine3( j->le, p, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor);
			}
			/*      v = vektorDiff( &j->rs, &j->re);
			 vektorNormal(&v);
			 glPushMatrix();
			 glTranslatef(j->re.x, j->re.y,j->re.z);
			 //      glRotatef(f+180, v.x,v.y,v.z);
			 translateZObjectOnVektor(&v);
			 glRotatef(90-f, 0,0,1);
			 glCallList( arm_list );
			 glPopMatrix();
			 */
			
		}
		
		
		// head
		if( !hasBodyModel) {
			float radius = j->headSize * jpPrefs.bodyFaktor;
			GLUquadricObj *theQuadric;
			GLfloat mode;
			GLfloat w;
			GLfloat v[4];
			
			glGetFloatv(GL_CURRENT_COLOR, v);
			
			
			// shoulders
			drawCylinderLine3( j->ls, j->rs, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor);
			// body
			drawCylinderLine3( j->lb, j->hb, j->lineSize * jpPrefs.bodyFaktor, j->lineSize * jpPrefs.bodyFaktor, bodyColor );
			// head
			
			if( hasHeadTexture && jpPrefs.texturedHead ) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, hasHeadTextureIndex);
				glGetTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mode );
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				setColorFromVector(white);
			} else {
				glDisable(GL_TEXTURE_2D);
				setColorFromVector (bodyColor);
			}
			glPushMatrix();
			theQuadric = gluNewQuadric();
			gluQuadricTexture(theQuadric, hasHeadTexture && jpPrefs.texturedHead );
			glGetFloatv( GL_LINE_WIDTH, &w);
			glLineWidth( 1 );
			glTranslatef(j->head.x, j->head.y, j->head.z);
			j->viewdirection.translateZObjectOnVektor();
			glRotatef( -120,1,0,0);
			gluSphere(theQuadric, radius  , jpPrefs.levelOfDetail*2, jpPrefs.levelOfDetail*2);
			glLineWidth( w );
			gluDeleteQuadric( theQuadric);
			glPopMatrix();
			
			if( jpPrefs.blindfolded ) {
#define N 10
				int c;
				float r1 = radius * 1.1;
				float r2 = radius * .93;
				float r3 = r1 *.95;
				float h = 8.f;
				
				setColorFromVector(punkt(.3f,.3f,.3f));
				
				
				glPushMatrix();
				
				glTranslatef(j->head.x, j->head.y, j->head.z);
				j->viewdirection.translateZObjectOnVektor();
				
				glBegin( GL_QUADS );
				
				for( c = 0; c < N; c++ ) {
					float z1, z2, x1,x2;
					
					z1 = sin( gradToRad(360./N * c) );
					z2 = sin( gradToRad(360./N * (c+1)) );
					x1 = cos( gradToRad(360./N * c) );
					x2 = cos( gradToRad(360./N * (c+1)) );
					
					glVertex3f( x1 * r1, 0, z1 * r1);
					glVertex3f( x1 * r2, h, z1 * r2);
					glVertex3f( x2 * r2, h, z2 * r2);
					glVertex3f( x2 * r1, 0, z2 * r1);
				}
				
				
				// endings
				glVertex3f( 0.4 * r3,   5, -.9 * r3 );
				glVertex3f( 0.6 * r3, -30, -1 * r3 );
				glVertex3f( 0.0 * r3, -25, -1 * r3 );
				glVertex3f( 0.1 * r3,   5, -1 * r3 );
				
				glVertex3f( 0.4 * r3,   5, -.9 * r3 );
				glVertex3f(-0.1 * r3, -27, -1 * r3 );
				glVertex3f(-0.6 * r3, -40, -1 * r3 );
				glVertex3f( 0.0 * r3,   5, -1 * r3 );
				glEnd();
				
				glPopMatrix();
			}
			
			if( hasHeadTexture && jpPrefs.texturedHead  ) {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
			}       
		}
		// They will get their numbers back, later...
		/*                      if( jpPrefs.jNumbers && p2.visible)
		 {
		 
		 old mac stuff
		 Str255 str;
		 short size;
		 size = max(3,12 * p2.relSize);
		 TextSize(  size );
		 str[0] = sprintf((char*)str+ 1, "%ld", c+1 );
		 drawStringExt(  str, p2.x - size/2 ,p2.y+size/2,  srcOr, bodyColor);
		 }
		 */
		
		if( hasBodyTexture ) {
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
			//                      glLightfv(GL_LIGHT0, GL_AMBIENT, oldAmbient);
		}
	}
	glDisable( GL_TEXTURE_2D);
	
	
#undef drawCylinder3
}

// rotate p around r, by alpha degrees
// vektor rotateAroundVektor( vektor *p, float alpha, vektor *r)
Vector3D getUpOfDir( Vector3D in ) {
	float r, t;
	Vector3D n, o1 = Vector3D(0,0,-1), o2 = Vector3D(0,-1,0);
	Vector3D out;
	
	// rotate to direction
	n = in.normalised();
	t = n.y;
	n.y = 0;
	
	n = n.normalised();
	r = getAngle( n, o1);
	if( n.x > 0 )
		r*=-1;
	
	out = n.rotateAroundVektor(180+r, o2);
	// bend to angle
	n = in;
	n = n.normalised();
	r = -getAngle( n, o2);
	out = out.rotateAroundVektor( 90+r, aX);
	
	return out;
}

/* Draw a ring */
void renderRing(void) {
	int i, j, k;
	double s, t;
	double x, y, z;
	double twopi;
	float rc = RingSize  * jpPrefs.objectFaktor * .5 * .2;
	int numc = 5;
	float rt = RingSize  * jpPrefs.objectFaktor * .5;
	int numt = 2 * jpPrefs.levelOfDetail;
	
	twopi = 2 * myPi;
	glPushMatrix();
	glRotatef( 90,0,1,0);
	glScalef( 1.f,1.f,.21f);        // rings are flat
	glScalef( .8f,.8f,.8f); // rings are smaller
	for (i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= numt; j++) {
			for (k = 1; k >= 0; k--) {
				s = (i + k) % numc + 0.5;
				t = j % numt;
				x = cos(t * twopi / numt) * cos(s * twopi / numc);
				y = sin(t * twopi / numt) * cos(s * twopi / numc);
				z = sin(s * twopi / numc);
				glNormal3f(x, y, z);
				x = (rt + rc * cos(s * twopi / numc)) * cos(t * twopi / numt);
				y = (rt + rc * cos(s * twopi / numc)) * sin(t * twopi / numt);
				z = rc * sin(s * twopi / numc);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glPopMatrix();
}

/** Zeichnet Keulen auf die Graphikausgabe.
 * Wenn mit Keulen Jongliert wired, ist dies der Hotspot im Code.
 * Deshalb sind hier eine Zeitoptimierungen drin, die nicht
 * so offensichtlich lesbar sind.
 * TR */
void renderClub( void ) {
	GLdouble r;
	GLfloat z;
	GLint i, j;
	const GLfloat stacks = ClubPoints;
	const GLfloat topRadius = 1;
	const GLfloat baseRadius = 1;
	const GLdouble  height = ClubSize;
	const GLint slices = 2 * jpPrefs.levelOfDetail;
	const GLfloat du = 1.f / slices;
	GLfloat tcx = 0.f, tcy = 0.f;
	GLboolean textured;
	//GLfloat nsign = 1.0;
	const GLdouble da = 2.0*myPi / slices;
	const GLdouble dr = (topRadius-baseRadius) / stacks;
	const GLfloat nz = (baseRadius-topRadius) / height;  /* Z component of normal vectors */
	const GLfloat normalDivisor = sqrt(1.f + nz*nz);     // Teiler zur schnellen Normierung
	// Bedenke: ist Verk√ºrzung von sqrt( sin^2 + cos^2 + nz^2)
	GLfloat x1,y1,x2,y2;
	GLdouble rf;  // Zwischenspeichern der Multiplikation r * f , damit diese nicht 4 mal durchgef√ºhrt werden mus
	
	glGetBooleanv( GL_TEXTURE_2D, &textured );
	// Durch durchtauschen der Ergebnisse werde sin und cos nur halb sooft gebraucht
	x2 = 0.; // = sin (0)
	y2 = 1.; // = cos (0)
	for (i=0; i < slices;i++) {
		//x1 = -sin(i*da);
		//y1 = cos(i*da);
		x1 = x2;
		y1 = y2;
		x2 = -sin((i+1)*da);
		y2 = cos((i+1)*da);
		
		z = 0.0;
		r = baseRadius;
		tcy = 0.0;
		
		glBegin( GL_QUAD_STRIP );
		for (j=0;j<=stacks;j++) {
			
			rf = r * club[j][1];
			z = club[j][2];
			
			
			// ALt war:
			// normal3f( x1*nsign, y1*nsign, nz*nsign );
			// Warum? nsign ist konstant = 1 und der Vektor ist wegen
			// sqrt (sin^2 + con^2) = 1 schneller normiert
			// somit schneller:
			glNormal3f(x1/normalDivisor,y1/normalDivisor,nz/normalDivisor);
			if( textured)
				glTexCoord2f(tcx, tcy);
			glVertex3f( x1*rf, y1*rf, z );
			// ALt war:
			// normal3f( x2*nsign, y2*nsign, nz*nsign );
			// Warum? nsign ist konstant = 1 und der Vektor ist wegen
			// sqrt (sin^2 + con^2) = 1 schneller normiert
			// somit schneller:
			glNormal3f(x2/normalDivisor,y2/normalDivisor,nz/normalDivisor);
			if( textured)
				glTexCoord2f(tcx+du, tcy);
			glVertex3f( x2*rf, y2*rf, z );
			r += dr;
			tcy  = club[j+1][2]/ club[ClubPoints][2];
			
		}
		tcx += du;
		glEnd();
	}
}

void renderFish( void ) {
	GLdouble da, r, dr, dz;
	GLfloat z, nz, nsign;
	GLint i, j;
	GLfloat stacks = FishPoints;
	GLfloat topRadius = 1, baseRadius = 1;
	GLdouble height = FishSize;
	GLint slices = 2 * jpPrefs.levelOfDetail;
	GLfloat du = 1.0 / slices;
	GLfloat tcx = 0.0, tcy = 0.0;
	GLboolean textured;
	
	nsign = 1.0;
	da = 2.0*myPi / slices;
	dr = (topRadius-baseRadius) / stacks;
	dz = height / stacks;
	nz = (baseRadius-topRadius) / height;  /* Z component of normal vectors */
	
	glGetBooleanv( GL_TEXTURE_2D, &textured );
	for (i=0; i < slices;i++) {
		GLfloat x1 = -sin(i*da);
		GLfloat y1 = cos(i*da);
		GLfloat x2 = -sin((i+1)*da);
		GLfloat y2 = cos((i+1)*da);
		
		z = 0.0;
		r = baseRadius;
		tcy = 0.0;
		
		glBegin( GL_QUAD_STRIP );
		for (j=0;j<=stacks;j++) {
			GLfloat f;
			
			f = fish[j][1];
			z = fish[j][2];
			normal3f( x1*nsign, y1*nsign, nz*nsign );
			if( textured)
				glTexCoord2f(tcx, tcy);
			glVertex3f( x1*r*f, y1*r*f, z );
			normal3f( x2*nsign, y2*nsign, nz*nsign );
			if( textured)
				glTexCoord2f(tcx+du, tcy);
			glVertex3f( x2*r*f, y2*r*f, z );
			r += dr;
			{
				float d = fish[FishPoints][2];
				tcy  = fish[j+1][2]/d;
			}
		}
		tcx += du;
		glEnd();
	}
}
void _testJuggler( int c) {
	Juggler *j = getJuggler( c );
	Vector3D a,n;
	punkt p1, p2;
	Vector3D norm, h;
	return;
	//              drawCylinderLine3( &j->ls, &j->le, lineSize * jpPrefs.bodyFaktor, lineSize * jpPrefs.bodyFaktor, bodyColor );
	p1 = j->le;
	p2 = j->ls;
	n = p1 - p2;
	a = p2;
	glPushMatrix();
	// shift to actual position
	glTranslatef(  a.x ,  a.y, a.z );
	
	// transform to orientation
	norm = n.translateZObjectOnVektor();
	glScalef(jpPrefs.objectFaktor,jpPrefs.objectFaktor,jpPrefs.objectFaktor);                                       // scale to desired size
#if 01
	
	glBegin(GL_LINES);
	glLineWidth( 1);
	glVertex3f (0,0,0);
	glVertex3f (50,0,0);
	glEnd();
#endif
	//      renderClub( );
	
	glPopMatrix();
	
	h = p2 + norm;
	drawOneLine3(p2.x, p2.y, p2.z, h.x,h.y,h.z);
}


void projectAllJugglerPaths( void ) {
	int c;
	if (!Juggler::walkMode)
		return;
	
	
	for( c = 0; c < jugglerCount; c++ )
		projectJugglerPath( c );
	
}

/*
  t is the time we´re looking for
  low and high are the real limitators of the interval
  as t can be lower than low and higher than high we may end up with 2 subintervals, 
  one includes the beginning of the interval
  and the other includes the end of the pattern
  or we may just have the original inverval, if it does not intersect the start / end
*/

/*
 we need the same for the real jugglers movement when we work with an offset
 Will also make it possible to use negative offsets, great
 
*/
bool splitInSubIntervals( float t, float low, float high, float *l1, float *h1, float *l2, float *h2)
{	
	bool hasSubIntervals;
	
	if( low < 0 ){
		*l1 = 0;
		*h1 = high;
		*l2 = low + t;
		*h2 = t;
		hasSubIntervals = true;
	}else if (high >= t ){
		*l1 = low;
		*h1 = t;
		*l2 = 0;
		*h2 = high - t;
		hasSubIntervals = true;
	}else{
		*l1 = low;
		*h1 = high;
		*l2 = *h2 = -1;
		hasSubIntervals = false;
	}
	
	return hasSubIntervals;
}

#if 0

bool isInInterval( float v, float low, float high );
bool isInInterval( float v, float low, float high )
{
	return v >= low && v <= high && low>= 0.;
}
#else 
#define isInInterval( v, low, high) (((v)>=(low)) && ((v)<=(high)) && ((v) >= 0.f))
#endif
#define STEP .1
#define TIME_BEFORE 1.f
#define TIME_TO_SEE 1.5f
void projectJugglerPath( int nr ) {
	
	sysColor theColor;
	int posCount;
	_jm *jm = firstJm[nr];
	float t;
	float tPast, tFuture;
	bool drawLine;
	float w;
	float futureL1, futureH1, futureL2, futureH2;
	float pastL1, pastH1, pastL2, pastH2;
	bool futureHas2Intevals, pastHas2Intevals;
	float f1, f2;
	
	
	theColor = getListColor( nr );
	
	// get number of positions for this juggler
	posCount = 0;
	while( jm ) {
		jm=jm->next;
		posCount++;
	}
	
	/* heres part 2 of the hack:
	 We have to hide the 2nd half of the values when we print the path (see parseSplineJugglerMovement )
	 */
	posCount *= .5;
	
	
	// no movement, no spline
	if( posCount == 0)
		return;
	
	// if we don´t show the whole path, we focus on this part
	tPast = aTime - TIME_BEFORE;
	tFuture = aTime + TIME_TO_SEE;

	futureHas2Intevals = splitInSubIntervals(  patternTime,  aTime,  tFuture, &futureL1, &futureH1, &futureL2, &futureH2);
	pastHas2Intevals = splitInSubIntervals(  patternTime,  tPast, aTime, &pastL1, &pastH1, &pastL2, &pastH2);

//	_myPrintf("[%2.4f %2.4f] => [%2.4f %2.4f]  [%2.4f %2.2f]\n", aTime,  tFuture, futureL1, futureH1, futureL2, futureH2);
//	_myPrintf("[%2.4f %2.4f] => [%2.4f %2.4f]  [%2.4f %2.2f]\n", tPast, aTime,  pastL1, pastH1, pastL2, pastH2);
	
	/*-----------------------------
	 Path of Positions
	 ------------------------------*/
	
	//	über hermite curves abbilden, entsprechend der Bewegung der Jongleure
	if(jpPrefs.showJugglerPos ) {
		for( t = 0; t < patternTime; t += STEP) {
			punkt v1, v2;
			
			
			// which style of line do we need?
			drawLine = true;
			
			w = 0.5f;
			if( isInInterval( t, pastL1, pastH1) || isInInterval( t, pastL2, pastH2) )
				w = 1.f;
			else if( isInInterval( t, futureL1, futureH1) || isInInterval( t, futureL2, futureH2) )
				w = 2.5f;
			else
				drawLine = showShortTimeMovementPaths == false;
			
			if( drawLine ){
				
				 // draw this inverval with the calucleted thickness
				 
				 f1 = t;
				 f2 = t + STEP;
				 
				 v1 = getPosOfJugglerAtOrAfter(nr, f1);
				 v2 = getPosOfJugglerAtOrAfter(nr, f2);
				 
				 drawCylinderLine3(v1, v2, w, w, theColor);
			} 

		}
		
	}
	
	theColor = getListColor( nr );
	setColorFromVector (theColor);

	/*-----------------------------
	 Path of Points Of Interest
	 ------------------------------*/
	if(jpPrefs.showJugglerPoi ) {
		Juggler *j;
		Vector3D curentPoi;
		// draw viewing dir
		j = getJuggler( nr );
		
		curentPoi = getPoiOfJugglerAtOrAfter(nr, aTime);
		curentPoi.y = j->head.y;
		
		drawCylinderLine3(j->head, curentPoi, 1, 1, theColor);
		drawBall3( &curentPoi, 3, jpPrefs.levelOfDetail, theColor );
		for( t = 0; t < patternTime; t += STEP) {
			punkt v1, v2;
			
			
			// which style of line do we need?
			drawLine = true;
			
			w = 0.25f;
			if( isInInterval( t, pastL1, pastH1) || isInInterval( t, pastL2, pastH2) )
				w = 0.75f;
			else if( isInInterval( t, futureL1, futureH1) || isInInterval( t, futureL2, futureH2) )
				w = 1.5f;
			else
				drawLine = showShortTimeMovementPaths == false;
			
			if( drawLine ){
				
				// draw this inverval with the calucleted thickness
				
				f1 = t;
				f2 = t + STEP;
				
					
				v1 = getPoiOfJugglerAtOrAfter(nr, f1);
				v2 = getPoiOfJugglerAtOrAfter(nr, f2);
				v1.y = v2.y = j->head.y;
				
				drawCylinderLine3(v1, v2, w, w, theColor);
			}
			
		}
		
	}	
}

// get position c for juggler nr at index i
_jm *getJMFromIndex( int nr, int i ) {
	int posCount, tn;
	_jm *jm = firstJm[nr];
	_jm *tjm;
	
	// get number of positions for this juggler
	posCount = 0;
	while( jm ) {
		jm=jm->next;
		posCount++;
	}
	
	/* heres part 2 of the hack:
	 We have to hide the 2nd half of the values when we print the path (see parseSplineJugglerMovement )
	 */
	posCount *= .5;
	
	if( i )
		while( i >= posCount )
			i-= posCount;
	
	
	tjm = firstJm[nr];
	for( tn = 0; tn < i; tn++ )
		tjm = tjm->next;
	
	return tjm;
}


// float should be the absolute pattern time,
// but is curently a mix,
// where the interer part names the position in list
// from frictional value the position in the curve is calculated
punkt getJugglerWalkPosition( int nr,  float c) {
	
	punkt pos;
	_jm *tjm;
	_jm *tjmn;
	
	tjm = getJMFromIndex( nr, c );
	tjmn = getJMFromIndex( nr, c + 1 );
	
	
	pos = getPointFromHermite( tjm->pos, tjmn->pos, tjm->posOut, tjmn->posIn, c - (int)c);
	
	
	return pos;
}


// get position of juggler nr at pattern time t
punkt getPosOfJugglerAtOrAfter( int nr,  float t) {
	
	punkt pos;
	static _jm *tjm, *last;
	//      static _jm *ltjm;
	float dt, rt;
	
 	if( Juggler::walkMode == false) //	if( nr >= numberOfRunaroundJugglers )	
		return getJuggler(nr )->position;
	
	rt = t / patternTime;
	
	last = getJmOfJugglerBefore( nr, t);
	tjm = getJmOfJugglerAtOrAfter(nr, t );
	
	// dt: time for this movement
	dt = tjm->t - last->t;
	
	if( dt < 0 )
		dt += patternTime;
	
	t -= last->t;
	
	if( t < 0 )
		t += patternTime;
	
	
	rt = t /dt;
	
	
	pos = getPointFromHermite( last->pos, tjm->pos,last->posOut, tjm->posIn, rt);
	
	
	return pos;
}

// get position of juggler nr at pattern time t
punkt getPoiOfJugglerAtOrAfter( int nr,  float t) {
	
	punkt poi, pos;
	static _jm *tjm, *last;
	float dt, rt;
	
 	// if walking does not work dor some reason:
	// return current position
	
	if( Juggler::walkMode == false) //	if( nr >= numberOfRunaroundJugglers )	
		return getJuggler(nr )->position + getJuggler(nr)->viewdirection.normalised() * 100;
	
	// my data structures for the last and the next saved moment
	last = getJmOfJugglerBefore( nr, t);
	tjm = getJmOfJugglerAtOrAfter(nr, t );
	
	
	// dt: time in between this movements
	dt = tjm->t - last->t;
	if( dt < 0 )
		dt += patternTime;
	
	// normalize time to relative time
	t -= last->t;
	if( t < 0 )
		t += patternTime;
	
	// fraction of the time passed
	rt = t /dt;
	
	
	poi = getPointFromHermite( last->poi, tjm->poi, last->poiOut, tjm->poiIn, rt);
	
	return poi;
}


_jm *getJmOfJugglerAtOrAfter( int nr,  float t) {
	_jm *p = getJmOfJugglerBefore( nr, t);
	
	if( p->next == NULL )
		p = firstJm[nr];
	else
		p = p->next;
	
	return p;
}



_jm *getJmOfJugglerBefore( int nr,  float t) {
	_jm *tjm;
	
	
	if( (tjm = firstJm[nr])->t > t) {
		
		// take the last entry
		while( tjm->next )
			tjm = tjm->next;
	} else {
		// search next entry
		while( tjm->next && tjm->next->t <= t )
			tjm = tjm->next;
		
	}
	
	return tjm;
}


void calcAllObjectDistances( void ) {
	int c, d;
	_object *b1, *b2;
	float f = -1.f;
	minDistanceValue = -1.f;
	
	for( c = 0; c < ballCount; c++ ) {
		b1 = getObject(c);
		b1->shortDistanceFlag = false;
	}
	
	if(jpPrefs.emphaseShortDistanceFlag == false )
		return;
	
	for( c = 0; c < ballCount; c++ ) {
		b1 =  getObject(c);
		
		for( d = c+1; d < ballCount; d++ ) {
			Vector3D vd;
			
			b2 = getObject(d);
			if( b1 == b2 )
				continue;
			
			// dont compare with selfs
			if( jpPrefs.emphaseShortDistanceFlagForSelfs ){
				if( (b1->thrower.juggler == b1->catcher.juggler) || ( b2->thrower.juggler == b2->catcher.juggler))					
					continue;
			}
			
			vd = b1->a - b2->a;
			f = vd.betrag();
			
			
			
			if( f < jpPrefs.minObjectDistance )
				b2->shortDistanceFlag = b1->shortDistanceFlag = true;
		}
		
		if( minDistanceValue < 0 )
			minDistanceValue = f;
		else
			minDistanceValue = min( minDistanceValue, f);
		
		
	}
	
	
}


void projectWarningSphere( _object *o) {
	float			winkel;
	float			f, angle;
	Vector3D		v, z = Vector3D(0,0,1);
	punkt			a, h, n, r;
	Vector3D		rot = Vector3D(1,1,0);
	float			size = jpPrefs.minObjectDistance / 2;
	sysColor		theColor = lightGray;
	
	if( o->shortDistanceFlag)
		theColor = theColor * 2.f ;
	
	glColor4f(theColor.x, theColor.y, theColor.z, distanceBlendValue);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	
	winkel = o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	n = o->n.normalised();
	r = o->r.normalised();
	a = o->a;
	if( o->thrower.h == leftHand )
		winkel *= -1;
	
	if( o->kind != balls ){
		
		h = o->n * (jpPrefs.objectFaktor * size *  o->rTime / (o->vTime + o->rTime));
		a = a + h;
		h = o->nDest * ( jpPrefs.objectFaktor* size *  o->vTime / (o->vTime + o->rTime));
		a = a + h;
	}
	
	
	
	
	// angle of rotation
	f = n * z ;
	angle = radToGrad( acos( f      )  );
	// axis of rotation (cross product)
	v = z.vektorProdukt(n);
	v = v.normalised();
	glPushMatrix();
	
	
	//      glRotatef( winkel , 0, 1, 0);   // rotate desired angle
	drawBall3_1_transparent( &a, size * jpPrefs.objectFaktor, theColor, winkel, &rot);
	
	glPopMatrix();
	
	
	glDisable( GL_BLEND);
}
