/*
 *  vector3D.h
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
#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <math.h>
/**
@author Thomas Ruhroth
*/


#if defined(__cplusplus)
extern "C++"
{
#endif

class Vector3D{
public:
    Vector3D();
    Vector3D(float x, float y, float z);
    Vector3D(const Vector3D& avector);
    ~Vector3D();

    inline float operator *(const Vector3D& secOperator) const;    //Skalarprodukt
    inline Vector3D operator *(double secOperator) const;          //Multiplikation mit Skalar
    inline Vector3D operator +(const Vector3D& secOperator) const; // Vectorsumme
    inline Vector3D operator -(const Vector3D& secOperator) const; // Vectordifference

    Vector3D asVector() const;
    inline float betrag() const;
    inline Vector3D normalised() const;
    Vector3D vektorProdukt(const Vector3D& secOperator) const;
    Vector3D drehungX(float winkel ) const;
    Vector3D drehungY(float winkel ) const;
    Vector3D drehungZ(float winkel ) const;
    Vector3D rotateAroundVektor(float alpha,const Vector3D& r) const;
    Vector3D translateZObjectOnVektor() const;

    float x;
    float y;
    float z;
    static const Vector3D xNormal;
    static const Vector3D yNormal;
    static const Vector3D zNormal;
    static const Vector3D null;
};

/* Da Operatoren-Overloading nicht für native Typen möglich ist, wird
 * hier einiges als Funktion definiert. So wird Z.B. die Multiplikation
 * mit Skalar kommutativ.
 */
Vector3D operator *(double firstOperator,const Vector3D& secOperator);          //Multiplikation mit Skalar

/* INLINE Methoden *********** */

//skalarprodukt
float Vector3D::operator *(const Vector3D& secOperator) const {
    return x * secOperator.x + y * secOperator.y + z * secOperator.z;

}
//Multiplikation mitSkalar

Vector3D Vector3D::operator *(double secOperator) const  //Multiplikation mit
{
    return Vector3D(x * secOperator,y * secOperator,z * secOperator);
}
// Vectorsumme
Vector3D Vector3D::operator +(const Vector3D& secOperator) const {
    return Vector3D(x + secOperator.x,y + secOperator.y,z + secOperator.z);
}
// Vectordifference
Vector3D Vector3D::operator -(const Vector3D& secOperator) const {
    return Vector3D(x - secOperator.x,y - secOperator.y,z - secOperator.z);
}

float Vector3D::betrag() const {
    return (float)sqrt(x * x + y * y +  z * z);
}

Vector3D Vector3D::normalised() const {
    const float b = betrag();
    return b ? (1.f/b) * (*this) : null;
}

#ifdef __cplusplus
}
#endif


#endif
