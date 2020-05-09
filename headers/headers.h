/*
 *  headers.h
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

#ifndef __JP_HEADERS__
#define __JP_HEADERS__

// max() definiert?
#include <stdlib.h>
#include "vtypes.h"
#include "mydefs.h"
// Objecttypen
#include "juggler.h"
#include "hcurve.h"
#include "hand.h"

#if defined(__cplusplus)
extern "C"
{
#endif
	
	//  ---------------------------------------- definitionen ----------------------------------------
#define LOOP_MAX                5000                    // number of loop
#define LMP_MAX                 5000                    // number of movements
#define LSP_MAX                 5000                    // Lokale style parameter
#define LP_MAX                  1000                   // Lokale object size
#define CMP_MAX                 5000                     // Lokale style parameter
#define JUGGLER_MAX				5000						// number of jugglers
#define BALL_MAX                10000                    // number of objects
#define EVENT_MAX				10000					// number of siteswap events
#define SPIN_MAX                BALL_MAX				// mam. number of spin
	
#define PATTERN_SIZE			64000					// currently fixed size buffer (baaaad)
	
	// Fr Abwurfposition
#define SIDE					x
#define UP						y
#define BACK					z
	
	// ---------------------------------------- Datantypen ----------------------------------------
	
	// ---------------------------------------- Datentypen 3----------------------------------------
	// Open GL vs Mac
	
#if 0
	// old mac stuff - obsolete
	// screen pixel
	typedef struct {
		float			x, y;							// coordinates
		float			dx, dy;							// factors for x and y (if not a round object)
		float			dz;								// distance to object
		float			relSize;						// relative size (depends on distance camera-projection)
		wwwBoolean		visible;
	} punkt2;
	typedef struct { punkt2 a, b; }line2;
	
	
	// Perpendicular on a plane, with distance
	typedef struct {
		punkt			p;								// coordinates
		float			dCam;							// distance to camera
		float			relSize;						// distance to projection point
		wwwBoolean		visible;						// visible?
	}_lotPunkt;
	
	// objects have a sice (rect on mac) and a distance to the screen
	typedef struct { 
		rect			r;
		float			d;
	}object;
#endif
	
	// ---------------------------------------- datentypen 3----------------------------------------
	// composite data:
	// objects in 3d, control structures for siteswap events...
	
	
	
	// postion with amount and kind of dwell 
	typedef struct  _sPunkt {
		punkt12			p;								// position, angle, spin
		float			deflectionValue;				// style deflection factor
		int				deflectionMode;					// dwell absolute (postion definitionstarts with '*')
	}dPunkt;
	
	typedef struct { punkt a, b; }line3;
	

	
	// camera movement                      
	typedef struct  cameraMovementPointer{  
		float			duration;						
		int				mode;
		punkt			p1, p2, p3, p4;
		float			angle;
		struct cameraMovementPointer *next;
	}_cameraMovementPointer;
	
	// nested loops
	enum { loop, event };
	
	typedef struct {
		int				typ;
		
		union{  
			struct		_sseLoop *loop;
			struct		_sse *event;
		}ssP;
		
	}_inTheLoop;
	
	
	// a loop itself
	typedef struct  _sseLoop{
		char			info[255];					// information text
		char			music[255];					// music, not implemented 
		float			delayList[JUGGLER_MAX][2];	// list of delay for all jugglers, both hands
		struct _lsp		*theLsp;					// pattern subloop
		struct _lmp		*theLmp;					// movement subloop
		struct _lp		*theLp;						// object size subloop
		
		_cameraMovementPointer *cameraMovementPointer;
		_inTheLoop inTheLoop;						// one level deeper, 
		struct _sseLoop	*nextLoop;					// continue here when finished with this loop
		struct _sseLoop	*outTheLoop;				// one level up (when nextLoop == nil)
		int				count;						// number of repetitions.
		float			duration;					// duration of a loop (including the duration of all subloops)
		int				passCorrectionMode;			// will styles for pass movements automatically be changed by defaults to make std. patterns look nice? (def: yes)
//yyy passCorrectionMode war wwBoolean
		int				rest;						// number of repetitions to go
	}sseLoop;
	
	// LoopStyleParameter (juggling style)
	typedef struct  _lsp{   
		struct _styleEl	*p;							// pointer to first throw or nil == no new pattern
		int who;									// who is doing it(-1: all)
		struct _lsp		*nextLsp;					// more pointers, for other jugglers
	}lsp;
	
	
	// LoopMotionParameter (movement, old style)
	typedef struct  _lmp{   
		int				who;						// who
		
		float			duration;					// how long does the movement take?
		punkt			pEnd;						// where to be at the end?
		Vector3D		nEnd;						// where to lool?
		float			modPath;					// modifier (neg: back, pos: fore)?
		float			p;							// number of pirouettes?
		struct _lmp		*nextLmp;					// more pointers?
	}lmp;
	
	
	enum loopParameterType {GravityType, TpsType, JugglerColorType, JugglerSizeType, JugglerObjectType, ObjectColorType, ObjectSizeType, ObjectKindType, DropAllType, EndOfType};
	
	// loop parameters - replaces the sub-loops in previous versions
	typedef struct  _lp{   
		int				type;
		int				nr;							// who / what are we talking about
		punkt			color;						// the desired color
		float			size;						// the desired size
		float			transitionTime;				// time to change color/size
		int				objectKind;					// object: which kind
		int				value;						// value for speed or gravity
		struct _lp		*nextLp;					// pointer to next struct 
	}lp;
	
	
	
	// style definition
	typedef struct _styleEl{
		struct _styleEl *next;						// pointer to next sub pattern (ring: end == begin)
		struct _sPunkt	v;
		struct _sPunkt	n;							// starting value, end value
		}styleEl;
	
	
	// style - list
	typedef struct _style{						
		struct _style	*next;						// pointer to next pattern, nil = last
		char			name[256];					// name of pattern
		int				count;						// number of entries for this pattern
		styleEl			*first;						// pointer to first structure of this pattern
	}style;
	
	// juggler orientation
	typedef struct  ort{
		float			absT;						// kw absolute time of throw / catch
		punkt			p;							// postion
		Vector3D		o;							// orientation
		Vector3D		n;							// normalized vector
		Vector3D		tDir;						// direction at moment of throw
	}_ort;
	
	
	// what happens at a thros
	typedef struct _sse{
		float			ss;                         // siteswap value, float
		float			dwell;                      // dwell time, if defined explicite 
		float			change;                     // air time, if defined explicite 
		float			delay;                      // amount of delay
		float			resultingDelay;             // total Delay (global + local delay)
		wwwBoolean		inDelay;                    // this event is in the delay queue
		wwwBoolean		cross;                      // force throw to change side (right-left, that is)
		int				who;                        // id of throing juggler?
		int				to;                         // id of catching juggler?
		int				sameHand;                   // us the last hand again!
		struct			{int t:1,c: 1, o:1;}rel;    // what of the following is defined 
		dPunkt			throwPosition;              // rel. throw position, if defined
		dPunkt			catchPosition;              // rel. catch position, if defined
		
		struct _sse		*sim;                       // pointer to simultanous throw
		struct _sse		*mux;                       // pointer to multiplex throw
		struct _sse		*jong;                      // pointer to next juggler (or nil )
		struct _sse		*next;                      // pointer to next event. last event points to first event
		float			duration;                   // duration of event, invalid for subsequent events
		float			spin;                       // number of spins
		float			spinFaktor;                 // factor for spin (-1 = negative rotation, chops)
		wwwBoolean		beep;                       // mark an event. say beep at throw time
		wwwBoolean		hurried;                    // marked as a hurried throw
		struct __object	*o;							// ball thrown by this event
		int				bounce;                     // bounce mode
		unsigned long	dummy;						// for unexpected cases
		int				th;                         // last time of execution
		wwwBoolean		invisible;                  // makes the ball invisible when thrown
		
		// will move to dPunkt later
		float			throwAngle;
		float			throwDeclination;
		Vector3D		orientation;
		Vector3D		rotation;
		float			dummieFloat;
		wwwBoolean		isMhnHurried;               // mhn* notation
		bool			usePassCorrection;
		wwwBoolean		color;
		int				handUsedThisTime;           
		int				relativeJugglerOffset;
		float			initialVelocity;
		int				bounceIndex;
		wwwBoolean      bounceParameterDefined;
		float           gravity;
		float           mux_correction;
		wwwBoolean		h2;
		bool			roll;
	}sse;
	
	
	// Ein Object
	typedef struct  __object{					// an object
		sse				*e;						// Pointer to triggering event for this object
		sse             *first;          	   	// Pointer to first event in event tree of the whole throw - contains also mux and sync

			
		int				t;						// throw index
		int				th;						// hand index 
		
		float			airtime;				// total airtime of this throw
		float			vTime;					// elapsed time
		float			rTime;					// remaining time
		float			nextThrowTime;			// time of next throw for this object
		float			delay;					// delay. Written when event is being triggered. Count´s down. Object is thrown if value is <= 0
		bool			inHand;					// true if the club has just been catched - first time that actions take place on catched clubs
		
		// pos
		punkt			s;						// abs. throwing position
		punkt			d;						// abs. catching oisition
		punkt			a;						// actual position
		
		// Handbewegung
		_side			thrower;				// Thrower: juggler, hand
		_side			catcher;				// Catcher: juggler = -1 <=> not defined yet)
		float			throwTime, catchTime;	// when does it happen
		wwwBoolean		h2;
		
		// rotation
		Vector3D		n;						// dest direction, normalized
		Vector3D		nDest;					// orientation at catch time
		Vector3D		nHold;					// vector at HoldTime
		Vector3D		na;						// aktueller Normalenvektor
		
		/* this flag rotates the club by 180 percent around his spin axis
		 what will make a club be held and cachted reverse
		 with every half spin of a club, switch this flag.
		 the 2nd flag marks an already reversed club that is thrown with half spins and will get it´s normal orientation back when it is catched
		 */ 
		bool			halfSpinFlag;
		bool			doesHalfSpinThrow;
		Vector3D		cIn, cOut;				// movement vector for incomming and outgoing movement at a catch, x = NOWHERE: not defined
		Vector3D		tIn, tOut;				// movement vector for incomming and outgoing movement at a thrwo, x = NOWHERE: not defined
		
		Vector3D		r;						// rotation vector
		float			spins;					// number of spins
		float			info;
		wwwBoolean		thrown, catched;
		wwwBoolean		dropped;

		int				bounce;					// mode of bounce
		float			v;                     	// offset for bouncing (lift, force)
		float			v0;
		float			h;
		void			*pos, *pos2;
		float			size;
		bool			roll;
		
		
		// for world view
		int             kind;					// kind of Object
		wwwBoolean		invisible;				// invisible object (not drawn)
		
		int				catchingJugglerIndex;	// view mode: receiving jugglers color 
		float			nextSiteswapValue;		// view mode: siteswap value
		
		wwwBoolean		shortDistanceFlag;		// true if distance is short to another object
		punkt			*pathList;				// list of points to print the path
		int				pathCount;				// number of points in list

		sysColor		color;					// which one
		sysColor		usedColor;
		Vector3D		dummiePoint;
		wwwBoolean		hasSelfColor;
		wwwBoolean		isSelf;
		wwwBoolean		isPass;
		
		wwwBoolean		colorDefined;			// is a color defined for this ball?
		sysColor		startColor, endColor;
		float			rColorTransitionTime, 
						vColorTransitionTime;
		wwwBoolean		sizeDefined;			// is a color defined for this ball?
		float			startSize, endSize;
		float			rSizeTransitionTime, 
						vSizeTransitionTime;
		struct te		*actThrow;
		int				orbitNr;
		unsigned long	dummy;					// for unexpected cases
		bool			beepThisTime;			// debugging information placeholder
	}_object;
	
	
	// all I have to know about a throw
	typedef struct  {
		int e;								// throw id
		struct ort      t;					// where, when, who, to whom is something thrown
		struct ort      c;					// where, when, who, by whom is something catched
		wwwBoolean set;						// true, if the position has ben saved
	}_jP;									// Jugglers Position
	
	
	
	
	typedef struct _handPos{
		float   absT;						// Absolute time
		int     th;							// event-Number
		_side   s;							// juggler / hand
		hCurve  hC;							// position and direction vectors for hand movement 
		wwwBoolean muxCount;				// sameTime: dont find this when looking for next action
		_object   *o;
		Vector3D  dtDir;					// direction vector at moment of throw
		Vector3D  rtDir;					// rotation  vektor at moment of throw
		Vector3D  dcDir;					// same for catch
		Vector3D  rcDir;
		bool	  rtDirSpecified;			// rtDir specivied by pattern => do not change vectors 
		bool	  dtDirSpecified;			// rtDir specivied by pattern => do not change vectors 
		int		 what;						// debug stuff
	}_handPos;
	
#if defined(__cplusplus)
}
#endif


#endif

