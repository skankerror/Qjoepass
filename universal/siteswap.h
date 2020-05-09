/*
 *  siteswap.h
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

#ifndef __SITESWAP__
#define __SITESWAP__



#if defined(__cplusplus)
extern "C++"
{
#endif


#include "headers.h"

typedef struct _eventListEntry {        struct _eventListEntry *next;
										sse		*p;
										int		hnd;
										int		nr;
								}eventListEntry;

/*
 calculating the dwell times:
 1. (first in preflight)
        make a list of all throws
        save all important stuff
        dont care if anything doesnt work (no ball etc)
        (...do the rest of the preflight)
 2. (while animating)
        get ListEntry for this hand.
        ss says:
        so many throws later the ball will be used
        I say:

        => get  actualthrow + ss for this hand
           (no match -> take the one afterwards???)
        => get entry before the one we found
        dt <= 0?
        dt += patternTime
        t = ss - dt *dwellFaktur
*/
typedef struct te{      float aTime;					// when has this object been thrown?
						float duration;                 // the duration of the actual event
						float x;						// was it a cross?
						_side who;						// who did it?
						_side to;						// who to throw next time?
						float ss;						// What is the Value?
						float delay;                    // how much is it delayed
						int   muxCount;					// Bei mux MUSS zum selben Zeitpunkte gefangen werden.
														// Wenn dieses Flag gesetzt ist, wird der Wurf bei der Berechnung der Dwellzeit nicht bercksichtigt
						struct te *next;                // pointers to next and previous structur for the same hand
						struct te *prev;
						wwwBoolean used;				// used for calculating dwell times.
						int	orbitNr;					// orbit of the object
						bool registered;
						sse *e;
						int i;
	
					}throwEntry;

eventListEntry                  *getTopLevelEventForJuggler( int i, int nr );
extern eventListEntry			*firstEventPointer;
extern int                      topLevelEventNumber;

throwEntry      *getThrowAt( _side to, float whenToThrow );
throwEntry      *getThrowBefore( throwEntry *t);

void            setSiteswapStart(sseLoop *first );
void            doSiteswap( void );
void            reInitAllObjects( void );
void            setVisibleAllObjects( void );


void            markAllBouncesUndefined( void );


void            disposeDelayedEvents( void );
punkt			rollObject( _object *o);
punkt			throwObject( _object *o);
punkt           bounceObject( punkt *p1, punkt *p2in, float tG, float tV );
punkt           forcebounceObject( punkt *p1in, punkt *p2, float tG, float tV );

extern float    globalDelayList[][2];

extern sseLoop  *loopPointer;
extern sse      *eventPointer;

extern int      ssThrow;
extern float    verstrichen;
extern float    gTime;
extern int		startHand;
extern wwwBoolean	ash;

extern float    remainingTime;                  // RestDauer des aktuellen zeitbestimmenden Wurfes
extern int      runde;
extern int      spinOffset;

extern int      patternLength;
extern int		orbitCount;

extern int      handPreflight;                      // Die Handbewegungen sind noch nicht komplett

extern int		muxCount;


#if defined(__cplusplus)
}
#endif




#endif
