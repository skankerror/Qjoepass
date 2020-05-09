/*
 *  parseSiteswap.h
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

#ifndef __PARSE_SITESWAP__
#define __PARSE_SITESWAP__


#if defined(__cplusplus)
extern "C++"
{
#endif


#include "headers.h"

// definitions
#define MAX_ONE_TIME                     2
#define ZEROX_TIME                      .2

#define BSTRING "#s b"
	

// Vorbelegungen fr Datenfelder
//#define ERROR							-2
#define SELF                            -1

//#define UNDEFINED                     -1
#define RES_TO_UNDEFINED				-1
#define SS_INIT                         -1
#define SS_EMPTY                        -2
#define SS_SHORT_HOLD					-3
	
#define NO_JUGGLER                       0 // War NULL
#define NO_CHANGE                       -1
#define FOREVER                         -1

#define DONT_BOUNCE                     -1
#define NO_COLOR                        -1

#define NIS								1   /* No Info String, don´t print new infos */

	
#define lineCommentCode					'!'	// Kommentare
#define blockCommentCode				'"'	// Kommentare
#define globalMetaCode					'#'	// Globale Parameter
#define localMetaCode					'#'	// Lokale Parameter
	
// defaults
//#define DEF_DWELL						.5f

#define selectRandomObject()    ((rand() % (nothing -1)) + 1)

// Siteswap-Modus: erweitert, einfach, nicht definiert
enum {xMode, sMode, nMode};

// Hnde manuell umschalten
enum { switchNot = 0, switchThis, switchAll};

// Handzuordnung
enum { syncSwitch,syncLeft, syncRight, syncUnforced};
enum { noBounce, stdBounce, forceBounce };
enum { bounceForce, bounceHyperLift, bounceLift, bounceHyperForce};
enum { HandDeflectionMultiply, HandDeflectionOverwrite};

	
	
typedef struct jm {	punkt pos;			// position
					punkt posIn;		// position, incomming
					punkt posOut;		// position, outgoing
					punkt poi;			// point of interest
					punkt poiIn;		// point of interest, incomming
					punkt poiOut;		// point of interest, outgoing
					float t;			// trigger time
					int pirouettes;		// number of additional pirouettes
					struct jm *next;    // pointer to next structure
				}_jm;

extern  _jm             *firstJm[];


extern int				gridX;
extern int				gridY;
extern bool				grid2Parameters;

extern float			hurriedValue;
extern wwwBoolean       hurriedMode;
extern wwwBoolean       hasIvisibilityTag;
extern wwwBoolean       beatmapMode;

extern int              lmpCount;
extern int              loopCount;
extern int              eventCount;
extern int              jugglerCount;
extern int              ballCount;
extern int              throwCount;
extern int              momentCount;
extern int				rawPatternTime;
extern float			siteswapCount;
extern int              ssHand;
extern int              passCount;
extern float			patternTime;
extern int              throwMomentCount;
extern int              ssMaxVal, ssMinVal;
extern float			handDwell;
extern wwwBoolean       obeyDelay;
extern float			maxDwell;

extern sseLoop			*theFirstLoop;

extern wwwBoolean       validPattern;
extern sseLoop			*theFirstLoop;
extern punkt			defThrowPosition;
extern char				*theActualPattern;

extern punkt			pN;
extern punkt			pGroupPositions;
extern punkt			jN;
extern int				jugglerPositionFactor;
extern bool				tableJugglingMode;
extern bool				forceTableJugglingMode;

extern dPunkt			defVPosition;
extern dPunkt			defNPosition;

extern style			stdStyle;
extern bool				siteswapError, lastSiteswapErrorState;

int                     getGlobalStyleLength( void );
void                    initSiteswapPattern( char *p, long size);
wwwBoolean              parseSiteswapText( void );
//void                    setJugglerOrientation( Juggler *j);
//punkt                   calcBodyCenter( punkt p );
void                    setHandVektors( Juggler *j );           // Ortsvektoren fr leftHand/rightHand setzen
//void                  setHandSideVektors( Juggler *j, int side ); // und dasselbe fr nur eine Hand
int                     setSiteswapMode( int mode );
dPunkt                  getDPoint( dPunkt* in );

punkt                   makeOutgoingVector( punkt p, punkt i );
punkt                   makeIncommingVector( punkt p, punkt i );
wwwBoolean              calculateSizeOfPlainSiteswap(char *p, int *j, int *e);
void                    setAnimationSpeed( float bps );
void                    setGravity( float g );
void					setTableJugglingMode( bool mode );
	

enum{	
						PositionSideToSide, 
						PositionBackToBack, 
						PositionLine, 
						PositionDropbackLine, 
						PositionCircle, 
						PositionFeed, 
						PositionGorillaFeed, 
						PositionReverseOrder,
						PositionTable,
						PositionShortDistance
};


extern	char			*positionMenuName[];
extern int				positionMode;
extern int				positionMirrorXFlag;
extern int				jugglersInPattern;
	
extern bool				bgSoundDefined;	
	
	
#if defined(__cplusplus)
}
#endif


#endif

