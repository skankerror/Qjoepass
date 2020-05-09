/*
 *  vector3D.cpp
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
#include "graphics.h"

#include "vector3d.h"
#include <math.h>
#include "vtypes.h"
#include "vmath.h"

#include "constants.h"
//extern punkt aX;


Vector3D::Vector3D() {}


Vector3D::~Vector3D() {}

const Vector3D Vector3D::xNormal(1,0,0);
const Vector3D Vector3D::yNormal(0,1,0);
const Vector3D Vector3D::zNormal(0,0,1);
const Vector3D Vector3D::null(0,0,0);


Vector3D::Vector3D(float inx, float iny, float inz) {
    x = inx;
    y = iny;
    z = inz;
}

Vector3D::Vector3D(const Vector3D& avector) {
    x = avector.x;
    y = avector.y;
    z = avector.z;
}





Vector3D Vector3D::asVector() const {
    return Vector3D(x,y,z);
}


Vector3D Vector3D::vektorProdukt(const Vector3D& secOperator) const {
    return Vector3D(
               y * secOperator.z - z * secOperator.y,
               z * secOperator.x - x * secOperator.z,
               x * secOperator.y - y * secOperator.x);
}
// Vektor um winkel Grad um X-achse drehen
// x -> x
// y ->  y*cos(a) + z*sin(a)
// z -> -y*sin(a) + z*cos(a)
// ok
Vector3D Vector3D::drehungX(float winkel ) const {
    return Vector3D(
               x,
               (- z) * sin(gradToRad(winkel))  + y * cos(gradToRad(winkel)),
               z * cos(gradToRad(winkel)) + y * sin(gradToRad(winkel)));
}

// Vektor um winkel Grad um Z-achse drehen
// x ->  x*cos(a) + y*sin(a)
// y -> -x*sin(a) + y*cos(a)
// z -> z;
// ok
Vector3D Vector3D::drehungZ(float winkel ) const {
    return Vector3D(
               (- y) * sin(gradToRad(winkel))  + x * cos( gradToRad(winkel)),
               y * cos(gradToRad(winkel)) + x * sin(gradToRad( winkel ) ),
               z);
}

// Vektor um winkel Grad um Y-achse drehen
// z ->  z*cos(a) + x*sin(a)
// x -> -z*sin(a) + x*cos(a)
// y -> y;
// ok
Vector3D Vector3D::drehungY(float winkel ) const {
    return Vector3D(
               (- z) * sin(gradToRad(winkel ) )  + x * cos(gradToRad(winkel)),
               y,
               z * cos( gradToRad( winkel ) ) + x * sin( gradToRad( winkel ) )
           );
}

// rotate p around r, by alpha degrees
Vector3D Vector3D::rotateAroundVektor(float alpha,const Vector3D& r) const {

    // 3rd vektor
    const Vector3D x = r.vektorProdukt(*this );

    // rotation vektor (p= -sin(alpha) * d + cos( alpha ) * x )

    const Vector3D ret = (x * -sin( gradToRad( alpha))) + ((*this) *  cos(gradToRad( alpha)));
    // Avoid Nummeric errors
    return ret.normalised() * betrag();


}
/* Achtung -> Siteneffekt auf GL */
Vector3D Vector3D::translateZObjectOnVektor() const {
    float r;
    Vector3D n = Vector3D(*this);


    // rotate to direction
    // n = this->normalised(); Warum ??? TR
    n.y = 0;

    n = n.normalised();
    r = getAngle( n, Vector3D(0,0,-1));
    if( n.x > 0 )
        r*=-1;

    glRotatef( 180+r, 0,1,0);

    this->rotateAroundVektor( 180+r, aY);

    // bend to angle
    // n = this->normalised();
    r = -getAngle( this->normalised(), Vector3D(0,-1,0));
    glRotatef( 90+r, 1,0,0);
    return rotateAroundVektor( 90+r, aX);
}



Vector3D operator *(double firstOperator,const Vector3D& secOperator) {
    return secOperator * firstOperator;
}
