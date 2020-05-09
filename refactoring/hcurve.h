/*
 *  hcurve.h
 *  joe2
 *
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
#ifndef HCURVE_H
#define HCURVE_H

#include "vector3d.h"
/**
@author Thomas Ruhroth
// Hermite curve, according to Foley, van Dam: Computer Grafic, 483ff
*/
class hCurve{
public:
    hCurve();
	~hCurve();
	
	bool		isCatchMovement;
	
	float		gTime;		// total time
	float		vTime;		// spent time
	
	Vector3D	s, d;		// absolute throw, catch positions
	Vector3D	p1, p4;		// relative start- and endpoint
	Vector3D	r1, r4;		// directions at p1, p4 
	Vector3D	userSpecifiedOutDir, userSpecifiedInDir; // overrides the incomming and outgoing velocity vectors for hand movement
	
	float		factor;		// factor for solo/pass actions
	bool		userSpecifiedInHandMovement;	// true when the movement is damped (default)
	bool		userSpecifiedOutHandMovement;	// true when the movement is damped (default)
	
	Vector3D	getPointFromHCurve(float t);
};

#endif
