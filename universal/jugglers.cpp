/*
 *  jugglers.cpp
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
Moving
*/


#include "globals.h"

#include "jugglers.h"
#include "vmath.h"
#include "mem.h"
#include "vtypes.h"
#include "mydefs.h"
#include "myutil.h"
#include "world.h"
#include "siteswap.h"
#include "animationWin.h"
#include "runaround.h"

#include "animationView.h"
#include "systemswitch.h"





_handPos *actMove[JUGGLER_MAX][2];              // Die aktuelle Bewegung fuer jede Hand

void walkOneJugglerAlongSpline(Juggler *juggler);

//float armForeFactor = .5;
//float armSideFactor = .5;
//float deflectionPassFactor = 1.5;

_handPos *handPos = NULL;
// jeder Hand:
_handPos *firstMove[JUGGLER_MAX][2];    // Zeiger auf erste Bewegungen fuer jede Hand

int hc;

bool      canMove = false;                                // Speicher zum Laufen alloziert.
// Positionen der Jongleure und Bewegung der Hnde
_jP             *jugglerPosition = NULL;
int             posCount;
int				actualJugglerViewIndex = 0;

void            setMovementContent( _handPos *a );
void            addMovement( _handPos *prev, _handPos *next, _handPos *ins);
void            checkHandPositions( void );
void            showHandList( _handPos *f );

void            doAllJugglersMovement( void );
void            moveAllHands( void );                                   // moves hands to their next position
void            moveOneJuggler( Juggler *j);
void            moveObjectsInHands( void );


punkt           calcElbow( punkt shoulder, punkt wrist, Vector3D jn, Vector3D center);

//------------------------------------ walk -----------------------------


void moveObjectsInHands( void ) {
    int c;
    _object *o;
    Juggler *j;
    punkt p;

    // Auch die Blle in den Hnden mssen bewegt werden
    for( c = 0; c < ballCount; c++) {
        o = getObject( c );
        if( o->catcher.juggler != NO_JUGGLER){	// already used
					
			if( o->rTime <= 0 ){      // Object wird gehalten
				j = o->catcher.juggler;                                         // von wem?
                p = j->offsetHand;
                p = j->position + p;
                p = o->catcher.getHandPosition();
                o->s = o->d = p;
            }
        }
    }
}


/* ---------------- Preflight ----------------
Positions-Angaben zu jedem Wurf speichern
1. jeder Object kennt das Event, das ihn geworfen hat.
2. wird ein Object gefangen, wird
   Zeitpunkt des Abwurfs
   Position des Jongleurs
   Ort seiner Hnde
   gespeichert.
   doMoveOneObject()
3. beim Werfen wird die knftige Fangposition abgefragt und dorthin geworfen.
   Ist die Position noch nicht bekannt, wird die aktuelle Position verwandt.
   doSingleThrow()
3a.Daher wird der Preflight nicht gezeigt: Die Wrfe gehen an komische Stellen.
4. Das Muster mu zweimal durchlaufen und gespeichert werden, da die Joes in der
   ersten Runde andere Wege laufen knnen.
5. Die Werte der zweiten Runde werden verwandt, sobald der Wurf nicht mehr
   im ersten Durchlauf ankommt.
   getJpOffset()
*/

// Neue Version: Alle Abwrfe werden einzelne gezhlt und so eindeutig numeriert
// Speicher anfragen
void initPosition( void ) {
    disposePosition();
    canMove = false;

    posCount = throwCount;// * 2;
    if( (jugglerPosition = (_jP*)malloc( sizeof( _jP) * posCount)) == NULL )
        return;

    canMove = true;
    resetPositions();       // Positionsspeicher lschen
}

void disposePosition(void) {
    if( jugglerPosition != NULL ) {
        free( jugglerPosition );
        jugglerPosition = NULL;
    }
}


// Offset im Speicher berechnen:
// WurfNummer und Runde bestimmen Speicherplatz
_jP *getPosition( int t ) {
    _jP *thePosition;

    //      Darf nicht vorkommen
    while( t >= posCount )
        t -= posCount;

    // Speicherabfrage, mit Positionsanpassung
    thePosition = jugglerPosition + t;
    return thePosition;
}

// Alle Positionen der Jongleure als ungltig eintragen
void resetPositions( void ) {
    _jP *thePosition;
    int c;

    if( !canMove )
        return;

    for( c = 0; c < posCount ; c++ ) {
        thePosition = getPosition( c );
        thePosition->t.p.x = NOWHERE;
        thePosition->c.p.x = NOWHERE;
        thePosition->e = NoIndex;
        thePosition->set
        = false;
    }

}



// Die restlichen Eintrge in Positionsspeicher eintragen.
// wird gesetzt, wenn ein Object gefangen wird
void setCatchPosition( _object *o ) {
    Juggler *theJuggler;
    _jP *thePosition;

    // Drfen wir nicht

    if( !canMove )
        return;
    //printf( "set catch at %2.2f\n", aTime );

    theJuggler = o->catcher.juggler;

    thePosition = getPosition( o->t );
    thePosition->e = o->t;
    thePosition->c.p = theJuggler->position;
    thePosition->c.o = theJuggler->offsetHand;
    thePosition->c.n = theJuggler->viewdirection;
    thePosition->set
    = true;

}

// Position von Jongleur j ermitteln,
// der Object von Wurf t fngt.
// Unbekannt: aktuelle Position zurckgeben
_ort *getCatchPosition(  int t, Juggler theJuggler) {
    static _jP *thePosition;
    static _ort jP;
    extern _object *actualObject;

    // Wenn wir uns nicht bewegen knnen
    if( !canMove ) {
        jP.p = theJuggler.position;
        jP.o = theJuggler.offsetHand;
        jP.n = theJuggler.viewdirection;
        jP.absT = -1;
        return &jP;
    }

    // Position holen
    thePosition = getPosition( t );

    // Position nicht eingetragen: Aktuelle Position
    if( thePosition->set
            == false ) {
        thePosition->c.p = theJuggler.position;
        thePosition->c.o = theJuggler.offsetHand;
        thePosition->c.n = theJuggler.viewdirection;
        if(Juggler::walkMode && actualObject ) {
            thePosition->c.p = getPosOfJugglerAtOrAfter( actualObject->catcher.juggler->getJugglerNr(),  aTime + actualObject->rTime);

        }

        //printf("unknown catch position\n");
    }

    return &thePosition->c;
}

// wird gesetzt, wenn ein Object geworfen wird
void setThrowPosition( _object *o ) {
    Juggler *theJuggler;
    _jP *thePosition;

    // Drfen wir nicht
    if( !canMove )
        return;

    // Wer denn?
    theJuggler = o->thrower.juggler;

    // Position zu diesem Wurf
    thePosition = getPosition( o->t );

    // Als Abwurf-Position speichern
    //    thePosition->t.j = b->catcher.juggler->getJugglerNr();
    thePosition->t.p = theJuggler->position;
    thePosition->t.o = theJuggler->offsetHand;
    thePosition->t.n = theJuggler->viewdirection;
    thePosition->t.tDir = o->n;
    thePosition->e = o->t;
}

// ------------------ hand stuff ----------------------

// Speicher fr Hnde anfragen und Inhalt lschen
void initHands( void ) {
    int c;
    _handPos *p;


    disposeHands();

    // get memory for every throw and every catch.
    hc = throwMomentCount * 2;
    if( (handPos = (struct _handPos *)malloc( sizeof( _handPos) * hc  )) == NULL)
        return;


    // init everything
    for( c = 0; c < JUGGLER_MAX; c++ ) {
        firstMove[c][leftHand]=firstMove[c][rightHand] = NULL;
        actMove[c][leftHand]=actMove[c][rightHand] = NULL;
    }

    for( c = 0;c < hc; c++ ) {
        p = handPos + c;

        p->absT = NoTime;
        p->th = NoIndex;
        p->s.juggler = NO_JUGGLER;
        p->s.h = NoIndex;
        p->hC.gTime = p->hC.vTime = NoTime;
		p->hC.userSpecifiedInHandMovement = p->hC.userSpecifiedOutHandMovement = false;
        p->muxCount = false;
        p->dtDir.x = NOWHERE;
        p->what = Init;
    }

}

void disposeHands( void ) {
    if( handPos ) {
        free( handPos );
        handPos = NULL;
    }
}


//------------------------------------ animation -----------------------------

// Alle Jongleure bewegen die Hnde
void moveAllHands( void ) {
    int c;
    hCurve *hC1, *hC2;

    if( handPos == NULL )   // Kein Speicher da
        return;

    for( c = 0; c < jugglerCount; c++ ) {
        Juggler* j = getJuggler(c);
        _side right= _side(j,rightHand); // war 0
        _side left = _side(j,leftHand);  // war 1 yyy
        hC1 = right.moveOneHand();
        hC2 = left.moveOneHand();
        j->calcJuggler();
        if( hC1 )
            hC1->vTime += timeStep;
        if( hC2 )
            hC2->vTime += timeStep;
    }
    return;
}


//------------------------------------ mem stuff -----------------------------

// Zeiger auf Struktur einer Aktion holen
// Eingaben: Ereignisnummer, Art der Aktion
_handPos *getHandPtr( int th, int what) {
    _handPos *hP;
    int i;


    // Liste  (aktion: 0=werfen, 1=fangen)    Speicherbank fr diese Runde
    i = th*2 + (what == Throw?0:1) ;

    hP = handPos + i;
    if( i >= hc )
        beep( );

    return hP;
}

// calculate content of hand movement
void setMovementContent( _handPos *a ) {
    _handPos        *n;
    float           t;


    // get pointer
    n = getHandAfter( a );

    // calc time
    t = n->absT - a->absT;
    while( t<0 )
        t+=patternTime;

    // avoid crashes under linux
    if( t == .0f ) {
        //      _myPrintf("setMovementContent: t =0\n");
        t = patternTime;
    }

    a->hC.gTime = t;
    a->hC.vTime = 0;


	// by default, hands follow std path
	a->hC.userSpecifiedOutHandMovement = a->hC.userSpecifiedInHandMovement = false;
	
	
    // set positions
    a->hC.p4 = n->hC.p1;

	// hand speed equals object speed at the moment of throw or catch
	a->hC.r4 = n->hC.r1;

	
	// NOT true if the user has specified hand vectors for this throw
    // set direction
	if( a->hC.userSpecifiedOutDir.x != NOWHERE ){
		a->hC.r1 = a->hC.userSpecifiedOutDir;
		a->hC.userSpecifiedOutHandMovement = true;
	}
	if( a->hC.userSpecifiedInDir.x != NOWHERE ){
		a->hC.r4 = a->hC.userSpecifiedInDir;
		a->hC.userSpecifiedInHandMovement = true;
	}
}



//------------------------------------ fill in values -----------------------------
// FangPosition speichern
// ggf. Veknpfen
// Eingabe: Neu ausgelste Bewegung
void saveThrow( _object *o) {
    _handPos *p;
    Vector3D r;
    float t;
    Juggler *j;
    j = o->thrower.juggler;
	

    if( handPos == NULL)    // Kein Speicher da
        goto bail;

    t = o->throwTime;						// Absolute WurfZeit

    while( t >= patternTime )				// Anpassen
    {       t -= patternTime;
    }


    p = getHandPtr( o->th, Throw );
    p->o = o;
    p->th = o->th;							// Ereignisnummer
    p->absT = t;
    p->s = o->thrower;						// Wer wirft?
    p->muxCount = muxCount;

	p->hC.isCatchMovement = false;
	
	p->hC.s = o->s;
	p->hC.d = o->d;
	
	p->hC.p1 = j->rels;						// relative Startposition

    r = o->d - o->s;						// Flugrichtung
    r.y += o->v0 * o->airtime;				// gravity sucks
    p->hC.r1 = r;  							// Fangvektor
    p->hC.factor = (o->thrower.juggler == o->catcher.juggler ? 1.: jpPrefs.deflectionPassFactor);

	// vector secified by parameter
	p->hC.userSpecifiedOutDir =  o->tOut;
	p->hC.userSpecifiedInDir =  o->tIn;
	
	
#if 01
	// not perfect
	if( o->bounce )
		r.y *= 10;
	if( o->bounce == forceBounce || o->bounce == bounceHyperForce)
		r.y *= -1.0;
#endif
	
    p->dtDir = o->n;
    p->rtDir = o->r;

bail:
    ;
    return;

}




// FangPosition speichern
void saveCatch( _object *o) {
    _handPos *p;
    Vector3D r;
    float t;
	Juggler *j;
	j = o->catcher.juggler;

    if( handPos == NULL)    // Kein Speicher da
        goto bail;

    t = o->catchTime;                                                       // Absolute WurfZeit

    while( t >= patternTime )                                       // Anpassen
    {       t -= patternTime;
    }


    p = getHandPtr( o->th, Catch );
    p->o = o;
    p->th = o->th;                                                  // Ereignisnummer
    p->absT = t;
    p->s = o->catcher;                                              // Wer wirft?
    p->muxCount = muxCount;


	p->hC.isCatchMovement = true;
	
	p->hC.s = o->s;
	p->hC.d = o->d;

    p->hC.p1 = j->reld;                                             // Endposition
    r = o->d - o->s;                  // Flugrichtung
#if 01
     r.y -=  o->airtime* o->airtime * jpPrefs.gravity/2; // Fangen: Vertikale Komponente umkehren
	// not perfect
	 if( o->bounce )
		r.y *= 10;
	if( o->bounce == forceBounce || o->bounce == bounceHyperForce)
			r.y *= -1.0;
	r = r * o->airtime;
#else
    if(o->e->bounceIndex == bounceHyperForce || o->e->bounceIndex == bounceForce)
        r.y -= -o->v0 * o->airtime;
    else    
        r.y -= o->v0 * o->airtime;
#endif

	if( tableJugglingMode )
		r.y *= -10;
    p->hC.r1 = r;                                                   // Fangvektor
    p->hC.factor = (o->thrower.juggler == o->catcher.juggler ? 1.: jpPrefs.deflectionPassFactor);

	// vector secified by parameter
	// vector secified by parameter
	p->hC.userSpecifiedOutDir =  o->cOut;
	p->hC.userSpecifiedInDir =  o->cIn;
	
    p->dcDir = o->nDest;
    p->rcDir = o->r;

bail:
    ;
    return;

}

/*
get object, that is thrown at whenToThrow
who                     Jugglers number
hand            Jugglers hand
whenToThrow     time of throw

Ausgaben:
Struktur
mit gefordertem Wert, wenn mglich
nchster, sonst
*/
_handPos *getHandAt( _side s, float whenToThrow ) {
    int c;
    _handPos *t, *d;

    t = handPos;

    while( whenToThrow >= patternTime)
        whenToThrow -= patternTime;

    for( c = 0; c < hc; c++) {
        d = handPos+c;
        if( d->s.juggler == s.juggler && d->s.h == s.h )    // juggler and hand ok
            {       if( d->absT == whenToThrow)                     // time also
                t = d;                                                  // weve got one
            // continue, look for last hand whith at time
        }

    }

    return t;
}


_handPos *getHandAfter( _handPos *t ) {
    int c;
    _handPos *h, *f;
    float n,o = 2 * patternTime;
    bool b;
    f = NULL;

    // browse all saved hand movements
    for( c = 0;c < hc; c++ ) {
        h = handPos + c;

        if(h->s.juggler == t->s.juggler && h->s.h == t->s.h)        // same hand
        {
            n = h->absT - t->absT;                                  // calc time difference
            while( n < 0. )                                                 // fix to pattern duration
                n+=patternTime;

            b = (n < o && n != 0);                                  // shortest difference, not same (multiplex!)

            if( h != t && b )                                               // store it. Dont break, take last valid entry.
            {       f = h;
                o = n;
            }
        }
    }

    if( !f )        // nothing found: use same
    {       f = t;
    }

    return f;
}


void setNextHandAction( _side s, float whenToThrow, int action ) {
#ifdef MAC_CLASSIC_VERSION
#pragma unused( action)
#endif

    static _handPos *t, *t1, *first;

    // next throw
    first = t = getHandAt( s, whenToThrow );

    // walk through the following throws.
    // stop, if its not the same ball / hand
    for(;;) {
        t1 = getHandAt( s, t->absT );
        // stop after one round
        // stop if different ball (i.e. no hold )
        if( t1 == t || t->o != t1->o )
            break;
        t = t1;
    }
    //      _myPrintf("%f\n", t->b->airtime);
    setMovementContent( t );
    t->what = action;
    actMove[s.juggler->getJugglerNr()][s.h] = t;
	
	// save handCurvePointer
	// needed for Feedback
	if( s.h == leftHand )	
		s.juggler->lC = t->hC;
	else {
		s.juggler->rC = t->hC;
	}


}

// ---------------------- use module ----------------------------
void moveAllJugglers( void ) {
    doAllJugglersMovement();
    moveObjectsInHands( );
    moveAllHands();
}




#ifndef MAC_VERSION

void nextJugglerView( bool mode ) {
#ifdef MAC_CLASSIC_VERSION
#pragma unused( mode )
#endif


    animationViewSet(FRONT_VIEW);

    if( actualJugglerViewIndex == -1 )
        actualJugglerViewIndex = 0;
    else {
        actualJugglerViewIndex++;
        if( actualJugglerViewIndex == jugglerCount )
            actualJugglerViewIndex = 0;
    }
	setCameraToActualJuggerView( );
}

void setCameraToActualJuggerView( void )
{
    punkt n, p;
    float f,angle;
    GLfloat   modelMatrix[4][4];
    Juggler *j;
	int i = actualJugglerViewIndex;
	
	if( i < 0 )
		i = 0;
	
    j = getJuggler( i );
    // angle == 0 => view direction == -z
    // get angle of (jugglers view direction, z-axis)
    angle = getAngle(j->viewdirection, aZ);
    f = j->viewdirection * aX;
    if( f < 0 ) {
        angle *= -1;//= 180;
    }
    n = 10.f * j->viewdirection;
    p = n + j->position;
    
	glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();

    glRotatef(-angle+180,0,1,0);                                    // rotate to view direction
    glRotatef( -10, j->offsetHand.x,  j->offsetHand.y,  j->offsetHand.z);      // look up
    glTranslatef(-p.x * jpPrefs.mag,                        // move to position
                 (p.y  - BASIC_HEIGHT + BASIC_POSITION - 20) * jpPrefs.mag,
                 -p.z  * jpPrefs.mag);
	
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)modelMatrix);       // save matrix
	glScalef(jpPrefs.mag, jpPrefs.mag, jpPrefs.mag);

    animationSetMatrix(modelMatrix);

    glPopMatrix();

    jpPrefs.rotationAroundCenter = 0;

}
#endif



#pragma mark -------------- work

// jugglers are walking
// curently only working from within the runaround editor
// 
void doAllJugglersMovement( void ) {
    int c;
	Juggler *j;
	
	// Walk mode activated? set by parseSplineJugglerMovement()
 	if( Juggler::walkMode ){
		for( c = 0; c < jugglerCount; c++){ //&& (c < numberOfRunaroundJugglers || walkMode == true); c++){
			j = getJuggler(c);
			j->walk();
		}
	}
}







