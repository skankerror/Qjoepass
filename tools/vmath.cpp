/*
 *  vmath.cpp
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
very simple vector mathematic
(when I startet, I didnt know anything...
german descriptions only. I dont know any of the english terms.
functions are not re-entrant
*/

#include <math.h>


#include "mydefs.h"
#include "vmath.h"
#include "vtypes.h"
#include "constants.h"

/*
|
| Gerichteter Abstand von punkt P zur Ebene A, n
| d = | <n, (P-A) > |
*/
float gerAbstandPE( punkt A, Vector3D n, punkt P ) {
    return n * (P - A);
}

// sin() rechnet in Radiant
// Zum Umrechnen in Grad dient:
float gradToRad( float winkel) {
    return myPi * winkel / 180.f;
}

float radToGrad( float bogen) {
    return  bogen * 180.f / myPi;
}

/*
| Gerichteter Abstand eines Punktes zu einer Geraden:
| d = | n x (P - A ) |
*/
float gerAbstandPG( punkt A, Vector3D n, punkt P) {
    Vector3D t;
    float d;
    float vz;

    t = n.vektorProdukt(P - A);
    vz =  n * t;
    d = t.betrag();

    //vp changed
    if( vz < 0. )
        d = -d;

    return d;
}

// ---------------------- Ebene in R3 ------------------------------

/*
 Schnittpunkt Ebene / Gerade berechnen
 Eingabe:
  Ebene in Normalenform:
        o = Ortsvektor
        n = Normalenvektor
  Gerade in 2Punkt-Form
        a = Punkt1
        b = Punt 2
 Ausgabe:
        p = Projektionspunkt

 Formel:
  g:=   a+(b-a)
  e:=   <x-o,n> = 0     (gesucht wird x e g )
  =>
  1 = <o,n>-<a,n>/<b-a,n>

  t =  a + 1(b-a)
 */
punkt projektPG(punkt o, Vector3D n, punkt a, punkt b ) {
    Vector3D t;
    float d1,d2;


    // Faktoren berechnen
    d1 = (o * n) - ( n * a);
    t =  b - a;
    d2 =  t * n;

    // kein Schnittpunkt
    if( d2 == 0. ) {
        t.x = t.y = t.z = NOWHERE;
        return t;
    }


    // t berechnen
    return  a + (t * (d1 / d2));
}

int ggt(int a, int b) {

    int gross = max(a,b);
    int klein = min(a,b);
    int rest;

    while((rest = gross % klein) > 0) {
        gross = klein;
        klein = rest;
    }
    return klein;

}

// look for smallest common... (dunno the english name)
int kgv( int a, int b) {
    return (a / ggt(a,b)) *b;
}



/* calculate rotation values (axis and angle )
   for an object with orientation vektor == (0,0,-1)
   to start in a, pointing to b
*/
void buildRotationVektor( Vector3D a, Vector3D b, float *angle, Vector3D *r) {
    const Vector3D v = a-b;
    const Vector3D z(0,0, -1);

    // axis of rotation (cross product)
    *r = z.vektorProdukt(v);
    if(r->betrag() == 0) {
        *angle = 0;
        *r = Vector3D(0,-1,0) ; // aY
    } else {
        *angle = getAngle(v,z);
        *r = r->normalised();
    }
}

float getAngle( Vector3D a, Vector3D b ) {
    return radToGrad(acos(min(1,max(-1,a * b))));
}


punkt getPointFromSpline( Vector3D *list, int elements, float t) {
    Vector3D p1, p2, p3, p4, pos, h;
    int i;

    if( elements <= 0 )
        return NullVector;

    // transform to int to find the control points
    i = t;

    // get the actual control points
    //      elements--;
    p1 = list[i%elements];
    p2 = list[(i+1)%elements];
    p3 = list[(i+2)%elements];
    p4 = list[(i+3)%elements];

    t = t - i;

    pos = p1 * ( -0.5f * t*t*t + t*t - 0.5f*t);   // 1
    h = p2 * ( 1.5f*t*t*t - 2.5f*t*t + 1);                // 2
    pos = pos + h;
    h = p3 * ( -1.5f*t*t*t + 2.f*t*t + 0.5f*t);   // 3
    pos = pos + h;
    h = p4 * ( 0.5f*t*t*t - 0.5f*t*t);                    // 4
    pos = pos + h;

    return pos;
}

punkt getPointFromHermite( Vector3D P1, Vector3D P4, Vector3D R1, Vector3D R4, float t) {
    Vector3D s1, s2, s3, s4;
    const float t2 = t*t;
    const float t3 = t*t*t;

    s1 = P1 * ( 2*t3 - 3*t2 + 1);
    s2 = P4 * ( -2*t3 + 3*t2);
    s3 = R1 * ( t3 - 2*t2 + t);
    s4 = R4 * ( t3 - t2);

    return  s1 + s2 + s3 + s4;
}


