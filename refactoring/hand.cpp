/*
 *  hand.cpp
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

#include "hand.h"
#include "headers.h"
#include "jugglers.h"


Hand::Hand()
{
}


Hand::~Hand()
{
}


hCurve* Hand::moveOneHand()
{

    Vector3D pA(20,0,0);
    hCurve *hC;
    _handPos *hM;


    // get actual movement
    hM = actMove[juggler->nr][h];

    // nothing saved, a
    if( hM == NULL ) {
        juggler->setHandSideVektors(h );
        return NULL;
    }
    hC = &hM->hC;

    // Schon da!
    if( hC->vTime > hC->gTime ) {
        pA = hC->p4;
    } else {
        pA = hC->getPointFromHCurve(hC->vTime/ hC->gTime);

    }

    if( h == leftHand ) {
        juggler->lh = (juggler->position + pA);
    } else {
        juggler->rh = (juggler->position + pA);
    }
    return hC;
}

// letzte gespeicherte Position einer Hand abfragen
// ruckelt, wenn groer animationSpeed gewhlt
punkt Hand::getHandPosition() {
    moveOneHand();
    return (h == leftHand? juggler->lh:juggler->rh);
}


Vector3D  Hand::getLowerArmDirection() const {
    Vector3D const e = (h == leftHand? juggler->le:juggler->re);
    Vector3D const d = (h == leftHand? juggler->lh:juggler->rh);
    return (d - e).normalised();
}


Vector3D Hand::getArmDirection( ) const {
    Vector3D const s = (h == leftHand? juggler->ls:juggler->rs);
    Vector3D const t = (h == leftHand? juggler->lh:juggler->rh);
    return (t - s).normalised();
}