/*
 *  cameraControl.h
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

#ifndef __CAMERA_CONTROL__
#define __CAMERA_CONTROL__

#include "vtypes.h"
#include "mydefs.h"


#if defined(__cplusplus)
extern "C++"
{
#endif


enum {noAutoMove = 0, autoMoveAlongSpline, autoMoveThroughPoints};

enum {  cameraSetPos,                   // ok
                cameraSetPoi,                   // ok
                cameraSetUp,                    // ok
                cameraMovePosAbsolute,  // ok
                cameraMovePoiAbsolute,  // ok
                cameraMoveAbsolute,
                cameraTilt,
                cameraPan,
                cameraNoMovement};

extern Vector3D cameraPos, cameraPoi, cameraUp;

void setCameraMode( bool mode );
bool isCameraMode( void );

void objectSetView(void);
void vomitSetView(void);
void nextVomitObject( void );
int  getVomitNumber( void );
void setVomitNumber( int n );
	
	
void tessedSetView(void);
void nextTessedJuggler( void );
	
	
int  getTessedJugglerNumber( void );
void setTessedJugglerNumber( int n );

	

void cameraReset( void );
void cameraSetView(void);
void setCameraPos( punkt *pos);
void setCameraPoi( punkt *pos);
void setCameraUp( punkt *pos);
punkt getCameraPos( void );
punkt getCameraPoi( void );
punkt getCameraUp( void);	
void setCameraPosMovement(  punkt *from, punkt *to, float t);
void setCameraPoiMovement(  punkt *from, punkt *to, float t);
void setCameraMovement(  punkt *from, punkt *to, float t);
void setCameraMoveTo(punkt *to, float t);
void setCameraPan(float angle, float t);
void setCameraTilt(float angle, float t);
void resetPosSpline( void );
void resetPoiSpline( void );
void resetUpSpline( void );
bool addPosSplinePoint( punkt p );
bool addPoiSplinePoint( punkt p );
bool addUpSplinePoint( punkt p );
int setAutoMove( int b);
void setMovementFactor( float f );
bool getCameraMode(void );
void setCameraMode( bool );
void setCameraCommands( bool );
bool hasCameraCommands( void );


extern int posSplineCount;
extern Vector3D posSplineList[];
extern int poiSplineCount;
extern Vector3D poiSplineList[];
extern int upSplineCount;
extern Vector3D upSplineList[];


#if defined(__cplusplus)
}
#endif


#endif
