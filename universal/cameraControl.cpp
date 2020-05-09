/*
 *  cameraControl.cpp
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

#include "cameraControl.h"
#include "prefDef.h"
#include "mem.h"
#include "vmath.h"
#include "vtypes.h"
#include "systemswitch.h"
#include "myutil.h"
#include "myPrintf.h"
#include "globals.h"
#include "animationView.h"



#define DefCameraPos Vector3D(500,400,0)
#define DefCameraPoi Vector3D(0,0,1)
#define DefCameraUp  Vector3D(0,1,0)
#define MAX_SPLINES 1000

bool	cameraControl = false;
bool	cameraCommands;

bool	vomitMode = false;
int		vomitNumber = 0;
int		tessedJugglerNumber = 0;
bool	tessedMode = false;
Vector3D tessedJugglerViewDir;


Juggler	*tessedJuggler;
punkt	tessedCameraPosition;


int		autoMove = noAutoMove;
float	movementFactor = 1;



int		posSplineCount = 0;

Vector3D posSplineList[MAX_SPLINES] = {
										Vector3D(200,100,00),
										Vector3D(-300,170,200),
										Vector3D(200,100,0100),
										Vector3D(200,100,-200),
										Vector3D(300,500,0),
										Vector3D(200,700,200),
										Vector3D(100,100,100)
									};

float posSplineTime = 0.;

int poiSplineCount = 0;
Vector3D poiSplineList[MAX_SPLINES] = {NullVector,Vector3D(100,0,0),Vector3D(100,100,0)};
float poiSplineTime = 0.;

int upSplineCount =  0;
Vector3D upSplineList[MAX_SPLINES] = {Vector3D(0,100,0),Vector3D(100,0,0),Vector3D(0,0,100)} ;
float upSplineTime = 0.;


Vector3D cameraPos				= DefCameraPos;
Vector3D cameraPoi				= DefCameraPoi;
Vector3D cameraUp				= DefCameraUp;

Vector3D moveCameraPosFrom		= DefCameraPos;
Vector3D moveCameraPosTo		= DefCameraPos;
float cameraMovePosVTime; 
float cameraMovePosGTime;

Vector3D moveCameraPoiFrom		= DefCameraPoi;
Vector3D moveCameraPoiTo		= DefCameraPoi;
float cameraMovePoiVTime;
float cameraMovePoiGTime;

Vector3D moveCameraFrom			= DefCameraPos;
Vector3D moveCameraTo			= DefCameraPos;
float cameraMoveVTime;
float cameraMoveGTime;

Vector3D panVektor				= DefCameraUp;
float cameraPanAngle			= 0;
float cameraPanVTime;
float cameraPanGTime;

Vector3D tiltVektor;
float cameraTiltAngle			= 0;
float cameraTiltVTime;
float cameraTiltGTime;

int actualMovement				= cameraNoMovement;

void moveCamera( void );
void moveCameraPos( void );
void moveCameraPoi( void );
void panCamera( void );
void tiltCamera( void );
void moveAlongSpline(void);
void moveThroughPoints( void );

punkt getPointFromSpline( Vector3D *list, int elements, float t);


void cameraReset( void ) {
	resetPosSpline();
	resetPoiSpline();
	resetUpSpline();
}

void setCameraMode( bool mode ) {
	cameraControl = mode;
}

bool getCameraMode(void ) {
	return cameraControl;
}

void    setCameraCommands( bool mode) {
	cameraCommands = mode;
}

bool hasCameraCommands(void ) {
	return cameraCommands;
}


void cameraSetView(void) {
	
	if( autoMove != noAutoMove) {
		switch( autoMove ) {
			case autoMoveAlongSpline:
				moveAlongSpline();
				break;
			case autoMoveThroughPoints:
				moveThroughPoints();
				break;
		}
	} else {
		switch (actualMovement ) {
			case cameraMovePosAbsolute:
				moveCameraPos();
				break;
			case cameraMovePoiAbsolute:
				moveCameraPoi();
				break;
			case cameraMoveAbsolute:
				moveCamera();
				break;
			case cameraTilt:
				tiltCamera();
				break;
			case cameraPan:
				panCamera();
				break;
				
		}
	}
	
}

void setCameraPan(float angle, float t) {
	actualMovement = cameraPan;
	cameraPanAngle = angle;
	cameraPanVTime = 0;
	cameraPanGTime = t;
}

void setCameraTilt(float angle, float t) {
	actualMovement = cameraTilt;
	cameraTiltAngle = angle;
	cameraTiltVTime = 0;
	cameraTiltGTime = t;
}

void setCameraPos( punkt *pos) {
	if( pos ) {
		cameraPos = *pos;
	}
}

void setCameraPoi( punkt *poi) {
	if( poi ) {
		cameraPoi = *poi;
	}
}

void setCameraUp( punkt *up) {
	if( up )
		cameraUp = *up;
}

punkt getCameraPos( void ) {
	return cameraPos;
}

punkt getCameraPoi( void ) {
		return cameraPoi;
}

punkt getCameraUp( void) {
		return cameraUp;
}


void setCameraPosMovement(  punkt *from, punkt *to, float t) {
	actualMovement = cameraMovePosAbsolute;
	
	if( from ) {
		moveCameraPosFrom = *from;
		cameraPos = *from;
	}
	if( to )
		moveCameraPosTo = *to;
	
	cameraMovePosVTime = 0;
	cameraMovePosGTime = t;
}

void setCameraPoiMovement(  punkt *from, punkt *to, float t) {
	actualMovement = cameraMovePoiAbsolute;
	if( from ) {
		moveCameraPoiFrom = *from;
		cameraPoi = *from;
	}
	
	if( to )
		moveCameraPoiTo = *to;
	
	cameraMovePoiVTime = 0;
	cameraMovePoiGTime = t;
}

void setCameraMovement(  punkt *from, punkt *to, float t) {
	actualMovement = cameraMoveAbsolute;
	if( from ) {
		moveCameraFrom = *from;
		cameraPos = *from;
	}
	if( to )
		moveCameraTo = *to;
	
	cameraMoveVTime = 0;
	cameraMoveGTime = t;
}

void setCameraMoveTo(punkt *to, float t) {
	if( to )
		moveCameraPosTo = *to;
	
	cameraMovePosVTime = 0;
	cameraMovePosGTime = t;
}

void moveCameraPos( void ) {
	Vector3D dir;
	
	// linear movement
	dir = moveCameraPosTo - moveCameraPosFrom ;
	
	if( cameraMovePosGTime == 0 || cameraMovePosVTime >= cameraMovePosGTime ) {
		return;
	} else {
		cameraMovePosVTime += timeStep ;
		cameraMovePosVTime = min( cameraMovePosVTime,cameraMovePosGTime);
		dir = dir * ( cameraMovePosVTime/cameraMovePosGTime);
		cameraPos = moveCameraPosFrom + dir ;
	}
}

void moveCameraPoi( void ) {
	Vector3D dir;
	
	// linear movement
	dir = moveCameraPoiTo - moveCameraPoiFrom;
	
	if( cameraMovePoiGTime == 0 || cameraMovePoiVTime >= cameraMovePoiGTime ) {
		return;
	} else {
		cameraMovePoiVTime += timeStep ;
		cameraMovePoiVTime = min( cameraMovePoiVTime,cameraMovePoiGTime);
		dir = dir * ( cameraMovePoiVTime/cameraMovePoiGTime);
		cameraPos = moveCameraPoiFrom + dir ;
	}
}

void panCamera( void ) {
	
	Vector3D dir;
	
	dir = cameraPoi - cameraPos;
	
	// finished
	if( cameraPanGTime == 0 || cameraPanVTime >= cameraPanGTime ) {
		return;
	} else {       // init
		;
		
		// move
		cameraPanVTime += timeStep ;
		cameraPanVTime = min( cameraPanVTime,cameraPanGTime);
		
		dir = dir.rotateAroundVektor(cameraPanAngle * cameraPanVTime / cameraPanGTime, cameraUp );
		cameraPoi = cameraPos + dir;
	}
}


void moveCamera( void ) {
	Vector3D dir;
	static Vector3D viewDir;
	
	// linear movement
	dir = moveCameraTo - moveCameraFrom;
	
	// finished
	if( cameraMoveGTime == 0 || cameraMoveVTime >= cameraMoveGTime ) {
		return;
	} else {       // init
		if( cameraMoveVTime == 0 ) {
			cameraPos = moveCameraFrom;
			viewDir = cameraPoi - cameraPos;
		}
		
		// move
		cameraMoveVTime += timeStep ;
		cameraMoveVTime = min( cameraMoveVTime,cameraMoveGTime);
		dir = dir * ( cameraMoveVTime/cameraMoveGTime);
		
		cameraPos = moveCameraFrom + dir;
		cameraPoi = cameraPoi + viewDir;
	}
}

void tiltCamera( void ) {
	
	Vector3D r,dir;
	
	
	//vp changed
	dir = cameraPos - cameraPoi;
	r = cameraUp.vektorProdukt(dir);
	
	// finished
	if( cameraTiltGTime == 0 || cameraTiltVTime >= cameraTiltGTime ) {
		return;
	} else {       // init
		;
		
		// move
		cameraTiltVTime += timeStep ;
		cameraTiltVTime = min( cameraTiltVTime,cameraTiltGTime);
		
		dir = dir.rotateAroundVektor(cameraTiltAngle * cameraTiltVTime / cameraTiltGTime, r );
		
		cameraPoi = cameraPos + dir;
		cameraUp = cameraUp.rotateAroundVektor(  cameraTiltAngle * cameraTiltVTime / cameraTiltGTime, r );
	}
}




void resetPosSpline( void ) {
	Vector3D v = DefCameraPos;
	cameraPos = v;
	posSplineTime = 0;
	posSplineCount = 0;
	
}

void resetPoiSpline( void ) {
	Vector3D v = DefCameraPoi;
	cameraPoi = v;
	poiSplineTime = 0;
	poiSplineCount = 0;
}

void resetUpSpline( void ) {
	Vector3D v = DefCameraUp;
	cameraUp = v;
	upSplineTime = 0;
	upSplineCount = 0;
}

bool addPosSplinePoint( punkt p ) {
	setAutoMove( autoMoveThroughPoints );
	if( posSplineCount >= MAX_SPLINES )
		return false;
	
	posSplineList[posSplineCount++] = p;
	return true;
}

bool addPoiSplinePoint( punkt p ) {
	setAutoMove( autoMoveThroughPoints );
	if( poiSplineCount >= MAX_SPLINES )
		return false;
	
	poiSplineList[poiSplineCount++] = p;
	return true;
}

bool addUpSplinePoint( punkt p ) {
	setAutoMove( autoMoveThroughPoints );
	if( upSplineCount >= MAX_SPLINES )
		return false;
	
	upSplineList[upSplineCount++] = p;
	return true;
}


int setAutoMove( int b) {
	int o = autoMove;
	autoMove = b;
	
	return o;
}


void setMovementFactor( float f ) {
	movementFactor = f;
}

void moveThroughPoints( void ) {
	
	// next timeStep
	posSplineTime += timeStep * movementFactor;
	poiSplineTime += timeStep * movementFactor;
	upSplineTime += timeStep * movementFactor;
	if( posSplineCount )
		cameraPos = getPointFromSpline(posSplineList, posSplineCount, posSplineTime );
	if( poiSplineCount ) {
		cameraPoi = getPointFromSpline(poiSplineList, poiSplineCount, poiSplineTime );
	}
	if( upSplineCount ) {
		cameraUp = getPointFromSpline(upSplineList, upSplineCount, upSplineTime );
	}
}
void moveAlongSpline( void ) {
	
	if( posSplineCount )
		cameraPos = getPointFromSpline(posSplineList, posSplineCount, posSplineTime );
	if( upSplineCount )
		cameraUp = getPointFromSpline(upSplineList, upSplineCount, upSplineTime );
	// next timeStep
	posSplineTime += timeStep * movementFactor;
	poiSplineTime += timeStep * movementFactor;
	upSplineTime += timeStep * movementFactor;
	if( poiSplineCount )
		cameraPoi = getPointFromSpline(posSplineList, poiSplineCount, poiSplineTime );
}

void setVomitNumber( int n){
	vomitNumber = n;
}

int getVomitNumber( void ){
	return vomitNumber;
}
void nextVomitObject( void ){
	int i = getVomitNumber( );
	i = (i+1) % ballCount;
	setVomitNumber( i );
}


void vomitSetView(void) {
	_object *o;
	Vector3D t;
	Vector3D pos, poi,up;
	Vector3D n;
	float d;
	
	
	if( (o = getObject( getVomitNumber() ) ) == NULL)
		return;
	d = o->spins * (  o->vTime / (o->vTime + o->rTime)) * 360;
	n = o->n.rotateAroundVektor( d, o->r);
	pos = o->a;
	poi = pos + n;
	up = n.vektorProdukt(o->r);
	
	// upside down, occurs when a juggler catches a pass
	if(o->rTime < 0 && up * aY < 0 )
		up = up * -1;
	
	glLoadIdentity();
	glScalef( jpPrefs.mag,  jpPrefs.mag,  jpPrefs.mag);            
	gluLookAt( pos.x, pos.y, pos.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);
}


void objectSetView(void) {
	_object *o;
	Vector3D t;
	Vector3D pos, poi,up;
	Vector3D n;
	Vector3D bp(0,300,0);
	
	if( (o = getObject( getVomitNumber() ) ) == NULL)
		return;
	pos = bp;//b->a - (b->a-bp).normalised() * 100.;
	poi = o->a;
	up = aY;
	
	
	glLoadIdentity();
	glScalef( jpPrefs.mag,  jpPrefs.mag,  jpPrefs.mag);            
	gluLookAt( pos.x, pos.y, pos.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);
}

void tessedSetView(void) {
	Juggler *j;
	Vector3D t;
	Vector3D pos, poi,up;
	Vector3D n;
	
	
	if( (j = getJuggler( getTessedJugglerNumber() ) ) == NULL)
		return;
#if 0
	
	pos = j->position - tessedCameraPosition;
	pos.y = 200*jpPrefs.mag;
	poi = j->position;
	up = getCameraUp();
#else
	poi = pos = j->position;
	pos.y = 1500*jpPrefs.mag * 100;
	up = tessedJugglerViewDir;
	
#endif
	
	// camera switches to bird view
	// camera hat to be at a relative position to selected juggler
	// i.e. has to move 
	
	glLoadIdentity();
	glScalef( jpPrefs.mag,  jpPrefs.mag,  jpPrefs.mag);            
	gluLookAt( pos.x, pos.y, pos.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);
}


void setTessedJugglerNumber( int n){
	tessedJugglerNumber = n;
}

int getTessedJugglerNumber( void ){
	return tessedJugglerNumber;
}


void nextTessedJuggler( void ){
	int i = getTessedJugglerNumber();
	i = (i+1) % jugglerCount;
	tessedJuggler = getJuggler( i );
	tessedCameraPosition = punkt(1000,200,00);
	tessedJugglerViewDir = tessedJuggler->viewdirection;
	
	
	
	setTessedJugglerNumber( i );
}

