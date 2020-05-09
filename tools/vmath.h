/*
 *  vmath.h
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

#ifndef __VMATH__
#define __VMATH__


#include "vtypes.h"


#if defined(__cplusplus)
extern "C++"
{
#endif

// Mathematische Routinen fr joe pass

float gradToRad( float winkel);
float radToGrad( float bogen);


float gerAbstandPG( punkt *O, Vector3D *n, punkt *P);
float gerAbstandPE( punkt O, Vector3D n, punkt P);


punkt projektPG(punkt *o, Vector3D *n, punkt *a, punkt *b );

void buildRotationVektor( Vector3D a, Vector3D b, float *angle, Vector3D *r);
float getAngle( Vector3D a, Vector3D b );


int kgv( int a, int b);


punkt getPointFromSpline( Vector3D *list, int elements, float t);
punkt getPointFromHermite( Vector3D P1, Vector3D P4, Vector3D R1, Vector3D R4, float t);



#if defined(__cplusplus)
}
#endif

#endif
