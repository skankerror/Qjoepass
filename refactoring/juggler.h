/*
 *  juggler.h
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

#ifndef JUGGLER_H
#define JUGGLER_H
//#include "headers.h"
#include "vtypes.h"
#include "prefDef.h"
#include "stdio.h"
#include "constants.h"
#include "mydefs.h"
#include "hcurve.h"

#define PELVIC_POSITION -60.f
#define BASIC_POSITION  -170.f
#define SHOULDER_POSITION  50.f

#if defined(__cplusplus)
extern "C"
{
#endif
	
	
	/**
	 @author Thomas Ruhroth
	 */
	class Juggler{
	public:
		// Constructors
		Juggler();
		
		~Juggler();
		
		// Getter & setter
		int getJugglerNr() {return nr;};
		void    setHandSideVektors( int side );
		void	setName( char *n);
		char	*getName( void );
		
		// Some Actioons of Juggler
		void showArmCurve();
		void moveOneJuggler();
		bool isSittingOnUnicycle() {return (unicycle || jpPrefs.unicycles);};
		void sittingOnUnicycle(bool value) {unicycle = value;};
		void walk() { walkMode?walkOneJugglerAlongSpline():moveOneJuggler();}
		void setHandVektors(){setHandSideVektors(leftHand );setHandSideVektors(rightHand);}
		void setJugglerOrientation(){offsetHand = viewdirection.drehungY(90.f ) * (SchulterBreite/2);};
		void calcJuggler();
		punkt calcElbow( punkt shoulder, punkt wrist, Vector3D jn, Vector3D center) ;
		
		// Fields of Juggler
		int nr;                    // Laufende nr
		punkt position;            // Hier steht er
		Vector3D viewdirection;    // Blickrichtung
		Vector3D offsetHand;       // Offset fr linke Hand, innen. Fr andere Positionen umdrehen!
		int start;                 // mit dieser Hand beginnt er
		int hand;                  // Damit wirf er als nchstes
		bool inSync;               // Macht gerade synchrone Wruefe
		bool inAsync;              // Macht gerade asynchrone Wuerfe
		bool leaveAsync;           // Sucht Ausweg aus asynchronen Wuerfe
		bool leaveSync;            // Sucht Ausweg aus synchronen Wuerfen
		int  syncMode;             // jugglers actual sync mode
		float   delay;             // Seine Standard-Verzgerung
		struct _styleEl *sE;       // Zeiger auf nchsten Wurf
		Vector3D dir;              // Bewegungsrichtung
		punkt pStart;              // Da steht er am Anfang der Bewegung
		punkt pEnd;                // Da am Ende
		float winkelN;             // Soviel muss er sich drehen
		float modPath;             // Anpassung des Weges
		
		punkt lastPos;             // Fr die animation des Einrades
		float pedalAngle;          // dt from last position
		float rotation;            // resulting unicycle rotation
		float idleMotion;          // actual ildeling position (if hes not moving fore / back)
		float idleFrequence;       // in this frequence
		Vector3D idlePosition;     // resulting position
		float vTime;               // So lange luft er schon
		float rTime;               // so viel mu er noch
		punkt rels, reld;          // Relative Positionen,
		// mssen zu abs. Position des Jongleurs addiert werden
		punkt lh, rh;              // aktuelle Position der Hnde
		punkt le, re;              // aktuelle Position der Ellbogen
		punkt ls, rs;              // schultern
		punkt lb, hb;              // krper
		punkt head;                // kopf
		punkt lf, rf;              // fu
		punkt lk, rk;              // knie
		punkt rls, rld;            // debug stuff:
		// Relative Positionen,
		// mssen zu abs. Position des Jongleurs addiert werden
		punkt rrs, rrd;            // debug stuff:
		// Relative Positionen,
		// mssen zu abs. Position des Jongleurs addiert werden
		float hurriedDelay;        // actual delay for hurries;
		float hurriedOffset;       // value that is subtracted from hurriedDelay, as long as its >0
		bool firstThrow;           // true if the throw is the first in the pattern (needed for hurries)
		bool colorDefined;         // is a color defined for this juggler?
		sysColor color;            // which one?
		int objects;               // what does he juggle?
		punkt dpl, dpr;
		Vector3D dummy;
		bool invisible;
		float size;
		
		// Klassenvariabeln (werden pro Klasse nur einmal gehalten)
		static bool walkMode;
		
		// Der Jongleur hat seinen Mittelpunkt in Hhe der Hnde.
		// Daraus wird die Schulterhhe abgeleitet.
		// Die Vektoren geben den relativen Punkt zur den Schultern an
		static Vector3D  schulter;
		static Vector3D  schritt;
		static Vector3D  fuss;
		static Vector3D  kopf;
		
		static float   SchulterFaktor;
		static float   FootFaktor;
		static float   headSize;
		static float   lineSize;
		
		sysColor		startColor, endColor;
		float			rColorTransitionTime, vColorTransitionTime;
		
		int			objectKind;
		hCurve	lC, rC; 
		
		// Privat Operations
	private:
		bool unicycle;			// does he sit on an unicycle?
		punkt ellbowContrain;	// BAD: second returnparameter of calcEllbow
		char *name;
		
		void walkOneJugglerAlongSpline();
		void _showArmCurve( hCurve curve);
		};
		
		
#if defined(__cplusplus)
		}
#endif
		
#endif
