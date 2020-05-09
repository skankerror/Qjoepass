/*
 *  mydefs.h
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

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include "systemswitch.h"
#include "graphics.h"

// ---------------------------------------- constants ----------------------------------------

#ifndef myPi
#define myPi 3.1415926535f
#endif

#define NOWHERE (-13524)        // Unbekannter Punkt
#define UNDEFINED NOWHERE

#ifndef true
#define false   0 
#define true    1
#endif


// Using unsigned char causes problems in the animation. 
// looks like result of calculation is beeing stuffed into var, hoping that it converts to 1 if the result is !=0
// fixed on 11.6.05, checked on mac os x
#if 01
#define wwwBoolean bool
#else
#define wwwBoolean unsigned char 
#endif


#ifdef MAC_VERSION
// mac typen

#define rect Rect

#else
// openGL typen

typedef struct  {
        short                                                   top;
        short                                                   left;
        short                                                   bottom;
        short                                                   right;
}rect;

#endif

#define red     x
#define blue    y
#define green   z

// ---------------------------------------- enums ----------------------------------------
// field-int for jugglers hands
enum { leftHand = 0, rightHand, noHand };
// enum { rightHand = 0, leftHand, noHand };


// ---------------------------------------- function macros----------------------------------------
#define toggle( a )     ((a) = ((a) == false ))
#define setBit(a,b) ((a) |= 1<<(b))                                     // set bit b in a


#ifndef min
#define min( a,b) ((a)<(b)?(a):(b))
#define max( a,b) ((a)>(b)?(a):(b))
#endif



#endif
