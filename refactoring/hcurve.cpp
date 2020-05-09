/*
 *  hcurve.cpp
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
#include "hcurve.h"
#include "jugglers.h"
#include "prefDef.h"
#include "vmath.h"
#include "globals.h"

#include <math.h>

// Handauslenkung: Formel
// Einfaches Modell. aber einigermaen realistisch
// knnte in eine Tabelle!
//float vertHandMovement = 8.0;

#if 0
#define handDeflection( b ) (b*0.001)
#elif 0
#define handDeflection( b ) ((b)/(b+.01)*.02)
#else
#define handDeflection( b ) (log(((b))+1.1)/(b)*jpPrefs.vertHandMovement)
#endif
hCurve::hCurve()
{
}


hCurve::~hCurve()
{
}


// Spline version:
// Vergl. Foley, van Dam: Computer Grafic, 485
Vector3D hCurve::getPointFromHCurve(float t)
{
	Vector3D	r12, r42, r;
	double		b, d;
	
	if(userSpecifiedInHandMovement == false ){
		b = r1.betrag();
		d = handDeflection(b) * factor;    // maximale Auslenkung (Asymptote)
		r12 = r1 * d;
	}else {
		r12 =  userSpecifiedInDir;
	}

	
	if(userSpecifiedOutHandMovement == false ){
		b = r4.betrag();
		d = handDeflection(b) * factor;    // maximale Auslenkung (Asymptote)
		r42 = r4 * d;
	}else{
		r42 =  userSpecifiedOutDir;
	}
	
	r = getPointFromHermite( p1, p4, r12, r42, t);
	return r;
	

}
