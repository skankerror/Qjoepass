/*
 *  juggler.cpp
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


#include <string.h>
#include <assert.h>


#include "globals.h"
#include "juggler.h"
#include "headers.h"
#include "jugglers.h"
#include "grafik.h"
#include"constants.h"
#include "vmath.h"
#include "world.h"

extern float timeStep;


Juggler::Juggler() {}


Juggler::~Juggler() {}

// Init von Klassenvariabeln
bool Juggler::walkMode = false;
Vector3D  Juggler::schulter  = Vector3D(0,   50.f, 0);
Vector3D  Juggler::schritt   = Vector3D(0,   PELVIC_POSITION, 0);
Vector3D  Juggler::fuss      = Vector3D(0,   BASIC_POSITION, 0);
Vector3D  Juggler::kopf      = Vector3D(0,   21.f, 0);
float   Juggler::SchulterFaktor  = 1.0f;
float   Juggler::FootFaktor      = 0.8f;
float   Juggler::headSize        = 13.0f;
float   Juggler::lineSize        = 3.5f;
//char	Juggler::*name			 = NULL;


void Juggler::setHandSideVektors( int side ) {

    punkt t, r;
    t = offsetHand * HAND_FAKTOR;
    if( side == rightHand )
        t = t * -1.f ;

    r = punkt( 0,-20,0);
    t = t + r;
    r = viewdirection * 20.f;
    t = t + r;

    if( side == leftHand )
        lh = position + t;
    else if( side == rightHand )
        rh = position + t;

}

void Juggler::showArmCurve() {

	_showArmCurve( lC);
	_showArmCurve( rC);
}

#define ShowCurve	01
#define ShowVectors	01

void Juggler::_showArmCurve( hCurve curve){
	Vector3D r1, r4;
	Vector3D h1, h4;
	float f = 0.2;
	punkt a,b;
	int c;
	sysColor modifierColor, pathColor;
	

	
	if( curve.isCatchMovement == false){
		modifierColor = red;
		pathColor = blue;
	}
	else{
		modifierColor = green;
		pathColor = yellow;
	}
	

#if ShowVectors
	h1 = position + curve.p4;
	r1 = h1 + curve.r1 * f;
	
	h4 = position + curve.p1;
	r4 = h4 + curve.r4 * f;
	
	drawLine3( &h1 , &r1, 1, modifierColor);
	drawBall3(&r1, 1, 5, modifierColor);
	
	drawLine3( &h4, &r4,  1, modifierColor);
	drawBall3(&r4, 1, 50, modifierColor);
#endif
	
#if ShowCurve	
#define STEPS	20
	a = position + curve.p1;
	for( c = 1; c < STEPS + 1; c++){
		f = c / (float)STEPS;
		b = curve.getPointFromHCurve(f) + position;
		drawLine3(&a, &b, 1, pathColor);
		a = b;
	}
#endif	
}

// Die< Jongleure berechnen
/* Benötigt:
NOWHERE
IDLE_LENGTH

*/
void Juggler::calcJuggler() {
    Vector3D          t;// = {0,0,0};
    Vector3D          ts;
    Vector3D          js; // Schritt
    line3           s, schultern;
    punkt           tfuss;
    Vector3D          h;
    Vector3D          v,o,n;
    float           fx,fy;
    float           l;
    Vector3D          d;

    bool lunicycle = isSittingOnUnicycle(); // Zur Beschleunigung
    punkt p = position;

    if(lunicycle) {

        // rotation
        if(lastPos.x == NOWHERE ) // init position
            lastPos = position;

        // difference in space
        d = position - lastPos;
        l = d.betrag();
        if(( viewdirection * d ) < 0)
            l *= -1;
        // save position for next time
        lastPos = position;


        // ideling
        if( l != 0) {
            idlePosition = p;
            idleMotion = 0.f;
        } else {

            idleMotion += 360.f/idleFrequence * timeStep;

#define IDLE_LENGTH 20.f

            v = viewdirection * ( IDLE_LENGTH * sin(gradToRad(idleMotion)));
            p = position + v;
            d = p - idlePosition;
            l = d.betrag();
            if(( viewdirection * d ) < 0)
                l *= -1;

            idlePosition = p;
        }

        // wheel rotation
        pedalAngle += l;                     //angle
        rotation = pedalAngle * 360 /( myPi * myPi* wheelSize);
    }



    // Die Schultern sind hier!
    // Alle Zeichnungen relativ dazu berechnen
    // Vergleiche setViewPosition()
    //      ts = vektorSumme( &p, &schulter);
    ts = p + schulter;


    // Arme und Hnde
    h = offsetHand * (SchulterFaktor);
    s.a = ts + h ;                   // s.a == linke Schulter
    h = s.b = ts - h ;                        // s.b == rechte Schulter
    schultern = s;

    // Schultern
    ls = s.a;
    rs = s.b;

    p = s.b;                // Schulterpositionen merken
    // Linker Ellbogen, erst berechnen
    s.b = calcElbow( s.a, lh, viewdirection, ts);
    dpl = ellbowContrain;
    le = s.b;    // we need this one for drawing clubs etc
    // Linke Hand
    s.a = s.b;
    s.b = lh;

    s = schultern;          // Wo waren die Schultern doch gleich

    // Rechter Ellbogen, erst berechnen
    s.a = calcElbow( s.b, rh, viewdirection, ts);
    dpr = ellbowContrain;
    re = s.a;    // we need this one for drawing clubs etc

    // Rechte Hand
    s.b = rh;

    // Krper
    s.a = ts;
    js = s.b = ts + schritt;
    lb = s.b;
    hb = s.a;

    // Kopf
    p = ts + kopf;
    head = p;

    // unicylce stuff


    if(lunicycle) {
        tfuss = ts + schritt;
        tfuss = ts + fuss;
        s.b = js;

        n = offsetHand.normalised();
        t = n * ( axisLength/2+pedalSize/2 );
        s.a = tfuss + t;
        lf = s.a;

        s.a = tfuss - t;
        rf = s.a;

        // Foot position
        fy = kurbelLength * cos( gradToRad(rotation));
        fx = kurbelLength * sin( gradToRad(rotation));

        lf.y += wheelSize;
        rf.y += wheelSize;
        lf.y += fy;
        rf.y -= fy;

        v = viewdirection * fx;
        rf = rf - v;
        lf = lf + v;
    } else {       // Beine
        tfuss = ts + schritt;
        tfuss = ts + fuss;
        s.b = js;

        t = offsetHand * FootFaktor;
        s.a = tfuss + t;
        lf = s.a;

        s.a = tfuss - t;
        rf = s.a;
    }

    if(lunicycle) {
        float d;
        float t;
        float beta;
        float kx,ky;
        Vector3D n,o,u;
        float h;

        h = 90 - fy;
        d = sqrt(fx*fx + h*h);
        t = (OBL*OBL - UBL*UBL + d*d)/(2 * OBL * d );

        beta = acos( t );
        kx = OBL * sin(beta);
        ky = -OBL * cos(beta);

        n = viewdirection * kx;
        lk = lb + n;       // front

        u = aY * ky;
        lk = lk + u;       // up

        o = offsetHand * .7f;
        lk = lk + o;       // outside

        fy = -fy;
        fx = -fx;
        h = 90 - fy;
        d = sqrt(fx*fx + h*h);
        t = (OBL*OBL - UBL*UBL + d*d)/(2 * OBL * d );

        beta = acos( t );
        kx = OBL * sin(beta);
        ky = -OBL * cos(beta);
        n = viewdirection * kx;
        rk = lb + n;       // front
        u = aY * ky;
        rk = rk + u - o;       // up & outside

    } else {
        const float f = OBL/(OBL+UBL);
        // Knee position
        lk = lb - ((lb - lf) * f);
        rk = lb - ((lb - rf) * f);

        n = viewdirection * 2.f;    // front
        o = offsetHand * .01f;   // outside
        lk = lk + n + o;
        rk = rk + n - o;
    }

}


punkt Juggler::calcElbow( punkt shoulder, punkt wrist, Vector3D jn, Vector3D center) {
    punkt elbow = NullVector;  // Das will ich wissen
    Vector3D SH;              // Vektor Schulter Hand
    Vector3D SH2;             // Die Hlfte davon
    Vector3D shn;             // Derselbe normalisiert
    Vector3D sc, scn; // Schulter, Krperzentrum (Nacken)
    punkt M, P, A;  // Siehe Skizze
    Vector3D ji;              // inverser Normalenvektor (kw)
    float h;                // Entfernung ellbogen von Mittelpunkt, nach Pythagoras
    float d,a;      // Abstnde, Lngen
    Vector3D t, u, un;

    float h1;               // Hhe der Hand: 0<=> ganz unten, 1 <=> ganz oben
    float x, y;     // Daraus resultierende Auslenkung nach hinten und seitlich.
    Vector3D s;               // Helferlein

    // Mittelpunkt von Schulter und Hand -> auf einem Kreis um diesen Punkt liegt der Ellbogen
    SH =  wrist - shoulder;             // Richtungsvektor Schulter Hand
    SH2 = SH * .5;                             // Mittelpunkt des Ellbogenkreises. Nicht exakt, da annahme OAL == UAL

    // Entfernung Ellbogen von Mittelpunkt:
    a = SH2.betrag();

    // Arm zu kurz: einfach verlngern
    if( OAL < a ) {
        h = 0;
    } else
        h = sqrt(OAL*OAL - a*a);

    // Wo ist der Mittelpunkt?
    M = shoulder + SH2 ;


    // Normalenvektor aus SH
    d = SH.betrag( );
    if( d == 0 )            // Schulter == Hand, drfte selten vorkommen
        return elbow;   // Nullpunkt zurckgeben, erstmal
    shn = SH * ( 1.f/d ); // Normalenvektor Ellbogen

    // Richtungsvektor Schulter, Krpermitte
    sc = shoulder - center;
    scn = sc * (1/ sc.betrag());       // normalisiert


    // Richtungsvektor fr Ellbogen basteln

    // Abstand Schulter-Hand
    // Uns interessiert nur die z Komponente
    h1 = SH.y;

    // Normalisieren:
    h1 /= OAL + UAL; // -1 ... +1, theoretisch
    // Praktisch:
    // Da wir den Armen erlauben, sich beliebig weit zu strecken, Lnge fr Berechnung anpassen
    h1 = max( -1, min( 1, h1));               // immer weniger als 1

    h1 += 1.;                       // 0...2
    h1 *= myPi / 2.;                       // 0...pi


    // Auslenkung:
    x = -cos( h1 ); // Nach vorn/hinten
    y =  max( 0,sin( h1 )*jpPrefs.ellbowDeflection) ;       // nach aussen ; Der Ellbogen ist nie nher am Krper als die Hand


    // Ortsvektor des Ellbogens bestimmen:
    s = jn * (x * jpPrefs.armForeFactor);                                 // n-richtung
    t = scn * ( y * jpPrefs.armSideFactor );                              // o-richtung
    t.y = -1 * t.betrag();
    ellbowContrain = shoulder + (t * 100.f);

    // Projektion von jn auf Ellbogen-Ebene
    P = M + (s + t).normalised();                              // Ortsvektor in Bezug auf Mittelpunkt
    d = gerAbstandPE( M, shn, P );               // Dessen Abstand zur Ebene
    A = P + (shn *  (-d )) ;                              // Vektor der Lnge d in Richtung der Ebene basteln Zu P addieren. Wir sind in der Ebene

    // Schnittpunkt von MA mit Umkreis ergibt Ellbogen-Position
    u = M - A;                        // Richtungsvektor in Ellbogen-Ebene
    d = u.betrag();                                       // Normalisieren
    if( d == 0 )            // Schulter == Hand, drfte selten vorkommen
        return elbow;   // Nullpunkt zurckgeben, erstmal

    un = u * ( 1/d );


    // Der Ellbogen
    elbow = M + (un * (-h )) ;          // mal Abstand ergibt Ellbogen-Richtung Plus Ortsvektor ergibt Position

    return elbow;
}

// Bewege Jongleur.
// Ziel steht in p1, n1.
// In Standpunkt wird der aktuelle Standpunkt eingetragen,
// vTime und rTime wie gehabt.
// aus dem aktuellen timestep wird dann die Schrittgre berechnet.
void Juggler::moveOneJuggler() {


    if( rTime == 0)                      // Wir sind schon da!
        return;

    const float dt = (rTime < timeStep)? rTime:timeStep;      // diesmal so lange

    // Zeit anpassen
    vTime += dt;
    rTime -= dt;

    // lineare Bewegung (vergl. doMoveOneObject() )
    position = pStart + ((pEnd - pStart) * ( vTime / (vTime + rTime) )) ;

    // Vorwrts, rckwrts
    position = position + (viewdirection * ( modPath * vTime * rTime )) ;

    // drehen, o neu berechnen
    viewdirection = viewdirection.drehungY(winkelN * dt);
    setJugglerOrientation();
    setHandVektors( );
}

void Juggler::walkOneJugglerAlongSpline() {

	assert( aTime >= 0 );
	
	position = getPosOfJugglerAtOrAfter( nr, aTime );

    const Vector3D t = getPoiOfJugglerAtOrAfter(nr, aTime) - position;
    viewdirection = t.normalised();

    setJugglerOrientation();
    setHandVektors();
}

void Juggler::setName( char *n){
	
	if( name )	
		free( name );
	name = NULL;
	
	if( n ){
		name = (char*)malloc( strlen( n ) + 1 );
		sprintf(name, "%s", n);
	}
}


char *Juggler::getName( void ){
	return name;
}


