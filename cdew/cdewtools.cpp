/*
 * cdewtools.cpp
 * joe2
 *
 *  created by wolfgang on 02.12.06.
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

#include "causal_editor.h"

#if CDEW_STANDALONE
// ------------------------------- vector math stuff ------------------------------------
#pragma mark ---------------------vektor math
// Lnge eines Vektors
float betrag(vektor *v)
{
        float t;
        t = v->x * v->x + v->y * v->y +  v->z * v->z;
        return sqrt( t );
}
// Normalenvektor erzeugen
vektor vektorNormal( vektor *v )
{
        float b;
        vektor r = {0,0,0};

        b = betrag( v );
        if( b )
                r = vektorMult( v, 1/b);
        return r;
}
// Vektoren substrakieren
vektor vektorDiff( vektor *A, vektor *B)
{
        vektor s;

        s.x = A->x - B->x;
        s.y = A->y - B->y;
        s.z = A->z - B->z;

        return s;
}
// Vektoren addieren
vektor vektorSumme( vektor *A, vektor *B )
{       vektor S;
        S.x = A->x + B->x;
        S.y = A->y + B->y;
        S.z = A->z + B->z;

        return S;
}
// Vektor mit Skalar multiplizieren
vektor vektorMult( vektor *V, float s)
{       vektor E;
        E.x = V->x * s;
        E.y = V->y * s;
        E.z = V->z * s;

        return E;
}
float gradToRad( float winkel)
{
        return myPi * winkel / 180.;
}
// Skalar-Produkt bilden
float   skalarProdukt( vektor *a, vektor *b)
{       float d;
        d = a->x * b->x + a->y * b->y + a->z * b->z;

        return d;
}
// rotate p around r, by alpha degrees
vektor rotateAroundVektor( vektor *p, float alpha, vektor *r)
{
        vektor r1, r2, ret;
        vektor x;
        float t;
        // 3rd vektor
        x = vektorProdukt(r,p );
        t = betrag( p );
        // rotation vektor (p= -sin(alpha) * d + cos( alpha ) * x )
        r1 = vektorMult ( &x, -sin( gradToRad( alpha)));
        r2 = vektorMult ( p,   cos(gradToRad( alpha)));

        ret = vektorSumme( &r1, &r2);
        ret = vektorNormal( &ret );
        ret = vektorMult(&ret, t);

        return ret;

}
// Vektor-Produkt bilden
vektor vektorProdukt( vektor *a, vektor *b)
{       vektor e;
#if 0
        e.x = a->y * b->z - a->z * b->y;
        e.y = a->x * b->z - a->z * b->x;
        e.z = a->x * b->y - a->y * b->x;
#else
        e.x = a->y * b->z - a->z * b->y;
        e.y = a->z * b->x - a->x * b->z;
        e.z = a->x * b->y - a->y * b->x;
#endif
        return e;
}
float radToGrad( float bogen)
{
        return  bogen * 180.f / myPi;
}
// vektor definieren
vektor  vektorSet( float x, float y, float z)
{       vektor v;
        v.x = x;
        v.y = y;
        v.z = z;

        return v;
}

#endif
