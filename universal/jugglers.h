/*
 *  jugglers.h
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
#ifndef __JUGGLERS__
#define __JUGGLERS__



#if defined(__cplusplus)
extern "C" {
#endif


#include "headers.h"

#define NoTime  -1.                                     // Keine Zeit (init)
#define NoIndex -1

#define BASIC_HEIGHT 120.f
#define OAL 45.f                                        // Lnge Oberarm
#define UAL 45.f                                        // Lnge Unterarm
#define OBL 52.f                                        // Lnge Oberbein
#define UBL 57.f                                        // Lnge Unterbein
#define HAND_FAKTOR 1.2f								// Hnde sind breiter als schultern

#define PELVIC_POSITION -60.f
#define BASIC_POSITION  -170.f
#define SHOULDER_POSITION  50.f


    // upper/lower leg ratrio
#define ULLRATIO        (OBL/UBL)
#define JUGGLING_PLANE (UAL*5.f/6.f)    // Abstand Krper/Jonglierebene



    extern _handPos *actMove[JUGGLER_MAX][2];

    enum what{ Throw, Catch, Init };        // what happens

    extern int hc;
    extern float headSize;
    extern float lineSize;
    extern int       posCount;

    void            moveAllJugglers( void );
    void            moveOneJuggler(Juggler *j);

    void            prepareNextJugglerMovement( lmp *theLmp );
    void            resetPositions( void );
    void            initPosition( void );
    void            disposePosition( void );

    _jP                     *getPosition( int t );
    _ort            *getCatchPosition(  int t,  Juggler juggler);
    void            setCatchPosition( _object *o );
    void            setThrowPosition( _object *o  );


    void            initHands( void );                                                      // init module
    void            disposeHands( void );                                           // dispose of moduel

    void            saveThrow( _object *o);                                           // add a throw to memory
    void            saveCatch( _object *o);                                           // dame for catch
    void            setNextHandAction( _side who, float t, int action);     // start movement that is closest to t

    _handPos        *getHandPtr( int t, int what );

    _handPos        *getHandAt( _side s, float whenToThrow );
    _handPos        *getHandAfter( _handPos *t );



    void            nextJugglerView( bool mode );
	void			setCameraToActualJuggerView( void );

#if defined(__cplusplus)
}
#endif

#endif
