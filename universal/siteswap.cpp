/*
 yyy
 bool dampMovement in handCurve eingeführt
Richtungsvektoren für Hände beim Werfen und Fangen auswerten
Dämpfung abschalten wenn werte angegebenen
*/


/*
 *  siteswap.cpp
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

#if 01
#define NEW_BOUNCE_STYLE
#endif

#if 0
#define FIX_MUX
#endif

#if 01
#define DO_FLIPS
#endif
/*
Siteswap-Routinen fr Joe Pass!
- Objekte gem Sitewap-Daten in R3 bewegen:
  - Blle
  - Jongleure
- Sound-Routinen aufrufen

Die Liste der Siteswap-Ereignisse wird im File
parseSiteswap.c aus einem Textfile gewonnen.

*/
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "globals.h"

#include "siteswap.h"
#include "myutil.h"
#include "playSound.h"
#include "myPrintf.h"

#include "jugglers.h"
#include "parseSiteswap.h"
#include "prefDef.h"
#include "siteswap.h"
#include "styleScanner.h"
#include "mydefs.h"

#include "vmath.h"
#include "vtypes.h"
#include "world.h"
#include "mem.h"
#include "info.h"
#include "constants.h"
#include "stringtoscreen.h"
#include "pathAccess.h"
#include "fileIO.h"


#ifndef MAC_VERSION
 #include "cameraControl.h"
 #include "animationWin.h"
 #include "systemswitch.h"
#else
 #include "playSound.h"
 #include "stdUtil.h"
#endif

#ifdef MAC_CLASSIC_VERSION
 #include <events.h>
#endif

#include "playSound.h"
#include "editor.h"



#include "JPResources.h"

#define			NO_TIME					-1
#define			NO_ORBIT				-1
_object			*actualObject				= NULL;
int				getOldestObjectCounter	= 0;
sse*			actEvent;

#define         BOUNCE_REDUCTION 1.
float           bounceReduction = BOUNCE_REDUCTION;
void            calcInitialVelocityForBounces(_object *o);
void            calcInitialvelocityForBounces2( _object *o);

bool checkBit( long a, long b);
bool beepOn;
bool isSyncThrowFlag;


typedef struct { 
	float	startValue;
	float	endValue;
	float	rTime, vTime;
	bool	defined;
}_valueTransition;

_valueTransition gravityTransition, speedTransition;	



enum passCorrection{ noCorrection = 0, passCorrection, passBackwardsCorrection, slapCorrection};

int passCorrectionMode = correctPassValues;


#if 01
#define  WW_SWITCH      'W'
#define WW_LEFT         'L'
#define WW_RIGHT        'R'
#endif

enum {ww_left, ww_right};

#define EVEN_OFFSET 0

_object                   *getObjectByEvent( sse *e );
eventListEntry          *getEventListEntryFromEvent( sse *e );
eventListEntry          *firstEventPointer = NULL;
void                    freeEventList( void );
void                    addToEventList( sse *e);
bool                    eventListError;
int                     topLevelEventNumber;
bool					throwFlag;
throwEntry				*getThrowAfter( _side to, float whenToThrow);

eventListEntry			*eventList[JUGGLER_MAX];
void					printEventList( void );

bool _break( void );
#define Break   if(_break())break;

bool allPositionsSet( void );

#define THROW_HEIGHT .7

//sse *fistEventInChain;
//sse *simEvent;

float eventTime = 0;
int spinOffset = 0;
//int errTime;


//Delay
float globalDelayList[JUGGLER_MAX][2];
float localDelayList[JUGGLER_MAX][2];

typedef struct mc {
    int         index;
    float   value, correction;
}
mc;

mc mux_correction[EVENT_MAX];
//int mccmp( mc *a, mc *b );

// Verzgerte Wrfe
typedef struct  {//     int th;                 // Hand-Ereignis-Nummer
    float eTime;            // Dann geschahs
    int runde;              // In welcher Runde geschahs
    float delay;            // So lange noch warten
    float restDelay;        // Restzeit bis zur Auslsung
    sse *event;                     // Dann dies Event auslsen
    int hand;                // Mit dieser Hand werfen
    int muxCount;    // Nr in multiplex chain
}
_dSse;

_dSse                           *delayedEvents;                         // Liste der verzgerten Blle

float newTimeStep = 0.; // noch nix gefunden
int delayCount;


int muxCount;


int th = 0;


int             throwEntryCount;
int             dwellPreflight;
throwEntry      *throwList = NULL;
int             patternLength;
int				orbitCount;

// ------------ Ablauf -------------------
//sse                     *patternStart;
sseLoop         *loopPointer;                           // nchste Schleife
sse             *eventPointer;                          // nchstes Ereignis

int             ssThrow = 0;                            // Welcher Wurf wird gerade geworfen? (War: event)

//--------------------- Variable Parameter ----------------------------
float           aTime;                                          // actual timecode
float           verstrichen;                            // berhang fr timeStep-Anpassung
float           remainingTime = 0.;                     // Verbleibende Dauer des aktuellen Ereignisses (whole -verstrichen)
int             runde;                                          // Anzahl der Musterdurchlufe
bool            canDelay = false;                       // Verzgerte Events werden zwischengespeichert.
bool            movementPreflight;                      // Positionsberechnung der Jongleure
int             handPreflight;                          // Positionsberechnung der Hnde
bool            preflight;                              // preflight -> save positions
bool			muteSound;								// don´t play sound (in extended preflight)
bool            invisibleObjectsFlag = false;

//--------------------- Prototypen ----------------------------
// Ablaufsteuerung
void            resetSiteswap(sseLoop *sseL);
void            resetLocalDelay(void);
sse             *getNextLoop( void );
void            prepareNextLoop( sseLoop *loopPointer );
void            prepareNextCameraMovement( _cameraMovementPointer *p);


int             getAlternateHand( int hand );   // defined in parseSiteswap

// Zeitsteuerung fr preflight
float           getNextPreflightTime( void );

// Initialisierungs- und Abfragefunktionen
_object			*getOldestObject( Juggler *juggler, int hand );
_object			*getUnsetObject( );
void            reInitOneObject( _object *o );
void            ballsToHands( void );
void            resetJugglers( void );
void            resetLoops( void );

// Timingfunktionen
bool             doPassThrow( sse * theEvent);
int              doSyncThrow(  sse * theEvent);
bool             doMultiThrow( sse * theEvent);
bool             doSingleThrow( sse *theEvent);

// animation
void            moveAllObjects( void );
punkt           moveOneObject( _object *o );

#ifdef NEW_BOUNCE_STYLE
punkt           newBounceObject( _object *o );
#else
punkt           bounceObject( punkt p1, punkt p2in, float tG, float tV );
punkt           forcebounceObject( punkt p1in, punkt p2, float tG, float tV );
#endif

void            setThrowValue( _object *o);


// throw times
void            allocThrowList( void );
throwEntry      *addThrowEntry( sse *theEvent, Juggler *j, float delayedTime);
throwEntry      *getThrowAt( _side to, float whenToThrow );
throwEntry      *getThrowBefore( throwEntry *t);
bool            connectTimeList( void );
throwEntry		*getConnection( throwEntry *in);

// delay
void            getDelayMemory();
void            resetDelayMemory( void );
void            addDelayedEvent( sse *theEvent, int theHand);
void            doDelayedEvents( );
_dSse           *getDelayedEvents( int c );

// Styles
void            prepareNextStyle( lsp *l );
void            setStyle( Juggler *j, styleEl *a );
void            addStyleToThrow( _object *o, sse *theEvent);
void            nextThrowInStyle( Juggler *j );

// misc
void            prepareNextLocalCommand( lp *l );
void			setObjectKind( lp *theLp);
void			setObjectSize( lp *theLp);
void			setObjectColor( lp *theLp );
void			setJugglerColor( lp *theLp );
void			setJugglerObjects( lp *theLp );
void			executeLocalCommands( void );

// misc
void            switchHands( sse *theEvent, Juggler *j );
int             checkForPassCorrection(sse *e, Juggler *a, _ort *b);




/*------------------ Ablaufsteuerung -------------------------------------
Musteranfang setzen
Das ganze Muster zweimal durchlaufen,
um alle wesentlichen Positionen zu lernen
Am Ende sind die Orte bekannt, an denen Werfer und Fnger stehen
Beides braucht man, um die Handbewegungen richtig animieren zu knnen
*/
// current
void setSiteswapStart( sseLoop *sseL ) {
    float dt;
    bool cRet;
    int bc;


    //_myPrintf("\n\n\nstarting preflight\n\n");

    beepOn = false;

    dt = timeStep;

    initHands();                    // Handbewegung
    //    patternErr = false;
    preflight = true;
	muteSound = true;
    dwellPreflight = true;

	gravityTransition.defined = false;
	speedTransition.defined = false;
	
    /* learn timing
    throw and catch all balls at least once
    */
    initPosition();
    getDelayMemory();
    resetSiteswap(sseL);


    allocThrowList();

    timeStep = 0;

    /* learn timing */
    bc = 0;
    do {
        doSiteswap();
// find errors in pattern
        timeStep = getNextPreflightTime();
        bc++;
        Break;
    } while( th || bc < ballCount*2 && runde < 2);

    dwellPreflight = false;
    cRet = connectTimeList( );

    // learn walk positions
    initPosition();
    getDelayMemory();
    resetSiteswap(sseL);
    movementPreflight = true;
    timeStep = 0;
    aTime = 0.;
    remainingTime = 0;
    do {
        doSiteswap();
 //       timeStep = getNextPreflightTime();

    } while(  runde < 2 );

    movementPreflight = false;

    // learn the rest of the hand positions
    // just the overhang from what we did before
    while( handPreflight > 0 ) {
        doSiteswap();
 //       timeStep = getNextPreflightTime();
        handPreflight--;
        Break;
    }

	
    timeStep = dt;

    preflight = false;

#if 01
#if 0
//	aTime = 0;
    reInitAllObjects();
	resetInfo();
#endif
	
   {
        resetSiteswap(sseL);
        timeStep = 0.2f;
        // for some strange reasons, we have to do an extra round with preflight off
        do {
            doSiteswap();
        } while( runde < 2 );
    }
#if 0
    loopPointer = sseL;                                                             // start of pattern
    resetLoops();
    //patternStart =
    eventPointer = getNextLoop();    // get first pattern
#else
	;//resetSiteswap(sseL);
#endif
	
#endif
    beepOn = true;
	muteSound = false;
    printEventList( );

	
	//~ if( isBackgroundSoundDefined() ) 
	   //~ playBackgroundSound();
	
    return;
}


bool allPositionsSet( void ) {

    for(int c = 0; c < posCount;c++) {
        if( getPosition(c)->set
                == false ) {
            return false;
        }
    }

    return true;
}

bool _break( void ) {
#ifdef MAC_CLASSIC_VERSION
    if( Button() )
        return true;
    else
        return false;
#else

    return false;
#endif
}

/*
in preflight we look only at the times when something takes place:
+ next throw (delayed and not delayed)
+ next catch
(+ end of movement)
*/
float getNextPreflightTime( void ) {
    int c;
    _object *o;
    _dSse *d;
    Juggler *j;

    // next throw
    newTimeStep = remainingTime; // rest of actual throw
    if( newTimeStep == 0. )
        newTimeStep = eventPointer->duration;

    // catch
    for( c = 0; c < ballCount; c++ ) {
        o = getObject( c );
        // this one is airborne
        if(      o->rTime > 0 ) {
            if( newTimeStep < 0 || newTimeStep > o->rTime ) {
                newTimeStep = o->rTime;
            }
        }
    }

    // movement
    for( c = 0; c < jugglerCount; c++) {
        j = getJuggler( c );
        if( j->rTime > 0 )
            if( newTimeStep < 0 || newTimeStep > j->rTime )
                newTimeStep = j->rTime ;
    }

    // delayed throw
    for( c = 0; c < delayCount; c++ ) {
        d = getDelayedEvents( c );
        if( d->event ) {
            newTimeStep = min( newTimeStep,d->restDelay);
        }
    }

    // should not happen
	// but in case: do an extra round of preflight
    if( newTimeStep <= 0 ) {
        newTimeStep += 1.;
//        beep( );
    }

    return newTimeStep;
}


void resetSiteswap(sseLoop *sseL) {
    loopPointer = sseL;                                                             // start of pattern

    resetLocalDelay();
    resetJugglers();                                                                // std. position
    resetLoops();
    //patternStart =
    eventPointer = getNextLoop();    // get first pattern
    ssThrow = 0;                                                                    // throw number.
    th = 0;                                                                                 // hand number
    remainingTime = 0;                                                              // no time left, start at once
    runde = 0;                                                                              // lap 1
    aTime = 0.;
    handPreflight = 0;
    newTimeStep = 0;
    eventTime = 0.;
    //    simEvent = NULL;

    resetDelayMemory();                                                             // nothing is delayed by now
    reInitAllObjects();
    resetInfo();
	freeEventList();
    //    saveStartHands();
}


void resetLocalDelay(void) {
    for(int c = 0; c < jugglerCount; c++)                              // noone delayes
    {       localDelayList[c][leftHand] = UNDEFINED;
        localDelayList[c][rightHand] = UNDEFINED;
    }
}

// set default position, hands on center
void resetJugglers( void ) {
    int c;
    Juggler *jStart, *j;

    for( c = 0; c < jugglerCount;c++) {

        jStart = getStartPosition( c );
        j = getJuggler( c );
        j->position = jStart->position;
        j->viewdirection = jStart->viewdirection;
        j->offsetHand = jStart->offsetHand;
        j->pStart = jStart->pStart;
        j->pEnd = jStart->pEnd;
        j->hand = jStart->hand;

        //              setHandVektors( getJuggler(c) );
        j->inSync = false;
        j->leaveSync = false;
        j->syncMode = syncLeft;
        j->firstThrow = true;
    }
}

/*
reset all loops
including childs
*/
void resetLoops( void ) {
    sseLoop *fLoop, *oldInLoop = NULL;

    fLoop = loopPointer;                            // first loop

    do {
        fLoop->rest = fLoop->count;             // init with maximum value

        if( oldInLoop != fLoop &&
			fLoop->inTheLoop.typ == loop )  // trace into
        {       fLoop = fLoop->inTheLoop.ssP.loop;
            continue;
        }

        if( fLoop->nextLoop) {
            fLoop = fLoop->nextLoop;                // next loops
        } else {
            fLoop = fLoop->outTheLoop;              // upper level
            oldInLoop = fLoop;                              // we had thisone already
        }
    } while( fLoop != loopPointer );                 // til we reach the end

}


// ----------------- Ablaufsteuerung -------------------------------

// Geht durch die Ereignisliste und lt die Blle werfen
void doSiteswap( void ) {
    sse *e;

    if( !validPattern )             // Nur gltige Muster werfen
        return;
	
	throwFlag = false;

    doDelayedEvents( );             // Verzgerte Ereignisse auslsen

	executeLocalCommands();
	
    remainingTime -= timeStep;
    if( remainingTime <= 0 )                // Ereignis vorbei?
    {
        //        if(!patternErr )
        //            errTime++;


        verstrichen = -remainingTime;                   // Soviel Zeit ist schon rum
        e = eventPointer;                                               // Aktuelles Ereignis

        // next time
        eventTime++;
        if( eventTime >= patternTime )
            eventTime = 0;

        remainingTime = e->duration - verstrichen;// So lange dauert das Ereignis noch

#ifdef __INFO__

        nextEventInfo();

#endif


        doPassThrow( e);                                                        // Das nchste Ereignis auslsen

        eventPointer = eventPointer->next;              // ein Ereignis weiter

        if( eventPointer == NULL )                              // Muster zu Ende. Was kommt jetzt?
        {       eventPointer = getNextLoop();
        }
    }

    moveAllObjects();

    moveAllJugglers();

	if( preflight )
		timeStep = getNextPreflightTime();


    aTime += timeStep;                              // relative time
    if( aTime >=patternTime )                               // fix actual time
        aTime -= patternTime;

	if( throwFlag && jpUntilNextThrow)
		jpAnimate = jpUntilNextThrow = false;
		
}


// All jugglers (pass throws)
bool doPassThrow( sse * theEvent) {
    sse *p;

    p = theEvent;

    topLevelEventNumber++ ;
    topLevelEventNumber %= momentCount*2;
    while( p ) {
        if( doSyncThrow( p) == false )
            return false;

        p = p->jong;
    }

    return true;
}


// sync throws
int doSyncThrow(  sse * theEvent) {
    Juggler *j;
    sse *thisE, *thisSimE;
    int hnd, simHnd;


    //    fistEventInChain = theEvent;    // this information will be addes to all balls that are thrown

    j = getJuggler( theEvent->who );

    thisE = theEvent;
    thisSimE = theEvent->sim;

    // Eingang in sync suchen
    if( thisSimE ) {
		isSyncThrowFlag = true;
        if( j->syncMode == syncLeft )
            j->hand = rightHand;
        if( j->syncMode == syncRight)
            j->hand = leftHand;

        if( j->start != startHand )
            j->hand = getAlternateHand(j->hand);
    }else {
		isSyncThrowFlag = false;
	}


    if( thisSimE )                                  // Synchron
    {       j->inSync = true;                               // Schalter setzen fr: syncchroner part
        j->leaveSync = false;
    } else if( j->inSync)                            // Schalter setzen fr:Jetzt wird es async
    {       j->inSync = false;
        j->leaveSync = true;                    // => ausweg suchen
    }


    //    simEvent = thisSimE;

    hnd = j->hand;
    simHnd = getAlternateHand(hnd);

    doMultiThrow( thisE );                  // Mit aktueller Hand werfen

    //    simEvent = thisE;
    if( thisSimE )                                  // Gibt es Simultan-Ereignisse?
        {       if( !doMultiThrow( thisSimE  ) )
            goto bail;
    }

    //    simEvent = NULL;

    // change beginning flag if hand swich follows on a sync event
    if(thisSimE && thisSimE->sameHand == switchAll) {
        if( j->syncMode == syncLeft )
            j->syncMode = syncRight;
        else if( j->syncMode == syncRight )
            j->syncMode = syncLeft;
    }

bail:

    return noHand;
}




// Multiplex Werfen
// Wir zhlen hier die Wrfe, da sich sonst doSingleEvent und delayedEvents um den
// Ereigniszhler kmmern mssen. (fehlertrchtig)
bool doMultiThrow( sse * theEvent ) {
    sse *m;
    bool ret = true;
    Juggler *j;
	_object *o;
	int tHnd;

    m = theEvent;
    j = getJuggler( m->who );
    muxCount = 0;

    // nothing to do...
	// NB: an empty throw can already hold a club. If there is an object in the hand, and a style is defined, activate it
	
    if(true && theEvent->ss == SS_EMPTY) {
        ret = true;
        addToEventList(theEvent);
    } else {

        while( m )                              // all multiplex-throws
        {       ret &= doSingleThrow( m );
            m = m->mux;
            muxCount++;
        }

        nextThrowInStyle( j );
    }

    // next hand please
    switchHands( theEvent, j );

    return ret;
}




// Lse das nchste Siteswap-Ereignis aus. ab vers 1.3
// D.h.: initialisiert den nchsten Object gem Angaben im Siteswap-Ereignis
// Ein ereignis wei nicht, in welchern Hand es geschieht!
// Im Gegensatz zu den bergeordneten Funktionen mu diese wissen,
// wieviel Zeit schon vergangen ist. Zur Anpassung der Flugzeit.
bool doSingleThrow( sse *theEvent) {
    _object *o, *o1, *o2;
    Juggler *js, *jd;
    bool ret = true;
    int tHnd, cHnd; // hand of thrower, catcher
    float thisDwell;
    float jdw, jdt; // delay for thrower and passer
    float ss;
    float delay;
    float dr;               // dwell ratio
    throwEntry *actThrow, *lastThrow;
    float diffTime = 1.,actTime;
    int dsh, dch;   // hand int for delay-lists

	
    // BAD foult
    if( theEvent == NULL )  // oops
    {       return false;
    }

	actEvent = theEvent;

	
	// before doing anything else:
    // say "beep", if user asked for
	//xxx	curently: why is there no flipß
#if 1
    if(beepOn && theEvent->beep ){
		beep( );
	}
#endif		

    // get jugglers
    js = getJuggler( theEvent->who );
    jd = getJuggler( theEvent->to);

#define STARTHAND if( js->firstThrow )  js->hand = ( js->start == rightHand?rightHand:leftHand)
    // hurried throws are done by the 'other' hand
    if( theEvent->hurried) {
        STARTHAND;
        else
            js->hand = getAlternateHand(js->hand);
    }


    // my little secret: force a special hand to throw now
    if( checkBit(theEvent->dummy, ww_left )) {
        js->hand = leftHand;
    }
    if( checkBit(theEvent->dummy, ww_right )) {
        js->hand = rightHand;
    }


    // get hands used this time
    cHnd = tHnd = js->hand;
    cHnd = (theEvent->cross == true ? getAlternateHand( cHnd): cHnd );
    // the hand to be used for catches (depends on start)
    if( js->start != jd->start ) {
        cHnd = getAlternateHand(cHnd);
    }
    // this is the int to check in delay list
    dsh = tHnd;
    dch = cHnd;






    // delay, if possible:
    // 1. individual
    // 2. lokal
    // 3. global
    // who to catch?
    jdw = localDelayList[theEvent->who][dsh];
    if( jdw == UNDEFINED )
        jdw = globalDelayList[theEvent->who][dsh];
    if( jdw == UNDEFINED )
        jdw = 0;
    jdt = localDelayList[theEvent->to][dch];
    if( jdt == UNDEFINED )
        jdt = globalDelayList[theEvent->to][dch];
    if( jdt == UNDEFINED )
        jdt = 0;

    js->delay = theEvent->delay;
    if(UNDEFINED == theEvent->delay)
        js->delay = jdw;
    else
        js->delay = jdw + theEvent->delay;

    delay = jdw - jdt;
    theEvent->resultingDelay = delay;

    // if this throw is delayed: save it, leave rest for later
    if( canDelay && !theEvent->inDelay && js->delay != 0.) {

        addDelayedEvent( theEvent, tHnd);
        //_myPrintf("resultingDelay = %f\n", theEvent->resultingDelay);
        return true;
    }

	// before doing anything else:
    // say "beep", if user asked for
	//xxx	curently: why is there no flipß
#if 0
    if(beepOn && theEvent->beep ){
		beep( );
	}
#endif	

    /*
    if something doesnt work now, its the pattern.
    Joe did all he could do.
    From now on: just go ahead.
    */

    if( js->leaveSync){
        // look for exit in sync throws
        // get oldest balls for both hands
        o = getOldestObject( getJuggler(theEvent->who),  leftHand );
        o1 = getOldestObject( getJuggler(theEvent->who), rightHand);

        if(!o && !o1){                                   // we are starting up
			o = getUnsetObject();
			if( !o )
				;//beep();         // should never happen xxx
        } else {       // ball still airborne
            if ( o == NULL || o->rTime > 0 ) {       // first try unused ball
                // otherwise, use the one that will reach the hand next
                if( (o2 = getUnsetObject( )) != NULL )
                    o = o2;
            }
            // same for the other hand
            if ( o1 == NULL || o1->rTime > 0 ) {
                if( (o2 = getUnsetObject( )) != NULL )
                    o1 = o2;
            }

            if( o && !o1 )                                                                          // ball only on one side found
                tHnd = js->hand = leftHand;
            else if (!o && o1) {
                tHnd = js->hand = rightHand;
                o = o1;
            } else {
                if( o->nextThrowTime < o1->nextThrowTime )              // use ball that is to be thrown next.
                    tHnd = js->hand = leftHand;
                else if( o->nextThrowTime > o1->nextThrowTime ) {
                    tHnd = js->hand = rightHand;
                    o = o1;
                }
            }
        }
        // now we know which hand has a ball.
        // if its not the one calculated above, switch destination hand
        if( tHnd != dsh ) // dsh is a relict from the delayed stuff from above
            cHnd = getAlternateHand( cHnd);

        siteswapError = true;
    } else
        // normaly, we change hands
    {       //xx why do we not have to switch catchin hand here
        if( theEvent->sameHand == switchThis)   // same hand this time
            tHnd = getAlternateHand( tHnd );

        // get an object
        o = getOldestObject(getJuggler(theEvent->who),  tHnd );
    }

    if( theEvent->hurried ) {
        STARTHAND;
    }
    js->firstThrow = false;

#if 0
    // my little secret: force a special hand to throw now
    if( checkBit(theEvent->dummy, ww_left )) {
        js->hand = leftHand;
    }
    if( checkBit(theEvent->dummy, ww_right )) {
        js->hand = rightHand;
    }
#endif
    // save actual throw
    // we need this to calculate the dwell time
    // must be here, otherwise empty patterns are not valid
	// will not be called in empty patterns (only siteswap 0) 
    if( o && o->orbitNr == NO_ORBIT )
		o->orbitNr = addThrowEntry( theEvent, js, delay )->orbitNr;
	
#if 0
	if(dwellPreflight) {
        goto noError;
    }
#endif
	
    theEvent->handUsedThisTime = js->hand;

    // 0 -> we may not hold anything
    // this is not true because there may be multiplex throws
    if(!theEvent->isMhnHurried ) {
        if( theEvent->ss == 0 && !theEvent->cross) {
            if( o && o->rTime <= 0) // drop it!
            {       reInitOneObject( o );
                if( !theEvent->cross) // error when its a cross
                {
                    goto bail;
                }
            }
            ret = true;
            goto bail;
        }
    } 
	
// 11.7.12 
// The following lines could only be reached when mhn* was off, which does not really makes sense
// When we leave sync pattens, we do this anyway, may they be hurried or not.
//	else
	{       // if we caught something, we know which hand is the next
        if( js->leaveSync)
            js->leaveSync = false;
    }
    // No ball at all, or ball still in the air
    //xx    if (b == NULL || b->rTime > 0 )
    if (o == NULL || o->rTime -timeStep> 0 ) {       // try to take a new one
        if( (o1 = getUnsetObject( )) != NULL )
            o = o1;
        else
        {    // take the one closest to the hand, otherwise
            ret = false;
            siteswapError = true;
        }    
    }

    // no ball: error
    if( o == NULL ) {
        if( jpPrefs.es && !muteSound)
            beep();//
        ret = false;
        goto bail;
    }

    // connect event and ball. (for info and debug)
    theEvent->o = o;
	o->beepThisTime = theEvent->beep;

    // we found something: fill in values
    // jugglers and hands
    o->thrower.juggler = getJuggler(theEvent->who);
    o->thrower.h = tHnd;
    o->catcher.juggler = getJuggler(theEvent->to);
    o->catcher.h = cHnd;

#if 01
	if(dwellPreflight) {
        goto noError;
    }
#endif
    // sound if ball was airborne
    //if( o->rTime > timeStep) {
        //if( jpPrefs.es && !muteSound)
            //doSound( airID, o->thrower,255 );//beep();//
    //}


    thisDwell = (theEvent->dwell == SS_INIT?handDwell:theEvent->dwell);

    ss = theEvent->ss + theEvent->mux_correction;

    if( obeyDelay )
        ss -= delay;

    // fill in values
    o->bounce = theEvent->bounce;
    if( objectBounceFrom != DONT_BOUNCE ) {
        if(ss >= objectBounceFrom )
            o->bounce = stdBounce;
    }
    o->e = theEvent;                                                        // for info-window
    o->delay = theEvent->delay;
    o->t = ssThrow;

    // calulate dwell Time from dwell ratio
    actTime = eventTime + ss;
    if( actTime < 0)
        actTime += patternTime;

	if( (actThrow = getThrowAt(o->catcher, actTime)) == NULL )
		actThrow = throwList;
	o->actThrow =  actThrow;
    lastThrow = actThrow->prev;
    diffTime = actThrow->aTime - lastThrow->aTime;
    if( diffTime <= 0 )
        diffTime += patternTime;


    dr = diffTime*thisDwell;                                        // get flight time from dwell ratio
    o->airtime =  (ss - dr);                                        // save time for hand movement
    if( !theEvent->ss == 0 )                                                        // dont lengthen the 0x
        o->airtime = max( jpPrefs.oneTime/**thisDwell*/, o->airtime );      // a snap also needs some time
	else
		myPrintf("zero\n");
    o->nextThrowTime = ss;

#if 0
	_myPrintf("thisDwell = %f\n", thisDwell);
    _myPrintf("dr        = %f\n", dr);
    _myPrintf("airtime   = %f\n", o->airtime);
#endif
	

    if( theEvent->change != NO_CHANGE )                     // set time explicite
        o->airtime = theEvent->change;
    else {
        if( hurriedMode ) {
            o->airtime = ss - hurriedValue;
            //                      _myPrintf(" corrected to hurried value (%f) %f -> %f\n", hurriedValue, ss, b->airtime  );
        }
        o->airtime = max( o->airtime, jpPrefs.minActionTime);
    }

	o->roll = theEvent->roll;
	
    if( 
	   theEvent->who == theEvent->to
	   && (theEvent->ss == 1 && theEvent->cross == true )
	   ){
		static int shc = 0;
		
//		_myBeep();
//		_myPrintf("short hold %03d!\n", shc++);
	}
	
    // save times
    o->vTime = 0;
    o->rTime = o->airtime - o->vTime;

    // check for hold
    if( 
	   theEvent->who == theEvent->to
        && (theEvent->ss == 2 && theEvent->cross == false )
		&& jpPrefs.h2
        && !beatmapMode
    )
        o->h2 = true;
    else
        o->h2 = false;

	if(beepOn && o->beepThisTime ){
		_myPrintf("beep index = %d\n", getObjectIndex(o ));
	}
	
    addStyleToThrow(o, theEvent);

    setThrowValue( o );

    // save everything

	saveThrow( o );
    saveCatch( o );

    // walk: where am I
    setThrowPosition( o );

    // save infos for jugglers walk
    if(movementPreflight ) {
        handPreflight = max( handPreflight, ceil(o->airtime) );
    }

    // init next hand movement
	if(!theEvent->h2)
		setNextHandAction(o->thrower, o->throwTime, Throw);
    o->thrown = true;
    o->dropped = false;
	o->inHand = false;

//    b->invisible = invisibleObjectsFlag |= theEvent->invisible;

    //if( 10 )
        //if(	jpPrefs.t && !muteSound)
            //doSound( throwID, o->thrower, theEvent->ss);

    // add throw to info
    //      setNextObjectInfo( b);
	
	throwFlag = true;
goto noError;

bail:
#if 0
Hand dummy;
		if( theEvent->ss == 0)
			doSound( throwID, dummy, 0);
#endif

	siteswapError = true;
noError:

    if( siteswapError && !lastSiteswapErrorState)
    {    initOneMessage( validPattern );
        lastSiteswapErrorState = true;
    }    


    // we are through with this throw. counters up!
    th++;                           // hand
    th = th % throwMomentCount;

    ssThrow++;                      // position
    ssThrow = ssThrow % throwCount;

    addToEventList( theEvent );

    if( ssThrow == 0 )      // siteswap
    {       runde++;
        //        saveStartHands();
    }

    return ret;

}

void switchHands( sse *theEvent, Juggler *j ) {
    j->hand = getAlternateHand( j->hand );


     if( !beatmapMode )	{ 
		 // only if not a sync throw
		 if(!isSyncThrowFlag && jpPrefs.switch1x) {
            if( theEvent->ss == 1  && theEvent->cross != true )     // change hands after a 1x
                j->hand = getAlternateHand( j->hand);
        }
    }    

    if( theEvent->sameHand == switchAll )                                   // force order of hands to be changed
        j->hand =  getAlternateHand( j->hand );



}


// set throw of juggler to next in style
void nextThrowInStyle( Juggler *j ) {
    j->sE = j->sE->next;
}

// Schleife vorbereiten
void prepareNextLoop( sseLoop *loopPointer ) {
    int c,d;

    loopPointer->rest = loopPointer->count;                 // initialisieren

    for( c = 0; c < JUGGLER_MAX;c++) {
        for( d = 0; d<2;d++) {
            if( loopPointer->delayList[c][d] >= 0 )
                localDelayList[c][d] = loopPointer->delayList[c][d];
            else if( loopPointer->delayList[c][d] == RES_TO_UNDEFINED )
                localDelayList[c][d] =  UNDEFINED;
        }
    }

    // Alle Bewegungen initialisieren
    prepareNextJugglerMovement( loopPointer->theLmp );
	prepareNextStyle( loopPointer->theLsp );
	prepareNextLocalCommand( loopPointer->theLp );
	prepareNextCameraMovement(loopPointer->cameraMovementPointer);
	
}


void executeLocalCommands( void )
{
	
	int i;
	Juggler *j;
	_object *o;
	
	// set juggler color
	for( i = 0; i < jugglerCount; i++){
		j = getJuggler(i);
		if( j->colorDefined ){
			j->rColorTransitionTime -= timeStep;
			j->vColorTransitionTime += timeStep;
			
			if( j->rColorTransitionTime >= 0){
				
				j->invisible = ( j->color.x < .0 || j->color.y < 0. || j->color.z < 0.);		
				if( !j->invisible)
					j->color = (j->startColor * j->rColorTransitionTime + j->endColor * j->vColorTransitionTime)*(1./(j->vColorTransitionTime+j->rColorTransitionTime));
			}else{
				j->color = j->endColor;
				j->invisible = ( j->color.x < .0 || j->color.y < 0. || j->color.z < 0.);		
			}
		}
	}
	
	// set object color
	for( i = 0; i < ballCount; i++){
		o = getObject(i);
		if( o->colorDefined ){
			o->rColorTransitionTime -= timeStep;
			o->vColorTransitionTime += timeStep;
			
			if( o->rColorTransitionTime >= 0){
				
				o->color = (o->startColor * o->rColorTransitionTime + o->endColor * o->vColorTransitionTime)*(1./(o->vColorTransitionTime+o->rColorTransitionTime));
				o->invisible = ( o->color.x < .0 || o->color.y < 0. || o->color.z < 0.);		
				
			}else{
				o->color = o->endColor;
				o->invisible = ( o->color.x < .0 || o->color.y < 0. || o->color.z < 0.);		
			}
		}			
	}
	
	// set object siue
	for( i = 0; i < ballCount; i++){
		o = getObject(i);
		if( o->sizeDefined ){
			o->rSizeTransitionTime -= timeStep;
			o->vSizeTransitionTime += timeStep;
			
			if( o->rSizeTransitionTime >= 0)				
				o->size = (o->startSize * o->rSizeTransitionTime + o->endSize * o->vSizeTransitionTime)*(1./(o->vSizeTransitionTime+o->rSizeTransitionTime));
			else
				o->size = o->endSize;
		}			
	}
	
	// change gravity
	if( gravityTransition.defined ){
		gravityTransition.rTime -= timeStep;
		gravityTransition.vTime += timeStep;
		if( gravityTransition.rTime <= 0 ){
			setGravity( gravityTransition.endValue);
			gravityTransition.defined = false;
		}else
			setGravity( (gravityTransition.startValue * gravityTransition.rTime + gravityTransition.endValue * gravityTransition.vTime)
						/(gravityTransition.vTime + gravityTransition.rTime));
	}
	
	// change speed
	if( speedTransition.defined ){
		speedTransition.rTime -= timeStep;
		speedTransition.vTime += timeStep;
		if( speedTransition.rTime <= 0 ){
			//			jpPrefs.animationSpeed = speedTransition.endValue;
			setAnimationSpeed(speedTransition.endValue);
			speedTransition.defined = false;
		}else
			setAnimationSpeed((speedTransition.startValue * speedTransition.rTime + speedTransition.endValue * speedTransition.vTime)
							  /(speedTransition.vTime + speedTransition.rTime));
		/*		jpPrefs.animationSpeed = (speedTransition.startValue * speedTransition.rTime + speedTransition.endValue * speedTransition.vTime)
			/(speedTransition.vTime + speedTransition.rTime);
		*/
		
	}
	
}


void setJugglerObjects( lp *theLp)
{
	int c;
	Juggler *j;
	
	if( theLp->nr == -1 ) {       //alle
		for( c = 0; c < jugglerCount; c++ ) {
			j = getJuggler( c );
			if( theLp->objectKind == random_object )
				j->objectKind = selectRandomObject();
			else
				j->objectKind = theLp->objectKind;
		}
	} else {
		if( theLp->nr <= ballCount )       // nix illegales tun!
		{       
			j = getJuggler( theLp->nr );  // Um den gehts
			if( theLp->objectKind == random_object )
				j->objectKind = selectRandomObject();
			else
				j->objectKind = theLp->objectKind;
		}
	}
}				


void setJugglerColor( lp *theLp ) {
    int c;
    Juggler *j;
	
	
	if( theLp->nr == -1 ) {       //alle
		for( c = 0; c < jugglerCount; c++ ) {
			j = getJuggler( c );
			j->startColor = j->color;
			j->endColor = theLp->color;
			j->vColorTransitionTime = 0.;
			j->rColorTransitionTime = theLp->transitionTime;
			j->colorDefined = true;
		}
	} else {
		if( theLp->nr <= jugglerCount )       // nix illegales tun!
		{       
			j = getJuggler( theLp->nr );  // Um den gehts
			j->startColor = j->color;
			j->endColor = theLp->color;
			j->vColorTransitionTime = 0.;
			j->rColorTransitionTime = theLp->transitionTime;
			j->colorDefined = true;
		}
	}
}


void setObjectColor( lp *theLp ) {
    int c;
    _object *o;
	

	if( theLp->nr == -1 ) {       //alle
		for( c = 0; c < ballCount; c++ ) {
			o = getObject( c );
			o->startColor = o->color;
			o->endColor = theLp->color;
			o->vColorTransitionTime = 0.;
			o->rColorTransitionTime = theLp->transitionTime;
			o->colorDefined = true;
		}
	} else {
		if( theLp->nr <= ballCount )       // nix illegales tun!
		{       
			o = getObject( theLp->nr );  // Um den gehts
			o->startColor = o->color;
			o->endColor = theLp->color;
			o->vColorTransitionTime = 0.;
			o->rColorTransitionTime = theLp->transitionTime;
			o->colorDefined = true;
		}
	}
}

void setObjectSize( lp *theLp)
{
	int c;
	_object *o;

	if( theLp->nr == -1 ) {       //alle
		for( c = 0; c < ballCount; c++ ) {
			o = getObject( c );
			o->startSize = o->size;
			o->endSize = theLp->size;
			o->vSizeTransitionTime = 0.;
			o->rSizeTransitionTime = theLp->transitionTime;
			o->sizeDefined = true;
		}
	} else {
		if( theLp->nr <= ballCount ) {      // nix illegales tun!
			o = getObject( theLp->nr );  // Um den gehts
			o->startSize = o->size;
			o->endSize = theLp->size;
			o->vSizeTransitionTime = 0.;
			o->rSizeTransitionTime = theLp->transitionTime;
			o->sizeDefined = true;
		}
	}
}

void setObjectKind( lp *theLp)
{
	int c;
	_object *o;

	if( theLp->nr == -1 ) {       //alle
		for( c = 0; c < ballCount; c++ ) {
			o = getObject( c );
			if( theLp->objectKind == random_object )
				o->kind = selectRandomObject();
			else
				o->kind = theLp->objectKind;
		}
	} else {
		if( theLp->nr <= ballCount )       // nix illegales tun!
		{       
			o = getObject( theLp->nr );  // Um den gehts
			if( theLp->objectKind == random_object )
				o->kind = selectRandomObject();
			else
				o->kind = theLp->objectKind;
		}
	}
}				


void oneStepOfAnimation( int dummy );

void prepareNextLocalCommand( lp *firstLp ) {
    lp *theLp;
	
	
    theLp = firstLp;
	
    while( theLp ) 
	{
		switch( theLp->type){
			case ObjectSizeType:
				setObjectSize( theLp );
				break;
			case ObjectKindType:
				setObjectKind( theLp );
				break;
			case ObjectColorType:
				setObjectColor( theLp );
				break;
			case JugglerColorType:
				setJugglerColor( theLp );
				break;
			case JugglerObjectType:
				setJugglerObjects( theLp );
				break;
			case GravityType:
				gravityTransition.startValue = jpPrefs.gravity;
				gravityTransition.endValue = theLp->value;
				gravityTransition.rTime = theLp->transitionTime;
				gravityTransition.vTime = 0.;
				gravityTransition.defined = true;
				break;
			case TpsType:
				speedTransition.startValue = jpPrefs.animationSpeed;
				speedTransition.endValue = theLp->value;
				speedTransition.rTime = theLp->transitionTime;
				speedTransition.vTime = 0.;
				speedTransition.defined = true;
				break;
			case DropAllType:
				reInitAllObjects();
//				oneStepOfAnimation( 0 );
				break;
			default:
				break;
		}
		theLp = theLp->nextLp;
    }
}


void prepareNextStyle( lsp *firstLsp ) {
    int c;
    Juggler *j;
    lsp *theLsp;


    theLsp = firstLsp;

    while( theLsp ) {
        if( theLsp->who == -1 ) {       //alle
            for( c = 0; c < jugglerCount; c++ ) {
                j = getJuggler( c );
                setStyle( j, theLsp->p );
            }
        } else {
            if( theLsp->who <= jugglerCount )       // nix illegales tun!
            {       j = getJuggler( theLsp->who );  // Um den gehts
                setStyle( j, theLsp->p );
            }
        }
        theLsp = theLsp->nextLsp;
    }
}


// Jongleur j auf muster a setzen. Nur, wenn nicht teil seines Musters
void setStyle( Juggler *j, styleEl *a ) {
    styleEl *p;

    if( (p = a) == NULL )   // Das neue Muster
        return;                 // Kindersicherung
    do {
        if( p == j->sE )        // Er hats schon
            return;                 // dann nix tun
        p = p->next;            // das nchste Muster
    } while( a != p );               // bis wir rum sind

    // also hat ers nicht
    j->sE = a;      // Aber jetzt

}

void prepareNextJugglerMovement( lmp *firstLmp ) {
    Juggler *j;
    float p, b;
    lmp *theLmp;
    float vz;


    theLmp = firstLmp;

    while( theLmp ) {
        j = getJuggler( theLmp->who );

        // Keine Bewegung <=> schon da
        if( theLmp->duration == 0 ) {
            j->position = j->pStart = j->pEnd = theLmp->pEnd;
            j->viewdirection = theLmp->nEnd;
        } else {

            // Bewegung in Abhngigkeit von der Zeit
            j->pStart = j->position;
            j->modPath = theLmp->modPath;
            j->pEnd = theLmp->pEnd;

            // Richtungsvektor drehen
            p = theLmp->nEnd *  j->viewdirection ;
            b = j->viewdirection.betrag()* theLmp->nEnd.betrag(  );
            p = min( 1,p / b);
            p = max(-1,p);

            // Damit linksrum und rechtsrum funktioniert:
            // gerichteter Winkel
            vz = theLmp->nEnd * j->offsetHand;

            // Winkel ausrechnen
            // Um soviel haben wir uns am Ende gedreht
            p = radToGrad( acos( p)  ) / theLmp->duration;

            // Winkel ggf. umdrehen
            j->winkelN = ( vz > 0.1 ? p : - p);

            // Pirouetten
            {
                float n;
                n = theLmp->p*360;                                      // So oft um 360 Grad drehen. float erlaubt!
                n /= theLmp->duration;                                  // Dann ist es vorbei
                j->winkelN += n;
            }
            //wann kommt er an
            j->vTime = 0.;
            j->rTime = theLmp->duration;
        }
        theLmp = theLmp->nextLmp;
    }
}

void    prepareNextCameraMovement( _cameraMovementPointer *theCameraPointer) {
    _cameraMovementPointer *p;

    p = theCameraPointer;
	
    while( p ) {
        switch( p->mode ) {
        case cameraSetPos:
            setCameraPos( &p->p1 );
            break;
        case cameraSetPoi:
            setCameraPoi( &p->p1 );
            break;
        case cameraSetUp:
            setCameraUp( &p->p1 );
            break;
        case cameraMovePosAbsolute:
            setCameraPosMovement( &p->p1, &p->p2, p->duration );
            break;
        case cameraMovePoiAbsolute:
            setCameraPoiMovement( &p->p1, &p->p2, p->duration );
            break;
        case cameraMoveAbsolute:
            setCameraMovement( &p->p1, &p->p2, p->duration );
            break;
        case cameraTilt:
            setCameraTilt( p->angle, p->duration);
            break;
        case cameraPan:
            setCameraPan( p->angle, p->duration);
            break;
        default:
            break;
        }
        p = p->next;
    }
}

// --------------- Auskunftfunktionen -------------------------------

// Zeiger auf Anfang des nchsten Musters holen, dabei Schleifen bercksichtigen.
// loopPointer zeigt auf die aktuelle Schleife
// Wiederlich, weils rekursion _nachbildet_
sse *getNextLoop( void ) {

    for( ;; ) {
        if( loopPointer->count == FOREVER )
            ;       // ewig wiederholen
        else if ( loopPointer->count < 0 )
            beep( );
        else {
            while( loopPointer->rest == 0 ) // Schleife abgearbeitet
            {       if( loopPointer->nextLoop != NULL ) {
                    loopPointer = loopPointer->nextLoop;    // Folgeschleife
                    prepareNextLoop( loopPointer );
                } else {
                    loopPointer = loopPointer->outTheLoop;  // Eine Ebene nach oben
                    loopPointer->rest--;                                    // einer weniger
                }
            }

        }

#ifdef __INFO__
        setInfoString( loopPointer->info );
#endif

#if 01
		if( loopPointer->music[0] != NIS)
			startMusic( loopPointer->music);		
#endif
		
        // pass correction Modifier
        if( loopPointer->passCorrectionMode != dontChangeMode )
            passCorrectionMode = loopPointer->passCorrectionMode;
        // _myPrintf("pcm = %d\n", passCorrectionMode);

        if( loopPointer->inTheLoop.typ == loop ) {       // Es gibt noch UnterSchleifen
            loopPointer = loopPointer->inTheLoop.ssP.loop;  // rein
            prepareNextLoop( loopPointer );
        } else {       // Einzelpattern wird unmittelbar abgearbeitet
            loopPointer->rest--;
            break;
        }
    }

    return loopPointer->inTheLoop.ssP.event;
}


void setVisibleAllObjects( void ) {
    int c;
    invisibleObjectsFlag = false;

    for( c = 0; c < ballCount; c++ )
        getObject( c )->invisible = false;

    if( hasIvisibilityTag == true )
        reInitAllObjects();

}
void reInitAllObjects( void ) {
    int c;

    for( c = 0; c < ballCount; c++ ) {
        reInitOneObject( getObject( c ));
    }
	getOldestObjectCounter = 0;
}

// Einen Object fallenlassen
void reInitOneObject( _object *o ) {
//    extern Vector3D aX;

    o->catcher.juggler = NO_JUGGLER;
    o->s = o->d= pN;
    o->e = NULL;
	o->dropped = true;
    o->rTime = NO_TIME;
    o->a = pN;
    o->spins = 0;
    o->r = o->n = aX;
    o->thrown = o->catched = false;
	o->orbitNr = NO_ORBIT;
//	b->halfSpinFlag = false;
//	b->doesHalfSpinThrow = false;
	o->cIn.x = o->cOut.x = NOWHERE;
	o->tIn.x = o->tOut.x = NOWHERE;
	o->beepThisTime = false;
}



//      Finde einen Object, der noch nicht gehalten wird
//      catcher.juggler ist dann NO_JUGGLER
_object *getUnsetObject( void ) {
    _object *o, *oR;
    int c;
	
    oR = NULL;
    c = ballCount;
	
    for( c = 0;     c < ballCount; c++ ) {
        o = getObject( c );
        if( o->catcher.juggler == NO_JUGGLER) {
            oR = o;
            break;
        }
    }
//	myPrintf("getUnsetObject()  = %2d @ %2.2f\n",getObjectIndex(b), aTime);
	
    return oR;
}

// ermittelt den Object,
// der am lngsten in der angegebenen Hand ist.
_object *getOldestObject( Juggler *j, int h ) {
    int c;
    _object *o, *of;
	
    // Object existiert nicht!
    of = NULL;
    // Ist schon ein Object in der Hand?
    for(c = 0; c < ballCount; c++) {
        o = getObject( c );
        // richtiger Jongleur und Hand
        if( ( o->catcher.juggler == j ) &&
                ( o->catcher.h == h ) ) {       // erster Object
            if( of == NULL )
// xxx warum gibt es hier eine zuweisung, wenn kein Objekt existiert? Fehler bei exitSync
                of = o;
            // Es gibt schon einen älteren Object. (gleichalt: niedrigeren Index
            else if((of->rTime > o->rTime) )
                of = o;
        }
    }

	o = getUnsetObject();
	
	// This is the one we wank
    if( of  && of->rTime-timeStep <= 0) {
        of->dropped = false;
    }
	// if no ball is in the hand, take an unset object
	else if( o )
			of = o;
	// otherwise, take an airbourne object
	else if( of )
		of->dropped = false;
	
//	myPrintf("%4d (%2.2f): getOldestObject() = %2d rTime = %2.2f @ %2.2f\n",getOldestObjectCounter++, actEvent->ss, getObjectIndex(bf)+1, bf->rTime, aTime);

    return of;
}




// Speicher fr verzgerte Events freigeben
void getDelayMemory() {
    disposeDelayedEvents();

    delayCount = eventCount * 2 * 10 ;// extra delay memory added. Maybe this helps with delays >= 1
    // holen
    delayedEvents = ( _dSse*)malloc( sizeof( _dSse) * delayCount);
    if( delayedEvents == NULL ) {
        canDelay = false;
        return;
    }
    resetDelayMemory();
    canDelay = true;
}

void disposeDelayedEvents( void ) {
    if( delayedEvents != NULL ) {
        free( delayedEvents );
        delayedEvents = NULL;
    }
}

_dSse *getDelayedEvents( int c ) {
    return delayedEvents + c;
}

// Es gibt bisher keine Verzgerung
void resetDelayMemory( void ) {
    int c;
    _dSse *d;
    if( !canDelay )
        return;

    for( c = 0; c < delayCount; c++ ) {
        d = getDelayedEvents(c);
        d->event = NULL;
    }
}

// Event in die Liste der verzgerten Events aufnehmen
// Leeren Platz suchen und eintragen.
// War das Event schon in der Liste, wird es berschrieben.
// keine Verzgerung, die ber Musterlnge hinausreicht
void addDelayedEvent( sse *theEvent, int theHand) {
    int c;
    _dSse *d;
    Juggler *j;


    // Kann ich ja garnicht
    if( !canDelay )
        return;

    // Freien Platz suchen
    // jo, wenn Verzgerung grer als 1 ist.
    // sollte also eine dynamische Liste werden...
    for( c = 0; c < delayCount; c++ ) {
        d = getDelayedEvents( c );
        if( d->event == NULL || d->event == theEvent)
            break;
    }

    // Jongleur holen
    j = getJuggler(theEvent->who);

    if(     j->delay == UNDEFINED ) {
        beep( );
        return;
    }

    //      d->th    = th;                          // Nummer des Handereignisses
    d->eTime = eventTime;           // Wann wurde ich ausgelst?
    d->delay = d->restDelay =       // Wie lange verzgern das event?
                   j->delay;
    d->event = theEvent;            // Das auslsende Event
    d->hand  = theHand;                     // Die benutzte Hand
    d->runde = runde;
    d->muxCount = muxCount;

    //_myPrintf("delay added: %f\n", d->delay);
}

// Liste der verzgerten Events durchsuchen.
// ggf. auslsen
//
void doDelayedEvents( void ) {

    _dSse           *d;
    float           eTime;
    Juggler *j;
    int             h;
    float           ed;
    int             md;

    if( !canDelay )
        return ;

    for(int c = 0; c < delayCount; c++ )             // search for delayed events
    {       d = getDelayedEvents( c );
        if( d->event )                            // found one!
        {       d->restDelay -= timeStep;         // countdown
            if( d->restDelay <= 0)                // trigger now
            {
                j = getJuggler( d->event->who );  // Who?
                h = j->hand;
                j->hand = d->hand;

                eTime = eventTime;                // remember actual time
                eventTime = d->eTime+d->delay;    // set time
                d->event->inDelay = true;         // dont delay another time
                ed = d->event->delay;
                d->event->delay = d->delay;       // absolute time

                md = muxCount;
                muxCount = d->muxCount;

                doSingleThrow( d->event);         // trigger event

                muxCount = md;
                d->event->inDelay = false;        // event is not longer in dealy-querry
                d->event->delay = ed;

                j->hand = h;                      // normal hand
                eventTime = eTime;                // restore time
                d->event = NULL;                  //remove from list
            }
        }
    }
}

// Ein Objekt fliegt durch den Raum:
// Anfangs- Endvektoren, gesamte & verstrichene Flugdauer, Schwerkraft bercksichtigen
punkt throwObject( _object *o)
//punkt p1, punkt p2, float tG, float tV ) 
{
    punkt pA;
    float f;

    // horizontale Bewegung
    pA = o->d - o->s;                                       // Flugvektor
    pA = pA * ( o->vTime / (o->vTime+ o->rTime));                         // verstrichene Zeit

    // vertikale Bewegung.
    //  Die Erde saugt: s = -1/2 * g * t^2
    //      pA.y += tV * tG * jpPrefs.gravity/2 - tV * tV * jpPrefs.gravity/2;
    // vereinfacht:

    f = hurriedMode? 1.5: 1.f;
    pA.y +=  jpPrefs.gravity/2 * o->rTime * o->vTime * f;
	if( tableJugglingMode){
		pA.y = 0;// = max( pA.y, 0 )+ 20;
		o->spins = 0;
		if( o->e ){
			if( o->e->ss < 3)
				pA.y +=  jpPrefs.gravity/2 * o->rTime * o->vTime * f;
		}
		
	}
    pA = o->s + pA;                            // Startposition addieren

    return pA;
}


// Ein Objekt fliegt durch den Raum:
// Anfangs- Endvektoren, gesamte & verstrichene Flugdauer, Schwerkraft bercksichtigen
punkt rollObject( _object *o)
//punkt p1, punkt p2, float tG, float tV ) 
//&b->s, &b->d,  tGes, b->vTime
{
    punkt pA;
    float f;
	
    // horizontale Bewegung
    pA = o->d - o->s;                                       // Flugvektor
    pA = pA * ( o->vTime / (o->vTime+ o->rTime));                         // verstrichene Zeit
	
    // vertikale Bewegung.
    //  Die Erde saugt: s = -1/2 * g * t^2
    //      pA.y += tV * tG * jpPrefs.gravity/2 - tV * tV * jpPrefs.gravity/2;
    // vereinfacht:
	
    f = hurriedMode? 1.5: 1.f;
	
    pA = o->s + pA;                            // Startposition addieren
    pA.y = jpPrefs.objectFaktor * o->size*10;
	
    return pA;
}




void freeEventList( void ) {
    eventListEntry *a = firstEventPointer, *n;

    printEventList();

    while( a ) {
        n = a->next;
        free( a );
        a = n;
    }

    firstEventPointer = NULL;
    eventListError =  false;
    topLevelEventNumber = 0;
}

void addToEventList( sse *e) {
    eventListEntry *t, *p;
    Juggler *j;

    assert( e != 0 );

    if( eventListError )
        return;

    if( runde >= 2  ) {
        t = firstEventPointer;
        return;
    }

    t = (eventListEntry * )malloc( sizeof( eventListEntry));

    if( t ) {
        t->next = NULL;
        t->p = NULL;
        t->nr = -1;

    } else {
        eventListError = true;
        return;
    }


    if( firstEventPointer == NULL )
        firstEventPointer = t;

    else {
        p = firstEventPointer;
        while( p->next )
            p = p->next;
        p->next = t;
    }

    t->p            = e;
    j               = getJuggler( e->who );
    t->hnd          = j->hand ;
    t->nr           = topLevelEventNumber;
}

void printEventList( void ) {
    //      eventListEntry *a = firstEventPointer;

#if 0
    getMacModifiers();
    if(!ckMacMode )
        return;

    _myPrintf("-------------\n");

    while( a ) {
        Juggler *j;
        int hand;

        j = getJuggler( a->p->who);
        hand = j->hand;
        _myPrintf("a = %lx ", a );
        _myPrintf("a->nr = %d, a->hand = %d, tlNr = %d ", a->nr, a->hnd , topLevelEventNumber);
        _myPrintf("a->p->who = %d, a->p->hnd = %d\n", a->p->who);
        c++;

        a = a->next;
    }

#endif
}


// get a pointer to the actual topLevelEvent for juggler i
eventListEntry *getTopLevelEventForJuggler( int i, int nr ) {
    eventListEntry *t = firstEventPointer, *h = firstEventPointer;

    nr = nr  % momentCount; //war (momentCount * 2)

    // get tleBlock
    while(  t && t->nr != nr)
        t = t->next;
    //      printf("%lx\n", t);
    // pointer to juggler i;
    while(t && t->p->who != i )
        t = t->next;

    // if there is a simultanous event that is pointing to t, take that one!
    if( t ) {
        while( h ) {
            if( h->p->sim == t->p ) {
                t = h;
                break;
            }
            h = h->next;
        }
    }

    return t;

}


// lt die Blle fliegen
void moveAllObjects(  ) {
    int ballIndex ;
    punkt *p;
	_object *o;
	
    for( ballIndex = 0; ballIndex < ballCount; ballIndex++) {
		if(ballIndex == 5)
			printf("");
		o = getObject( ballIndex);
		if(o->beepThisTime)
			printf("");
        p = getObjectPosition( ballIndex); // Böser Seiteneffekt zum setzen der Objectposition!! TR
        *p = moveOneObject( o );
    }
}


// Diese Funktion berechnet, wo der Object ist
// Gibt Koordinaten des augenblicklichen Punktes zurck
// trgt neue Flugzeiten ein
// und kmmert sich darum, dass gehaltene Objekte in sinnvolle Richtungen weisen
punkt moveOneObject( _object *o ) {
    punkt p;
    int c;
    bool theCatch;
    float t;
	int tmp;
	
	
	if(beepOn && o->beepThisTime ){
		o->beepThisTime = false;
	}
	
    if( o->catcher.juggler == NO_JUGGLER) {
        return o->s;
    }

    // if delays do not work preoperly
    if( !canDelay && o->delay > 0 )
        o->delay -= timeStep;
    else {
        t = o->rTime - timeStep;
		
        // CATCH!!!			
        if((o->rTime >= 0 && t <= 0) && o->catcher.juggler != NO_JUGGLER) {
			
			if( o->doesHalfSpinThrow){
				toggle( o->halfSpinFlag);
				o->doesHalfSpinThrow = false;
			}
				
			// object in hand
			if( o->inHand == false ){
				throwEntry *te;
				o->inHand = true;
				o->hasSelfColor = true;
				o->spins = 0;      // no spin
				
				if( (te = getThrowAfter(o->catcher, aTime)) == NULL)
					return o->d;
				else{
					if( te->to.juggler == te->who.juggler )
						o->hasSelfColor = true;
					else {
						o->hasSelfColor = false;
					}
				}
				o->catchingJugglerIndex = te->to.juggler->getJugglerNr();
				o->nextSiteswapValue = te->ss;
			}
			
			
			
			
			
			
//			b->invisible = invisibleObjectsFlag;
//			if(preflight)//movementPreflight || handPreflight)
            {       // save catching position
                // depends on actual position of juggler
                setCatchPosition( o );
            }

            {
                Juggler *j2 = o->catcher.juggler;
				// set the object according to juggler
				tmp = j2->objectKind;
				if( tmp >= 0 ){
					if( tmp == random_object )
						tmp = selectRandomObject();			
					o->kind = tmp;
				}
/*                if( j2->objects != UNDEFINED )
                    b->kind = j2->objects;
*/
            }

            setNextHandAction( o->catcher, o->catchTime, Catch );
            o->catched = true;

            // Ist schon ein Object in der Hand?
            theCatch = false;

            if( o->catcher.juggler != NO_JUGGLER) // Fehler -> kein sound!
            {       for(c = 0; c < ballCount; c++) {
                    _object *bt;

                    bt = getObject( c );
                    // Nicht dasselbe Object beachten
                    if( bt != o ) {       // Schon ein Object in der Hand?
                        if( bt->catcher.juggler == o->catcher.juggler &&
                                bt->catcher.h == o->catcher.h &&
                                bt->rTime <= 0) {
                            theCatch = true;
                            break;
                        }
                    }
                }
            }
            // Hand schon voll -> anderen Sound spielen
            //~ if(jpPrefs.s && (1 || o->h2 == false) && !muteSound) {

                //~ if( theCatch == true) {
                    //~ doSound( multiCatchID, o->catcher, o->airtime);//beep();//
                //~ } else {
                    //~ doSound( catchID, o->catcher,  o->airtime);//beep();//

                //~ }
            //~ }
        }
    }

    {
		float tGes;

        tGes = o->vTime+o->rTime;

#if 0
        // 1st try: just 1 bounce
        switch( o->bounce ) {
            punkt pTemp;
        case stdBounce:
#ifdef NEW_BOUNCE_STYLE
            p = o->a = newBounceObject( o);
#else

            p = o->a = bounceObject( &o->s, &o->d, tGes , o->vTime); // new position
            if( o->vTime < tGes/2 && o->vTime+timeStep >= tGes/2 ) {
                o->spins *= -1;
                if(throwHasPassCorrection != noCorrection )
                    o->spins += .5;
            }
#endif
            break;
        case forceBounce:
            p = forcebounceObject( &o->s, &o->d,  tGes, o->vTime); // new position
            pTemp = forcebounceObject( &o->s, &o->d,  tGes, o->vTime+timeStep); // new position

            if( p.y < 0 && pTemp.y > 0 ) {
                o->spins *= -1;
            }
            if( p.y < 0)
                p.y *= -1.0;


            o->a = p;
            break;
        case noBounce:
        default:
			if( o->roll )
			{
				p = o->a = rollObject(o); // new position
			}else
				p = o->a = moveObject(o);// new position
            break;
        }
#else

        if( o->bounce )
#ifdef NEW_BOUNCE_STYLE
            p = o->a = newBounceObject( o);
#else
           p = o->a = bounceObject( o->s, o->d, tGes , o->vTime); // new position
#endif
            
		else
		{
//			p = b->a = throwObject( b);
			if( o->roll )
			{	p = o->a = rollObject(o); // new position
				o->spins = 0;
			}
			else
				p = o->a = throwObject(o);// new position
		}
#endif
    }

    // object is in hand
    //      if( b->rTime <= 0 || b->hold )
//    if( b->rTime <= 0 ) {
	if( o->inHand ){
        Vector3D d;
        float t1;
        float gTime, vTime;
        float  tf1, tf2;
        Vector3D n1, n2, n3, n;
        _handPos *pos = 0, *pos2 = 0;
		
        if( o->catcher.juggler == NO_JUGGLER ) {
            return o->d;
        }

		
		o->catcher.juggler->moveOneJuggler();
        p = o->a = o->catcher.getHandPosition();

        // at the end, the handle shall have this direction
        d = o->catcher.getArmDirection(); // points to hand
        d.y = o->catcher.getLowerArmDirection().y;



        pos = getHandAt( o->catcher, o->catchTime);
        t1 = pos->absT;

        // get next movement
        pos2 = getHandAfter( pos);

	
        // difference in time
        gTime = pos2->absT  - t1 ;
        // time before next action
        while( gTime < 0 )
            gTime += patternTime;
        while( gTime > patternTime )
            gTime -= patternTime;

        vTime = -o->rTime;

        tf2 = vTime/gTime;              // passed time
        tf1 = (gTime - vTime)/gTime;    // remaining time


 

        // calculate object direction
        n1 = pos->dcDir.normalised();									// direction of handle at catch
        n2 = (aY * -1.f).normalised();									// current direction hand - shoulder
        n3 = pos2->dtDir.normalised();									// next throw direction
		
     // movement of object in hand
		// needed for passes
        if( 01) {
            float t;

            t = tf2*.8;
            n1 = n1 * (2.f*t*t*t - 3.f*t*t +1.f);  // catch direction
            n2 = n2 * (  3.f * tf1 * tf2 );         // move along arm direction
            n3 = n3 * ( -1.f*t*t*t+4.f*t*t);          // next throw direction
            n = n1 + n2 + n3;
			
        } else {
            n = n3;
        }




        if( 01 ){
            float factor;
            // folgt ein self oder en Pass? Rotation umkehren.
            // maw: was macht dieser Jongleur als nchstes
            // nach diesem Wrf->

			factor = vTime *(gTime-vTime)/gTime;
			factor = min( 1., factor) * jpPrefs.selfDwellElevation;
			n = n.rotateAroundVektor(-factor ,pos2->rtDir); 
			o->nDest =o->n = n.normalised();
			

        }
		
		// if a hand movement is specified, the object´s orientation follows the lower arm orientation
		if( 01 ){
			if( o->cOut.x != NOWHERE|| o->cIn.x != NOWHERE){
				o->n = o->nDest = o->catcher.getLowerArmDirection();
			}
		}
		
		
#ifdef FIX_MUX
        // for every object that is already in the hand:
        // rotate the current object by N degrees.
        {
            _object *bt;
            int bc;

//#define N 5

            for( bc = 0; bc < ballCount; bc ++) {
                bt = getObject( bc );
                if( bt->e == NULL)
                    continue;

                // multiplex throw
                if( bt->thrower.j == o->thrower.j && bt->thrower.h == o->thrower.h ) {

                    // multiplex throws will mess up rotation.
                    // so it will be set to zero the hard way
                    o->spins = 0;

                }
            }

        }
#endif

    }

    o->rTime -= timeStep;
    o->vTime += timeStep;
    o->nextThrowTime -= timeStep;

    if( o->h2 ) {
        const Juggler j3 = *(o->catcher.juggler);
        o->a = o->catcher.h == leftHand? j3.lh:j3.rh;
    }
    return p; // current position
}

// ------------------------------------------------------
// timing is done by doSiteswap...doSingleThrow.
// her we add values for throw/catch - location and spin.
// ------------------------------------------------------

void addStyleToThrow( _object *o, sse *theEvent) {

    dPunkt throwPosition, catchPosition;
    punkt t;
    int spinIndex;
    _ort  jdp;
    Juggler *js, *jd;
    int throwHasPassCorrection;
	bool isMuxPass;
    int tHnd, cHnd;
    Vector3D v;
    float alpha;
    Vector3D nThrow;
    float sideFaktor;
    float rotFaktor;
    float rotation, declination;
    float frictionalRotation;
	bool hsp = false;


    // ---------------------------------------------
    //                      collect all my pointers
    // ---------------------------------------------
    // get jugglers
    js = getJuggler( theEvent->who );
    jd = getJuggler( theEvent->to);
	
	
    actualObject = o;

	// position stuff
	jdp = *getCatchPosition(o->t, *jd);
    
    if( Juggler::walkMode ) {
        float t;

        t =  o->rTime + aTime;
        while( t >= patternTime )
            t -= patternTime;

        jdp.p = getPosOfJugglerAtOrAfter( theEvent->to, t);
        /*      _myPrintf("%2.2f %2.2f %2.2f, %d, %2.2f\n",
                                        jdp.p.x, jdp.p.y,jdp.p.z,
                                        theEvent->to,  b->rTime + aTime);
        */
    } else
        ;

    tHnd = o->thrower.h;
    cHnd = o->catcher.h;

    // ---------------------------------------------
    //                      Stylebeschreibung holen
    // ---------------------------------------------
    throwPosition = js->sE->v;
    catchPosition = js->sE->n;


    // ---------------------------------------------
    //                      hand movement
    // ---------------------------------------------

    if( theEvent->rel.t)
        throwPosition = theEvent->throwPosition;
    if( theEvent->rel.c )
        catchPosition = theEvent->catchPosition;

    o->isSelf = ( theEvent->who == theEvent->to );
	
    throwHasPassCorrection = checkForPassCorrection(theEvent, js, &jdp);	
	
	o->hasSelfColor = (js == jd);

	
    // ---------------------------------------------
    //                      left / right movement
    // ---------------------------------------------
    // left / right movement
    // depends on height of throw
    if( throwHasPassCorrection == noCorrection ) {
#define DefelctionFactor (min (theEvent->ss * 0.2f, 2) )
		
		if( throwPosition.deflectionMode == HandDeflectionOverwrite )
			throwPosition.p.SIDE -= throwPosition.deflectionValue*DefelctionFactor;
		else // HandDeflectionMultiply is the default
			throwPosition.p.SIDE -= throwPosition.deflectionValue * jpPrefs.deflection*DefelctionFactor;
		
		
		if( catchPosition.deflectionMode == HandDeflectionOverwrite )
			catchPosition.p.SIDE += catchPosition.deflectionValue*DefelctionFactor;
		else // HandDeflectionMultiply is the default
			catchPosition.p.SIDE += catchPosition.deflectionValue * jpPrefs.deflection*DefelctionFactor;
		
        if( o->isSelf ) {
            if( (int)floor(theEvent->ss)%2 == 0 ) {
                throwPosition.p.SIDE += EVEN_OFFSET;
                catchPosition.p.SIDE += EVEN_OFFSET;
            }
        }
    }
    // ---------------------------------------------
    // more left / right when a pass is a cross
    // ---------------------------------------------
    else {       
		//not more inside! throwPosition.p.SIDE -= 2 * (throwPosition.a?1.:jpPrefs.deflection) * throwPosition.d;
		//		catchPosition.p.SIDE += 2*(catchPosition.a? catchPosition.d:jpPrefs.deflection);
		
		// Pass throws go a bit more to the outside
#define PassCatchOutsideFactor 1.5f
#define PassThrowInsideFactor 1.2f
		if( throwPosition.deflectionMode == HandDeflectionOverwrite )
			throwPosition.p.SIDE -= PassThrowInsideFactor * throwPosition.deflectionValue;
		else // HandDeflectionMultiply is the default
			throwPosition.p.SIDE -= PassThrowInsideFactor * throwPosition.deflectionValue * jpPrefs.deflection;
		
		if( catchPosition.deflectionMode == HandDeflectionOverwrite )
			catchPosition.p.SIDE += PassCatchOutsideFactor * catchPosition.deflectionValue;
		else // HandDeflectionMultiply is the default
			catchPosition.p.SIDE += PassCatchOutsideFactor * catchPosition.deflectionValue * jpPrefs.deflection;

    }

    // ----------------------------------------------------
    //      catch / throw positions change with passes
    //      default, but sometimes unhandy if you want to
    //      have full control over the animation
    // ----------------------------------------------------
    if( passCorrectionMode == correctPassValues ) {
        // hands are a bit away from shoulders
        throwPosition.p.SIDE *= HAND_FAKTOR;
        catchPosition.p.SIDE *= HAND_FAKTOR;

        alpha = js->viewdirection * jd->viewdirection;
        alpha = fabs( alpha);
        alpha = min( 1, alpha);
        alpha = radToGrad( acos( alpha ) )/2;


        if( theEvent->who !=  theEvent->to ) {
            v =  js->position - jd->position;
            if( (js->offsetHand * v) > 0) {
                alpha = -alpha;
            }
        }
    }

	if(tableJugglingMode ){
		throwPosition.p.SIDE = (int)(o->e->ss)%2? -1:1;
		if(0 && o->isSelf == false )
			throwPosition.p.SIDE *= -1.;
		if( o->e->ss < 4 )
			throwPosition.p.BACK = 1;
		else
			throwPosition.p.BACK = ((int)(o->e->ss-2))/2 + .5;
		
		// up hat keinen Einfluss auf tableJuggling
		if( (int)o->e->ss < 3  )
			throwPosition.p.UP = 20;
		else
			throwPosition.p.UP = 100;

		/* shift the ball to the outside depending on how many balls are in this position for the lifetime of this throw
		throwEntry *t;
		 
		int c, throwsInOrbit = 0, ballsInOrbit;
		int ssSum = 0;
		for( c = 0; c < throwEntryCount; c++ ){
			throwEntry *t;
			t = throwList+c;
			
			if( t->orbitNr == b->orbitNr ){
				throwsInOrbit++;
				ssSum += t->ss;
			}
		}
		ballsInOrbit = ssSum / throwsInOrbit;
		*/
		throwEntry *t, *tn;
		int ss = o->e->ss;
		int dt;
		int spaceNeeded = 1;
		t = getThrowAt( o->catcher, aTime);
		while(t && ss > 0 ){
			tn = t->next;
			if(tn == 0 )
				break;
			dt = tn->aTime-t->aTime;
			if( dt < 0 )
				dt += patternTime;
			ss -= dt;
			spaceNeeded++;
			t = tn;
		}	
				
		switch((int)o->e->ss){
			case 1:
			case 3:
				catchPosition.p.SIDE = -throwPosition.p.SIDE;
				break;
			case 2:
				catchPosition.p.SIDE = 1;
				break;
			case 4:
			case 6:
			case 8:
			case 10:
			case 12:
				catchPosition.p.SIDE =  throwPosition.p.SIDE * spaceNeeded;//(int)(b->e->ss-2);
				break;
			default:
				catchPosition.p.SIDE = -throwPosition.p.SIDE * spaceNeeded;//(int)(b->e->ss-3);
				break;
		}
		if(0 && !o->isSelf && (int)(o->e->ss)%2)
			catchPosition.p.SIDE *= -1;
		
		catchPosition.p.BACK = throwPosition.p.BACK;
		catchPosition.p.UP = 1;
	}
	
	// ---------------------------------------------
    //                      catch / throw positions
    // ---------------------------------------------
    // throw position,
    // relative to juggler
    // left / right
    sideFaktor = (tHnd==leftHand?1:-1);
    js->rels = js->offsetHand * throwPosition.p.SIDE * sideFaktor;
    // back / front
    t = js->viewdirection * ( throwPosition.p.BACK * JUGGLING_PLANE);
    js->rels = js->rels + t;
    // up / down
    t = Vector3D( 0, 1, 0 );
	if ( !tableJugglingMode ) 
		t = t *( (throwPosition.p.UP-THROW_HEIGHT) * (UAL+OAL) *0.6);
    js->rels = js->rels + t;
    js->rels = js->rels.drehungY(alpha);
    // absolute position
    o->s = js->position + js->rels;
	// catch
    // same as above
    jd->reld = jdp.o * ( catchPosition.p.SIDE * (cHnd==leftHand?1:-1) );
    t = jdp.n * ( catchPosition.p.BACK * JUGGLING_PLANE);
    jd->reld = jd->reld + t;
    t = Vector3D( 0, 1, 0 ) *( (catchPosition.p.UP-1) * (UAL+OAL) * .6);
    jd->reld = jd->reld + t;
    jd->reld = jd->reld.drehungY(-alpha);
    o->d = jdp.p + jd->reld;
	

	// get access on the relative position of another juggler, 
	// i.e. let juggler A throw to juggler b´s relative position
	// leads to the need to make it possible to throw to a given position in 3d space

	if(0 && tableJugglingMode && ! o->isSelf){
		o->s = o->d ;
		o->s.y += 20;
		o->s.x += 100;
		js->rels = o->s;
		
	}
	
    if( tHnd == leftHand ) {
        js->rls = js->rels;
        js->rld = jd->reld;
    } else {
        js->rrs = js->rels;
        js->rrd = jd->reld;
    }

    isMuxPass = false;
#ifdef FIX_MUX
    // for every object that is already in the hand:
    // rotate the current object by N degrees.
    {
        _object *bt;
        int bc;
        bool isMux = false,
             isPass = false;


        for( bc = 0; bc < ballCount; bc ++) {
            bt = getObject( bc );
            if( bt->thrower.j == o->thrower.j && bt->thrower.h == o->thrower.h ) {
                isMux = true;
                isPass |= (bt->thrower.j != bt->catcher.j);
            }


        }
        if( isMux && isPass )
            isMuxPass = true;

    }
#endif
    // ---------------------------------------------
    //                      passes to other jugglers
    //                      throw lower, catch higher
    // ---------------------------------------------
    if(!o->isSelf && throwHasPassCorrection == passCorrection)// && !isMuxPass)
    {
        Vector3D r, t;

        o->s.UP     += jpPrefs.passCorrectionVector.UP;
        js->rels.UP += jpPrefs.passCorrectionVector.UP;

        r = jdp.n * jpPrefs.passCorrectionVector.BACK;
        js->rels = js->rels + r ;
        o->s = o->s + r;
		
        t = js->offsetHand.normalised();
        r = t * ( sideFaktor *  jpPrefs.passCorrectionVector.SIDE);
        js->rels = js->rels + r ;
        o->s = o->s + r ;

        o->d.UP         += jpPrefs.catchCorrectionVector.UP;
        jd->reld.UP += jpPrefs.catchCorrectionVector.UP;

        r = jdp.n * ( jpPrefs.catchCorrectionVector.BACK);
        jd->reld = jd->reld + r ;
        o->d = o->d + r ;

        t = jdp.o.normalised();
        r = t * ( (cHnd==leftHand?1:-1) *  jpPrefs.catchCorrectionVector.SIDE);
        jd->reld = jd->reld + r ;
        o->d = o->d + r ;
    }
	
    // ok
    //-----------------------------------------------------------------

    // ---------------------------------------------
    //                      calculate spins
    // ---------------------------------------------
    rotFaktor = throwPosition.p.relRot;

    //      if( !b->hold )
    if( true ) {
        if(theEvent->spin != NOWHERE ){
			
			o->spins = theEvent->spin;
		}
        else {
            if( obeyDelay )
                spinIndex = max(0,floor(theEvent->ss - theEvent->resultingDelay));
            else
                spinIndex = max(0,floor(theEvent->ss) );

            //_myPrintf("spinIndex %d\n",spinIndex);
            spinIndex = min(SPIN_MAX - 1, spinIndex);
            o->spins = getSpin( spinIndex)->n;

        }
    } else
        o->spins = 0;

#ifdef DO_FLIPS  
	float spin = theEvent->spin;
	if( spin != NOWHERE){
		hsp = spin != (int)spin;
		if( hsp )//&& o->e->ss <= 1)
			o->spins = theEvent->spin;
	}

#endif
	
    // ok
    //-----------------------------------------------------------------

    if(o->isSelf == false && throwHasPassCorrection != noCorrection) {
        nThrow = o->d - o->s;
        nThrow = nThrow.normalised();
    } else
        nThrow = js->viewdirection;

    // ok
    //-----------------------------------------------------------------
    if( 1){
        float offset;
        o->spins = (o->spins * rotFaktor);
        o->spins += spinOffset;

        frictionalRotation = o->spins - (int)o->spins;
 		
		offset = rotFaktor -(int)rotFaktor;
        if( o->spins * rotFaktor < 0 )
            offset *= -1.;

        frictionalRotation += offset;
        o->spins = (int)o->spins;
		
    }


    // ok
    //-----------------------------------------------------------------


    // ---------------------------------------------
    //                       calculate axis
    // from
    // ---------------------------------------------
    rotation = -throwPosition.p.rotation;
	declination = throwPosition.p.declination;

    // correct backwards passes
    if( throwHasPassCorrection == passBackwardsCorrection)
         rotation+=180;
	if( throwHasPassCorrection == passCorrection)
		declination-=50; // calculate value from throw position
    

    while(rotation >  360)
        rotation-=360;
    while( rotation <  -360)
        rotation += 360;


    if( tHnd == rightHand )
        rotation *= -1;

	
    // club direction is defined by rotation and elevation angles from j->n
    o->r = nThrow.rotateAroundVektor(-90, aY);
    o->r = o->r.rotateAroundVektor(rotation, aY);
    o->n = nThrow.rotateAroundVektor(rotation, aY);
    o->n = o->n.rotateAroundVektor(declination, o->r);

	
	
	
    o->nDest = o->n = o->n.normalised();

    // use direction of throw for passes to all jugglers that are facing each other
    // thus NOT back to back throws
	if(throwHasPassCorrection == passCorrection ) {
		Vector3D d;

		d = js->position - jdp.p ;

		if(  (js->viewdirection * d ) > 0 ) {
			o->n = aY * -1;
			o->n = o->n + js->viewdirection;
			o->n = (o->n).normalised();
		}
	}

	// ok
    //-----------------------------------------------------------------

    // ---------------------------------------------
    //                       calculate axis
    // to
    // ---------------------------------------------
    if(catchPosition.p.rotation != NOWHERE) {
        rotation = -catchPosition.p.rotation;
        declination = catchPosition.p.declination;

        while(rotation >  360)
            rotation-=360;
        while( rotation <  -360)
            rotation += 360;

        if( cHnd == rightHand )
            rotation *= -1;

        o->nDest = nThrow.rotateAroundVektor(  rotation, aY);
        o->nDest = o->nDest.rotateAroundVektor(  declination, o->r);
    } else {
        if(throwHasPassCorrection == passCorrection) {
            o->nDest = aY * 1.f;
            o->spins *= -1;
		}
		else if(throwHasPassCorrection == slapCorrection) {
            o->spins *= -1;
			hsp = true;
        } else
            // default catch vektor same as throw vektor, otherwise
            o->nDest = o->n;
    }

    o->nDest = o->nDest.normalised();
    o->n = o->n.normalised();
    // ok
    //-----------------------------------------------------------------

    // ---------------------------------------------
    //    recalulate axis according to declination
    // ---------------------------------------------
    if( 01 )
	{   Vector3D r;
        float  alpha, f;


        // angle between source and dest. Vektor
        f =  o->n * o->nDest;

        if( f > .99999)
            f = 1;
        if( f < -.9999 )
            f = -1;

        alpha = radToGrad( acos(f ));
        // alpha = getAngle(b->n , b->nDest)
        o->spins -= alpha /360;

        // not colinear: recompute axis
        if( fabs(f) < 1) {
            r = o->n.vektorProdukt(o->nDest);
            o->r = r.normalised();

        }
    }

    // ok
    //-----------------------------------------------------------------
    o->nDest = o->nDest.rotateAroundVektor( frictionalRotation*360, o->r);
	o->spins += frictionalRotation;

    // ---------------------------------------------
    //                      fill in values
    // ---------------------------------------------
    o->th = th;
    o->throwTime = eventTime;
    o->catchTime = o->throwTime + o->airtime;


    if( throwHasPassCorrection != noCorrection ) {
        o->r = o->r * -1.f;
        o->spins *= -1.;
    }

    // ---------------------------------------------
    //                      normalize everything. 
    // ---------------------------------------------
    o->n = o->n.normalised();
    o->nDest = o->nDest.normalised();
	
//yyy pass 3@1.5=	wird reverse waagerecht gefangen, abwurfvektor hat aber eine halbe drehung extra (ist normal!)
	
	if( hsp )
	{
		/* the club does a half spin
		   with the next catch it´s orientation will be reversed
		*/ 
		o->doesHalfSpinThrow = true;
		// therefore, it´s destination orientation vector must be reversed, too
		o->nDest = o->nDest.rotateAroundVektor(180, o->r);
	}else {
		o->doesHalfSpinThrow = false;
	}
	
    // ---------------------------------------------
    // modify the path of the hands:
	// save throw and catch movement vectors for hands,
	// if specified	
    // ---------------------------------------------	
	Vector3D tx, ty, tz;

	if( throwPosition.p.outx == NOWHERE){
		o->tOut.x = NOWHERE;
	}else{
		tx = js->offsetHand.normalised() * throwPosition.p.outx * (tHnd == leftHand ? 1 : -1); 
		ty = Vector3D( 0,1,0) * throwPosition.p.outy;
		tz = js->viewdirection.normalised() * throwPosition.p.outz;
		o->tOut = tx + ty + tz;
	}
	
	if( throwPosition.p.inx == NOWHERE){
		o->tIn.x = NOWHERE;
	}else{
		tx = js->offsetHand.normalised() * throwPosition.p.inx * (tHnd == leftHand ? 1 : -1); 
		ty = Vector3D( 0,1,0) * throwPosition.p.iny;
		tz= js->viewdirection.normalised() * throwPosition.p.inz;
		o->tIn = tx + ty + tz;
	}

	if( catchPosition.p.inx == NOWHERE){
		o->cIn.x = NOWHERE;
	}else{
		tx = jd->offsetHand.normalised() * catchPosition.p.inx * (cHnd == leftHand ? 1 : -1); 
		ty = Vector3D( 0,1,0) * catchPosition.p.iny;
		tz = jd->viewdirection.normalised() * catchPosition.p.inz;
		o->cIn = tx + ty + tz;
	}
	
	if( catchPosition.p.outx == NOWHERE){
		o->cOut.x = NOWHERE;
	}else{
		tx = jd->offsetHand.normalised() * catchPosition.p.outx * (cHnd == leftHand ? 1 : -1); 
		ty = Vector3D( 0,1,0) * catchPosition.p.outy;
		tz = jd->viewdirection.normalised() * catchPosition.p.outz;
		o->cOut = tx + ty + tz;
	}


}


// checks if an object needs a special rotation axis, depending on it´s catch position
// NB: Wrong name, as this correction may also be neccessary in cases of strange selfs
int checkForPassCorrection( sse *e, Juggler *a, _ort *b) {
    int ret = noCorrection;
    float f;
    Vector3D v;

	if( tableJugglingMode )
		return ret;
	
	if( e->usePassCorrection == false ){
		ret = slapCorrection;
		return ret;
	}
	
    //yyy check fr multiplex einbauen

    f = a->viewdirection * b->n;

    if( f < .5 )
        ret = passCorrection;

    v = a->position - b->p;
    if( ( v * a->viewdirection ) > 0 && ( v * b->n )< 0 )
        ret =  passBackwardsCorrection;


    return ret;
}






// bouncen
#ifdef NEW_BOUNCE_STYLE


punkt newBounceObject( _object *o) {
    punkt p, orientation;
    float t;
    float h;
    float g = jpPrefs.gravity;


    if( o->e->gravity != NOWHERE )
        g = o->e->gravity;


    /*      printf("b->h = %.2f\n", b->n);

            if( h < 0 )
                return b->a;
    */



    // horizontal movement
    // this is identical for every kind of throw (toss or bounce)
    orientation = o->d - o->s;                    // vector from source to dest
    orientation = orientation * ( o->vTime / o->airtime);   // divided to get vector per timestep
    p = orientation + o->s;                       // actual position


    // vertical movement
    t = timeStep ;
    h = o->h;

    h += -g * .5 * t * t + o->v0 * t;

    // bounce this time
    if( h < 0 ) {
        float h0;
        float t0, tr;

        // time at which object hits the ground
        t0 = sqrt( 2 * o->h / g + o->v0*o->v0/(g*g)) + o->v0 / g;

        h0 =o->h + -g * .5 * t0 * t0 + o->v0 * t0;

        // speed at this moment
        o->v0 += -g *t0;

        // reversed and reduced speed
#if 01
        o->v0 *= -BOUNCE_REDUCTION;
#else
        o->v0 *= -1.f;
#endif
        // amount of rebounce at this interval
        tr = timeStep - t0;
        o->h = -g * .5 * tr * tr + o->v0 * tr;
        //          _myPrintf("t0 = %.2f, h0 = %.2f, v0 = %.2f\n", t0, h0, b->v0);

    } else {
        o->h = h;
        o->v0 += -g * t;
    }

    p.y = o->h;


    return p;


}
#else

// der 2. punkt wird am Boden gespiegelt
// d.h. p2.y wird negativ.
// Resultierenden Punkt normal berechnen
// Ergebnis unter Boden => Ergebnis spiegeln
punkt bounceObject( punkt p1, punkt p2, float tG, float tV ) {
    punkt pA;
    punkt pN;
    float t = tG/2.0;

    // find middle position
    pN = p2 + p1 ;
    pN.y = 0.;
    pN = .5f * pN;
    if( tV < t) {       // phase 1
        pA = throwObject( p1, pN, t, tV );
    } else {       // phase 2
        pA = throwObject( pN, p2, t ,tV-t);
    }


    return pA;
}


// force - bouncen
// der 2. punkt wird am Boden gespiegelt
// d.h. p2.y wird negativ.
// Resultierenden Punkt normal berechnen
// Ergebnis unter Boden => Ergebnis spiegeln
punkt forcebounceObject( punkt p1in, punkt p2, float tG, float tV ) {
    punkt pA;
    punkt p1 = punkt(p1in);
    p1.y *= -1.0;


    // horizontale Bewegung
    pA = p2 - p1;                                      // Flugvektor
    pA = pA * (tV / tG);                         // verstrichene Zeit

    // vertikale Bewegung.
    //  Die Erde saugt: s = -1/2 * g * t^2
    //      pA.y += tV * tG * jpPrefs.gravity/2 - tV * tV * jpPrefs.gravity/2;
    // vereinfacht:
    pA.y +=  jpPrefs.gravity/2 * ( tG - tV ) * tV;

    pA = p1 + pA ;                           // Startposition addieren

    /*      // Kann (noch) nicht bouncen
            if( pA.y < 0)
                    pA.y *= -1.0;

    */
    return pA;
}



#endif


void setThrowValue( _object *o) {
    // ---------------------------------------------
    //                  set bounce parameters
    // ---------------------------------------------

    // initial height
    o->h = o->s.y;

#if 1
    // throw velocity
    if(o->e->initialVelocity != NOWHERE ) {
        o->v0 = o->e->initialVelocity;
        return;
    }
#endif

    
    
 #if 01
    if(o->bounce  ) {

        //      myPrintf("init bounce %x\n", b);
        if(1 ||  o->e->bounceParameterDefined == false) {
            o->e->bounceParameterDefined = true;
            calcInitialVelocityForBounces( o);
        }
    } 
#else
    if(o->bounce  ) {
         calcInitialvelocityForBounces2( o);
         o->v0 = o->e->initialVelocity;
         return;
    }
#endif
    else {
        o->v0 =  jpPrefs.gravity * o->airtime / 2;

        // difference between hand height
        o->v0 -= (o->s.y - o->d.y) / o->airtime ;

    }

}

void markAllBouncesUndefined( void ) {
    int c;
    sse *e;
    //    _myPrintf("undefine Bounces, gravity = %.2f\n",jpPrefs.gravity);

    for( c = 0; c < eventCount; c++ ) {
        e =  getEvent( c);
        e->bounceParameterDefined = false;
    }

}

void calcInitialVelocityForBounces(_object *o) {
    float vMax;
    float vMin;
    float vAct;
    float last;

    float tmpStep = timeStep;
    punkt p;
    float bounceList[5];
    int bounceCount = 0;
    int d;

    //      jpPrefs.gravity = 98.1;

    vMax = jpPrefs.gravity * o->airtime * 2.1;
    vMin = -2 * vMax;
    //      vAct = vMax;
    last = vMax * 2;


    timeStep = o->airtime;

    // find possible bounce parameters
    for( vAct = vMin; vAct < vMax; vAct += .01) {
        float delta;

        o->h =  o->s.y;
        o->v0 = vAct;

#ifdef NEW_BOUNCE_STYLE
        p = newBounceObject( o );
#else
        p = o->a = bounceObject( o->s, o->d, o->airtime , o->vTime); // new position

#endif
#if 0
{
Vector3D v0, v1, vd;

        v0 = o->a = bounceObject( o->s, o->d, o->airtime , .01); // new position
        v1 = o->a = bounceObject( o->s, o->d, o->airtime , .02); // new position

        vd = v1 - v0;
        
        o->myV0 = vd * 100;
}
#endif
        

        // check if value is near enough
        delta = p.y - o->d.y;
        /*if( delta < .1 )
        _myPrintf("delta = %.2f\n", delta);
        */
        if( fabs(delta ) < .1 && fabs(last -vAct) > 1) {
            last = bounceList[bounceCount++] = vAct;
            //              break;
        }
    }

    o->h =  o->s.y;

    //      myPrintf("%d bounce parameter found\n", bounceCount);

    // no bounce found: toss
    if( bounceCount == 0)
        o->v0 = jpPrefs.gravity * o->airtime / 2;
    else {
        // get bounce with index, if existing
        if( o->e->bounceIndex == bounceHyperForce )
            d = 0;
        else {
            d = min(o->e->bounceIndex, bounceCount-1);
        }
        o->v0 = bounceList[d];
    }

#if 0
{
    int c;
    for( c = 0; c < bounceCount; c++ )
        myPrintf("v[%d] = %.2f\n", c, bounceList[c]);
    myPrintf("%.2f selected (%d of %d)\n", o->v0, d, bounceCount);
}
#endif
o->e->initialVelocity = o->v0;
timeStep = tmpStep;
}

bool checkBit( long a, long b) {
    return (a & (1<<b)?true:false);
}


/*
                                            e->bounceIndex = bounceLift;
                                        else if( c == 'f')
                                            e->bounceIndex = bounceForce;    


}
*/

void calcInitialvelocityForBounces2( _object *o)
{

    float v0, vGes;
    float h, H;
    float t;
    Juggler *j;
    float r, f, t2;
    float g;
    
    
    j = o->thrower.juggler;
    h = j->position.y;
    t = o->airtime * .5f;
    g = jpPrefs.gravity*.1f;

    
    H = .5 * g * t * t;
    vGes = g * t;
  
    f = .5 * g;    
    t2 = vGes /(2 * f);
    
    r = H + t2 * t2;
    r = sqrt( r );
    
    t = (r + t2)/f;
    
    v0 = -vGes + g * t; 
    o->e->initialVelocity = v0;
    
    _myPrintf("v0 = %.8f\n", v0);
}    

//---------------------------------------------------------------------------
// make a list of all throws
// needed for:
// + calculation of dwell times
// + calulation of balls in orbits,
// + needed for calculating length of pattern with different objects
// ( calculation of hand movement - uses own list)

// get memory
void allocThrowList( void ) {
    int c;
    throwEntry *t;
	
    if( throwList )
        free( throwList );
	
#if 0
	myPrintf("---------------------\n"
			 "initalizing thorwList\n"
			 "---------------------\n");
#endif
	
    throwEntryCount = throwMomentCount;
	
    throwList = (throwEntry *)malloc( sizeof( throwEntry) * throwEntryCount );
	
    for( c = 0; c < throwEntryCount; c++ ) {
        t = throwList+c;
		t->who.juggler = t->to.juggler = getJuggler(0);
        t->who.h = t->to.h = noHand;
        t->aTime = 0;
		t->ss = 0;
		// xxx every pointer points to itself by default
		// neccessary for the first entry, but why not initailize them all the same way?
		// this should fix the bug that an empty pattern (siteswap 0) will hang up the app as it has no balls, no objects and no
		// connection between throws
#if 0
        t->next = NULL;		// xxx = t;
        t->prev = NULL;		// xxx = t;
#else
        t->next = t;
        t->prev = t;
#endif
		
		t->orbitNr = NO_ORBIT;
		t->used = false;
		t->registered = false;
    }
}


// add to memory
throwEntry *addThrowEntry( sse *theEvent, Juggler *j, float delayedTime ) {
    throwEntry *t;
    Juggler *jd;
    t = throwList + th;

	if( t->registered )
		return t;
	else{		
		t->registered = true;									
		t->aTime = aTime;										// when does it happen
		
		t->duration = eventPointer->duration;					// duration (not needed yet)
		t->who.juggler = getJuggler(theEvent->who);				// who did throw
		t->who.h = j->hand;
		t->to.juggler = getJuggler(theEvent->to);				// who to catch
		t->to.h = (theEvent->cross == true ? getAlternateHand( j->hand): j->hand );
		if( theEvent->who != theEvent->to) {
			jd = getJuggler( theEvent->to);
			if( j->start != jd->start )
				t->to.h = getAlternateHand(t->to.h);
		}
		
		t->ss = theEvent->ss;                                   // what was it?
#if 0
		assert( t->ss < 100 );	
#endif
		t->x = theEvent->cross;
		t->muxCount = muxCount;
		t->delay = delayedTime;
		t->e = theEvent;
		
//		myPrintf("throw entry added: %2.2f%c at %2.2f index = %3d\n", t->ss, t->who.juggler == t->to.juggler?' ':'p', aTime, th);
	}
	
    return t;
}

/*
 get object, that is thrown at whenToThrow
 who				Jugglers number
 hand            Jugglers hand
 whenToThrow     time of throw
 
 Ausgaben:
 Struktur
 mit gefordertem Wert, wenn mglich
 nchster, sonst
 */
throwEntry *getThrowAt( _side to, float whenToThrow ) {
    int c;
    throwEntry *t, *d;
    float dt;
	
    t = NULL; // was throwList
	
    while( whenToThrow >= patternTime)
        whenToThrow -= patternTime;
	
    for( c = 0; c < throwEntryCount; c++) {
        d = throwList+c;
        dt = d->aTime-whenToThrow;      // time
		
        if( dt == 0 ){
			if( d->who.juggler == to.juggler && d->who.h == to.h){	// juggler and hand ok
				if(!d->muxCount){									// mo follow ups in mux
					t = throwList+c;								// weve got him
					break;
				}
			}
        }
    }
	
    return t;
}


/*
 look for the throw with same hand before a given throw
 needed to calculate dwell time from dwell ratio
 */
throwEntry *getThrowBefore( throwEntry *t) {
    int d;
    throwEntry *h, *f;
    float n,o = patternTime;
	
    f = NULL;
	
    // look for same person and hand before t
    for( d = 0; d < throwEntryCount; d++) {
        h = throwList+d;
		
        if(h->who.juggler == t->who.juggler && h->who.h == t->who.h){		// h.who, was h.to
        	n = t->aTime - h->aTime;										// time near to t
            while( n < 0. )
                n += patternTime;
            if( h != t && n < o ){		//dont find same   && n
				if(!h->muxCount){		// no follow ups in mux
					f = h;				// save
					o = n;
				}
            }
        }
    }
	
    if( !f )        // nothing found: use same
        f = t;
	
    return f;	
}

/*
 look for the throw with same hand after a given throw 
 to find orbits in hurried patterns. 
 in hurried patterns, now+ss does not neccessarily give the time when an object is rethrown.
 */
throwEntry *getThrowAfter( _side to, float whenToThrow) {
	int c;
    throwEntry *t, *d;
    float dt, o = patternTime*2;
	
    t = NULL; // was throwList
	
    while( whenToThrow >= patternTime)
        whenToThrow -= patternTime;
	
    for( c = 0; c < throwEntryCount; c++) {
        d = throwList+c;
		
		// no follow ups in multiplex throws
		if( d->muxCount )
			continue;
		
		if( d->who.juggler == to.juggler && d->who.h == to.h){		// juggler and hand ok
			dt = d->aTime-whenToThrow;								// delta t
			while( dt < 0 )											//  normed to pattern time
				dt += patternTime;
			
			if( t == NULL || dt < o ){								// first: take this, otherwise: take it, if better
				t = d;												// save pointer
				o = dt;												// save delta t
			}
        }
    }
	
    return t;
}

/* 
 information about every is saved in a file, 
 as it it used later to combine some of the orbits:
 same self throws done by differnt juggers get the same orbit color
 here we generate the filename
*/
void genOrbitFileName(char *name, int i ){
	sprintf( name, "%sorbitFile%d.txt", sysDir, i);
}


/* + sets pointers to previous and following throws
// + calulates round of pattern, until all balls are at the starting place again.
// + calculates number of orbits
// returns bool for valid Pattern
*/
bool connectTimeList( void ) {
    throwEntry *actThrow, *lastThrow, *t, *u;
    int c;
    float ballsInOrbit;
    float ssValues;
    bool ret = true;
	char orbitFile[10000];
	FILE *fp;	
	
	if(0 && th == 0 )
		return false;
	
	chdir( sysDir);
	
	// reset counter and markers
	for( c = 0; c < throwEntryCount; c++) {
        actThrow = throwList + c;
        lastThrow = getThrowBefore( actThrow );
        actThrow->prev = lastThrow;
        lastThrow->next = actThrow;
		actThrow->i = c;
		
        actThrow->used = false;
		actThrow->orbitNr = NO_ORBIT;
		
		while( actThrow->aTime >= patternTime)
			actThrow->aTime -= patternTime;
			
    }
	orbitCount = -1;
    patternLength = 1;
	
	

    // get amount of balls in orbits
    // needed for calculating length of films
	
	// walk through all entries
    for( c = 0; c < throwEntryCount; c++) {
        actThrow = throwList + c;
        t = actThrow;
        ssValues = 0;
		
		
		// mark zeroes as used, i.e. orbit has been checked
		// No orbit for zeroes!
		if( t->ss == 0 )
			t->used = true;
	
		
		
		// we reached a throw that was already marked
		// this chain has been found before
		// continue with next entry
        if( t->used )
            continue;
		else{
			// this one was unmakred. 
			// => We found a new orbit.
			orbitCount++;

			genOrbitFileName(orbitFile, orbitCount );			
			fp = fopen(orbitFile, "w");
			if( fp == 0 )
				return false;			

			// walk through all throws in orbit
			do {
				int r;
				
				fprintf(fp, "%2.2f", t->ss	);
				r = t->who.juggler - t->to.juggler;
				if( r ) {
					if( r < 0 )
						r += jugglerCount;
					fprintf(fp, "r%d", r);
					// print marker for same hand (don´t care if cross or tramline)
					if( t->who.h == t->to.h)
						fprintf(fp, "i"	);
						
				}	
				// sum up, mark as used, save orbit number
#if 0
				assert( t->ss < 100 );	
#endif
				ssValues += t->ss;
				t->used = true;
				actThrow->orbitNr = orbitCount;
							
				// find next throw
				u = getConnection(t);
		
				// fill in pointers
				t->next = u;
				u->prev = t;
				
				// prepare for next round
				t = u;

				// if we arrive at an entry we read already,
				//  we found all throws in the orbit
				//  or there is a logical fault in the pattern.
				// => abort the search
			} while( !t->used ); 
			
			
			fclose(fp);
		}
				
		if( ssValues ) {
            double d1, d2;
            ballsInOrbit = ssValues/momentCount;
            if((d1 = modf(ssValues/momentCount, &d2)) != 0) {
                ballsInOrbit *= 1 + d2;
            }
            //_myPrintf("ballsInOrbit %f\n", ballsInOrbit );
            if( ballsInOrbit >= 1)
                patternLength = kgv( patternLength, ballsInOrbit);
        }
    }
    patternLength *= momentCount;//in * (ash?2:1);
	
    //_myPrintf("patternLength %d\n", patternLength );

	
	// check for identical orbits
	// read every orbitFile
	// print it to buffer twice. Makes it easy to find rotations
	// open every orbitFile with bigger index
	// if buffer contains bigger indexed orbit file, this will become the same index as first file
    if( 01 ){
		int i, j, k;
		char *c, *d, *e;
		
		// walk through all orbit files
		for( i = 0; i < orbitCount+1; i++ ){
			genOrbitFileName(orbitFile,i);
			c = readFile(orbitFile, "r");
			if( c ){
					
				// save the content in my buffer twice
				d = (char *)malloc( strlen(c)*2+1);
				sprintf(d, "%s%s", c,c);
				free( c );
				
				// check all orbit files with higher index
				for(j = i+1; j<orbitCount+1; j++){
					genOrbitFileName(orbitFile,j);
					e =	readFile(orbitFile, "r");
					if( e && !strchr( e, 'r') ){
						// if content of file is in buffer, it has the same orbit
						if( strstr( d, e )){
							for( k = 0; k < throwEntryCount; k++){
								throwEntry *t = throwList+k;
								
								if( t->orbitNr == j )
									t->orbitNr = i;
							}
						}
						free( e );
					}
				}
				free( d );
			}
		}
	}

	// clean up		
	for( c = 0; c < orbitCount+1; c++ ){
		genOrbitFileName(orbitFile,c);
		remove( orbitFile);
	}
	
	// removing orbits may have produced some unused orbit numbers
	// renumber orbits starting with 0
	int i,j;	
	int currentOrbitNr = 0;
	// walk through all orbit files
	for( i = 0; i < orbitCount; i++ ){
		bool orbitFound = false;
		for( j = 0; j < throwEntryCount; j++ ){
			t = throwList + j;
			if( t->orbitNr == i ){
				t->orbitNr = currentOrbitNr;
				orbitFound = true;
			}
		}
		if( orbitFound )
			currentOrbitNr++;
	}
			
	return ret;
}


// look up the next trow with the same object
// i.e. destination juggler and hand, 
// precise or next time

throwEntry *getConnection( throwEntry *now) {
    throwEntry *d;
 	
	// no throw: return this
    if( now->ss == 0 || now->ss < 0)
        return now;

	
	if( (d = getThrowAfter(now->to, now->aTime+now->ss-hurriedValue)) == NULL){
		d = now;
	}
	return d;
	
}
