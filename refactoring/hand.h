/*
 *  hand.h
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

#ifndef HAND_H
#define HAND_H

#include "juggler.h"
/**
@author Thomas Ruhroth
*/
#include "hcurve.h"

class Hand{
public:

 //    int j;                          // Listen-Nr des Jongleurs
     Juggler *juggler;
     int h;                   // Hand des Jongleurs

    Hand();
    Hand(Juggler *aJuggler, int side) {juggler = aJuggler;h = side;};
    ~Hand();

    hCurve* moveOneHand();
    punkt getHandPosition();
    Vector3D getLowerArmDirection() const;
    Vector3D getArmDirection( ) const;

};
typedef Hand _side;
#endif
