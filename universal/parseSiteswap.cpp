/*
 *  parseSiteswap.cpp
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
//--------------------------------------------
// parse a text
// read siteswap information
// wolfgang westerboer
// westwolf@gmx.de
// 05.04.2001
//--------------------------------------------
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>

#include "globals.h"
#include "jugglers.h"
#include "parseSiteswap.h"
#include "prefDef.h"
#include "scanner.h"
#include "siteswap.h"
#include "styleScanner.h"
#include "constants.h"
#include "mydefs.h"

#include "vmath.h"
#include "vtypes.h"
#include "world.h"
#include "mem.h"
#include "applicationFolders.h"
#include "grafik.h"
#include "cameraControl.h"
#include "myutil.h"
#include "myPrintf.h"
#include "stringtoscreen.h"
#include "inputWindow.h"

#include "editor.h"

#ifndef MAC_VERSION
#include "systemswitch.h"
#else
#include "stdUtil.h"
#endif
#include "fileIO.h"
#include "info.h"
#include "fileaccess.h"
#include "preprocess.h"
#include "playSound.h"
#include "runaround.h"


#define abortOn(value)	if((value)) return false
#define breakOn(value)	if((value)) break

#define DefHurriedValue         (2 * .66f)
#define DefDwellValue           DefHurriedValue
#define AutoHurriedValue        (1.5f)

static int      call;
bool			tableJugglingMode = false;
bool			forceTableJugglingMode = false;

_jm             *firstJm[JUGGLER_MAX];
_jm             *getFreeJm( void );
void			freeAllJm( void );
_jm				*addJm( int nr );
void			setJugglerSplineMovement( void );
int				jmCompare( _jm *a, _jm *b );


int				getAlternateHand( int hand );

#if 0
#define __WW_SWITCH__
#define WW_LEFT 'l'
#define WW_RIGHT 'r'
enum {ww_left, ww_right};
#endif


#if 1
#define ChristophesPassingTagForMoreThan2People
#endif
void setRelativePasses( void );
#define relativePassCode 'r'

#ifdef ChristophesPassingTagForMoreThan2People
wwwBoolean christophesPassingTagForMoreThan2People;
wwwBoolean christophesPassingTagForMoreThan2PeopleActivated = false;
#endif

float           hurriedValue    = DefHurriedValue;
wwwBoolean      hurriedMode     = true;
wwwBoolean      mhnHurriedMode  = false;
wwwBoolean      hasIvisibilityTag;
wwwBoolean 		beatmapMode;

//				Order is used in animation->position menu 
char			*positionMenuName[] = {"Side To Side", "Back to Back", "Line", "Dropback Line", "Circle", "Feed", "GorillaFeed", "Reverse Order", "Short Distance"};
int				positionMode = PositionCircle;
int				positionMirrorXFlag = 1;
bool			reverseViewIndex[JUGGLER_MAX];					// reverse orientation in Line
bool			getReverseViewList( void );
bool			closePositionFlag = false;

sysColor defPassColor = sysColor(1,0,0);
sysColor defSelfColor = sysColor(1,1,0);
sysColor passColor;
sysColor selfColor;

lp *getNewLpToLpChain( void );
wwwBoolean	parseGlobalObjectPassColor( void );
wwwBoolean	parseGlobalObjectSelfColor( void );

float   handDwell ;
int     startHand               = leftHand;
int     ssMaxVal;
int     ssMinVal;
float   maxDwell;
float   pt;
float   patternTime;
float	objectColorRange	= 1.f;
float	stdGravity			= 9.81f * 9.81f;
int		gridX				= 6;
int		gridY				= 6;
bool	grid2Parameters		= false;
int		jugglersInPattern	= 1;


//--------------------------------------------
// there has to be at least one style defined.
// Here it is:
// Name of the std style
//--------------------------------------------
char hPName[256]			= "cascade";
#define defStyleCode		'g'                // DefaultMuster eingeben
int		getDefaultStyleIndex( void );
styleEl	*readStyle( void );
style	firstStyle			= { NULL, "", 0, NULL};        // Die im Siteswap-Text definierten Muster
style	stdStyle			= { NULL, "", 1, NULL};  // Muster aus dem File styles.txt
styleEl	*defStyle;
int		getStyleLength( styleEl *f );
void	initStyles( void );

//--------------------------------------------
// std deflections
//--------------------------------------------
#define defVFront               1.f
#define defNFront               1.f
#define defBack                 1.f
#define defUp                   1.f
#define DefRotation             5.f
#define DefElevation			0.f
#define DefRelSpin              1.f
#define DefVx					NOWHERE
#define DefVy					0.f
#define DefVz					0.f
#define defDwell                0.5f

dPunkt          defVPosition = {{	defVFront, defBack, defUp,				// relative throw position
									DefRotation,DefElevation,DefRelSpin,	// object orientation and spin
									DefVx, DefVy, DefVz,					// hand movement: outgoing
									DefVx, DefVy, DefVz },					// hand movement: incomming
									defDwell, false};						// default dwell, true: absolute hand position, ignore dwell

dPunkt          defNPosition = {{	defNFront, defBack, defUp, 
									NOWHERE, DefElevation, NOWHERE, 
									DefVx, DefVy, DefVz, 
									DefVx, DefVy, DefVz}, 
									defDwell, false};


//--------------------------------------------
// object defaults
//--------------------------------------------
punkt pN = punkt(  0, 0, 0); // Liegen anfangs alle im Nullpunkt
punkt pGroupPositions = punkt(0, 0, -200);   // Standpunkt bei Gruppenjonglage ( stellen sich im Kreis auf)
punkt pTablePositions = punkt(0, 0, -100);   // Standpunkt bei Tischjonglage ( stellen sich im Kreis auf)
punkt jN = punkt( 0, 0, 1 );                 // Normalenvektor fr Blickrichtung der Jongleure

typedef struct  {   char *name;
	wwwBoolean(*function)( void);
}commandAtom;

//--------------------------------------------
// camera motion
//--------------------------------------------

_cameraMovementPointer *getCameraMovementPointer( void );
_cameraMovementPointer *getFreeCmp( void );

//--------------------------------------------
// set up the parser
//--------------------------------------------

wwwBoolean parseNewCommands( sseLoop *f);
wwwBoolean
// siteswap engine
parseDefaultJugglerPositions( void ),
parseGlobalXMode( void ),
parseGlobalBMode( void ),
parseGlobalMode( void ),
//              parseGlobalDelayMode( void ),
parseGlobalHurriedValue( void ),
parseInfo(void),
parseSound(void),
parsePassCorrectionMode(void),
parseGlobalMHN( void ),
parseGlobalMHNHurried( void ),
parseGlobalSiteswap( void ),
parseGlobalBeatmap( void ),
parseGlobalAutoHurriedMode( void ),
parseGlobalDwellRatio( void  ),
parseGlobal1DwellRatio( void  ),
parseGlobalStartHand( void ),
parseGlobalMaximumDwell( void  ),
//"( void ", parseGlobalSync( void  ),
parseGlobalEmptyBeats( void ),
parseGlobalDelayMode( void ),
setReplacement( void ),
// camera control
parseCameraReset( void ),
parseCameraPoiMovement( void ),
parseCameraPosMovement( void ),
parseCameraMovement( void ),
parseCameraPan ( void ),
parseCameraTilt ( void ),
parseCameraMovementFactor( void ),
parseCameraPosMoveThrough( void ),
parseCameraPoiMoveThrough( void ),
parseCameraUpMoveThrough( void ),
parseCameraMoveAlong( void ),
parseCameraPos( void ),
parseCameraPoi( void ),
parseCameraUp ( void ),
parseCameraMode( void ),


// object control
parseGlobalObjectCount( void ),
parseGlobalObjectKind( void ),
parseGlobalObjectSpins( void ),
parseGlobalObjectSize( void ),
parseGlobalObjectPassFrom( void ),
parseGlobalObjectColor( void ),
parseGlobalObjectColorMode( void ),
parseGlobalObjectBounceFrom( void ),
parseGlobalObjectPassColor( void ),
parseGlobalObjectSelfColor( void ),
parseGlobalObjectInHalfSpin( void ),

parseLocalObjectColor(void ),
parseLocalObjectSize(void ),
parseLocalObjectKind( void ),

// style control
parseLocalStyle( void ),
parseGlobalStyle( void ),

// juggler control
parseGlobalJugglerSize( void ),
parseGlobalJugglerColor( void ),
parseGlobalJugglerPosition( void ),
parseGlobalJugglerUnicycle( void ),
parseLocalJugglerMovement( void ),
parseGlobalJugglerDelay( void ),

parseLocalJugglerDelay( void ),
parseChristophesPassingNotation( void ),
parseLocalJugglerColor(void ),
parseLocalJugglerObjects( void ),

// world control
parseGravity( void ),
parseStdGravity( void ),
parseTPS( void ),
parseTPS10( void ),
parseLocalGravity( void ),
parseLocalDrop( void ),
parseLocalTPS( void ),
parseLocalTPS10( void ),

parseSplineJugglerMovement( void ),
parseGridSize( void ),
parseObjectColorRange( void),
parseJugglerName( void ),

parseSideToSide( void ),
parseLine( void ),
parseDropbackLine( void ),
parseCircle( void ),
parseBackToBack( void ),
parseFeed( void ),
parseTerrorFeed( void ),

parseTableJuggling( void ),

parseShow( void ),

parseDefaultThrowForRunaroundPatterns( void )

//parseWalkDelay( void )
;


commandAtom commandList[] = {
	{"tableJuggling", parseTableJuggling},
	
	{"show", parseShow},
	
	{"sidetoside", parseSideToSide},
	{"sideToSide", parseSideToSide},
	{"SideToSide", parseSideToSide},
	{"line", parseLine},
	{"dropbackline", parseDropbackLine},
	{"dropbackLine", parseDropbackLine},
	{"DropbackLine", parseDropbackLine},
	{"circle", parseCircle},
	{"backtoback", parseBackToBack},
	{"backToBack", parseBackToBack},
	{"BackToBack", parseBackToBack},
	{"feed", parseFeed},
	{"gorillafeed", parseTerrorFeed},
	{"gorillaFeed", parseTerrorFeed},
	{"GorillaFeed", parseTerrorFeed},
	
	
	{"jugglerName", parseJugglerName},
	{"gridSize", parseGridSize},
	{"objectColorRange", parseObjectColorRange},
	
	{"gravity", parseGravity},
	{"stdGravity", parseStdGravity},
	{"speed", parseTPS},
	{"bps", parseTPS},
	{"bps10", parseTPS10},
	{"throwsPerSecond", parseTPS},
	{"throwsPerSecond10", parseTPS10},
	{"tps", parseTPS},
	{"tps10", parseTPS10},
	
	{"localGravity", parseLocalGravity},
	{"localDrop", parseLocalDrop},
	{"localTps", parseLocalTPS},
	{"localTps10", parseLocalTPS10},
	
	{"cpn", parseChristophesPassingNotation},
	
	{"walk", parseSplineJugglerMovement},
	
	{"autoHurried", parseGlobalAutoHurriedMode},
	{"hurriedValue", parseGlobalHurriedValue},
	{"HurriedValue", parseGlobalHurriedValue},
	{"hurried", parseGlobalHurriedValue},
	{"Hurried", parseGlobalHurriedValue},
	
	{"cameraMoveReset", parseCameraReset},
	{"cameraMovementReset", parseCameraReset},
	{"cameraReset", parseCameraReset},
	{"cameraSetPos", parseCameraPos},
	{"cameraSetPoi", parseCameraPoi},
	{"cameraSetUp", parseCameraUp},
	{"cameraPosMovement", parseCameraPosMovement},
	{"cameraPoiMovement", parseCameraPoiMovement},
	{"cameraMovement", parseCameraMovement},
	{"cameraPan", parseCameraPan},
	{"cameraTilt", parseCameraTilt},
	{"cameraPosMoveThrough", parseCameraPosMoveThrough},
	{"cameraUpMoveThrough", parseCameraUpMoveThrough},
	{"cameraPoiMoveThrough", parseCameraPoiMoveThrough},
	{"cameraMoveAlong", parseCameraMoveAlong},
	{"cameraMovementFactor", parseCameraMovementFactor},
	{"cameraMode",  parseCameraMode},
	{"cameraMovement",  parseCameraMode},
	
	{"replace", setReplacement},
	{"siteswapMode", parseGlobalMode},
	{"extendedSiteswap", parseGlobalXMode},
	{"basicSiteswap", parseGlobalBMode},
	{"delayMode", parseGlobalDelayMode},
	//typo                                                          {"dwellTime", parseGlobalHurriedValue},
	{"emptyBeats", parseGlobalEmptyBeats},
	{"mhn*", parseGlobalMHNHurried},
	{"mhn", parseGlobalMHNHurried},
	{"siteswap", parseGlobalSiteswap},
	{"beatmap", parseGlobalBeatmap},
	{"bm", parseGlobalBeatmap},
	{"maximumDwell", parseGlobalMaximumDwell},
	{"useStyle", parseLocalStyle},
	{"dwellRatio", parseGlobalDwellRatio},
	{"dwellRatio1", parseGlobal1DwellRatio},
	
	{"jugglerGlobalStyle", parseGlobalStyle},
	{"jugglerLocalStyle", parseLocalStyle},
	{"jugglerDelay", parseLocalJugglerDelay},
	{"jugglerStartLeft", parseGlobalStartHand},
	{"jugglerPosition", parseGlobalJugglerPosition},
	{"defaultJugglerPositions", parseDefaultJugglerPositions},
	{"jugglerUnicycle", parseGlobalJugglerUnicycle},
	{"jugglerMove", parseLocalJugglerMovement},

	{"objectCount", parseGlobalObjectCount},
	{"objectSize", parseGlobalObjectSize},
	{"jugglerColor", parseGlobalJugglerColor},
	{"objectColor", parseGlobalObjectColor},
	{"objectColorMode", parseGlobalObjectColorMode},
	{"objectKind", parseGlobalObjectKind},
	
	{"jugglerLocalColor", parseLocalJugglerColor},
	{"jugglerObjects", parseLocalJugglerObjects},
	{"jugglerLocalObjects", parseLocalJugglerObjects},
	
	{"objectLocalColor", parseLocalObjectColor},
	{"objectLocalSize", parseLocalObjectSize},
	{"objectLocalKind", parseLocalObjectKind},
	{"objectSpin", parseGlobalObjectSpins},
	{"objectBounceFrom", parseGlobalObjectBounceFrom},
	{"objectPassColor", parseGlobalObjectPassColor},
	{"objectSelfColor", parseGlobalObjectSelfColor},
	{"objectInHalfSpin", parseGlobalObjectInHalfSpin},

	{"showInfo", parseInfo},
	{"playSound", parseSound},
	{"passCorrection", parsePassCorrectionMode},
	// shorter definitions and repetitions
	{"vanillaSiteswap", parseGlobalBMode},
	{"vanilla", parseGlobalBMode},
	{"extendedSiteswap", parseGlobalBMode},
	{"extended", parseGlobalBMode},
	{"info", parseInfo},
	{"position", parseGlobalJugglerPosition},
	{"delay", parseLocalJugglerDelay},
	{"move", parseLocalJugglerMovement},
	{"globalStyle", parseGlobalStyle},
	{"localStyle", parseLocalStyle},
	{"startLeft", parseGlobalStartHand},
	{"spin", parseGlobalObjectSpins},
	{"count", parseGlobalObjectCount},
	{"kind", parseGlobalObjectKind},
	{"passColor", parseGlobalObjectPassColor},
	{"selfColor", parseGlobalObjectSelfColor},
	{"bounceFrom", parseGlobalObjectBounceFrom},
	
	{"defaultThrowForRunaroundPatterns", parseDefaultThrowForRunaroundPatterns},
	
//	{"runaroundDelayWalk", parseWalkDelay},
	
	// shortest definition
	{"d", parseLocalJugglerDelay},
	{"sx", parseGlobalXMode},
	{"sb", parseGlobalBMode},
	{"s", parseGlobalMode},
	{"i", parseInfo},
	{"D", parseGlobalDelayMode},
	{"H", parseGlobalHurriedValue},
	{"@", parseGlobalObjectSpins},
	{"k", parseGlobalObjectKind},
	//                              {"(", parseGlobalSync},
	{"-", parseGlobalEmptyBeats},
	{"r", setReplacement},
	{"j", parseGlobalJugglerPosition},
	{"b", parseGlobalObjectCount},
	{"J", parseGlobalJugglerColor},
	{"O", parseGlobalObjectColor},
	{"o", parseGlobalDwellRatio},
	{"1", parseGlobal1DwellRatio},
	{"h", parseGlobalStartHand},
	{"a", parseGlobalMaximumDwell},
	{"g", parseGlobalStyle},
	{"l", parseLocalStyle},
	{"m", parseLocalJugglerMovement},
	
	{NULL, NULL}    // end of commandList
};

#define commandEnd				';'
#define nextSplinePoint         ','
#define endOfSplinePoints       ';'


										// Meta-Parameter
										// global
#define globalHurriedCode       'H'                     // Hurried switch
#define autoHurriedCode         '+'
#define notAutoHurriedCode      '-'
#define hurriedOn      '+'
#define hurriedOff     '-'
#define delayCode       'd'             // delay angeben
#define rightDelay     'l'             // in the beginning, hands are changed (for some stupid reasons)
#define leftDelay      'r'             // so letters has to be changed here, too
#define noDelay        '='             // Kein Wert ist auch ein Wert
#define delayModeCode   'D'             // event an delay anpassen?
#define delayOn        '+'
#define delayOff       '-'
#define replaceCode     'r'             // Textersatz
#define jugglerCode     'j'				// Jongleur-Beschreibung
#define ballCode         q				// Anzahl Blle
#define modeCode        's'             // Siteswap-Modus
#define simpleMode     'b'             // einfach
#define extendMode     'x'             // erweitert
#define dwellCode       'o'             // dwell Ratio
#define dwell1Code      '1'             // dwellRatio fr 1 angeben
#define handCode        'h'             // Anderer Hand benutzen
#define maxDwellCode    'a'				// absolute maximum of dwell
#define jugglerColorCode 'J'            // jugglers color
#define objectColorCode 'O'             // Objects color
#define colorCode       'c'             // define juggler / object color

#define syncLeftCode	'l'             //  immer mit leftHand beginnen
#define syncRightCode	'r'				//  immer mit rightHand beginnen
#define syncSwitchCode	'%'				//  immer umschalten, auer bei 1x (Joes Methode)
#define emptyBeatsCode  '-'				// enter default for empty beats
#define spinCode		'@'             //  number of spins
#define usePassCorrectionCode '='			// no extra spin on passes

#define spinFaktorCode	'f'             // spin faktor (-1 = reverse spin, chops)
#define startSpinCode  ':'				// first value for spin is named
#define kindCode		'k'             //
										// local
#define infoCode		'i'             // Info Text folgt
#define movementCode	'm'             // Bewegung der Jongleure
#define pathCode		'x'             // Nichtlineare Bewegung
#define pointStart		'('             // punkt einlesen
#define pointNext		','
#define pointEnd		')'
#define loopStart		'{'             // Das folgende Muster wird mehrmals wiederholt
#define loopEnd			'}'
#define loopPar			','             // Darf nach der Anzahl der Wiederholungen stehen
#define metaPar			','             // Mu nach Parameterangaben stehen
										//  Siteswap-Ereignisse
#define simStart		'('             // Beginn SimultanErgegnisse
#define simNext			','             // Trenner bei simultan (optional bei Joe)
#define simEnd			')'             // deren Ende
#define passStart		'<'             // Beginn Passingereignisse
#define passNext		'|'             // Trenner beim Passen
#define passEnd			'>'             // deren Ende
#define muxStart		'['             // Beginn Multiplex-Wrfe
#define muxEnd			']'             // deren Ende
#define emptyCode		'-'             // ein leeres Ereignis
#define beepCode		'm'             // say "beep"
#define bounceCode		'b'             // add one bounce every time
#define newBounceCode	'B'             // New bounce modifier, according to bouncing in Paris
#define hyperCode		'H'
#define forceCode		'F'
#define liftCode		'L'
#define hurriedCode		'*'             // throw is hurried (i.e. done by the other hand)

// Parameter fr Siteswap-Ereignisse
// Single-Ereignisse
#define crossCode		'x'             // cross
#define cascadeCode		'c'             // cascade
#define fountainCode	'f'             // fountain
#define passCode		'p'             // pass
#define boyceCode		':'				// Ziel bei einf. Siteswap
#define throwCode		'v'             // Von hier werfen
#define catchCode		'n'				// Da ist die Hand dann
#define hMoveCode		'*'             // Switch: hand deflection absolute if true
#define hMoveCode2		'@'             // Switch: hand deflection absolute if true (only x xoordinate)
#define changeCode		'e'				// Siteswap-Dauer explizit angeben
#define switchCode		'%'				// Hand dauerhaft umschalten
										//#define switch2Code   'r'				// das benutzt juggler saver
#define switch2Code		'*'				// das benutzt juggler saver
#define angleCode       'a'             // hor angle of throw
#define declinationCode '/'				// declination throw
#define hStyleCode		'l'				// Name eines HandMusters
#define hStyleExpl		'#'				// Wird im Text definiert
#define hStyleNum		':'				// Gilt nur fr diesen Jongleur
#define hObjectNum		':'				// Gilt nur fr dieses Objekt
#define hTimeCode		'@'						
char ssEL[] = "<([";                    // Klammern, in dieser Reihenfolge!
wwwBoolean              validPattern = false;           // false in case of syntax error
bool                    siteswapError, lastSiteswapErrorState;                  // true if a pattern is not a siteswap
sseLoop         *theFirstLoop;									// Zeiger auf den Musteranfang
																// Zhler zur Belegung der Listen
int             lmpCount;                                       // Anzahl lokaler Bewegungs Ereignisse
int             lspCount;                                       // Anzahl lokaler Stil Ereignisse
int             ljpCount;                                       // lokale Jongleur Farbe
int             locpCount;                                      // lokale Objekt Farbe
int             lpCount;										// lokale Objekt Size
int             lokpCount;                                      // lokale Objekt kind
int             cmpCount;                                       // Camerabewegungen
int             loopCount;                                      // Anzahl Schleifen
int             jugglerCount;									// Anzahl Jongleure
int             eventCount;                                     // Anzahl Events
																// von siteswap.c bentigt
int             throwCount;                                     // Wie oft wird insgesamt abgeworfen?
int             momentCount;									// Wieviel AbwurfMomente gibt es? (Passen sind einer!)
int				rawPatternTime;									// pattern time, dont try to calc anything
int             throwMomentCount;									// Fr Hnde: Anzahl der Abwrfe, bis das Muster wieder symetrisch ist. (musterlnge bercksichtigen)
float           siteswapCount;									// Gesamt-Summe der SiteswapWerte (float, damit erkennbar ist, ob fehler vorliegen)
wwwBoolean      ssDefined;                                      // true, wenn ein Muster eingegeben wurde (nur - ist ein Fehler)
int             ballCount;                                      // und Blle
float           durartion;                                      // Dauer eines Musters;
int             who;                                            // Wer wirftn da?
int            boyceC;                                         // Dasselbe bei bryce's notation
int             theSiteswapMode;                                // Siteswap-Modus
float           eDuration;                                      // Dauer des SubMusters
wwwBoolean      ash, ash2;                                      // Alternating startHand. true, wenn die Starthand bei jedem Musterbeginn wechselt
int             oneXCount;                                      // Anzahl der 1x im Muster
int             switchHandsCount;								// Anzahl der Handwechsel im Muster
int             emptyBeats;                                     // how many empties after a throw?
wwwBoolean      hasSync, hasAsync;								// check if pattern contains sync and asnyc throws (For info window)
wwwBoolean      obeyDelay;                                      // Delay bei Flugdauer bercksichtigen
#define         DefaultJugglerPositionFactor 1
int             jugglerPositionFactor = DefaultJugglerPositionFactor;

// -------------- Datenfelder,yyy Listen sind im Moment statisch! ------------------
// Es knnten schon jetzt Object-Listen und Jongleur-Listen alloziiert werden
wwwBoolean		firstLoop, isLoop;                                      // Merker fr Schleifen
sseLoop			*actSseLoop;
//----------------------------------------------------
void _initSiteswapPattern( char *thePattern, long theCount);
// Initialisierungsfunktionen
void    initSpins( void );
void    initDelay( void );
void    initLoops( void );
void    initLsp( void );
void    initLmp( void );
void    initLp( void );
void    initCmp( void );
void    initEvents( void );
void    initJugglers( void );
void    initObjects( int bc );
void    setJugglers( int jugglerCount );
void    setDelay( void );
punkt   calcBodyCenter( punkt p );
// Aabfragen, keinen Speicher reservieren
sseLoop *peekFreeLoop( void );                                  // Zeiger auf freie Strukturen abfragen, nicht initialisieren
lmp     *peekFreeLmp( void );
sse     *peekFreeEvent( void );
// Zeiger auf Struktur holen, Speicher reservieren
sseLoop *getFreeLoop( void );                                   // freie Strukturen holen
lmp		*getFreeLmp( void );
sse     *getFreeEvent( void );
lsp     *getFreeLsp( void );


wwwBoolean parseLocalParameters( sseLoop *f);
wwwBoolean _parseLocalJugglerMovement( lmp *theLmp );

punkt   parsePoint( void );										// Ein Punkt
sseLoop *parseLoops( sseLoop *upperLoop );                      // Schleifen
sse     *parseEvents( void );									// Events
sse     *parseSimultanEvent( void );                            // Simultan-Eregnisse
sse     *parsePassEvent( );										// Pass-Ereignisse
sse     *parseMultiplexEvent( void );                           // Multiplex-Ereignisse
sse     *parseSingleEvent( void );								// Einzel-Ereignisse
char    getDigLet( void );										// (groen) Buchstaben oder Zahl einlesen
																// Wurfparameter
wwwBoolean	isCross( char *theString );
int			getDestJuggler( char *theString );
float		getDelay( char *theString );


// ------------------ Parser fr Joe Pass ------------------------
// ---------------------------- Initialisierern -----------------------------------
// Routine bei jedem neuen Muster aufrufen!
void initSiteswapPattern( char *thePattern, long theCount)
{
	char p;
	char *pAdd , *all = NULL, *out;
	
	call = 0;
	
	p = thePattern[theCount];
	thePattern[theCount] = '\0';
	
	
	// ---------------------------------------------------------------------------
	// Some modules are freeing the memory after calling this routine.
	// So we save the pattern.
	// Maybe the user wants to restart it with a different style...
	// ---------------------------------------------------------------------------
	writeFile( actualTempPath, "w", thePattern, strlen( thePattern ));
	
	// check if theres a pattern called FILENAME.add
	// if so,  place it in front of the pattern
	// it describes additional stuff like positions, styles etc
	// this file is not linked to the pattern file by an #include directive, but by it´s name.
	// no changes to the pattern file(s) are neccessary to use this feature
	// not intalled yet
	if( 0 )
	{       
		char *addName = (char *)malloc( strlen( thePatternTitle) + 1000 );
        char *p1;
        strcpy( addName, thePatternTitle);
        p1 = strrchr( addName, '.');
        if( p1 )
        {       *p1 = '\0';
        }
        strcat( addName, ".add");
		
        pAdd = readFile( addName, "r");
		
        free( addName );
	}else {
		pAdd = NULL;
	}

	
	if( pAdd ){
		all = (char *) malloc( strlen(thePattern) + strlen( pAdd ) + 5);
		sprintf(all, "%s\n%s", pAdd, thePattern);
	}else{
		char fBuf[1000];
//yyy		sprintf(fBuf, " \%%.%lds", theCount);
		sprintf(fBuf, " %%.%lds", theCount);	

		all = (char *) malloc( strlen(thePattern)+100);
//		sprintf(all, "%s", thePattern);
		sprintf(all, fBuf, thePattern);
	}
	

	out = all;
	
	_initSiteswapPattern( out, strlen( out ));
	
	thePattern[theCount] = p;
	
	if( pAdd )
		free( pAdd );
	free( all );
}








void _initSiteswapPattern( char *thePattern, long theCount)
{
	int i;
	style *dHPattern;
	char *p;
	
	
#ifdef ChristophesPassingTagForMoreThan2People
	// ---------------------------------------------------------------------------
	//      a request from christophe:
	//      allow p in patterns with  more than 2 jugglers.
	//      if there are more than 2 jugglers, after a 'p' tag a number has to follow.
	// ---------------------------------------------------------------------------
	
	christophesPassingTagForMoreThan2PeopleActivated = false;
	// number of cdewJugglers
	{       int j = 1;
		char *t;
		if( ( t = strchr( thePattern, '<')) != NULL )
		{
			char tmp;
			char *p = strchr( t, '>');
			if( p )
			{
				tmp = *(p+1);
				*(p+1) = '\0';
			}
			while(t && (t = strchr(t, '|')) != NULL  )
			{
				if( *t == '>' )
					break;
				
				
				t++;
				j++;
			}
			
			if( p )
				*(p+1) = tmp;
		}
		christophesPassingTagForMoreThan2People = ( j > 2 );
	}
#endif
	
	
	// -------------------------------------
	// set parameters to defaults
	// -------------------------------------
	loopCount = 0;
	eventCount = 0;
	jugglerCount = NO_JUGGLER;
	siteswapCount = 0;
	ssDefined = false;
	throwCount= 0;
	momentCount = 0;
	ssMaxVal = 0;
	ssMinVal = -1;
	oneXCount = 0;
	switchHandsCount = 0;
	hurriedMode = true;
	hurriedValue = DefDwellValue;
	mhnHurriedMode = false;
	handDwell = jpPrefs.defhandDwell;
	hasSync = hasAsync = false;
	emptyBeats = jpPrefs.emptyBeats;
	obeyDelay = jpPrefs.obeyDelay;
	objectBounceFrom = DONT_BOUNCE;
	passColor = defPassColor;
	selfColor = defSelfColor;
	jugglerPositionFactor = DefaultJugglerPositionFactor;
	ballCount = -1;
	Juggler::walkMode = false;
	hasIvisibilityTag = false;
	passCorrectionMode = correctPassValues;
	beatmapMode = false;
	jugglersInPattern = 1;
	setGravity(stdGravity);
	//stopBackgroundSound();
	runaroundDelayWalk = 0.f;
	

	
	if( !forceTableJugglingMode )
		tableJugglingMode = false;
	forceTableJugglingMode = false;
	setTableJugglingMode( tableJugglingMode );
	
	freeAllJm();
	// -------------------------------------
	// read profile and copy it in front of the siteswap file
	// -------------------------------------
	readProfile();
	if( proFile )
	{
		size_t l;
		p = (char *)malloc( theCount + strlen(proFile) + 10);    // get buffer
		if(!p){
			beep();
			return;
		}
		sprintf(p, "%s\n", proFile);			// copy profile in buffer, add newline
												// Otherwise the first line of the pattern would be added to the last line of the profile
		l = strlen( p ) ;						// resulting lenght
		
		strncat( p, thePattern, theCount);      // add (portion of) pattern file
		theCount += l;							// calulate total length of pattern
		*(p+theCount) = '\0';					// terminate buffer
	}else
	{       p = thePattern;
	}
	
	
	// -------------------------------------
	// init memory (static lists)
	// -------------------------------------
	initScanner(p, theCount);
	setScanLineComment( lineCommentCode );                  // Was ist ein Kommentar?
	setScanBlockComment( blockCommentCode );                        // Was ist ein Kommentar?
	setSiteswapMode( jpPrefs.sm );                                  // default-Siteswap-Modus
	
	i = getDefaultStyleIndex( );                    // DefaultPattern abholen
	dHPattern = getStyleFromIndex( i );
	defStyle = dHPattern->first;
	
	initSpins();
	initDelay();
	initLoops();
	initLmp();
	initLsp();
	initLp();
	initEvents();
	initJugglers();
	initObjects( BALL_MAX );                          // Alles auf Anfang setzen
	setCameraCommands( false );
	setCameraMode( false );
	setAutoMove( false );
	
	initCmp( );
	
	// -------------------------------------
	//  here we go
	// -------------------------------------
	if( (validPattern = parseSiteswapText()) == false )             // Muster initialisieren, Anzahlen ermitteln
	{       goto ende ;
	}
	
	
	// -------------------------------------
	//  empty patterns are faults, too
	// -------------------------------------
	if( !ssDefined )
	{       validPattern = false;
		goto ende;
	}
	
	maxDwell = jpPrefs.maxDwell; // duration of one throw * absolute maximum Dwell
	
	
	// -------------------------------------
	//  cant remember why I defined 0 as a fault.
	// therefore:
	// empty patterns are faults, too
	// is NOT true
	// -------------------------------------
	if( 0 && siteswapCount == 0)
	{       validPattern = false;
		goto ende;
	}
	if( ballCount == -1 )
		ballCount = (float)ceil((siteswapCount)/ (momentCount));
	if( ballCount > BALL_MAX)
	{       validPattern = false;
		goto ende;
	}
	
	setJugglers( jugglerCount );
	setDelay();
	setRelativePasses(  );
	setJugglerSplineMovement();
	
	
	// -------------------------------------
	// start animation
	// -------------------------------------
	setSiteswapStart( theFirstLoop );       // Dem Siteswap-Modul sagen, da es von vorn losgeht
#ifdef __INFO__
	clearInfoString();                                      // Bisher kein Info-String
#endif
	
ende:
		
#if 01
	{
        static  FILE *errorFile, *historyFile;
        // -------------------------------------
        // section abandoned
        // -------------------------------------
		
		
        // -------------------------------------
        // save error information
        // -------------------------------------
		
        errorFile = fopen( errPath, "w");	// save to new file
        if( errorFile )
        {
			if( validPattern )
			{       fprintf( errorFile, "ok");
			}else
			{//     beep( );
				if( ballCount > BALL_MAX)
				{       fprintf( errorFile, "too many objects");
				}else
				{       char c = *(st.parseStart + st.readPos);
#define OFFSET 0
					*(st.parseStart + st.readPos+OFFSET) = '\0';
					fprintf( errorFile, "error at pos %ld:\n\n%s", st.readPos, st.parseStart );
					*(st.parseStart + st.readPos+OFFSET) = c;
					
					// change to line, char in line...
					fprintf(errorFile, "\n\n absolute offset in pattern file is %ld\n", st.readPos+OFFSET-strlen(proFile));
				}
			}
			fclose( errorFile);
        }
        // -------------------------------------
        // save file information
		// DISABLED
        // -------------------------------------
        if(0 && (historyFile = fopen( historyPath, "a+")) != NULL)
        {
			if( !validPattern )
			{       fprintf( historyFile, "%s invalid\n", thePatternTitle);
			}else
			{       fprintf( historyFile, "%s appears to be ok\n", thePatternTitle);}
			fclose( historyFile);
        }
	}
#endif
	
	// -------------------------------------
	// free alocated memory
	// -------------------------------------
	disposeScanner();
	if( proFile )
		free( p );
	
#ifdef __INFO__
	// -------------------------------------
	// init info module
	// -------------------------------------
	resetEventInfo();
#endif
	if(!validPattern )
	{
#if 01
		initOneMessage( validPattern );
#else            
		// bring info window to top
		createInfoWindow( false);
		createInfoWindow( true);
#endif
		
	}else
		siteswapError = lastSiteswapErrorState = false;
	
}


// Kontrollstrukturen initialisieren
void initSpins( void )
{
	int c;
	spinElement *spin;
	
	for( c = 0; c < SPIN_MAX; c++)
	{       spin = getSpin( c );
		spin->n = (c>=2?c-2:0);
		spin->rev = false;
	}
}

// Delay
void initDelay( void )
{       int c;
	for( c = 0; c<JUGGLER_MAX;c++)
		globalDelayList[c][rightHand] = globalDelayList[c][leftHand] = UNDEFINED;
}
// Schleifen
void initLoops( void )
{
	int c,d;
	sseLoop *l;
	
	for( c = 0; c < LOOP_MAX; c++)
	{       l = getLoop( c );
#ifdef __INFO__
		l->info[0] = NIS;
#endif
		l->music[0] = NIS;
		
		
		l->passCorrectionMode = dontChangeMode;
		l->inTheLoop.ssP.loop = NULL;
		l->theLmp = NULL;
		l->theLsp = NULL;
		l->theLp = NULL;
		l->cameraMovementPointer = NULL;
		for( d = 0; d<JUGGLER_MAX;d++)
		{       l->delayList[d][rightHand] = UNDEFINED;
			l->delayList[d][leftHand] = UNDEFINED;
		}
	}
}
// Lokale MetaEreignisse initialisieren
void initLmp( void )
{       int c;
	lmp *theLmp;
	
	for( c = 0; c < LMP_MAX; c++)
	{       theLmp = getLmp( c );
		theLmp->who = 0;
		theLmp->duration = 0;
		theLmp->modPath = 0;
		theLmp->p = 0;
		theLmp->pEnd = pN;
		theLmp->nEnd = pN;
		theLmp->nextLmp = (lmp*)UNDEFINED;
	}
}
// Lokale MetaEreignisse initialisieren
void initLsp( void )
{       int c;
	lsp *theLsp;
	
	for( c = 0; c < LSP_MAX; c++)
	{       theLsp = getLsp( c );
		theLsp->p = NULL;
		theLsp->who = -1;
		theLsp->nextLsp = (lsp*)UNDEFINED;
	}
}


// Lokale MetaEreignisse initialisieren
void initLp( void )
{       int c;
	lp *theLp;
	
	for( c = 0; c < LP_MAX; c++)
	{       
		theLp = getLp( c );
		theLp->nr = -1;
		theLp->transitionTime = 0.f;
		theLp->objectKind = -1;
		theLp->nextLp = (lp*)UNDEFINED;
	}
}

// Lokale MetaEreignisse initialisieren
void initCmp( void )
{       int c;
	_cameraMovementPointer *theCmp;
	
	for( c = 0; c < CMP_MAX; c++)
	{       theCmp = getCmp( c );
		theCmp->next = (_cameraMovementPointer *)UNDEFINED;
	}
}
// Jongleure initialisieren

//in klasse verfrachten
void initJugglers( void )
{ 
	int c;
	Juggler *j;
	
	for( c = 0; c < JUGGLER_MAX; c++ )
	{
		reverseViewIndex[c] = false;
		j = getJuggler( c );
		j->nr = c;
		j->start = j->hand = startHand;// rightHand beginnt, zu Beginn wird die Hand gewechselt.
		j->position = Vector3D(NOWHERE,NOWHERE,NOWHERE);
		j->viewdirection = pN;
		j->offsetHand = pN;
		j->delay = 0.f;
		
		j->dir = pN;
		j->pStart = j->pEnd = pN;
		j->winkelN = 0.f;
		j->modPath = 0.f;
		
		j->vTime = j->rTime = 0.f;
		
		j->sE = defStyle;       // Default-Style speichern
		j->hurriedDelay = 0.0f; // actual delay for hurries;
		j->hurriedOffset = 0.0f;        // value that is subtracted from hurriedDelay, as long as its >0
		j->firstThrow = true;
		
		j->pedalAngle = 45;
		j->lastPos = Vector3D(NOWHERE,NOWHERE,NOWHERE);
		j->idleMotion = 10 * rand() / RAND_MAX;
		j->idleFrequence =3.f * (1.f + rand() / RAND_MAX);
		j->colorDefined = false;
		
		j->sittingOnUnicycle(false);
		j->objects = UNDEFINED;
		
		j->setHandVektors();
		j->invisible = false;
		j->size = 1.f;
		
		j->objectKind = -1;
		j->setName(NULL);
		//			if( j->name )
		//				free( j->name);
//		j->lC = j->rC = NULL;
	}
}

// Ereignisse initialisieren
void initEvents( void )
{       int c;
	sse *e;
	
	// Alle Ereignisse sind leer
	for( c = 0; c < EVENT_MAX; c++ )
	{       e = getEvent( c );
		e->ss = SS_INIT;                                        // Initialisierungs-Flag
		e->dwell = SS_INIT;                                     // kein expleziter dwell
		e->duration = 1.0f;                                     // Dauer der gleichzeitigen Ereignisse
		e->catchPosition = defVPosition;						// Standard-Position
		e->throwPosition = defVPosition;
		e->delay = UNDEFINED;									// nicht verzgert
		e->inDelay = false;                                     // Keine Verzgerung in Arbeit
		e->who = 0;
		e->to = 0;
		e->sim = e->mux = e->jong = e->next = 0;
		e->sameHand = switchNot;
		e->change = NO_CHANGE;
		//e->forcedCross = false;								// normaly everything is determinated by numbers (no 'x')
		e->next = NULL;
		e->spin = NOWHERE;
		e->spinFaktor = 1;
		e->beep = false;
		e->hurried = false;
		e->bounce = noBounce;
		e->dummy = 0l;
		
		e->throwAngle = jpPrefs.throwAngle;
		e->throwDeclination = 0;
		e->orientation.x = NOWHERE;
		e->rotation .x = NOWHERE;
		e->initialVelocity = NOWHERE;
		e->bounceIndex = 0;
		e->bounceParameterDefined = false;
		e->invisible = false;
		e->gravity = NOWHERE;
		e->h2 = false;
		e->roll = false;
		e->usePassCorrection = true;
	}
}
// Blle initialisieren
void initObjects( int bc )
{       int c;
	int kind;
	_object *o, oN;
	oN.t    = 0;                                    // t
	oN.th   = 0;                                    // th
	oN.e    = 0;                                    // ssInfo
	oN.throwTime = 0.f;
	oN.catchTime = 0.f;                             // throwTime, catchTime
	oN.airtime = 0.f;								// airtime
	oN.vTime = 0.f;
	oN.rTime = 0.f;									// vTime, rTime
	oN.inHand = false;
	
	oN.s = Vector3D( 0,0,0 );
	oN.d = Vector3D( 0,0,0 );      // s,d
	oN.thrower.juggler = getJuggler(0);
	oN.thrower.h = noHand;          //Ort
	oN.delay = 0;                                   // delay
	oN.pathList = NULL;
	oN.pathCount = 0;
	
	for( c = 0; c < bc; c++ ){
		o = getObject( c );
		*o = oN;
		o->s = o->d = pN;
		o->e = NULL;
		o->catcher.juggler = NO_JUGGLER;
		o->rTime = -2;                  // Alle Blle sind in den Hnden ( 0 zu Beginn ist ein Fehler )
		o->nextThrowTime = -(c+2);      // Jeder Object kann geworfen werden. Fr spter: int legt die Reihenfolge fest
		o->a = pN;
		//              b->hold = false;
		o->h2 = false;
		o->spins = 0.f;
		o->n = aY;
		o->r = aY;
		
		o->halfSpinFlag = false;
		o->doesHalfSpinThrow = false;
		o->cIn.x = o->cOut.x = NOWHERE;
		o->tIn.x = o->tOut.x = NOWHERE;
		o->bounce = noBounce;
		o->colorDefined = false;
		o->sizeDefined = false;
		o->size = 1.f;
		o->roll = false;
		kind = jpPrefs.os;
		if( kind == random_object )
			kind =  selectRandomObject();
		o->kind = kind;
		o->dummy = 0l;
		o->invisible = false;
	}
}
// SiteswapModus einstellen
int setSiteswapMode( int mode )
{       int ret = theSiteswapMode;
	theSiteswapMode = mode;
	return ret;
}

// switch delay according to startHand
// exchange delay for left and right for all Jugglers starting with the other hand
void setDelay( void )
{
	int c;
	float t;
	Juggler *j;
	for( c = 0; c < JUGGLER_MAX; c++ )
	{       j = getJuggler( c );
		if( j->start == rightHand )
		{       t =globalDelayList[c][rightHand];
			globalDelayList[c][rightHand] = globalDelayList[c][leftHand];
			globalDelayList[c][leftHand] = t;
			
		}
	}
}


void setRelativePasses( void )
{
	
	int c;
	sse *e;
	
	for( c = 0; c < eventCount; c++)
	{
		e = getEvent( c );
		while( e->relativeJugglerOffset < 0 )
			e->relativeJugglerOffset += jugglerCount; 
		
		e->to = (e->to + e->relativeJugglerOffset) % jugglerCount;
		
	}
}

// Abfragen
sseLoop *peekFreeLoop( void )
{       int c;
	sseLoop *l;
	
	for( c = 0; c < LOOP_MAX; c++ )
	{       l = getLoop( c );
		if( l->inTheLoop.ssP.loop == NULL )
			return l;
	}
	return NULL;
}

/*
 Die Krpermitte ist etwas hher
 Daher werden alle Positionen entsprechend der Gre der Jongleure verschoben
 
 Eingabe: Standort
 Ausgabe: Mittelpunkt des Jongleurs (Mitte zwischen Hnden)
 */
punkt calcBodyCenter( punkt p )
{       punkt t = p;       
	t.y -= BASIC_POSITION + SHOULDER_POSITION;
	
	return t;
}
// lokale Parameter-Struktur
lmp *peekFreeLmp( void )
{       int c;
	lmp *l;
	
	for( c = 0; c < LMP_MAX; c++ )
	{       l = getLmp( c );
		if( l->nextLmp == (void *)-1 )
			return l;
	}
	return NULL;
}
sse *peekFreeEvent( void )
{       int c;
	sse *e;
	
	for( c = 0; c < EVENT_MAX; c++ )
	{       e = getEvent( c );
		
		if(e->ss == SS_INIT )
		{       return e;
		}
	}
	
	return NULL;
}

//------------------------------- Ereignisse einlesen --------------------------------------
// Zeiger auf nchste Schleifenstruktur holen
sseLoop *getFreeLoop( void )
{       int c;
	sseLoop *l;
	
	for( c = 0; c < LOOP_MAX; c++ )
	{       l = getLoop( c );
		if( l->inTheLoop.ssP.loop == NULL )
		{       l->inTheLoop.ssP.loop = (sseLoop *)UNDEFINED;   // Wurde schon abgefragt!
			l->nextLoop = NULL;     // Ende der Schleife
			*l->info = NIS;
			loopCount++;            // eine mehr
			return l;
		}
	}
	return NULL;
}
// lokale Parameter-Struktur: Laufen
lmp *getFreeLmp( void )
{       int c;
	lmp *l;
	
	for( c = 0; c < LMP_MAX; c++ )
	{       l = getLmp( c );
		if( l->nextLmp == (void *)UNDEFINED )
		{       l->nextLmp = NULL;
			lmpCount ++;
			return l;
		}
	}
	return NULL;
}
// lokale Parameter-Struktur: Stil
lsp *getFreeLsp( void )
{       int c;
	lsp *l;
	
	for( c = 0; c < LSP_MAX; c++ )
	{       l = getLsp( c );
		if( l->nextLsp == (void *)UNDEFINED )
		{       l->nextLsp = NULL;
			lspCount ++;
			return l;
		}
	}
	return NULL;
}


// lokale Parameter-Struktur: Objekt Farbe
lp *getFreeLp( void )
{   int c;
	lp *l;
	
	for( c = 0; c < LP_MAX; c++ )
	{       l = getLp( c );
		if( l->nextLp == (void *)UNDEFINED )
		{       l->nextLp = NULL;
			lpCount ++;
			return l;
		}
	}
	return NULL;
}


// lokale Parameter-Struktur: Camerabewegung
_cameraMovementPointer *getFreeCmp( void )
{       int c;
	_cameraMovementPointer *l;
	
	for( c = 0; c < CMP_MAX; c++ )
	{       l = getCmp( c );
		if( l->next == (void *)UNDEFINED )
		{
			l->next = NULL;
			cmpCount ++;
			return l;
		}
	}
	return NULL;
}

// Zeiger auf nchstes freies Event holen
sse *getFreeEvent( void )
{       int c;
	sse *e;
	
	for( c = 0; c < EVENT_MAX; c++ )
	{       e = getEvent (c );
		
		if(e->ss == SS_INIT )
		{
			e->ss = 0;                              // Vorgaben
			e->to = e->who = who;   // wer wirft?
			e->relativeJugglerOffset = 0;
			
			
			eventCount ++;                  // eins mehr
			
			return e;
		}
	}
	
	return NULL;
}

// reads a digit or a letter

char getDigLet( void )
{
	char c, ret;
	
	c = peekChar();
	if( isdigit(  c )){
//		ret = getInt();
		ret = getChar() - '0';
	}else if( isupper( c ))
	{       ret = getChar() - 'A'+10;
	}else
	{       ret = -1;
		parameterError = true;
	}
	return ret;
}

punkt parsePoint( void )
{
	wwwBoolean err = false;
	punkt p;
	
	if( peekChar() == pointStart )
	{       getChar();
		p.x = getSignedFloat();
		if( getChar() != pointNext )
			err = true;
		p.y = getSignedFloat();
		if( getChar() != pointNext )
			err = true;
		p.z = getSignedFloat();
		if( getChar() != pointEnd )
			err = true;
	}else
		err = true;     // Das ist kein Punkt!
	
	/*      if( err )
		parameterError = true;
	
	*/      return p ;
}


wwwBoolean parseChristophesPassingNotation( void )
{
	christophesPassingTagForMoreThan2PeopleActivated = true;
	
	return true;
	
}

wwwBoolean parseLocalJugglerDelay( void )
{
	float d;
	int     i;
	int h;
	char c;
	Juggler *j;
	sseLoop *f = actSseLoop;
	for(;;)
	{
		// who
		i = getInt() - 1;
		if( parameterError || i < 0 || i >= JUGGLER_MAX)
			return false;
		// which hand?
		do
		{       if( peekChar() == leftDelay)
		{       getChar();
			h = leftHand;
		}else if( peekChar() == rightDelay )
		{       getChar();
			h = rightHand;
		}else
			h = noHand;     // both!
			
			// how much?
			if( (c = peekChar()) == noDelay )
			{       getChar();
				d = RES_TO_UNDEFINED;
			}else
			{
				d = getSignedFloat();
				
				if( parameterError )
					return false;
			}
			
			if( h == noHand ){
				f->delayList[i][leftHand] =
				f->delayList[i][rightHand] = d;    // save
			}else
			{       // switch delay according to hand
				j = getJuggler(i);
				if( j->start != startHand )
					h = getAlternateHand( h );
				
				f->delayList[i][h] = d;
			}
		}while( (c = peekChar()) == leftDelay || c == rightDelay);
		
		if( peekChar() != ',' ) // more jugglers?
			break;
		getChar();
	}
	
	return parameterError == false; // error
}
wwwBoolean parseGlobalJugglerUnicycle( void )
{
    int	i;
	
	i = -1;
	for(;;)
	{
		char c;
		char mode;
		
		c = peekChar();
		
		if( c == '+' || c == '-')
			i = -1;
		else{
			
			// who
			i = getInt() - 1;
			if( parameterError || i < 0 || i >= JUGGLER_MAX)
				return false;
#ifdef syntaxStrict
			if( peekChar() != hObjectNum )
				return false;
			getChar();
#else
			if( peekChar() == hObjectNum )
				getChar();
#endif				
		}
		// syntax check
		c = getChar();
		switch ( c )
		{       case '-':
			mode = false;
			break;
		case '+':
			mode = true;
			break;
		default:
			return false;
			break;
		}
		
		if( i == -1 ){
			for( i = 0; i < JUGGLER_MAX; i++ )
				getJuggler( i )->sittingOnUnicycle(mode);					
		}else{
			getJuggler(i)->sittingOnUnicycle(mode);
		}
		
		if( peekChar() != ',' ) // more jugglers?
			break;
		getChar();
	}
	
	return parameterError == false; // error
}


wwwBoolean parseDefaultJugglerPositions( void )
{
	wwwBoolean ret = true;
	int nr;
	
	nr = getInt();
	
	switch( nr )
	{
		case 0:         //clockwise
			jugglerPositionFactor = DefaultJugglerPositionFactor;
			break;
		case 1:
			jugglerPositionFactor = -DefaultJugglerPositionFactor;
			break;
		default:
			ret = false;
	}
	return ret;
}
// Anzahl der Blle ist vorgegeben!
wwwBoolean parseGlobalObjectCount( void )
{
	int i;
	i = getInt();                   // wie viele?
	
	
	if( i > BALL_MAX )
		parameterError = true;
	
	ballCount = max(ballCount, i );
	
	return parameterError == false; // Fehler
}
// Default Throw for all Jugglers in Runaround Pattern
wwwBoolean parseDefaultThrowForRunaroundPatterns( void )
{
	int i;
	i = getInt();                   // wie viele?
	
	defaultThrowForRunaroundPatterns = i;
	
	
	return parameterError == false; // Fehler
}// Modus einlesen
wwwBoolean parseGlobalMode( void )
{
	char c;
	int mode;
	c = getCharacter();
	switch ( c )
	{       case simpleMode:
		mode = sMode;
		break;
	case extendMode:
		mode = xMode;
		break;
	default:
		return false;
		break;
	}
	setSiteswapMode( mode );
	return parameterError == false; // Fehler
}
// Modus einlesen
wwwBoolean parseGlobalAutoHurriedMode( void )
{
	char c;
	c = getCharacter();
	switch ( c )
	{       case notAutoHurriedCode:
		jpPrefs.autoHurriedMode = false;
		break;
	case autoHurriedCode:
		jpPrefs.autoHurriedMode = true;
		break;
	default:
		return false;
		break;
	}
	return parameterError == false; // Fehler
}
// Modus einlesen: extended
wwwBoolean parseGlobalXMode( void )
{
	setSiteswapMode( xMode );
	return parameterError == false; // Fehler
}
// Modus einlesen: basic
wwwBoolean parseGlobalBMode( void )
{
	setSiteswapMode( sMode );
	return parameterError == false; // Fehler
}
// Anzahl der Blle ist vorgegeben!
wwwBoolean parseGlobalDwellRatio( void )
{
	float d;
	d = getFloat();                 // wie viel?
	
	if( parameterError || d <= 0 || d >= 1)
		return false;
	handDwell = d;
	return parameterError == false; // Fehler
}
// Wie lange dauert eine 1?
wwwBoolean parseGlobal1DwellRatio( void )
{
	float d;
	d = getFloat();                 // wie viel?
	
	if( parameterError || d <= 0 || d >= MAX_ONE_TIME)
		return false;
	
	jpPrefs.oneTime = d;
	return parameterError == false; // Fehler
}
// Amount of empty beats after a sync throw
wwwBoolean parseGlobalEmptyBeats( void )
{
	int d;
	d = getInt();                   // wie viel?
	if( parameterError || d < 0)
		return false;
	
	emptyBeats = d;
	return parameterError == false; // Fehler
}
wwwBoolean parseGlobalSiteswap( void )
{
	emptyBeats = 1;
	return parameterError == false; // Fehler
}
wwwBoolean parseGlobalBeatmap( void )
{
	emptyBeats = 0;
	beatmapMode = true;
	setSiteswapMode(sMode);
	hurriedValue = .6f;
	hurriedMode = true;
	return parameterError == false; // Fehler
	
}
wwwBoolean parseGlobalMHN( void )
{
	emptyBeats = 0;
	return parameterError == false; // Fehler
}
wwwBoolean parseGlobalMHNHurried( void )
{
	emptyBeats = 0;
	hurriedMode = true;
	hurriedValue = 2 * .66f;
	mhnHurriedMode = true;
	theSiteswapMode = xMode;
	return parameterError == false; // Fehler
}
// alle Jongleure angeben, die mit der linken Hand beginnen
wwwBoolean parseGlobalDelayMode( void )
{
	wwwBoolean ret;
	char c;
	
	c = getChar();
	ret = true;
	
	switch( c )
	{       case delayOn:
		obeyDelay = true;
		break;
	case delayOff:
		obeyDelay = false;
		break;
	default:
		ret = false;
		break;
	}
	
	
	return ret;
}
// get the value for hurried throw correction
wwwBoolean parseGlobalHurriedValue( void )
{
	hurriedValue = getFloat();
	hurriedMode = true;
	return parameterError == 0;
}
// alle Jongleure angeben, die mit der linken Hand beginnen
wwwBoolean parseGlobalStartHand( void )
{
	int nr;
	Juggler *j;
	char c;
	
	for(;;)
	{       // Wer bin ich
		nr = getInt() - 1;
		if( parameterError )
			return false;
		if(nr < 0 || nr >= JUGGLER_MAX )
			return false;
		j = getJuggler( nr );
		j->start = j->hand = getAlternateHand(j->hand) ;
		c = peekChar();
		if( c != ',' )
			break;
		c = getChar();
	}
	
	return true;
}

wwwBoolean parseLocalStyle( void )
{
	lsp *newLsp, *actLsp;
	int i;
	actLsp = actSseLoop->theLsp;
	// Speicher fr Schleife
	if( (newLsp = getFreeLsp()) == NULL )
		return false;
	
	if( actLsp == NULL )
	{       actSseLoop->theLsp = newLsp;                                    // Kette verankern
	}else
	{       while( actLsp->nextLsp != NULL )                                // Ende suchen
		actLsp = actLsp->nextLsp;
		actLsp->nextLsp = newLsp;                                                       // anhngen
	}
	i=-1;
	if( isdigit(peekChar()) )                                                       // wer denn?
	{       i = getInt()-1;                                                                 // Index aus Nummer berechnen
		if( parameterError || i < 0 || i > JUGGLER_MAX )
			return false;
		newLsp->who = i;                                                                // Eintragen
																						// syntax check
		if( peekChar() != hObjectNum )
			return false;
		getChar();
	}
	
	if( (newLsp->p = readStyle()) == NULL )
		return false;
	
	return true;
}
wwwBoolean parseGlobalStyle( void )
{
	int i;
	styleEl *s;
	Juggler *j;
	i = -1;
	if( isdigit(peekChar()) )               // wer denn?
	{       i = getInt()-1;                 // Index aus Nummer berechnen
		if( parameterError || i < 0 || i > JUGGLER_MAX )
			return false;
		
#ifdef syntaxStrict
		if( peekChar() != hObjectNum )
			return false;
		getChar();
#else
		if( peekChar() == hObjectNum )
			getChar();
#endif			
		
	}
	if( (s = readStyle()) == NULL ) // Style einlesen
		return false;
	
	if( i == -1 )                                   // fr jeden
	{       for( i = 0; i < JUGGLER_MAX; i++ )
	{       j = getJuggler( i );
		j->sE = s;
	}
	}else                                                   // Nur einer
	{       j = getJuggler( i );
		j->sE = s;
	}
	return true;
}




// Den ganzen Text durchgehen
// Diese und alle untergeordneten Routinen:
// return == true <=> alles ok
wwwBoolean parseSiteswapText( void )
{
	sseLoop *t, *t2;
	int c;
	
	initStyles();
	
	
	
	/*      if( parseGlobalParameters() == false)   // Parameter einlesen
	 return false;
	 */
	theFirstLoop = getFreeLoop();
	t2 = getFreeLoop();
	
	theFirstLoop->inTheLoop.typ = loop;                             // Legt eine Endlos-Schleife um alles
	theFirstLoop->inTheLoop.ssP.loop = t2;                  // Darin das eigentliche Muster
	theFirstLoop->nextLoop = theFirstLoop;
	theFirstLoop->count = -1;
	t2->inTheLoop.typ = loop;                                               // Darin eine Schleife von der Lnge des DefaultMusters
	t2->nextLoop = NULL;
	t2->outTheLoop = theFirstLoop;
	if( (t = parseLoops( t2 )) == NULL )                            // Darein Schleifen incl. Events einlesen
		return false;
	
	c = getGlobalStyleLength();                                                             // aus Musterlnge der defaultPatterns kgV berechnen
	
	t2->inTheLoop.ssP.loop = t;                                             // Rest der Schleifen verknpfen und anpassen
	
	t2->count = t->count * c;
	t2->duration *= c;
	theFirstLoop->duration = t2->duration;
	
	throwCount *= c;                // Alles mal Lnge des DefaultMusters
	siteswapCount *= c;
	momentCount *= c;
	momentCount /= jugglerCount;
	rawPatternTime = momentCount;
	
	// Gesamtwrfe und -zeiten fr symetrische Muster:
	ash2 = ash = (momentCount%2!=0);                                                        // Ungerade Muster sind erst nach 2 Durchgngen symetrisch
																							//      ash |= oneXCount|switchHandsCount;                                                      // 1x oder forcierter Handwechsel -> immer doppelte Menge an Handspeicher
	ash |= (switchHandsCount/jugglerCount) != 0;  // yyy klammern und vergleich gesetzt                                                                 //      forcierter Handwechsel -> immer doppelte Menge an Handspeicher
																											//      ash = true;     // immer 2 runden. Zu viele ausnahmen sonst
	pt = theFirstLoop->duration;
	// odd number -> double length
	if( pt/2 != (int)pt/2 )
		pt *= 2;
	pt *= (ash?2:1);//xx oneXCount || switchHandsCount ?2:1;
	patternTime = theFirstLoop->duration * (ash?2:1);       // Gesamtdauer des Musters
	throwMomentCount = throwCount * (ash?2:1);                          // Gesamtzahl der Wrfe
	throwCount = throwMomentCount;      //xx I should get used to comment these little hacks
										//      momentCount *=  (ash?2:1);
	return true;
}

// Eine Ereignis-Liste initialisieren
// Erstes Event oder NULL zurck
// Leerer Text ist ein Fehler!
sse* parseEvents( void )
{
	sse *first, *aktual, *old;
	wwwBoolean doParse, ret = true;
	char c;
	float passDuration;
	
	old = NULL;
	
	first = NULL;
	throwCount = 0;                 // Wir stehen am Anfang
	siteswapCount = 0.f;
	momentCount = 0;
	eDuration = 0.f;
	doParse = true;
	do
	{
		passDuration = 1;       // Bisher keine Anpassung
		
		
		aktual = parsePassEvent( );                     // Muster holen, bis zum Abwinken
		if( aktual == NULL )                                            // Ungltiges Event
		{       ret = false;
			continue;
		}
		if( first == NULL )
			first = aktual;
		
		if( theSiteswapMode == sMode )                                                          // Einfaches Siteswap
		{
			c=peekChar();
			if( c == '\0' )                                                 // Fertig
			{       doParse = false;
			}else if(( c = peekChar() ) == loopStart || c == loopEnd || c == localMetaCode || c == '\0' )   // Das mache ich nicht hier!
			{               doParse = false;
			}
		}
		else
		{
#ifdef USE_TIME_CODE
			if( peekChar() == timeCode )                                    // Lnge des Ereignisses.
			{       getChar();
				passDuration = getFloat();
				
				if( parameterError )
				{       doParse = false;
					ret = false;
				}
			}
#endif
			c=peekChar();
			if( c == '\0' )                                                 // Fertig
			{       doParse = false;
			}else if(( c = peekChar() ) == loopStart || c == loopEnd || c == localMetaCode || c == '\0' )   // Das mache ich nicht hier!
			{               doParse = false;
			}
		}
		// Mit alten Ereignis verknpfen
		if( old == NULL )               // erstes: eintragen
			old = aktual;
		else                                    // folgende: anhngen
		{
			while( old->next )      // Aber bitte als letztes in der liste!
				old = old->next;
			old->next = aktual;
			old = aktual;
		}
		// Dauer eintragen, Lnge berechnen
		{sse *t;
			t = aktual;
			do
			{
				t->duration = passDuration;
				eDuration += passDuration;
				t = t->next;
			}while( t );
		}
		
	}while( doParse == true && ret == true);
	
	if( throwCount == 0 )
		ret = false;
	return ret == true? first:NULL;                                 // Alles ok?
}


// Passing-Event einlesen
// Hier werden die Selfs korrekt eingetragen
sse *parsePassEvent( )
{
	
	sse  *f, *e;
	char c, h;
	sse *ep = NULL;
	wwwBoolean next = true;
	int thisC, totalC;
	// Wir passen.
	// Aktueller Werfer steht in globaler Variable who
	f = NULL;
	who = 0;
	boyceC = 0;
	
	// einen Wurf mehr
	thisC = totalC = 0;
	if(peekChar() == passStart )    // Pass-Muster
	{
		getChar();                      // Beginn berspringen
		do
		{
			e = parseSimultanEvent();               // Ereignisse einlesen
			if( e == NULL )                         // oops
				return NULL;
			
			if( totalC && thisC >= totalC )         // nicht der erste und mehr als beim ersten: Fehler
				return NULL;
			// das erste Ereignis
			if( who == 0 )
			{       if( f == NULL)                          // erstes Event merken
				f = e;
			else                                            // Andere Wrfe anhngen
			{       sse *t2;
				
				t2 = f;
				while( t2->next )
					t2 = t2->next;
				t2->next = e;
			}
				do                                                              // Wrfe zhlen
				{       thisC++;
				}while( (e = e->next) != NULL );
			}else
			{       sse *t3,*u;
				
				
				do
				{
					
					// one throw more. stop on error!
					if(++thisC > totalC )
						return NULL;
					
					t3 = ep;                                        // Dieses Ereignis
					while(t3->jong)                         // Den letzten in der Reihe suchen
						t3 = t3->jong;
					t3->jong = e;                           // diesen dranhngen
					
					u = e->next;                                    // Mehrere Wrfe (waren also simultan):
					if( u )                                                 // Verkettung aufspalten
					{       e->next = NULL;
						
						
					}
					ep = ep->next;                          // Beim ersten Jongleure eintragen
					
				}while( (e = u) != NULL);               // Solange was passiert
			}
			h = c = peekChar();
			
			// kommt noch einer?
			if( c == passNext || c == passEnd)
			{
				// erster: Lnge merken
				if( totalC == 0 )
					totalC = thisC;
				// unterschiedliche Lnge (i.e weniger als beim ersten): fehler
				if( totalC != thisC )
					return NULL;
				thisC = 0;
				who++;
				ep = f; // Die Liste wieder von vorn durchsuchen
			}
			
			if( c == passNext )
				getChar();
			// wir haben fertig!
			if ( c == passEnd )
			{       next = false;
			}
			
			
		}while( (next &&
			   ( isdigit( c = peekChar()) || ((isupper(c)) && (theSiteswapMode == sMode)))) ///theSiteswapMode == true)  xxx has to be sMode, not true?
			   || c == simStart || c == muxStart || c == emptyCode
#ifdef __WW_SWITCH__
			   || c == WW_LEFT || c == WW_RIGHT
#endif
			   );
		if(  next || c != passEnd )
			return NULL;
		getChar();      // Ende berspringen
		if( jugglerCount == NO_JUGGLER )        // erstes Mal:
			jugglerCount = who;                     // Anzahl merken
		else if( jugglerCount != who )  // Es mu immer dieselbe Anzahl sein!
			return NULL;
		if( 0 && jugglerCount == 1 )         // Mehr als einer
			return NULL;
		if( boyceC > jugglerCount )     // angegebene Nummer ist zu gro
			return NULL;
		
	}else                           // Einzel-Muster
	{       if( jugglerCount == NO_JUGGLER )
		jugglerCount = 1;
		if( jugglerCount != 1 ) // einmal single, immer single
			return NULL;
		f = parseSimultanEvent();
	}
	return f;
}

// Simultan-Events einlesen
sse *parseSimultanEvent( void )
{
	sse *e1, *e2, *t1, *t2;
	float d;
	char c;
	int eb;
	
	e1 = e2 = NULL;
	if(  peekChar() == simStart )   // Ist simultan
	{		char cx;
		bool crossFirstTime = false, crossSecondTime = false;
		hasSync = true;
		getChar();                                      // Beginn berspringen
		
		
		cx = peekChar();
#define BMCROSSENDHANDSCODE '<'
		if(beatmapMode &&  cx == BMCROSSENDHANDSCODE )
		{
			getChar();
			crossFirstTime = true;
		}
		
		e1 = parseMultiplexEvent();     // Ereignisse einlesen
		
		if( peekChar() == simNext ) // Trenner bei sim ist optional
			getChar();
		
		e2 = parseMultiplexEvent();
		cx = peekChar();
		if(beatmapMode &&  cx == BMCROSSENDHANDSCODE )
		{
			getChar();
			crossSecondTime = true;
		}
		
		if( beatmapMode )
		{
			e1->throwPosition.p.x =	 1;
			e1->throwPosition.p.y =  1;
			e1->throwPosition.p.z =  1;
			e1->rel.t = true;
			e2->throwPosition.p.x =	 1;
			e2->throwPosition.p.y =  1;
			e2->throwPosition.p.z =  1;
			e2->rel.t = true;
			if( crossFirstTime)
				e1->throwPosition.p.y = .5f;
			if( crossSecondTime)
				e2->throwPosition.p.y = .5f;
		}
		
		
		
		
		
		if( e1 == NULL || e2 == NULL || peekChar() != simEnd)   // Fehler
			return NULL;
		
		e1->sim = e2;                           // Simultan-Ereignis eintragen
		momentCount += 1;
		getChar();                                      // Nchstes Zeichen
														// add default of empty beats after a sync
		momentCount += emptyBeats;
		t2 = e1;        // init empty event chain
		for(eb = emptyBeats ; eb > 0; eb--)
		{
			
			t1 = getFreeEvent();    // get empty event
			if( t1 )
				t1->ss = SS_EMPTY;
			else
				return NULL;
			t2->next = t1;                  // add to chain
			t2 = t2->next;
			
		}
	}else
	{       hasAsync = true;
		e1 = parseMultiplexEvent();
		momentCount += 1;
	}
	if( peekChar() == delayCode)                            // Verzgern
	{       getChar();
		d = getFloat();
		if( d <= 0 || d >= 1 )
		{       parameterError = true;
			return NULL;
		}
		//xx            e1->dwell = e2->dwell = d;      // wieso stand das denn hier?
		e1->delay = e2->delay = d;      // sollte so heien
		
	}
	if( e1 == NULL )
		return NULL;
	
	if( peekChar() == handCode)                     // dieselbe Hand nur diesmal umschalten
	{       getChar();
		e1->sameHand = e2->sameHand = switchThis;
	}
	
	if( (c = peekChar()) == switchCode || c == switch2Code)                 // dieselbe Hand dauerhaft umschalten
	{       getChar();
		if( e2 )                                                // you have to switch after e2(sync) resp. e1(async).
			e2->sameHand = switchAll;
		else
			e1->sameHand = switchAll;
		switchHandsCount++;                             // ein Handwechsel mehr
	}
	
	
	return e1;
}

// Multiplex-Wrfe einlesen
sse *parseMultiplexEvent( void )
{
	sse  *f, *e, *o;
	char c, h;
	dPunkt pos;
	sse *t;
	float d;
	
	o = NULL;       // no Multiplex-Event yet
	
	if( (c = peekChar()) == muxStart)
	{
		getChar();                              // Beginn berspringen
		
		do
		{
			e = parseSingleEvent();         // Ereignisse einlesen
			if( e == NULL )
				return NULL;
			
			if( o == NULL)                          // erstes Event merken
				f = e;
			else
				o->mux = e;                     // Andere anhngen
			o = e;
			
			h = c = peekChar();
		}while( isdigit( c) || (theSiteswapMode == sMode && isupper( c ) ) );
		
		if( c != muxEnd )
			return NULL;
		if( f == o )            // Es mssen mindestend 2 Angaben erfolgt sein!
			return NULL;
		getChar();      // Ende berspringen
	}else
	{ 
		if(  beatmapMode )
#if 01
		{
			_myPrintf("first : %d (%c)\n", c,c);                        
			char cx = peekChar();
			bool crossThisTime = false;
			bool alreadyCrossed = false;
#define BMCROSSENDHANDSCODE '<'
			if( cx == BMCROSSENDHANDSCODE )
			{
				getChar();
				crossThisTime = true;
			}	
			
			do{
				
				e = parseSingleEvent();         // Ereignisse einlesen
				if( e == NULL )
					return NULL;
				
				if( o == NULL)                          // erstes Event merken
					f = e;
				else
					o->mux = e;                     // Andere anhngen
				o = e;
				
				if( crossThisTime )
				{
					_myPrintf("hands crossed now\n");
					e->throwPosition.p.x = -1;
					e->throwPosition.p.y =  1.25f;
					e->throwPosition.p.z =  1;
					
					e->rel.t = true;
					
					if( e != f )
					{
						_myPrintf("hands crossed in 2nd throw\n");
						f->throwPosition.p.x =	1;
						f->throwPosition.p.y =  -1.25f;
						f->throwPosition.p.z =  -10;
						f->rel.t = true;							
					}
					
					alreadyCrossed = true;
				}else if( alreadyCrossed )
				{
					_myPrintf("hands already crossed before\n");
					e->throwPosition.p.x =  1;
					e->throwPosition.p.y =  -1.25f;
					e->throwPosition.p.z =  1;
					f->rel.t = true;							
				}	
				
				
				c = peekChar();
				_myPrintf("next char: %d (%c)\n", c,c);                        
			}while( isdigit( c ) || isupper( c ));
			_myPrintf("exiting beatmap mux\n");                    
		}
#else
		{
			_myPrintf("first : %d (%c)\n", c,c);                        
			do{
				
				e = parseSingleEvent();         // Ereignisse einlesen
				if( e == NULL )
					return NULL;
				
				if( o == NULL)                          // erstes Event merken
					f = e;
				else
					o->mux = e;                     // Andere anhngen
				o = e;
				
				c = peekChar();
				_myPrintf("next char: %d (%c)\n", c,c);                        
			}while( isdigit( c ) || isupper( c ));
			_myPrintf("exiting beatmap mux\n");                    
		}
		
#endif				
		else{
			if( (f = parseSingleEvent()) == NULL )
				return f;
		}        
	}
	
	// Wurfort angegeben
	if( peekChar() == throwCode )                           // von wo werfen wir?
	{       getChar();
		t = f;
		pos = getDPoint(&defVPosition);
		
		do                                                                              // gilt fr alle Blle
		{       t->throwPosition = pos;
			t->rel.t = true;
		}while( (t = t->mux) != NULL );
	}
	if( peekChar() == dwellCode )                           // von wo werfen wir?
	{       getChar();
		d = getFloat();
		if( d <= 0 || d >= 1 )
		{       parameterError = true;
			return NULL;
		}
		t = f;
		do                                                                              // gilt fr alle Blle
		{       t->dwell = d;
		}while( (t = t->mux) != NULL );
	}
	
	return f;
}


// get an event, fill in values, return event or NULL in case of an error
sse *parseSingleEvent( void )
{
	sse *e;
	char c;
	wwwBoolean moreParameters;
	wwwBoolean cross;
	wwwBoolean hurryCorrection = false;     // true if a throw is a hurry
	
	// get event
	if( (e = getFreeEvent( )) == NULL )
		return NULL;
	e->isMhnHurried = false;
	// nothing inside
	if( peekChar() == emptyCode )
	{	getChar();
		e->ss = SS_EMPTY;
		if( peekChar() == beepCode )
		{	getChar();
			e->beep = true;
		}
		return e;
	}
	
	
	throwCount++;   // Ein Wurf mehr
#ifdef __WW_SWITCH__
	if( peekChar() == WW_LEFT || peekChar() == WW_RIGHT )
	{       char c;
		c = getChar();
		switch( c )
		{       // remember, the fist thing we do is switching hands.
				// shouldnt have done it like this...
			case WW_LEFT:
				setBit( e->dummy, ww_right );
				break;
			case WW_RIGHT:
				setBit( e->dummy, ww_left);
				break;
			default:
				break;
		}
	}
#endif
	
	// Werte aus defaultPattern holen
	// Auf nchstes Teilmuster setzen
	e->rel.t = e->rel.c = e->rel.o = false;
	e->rotation.x = e->orientation.x = NOWHERE;
	e->dummieFloat = 0.f;
	moreParameters = true;
	cross = false;
	//einfaches site swap
	if( theSiteswapMode == sMode )
	{       e->ss = getDigLet( );
		if(parameterError )
			return NULL;
	}
	// erweitertes site swap
	else
	{
		e->ss = getFloat( );            // Als erstes kommt immer ein Ss-Wert
		if( parameterError )
			return NULL;
	}
	
	if( tableJugglingMode ){		
		if( e->ss != 0){
			e->catchPosition.p.UP =e->throwPosition.p.UP = 1;
			e->catchPosition.p.SIDE = 1;
			if(e->ss == 1 || e->ss == 2)
				e->catchPosition.p.UP = 1.5f; 
			if( e->ss == 2 )
				e->throwPosition.p.UP  = 1.5f;
			if(e->ss/2. == floor(e->ss/2.) )
				e->throwPosition.p.SIDE = 1; // even numbers are placed on the same side of the table
			else
				e->throwPosition.p.SIDE *= -1; // odd numbers are placed on the opposite side of the table
				
			if( e->ss > 2 )
				e->catchPosition.p.BACK = e->throwPosition.p.BACK = (e->ss-2)/3.f + 0.3f;
			// numbers higher than 3 may contain more than 1 ball in the orbit. 
			// shift ball by default to the outside. Increase amount of shift for higher numbers.
			// better: do so only if there are multiple balls in the orbit
			// much better: do the real shift
			if( e->ss > 3 )
				e->catchPosition.p.SIDE = 1.f + e->ss/4.f*.3f;
			
			e->rel.t = e->rel.c = true;
			e->catchPosition.deflectionValue = e->throwPosition.deflectionValue = 1;
			e->spin = 0;
		}
	}
	
	do
	{
		c = peekChar();                 // gibts noch mehr?
		switch( c )
		{       int i;
			float f;
			
			case 'i':
				getChar();
				e->initialVelocity = -getSignedFloat();
				if( parameterError )
					return NULL;
					break;
			case 'v': // was 'B'which is not possible here. remember vincent!
			{
				
				
				char c;
				
				getChar();
				e->bounce = true;
				
				c = getChar();
				if( c == 'h')
				{
					c = getChar();
					if( c == 'l')
						e->bounceIndex = bounceHyperLift;
					else if( c == 'f')
						e->bounceIndex = bounceHyperForce;
					else
						parameterError = true;
				}else
				{
					if( c == 'l')
						e->bounceIndex = bounceLift;
					else if( c == 'f')
						e->bounceIndex = bounceForce;
					else
						e->bounceIndex = bounceLift;	// default bounce style
					break;//parameterError = true;
				}
				
				
				if( parameterError )
					return NULL;
				break;
			}
			case 'G':
				c = getChar();
				e->gravity = getSignedFloat( );
				if( parameterError )
					return NULL;
					break;
			case relativePassCode:
			{
				int o;
				
				getChar();
				o = getSignedInt();
				if( parameterError )
					return NULL;
				e->relativeJugglerOffset = o;
				
				break;
			}
			case 's':
			{
				styleEl *s;
				getChar();
				s = readStyle( );
				e->throwPosition = s->v;
				e->catchPosition = s->n;
				e->rel.t = true;
				e->rel.c = true;
				break;
			}
			case 'f':
				//                            case throwCode:                                     // von wo werfen wir?
				getChar();
				e->throwPosition = getDPoint( &defVPosition );
				e->rel.t = true;
				break;
				
			case 't':
				//                        case catchCode:                                 // Nach wo werfen?
				getChar();
				e->catchPosition.p.rotation = NOWHERE;
				e->catchPosition = getDPoint( &defNPosition );
				e->rel.c = true;
				break;
				
			case 'j':                                       // Nach wo werfen?
				getChar();
				e->orientation = parsePoint();
				if( parameterError )
					return NULL;
					break;
				
			case 'y':
				getChar();
				e->rotation =       parsePoint();
				if( parameterError )
					return NULL;
					break;
#if 0				
			case 'D':
				getChar();
				e->dummieFloat = getSignedFloat();
				if( parameterError )
					return NULL;
					break;
#endif
				
			case  spinCode:                         // number of spins
				getChar();
				if( peekChar() == spinFaktorCode )      // relative spin
				{       getChar();
					f = getSignedFloat();
					if( parameterError )
						return NULL;
					else
						e->spinFaktor = f;
					
				}else
				{       f = getSignedFloat();
					if( parameterError )
						return NULL;                            // Keine Zahl!
					e->spin = f;
				}
					break;
			case usePassCorrectionCode:
				getChar();
				e->usePassCorrection = false;
				break;
			case angleCode:
				getChar();
				e->throwAngle = getSignedFloat();
				break;
			case declinationCode:
				getChar();
				e->throwDeclination = getSignedFloat();
				e->throwDeclination *= -1.f;
				break;
			case passCode:                                  // Passen
				getChar();
#ifdef ChristophesPassingTagForMoreThan2People
				if( christophesPassingTagForMoreThan2People && christophesPassingTagForMoreThan2PeopleActivated == true)
				{
					
					i = getDigLet();                                // Wer denn?
					if( parameterError )
						return NULL;                            // Keine Zahl!
					e->to = i-1;
					boyceC = max( boyceC, i);       // hchste Zahl speichern, fr spteren Check
				}else
#endif
				{                               if( who > 1 )
					return NULL;
					e->to = (who==0?1:0);           // Nur Zwei, jeder zum anderen
				}
					break;
				
			case  boyceCode:                                // Vorschlag Boyce
				getChar();                                      // Auslesen
				i = getDigLet();                                // Wer denn? nnn
				if( parameterError )
					return NULL;                            // Keine Zahl!
					e->to = i-1;
				boyceC = max( boyceC, i);       // hchste Zahl speichern, fr spteren Check
				break;
				
			case crossCode:                                 // cross werfen
				getChar();
				toggle(cross);//cross = true;
				break;
				/*                      case cascadeCode:                               // cascade werfen
				getChar();
				if( !(int)e->ss%2 )
				cross = true;
				break;
			case fountainCode:                                      // fountain werfen
				getChar();
				if( (int)e->ss%2 )
					cross = true;
					break;
				*/
			case changeCode:                                // Andere Flugdauer
				getChar();
				e->change = getFloat();
				break;
				// ----------------- move! -----------------------
				// nicht bewegen. Bleibt zum Spielen auch hier, genau wie die Option v.
				// Die Eingaben sind an den angegebenen Stellen auch gltig und berschreiben die hier gemachten
				//sync
			case delayCode:                         // Verzgern
				c = getChar();
				if( c == noDelay )
					e->delay = RES_TO_UNDEFINED;
				else
				{       e->delay = getSignedFloat();
					if(parameterError)
						return NULL;
				}
					break;
#if 0
			case switchCode:                                // change forever
			case switch2Code:                               // juggle saver switch code
				getChar();
				e->sameHand = switchAll;
				break;
#endif
			case handCode:                                  // change this time
				getChar();
				e->sameHand = switchThis;
				break;
			case dwellCode:
				getChar();
				e->dwell = getFloat();
				if( e->dwell <= 0 || e->dwell >= 1 )
				{       parameterError = true;
					return NULL;
				}
					break;
			case beepCode:
				getChar();
				e->beep = true;
				break;
			case bounceCode:
				getChar();
				if( e->bounce == noBounce )
					e->bounce = stdBounce;
				else
					e->bounce = forceBounce;
				break;
				/*                      case forceBounceCode:
				getChar();
				e->bounce = forceBounce;
				break;
			*/                      
			case hurriedCode:
				getChar();
				e->isMhnHurried = true;
				if( mhnHurriedMode == true )
				{       hurryCorrection = true;
				}else
				{       e->hurried = true;
					if( jpPrefs.autoHurriedMode )
					{       hurriedValue = AutoHurriedValue;
						hurriedMode = true;
					}
					
					switchHandsCount ++;
				}
					
					break;
			case '?':
				// christopes hurried notation
				getChar();
				hurryCorrection = true;
				break;
				
			case 'R':
				getChar();
				e->roll = true;
				e->throwPosition.p.y = -1;
				e->catchPosition.p.y = -1;
				e->rel.t = e->rel.c = true;
				break;
				/*
				 case 'L':
					 getChar();
					 e->dummy = 3;
					 break;
				 case 'q':
					 getChar();
					 e->invisible = true;
					 hasIvisibilityTag = true;
					 break;
					 */
				
			case 'z':
				getChar();
				e->h2 = true;
				break;
			default:
				moreParameters = false;
				break;
		}
	}while( parameterError == false && moreParameters );
	
	if( parameterError == true )                    // Fehler beim Einlesen der Parameter
		return NULL;
	// (xx war hinter dem folgenden vergleich)
	siteswapCount += e->ss;                                 // zur Gesamtsumme addieren
	if( hurryCorrection && e->ss > 0)
		siteswapCount--;
	
	// unify interpretation of hurried throws cdew and mhn* patterns.
	// xxxxx 31.5.16 warum war das ausgeschaltet?
	// fehler, wenn in short hold eine bewegung ausgeführt werden soll:
	// flip, position der hand etc.
	// nb. eine Leere Hand kann nicht flippen.
	// ggf neuer Code für short hold: nicht zählen, aber mit kurzer Zeit eintragen
	if(true &&  e->isMhnHurried )
	{       // short holds can't be hurried by nature:
			// a short hold implies an action of the same hand before it happens.
			// so the hand is busy, the hold can´t happen at that moment.
			// they will be transformed to absorbs
			// not true for passes
		if( e->ss == 1 && cross == true && e->who == e->to )
		{
			e->ss = SS_EMPTY;
			throwCount--;
			return e;

		}else if( e->ss < 1 && cross == false )
		{
			e->ss = SS_EMPTY;
			throwCount--;
			return e;
			
		}
		// empty hands can't be hurried and
		// will be transformed to absorbs
		if( e->ss == 0 )
		{   e->ss = SS_EMPTY;
			throwCount--;
			return e;
		}
		
	}
	
	ssDefined = true;
	
	// beatmap notation        
	if( beatmapMode == true)
	{
		if( e->who != e->to) // passes are straight by default
			toggle( cross);
		
		e->cross = cross;
		
		/*
		 if( e->ss == 1 )	// remove dwell hold movement
		 e->ss = SS_EMPTY;
		 */
        emptyBeats = 0;
        hurriedMode = true;
        hurriedValue = 1 * .66f;
        mhnHurriedMode = true;
	}else
	{
        e->cross = ((int)e->ss % 2?true:false );
        e->cross ^= cross;                                              // Soll andersrum sein!
		if( cross && tableJugglingMode )
			e->throwPosition.p.SIDE *= -1;
        if( e->ss == 1 && cross == true)
			oneXCount ++;                                           // 1x zhlen
	}
	
	ssMaxVal = max(ssMaxVal, (float)ceil(e->ss));  // store lowest / highest throw value
	if( ssMinVal == -1 )                                    // draw all niveaus in between, if activated
		ssMinVal =  e->ss;
	else
		ssMinVal = min(ssMinVal, e->ss);
	return e;
}


// Alle Schleifen einer Ebene einlesen und mit bergeordneter Ebene verknpfen
// NB: Aus Siteswap-Folgen ohne Klammerung mache ich auch eine Schleife!
sseLoop *parseLoops( sseLoop *upperLoop )
{
	sseLoop *firstLoop, *theLoop, *oldLoop, *l;
	int c;
	sse *e;
	float sCnt;
	int tCnt, mCnt;
	float duration;
	int taktCnt = 0;
	
	sCnt = tCnt = mCnt = 0; // Hier wurde noch nix geworfen
	taktCnt = 0;
	
	duration = 0.f; // Keine Zeit...
	
	firstLoop = oldLoop = NULL;
	
	do                                      // Alles aus meiner Ebene
	{
		if( (theLoop = getFreeLoop()) == NULL ) // Nchste Schleife holen
			return NULL;
		
		if( oldLoop == NULL )                                           // Erstes Mal
			firstLoop = oldLoop = theLoop;
		else
		{       oldLoop->nextLoop = theLoop;                    // Schleifen verknpfen
			oldLoop = theLoop;
		}
		
		
		if( peekChar( ) == localMetaCode )                      // lokale Parameter fr die aktuelle Schleife einlesen
		{       if(  parseLocalParameters( theLoop) == false )
			return NULL;
		}
		if( peekChar()  == loopStart )                          // Unterschleifen einlesen
		{       getChar();
			
			if( (l = parseLoops( theLoop )) == NULL )
				return NULL;
			
			theLoop->inTheLoop.ssP.loop = l;
			theLoop->inTheLoop.typ = loop;
			
			if( getChar() != loopEnd )
				return NULL;
			
			if( (c = getInt() ) == 0 )
#if 0
				return NULL; 
#else
			// FOREVER is diabled, as it conflicts with runarounds
			c = FOREVER;
#endif
			
			
			if( parameterError )
				return NULL;
			
			theLoop->count = c;                                             // Anzahl der Wiederholungen
			theLoop->duration *= (c==FOREVER?1:c);  // Gesamtzeit
			duration += theLoop->duration;
			
			// Alle relevanten Werte mit Schleifenzahl multiplizieren
			tCnt += throwCount * max( 1 , c );
			sCnt += siteswapCount * max( 1, c );
			mCnt += momentCount * max( 1, c );
			taktCnt += taktCnt * max( 1,c);
			// Es mu folgen:
			// File-Ende,
			// SchleifenTrenner,
			// SchleifenAnfang,
			// SchleifenEnde
			// MetaParameter
			// Muster
			if( ( c=peekChar()) !='\0' && c != loopStart && c != loopEnd && c != localMetaCode &&
				c != simStart && c != passStart && c!= muxStart && !isdigit( c ) )
			{       if( c == loopPar )
				getChar();
				else
					return NULL;
			}
		}
		else                                                    // Einzel-Ereignis in Schleife packen
		{
			if( (e = parseEvents()) == NULL )
				return NULL;
			theLoop->inTheLoop.ssP.event = e;
			theLoop->inTheLoop.typ = event;
			theLoop->count = 1;
			duration += theLoop->duration = eDuration;
			
			tCnt += throwCount;
			sCnt += siteswapCount;
			mCnt += momentCount;
			taktCnt += taktCnt;
			
			// Es mu File-Ende oder SchleifenEnde oder SchleifenAnfang oder metaParameter folgen
			if( ( c=peekChar()) !='\0' && c != loopStart && c != loopEnd && c != localMetaCode)
				return NULL;
		}
	}while( peekChar() !='\0' && peekChar() != loopEnd);    // Solange noch was kommt und zu unserer Ebene gehrt
	
	theLoop->outTheLoop = upperLoop;                // Verknpfung mit bergeordneter Schleife herstellen
	upperLoop->duration = duration;                 // So lange dauert alles, was wir hier getan haben
	
	
	throwCount = tCnt;              // Sitesap-Werte aus meiner Ebene
	siteswapCount = sCnt;
	momentCount = mCnt;
	taktCnt = taktCnt;
	
	return firstLoop;
}



dPunkt getDPoint( dPunkt *in )
{
	dPunkt dp = *in;
	float t = 1;
	
	dp.deflectionMode = HandDeflectionMultiply;

#if 01
	if( peekChar() == hMoveCode ){   // Deflection ist absolute
		getChar();
		dp.deflectionMode = HandDeflectionMultiply;
	} 
#endif
	
	if(	peekChar() == hMoveCode2 ){   // Deflection ist absolute
	   getChar();
		dp.deflectionMode = HandDeflectionOverwrite;
	}
	
	dp.deflectionValue = 1.f;
	if( peekChar() != pointStart ){  // deflection
		t = getSignedFloat();
		if( !parameterError ){
			dp.deflectionValue = t;
		}
		else
			return dp;
	}
	
// point definition starts here
// empty field is ok, defaults are used
	if( peekChar() == pointStart ){
		getChar();
		

		// position coordinates, relative
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.x = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.y = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.z = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		
// object rotation and elevation
// object spin factor
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.rotation = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.declination = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.relRot = getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
//get out parameter for hand movement
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.outx = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.outy = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.outz = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		
//get in parameter for hand movement
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.inx = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.iny = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();
		
		if(peekChar() == pointEnd )				// closing tag = break
			goto bail;		
		if( peekChar() != pointNext ){			// a seperator?
			dp.p.inz = -getSignedFloat();			// no => read value
			if( parameterError )
				return dp ;
		}
		if( peekChar() == pointNext )
			getChar();

		
bail:   getChar();                                      // trenner berspringen
	}
	
	return dp;
}


// Welches Muster ist gemeint?
// Aus Index des Menu-Eintrags ableiten
int getDefaultStyleIndex( void )
{
	return defStyleIndex;
}
/*
 Pattern-definition einlesen
 a. Namen angeben -> aus defaultFile holen
 b. mit special Char beginnen und lesen
 bis Zeichen EOPATTER wie in StyleScanner definiert
 */
styleEl *readStyle( void )
{
	char patName[256];
	style   *dHPattern;
	style *aS, *t;
	styleEl *pE;
	
	if( peekChar() == hStyleExpl )
	{       getChar();
		// letzten Eintrag suchen
		aS = &firstStyle;
		while( aS->next )
			aS = aS->next;
		
		if( readMoves( aS) == false )
			return NULL;    // Fehler
		if( peekChar() == hStyleExpl )
		{       getChar();
			// als aktuelles Pattern setzen
			pE = aS->first;
		}
		
		// Platz machen fr nchste Struktur
		if( ( t = (style *)malloc(sizeof( style)))==NULL        ) // Speicher anfragen
		{       return false;
		}else
		{       aS->next = t;
			t->next = NULL;
			strcpy(t->name, "");
			t->count = 0;
			t->first = NULL;
		}
		
	}else
	{
		// Default-File verwenden
		readWord(patName, 255);
		dHPattern = getStyle( patName, &stdStyle);      // Muster einlesen, default verwenden falls nicht auffindbar
		pE = dHPattern->first;
	}
	
	if( peekChar() == hStyleNum )   // Mit welchem Wurf beginnen?
	{       int n;
		
		getChar();
		n = getInt()-1;                         // Index aus Nummer berechnen
		if( n< 0 || parameterError )
			return false;
		while( n )                              //      Sooft berspringen
		{       n--;
			pE = pE->next;
		}
	}
	return pE;
}

int getStyleLength( styleEl *f )
{       styleEl *t;
	int c;
	
	c = 0;
	
	if( (t = f) == NULL )
		return 0;
	
	do{
		c++;
		t=t->next;
	}while(t != f);
	
	
	return c;
}

// Muster initialisieren == Speicher freigeben
void initStyles( void )
{
	releaseStyles( &firstStyle );
	firstStyle.next = NULL;
	strcpy(firstStyle.name, "");
	firstStyle.count = 0;
	firstStyle.first = NULL;
}
// Aus Lnge aller default-styles kgV berechnen
// Wird Lnge des Gesamt-Musters
int getGlobalStyleLength( void )
{
	int i, l;
	Juggler *j;
	
	
	l = max( 1, getStyleLength( defStyle));
	
	for( i = 0; i < JUGGLER_MAX; i++ )
	{
		j = getJuggler( i );
		l = kgv( l, getStyleLength( j->sE));
	}
	return l;
	
}
// Die andere Hand auswhlen
// bei ungltiger Eingabe: die rechte Hand
int getAlternateHand( int hand )
{
	if( hand == noHand )
		return noHand;
	return hand == leftHand?rightHand:leftHand;
}



wwwBoolean parseGlobalObjectBounceFrom( void )
{
	objectBounceFrom = getSignedFloat();
	
	return parameterError == 0;
}
// Eine Liste mit spins angeben
wwwBoolean parseGlobalObjectSpins( void )
{   int start = 2;
	float v;
	double dummy;
	
	for(;;)
	{
		spinElement *s;
		
		
		// get value
		v = getSignedFloat();
		if( parameterError )
			return false;
		
		
		// is a starting code defined?
		if( peekChar() == startSpinCode )
		{       getChar();
			
			//no negative siteswaps are allowed
			if( v < 0 || modf(v, &dummy) )
				return false;
			
			// ok, this is the starting number
			start = v;
			// get the spin for this number
			v = getSignedFloat();
			if( parameterError )
				return false;
		}else
			start++;        // starting code is next number otherwise
		
		s = getSpin( start );
		s->n = v;
		start++;
		
		// more to come
		if( peekChar() != ',' )
			break;
		getChar();
		
		// I dont think anyone will run in this error
		if( start == SPIN_MAX )
			return false;
	}
	
	return true;
}
wwwBoolean parseGlobalObjectPassColor( void )
{
	punkt p;
	sysColor c;
	p = parsePoint( );
	p = p * (1./objectColorRange);
	c = sysColor( p );
	if( ! parameterError )
		passColor = c;
	
	return parameterError == 0;
	
}

// list of objects with reversed orientation on pattern start
wwwBoolean parseGlobalObjectInHalfSpin( void )
{
	int nr;
	_object *o;
	char c;
	
	for(;;)
	{       // Wer bin ich
		nr = getInt() - 1;
		if( parameterError )
			return false;
		if(nr < 0 || nr >= BALL_MAX )
			return false;
		o = getObject( nr );
		toggle(o->doesHalfSpinThrow) ;
		toggle(o->halfSpinFlag) ;
		o->doesHalfSpinThrow = true ;
		o->halfSpinFlag = true ;
		c = peekChar();
		if( c != ',' )
			break;
		c = getChar();
	}
	
	return true;
}


wwwBoolean parseGlobalObjectSelfColor( void )
{
	punkt p;
	sysColor c;
	p = parsePoint( );
	p = p * (1./objectColorRange);
	c = sysColor( p );
	if( ! parameterError )
		selfColor = c;
	
	return parameterError == 0;	
}

wwwBoolean parseGlobalObjectKind( void )
{
	int i = 0;
  		int v1,v2;
		int kind;
		
		
        for(;;)
        {
			if( i == BALL_MAX )
				return false;
			v1 = getInt();
			if( parameterError )
				return false;
			
			if( peekChar() == hObjectNum ) // individually defined
			{      
				getChar();
				
				i = v1-1;
				if( i < 0 || i == BALL_MAX )
					return false;
				
				v2 = getInt();
				
				if( parameterError || v2 >= endOfObjects )
					return false;
				
				if( v2 == random_object )
					kind = selectRandomObject();
				else
					kind = v2;
				
				getObject( i )->kind = kind;
			}else
			{
				int c;
				for( c = i;c<BALL_MAX; c++ )
				{   
					if( v1 == random_object )
						kind = selectRandomObject();
					else
						kind = v1;
					
					getObject( c )->kind = kind;
				}
			}
			// more to come
			if( peekChar() != ',' )
				break;
			getChar();
			
        }
		
        return true;
}
wwwBoolean parseGlobalMaximumDwell( void )
{
	float v;
	
	v = getSignedFloat();
	if( parameterError || v == 0)
		return false;
	
	jpPrefs.maxDwell = v;
	return true;
}

wwwBoolean parseGlobalJugglerColor(void )
{
	int nr;
	Juggler *j;
	punkt p;
	
	nr = -1;
	
	for(;;)
	{
		if( peekChar() != pointStart ){
			nr = getInt() - 1;              // Wer ist es?
			if(parameterError || nr < 0 || nr >= JUGGLER_MAX )
				return false;
#ifdef syntaxStrict
			if( peekChar() != hObjectNum )
				return false;
			getChar();
#else
			if( peekChar() == hObjectNum )
				getChar();
#endif
		}
		
		p = parsePoint() * (1./objectColorRange);
		if (parameterError )
			return false;   // Fehler
		
		
		if( nr == -1 ){
			for( nr = 0; nr < JUGGLER_MAX; nr++){
				j = getJuggler( nr );
				j->endColor = sysColor( p );
				j->invisible = ( p.x < .0 || p.y < .0 || p.z < .0 );
				j->colorDefined = true;
			}
		}else{
			j = getJuggler( nr );
			j->endColor = sysColor( p );
			j->invisible = ( p.x < .0 || p.y < .0 || p.z < .0 );
			j->colorDefined = true;
		}
		
		if( peekChar() != ',' )
			break;
		getChar();
	}
	
	return parameterError == 0;
	
}





wwwBoolean parseGlobalObjectColor(void )
{
	int nr;
	_object *o;
	punkt p;
	
	nr = -1;
	
	for( ;;){
		
		if(peekChar() != pointStart ){
			
			nr = getInt() - 1;              // Wer ist es?
			if(parameterError || nr < 0 || nr >= BALL_MAX )
				return false;
#ifdef syntaxStrict
			if( peekChar() != hObjectNum )
				return false;
			getChar();
#else
			if( peekChar() == hObjectNum )
				getChar();
#endif			
		}
		
		p = parsePoint() * (1./objectColorRange);
		if (parameterError )
			return false;   // Fehler
		
		if( nr == -1 ){
			for( nr = 0; nr < BALL_MAX; nr++){
				o = getObject( nr );
				o->endColor = sysColor( p );
				o->invisible = ( p.x < .0 || p.y < .0 || p.z < .0 );
				o->colorDefined = true;
			}
		}else{
			o = getObject( nr );
			o->endColor = sysColor( p );
			o->invisible = ( p.x < .0 || p.y < .0 || p.z < .0 );
			o->colorDefined = true;
		}
		
		
		if( peekChar() != ',' )
			break;
		getChar();
	}
	return parameterError == 0;
	
}



wwwBoolean parseGlobalObjectSize( void )
{
	int i;
	float v;
	char c;
	
	for(;;){
		
		i=-1;
		// get value.
		// can be jugglers index (int) or object size (float)
		v = getFloat();   
		abortOn( parameterError );		
		c = peekChar();
		if(c == hObjectNum){
			//seperator => value was object index 
			getChar();		
			// read seperator
			i = v-1;	
			// get object index
			abortOn( i < 0 || i > BALL_MAX);
			
			// next value must be object size
			v = getFloat();
			abortOn( parameterError );		
		}
		
		// save default object size
		if( i == -1 ){
			// .. for all objects
			int c;
			for( c = 0; c < BALL_MAX; c++)
				getObject(c)->size = v;
		}else{
			// .. for the named object
			getObject( i )->size = v;
		}
		
		// more definitions
		breakOn(peekChar() != ',' );
		getChar();
	}
	
	return parameterError == false;
}



wwwBoolean parseCameraPos ( void )
{
	punkt p;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	p = parsePoint();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraSetPos;
	actCameraMovementPointer->p1 = p;
	
	return true;
}

wwwBoolean parseCameraPoi ( void )
{
	punkt p;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
	{
		_myPrintf("no getCameraMovementPointer found\n");
		return false;
	}
	p = parsePoint();
	if( parameterError )
	{       _myPrintf("parsePoint error\n");
		return false;
	}
	actCameraMovementPointer->mode = cameraSetPoi;
	actCameraMovementPointer->p1 = p;
	
	return true;
}
wwwBoolean parseCameraUp ( void )
{
	punkt p;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	p = parsePoint();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraSetUp;
	actCameraMovementPointer->p1 = p;
	
	return true;
}
wwwBoolean parseCameraPan ( void )
{
	float a, d;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	
	// angle
	a = getSignedFloat();
	if( parameterError )
		return parameterError == 0;
	// duration
	d = getFloat();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraPan;
	actCameraMovementPointer->angle = a;
	actCameraMovementPointer->duration = d;
	
	return true;
}

wwwBoolean parseCameraTilt ( void )
{
	float a, d;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	
	// angle
	a = getSignedFloat();
	if( parameterError )
		return false;
	// duration
	d = getFloat();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraTilt;
	actCameraMovementPointer->angle = a;
	actCameraMovementPointer->duration = d;
	
	return true;
}
wwwBoolean parseCameraReset( void )
{       cameraReset();
	return true;
}
wwwBoolean parseCameraPoiMovement( void )
{
	punkt p1, p2;
	float f;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	p1 = parsePoint();
	if( parameterError )
		return false;
	
	p2 = parsePoint();
	if( parameterError )
		return false;
	f = getFloat();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraMovePosAbsolute;
	actCameraMovementPointer->p1 = p1;
	actCameraMovementPointer->p2 = p2;
	actCameraMovementPointer->duration = f;
	
	return true;
}

wwwBoolean parseCameraMovement( void )
{
	punkt p1, p2;
	float f;
	_cameraMovementPointer *actCameraMovementPointer;
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	p1 = parsePoint();
	if( parameterError )
		return false;
	
	p2 = parsePoint();
	if( parameterError )
		return false;
	f = getFloat();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraMoveAbsolute;
	actCameraMovementPointer->p1 = p1;
	actCameraMovementPointer->p2 = p2;
	actCameraMovementPointer->duration = f;
	
	return true;
}
wwwBoolean parseCameraPosMovement( void )
{
	punkt p1, p2;
	float f;
	_cameraMovementPointer *actCameraMovementPointer;
	
	
	resetPosSpline();
	
	if( (actCameraMovementPointer = getCameraMovementPointer()) == NULL)
		return false;
	p1 = parsePoint();
	if( parameterError )
		return false;
	
	p2 = parsePoint();
	if( parameterError )
		return false;
	f = getFloat();
	if( parameterError )
		return false;
	actCameraMovementPointer->mode = cameraMovePosAbsolute;
	actCameraMovementPointer->p1 = p1;
	actCameraMovementPointer->p2 = p2;
	actCameraMovementPointer->duration = f;
	
	return true;
}

wwwBoolean parseCameraMovementFactor( void )
{
	float f;
	f = getFloat( );
	if( parameterError )
		return false;
	setMovementFactor( f );
	
	return true;
}
wwwBoolean parseCameraPosMoveThrough( void )
{
	punkt p;
	char c;
	
	resetPosSpline();
	
	setAutoMove( autoMoveThroughPoints );
	for(;;)
	{
		p = parsePoint();
		if( parameterError )
			return false;
		
		if( addPosSplinePoint( p ) == false )
			return false;
		
		if( (c = peekChar()) != nextSplinePoint )
		{       break;
		}
		getChar();
	}
#ifdef CHECK_END_OF_SPLINE_POINTS		
	if( (c = peekChar()) != endOfSplinePoints )
	{       return false;
	}else
		getChar();
#endif
	
	return true;
}
wwwBoolean parseCameraPoiMoveThrough( void )
{
	punkt p;
	char c;
	
	resetPoiSpline();
	
	setAutoMove( autoMoveThroughPoints );
	
	for(;;)
	{
		p = parsePoint();
		if( parameterError )
			return false;
		
		if( addPoiSplinePoint( p ) == false )
			return false;
		
		if( (c = peekChar()) != nextSplinePoint )
		{       break;
		}
		getChar();
	}
#ifdef CHECK_END_OF_SPLINE_POINTS		
	if( (c = peekChar()) != endOfSplinePoints )
	{       return false;
	}else
		getChar();
#endif
	
	return true;
}
wwwBoolean parseCameraUpMoveThrough( void )
{
	punkt p;
	char c;
	
	resetUpSpline();
	
	setAutoMove( autoMoveThroughPoints );
	
	for(;;)
	{
		p = parsePoint();
		if( parameterError )
			return false;
		
		if( addUpSplinePoint( p ) == false )
			return false;
		
		if( (c = peekChar()) != nextSplinePoint )
		{       break;
		}
		getChar();
	}
#ifdef CHECK_END_OF_SPLINE_POINTS		
	if( (c = peekChar()) != endOfSplinePoints )
	{       return false;
	}else
		getChar();
#endif
	
	return true;
}
wwwBoolean parseCameraMoveAlong( void )
{
	punkt p;
	char c;
	
	resetPosSpline();
	resetPoiSpline();
	
	setAutoMove( autoMoveAlongSpline );
	
	for(;;)
	{
		p = parsePoint();
		if( parameterError )
			return false;
		
		if( addPosSplinePoint( p ) == false )
			return false;
		
		if( (c = peekChar()) != nextSplinePoint )
		{       break;
		}
		getChar();
	}
#ifdef CHECK_END_OF_SPLINE_POINTS		
	if( (c = peekChar()) != endOfSplinePoints )
	{       return false;
	}else
		getChar();
#endif
	return false;
}


wwwBoolean parseCameraMode( void )
{
	int i;
	
	i = getInt();
	if( parameterError )
		return false;
	
	setCameraMode( (wwwBoolean)i);
	
	return true  ;
}
_cameraMovementPointer *getCameraMovementPointer( void )
{       _cameraMovementPointer *theCmp;
	
	// try to get a new structure
	// first
	if( actSseLoop->cameraMovementPointer == NULL )
	{       actSseLoop->cameraMovementPointer = getFreeCmp();
		theCmp = actSseLoop->cameraMovementPointer;
		if( theCmp == NULL)
		{       return NULL;
		}
	}
	// follow ups
	else
	{       theCmp = (_cameraMovementPointer*)actSseLoop->cameraMovementPointer;
		while( theCmp->next )
			theCmp = theCmp->next;
		if( (theCmp->next = getFreeCmp()) == NULL)
		{       return NULL;
		}
		theCmp = theCmp->next;
	}
	// ok, return pointer
	return theCmp;
}



# pragma mark ---------- juggler movement
// juggler movement
// 2 versions
// old + spline version
wwwBoolean parseLocalJugglerMovement( void )
{
	lmp *newLmp, *actLmp;
	actLmp = actSseLoop->theLmp;
	// Speicher fr Schleife
	if( (newLmp = getFreeLmp()) == NULL )
		return false;
	
	if( actLmp == NULL )
	{       actSseLoop->theLmp = newLmp;                                    // Kette verankern
		
	}else
	{       while( actLmp->nextLmp != NULL )
		actLmp = actLmp->nextLmp;                                       // Ende suchen
		actLmp->nextLmp = newLmp;                                                       // anhngen
	}
	//      actLmp->nextLmp = newLmp;                                                       // anhngen
	if(  _parseLocalJugglerMovement( newLmp ) == false )    // Werte einfllen
		return false;
	return true;
}
wwwBoolean parseInfo( void )
{
	getString(actSseLoop->info, 255);
	if( actSseLoop->info[0] == '#' )
        actSseLoop->info[0] = '\0';
	return true;
}


wwwBoolean parseSound( void )
{
	char w[DIR_SIZE];
/*	getString(actSseLoop->music, 255);
	if( actSseLoop->info[0] == '#' )
        actSseLoop->info[0] = '\0';
 */
	// get the file name (MAY NOT INCLUDE SPACES!)
	sprintf(w, "%s", getWord());
	//initBackgroundSound( w );
	

	
	return true;
}
wwwBoolean parsePassCorrectionMode( void )
{
	int i;
	
	i = getInt();
	if( i == 0 )
		actSseLoop->passCorrectionMode = dontCorrectPassValues;
	else
		actSseLoop->passCorrectionMode = correctPassValues;
	
	return true;
}


// dispatches commands
// return: true <=> command found
// error is in parameterError
wwwBoolean parseNewCommands( sseLoop *f)
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( f )
#endif
	char *p;
	commandAtom *cmd = commandList;
	wwwBoolean ret = true;
	
	p =getWord();
	ret = false;
	while( cmd->name )
	{
		if( strcmp(cmd->name, p ) == 0 )
		{
			ret = cmd->function();
			
			if( peekChar() == ';' )
				getChar();
			break;
		}
		cmd++;
	}
	if(0 && ret == false )
	{       printf(" unknown command %s\n", p);
		exit( 0 );
	}
	return ret;
}
// interprete parameters:
wwwBoolean parseLocalParameters( sseLoop *f)
{
	wwwBoolean ret = true;
	sseLoop *oldSseLoop;
	
	// recursive function. save actual state;
	oldSseLoop = actSseLoop;
	
	actSseLoop = f;
	actSseLoop->theLsp = NULL;
	actSseLoop->theLmp = NULL;
	
	while( peekChar() == localMetaCode)     // Ich bin zustndig
	{
		getChar();      // also Zeichen lesen
		if( parseNewCommands( f ) && !parameterError )
			continue;
		else
			return false;
	}
	
	actSseLoop = oldSseLoop;
	return ret;
}


wwwBoolean _parseLocalJugglerMovement( lmp *theLmp )
{
	int nr;
	punkt p, t, z, d;
	float b;
	// Erweiterte Bewegung
	if( peekChar() == pathCode )
	{       getChar();
		theLmp->modPath = getSignedFloat();
		if( parameterError )
			return false;
	}
	
	// Wer bin ich
	nr = getInt() - 1;
	if( parameterError )
		return false;
	if(nr < 0 || nr >= JUGGLER_MAX )
		return false;
	else
		theLmp->who = nr;
	
#ifdef syntaxStrict
	if( peekChar() != hObjectNum )
		return false;
	getChar();
#else
	if( peekChar() == hObjectNum )
		getChar();
#endif	
	
	
	// Wann kommt er an
	theLmp->duration = getSignedFloat();
	if( parameterError )
		return false;
	// pirouette
	if( peekChar() == 'p' )
	{       getChar();
		theLmp->p = getSignedFloat();
		if( parameterError )
			return false;
	}
	
	// Ziel
	z = p = parsePoint();
	if( parameterError )
		return false;
	theLmp->pEnd = calcBodyCenter( p );
	// Blickrichtung
	d = p = parsePoint();
	if( parameterError)
		return false;
	
	p = d - z;                // Blickrichtung berechnen
	if( (b = p.betrag()) == 0 )                   // falscher Richtungsvektor
		return false;
	
	
	t = calcBodyCenter( p );
	theLmp->nEnd = t.normalised();               // Normalisieren
	theLmp->nEnd.y = 0;                                             // Wir stehen immer mit beiden Beinen auf dem Boden
	return true;
}


_jm     *getFreeJm( void )
{
	_jm *p;
	
	if( (p = (_jm*)malloc( sizeof( _jm ))) == NULL)
		return NULL;
	
	p->pos = p->posIn = p->posOut = p->poi = p->poiIn = p->poiOut = pN;
	p->t = -1.f;
	p->pirouettes = 0;
	p->next = 0;
	
	return p;
}

_jm *addJm( int nr )
{
	_jm *p, *n;
	
	if( (p = getFreeJm()) == NULL )
		return NULL;
	
	// first entry: create list
	if( NULL == firstJm[nr] )
		firstJm[nr] = p;
	
	// other entries: continue list
	else
	{
		n = firstJm[nr];
		
		while( n->next )
			n = n->next;
		n->next = p;
	}
	
	return p;
}

void freeAllJm( void )
{
	_jm *p, *n;
	int c;
	
	
	for( c = 0; c < JUGGLER_MAX; c++ )
	{
		p = firstJm[c];
		while( p )
		{
			n = p->next;
			free( p );
			p = n;
		}
		firstJm[c] = 0;
	}
}

// jongleure laufen
// spline version
// time is a positive value, read this as percentual time.
// LATER:
// if time is negative, calculate time by spread it evently between neighbour values.
// if neighbour values are negative, too,
// sort them by value (highest value first), then calculate percentual value
// same values describe same moments
// default for time is -1
// a special value is NOW (==-12345, defined in profile)
// wich saves the time in which the line is written in the text file
wwwBoolean parseSplineJugglerMovement( void )
{
	int nr;
	punkt pos,  poi, t;
	_jm *jm;
	_jm *jm2;
	
	Juggler::walkMode = true;
	// Wer bin ich
	nr = getInt() - 1;
	if( parameterError )
		return false;
	if(nr < 0 || nr >= JUGGLER_MAX )
		return false;
	
	
	if( (jm = addJm(nr)) == NULL )
		return false;
	
	// when does he arive
	// NB: relative time (in relation to total time)
	jm->t = getSignedFloat();
	if( parameterError )
		return false;
	
	// fix values to 0 ...100 (were talking about percent here
	while( jm->t < 0. )
		jm->t += 100.f;
	while( jm->t >= 100.f )
		jm->t -= 100.f;
	
	// Ziel
	pos = parsePoint();
	if( parameterError )
		return false;
	
	// point of interest
	poi = parsePoint();
	if( parameterError)
		return false;
	
	t = pos - poi;				// calc viewing direction
	t.y = 0;					// always upside down
	if( t.betrag( ) == 0 )		// may not be zero
		return false;
	
	jm->pos = pos;
	jm->poi = poi;
	
	jm->posIn = jm->posOut = pN;
	jm->poiIn = jm->poiOut = pN;
	
	// point of interest vektors
	// l == look
	if( peekChar() == 'l' ){
		getChar();
		if( peekChar() == 'i' ){
			punkt p;
			
			getChar();
			p = parsePoint();
			if( parameterError )
				return false;
			
			jm->poiIn = makeIncommingVector( poi, p );
		}
	}
	if( peekChar() == 'l' ){
		getChar();
		if( peekChar() == 'o' ){
			punkt p;
			
			getChar();
			p = parsePoint();
			if( parameterError )
				return false;
			jm->poiOut = makeOutgoingVector( poi, p );
		}
	}
	
	
	// movement vektors
	// no identifier
	// incoming vector
	if( peekChar() == 'i' ){
		punkt p;
		
		getChar();
		p = parsePoint();
		if( parameterError )
			return false;
		
		jm->posIn = makeIncommingVector( pos, p );
	}
	if( peekChar() == 'o' ){
		punkt p;
		
		getChar();
		p = parsePoint();
		if( parameterError )
			return false;
		jm->posOut = makeOutgoingVector( pos, p );
	}
	
	
	jm->pos   = calcBodyCenter( jm->pos );
	jm->poi   = calcBodyCenter( jm->poi );
	
	
	
	
	/*  heres a hack
        (Part 1. For part 2 see projectJugglerPath)
        to avoid problems with starting hands, jp saves all patterns wit double length
        this way its always the same hand that starts a pattern
        as we are using percentage to specify walking position, we do a little trick:
        the values are saved twice, one time for the first part of the pattern and another time
        for the 2nd part.
        We have to hide the 2nd half of the values when we print the path (see projectJugglerPath )
		*/
	
	if( 01 )
	{
		jm2 = addJm(nr );
		jm->t *= .5f;
		jm2->pos = jm->pos;
		jm2->posIn = jm->posIn;
		jm2->posOut = jm->posOut;
		jm2->poi = jm->poi;
		jm2->poiIn = jm->poiIn;
		jm2->poiOut = jm->poiOut;
		jm2->t = jm->t + 50;
		
	}
	
#if 0
	;
#elif 0
	{
		// sort _all_ entries by time
		_jm *p;
		int c;
		
		p = firstJm[nr];
		
		for( c = 0; p != NULL; p = p->next)
			c++;
		
		qsort( firstJm, c, sizeof( _jm ), (void *)jmCompare);
		
		
	}
	
#else
	{
		// sort _all_ entries by time
		_jm *p, *pNext, *pLast;
		wwwBoolean done;
		
		do{
			done = true;
			p = firstJm[nr];
			pLast = NULL;
			
			// still something to do
			while(p && p->next ){
				//printf("p = %lx, p->next = %lx\n", p, p->next);
				
				// swap order of entries
				if( p->t > p->next->t){
					done = false;
					
					pNext = p->next;
					
					// first entry
					if( NULL == pLast ){
						//printf("sorting in first place\n");
						p->next = p->next->next;
						pNext->next = p;
						firstJm[nr] = pNext;
						//printf("ok\n");
					}
					// somewhere in the chain
					else{
						//printf("sorting somewhere\n");
						p->next = p->next->next;
						pNext->next = p;
						pLast->next = pNext;
						//printf("ok\n");
					}
				}
				
				// set pointers to next entries
				pLast = p;
				p = p->next;
				//printf("next p is %lx\n", p);
			}
		}while( !done );
		
	}
#endif
	
	return true;
}


void setJugglerSplineMovement( void )
{
	int nr;
	_jm *jm;
	int n;
	
	for( nr = 0; nr < jugglerCount; nr++)
	{
		jm = firstJm[nr];
		n = 0;
		
		
		// percent to absolute values
		while( jm )
		{
			jm->t *= (patternTime * 2.f/100.f);
			jm = jm->next;
			n++;
		}
		
	}
	
}


int jmCompare( _jm *a, _jm *b )
{
	//printf("comparing %lx wwith %lx\n", a, b);
	
	return a->t - b->t;
}

punkt makeIncommingVector( punkt p, punkt i )
{
	return (p - i) * jpPrefs.walkFactor;
}

punkt makeOutgoingVector( punkt p, punkt i )
{
	return (i - p) * jpPrefs.walkFactor;
	
}

wwwBoolean calculateSizeOfPlainSiteswap(char *p, int *rj, int *re)
{
	char *t = p;
	wwwBoolean stopEntryCount = false;
	int j, e;
	
	j = 0;
	e = 0;
	
	if( (t = strchr( p, '<' ) ) != NULL )
		p = t;
	
	
	t = p;
	
	// count cdewEntries
	do
	{
		
		// skip whitespace
		while( isspace( *p ) )
			p++;
		
		// skip line comments
		if( *p == lineCommentCode )
		{
			while( *p && !(*p == '\n' || *p == '\r') )
				p++;
		}
		
		// skip block comments (enclosed in '"')
		if( *p == '"' )
		{
			do {
				p++;
			}while( *p && *p != '"' );
			
			if( *p )
				p++;
		}
		
		if( *p == '<' )
		{       j = 0;
			stopEntryCount = false;
		}
		
		// next juggler
		if( *p == '|' )
		{       j++;
			stopEntryCount = true;
		}
		
		// next entry
		if( !stopEntryCount )
		{
			// dont count passers number
			if( *p == boyceCode )
			{       p+=2;
				continue;
			}
			
			// count siteswap number
			if( isdigit( *p ) )
				e++;
		}
		p++;
		
	}while( *p );
	
	*rj = j+1;
	*re = e;
	
	return true;
}

wwwBoolean parseObjectColorRange( void )
{
    objectColorRange = getFloat();        
	
    return parameterError == 0;
}
wwwBoolean parseJugglerName( void )
{
	Juggler *j;
	
	for( ;;){
		bool b;
		
		
		int i = getInt()-1;
		
		if( peekChar() != ':')
			return false;
		
		getChar();
		
		char *n =  getWord();
		if( parameterError || i < 0 || i >= JUGGLER_MAX )
			return parameterError;
		
		j = getJuggler( i );
		if( j == NULL )
			return false;
		
		char c = n[strlen(n)-1];
		//		_myPrintf("n[strlen(n)-1] = %c\n", c);
		if( c == ','){
			n[strlen(n)-1] = '\0';
			b = true;
		}else{
			if( (b = peekChar() == ',') == true )
				getChar();
		}
		
		j->setName( n );
		
		if( !b )
			break;
		
		
	}
	
	if(peekChar() != ';')
		return false;
	getChar();
	
	return parameterError == 0;
}

wwwBoolean parseTableJuggling( void )
{
	tableJugglingMode = true;
	
	setTableJugglingMode( tableJugglingMode );
	
	return true;
}

void setTableJugglingMode( bool mode )
{
	if( mode ){
		int c;
		setSiteswapMode( sMode );
		positionMode = PositionTable;		
		for( c = 0;c<BALL_MAX; c++ )	
			getObject( c )->kind = balls;
	}
	
	setGravity(stdGravity);
	
}
wwwBoolean parseGravity( void )
{
	float g =  getFloat();// fix to * 1
	setGravity( g*g );
	
	return parameterError == 0;
}

wwwBoolean parseStdGravity( void )
{
	float g =  getFloat();// fix to * 1
	stdGravity = g*g;
	
	return parameterError == 0;
}

wwwBoolean parseGridSize( void )
{
	metersOnScreen = getInt();
	if( metersOnScreen <= 0 )
		return false;
	if(peekChar() == ',' )
	{
		getChar();
		gridX = metersOnScreen;
		gridY = getInt();
		grid2Parameters = true;
	}else
		grid2Parameters = false;
	
	
    return parameterError == 0;
}


wwwBoolean parseTPS( void )
{
	float bps = getFloat();
	setAnimationSpeed( bps);
	return parameterError == 0;
}

wwwBoolean parseTPS10( void )
{
	float bps10 = getFloat();
	setAnimationSpeed( bps10/10.f);
	return parameterError == 0;
}

void setAnimationSpeed( float tps )
{
	jpPrefs.animationSpeed = tps;
	setGravity( stdGravity * 4 / (tps*tps) ); 
	
}    


void setGravity( float g )
{    
  	jpPrefs.gravity =  g;
}


lp *getNewLpToLpChain( void )
{
	lp *theLp, *actLp;
	actLp = actSseLoop->theLp;
	
	// Speicher fr Schleife
	if( (theLp = getFreeLp()) == NULL )
		return NULL;
	
	if( actLp == NULL )
	{       actSseLoop->theLp = actLp = theLp;                                    // Kette verankern
	}else
	{       while( actLp->nextLp != NULL )                                // Ende suchen
		actLp = actLp->nextLp;
		actLp->nextLp = theLp;                                                       // anhngen
	}	
	
	return theLp;
}


wwwBoolean parseLocalObjectSize( void )
{
	lp *newLp;
	int i;
	float v;
	char c;
	
	for(;;){
		
		newLp = getNewLpToLpChain( );
		newLp->type = ObjectSizeType;
		
		i=-1;
		// get value.
		// can be jugglers index (int) or object size (float)
		v = getFloat();   
		abortOn( parameterError );		
		c = peekChar();
		if(c == hObjectNum){
			//seperator => value was object index 
			getChar();		
			// read seperator
			i = v-1;	
			// get jugglers index
			abortOn( i < 0 || i > BALL_MAX);
			newLp->nr = i;                                                               
			
			// next value must be object size
			v = getFloat();
			abortOn( parameterError );		
		}
		newLp->size = v;
			
		// transition time follows
		if( peekChar() == hTimeCode){
			getChar();
			newLp->transitionTime = getFloat();
			abortOn( parameterError );
		}
		
		// more definitions
		breakOn(peekChar() != ',' );
		getChar();
	}
	
	return parameterError == false;
}



wwwBoolean parseLocalJugglerObjects( void )
{
	lp *newLp;
	int i = 0;
	int v1,v2;
	
	for(;;){
		newLp = getNewLpToLpChain( );
		newLp->type = JugglerObjectType;
		
		abortOn( i == JUGGLER_MAX );
		v1 = getInt();
		abortOn( parameterError );
		
		if( peekChar() == hObjectNum ) { // individually defined
			getChar();
			
			i = --v1;
			v2 = getInt();
			
			abortOn( parameterError || v1 < 0 || v2 >= endOfObjects );
			
			newLp->nr = v1;
			newLp->objectKind = v2;
		}else{
			newLp->nr = -1;
			newLp->objectKind = v1;
		}
		
		breakOn( peekChar() != ',' );
		getChar();
	}
	
	return parameterError == 0;
}


wwwBoolean parseLocalJugglerColor( void )
{
	lp *newLp;
	int i;
	punkt p;
	
	for(;;){
		
		newLp = getNewLpToLpChain( );									// get memory for my loop
		newLp->type = JugglerColorType;
		
		i=-1;
		if( isdigit(peekChar()) ) {                                     
			i = getInt()-1;                                             // which index?
			abortOn( parameterError || i < 0 || i >= JUGGLER_MAX );

			newLp->nr = i;                                              // save

			abortOn( peekChar() != hObjectNum );						// syntax check
			getChar();
		}
		
		p = parsePoint();
		p = p * (1./objectColorRange);
		
		newLp->color = sysColor(p);

		if( peekChar() == hTimeCode){
			getChar();
			newLp->transitionTime = getFloat();
			abortOn( parameterError );
		}
		
		breakOn( peekChar() != ',' );
		getChar();
	}
	
	return parameterError == 0;
}

wwwBoolean parseLocalObjectColor( void )
{
	lp *newLp;
	int i;
	punkt p;
	
	for(;;){
		
		// Speicher fr Schleife
		newLp = getNewLpToLpChain( );
		newLp->type = ObjectColorType;
		
		i=-1; // all objects is default
		// only the named object
		if( isdigit(peekChar()) )                                                       // wer denn?
		{   
			i = getInt()-1;                                                                 // Index aus Nummer berechnen
			abortOn( parameterError || i < 0 || i >= BALL_MAX );
			newLp->nr = i;                                                                // Eintragen
			
			// syntax check
			abortOn( peekChar() != hObjectNum );
			getChar();
		}
		p = parsePoint();
		p = p * (1./objectColorRange);
		newLp->color = sysColor( p );
		
		if( peekChar() == hTimeCode){
			getChar();
			newLp->transitionTime = getFloat();
			abortOn( parameterError );
		}
		
		breakOn( peekChar() != ',' );
		getChar();
	}
	
	return parameterError == 0;
}




wwwBoolean parseLocalObjectKind( void )
{
	lp *newLp;
	int i = 0;
	int v1,v2;
	
	for(;;){
		newLp = getNewLpToLpChain( );
		newLp->type = ObjectKindType;
		
		if( i == BALL_MAX )
			return false;
		v1 = getInt();
		abortOn(parameterError);
		
		if( peekChar() == hObjectNum ){ // individually defined
			getChar();
			
			i = --v1;
			v2 = getInt();
			
			abortOn(parameterError || v1 < 0 || v2 >= endOfObjects );
			
			newLp->nr = v1;
			newLp->objectKind = v2;
		}else{
			newLp->nr = -1;
			newLp->objectKind = v1;
		}
		
		breakOn( peekChar() != ',' );
		getChar();
	}
	
	return parameterError == 0;
}



bool  parseLocalDrop( void )
{
	lp *newLp = getNewLpToLpChain( );
	
	newLp->type = DropAllType;
	
	return parameterError == false;
}

wwwBoolean parseLocalTPS( void )
{
	lp *newLp = getNewLpToLpChain( );
	
	newLp->type = TpsType;
	newLp->value = getFloat( );
	
	if( peekChar() == '@'){
		getChar();
		newLp->transitionTime = getFloat();
	}else
		newLp->transitionTime = 0;
	
	return parameterError == false;
}

wwwBoolean parseLocalTPS10( void )
{
	lp *newLp = getNewLpToLpChain( );
	
	newLp->type = TpsType;
	newLp->value = getFloat( )/10.f;
	
	if( peekChar() == '@'){
		getChar();
		newLp->transitionTime = getFloat();
	}else
		newLp->transitionTime = 0;
	
	return parameterError == false;
}


wwwBoolean parseLocalGravity( void )
{
	_lp *newLp = getNewLpToLpChain( );
	
	newLp->type = GravityType;
	newLp->value = getFloat( )*10.f;
	if( peekChar() == '@'){
		getChar();
		newLp->transitionTime = getFloat();
	}else
		newLp->transitionTime = 0;
	
	return parameterError == false;
}


bool getReverseViewList( void )
{
	if( peekChar() == 'r' ){
		getChar();
		
		for(;;){	
			int i;
			
			i = getInt() - 1;
			if( parameterError || i < 0 || i >= JUGGLER_MAX)
				return false;
			
			reverseViewIndex[i] = true;
			
			if(peekChar() != ',')
				break;
			getChar();		
		}
		
		if(peekChar() != ';')
			return false;
		getChar();
	}
	
	return true;
}

wwwBoolean parseSideToSide( void ){
	positionMode = PositionSideToSide;
	return getReverseViewList();
}

wwwBoolean parseTerrorFeed( void ){
	positionMode = PositionGorillaFeed;
	return getReverseViewList();
}

wwwBoolean parseDropbackLine( void ){
	positionMode = PositionDropbackLine;
	return getReverseViewList();
}
wwwBoolean parseLine( void ){
	positionMode = PositionLine;
	return getReverseViewList();
}

wwwBoolean parseCircle( void ){
	positionMode = PositionCircle;
	return getReverseViewList();
}



wwwBoolean parseBackToBack( void ){
	positionMode = PositionBackToBack;
	return getReverseViewList();
}

wwwBoolean parseFeed( void ){
	positionMode = PositionFeed;
	return getReverseViewList();
}

// geht nicht? (neue version)
// Wo steht ein Jongleur?
// Jeder Jongleur hat einen Standort und
// einen Normalenvektor, der seine Blickrichtung angibt.
// Der Standort ist ein punkt im Raum in der Form: (x,y,z)
// Der Normalenvektor wird aus einem 2. Punkte gewonnen, zu dem er blickt.
wwwBoolean parseGlobalJugglerPosition(void )
{
	int nr;
	Juggler *j;
	punkt p, dir, poi, pos;
	float b;
	
	for( ;;){
		
		nr = getInt() - 1;              // Wer ist es?
		if(parameterError || nr < 0 || nr >= JUGGLER_MAX )
			return false;
		
		j = getJuggler( nr );
		
		// syntax check
#ifdef syntaxStrict
		if( peekChar() != hObjectNum )
			return false;
		getChar();
#else
		if( peekChar() == hObjectNum )
			getChar();
#endif			
		
		pos = p =  parsePoint();        // Standpunkt
		if( parameterError )
			return false;
		j->position = p;
		
		poi = p = parsePoint(); // Blickrichtung
		if( parameterError )
			return false;
		// Man kann nicht schief in der Luft hngen
		p.y = j->position.y;
		
		
		dir =  poi - pos;			// Blickrichtung berechnen
		dir.y = 0;					// jugglers do it standing
		b = dir.betrag();
		if( b  == 0 )				// wrong direction
			return false;
		
		j->viewdirection = dir.normalised();      
		
		if( peekChar() != ',' )
			break;
		getChar();
	}	
	
	if( peekChar() == ';')
		getChar();
	
	return true;
}



// set jugglers positions (if not defined by the user)
void setJugglers( int jc )
{       
	int c;
	Juggler *j, *js;
	int o;
	int x0;
	int xrev = positionMirrorXFlag? -1:1;
	
#define POSITION_OFFSET 150				
	o = (jc/2 == jc/2.?-POSITION_OFFSET/2:0);
	
	
	for( c = 0; c < jc; c++ ){       
		bool evenFlag = c/2==c/2.;
		j = getJuggler( c );
		js = getStartPosition( c );
		
		if(j->position.x == NOWHERE ){                 // not defined: place him
			switch( positionMode ){
				
				case PositionFeed:
					x0 = (c+1)/2*(evenFlag?-POSITION_OFFSET:POSITION_OFFSET) 
					+ o 
					+ jc*POSITION_OFFSET/4*(evenFlag?1:-1)
					- (jc/2==jc/2.? 0 : POSITION_OFFSET/4);
					if( evenFlag)
						x0 *= -1;
					j->position = Vector3D(x0,0,evenFlag?200:-200);
					j->viewdirection = Vector3D(0,0,evenFlag?-1:1);						
					break;
					
				case PositionSideToSide:
					j->position = Vector3D(POSITION_OFFSET*c - POSITION_OFFSET*(jc-1)/2.f,0,0);
					j->viewdirection = Vector3D(0,0,1);
					break;
				case PositionGorillaFeed:
					if( c == 0 ){
						j->position = Vector3D(0,0,150);
						j->viewdirection = Vector3D(0,0,-1);
					}else{
						j->position = Vector3D(POSITION_OFFSET*(c-1) - POSITION_OFFSET*(jc-2)/2.f ,0,-150);
						j->viewdirection = Vector3D(0,0,1);
					}
					break;
					
				case PositionLine:	
					j->position = Vector3D(POSITION_OFFSET*c - POSITION_OFFSET*(jc-1)/2.f,0,0);					
					j->viewdirection = Vector3D(-1,0,0);
					break;
					
				case PositionDropbackLine:	
					j->position = Vector3D(POSITION_OFFSET*c - POSITION_OFFSET*(jc-1)/2.f,0,0);					
					if( c == 0 ){
						j->position = j->position + Vector3D(-100,0,0);
						j->viewdirection = Vector3D(1,0,0);
					}else
						j->viewdirection = Vector3D(-1,0,0);
					break;

				case PositionBackToBack:
				{	punkt jP;				
					jP = (jc == 1? pN:Vector3D(0,0,-10*jc));
					j->position = jP.drehungY((c * 360.f * jugglerPositionFactor) / jc);
					j->viewdirection = jN.drehungY((c * 360.f * jugglerPositionFactor) / jc) * -1;
					break;
				}

				case PositionTable:
				{	punkt jP;				
					jP = pTablePositions;
					j->position = jP.drehungY((c * 360.f * jugglerPositionFactor) / jc);
					j->viewdirection = jN.drehungY((c * 360.f * jugglerPositionFactor) / jc);
					break;
				}
				case PositionCircle:
				default:	
				{	punkt jP;				
					jP = (jc == 1? pN:pGroupPositions);
					j->position = jP.drehungY((c * 360.f * jugglerPositionFactor) / jc);
					j->viewdirection = jN.drehungY((c * 360.f * jugglerPositionFactor) / jc);
					break;
				}
			}
			if(reverseViewIndex[c])
				j->viewdirection = j->viewdirection.drehungY( 180 );
			if( closePositionFlag )
				j->position = j->position * .3f;
			
			
		}			
		j->position.x *= xrev;
		j->viewdirection.x *= xrev;
		j->pStart = j->pEnd     = j->position = calcBodyCenter( j->position );
		j->setJugglerOrientation();
		j->setHandVektors();
		*js = *j;
	}
}	

bool parseShow( void )
{
	
	return false;
}

bool parseGlobalObjectColorMode( void )
{
	int i;
	i = getInt();
	if( !parameterError && i >= 0 && i < ColorObjEnd)
		jpPrefs.objectColorMode = i;
	else
		parameterError = false;
	
	return parameterError == 0;
}

#if 0
// this parameter is a factor that is multiplied with every time a walk event happens
// the value is interpreted as a percentage of the pattern time and is fixed to the pattern lenght 
// This makes it possible to shift the movement in time in relation to the throws
bool parseWalkDelay( void )
{
	float f;
	
	f = getFloat();
	runaroundDelayWalk = f;
	
	return parameterError == 0;
}
#endif
