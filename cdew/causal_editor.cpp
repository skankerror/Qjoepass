/*
 * causal_editor.cpp
 * joe2
 *
 *  created by wolfgang on some day
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
 simple causal diagram editor
 
 ANSI C + openGL and glut library
 written by wolfgang westerboer
 westwolf@gmx.de
 
 
 functionions added
 + add throw
 + delete throw
 + add juggler
 + delete juggler
 + save mhn*
 + load mhn*
 lisat not longer maintained. see documentation
 (important: working on multiple window support)
 
 functions to add
 + handle delay
 + shift marked throws one cell, keep connections
 nice (at least for Martin)
 + save cde format
 + load cde format
 
 define MultiLineMode
 wenn ein Jongleur ber mehrere Zeilen verteilet wird
 
 
 cdew version history
 1.0     first try
 1.1 multiple window support
 1.2     integration in JoePass
 (with stand-alone option)
 some bug fixes
 
 cdew file version history
 1.0     1st try
 support of mhn*, with differences in interpretation of siteswap values of 2 and less
 1.1     marks added.
 Many functions that use marks
 1.2
 1.3     delays added
 postscript added
 program version number 1.0
 on the web at DUNNO
 
 support of christophes interpretation of causal and mhn
 color postscript, postscript currently disabled
 add diagrams to existing diagram
 program version number 1.01
 working on multiple window support
 
 
 
 */
#if 0
#define CDEW_STANDALONE 01
#endif

#if 0
#define VerboseCdewOutput
#endif

#define WindowSizeSavedPropperly 0

#define CDEWSIGNATURE   "cdewSignature"
#define CDEWLASTCHANGED "cdewLastChanged"
#define cdewSigFileName "signature.txt"
#define AnonSig         "anonymous"



#define         CDEW_DEF_X 0.379688f
#define         CDEW_DEF_Y 0.680556f
#define         CDEW_DEF_W 0.262500f
#define         CDEW_DEF_H 0.135417f


#define         DefWindowW              CDEW_DEF_W/*210.*/
#define         DefWindowH              CDEW_DEF_H/*100.*/

#define         DefCellW                64.
#define         DefCellH                50.

#define CDEW_DEFAULT_TITLE      "untitled"


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*****
#pragma mark ---------------------include
*****/

#include "graphics.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include <assert.h>


#include "prefDef.h"
#include "systemswitch.h"

#include "cdewClass.h"
#include "causal_editor.h"
#include "cdewDraw.h"
#include "cdewMenu.h"
#include "openglmenu.h"


#include "pathAccess.h"
#include "fileaccess.h"


#include "fileIO.h"
#include "mydefs.h"
#include "myPrintf.h"
#include "myutil.h"
#include "prefs.h"

#include "mainGL.h"
#include "cmdLine.h"

#include "workspace.h"
//#include "dialog.h"

#include "jpversion.h"

wwwBoolean dontChangeActualCellSizeFlag = false;

#ifdef USE_CDEW

// JoePass headers
#include "applicationFolders.h"             // directory access

#include "editor.h"
#include "pattern.h"
#include "animationWin.h"

#include "globals.h"
#include "parseSiteswap.h"


wwwBoolean		alreadyBusyToCreateOglwin = false;
bool			showErrors= true;


char			*cdewSigPath;
char			*ownerSignature = NULL;
void			readOwnerSignature ( void );

wwwBoolean		askForSaveLocation = true;


void            setCellNumber( _causalCell *p, int i);

float           dragDelay = 0.f;
bool			cdewSnap = true;

_causalCell		*cdewCopyBuffer = NULL;
int				cbJugglers =0, cbHands = 0, cbEntries = 0 ;

bool			cellBackgroundFlag = true;

oglwin*			actualOglwin;

enum			{StaggerXAndYMode, StaggerXMode, StaggerYMode};
int				cdewStaggerMode = StaggerXAndYMode;

int				dragJugglerFlag = jugglerNoDrag;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*****
#pragma mark --------------------- definitions
*****/

#define TempFolderName                  "temp"
#define InitW 300
#define InitH 100
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*****
#pragma mark --------------------- global headers for my classes
*****/
int                             mod( int v, int m);

void                    menuSelection( int r );

void                    _cdewDisplay(void);
void                    _cdewReshape(int w, int h);
void                    _cdewKeyboard(unsigned char key, int x, int y);
void                    _cdewSpecialKey(int key, int x, int y);
void                    _cdewMouse(int button, int state, int x, int y);
void                    _cdewMotion( int x, int y);
void                    _cdewPassiveMotion( int x, int y);
void                    cdewMenu( int id);

oglwin                  *getAnimatedWindow( void );
oglwin                  *getOglwinFromID( int id );
#if MAC_CLASSIC_VERSION
oglwin					*getOglwinFromMacWindow( WindowPtr macWindow );
#endif
void                    setAllWindowsUnused( void );
int                     getLastUsedWindow( void );

/*****
#pragma mark ---------------------more global prototypes
*****/
void                    _saveDiagramWithFP( FILE *fp );
void                    writeStdFile( void );
void                    initUndo( void );
void                    makeUndoDirectory( void );
void                    removeUndoDirectory( void );
void                    closeAllWin( void );
char                    *skipWord( char *p );
//void                  setCausalEditorWindowTitle( char *name );

/*****
#pragma mark -------------------- globals
*****/
oglwin                  *firstCdewWindow;
//oglwin                        *p, *l = 0;

/*
 
 int                     cdew1x							= CDEW_DEF_X;
 int                     cdew1y							= CDEW_DEF_Y;
 int                     cdew1w							= CDEW_DEF_W;
 int                     cdew1h							= CDEW_DEF_H;
 */
float                   templateCellWidth				=   DefCellW;
float                   templateCellHeight				=   DefCellH;
bool					autoFixConnections				= true;
int						cdewDefaultWidth				= 1;
bool					drawAsBowFlag					= false;
bool					showMarkFlag					= true;
bool					dontSetMeToTrueUnlessYouAreWWFlag			= false;

int                     xDefault                        = 500,
						yDefault						= 400,
						wDefault						= 400,
						hDefault						= 150;

//      ( windowHeight/(cdewJugglers * 2 * linesPerJuggler) )
int                     linesPerJuggler = 1;


bool					showUnusedHandsInSyncMode		= true;
int						xOffset							= XOFFSET,
						yOffset							= YOFFSET;
char					curFolder[FSIZE];
char					*undoFolder;
int						maxCellColors					= MaxCellColors;
bool					cdewAutoStart					= true;
bool					cdewAutoOpen					= false;
sysColor				timeCursorColor(1,.5,.5);
float					timeCursorWidth					= 3.;
float					timeCursorOffset				= .45;
char					*tempFolderName					= TempFolderName;
char					*causalContentTitle				= CausalContentTitle;
sysColor				inactiveColor(.93f, .93f, .93f);
sysColor				activeColor(1.f, 1.f, 1.f);

char                    cdew1StdFileName[]				= CDEW_1_FILE_NAME;
char                    cdewTemplateFileName[]          = CDEW_TEMPLATE_FILE;
char                    cdewPrefFileName[]				= CDEW_PREF_NAME;
char                    cdewStartFileName[]				= CDEW_START_NAME;
char                    cdewCrashFileName[]				= CDEW_CRASH_NAME;
char                    cdewSelectFileName[]            = CDEW_SELECT_FILE;

char                    *cdew1StdPath;
char                    *cdewTemplatePath;
char                    *cdewPrefPath;
char                    *cdewStartPath;
char                    *cdewCrashPath;
char                    *cdewSelectPath;

extern int              xDefault,
yDefault;
extern int              xOffset,
yOffset;

int						theMouseInputMode				= mimDrag;
int						autofixByUserSelection			= false;


wwwBoolean              cdewStartedPattern				= false;        // set to true if a pattern has been started. For startup needed
int						inactiveBackgroundMode			= cdewBackgroundGrey;

int						cdewWindowDefaultX				= -1,
						cdewWindowDefaultY				= -1,
						cdewWindowDefaultW				= -1,
						cdewWindowDefaultH				= -1;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*****
#pragma mark ---------------------psHeader
*****/

/* my colors:
the last entry is only used in color mode, for arrows that dont have a color information
*/sysColor colors[MAX_COLOR+1] = {        sysColor(0,0,.0f),
	sysColor(0,0,0.5f),
	sysColor(0,0.5f,.0f),
	sysColor(0.5f,0,.0f),
	sysColor(0.5f,0.5f,.0f),
	sysColor(0,0.5f,0.5f),
	sysColor(0.5f,0,0.5f),
	sysColor(.3f,.3f,.7f),
	sysColor(.6f,.1f,.3f),
	sysColor(.3f,.5f,.4f),
	sysColor(.6f,.2f,.6f),
	sysColor(.5f,.2f,.1f),
	sysColor(.3f,.3f,.3f)           // last entry
};



int getNumberOfWindows( void );

int getNumberOfWindows( void )
{
	int c = 0;
	oglwin *p = firstCdewWindow;
	
	while( p )
	{
		c++;
		p = p->next;
	}
	return c;
}



oglwin *getActualOglwin( void )
{	
	return actualOglwin;
}


/*****
#pragma mark -------------------- class functions
*****/
oglwin::oglwin(char *name)
{
	class oglwin *p = firstCdewWindow;
	
	actualOglwin = this;

#ifdef VerboseCdewOutput
	printf("creating causal editor window\n");
#endif
	if( alreadyBusyToCreateOglwin )
	{
		_myPrintf("trying to create oglwin while busy to create another one\n");
		getchar();
	}
	
	alreadyBusyToCreateOglwin = true;
	
	// add class to chain
	this->next = 0;
	
	if( !firstCdewWindow)
	{       firstCdewWindow = this;
		firstCdewWindow->next = 0;
	}
	else
	{       while( p->next)
	{//     last = p;
		p = p->next;
	}
		p->next = this;
	}
	
	jpPrefs.causalEditorHasOpenWindows = true;
	
	cdewWindowX                     = CDEW_DEF_X;
	cdewWindowY                     = CDEW_DEF_Y;
	cdewCellWidth                   = DefCellW;
	cdewCellHeight                  = DefCellH;
	
	initVars();
	if( this == firstCdewWindow )
		readCDEWPrefs();
	initPattern();
	initGLwin( name);
	
	
	autoStartRestartFlag = true;
#if 0
#if !CDEW_STANDALONE
	if( cdewAutoStart )
		transferToAnimation();
	if( cdewAutoOpen)
		transferToTextEditor();
#endif
#endif
	
	alreadyBusyToCreateOglwin = false;
	
#ifdef VerboseCdewOutput
	printf("creating causal editor window %d ... done\n\n", id);
#endif
}

void oglwin::initVars( void )
{
	// set class globals to default
#ifdef VerboseCdewOutput
	printf("init cdew vars \n");
#endif
	
	defaultThrow                    = DefaultThrow;
	mouseState                      = GLUT_UP;
	
	cdewJugglers                    = DefJugglers;
	cdewHands                       = Hands;
	cdewEntries                     = DefEntries;
	maxCellColors					= MaxCellColors;
	cellColor						= MaxCellColors-1;
	
	// cdewDisplay stuff
	defaultWidth					= cdewDefaultWidth;
	
	showDelays                      = false;
	printArrowLength                = false;
	printSiteswap                   = LetterDisplayMode;
	displayMode                     = singleLineMode;
	radius                         = RadiusOFHandCircle;
	jugglerDelayList                = 0;
	jugglerLines                    = false;
	hasHandDelay                    = false;
	changeBowfactor                 = false;
	bowFactor                               = 1.;                   // factor for causals drawn as bows
	arrowAngleFactor                = 1.;   // factor for arrows on bows
	arrowheadFactor                 = 1.;   // size factor for arrowheads
	
	cdewColorMode                   = LengthColorMode;
	
	arrow1							= Vector3D( -3,6,0);            // arrowwhead
	arrow2							= Vector3D( 3,0,0);
	arrow3							= Vector3D(-3,-6,0);
	arrow4							= NullVector;
	// memory stuff
	undoCounter                     = 0;
	maxUndo							= 0;
	highestUndoNumber               = 0;
	oglwinUndoState                 = undoActivated;
	fileChangedFlag                 = false;
	// causal input stuff
	userDoesJugglerDelayDrag		= false;
	userDoesJugglerDelayOneHandDrag= false;
	userDoesThrowDelayDrag			= false;
	userDoesMarkDrag                = false;
	userDoesDrag                    = false;
	userDoesFactorDrag              = false;
	showNumbers                     = false;
	showDisplayMode                 = false;
	showColorMode					= false;
	showErrors						= true;
	
	unhurriedDiagram                = false;
	
	// siteswap input stuff
	nextSiteswapInputCell			= false;
	siteswapInputEntry              = 0;
	siteswapInputJuggler			= 0;
	siteswapInputHand               = rightHand;
	siteswapInputMode               = false;
	christophesNotation				= false;
	siteswapInputForceOtherHand		= false;
	showObjects                     = false;
	// postscript stuff
	printPS                         = false;
	psSaved                         = false;
	//      colorPostscript                 = false;
	// file stuff
	patternSaved                    = false;
	patternSavedByUser              = false;
	autoSaveFlag                    = false;
	
	sprintf(cdewFileName, "%s", "untitled.pass");
	sprintf(cdewPathName, "%s", ".");
	sprintf(cdewPathAndFileName, "%s", "");
	
	showTimeCursor                  = false;
	imTheAnimatedWindow             = false;
	imTheLastUsedWindow             = false;
	imTheOnePersonNotationWin       = false;
	header                          = NULL;
	
	showHurries                     = false;
	showCrosses                     = true;
	showStart                       = false;
	
	pasteInProgressFlag				= false;
	pasteTempBuffer					= NULL;
	
	markedDragInProgressFlag		= false;
	markedDragTempBuffer			= NULL;
	
	if( !ownerSignature )
	{
//		showDialog( "owner signature not defined ");
		getchar();
		exit(0);
	}
	
	sig = NULL;
	lastChanged = NULL;
    setSignature( ownerSignature );
    setLastChanged( ownerSignature );
    
    startHandList                       = NULL; 
	
#ifdef VerboseCdewOutput
	printf("init cdew vars ... done\n");
#endif
}

void oglwin::setSignature( char *newSig )
{
    size_t  l;
	
	
    if( sig )
        free( sig );
	
	l = strlen( newSig );
	sig = (char *)malloc( l + 1 );
	sprintf(sig, "%s", newSig);
	
	
}

void oglwin::setLastChanged( char *newLastChanged )
{
    size_t  l;
	
	
    if( lastChanged )
        free( lastChanged );
	
	l = strlen( newLastChanged ) + 1 ;
	lastChanged = (char *)malloc( l);
	sprintf( lastChanged, "%s", newLastChanged);
	
	
}

void readOwnerSignature ( void )
{
    FILE *fp;
    size_t c, l;
    wwwBoolean sigDefined = false;
	
#ifdef VerboseCdewOutput
	printf( "reading owner signature \n");
#endif
	
    if( ownerSignature )
        free( ownerSignature );
	
    fp = fopen( cdewSigPath, "r" );
    if( fp != NULL )
    {
        l = getFileSize( fp );
        ownerSignature = (char *)malloc( l + 1);
        if( ownerSignature == NULL )
			fclose( fp );
        else
        {
            fread( ownerSignature, l, 1, fp);
            fclose( fp );
            *(ownerSignature + l) = '\0';
            sigDefined = true;
			//_myPrintf("signature = %s\n", ownerSignature);			
            for( c = 0; c < l; c++ )
            {
                if( *(ownerSignature+c) == '\n' ||
                    *(ownerSignature+c) == '\r')
                {
                    *(ownerSignature+c) = '\0';
                }
            }
			
			
        }
    }
	
	
    if( !sigDefined )
    {
		
        ownerSignature = (char *)malloc( strlen(AnonSig) + 1);
        sprintf(ownerSignature, "%s", AnonSig);
    }
	
	//printf(" signature is %s\n", ownerSignature);
	
	
}


void oglwin::initPattern( void )
{
	// in case there is no prefs-File or its corrupted
#ifdef VerboseCdewOutput
	printf("init cdew pattern \n");
#endif
	if( cdewJugglers == 0 )
		cdewJugglers            = DefJugglers;
	if( cdewHands == 0 )
		cdewHands               = Hands;
	if( cdewEntries == 0)
		cdewEntries				= DefEntries;
	
	// initDefaultPattern
	initCL( );
	
	
	
	// first pattern: read content of previous first pattern
	if(this == firstCdewWindow )
	{       
		wwwBoolean undoStateTemp = oglwinUndoState;
		oglwinUndoState = undoDeactivated;
		
		// no first pattern saved (first time ever run, or folder sys empty / deleted )
		// set default hand
		// make every entries / 2 throws a pass to juggler (j+1)%jugglers (create a cycling feed)
		if( readInputFile( cdew1StdFileName ) == false )
		{       _causalCell *p;
			int c = 0;
			int count = (cdewEntries%3) == 0? 3:2;
			
			setDefaultHand();
			fileChangedFlag = false;
			
			allCells(
					 p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					 if(c % count == 0)
					 p->destJuggler = (p->destJuggler+1)%cdewJugglers;
					 c++;
					 )
				cdewCellWidth = DefWindowW / count;
			cdewCellHeight = DefWindowH / cdewJugglers;
			
		}
		oglwinUndoState = undoStateTemp;
	}
	// read template otherwise
	else{
		readInputFile( cdewTemplatePath );
	}
	
#ifdef VerboseCdewOutput
	printf("init cdew pattern ... done \n");
#endif
	
}


void staggerWindowPosition( int *x, int *y)
{
	
	int     wDesk, hDesk;
	int     initW, initH;
	
	
	// default sizes
	initW = InitW;
	initH = InitH;
	
	
	// right border behind screen?
	// mac os x reports glut error here: no active window
	wDesk = glutGet(GLUT_SCREEN_WIDTH) - initW;
	hDesk = glutGet(GLUT_SCREEN_HEIGHT) - initH;
	
	switch( cdewStaggerMode ){
		case StaggerYMode:
			*y += yOffset + glutGet( GLUT_WINDOW_HEIGHT);
			break;
		case StaggerXMode:
			*x += xOffset + glutGet( GLUT_WINDOW_WIDTH);
			break;
		default:
		case StaggerXAndYMode:
			*x += xOffset;
			*y += yOffset;
			break;
	}
	
	while( *x >= wDesk )
	{       *x -= wDesk;
		*y = 0;
	}
	*y %= hDesk;
	
#define                 xMin  100
#define                 yMin  50
#define                 wMin  100
#define                 hMin  30
	*x = max(xMin, *x);
	*y = max(yMin, *y);
	
}


void oglwin::initGLwin( char *name )
{
#ifdef VerboseCdewOutput
	printf("init cdew window \n");
#endif
	
	int     wDesk, hDesk;
	int     initW, initH;
	
	
	// default sizes
	initW = InitW;
	initH = InitH;
	
	
	// right border behind screen?
	// mac os x reports glut error here: no active window
	wDesk = glutGet(GLUT_SCREEN_WIDTH) - initW;
	hDesk = glutGet(GLUT_SCREEN_HEIGHT) - initH;
	
	
	// set init window sizes
	// first: fixed position
	if( firstCdewWindow == this )
	{
		
		if( cdewCellWidth == 0 )
			cdewCellWidth = DefCellW;
		if( cdewCellHeight == 0 )
			cdewCellHeight = DefCellH;
		
		windowWidth =
			cdewWindowW = (int)cdewCellWidth * cdewEntries;
		windowHeight =
			cdewWindowH = (int)cdewCellHeight * cdewJugglers;

		
		xDefault = cdewWindowX;
		yDefault = cdewWindowY;
		wDefault = cdewWindowW;
		hDefault = cdewWindowH;
	}
	// stagger other windows
	else{
		
		readTemplatePrefs();
		cdewCellWidth = templateCellWidth;
		cdewCellHeight = templateCellHeight;
		
		windowWidth =
			cdewWindowW = (int)cdewCellWidth * cdewEntries;
		windowHeight =
			cdewWindowH = (int)cdewCellHeight * cdewJugglers;
		
		wDefault = cdewWindowW;
		hDefault = cdewWindowH;
		

		staggerWindowPosition( &xDefault, &yDefault);
	}
	

	if( xDefault < xMin || xDefault >= wDesk)
		xDefault = xMin;
	if( wDefault < yMin || wDefault >= wDesk )
		wDefault = (int)(wDesk * .25);
	if( hDefault < wMin || hDefault >= hDesk)
		hDefault = (int)(hDesk * .20);
	if( yDefault < hMin || yDefault >= hDesk)
		yDefault = (int)(hDesk* .25);
	
	{	int x,y,w,h;
		getDefaultCdewWindowSize( &x, &y, &w, &h);
		if( x > 0 ){
			xDefault = x;
			yDefault = y;
			wDefault = w; 
			hDefault = h;
		}
	}
	
#ifdef VerboseCdewOutput
	printf("init cdew window size to %d %d %d %d \n",xDefault,yDefault, wDefault, hDefault);
#endif
	
	// set openGL parameters
	glutInitWindowSize (wDefault, hDefault);
	glutInitWindowPosition (xDefault, yDefault );
	// create window and set name
	id = glutCreateWindow("untitled");
	
	_myPrintf("init cdew window ID = %d\n", id);

	
	if( id <= 0 )
        getchar();
	
	//printf("cdewID = %d\n", id );
	setCdewWindowTitle( name );
	
#ifdef MAC_CLASSIC_VERSION
    macWindow = FrontWindow();
#endif
	
	// set callbacks
	
#ifdef VerboseCdewOutput
	printf("init cdew window functions\n");
#endif
	
	glutDisplayFunc(_cdewDisplay);
	glutReshapeFunc(_cdewReshape);
	glutSpecialFunc(_cdewSpecialKey);
	glutKeyboardFunc(_cdewKeyboard);
	glutMouseFunc(_cdewMouse);
	glutMotionFunc(_cdewMotion);
	glutPassiveMotionFunc(_cdewPassiveMotion);
	glutMenuStatusFunc(globalMenuStatusFunc);
	
	
#ifdef VerboseCdewOutput
	printf("init cdew window menu\n");
#endif
	cdewInitMenu();
	
#ifdef VerboseCdewOutput
	printf("init cdew window redisplay\n");
#endif
	glutPostRedisplay();
	
#ifdef VerboseCdewOutput	
    printf("cdewWinID = %d\n", id);
#endif
} 




// alt, aber gut: close one window
wwwBoolean oglwin::closeWin(  )
{
	glutSetWindow( id );
	
	
	if( !cmdLineToCdew){		
		if( jpPrefs.askForSaveFlag ){
			if( autoSaveFlag == true )
				saveDiagramAuto();
			else
//				saveDiagram();
				_saveDiagram( true );
		}
	}
	
	if( this == firstCdewWindow )
		saveDiagramWithName( cdew1StdFileName );
	
	if( imTheOnePersonNotationWin )
		askForSaveLocation = true;

	delete this;
	
	
	return true;
	
} 

oglwin::~oglwin()
{
	class oglwin *p;
	class oglwin *last = NULL;
	p = firstCdewWindow;
	
#ifdef VerboseCdewOutput
	printf("\ndestroy causal editor window %d\n", id);
#endif
	
	if( this == NULL){
		printf("bogous window class\n");
		return;
	}
	if( id <= 0 )
		printf("unknown window\n");
	
	
	removeUndoFiles();
	
	clearHeader();
	
	if( this == firstCdewWindow )
		writeCDEWPrefs();
	
	if( causalCells ){
		free( causalCells );
		causalCells = 0;
	}
	glutDestroyWindow(id);
	
	
	
	// get pointer to this
	// firstCdewWindow can not be found, see above
	// thus, last never can be 0
	while( p && p != this )
	{
		last = p;
		p = p->next;
	}
	if( this == firstCdewWindow )
		firstCdewWindow = firstCdewWindow->next;
	else if( last )
		last->next = p->next;
	
	if( firstCdewWindow == NULL )
		removeUndoDirectory();
	
#ifdef VerboseCdewOutput
    printf("cdewWinID = %d\n", id);
#endif
	
}


// ------------ windows ---------------
/*****
#pragma mark --------------------- window routined
*****/
int makeOglwin( char *title )
{
	oglwin *p;
	
	if( firstCdewWindow )
		p = new oglwin( title);
	else
	{
		initCausalEditor(title);
		p = firstCdewWindow;
	}
	if( p )
		return p->id;
	else
		return 0;
	
}


int getActiveOglwin( void )
{
	oglwin *p = getAnimatedWindow();
	if( p )
	{       glutSetWindow(p->id);
		return p->id;
	}else
		return 0;
}

wwwBoolean writePatternToNewOglwin( char *name, bool allowSameFilename )
{
	return writePatternToOglwin( -1, name,  allowSameFilename );
}


wwwBoolean writePatternToOglwin( int id, char *name, bool allowSameFilename )
{     
	wwwBoolean ret;
	char pa[MAX_FULL_FILE_NAME], n[MAX_FULL_FILE_NAME], pn[MAX_FULL_FILE_NAME];
	oglwin *t;
	oglwin *p;
	
	
	splitPathName( name, pa, n, pn);
	
	t = firstCdewWindow;
	if(!allowSameFilename){
		//yyy
		//look for the window who has the same path as the one we use
		// neccessary to make it possible that the text editor opens several causal windows,
		// but not to open a given file more than once
		// hence, allowSameFilename is true when called from the text editor
		while(t ){
			if( strcmp(t->cdewPathAndFileName, name ) == 0)
				return false;
			t = t->next;
		}
	}
	
	if( id >= 0 )
		p = getOglwinFromID(id);
	else {
		p = new oglwin(n);
		id = p->id;
	}
	
	
	if( p )
	{
		getOglwinFromID( id );
		glutSetWindow( id );
		ret = p->readInputFile( name );
		p->imTheLastUsedWindow = true;
		
		return ret;
		
		
	}else
		return false;
}

#if 0

wwwBoolean writePatternToOglwin( int id, char *name, bool allowSameFilename )
{     
	wwwBoolean ret;

	oglwin *t;
	
	t = firstCdewWindow;
	if(!allowSameFilename){
		//yyy
		//look for the window who has the same path as the one we use
		// neccessary to make it possible that the text editor opens several causal windows,
		// but not to open a given file more than once
		// hence, allowSameFilename is true when called from the text editor
		while(t ){
			if( strcmp(t->cdewPathAndFileName, name ) == 0)
				return false;
			t = t->next;
		}
	}

	
	oglwin *p = getOglwinFromID(id);
	
	if( p )
	{
		getOglwinFromID( id );
		glutSetWindow( id );
		ret = p->readInputFile( name );
		p->imTheLastUsedWindow = true;
		
		return ret;
		
		
	}else
		return false;
}
#endif


void redisplayAllCDEWWindows( void )
{
	oglwin *p = firstCdewWindow;
	int id = glutGetWindow();
	
	while( p )
	{
		glutSetWindow( p->id );
		glutPostRedisplay( );
		p = p->next;
		
		
	}
	if( id )
		glutSetWindow( id );
	
}

oglwin *activateCausalEditor( void )
{
	oglwin *p;
	if( !firstCdewWindow )
		p = initCausalEditor(CDEW_DEFAULT_TITLE);
	
	else
		p = new oglwin(CDEW_DEFAULT_TITLE );
	
	return p;
}


void setAllWindowsInactive( void )
{
	oglwin *p =firstCdewWindow;
	
	while( p )
	{
		p->imTheAnimatedWindow = false;
		p = p->next;
	}
}

void setAllWindowsUnused( void )
{
	oglwin *p =firstCdewWindow;
	
	while( p )
	{
		p->imTheLastUsedWindow = false;
		p = p->next;
	}
}



int getLastUsedWindow( void )
{
	oglwin *p =firstCdewWindow;
	
	while( p )
	{
		if( p->imTheLastUsedWindow)
			break;
		p = p->next;
	}
	
	// return window id, zero if no window exists
	if( p )
		return p->id;
	else
		return 0;
	
}


void oglwin::setCdewWindowTitle( char *name )
{
	char *p;
	GLint tmpID = glutGetWindow();
	glutSetWindow(id );
	
	p = (char *)malloc( strlen( name ) + 100);
	sprintf(p, "%s (%d)", name, id);
	
	glutSetWindowTitle( p );
	
	free( p );
	
	glutSetWindow( tmpID);
	
//	setPatternTitle( name);
}



/*****
#pragma mark -------------------- memory
*****/
oglwin *getOglwinFromID( int id )
{
	oglwin *p = firstCdewWindow;
	
	while( p )
	{
		if( p->id == id )
			return p;
		p = p->next;
	}
	
	return 0;
}
int getOglwinOnePersonNotationID( void )
{
	oglwin *p = firstCdewWindow;
	
	while( p )
	{
		if( p->imTheOnePersonNotationWin == true )
			return p->id;
		p = p->next;
	}
	
	return 0;
}

#if MAC_CLASSIC_VERSION
// pre os x stuff
oglwin *getOglwinFromMacWindow( WindowPtr macWindow )
{
	oglwin *p = firstCdewWindow;
	
	while( p )
	{
		if( p->macWindow == macWindow )
			return p;
		p = p->next;
	}
	
	return 0;
}
#endif
oglwin *getAnimatedWindow( void )
{       oglwin *t = firstCdewWindow;
	
	// get last window
	while( t != NULL)
	{
		if( t->imTheAnimatedWindow)
			break;
		t = t->next;
	}
	
	return t;
}

/*****
#pragma mark ---------------------memory access
*****/
// ------------------------------- memory stuff ------------------------------------
wwwBoolean oglwin::isSyncThrow( _causalCell *p )
{
	_causalCell *p2;
	int j,h,e;
	wwwBoolean a,b, isNearlySync;
	
	if( getIndicesFromCellPointer( p, &j, &h, &e ) == false )
	{
		//_myPrintf("wrong pointer \n");
		return false;
	}
	p2  = getCausalCell( causalCells, j, switchHand( h), e, cdewJugglers, cdewHands, cdewEntries );
	
	a = p->marked || p->length != absorb || p->notReached || p->notOutgoing || p->showIfEmpty;
	
	b = p2->marked || p2->length != absorb || p2->notReached || p2->notOutgoing || p2->showIfEmpty;
	
	isNearlySync = fabs(p->delay - p2->delay) < .1;
	
	return a && b && isNearlySync;
}



int oglwin::switchHand( int h)
{
	return h == leftHand?rightHand:leftHand;
}

_causalCell *oglwin::copyDiagram( void )
{
	_causalCell *p, *s, *d;
	int j,h,e;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return 0;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{       for( h = 0; h < cdewHands; h++ )
	{
		for( e = 0; e < cdewEntries; e++ )
		{       s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, h, e, cdewJugglers, cdewHands, cdewEntries);
			*d = *s;
		}
	}
	}
	return p;
}


/*****
#pragma mark --------------------- helpers
*****/


// % operator may not work correct for negatice value
int mod( int v, int m)
{
	while( v < 0)
		v += m;
	while( v >= m )
		v -= m;
	return v;
}


void oglwin::nextMouseInputMode( void )
{
	theMouseInputMode++;
	theMouseInputMode %= mimEnd;
}

/*
 1.      if a juggler has a delay > .8, add difference to 1 to all jugglers
 2.      for all jugglers with delay value >= 1
 move all throws one beat to the rightHand
 add 1 to pass throws to this juggler
 decrease delay by 1
 */
void oglwin::fixAllConnections( void )
{
	int mode = displayMode;
	if( 0 && displayMode == syncLineMode)// && !autofixByUserSelection) 
		return;
	
	addToUndoList();
	// fixConnections usualy doesnt work in sync mode
	//      displayMode =syncLineMode;
	allCells( _fixConnections(jAll, eAll);)
	displayMode = mode;
	
	
}
/*      this function walks through all cells and checks if they are pointing to a used cell.
if not, i.e. a cell points to an unused cell but the sync cell is used, it switches cdewHands
*/
void oglwin::fixConnections( int sJ, int sE )
{
	
	if(displayMode == syncLineMode )
		return;
	
	_fixConnections( sJ, sE );
}

void oglwin::_fixConnections( int sJ, int sE )
{
	int j,e;
	_causalCell *pl, *pr;
	
	if( !autoFixConnections )
        return;
	
	
	pl = getCausalCell( causalCells, sJ, leftHand, sE, cdewJugglers, cdewHands, cdewEntries);
	pr = getCausalCell( causalCells, sJ, rightHand, sE, cdewJugglers, cdewHands, cdewEntries);
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( e = 0; e < cdewEntries ; e++)
		{
			_causalCell *l, *r, *ptl = 0, *ptr = 0;
			
			// get pointer to leftHand hand
			l = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries );
			if( l->length != absorb && !(l->length == empty))
			{
				ptl = getCausalCell( causalCells, l->destJuggler, l->destHand, e + l->length, cdewJugglers, cdewHands, cdewEntries );
				ptr = getCausalCell( causalCells, l->destJuggler, switchHand(l->destHand), e + l->length, cdewJugglers, cdewHands, cdewEntries );
				if( ptl == pl )
					if( (pl->length == absorb && !pl->marked) && pr->length != absorb )
						l->destHand = switchHand( l->destHand);
				if( ptr == pl )
					if( (pr->length == absorb  && !pr->marked) && pl->length != absorb )
						l->destHand = switchHand( l->destHand);
			}
			
			ptl = ptr = 0;
			// get pointer to rightHand hand
			r = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries );
			if( r->length != absorb && !(r->length == empty))
			{
				ptr = getCausalCell( causalCells, r->destJuggler, r->destHand, e + r->length, cdewJugglers, cdewHands, cdewEntries );
				ptl = getCausalCell( causalCells, r->destJuggler, switchHand(r->destHand), e + r->length, cdewJugglers, cdewHands, cdewEntries );
				if( ptr == pr )
					if( (pr->length == absorb  && !pr->marked) && pl->length != absorb )
						r->destHand = switchHand( r->destHand);
				if( ptl == pr )
					if( (pl->length == absorb  && !pl->marked) && pr->length != absorb )
						r->destHand = switchHand( r->destHand);
			}
		}
	}
	autoStartRestartFlag = fileChangedFlag = true;
}


// one of the many hacks to make christophes notation work.
// here: change 2 to 1x if another throw follows. Not interpreted as a hurry
void oglwin::change2to1x( void )
{
	_causalCell *p1, *p;
	//myPrintf("changing 2 to 1x\n");
	
	allCells(
			 p = getCausalCell(causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 p->c1x = false;
			 if( christophesNotation )
			 {       p1 = getCausalCell(causalCells, jAll, hAll, eAll+1, cdewJugglers, cdewHands, cdewEntries);
				 if( p->length == 0 && p1->length != absorb)
					 p->c1x = true;
			 }
			 );
	//myPrintf("changing 2 to 1x ...done\n");
}

// returns true if all loops are closed,
// i.e. the pattern is correct
wwwBoolean oglwin::allLoopsClosed( void )
{
	_causalCell *p;
	int errFlag;
	wwwBoolean patternEmpty = true;
	
	
	errFlag = showErrors;
	showErrors = true;
	
	markFaults();
	
	showErrors = errFlag;
	
	patternHasOpenLoops = false;
	
	allCells(
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 
			 if( p->notOutgoing || p->notReached || p->reached> 1)
			 {       patternHasOpenLoops = true;
				 return false;
			 }
			 if( p->length != absorb )
			 patternEmpty = false;
			 
			 )
		
        if( patternEmpty )
			return false;
	
	return true;
	
}



/*****
#pragma mark ---------------------initialisation
*****/
// ------------------------------- initialisation stuff ------------------------------------
void oglwin::initCL( void )
{
	int j,h,e;
	
	causalCells = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( causalCells == NULL)
	{
		printf("initGL: out of memory\r");
        exit( 1 );
		
		
	}
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
				initCC(causalCells, j, h, e );
	
	delayableJugglers = cdewJugglers;
	if( jugglerDelayList != NULL)
		free(jugglerDelayList);
	jugglerDelayList = allocDelayList( cdewJugglers);
	siteswapInputEntry = 0;
	nextSiteswapInputCell = false;
	siteswapInputHand = rightHand;
	siteswapInputForceOtherHand = false;
}

// NB: Function uses actual values of cdewJugglers, cdewHands, cdewEntries
void oglwin::initCC( _causalCell *p,  int j, int h, int e )
{
	
	_initCC( p, j, h, e, cdewJugglers, cdewHands, cdewEntries);
}



void oglwin::_initCC( _causalCell *p,  int j, int h, int e, int jMax, int hMax, int eMax )
{       int hand,destHand;
	_causalCell *p1, *p2;
	
	
	// even indices are thrown with the rightHand hand
	hand = e%2? leftHand:rightHand;
	
	p1 = getCausalCell( p, j,rightHand,e, jMax, hMax, eMax);
	p2 = getCausalCell( p, j,leftHand,e, jMax, hMax, eMax);
	
	
	p1->length = p2->length = DefContent;
	p1->destJuggler = p2->destJuggler = j;
	
	// switch destination hand for odd numbers
	destHand = hand;
	if( p1->length % 2 )
		destHand = (hand == leftHand?rightHand:leftHand);
	p2->destHand = p1->destHand = destHand;
	if( hand == leftHand )
	{
		p1->length = absorb;
	}else
	{
		p2->length = absorb;
	}
	p1->length = p2->length = absorb;
	
	p1->userDefArrowColor= p2->userDefArrowColor		= NO_COLOR;
	p1->userDefBackgroundColor= p2->userDefBackgroundColor		= NO_COLOR;
	
	p1->autoColor   = p2->autoColor			= NO_COLOR;
	p1->dot         = p2->dot               = -1;
	p1->dash        = p2->dash              = -1;
	p1->width       = p2->width				= defaultWidth;
	p1->marked      = p2->marked			= false;
	p1->delay       = p2->delay				= 0.;
	p1->c1x         = p2->c1x               = false;
	p1->invertHandHeightOffset = p2->invertHandHeightOffset = false;
	p1->showIfEmpty = p2->showIfEmpty		= false;
	p1->feedbackFlag= p2->feedbackFlag		= false;
	
	p1->factor      =  p2->factor			= 0.;
	
	p1->beep        = p2->beep				= false;
	p1->bounce      = p2->bounce			= false;
	p1->halfspin      = p2->halfspin		= false;
	p1->repeatIn    = p2->repeatIn			= false;
	p1->repeatOut    = p2->repeatOut		= false;
	p1->startConflict= p2->startConflict	= false;
	
	
}





// copy content of actual pattern to buffer

void setOglwinSize( int id )
{
    int oldID ;
    int w,h;
    // make screen somewhat smaller for wwindow borders
    int sw = glutGet(GLUT_SCREEN_WIDTH );
    int sh = glutGet(GLUT_SCREEN_HEIGHT );
	
    oldID = glutGetWindow();
	oglwin *p = getOglwinFromID(id);
	
	if( p )
	{
		
		
		//              getOglwinFromID( id );
		w = (int)p->cdewCellWidth * p->cdewEntries;
		h = (int)p->cdewCellHeight * p->cdewJugglers;
		
		if( w > sw )
        {
            p->cdewCellWidth = (int)(sw / p->cdewEntries) ;
            w = (int)p->cdewCellWidth * p->cdewEntries;
			//_myPrintf("recalculating width\n");
        }
		
        if( h > sh)
		{
			p->cdewCellHeight = (int)(sh / p->cdewJugglers) ;
			h = (int)p->cdewCellHeight * p->cdewJugglers;
			//_myPrintf("recalculating height\n");
		}
		
		
		glutReshapeWindow(w,h );
	}
	
	glutSetWindow( oldID );
}

/*void calctTemplateCellSize( oglwin *p )
{
	cdewTemplateCellH = glutGet( GLUT_WINDOW_HEIGHT) / p->cdewJugglers;
	cdewTemplateCellW = glutGet( GLUT_WINDOW_WIDTH) / p->cdewEntries;
	//_myPrintf("defSize(w/h) = %d %d\n", cdewTemplateCellW, cdewTemplateCellH);
}
*/

// bldsinn: Berechnet Zellengre!



void oglwin::getActualCellSize( int *cellWidth, int *cellHeight )
{
	*cellWidth = glutGet(GLUT_WINDOW_WIDTH) / cdewEntries;
	*cellHeight = glutGet(GLUT_WINDOW_HEIGHT) / cdewJugglers;
	
}

/*
 addSymPattern etc achten selber auf die Fenstergre
 addThrow etc muss das nur tun, wenn es direkt aufgerufen wird.
 -> grennderung in den Aufruf.
 */

void oglwin::setActualCellSize(int *cellWidth, int *cellHeight )
{
	
	if( dontChangeActualCellSizeFlag )
		return;
	
	float cw = *cellWidth, ch = *cellHeight;
	int w,h;
	int ww, wh;
	
	ww = glutGet(GLUT_SCREEN_WIDTH);
	wh = glutGet(GLUT_SCREEN_HEIGHT);
	
	if( cw * cdewEntries > ww )
		cw = ww / cdewEntries;
	
	if( ch * cdewJugglers > wh )
		ch = wh / cdewJugglers;
	
	w = (int)(cw * cdewEntries);
	h = (int)(ch * cdewJugglers);
	
	
	*cellWidth = (int)cw;
	*cellHeight = (int)ch;
	
	cdewReshapeWindow( w,h);
	
}

void oglwin::cdewReshapeWindow( int windowWidth, int windowHeight)
{
	/*
	 float f = 1.f;
	 
	 *w = min( *w * cdewEntries,(int)(glutGet( GLUT_SCREEN_WIDTH )*f));
	 *h = min( *h * cdewJugglers, (int)(glutGet( GLUT_SCREEN_HEIGHT )*f));
	 
	 
	 if( *w + glutGet( GLUT_WINDOW_X) < 0 )
	 {
		 GLint y;
		 
		 y = glutGet( GLUT_WINDOW_Y);
		 glutPositionWindow(0,y);
		 _myBeep();
	 }
	 
	 if( *w> 0 && *h > 0)
	 */
	
	
	
	glutReshapeWindow(windowWidth, windowHeight);
	
}

/*****
#pragma mark ---------------------cell selection
*****/
// determinate wich cell (leftHand or rightHand) to use for a throw
// called when user makes a cdewMouse click

// if not sync
// there is only one cell pointing to this beat,
// and if its to the wrong hand: change its hand
void oglwin::getDesiredCell( int *sJ, int *sH, int *sE )
{
    _causalCell *pl, *pr, *psl, *psr;
    _causalCell *p, *ps;
	
	
    _getDesiredCell( sJ, sH, sE );
	
	// if not in syncLineMode, only one cell is used.
	// the used cell depends on the user input (upper part of the cell: right, lower part: left, center: opposite to last used.
	// the incomming arrow is connected with the so defined cell.
	// in sync line mode this is not a useful behavieour and is disabled 
    if(displayMode != syncLineMode)
    {
		pr = getCausalCell( causalCells, *sJ, rightHand, *sE, cdewJugglers, cdewHands, cdewEntries);
        pl = getCausalCell( causalCells, *sJ, leftHand, *sE, cdewJugglers, cdewHands, cdewEntries);
		
        // me is empty
        if( pl->length == absorb &&  pr->length == absorb )
        {
            p = getCausalCell( causalCells, *sJ, *sH, *sE, cdewJugglers, cdewHands, cdewEntries);
			
            // there is exactly one pointer to me
            psl = getCellToCell( pl );
            psr = getCellToCell( pr );
            if( psl && psr == NULL || psl==NULL && psr )
            {
				// make it point to the selected field
                ps = (psl?psl:psr);
                ps->destHand = *sH;
            }
        }
    }
}


void oglwin::_getDesiredCell( int *sJ, int *sH, int *sE )
{
	_causalCell *pl, *pr, *p, *pPrev, *pPrevSync, *pSync;
	wwwBoolean sync;
	int e;
	getCellFromMousePosition( mouseX,  mouseY, sJ, sH, sE);
	
	p = getCausalCell( causalCells, *sJ, *sH, *sE, cdewJugglers, cdewHands, cdewEntries);
	pSync = getCausalCell( causalCells, *sJ, switchHand(*sH), *sE, cdewJugglers, cdewHands, cdewEntries);
	
	pPrev = getCausalCell( causalCells, *sJ, *sH, *sE-1, cdewJugglers, cdewHands, cdewEntries);
	pPrevSync = getCausalCell( causalCells, *sJ, switchHand(*sH), *sE-1, cdewJugglers, cdewHands, cdewEntries);
	
	pl = getCausalCell( causalCells, *sJ, leftHand, *sE, cdewJugglers, cdewHands, cdewEntries);
	pr = getCausalCell( causalCells, *sJ, rightHand, *sE, cdewJugglers, cdewHands, cdewEntries);
	
	sync = (pl->length != absorb && pr->length != absorb );
	
	// in displayMode == singleLineMode,
	// 1. if the user makes very clear that he wants something different, do it
	// 2. a. take the hand already defined or marked (and shown as cell)
	//        b. if in sync, do not disable center of cell
	// 3. take the opposite of the last hand
	// 4. if no hand is defined, calc hand from pattern start (rl order)
	//-------------------
	// 5. if neccessary, fix connections:
	//              get cells that point to this beat
	//              if it points to an absorbing cell and the sync cell is not absorbing,
	//              switch destination hand of throwing cell
	if(displayMode == singleLineMode || displayMode == colorLineMode)
	{
		float cx, cy;
		calcCenterOfCell(*sJ, *sH, *sE, &cx, &cy);
		/*
		 // 6.   // if there is a cell that points to this moment, but to the other hand,
		 // then the actual user input has higher priority
		 if( pl->length == absorb && pr->length == absorb )
		 {
             _causalCell *pSL, *pSR;
             pSL =  getCellToCell(pl );
             pSR =  getCellToCell(pr );
			 
             // only one incomming arrow
             //
             if( pSL->length != pSR->length && ( pSL->length == absorb ||ÔøΩpSR->length == absorb ))
             {
				 if( pSL->length != absorb )
				 {
					 
					 destHand == leftHand && pl->length == absorb )
pSL->destHand = rightHand;
else if( pSL->destHand == rightHand && pl->length == absorb )
pSL->destHand = leftHand;
             }else
if( pSR->length != absorb && pSL->length == absorb )
{
	if( pSR->destHand == leftHand && pl->length == absorb )
		pSR->destHand = rightHand;
	else if( pSR->destHand == rightHand && pl->length == absorb )
		pSR->destHand = leftHand;
}



        }
*/
		// diagram may not contain hurries
		if( unhurriedDiagram )
		{
			// allow if it is allready a hurry (maybe user wants to remove it)
			if( p->length == absorb )
			{
				// is hurried
				if( pPrev->length != absorb )
				{
					// there is an unhurried solution
					if(pPrevSync->length == absorb )
						// use it
						*sH = switchHand( *sH );
					
					return;
				}
			}
		}
		
		// 1. -------------------------------------------------------------
		// clear input:
		// take it.
		
		// if theres a cell pointing to the opposite cell, change its destination
		if( fabs( (windowHeight - mouseY) - cy) > lineHeight/4.)
		{
			return;
		}
		
		// 2. -------------------------------------------------------------
		if(     !sync)
		{
			
			wwwBoolean cellDefined = false;
			
			// a.
			if( pl->length == absorb && pr->length != absorb )
			{       *sH = rightHand;
				cellDefined = true;
			}
			else if( pr->length == absorb && pl->length != absorb )
			{       *sH = leftHand;
				cellDefined = true;
			}else if( pl->marked && !pr->marked )
			{       *sH = leftHand;
				cellDefined = true;
			}else if( pr->marked && !pl->marked )
			{       *sH = rightHand;
				cellDefined = true;
			}
			
			// b.
			if( ! cellDefined )
				if( fabs( (windowHeight - mouseY) - cy) < 10 )
				{       _causalCell *l;
					wwwBoolean cellFound;
					
					cellFound = false;
					// 3. -------------------------------------------------------------
					for( e = 1; e < cdewEntries - 1; e++)
					{       l = getCausalCell( causalCells, *sJ, leftHand, *sE - e, cdewJugglers, cdewHands, cdewEntries);
						if( l->length != absorb )
						{       *sH = rightHand;
							cellFound = true;
							break;
						}
						l = getCausalCell( causalCells, *sJ, rightHand, *sE - e, cdewJugglers, cdewHands, cdewEntries);
						if( l->length != absorb )
						{       *sH = leftHand;
							cellFound = true;
							break;
						}
					}
					// 4. -------------------------------------------------------------
					if(! cellFound )
					{
						*sH = *sE%2?leftHand:rightHand;
					}
					
				}
					
		}
			// 5. -------------------------------------------------------------
			// 5. if neccessary, fix connections:
			//              get cells that point to this beat
			//              if it points to an absorbing cell and the sync cell is not absorbing,
			//              switch destination hand of throwing cell
			//      _fixConnections( *sJ, *sE );
			
			// only one cell defined, connections will be fixed on nouseUp
			// take the used cell
			
			/*        if( pl->length == absorb && pr->length == absorb )
			{
				_causalCell *pSL, *pSR;
				pSL =  getCellToCell(pl );
				pSR =  getCellToCell(pr );
				
				if( pSL != absorb && pSR == absorb )
				{
					if( pSL->destHand == leftHand && pl->length != absor
						
             }


        }
*/
        }
}


wwwBoolean oglwin::drawAsBow( _causalCell *ps )
{
	_causalCell *pd, *pd2, *pt;
	int j,h,e, e2; 
	int bow = true;
	float centerXOld, centerYOld, centerXNew, centerYNew,  centerXT, centerYT;
	
	if( ps->length == absorb )
		return false;
	
	// allow user to change straight arrows to bows
	if(  fabs(ps->factor)  )
		return true;
	
	
	allCells(       if( (pt = getCausalCell(causalCells, jAll, hAll,eAll, cdewJugglers, cdewHands, cdewEntries)) == ps )
					{       j = jAll;
		h = hAll;
		e = eAll;
					}
					)
        if( christophesNotation && ps->c1x)
			return false;
	
	e2 = ps->length+e;
	pd = getCausalCell(causalCells, ps->destJuggler, ps->destHand, e2, cdewJugglers, cdewHands, cdewEntries);
	pd2 = getCausalCell(causalCells, pd->destJuggler, pd->destHand, pd->length+e2, cdewJugglers, cdewHands, cdewEntries);
	
	_calcCenterOfCell(  j,  h,  e, &centerXOld, &centerYOld);
	_calcCenterOfCell(  ps->destJuggler, ps->destHand, ps->length+e, &centerXNew, &centerYNew);
	// arrows of cells are a closed loop
	// the backwards pointing arrow is drawn as bow
	// not if cells are on different height
	if( pd->length != absorb )
	{       if( ps == pd2 )
	{
		if( ps->length < 0 )
			if( centerYOld == centerYNew )
				return true;
			else
				return false;
	}
	}
	if( ps->length == 0 )
	{       if( ps == pd )
		bow = true;
		else
			bow = false;
	}else
	{
		// goes to same screen position
		// maybe its a bow. Check if a cell is used or marked is in between source and destination cell
		if( centerYOld == centerYNew )
		{       int j1, h1, e1;
			int j2, h2, e2;
			
			bow = false;
			// check if any cell in between ps and pd is used
			// forward throws
			if( ps->length > 0 )
			{
				for( j1 = 0; j1 <cdewJugglers; j1++)
				{
					for( h1 = 0; h1 < cdewHands;h1++)
					{
						for( e1 = e+1; e1 < e+ps->length; e1++)
						{
							pt = getCausalCell(causalCells, j1, h1,e1, cdewJugglers, cdewHands, cdewEntries) ;
							if( displayMode == syncLineMode && h1 != h || ( pt->length == absorb && !pt->marked && !pt->notOutgoing) )
								continue;
							_calcCenterOfCell(  j1,  h1,  e1, &centerXT, &centerYT);
							if(pt->length != absorb && centerXT > centerXOld && centerXT < centerXNew && centerYOld == centerYT)
								bow = true;
						}
					}
				}
				
			}
			// backward throws
			else if( ps->length > absorb && ps->length < 0 )
			{
				for( j2 = 0; j2 <cdewJugglers; j2++)
				{
					for( h2 = 0; h2 < cdewHands;h2++)
					{
						for( e2 = e + ps->length+1; e2 <= e; e2++)
						{
							pt = getCausalCell(causalCells, j2, h2,e2, cdewJugglers, cdewHands, cdewEntries) ;
							if( !displayMode == singleLineMode && h2 != h || ( pt->length == absorb && !pt->marked && !pt->notOutgoing) )
								continue;
							_calcCenterOfCell(  j2,  h2,  e2, &centerXT, &centerYT);
							if(pt->length != absorb && centerXT < centerXOld && centerXT >= centerXNew && centerYOld == centerYT )
								bow = true;
						}
					}
				}
				
			}
		}else
			bow= false;
	}
	
	return bow;
}
/*****
#pragma mark ---------------------user input
*****/
// ------------------------------- input stuff ------------------------------------
// ------------ fileIO helpers
// new: classical pattern
// print as few informations as possible
// (only integers, no e,@...)
// ------------ fileIO helpers
void oglwin::clearDiagram( void )
{
	sprintf(cdewFileName, "%s", "untitled.pass");
	setCdewWindowTitle( cdewFileName);
	addToUndoList();
	initCL();
	setSignature(ownerSignature);
}

wwwBoolean oglwin::readInputFile( char *name)
{
	char *p;
	wwwBoolean ret= false;
	
	p = readFile( name, "r" );
	//_myPrintf("read file %s\n", name);
	
	if( p )
	{
		ret = _readDiagram(p );
		free( p );
		if( ret )
			setSaveFullFileName( name );
		else
		{
			initPattern();
			clearDiagram( );
		}
	}else
	{
		return false;
	}
	return ret;
}



void oglwin::changeCellContentByDrag( void )
{       _causalCell *p;
	int l;
	int minValue;
	// fill in actual values
	p = getCausalCell( causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
	l = destE - startE;
	
	if( (destH != startH || destJ != startJ) && l < -1)
		l = absorb;
	minValue = christophesNotation? -1:-2;
	if(  l < minValue || mouseY < 0 || mouseY > windowHeight )
	{
		p->length = absorb;
		return;
		
	}

	p->length = l;
	p->destJuggler = destJ;
	p->destHand = destH;
}

void oglwin::switchDisplayMode( void )
{
	displayMode ++;
	if( displayMode == maxDisplayMode )
		displayMode = 0;
	
}
// add 2nd diagram
// fill 2nd half with inverted pattern
void oglwin::invertDiagram( void )
{
	int jc = cdewJugglers;
	int c;
	wwwBoolean tempUndoState;
	int j,h,e;
	int lj, lh, le;
	_causalCell *p, *last, *n;
	
	addToUndoList();
	tempUndoState = oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	
	for( c = 0; c < jc; c++ )
		addJuggler();
	for( j= 0; j < jc; j++ )
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h < 2; h++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				last = getCellToCell( p );
				if( last )
				{       getIndicesFromCellPointer( last, &lj, &lh, &le);
					n = getCausalCell( causalCells, j+jc, h, cdewEntries -( e + 1), cdewJugglers, cdewHands, cdewEntries);
					*n = *last;
					
					n->destJuggler = lj+jc;
					
					if( h != lh)
						n->destHand = switchHand( n->destHand);
				}
			}
		}
	}
	
	autoStartRestartFlag = fileChangedFlag = true;
	
	oglwinUndoState = tempUndoState;
	
}

// auotmatic pattern coloration (mode == true), remove color (mode = false)
void oglwin::colorPattern( bool mode )
{
	_causalCell *p,*s,*d;
	_causalCell *p1;
	int j, h, e;
	int col = 0;
	int cmp;
	
	wwwBoolean tempUndoState;
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	
	cmp = cdewColorMode;
	cdewColorMode = LengthColorMode;
	
	addToUndoList();
	tempUndoState = oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	// save marks
	// always remove color 
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( p, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					s->autoColor = NO_COLOR;
					*d = *s;
					)
		// color pattern only if requested
		if(mode){
			for(e = 0; e < cdewEntries; e++ ){
				for(j = 0; j < cdewJugglers;j++ ){
					for(h = 0; h < cdewHands; h++ ){
						
						p1 = getCausalCell(causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries) ;
						if(p1->length != absorb && p1->autoColor == NO_COLOR){
							unmarkAll();
							p1->marked = true;
							markPath();
							col += 1;
							if( col == MAX_COLOR )
								col = 0;
							
							setMarkedArrowColor(causalCells, col, autoColorMode );
						}
					}
				}
			}
        }
	
	// restore marks
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( p, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					s->marked = d->marked;
					)
        free( p );
	oglwinUndoState = tempUndoState;
	cdewColorMode = cmp;
}


void oglwin::setDefaultHand( void )
{
	_causalCell *p;
	
	addToUndoList();
	
	allCells(       p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					if( eAll%2 != hAll)
					if( p->length == absorb )
					{       p->length = defaultThrow;
						p->destHand = p->length%2?switchHand(hAll):hAll;
						p->destJuggler = jAll;
						
					}
					
					)
        autoStartRestartFlag = fileChangedFlag = true;
}



// -------------------------------------- marked cell stuff

void oglwin::setMarkedArrowColor(_causalCell *cellList, int i, int mode )
{
	int j,h,e;
	_causalCell *p;
	
	if( i < -1 )
		i = NO_COLOR;
	
	addToUndoList();
	fileChangedFlag = true;	
	
	for( j = 0; j < cdewJugglers; j++ ){
		for( h = 0; h < cdewHands; h++ ){
			for( e = 0; e < cdewEntries; e++ ){
				p = getCausalCell( cellList, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				
				
				if( mode == allColorMode) 	
					p->userDefArrowColor = p->autoColor = i;
				else if( p->marked ){
					if( mode == userColorMode )
						p->userDefArrowColor = i;
					if( mode == markedColorMode)
						p->userDefArrowColor = p->autoColor = i;	
					if( mode == autoColorMode )
						p->autoColor = i;
					
				}				
			}
		}
	}
	
}

// set color index for background color 
// negative values => use default color
void oglwin::setMarkedBackgroundColor(_causalCell *cellList, int i)
{
	int j,h,e;
	_causalCell *p;
	
	if( i < -1 )
		i = NO_COLOR;

	addToUndoList();
	fileChangedFlag = true;	
	
	for( j = 0; j < cdewJugglers; j++ ){
		for( h = 0; h < cdewHands; h++ ){
			for( e = 0; e < cdewEntries; e++ ){
				p = getCausalCell( cellList, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					p->userDefBackgroundColor = i;
			}
		}
	}	
}

void oglwin::nextThickness( void )
{
	int j,h,e;
	_causalCell *p;
	
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					p->width = (p->width+1)%5;
			}
		}
	}
}



void oglwin::nextColor( void )
{
	int j,h,e;
	_causalCell *p;
	
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					p->userDefArrowColor = (p->userDefArrowColor+1) % MAX_COLOR;
			}
		}
	}
	
	cdewColorMode = cdewColorMode;
}

void oglwin::arrowDefault( void )
{
	int j,h,e;
	_causalCell *p;
	
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{  
					p->userDefArrowColor = NO_COLOR;
					p->width = 1;
				}
				
			}
		}
	}
	
	cdewColorMode = LengthColorMode;
}


void oglwin::setMarkedWidth( int w )
{
	int j,h,e;
	_causalCell *p;
	
	addToUndoList();
	fileChangedFlag = true;	
		
	
	if( w < -1 )
		w = -1;
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					p->width = w;
			}
		}
	}
}



void oglwin::markFaults( void )
{
	
	_causalCell *s, *d;
	// unmark all fault settings
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					s->notOutgoing  = false;
					s->notReached   = false;
					s->reached              = 0;
					)
		
        patternHasFaults = false;
	
	// points to vacant cell
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					s->notOutgoing = false;
					)
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						if( s->length != absorb )
						{
							d = getCausalCell( causalCells, s->destJuggler, s->destHand, s->length + eAll, cdewJugglers, cdewHands, cdewEntries);
							if( d->length == absorb )
							{
								d->notOutgoing = true;
								patternHasFaults = true;
							}
						}
						
                        )
		
        // used, but no incomming throw
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						if( s->length == absorb )
						s->notReached = false;
						else
						{
							s->notReached = true;
							patternHasFaults = true;
						}
                        )
		
		
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						if( s->length != absorb )
						{       d = getCausalCell( causalCells, s->destJuggler, s->destHand, s->length + eAll, cdewJugglers, cdewHands, cdewEntries);
							d->notReached = false;
							patternHasFaults = true;
						}
                        )
		
        // howe often is this cell reached?
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						if( s->length != absorb )
						{       d = getCausalCell( causalCells, s->destJuggler, s->destHand, s->length + eAll, cdewJugglers, cdewHands, cdewEntries);
							d->reached++;
							patternHasFaults = true;
						}
                        )
		
}

enum mfMarked { mfUnmarked , mfMarked, mfMarkedAsPath};

// mark all beats in this path
void oglwin::markPath( void )
{       _causalCell *p, *d;
	int j,h,e;
	addToUndoList();
	
	/*	save all cells that have marks 
		walk through all cells:
		if it has a mark, walk through the path
		no following cell -> break
		following cell has attribute mfMarkedAsPath -> break
otherwise:
		mark it with mfMarkedAsPath
		
		
		*/
	allCells(
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 if( p->marked )
			 p->markedHelperFlag = mfMarked;
			 else
			 p->markedHelperFlag = mfUnmarked;
			 
			 )
		
        for( j = 0; j < cdewJugglers; j++ )
        {
			for( h = 0; h < cdewHands; h++ )
			{
				for( e = 0; e < cdewEntries; e++ )
				{
					p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					if( p->marked )
					{
						d = p;
						while( d && d->length != absorb && d->markedHelperFlag != mfMarkedAsPath){
							
#ifdef TARGET_OS_MAC
							if( Button() )
								return;
#endif
							d->markedHelperFlag = mfMarkedAsPath;
							d = getDestinationCell( d );
						}
					}
				}
			}
        }
	
	allCells(
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 if( p->markedHelperFlag != mfUnmarked)
			 p->marked = marked;
			 )
}


// mark all used throws done at this beat
void oglwin::markBeat( void )
{
	int j,h,e;
	int j1, h1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( j1 = 0; j1 < cdewJugglers; j1++ )
					{
						for( h1 = 0; h1 < cdewHands; h1++ )
						{
							d = getCausalCell( causalCells, j1, h1, e, cdewJugglers, cdewHands, cdewEntries);
							if( d->length != absorb )
								d->marked = true;
						}
					}
				}
			}
		}
	}
}
// mark all used throws done by this juggler
void oglwin::markJuggler( void )
{
	int j,h,e;
	int h1, e1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( e1 = 0; e1 < cdewEntries; e1++ )
					{
						for( h1 = 0; h1 < cdewHands; h1++ )
						{
							d = getCausalCell( causalCells, j, h1, e1, cdewJugglers, cdewHands, cdewEntries);
							if( d->length != absorb )
								d->marked = true;
						}
					}
				}
			}
		}
	}
}

// mark all used throws done by the same hand of the same juggler
void oglwin::markJugglersHand( void )
{
	int j,h,e;
	int e1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( e1 = 0; e1 < cdewEntries; e1++ )
					{
						d = getCausalCell( causalCells, j, h, e1, cdewJugglers, cdewHands, cdewEntries);
						if( d->length != absorb )
							d->marked = true;
					}
				}
			}
		}
	}
}
// mark all beats on the rightHand siede of a marked beat
void oglwin::markRight( void )
{       _causalCell *p, *d;
	int j1,h1,e1;
	int h2,e2;
	addToUndoList();
	for( j1 = 0; j1 < cdewJugglers; j1++ )
	{
		for( h1 = 0; h1 < cdewHands; h1++ )
		{
			for( e1 = 0; e1 < cdewEntries; e1++ )
			{
				p = getCausalCell( causalCells, j1, h1, e1, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( h2 = 0; h2 < cdewHands; h2++ )
					{
						for( e2 = e1+1; e2 < cdewEntries; e2++ )
						{       d = getCausalCell( causalCells, j1, h2, e2, cdewJugglers, cdewHands, cdewEntries);
							if( d->length != absorb )
								d->marked = true;
							
						}
						
					}
				}
			}
		}
	}
}


// mark all beats in this path
void oglwin::unmarkPath( void )
{       _causalCell *p, *d;
	int j,h,e;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for(d = p; ;)
					{
						if( d->length == absorb )
							break;
						
						e += d->length;
						d = getCausalCell( causalCells, d->destJuggler, d->destHand, e, cdewJugglers, cdewHands, cdewEntries);
						if( d == p )
							break;
						d->marked = false;
						
					}
					p->marked = false;
				}
			}
		}
	}
}
// mark all used throws done at this beat
void oglwin::unmarkBeat( void )
{
	int j,h,e;
	int j1, h1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( j1 = 0; j1 < cdewJugglers; j1++ )
					{
						for( h1 = 0; h1 < cdewHands; h1++ )
						{
							d = getCausalCell( causalCells, j1, h1, e, cdewJugglers, cdewHands, cdewEntries);
							d->marked = false;
						}
					}
				}
			}
		}
	}
}
// mark all used throws done by this juggler
void oglwin::unmarkJuggler( void )
{
	int j,h,e;
	int h1, e1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( e1 = 0; e1 < cdewEntries; e1++ )
					{
						for( h1 = 0; h1 < cdewHands; h1++ )
						{
							d = getCausalCell( causalCells, j, h1, e1, cdewJugglers, cdewHands, cdewEntries);
							d->marked = false;
						}
					}
				}
			}
		}
	}
}

// mark all used throws done by the same hand of the same juggler
void oglwin::unmarkJugglersHand( void )
{
	int j,h,e;
	int e1;
	_causalCell *p, *d;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
				{
					for( e1 = 0; e1 < cdewEntries; e1++ )
					{
						d = getCausalCell( causalCells, j, h, e1, cdewJugglers, cdewHands, cdewEntries);
						d->marked = false;
					}
				}
			}
		}
	}
}
// invertMarked all marks
void oglwin::invertMarked( void )
{
	int j,h,e;
	_causalCell *p;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if(p->length != absorb )
					toggle(p->marked);
			}
		}
	}
}
// remove all marks
void oglwin::unmarkAll( void )
{
	int j,h,e;
	_causalCell *p;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				p->marked = false;
			}
		}
	}
}
// -------------------------------------- causal editor functions
void oglwin::addJuggler( void )
{
	_causalCell *p, *s, *d;
	int j,hnd,e;
	float *dl;
	
	int w,h;
	
	getActualCellSize(&w, &h);
	
	// get new / bigger buffer
	p = allocCausalCells( cdewJugglers+1, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	
	// initialize it
	for( j = 0; j < cdewJugglers+1; j++ )
		for( hnd = 0; hnd < cdewHands; hnd++)
			for( e = 0; e < cdewEntries; e++ )
				initCC( p,  j, hnd, e ) ;
	
	// ok, add old pattern to undo list
	addToUndoList();
	
	// copy contens
	for( j = 0; j < cdewJugglers; j++ ){
		for( e = 0; e < cdewEntries; e++ ){
			s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, leftHand, e, cdewJugglers+1, cdewHands, cdewEntries);
			*d = *s;
			
			s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, rightHand, e, cdewJugglers+1, cdewHands, cdewEntries);
			*d = *s;
		}
	}
	
	// initialize new juggler in diagram
	cdewJugglers++;
	for( hnd = 0; hnd < cdewHands; hnd++)
		for( e = 0; e < cdewEntries; e++ )
			initCC( p, cdewJugglers-1 ,hnd, e);
	
	// swap buffers
	free(causalCells);
	causalCells = p;
	
	// make new delay list
	delayableJugglers = cdewJugglers;
	dl = allocDelayList( cdewJugglers);
	for( j = 0; j < cdewJugglers-1; j++ ){	
		int h;
		for( h = 0; h < 2; h++ )
			*(dl + 2 * j + h) = *( jugglerDelayList + 2 * j + h);
	}
	
	// swap delay buffers
	if( jugglerDelayList != NULL)
		free(jugglerDelayList);
	jugglerDelayList = dl;
	
	setActualCellSize(&w,&h);
	autoStartRestartFlag = fileChangedFlag = true;
}

void oglwin::removeJuggler( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	float *dl;
	int w,h;
	getActualCellSize( &w, &h);
	
	
	if( cdewJugglers == 1 )
		return;
	
	p = allocCausalCells( cdewJugglers-1, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	
	addToUndoList();
	
	for( j = 0; j < cdewJugglers-1; j++ ){
		for( e = 0; e < cdewEntries; e++ ){
			s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, leftHand, e, cdewJugglers-1, cdewHands, cdewEntries);
			*d = *s;
			
			s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, rightHand, e, cdewJugglers-1, cdewHands, cdewEntries);
			*d = *s;
		}
	}
	cdewJugglers--;
	free(causalCells);
	causalCells = p;
	
 	// make new delay list
	delayableJugglers = cdewJugglers;
	dl = allocDelayList( cdewJugglers);
	for( j = 0; j < cdewJugglers; j++ ){
		int h;
		for( h = 0; h < 2; h++ )
			*(dl + 2 * j + h) = *( jugglerDelayList + 2 * j + h);		
	}
	
	// swap delay buffers
	if( jugglerDelayList != NULL)
		free(jugglerDelayList);
	jugglerDelayList = dl;
	
	
	setActualCellSize(&w,&h);
	autoStartRestartFlag = fileChangedFlag = true;
}
void oglwin::addThrow( void )
{
	_causalCell *p, *s, *d;
	int j,hnd,e;
	int w,h;
	
	getActualCellSize(&w, &h);
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries+1);
	if( p == NULL )
		return;
	
	for( j = 0; j < cdewJugglers; j++ )
		for( hnd = 0; hnd < cdewHands; hnd++)
			for( e = 0; e < cdewEntries+1; e++ )
				initCC( p,  j, hnd, e ) ;
	
	addToUndoList();
	
	for( j = 0; j < cdewJugglers; j++ ){
		for( e = 0; e < cdewEntries; e++ ){
			s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries+1);
			*d = *s;
			
			s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries+1);
			*d = *s;
		}
	}
	cdewEntries++;
	for( j = 0; j < cdewJugglers; j++ )
		for( hnd = 0; hnd < cdewHands; hnd++)
			initCC( p, j, hnd, cdewEntries-1);
	
	free(causalCells);
	causalCells = p;
	
	setActualCellSize(&w,&h);
	autoStartRestartFlag = fileChangedFlag = true;
	
}



void oglwin::removeThrow( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	int w,h;
	if( cdewEntries == 1 )
		return;
	getActualCellSize(&w, &h);
	
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries-1);
	if( p == NULL )
		return;
	
	addToUndoList();
	
	for( j = 0; j < cdewJugglers; j++ ){
		for( e = 0; e < cdewEntries-1; e++ ){
			s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries-1);
			*d = *s;
			
			s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			d = getCausalCell( p, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries-1);
			*d = *s;
		}
	}
	cdewEntries--;
	free(causalCells);
	causalCells = p;
	setActualCellSize(&w,&h);
	autoStartRestartFlag = fileChangedFlag = true;
}


void oglwin::addDiagram( void )
{
	char *p;
	int w,h;
	
	p = cdewSelectAndLoadFile();
	if( p == NULL )
		return;
	
	getActualCellSize(&w, &h);
	dontChangeActualCellSizeFlag = true;
	
	setCdewWindowTitle( cdewFileName );
	
	windowX = windowY = windowW = windowH = -1;
	_addDiagram(p );
	free( p );
	
	dontChangeActualCellSizeFlag = false;
	if( windowW > 0 && windowH >0 )
		glutReshapeWindow( windowW, windowH);
	else
		setActualCellSize(&w, &h);
	
	
	glutPostRedisplay();
	
}

void oglwin::_addDiagram( char *p )
{
	int j1, h1, e1;
	int j2, h2, e2;
	int jSum, hSum, eSum;
	
	int j, h, e;
	_causalCell *oldP, *newP, *compP;
	_causalCell *s, *d;
	
	int dm, cpm, jl, cc;
	
	addToUndoList();
	
	
	// save old pattern size
	j1 = cdewJugglers;
	h1 = cdewHands;
	e1 = cdewEntries;
	
	// save old pattern
	oldP = copyDiagram( );
	// save settings
	// cdewDisplay Mode
	dm = displayMode;
	
	// color pattern Mode
	cpm = cdewColorMode;
	
	// seperator lines
	jl = jugglerLines;
	// cell color
	cc = cellColor;
	
	
	// read new pattern
	christophesNotation = false;
	readMhnh( p );
	readCdew( p );
	
	newP = causalCells;
	if( newP == NULL )
		return;
	
	// save new pattern size
	j2 = cdewJugglers;
	h2 = cdewHands;
	e2 = cdewEntries;
	// calc composed pattern size
	jSum = j1 + j2;
	hSum = 2;               // xx maybe this will change one day - I dont think so.
	eSum = max(e1, e2);
	// now ask for a buffer that is big enough for both patterns
	compP = allocCausalCells( jSum, hSum, eSum);
	if( compP == NULL )
		return;
	// save new size
	causalCells = compP;
	cdewJugglers    = jSum;
	cdewHands               = hSum;
	cdewEntries             = eSum;
	
	
	// init cells
	for( j = 0; j < jSum; j++ )
		for( h = 0; h < hSum; h++)
			for( e = 0; e < eSum; e++ )
				initCC( compP,  j, h, e);
	
	// add pattern 1
	for( j = 0; j < j1; j++ )
	{
		for( h = 0; h < hSum; h++ )
		{
			for( e = 0; e < e1; e++ )
			{
				s = _getCausalCell( oldP, j, h, e, j1, h1, e1);
				d = _getCausalCell( compP, j, h, e, jSum, hSum, eSum);
				*d = *s;
			}
		}
	}
	// add pattern 2
	for( j = 0; j < j2; j++ )
	{
		for( h = 0; h < hSum; h++ )
		{
			for( e = 0; e < e2; e++ )
			{
				s = _getCausalCell( newP, j, h, e, j2, h2, e2);
				d = _getCausalCell( compP, j+j1, h, e, jSum, hSum, eSum);
				*d = *s;
				// correct offset for cdewJugglers
				d->destJuggler += j1;
			}
		}
	}
	if( jugglerDelayList )
		free( jugglerDelayList );
	jugglerDelayList = allocDelayList( cdewJugglers );
	delayableJugglers = cdewJugglers;
	
	free( oldP );
	free( newP );
	
	// restore settings
	// cdewDisplay Mode
	displayMode = dm;
	
	// color pattern Mode
	cdewColorMode = cpm;
	
	// seperator lines
	jugglerLines = jl;
	// cell color
	cellColor = cc;
	
	autoStartRestartFlag = fileChangedFlag = true;
	
}

void oglwin::mergeDiagram( void )
{
	char *p;
	int w, h;
	
	p = cdewSelectAndLoadFile();
	if( p == NULL )
	{       return;
	}
	getActualCellSize(&w, &h);
	dontChangeActualCellSizeFlag = true;
	
	setCdewWindowTitle( cdewFileName );

	
//	windowX = windowY = windowW = windowH = -1;
	_mergeDiagram(p );
	free( p );
	glutSetWindow( id );
	
	dontChangeActualCellSizeFlag = false;
	if( windowW > 0 && windowH >0 )
		glutReshapeWindow( windowW, windowH);
	else
		setActualCellSize(&w,&h);
	
	
	glutPostRedisplay();
	return;
}
void oglwin::_mergeDiagram( char *p )
{
	int j1, h1, e1;
	int j2, h2, e2;
	int jSum, hSum, eSum;
	
	int j, h, e;
	_causalCell *oldP, *newP, *compP;
	_causalCell *s, *d;
	
	int dm, cpm, jl, cc;
	
	addToUndoList();
	
	
	// save old pattern size
	j1 = cdewJugglers;
	h1 = cdewHands;
	e1 = cdewEntries;
	
	// save old pattern
	oldP = copyDiagram( );
	// save settings
	// cdewDisplay Mode
	dm = displayMode;
	
	// color pattern Mode
	cpm = cdewColorMode;
	
	// seperator lines
	jl = jugglerLines;
	// cell color
	cc = cellColor;
	
	
	// read new pattern
	christophesNotation = false;
	readMhnh( p );
	readCdew( p );
	
	newP = causalCells;
	if( newP == NULL )
		return;
	
	// save new pattern size
	j2 = cdewJugglers;
	h2 = cdewHands;
	e2 = cdewEntries;
	// calc composed pattern size
	jSum = max(j1, j2);
	hSum = 2;
	eSum = e1 + e2;
	// now ask for a buffer that is big enough for both patterns
	compP = allocCausalCells( jSum, hSum, eSum);
	if( compP == NULL )
		return;
	// save new size
	causalCells = compP;
	cdewJugglers    = jSum;
	cdewHands       = hSum;
	cdewEntries     = eSum;
	
	
	// init cells
	for( j = 0; j < jSum; j++ )
		for( h = 0; h < hSum; h++)
			for( e = 0; e < eSum; e++ )
				initCC( compP,  j, h, e);
	
	// add pattern 1
	for( j = 0; j < j1; j++ )
	{
		for( h = 0; h < hSum; h++ )
		{
			for( e = 0; e < e1; e++ )
			{
				s = _getCausalCell( oldP, j, h, e, j1, h1, e1);
				d = _getCausalCell( compP, j, h, e, jSum, hSum, eSum);
				*d = *s;
			}
		}
	}
	// add pattern 2
	for( j = 0; j < j2; j++ )
	{
		for( h = 0; h < hSum; h++ )
		{
			for( e = 0; e < e2; e++ )
			{
				s = _getCausalCell( newP, j, h, e, j2, h2, e2);
				d = _getCausalCell( compP, j, h, e + e1, jSum, hSum, eSum);
				*d = *s;
			}
		}
	}
	if( jugglerDelayList )
		free( jugglerDelayList );
	jugglerDelayList = allocDelayList( cdewJugglers );
	delayableJugglers = cdewJugglers;
	
	free( oldP );
	free( newP );
	
	// restore settings
	// cdewDisplay Mode
	displayMode = dm;
	
	// color pattern Mode
	cdewColorMode = cpm;
	
	// seperator lines
	jugglerLines = jl;
	// cell color
	cellColor = cc;
	
	
}



void oglwin::exchangeMarkedHands( void )
{
	int j,e;
	_causalCell *pl, *pr, pt;
	
	 
	addToUndoList();
	
	// transfer markers to dot flag (unused)
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					p->markedHelperFlag = p->marked;
					)
		
        for( j = 0; j < cdewJugglers; j++ )
        {
			for( e = 0; e < cdewEntries; e++)
			{	bool isSync;
				
				pl = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries );
				pr = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries );
				
				// sync throws if
				// both throws used and at least one marked
				isSync = (pl->length != absorb && pr->length != absorb ) && (pl->markedHelperFlag || pr->markedHelperFlag);
				// change handedness of marked cells
				if( isSync ){
					_causalCell *tl, *tr;
					
					
					// get cells to this cell
					tl = getCellToCell( pl );
					tr = getCellToCell( pr );
					
					// switch cell content
					pt = *pr;
					*pr = *pl;
					*pl = pt;
					
					// switch destination of outgoing throws
					pl->destHand = switchHand( pl->destHand );
					pr->destHand = switchHand( pr->destHand );
					
					// switch desitnation hand of incomming throw2
					// not, if length is zero, as this is a zip and has been done already above
					if( tl && tl->length != 0 )
						tl->destHand = switchHand( tl->destHand );
					if( tr && tr->length != 0 )
						tr->destHand = switchHand( tr->destHand );
					pl->markedHelperFlag = pr->markedHelperFlag = false;
				}else{
					if( pl->markedHelperFlag){
						_causalCell t, *p;
						if( (p = getCellToCell( pl)) != NULL )
							p->destHand = switchHand( p->destHand);	// throw to the other hand
						pl->markedHelperFlag = false;
						t = *pl;
						*pl = *pr;
						*pr = t;
					}
					if( pr->markedHelperFlag){
						_causalCell t, *p;
						if( (p = getCellToCell( pr)) != NULL )
							p->destHand = switchHand( p->destHand);	// throw to the other hand
						pr->markedHelperFlag = false;
						t = *pl;
						*pl = *pr;
						*pr = t;
					}
					
				}
			}
        }
	autoStartRestartFlag = fileChangedFlag = true;
	
}


void oglwin::setMarkedHands( int hand )
{
	int j, e;
	_causalCell *pl, *pr;
	
	
	addToUndoList();
	
	
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					p->dot = -1;
					)
		
        for( j = 0; j < cdewJugglers; j++ )
        {
			for( e = 0; e < cdewEntries; e++)
			{
				pl = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries );
				pr = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries );
				// change handedness of marked cells
				if( pl->marked && pr->marked )
				{
					// dont work on sync cells
				}else
				{
					if(hand == rightHand && pl->marked )
					{
						*pr = *pl;
						pl->marked = false;
						pl->length = absorb;
						fixConnections( j, e);
					}else if(hand == leftHand && pr->marked)
					{
						*pl = *pr;
						pr->marked = false;
						pr->length = absorb;
						fixConnections( j, e);
					}
				}
			}
        }
	autoStartRestartFlag = fileChangedFlag = true;
}
void oglwin::cyclePatternLeft( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{       for( e = 0; e < cdewEntries; e++ )
	{       s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, j, leftHand, e-1, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		
		s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, j, rightHand, e-1, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
	}
	}
	free( causalCells );
	causalCells = p;
	autoStartRestartFlag = fileChangedFlag = true;
}
void oglwin::cyclePatternRight( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{       for( e = 0; e < cdewEntries; e++ )
	{       s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, j, leftHand, e+1, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		
		s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, j, rightHand, e+1, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
	}
	}
	free( causalCells );
	causalCells = p;
	autoStartRestartFlag = fileChangedFlag = true;
}
void oglwin::cyclePatternUp( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	int jd;
	float *dl;
	
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{       for( e = 0; e < cdewEntries; e++ )
	{
		jd = j-1;
		if( jd < 0)
			jd += cdewJugglers;
		
		s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, jd, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		if( d->destJuggler == 0)
			d->destJuggler = cdewJugglers-1 ;
		else
			d->destJuggler--;
		
		s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, jd, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		if( d->destJuggler == 0)
			d->destJuggler = cdewJugglers-1 ;
		else
			d->destJuggler--;
	}
	}
	free( causalCells );
	causalCells = p;
	
	
	dl = allocDelayList( cdewJugglers );
	
	if( dl )
	{
		for( j = 0; j < cdewJugglers ; j++)
		{
			int h;
			for( h = 0; h < 2; h++ )
			{
				jd = j - 1;
				if( jd < 0 )
					jd += cdewJugglers;
				*(dl+2*jd+h) = *getDelayedJuggler( j,h );
			}
		}
		free( jugglerDelayList );
		jugglerDelayList = dl;
	}
	
	autoStartRestartFlag = fileChangedFlag = true;
}


void oglwin::cyclePatternDown( void )
{
	_causalCell *p, *s, *d;
	int j,e;
	int jd;
	float *dl;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{       for( e = 0; e < cdewEntries; e++ )
	{
		jd = j+1;
		if( jd >= cdewJugglers)
			jd -= cdewJugglers;
		s = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, jd, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		if( d->destJuggler == cdewJugglers-1 )
			d->destJuggler = 0;
		else
			d->destJuggler++;
		
		s = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( p, jd, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
		if( d->destJuggler == cdewJugglers-1 )
			d->destJuggler = 0;
		else
			d->destJuggler++;
	}
	}
	free( causalCells );
	causalCells = p;
	
	dl = allocDelayList( cdewJugglers );
	
	if( dl )
	{
		for( j = 0; j < cdewJugglers ; j++)
		{
			int h;
			
			for( h = 0; h < 2; h++ )
			{
				jd = j + 1;
				if( jd >= cdewJugglers )
					jd -= cdewJugglers;
				*(dl+2*jd+h) = *getDelayedJuggler( j,h );
			}
		}
		free( jugglerDelayList );
		jugglerDelayList = dl;
	}
	
	autoStartRestartFlag = fileChangedFlag = true;
}





void oglwin::rotateDiagram( void )
{
	_causalCell  *s, *d, *p;
	int j,h;
	float dTmp;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	
	addToUndoList();
	
	// rotate throws
	allCells(
			 s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 
			 j = jAll;
			 h = hAll;
			 rotatePosition(&j, &h);
			 
			 d = getCausalCell( p, j, h, eAll, cdewJugglers, cdewHands, cdewEntries);
			 
			 *d = *s;
			 rotatePosition( &d->destJuggler, &d->destHand);
			 
			 )
		
		// rotate delays
		dTmp = *getDelayedJuggler( 0,0);
	
	for( j = 0; j < cdewJugglers-1; j++)
	{
		*getDelayedJuggler( j,0) = *getDelayedJuggler( j,1);
		*getDelayedJuggler( j,1) = *getDelayedJuggler( j+1,0);
	}
	*getDelayedJuggler( cdewJugglers-1, 0) = *getDelayedJuggler( cdewJugglers-1, 1);
	*getDelayedJuggler( cdewJugglers-1, 1) = dTmp;
	
	free(causalCells);
	causalCells = p;
	
	autoStartRestartFlag = fileChangedFlag = true;
	
}

void oglwin::rotatePosition( int *j, int *h)
{
	if( *h == leftHand )
		*h = rightHand;
	else
	{
		*h = leftHand;
		*j = (*j + 1) % cdewJugglers;
	}
}

void oglwin::showMarkedWhenEmpty( wwwBoolean v )
{
	int j,h,e;
	_causalCell *p;
	
	addToUndoList();
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					p->showIfEmpty = v;
			}
		}
	}
	
}



/*****
#pragma mark ---------------------siteswap input
*****/
int oglwin::getSiteswapInputHand( void )
{
	_causalCell *p, *p2;
	int hnd;
	
	if(displayMode == syncLineMode )
		return siteswapInputHand;
	
	hnd      = !(siteswapInputEntry%2);
	if( siteswapInputForceOtherHand )
		hnd = switchHand(hnd);
	p  = getCausalCell( causalCells, siteswapInputJuggler, siteswapInputHand, siteswapInputEntry, cdewJugglers, cdewHands, cdewEntries );
	p2 = getCausalCell( causalCells, siteswapInputJuggler, switchHand(siteswapInputHand), siteswapInputEntry, cdewJugglers, cdewHands, cdewEntries );
	
	/*      if( !(p->length == absorb && p2->length != absorb ))
		hnd = switchHand(hnd);
	
	*/
	siteswapInputHand = hnd;
	return hnd;
}
// this routine reads std siteswaps into the editor.
// only digits and and passing tags
// no hurries
wwwBoolean oglwin::readSiteswap( char *p )
{
	int j,e;
	char temp[2] = { 0,0 };
	_causalCell *cell;
	wwwBoolean numberFound = false;
	int maxE;
	char *t;
	
	char *end = strstr(p, cdewVersionString), tmp;
	
	
	// no multiplex
	if( strchr( p, '[')     )
		return false;
	
	// no sync
	if( strchr( p, '(' ))
		return false;
	
	//dont read cdew description
	if( end)
	{
		tmp = *end;
		*end = '\0';
	}
	
	if( !calculateSizeOfPlainSiteswap(p, &j, &e) )
		return false;
	
	
	
	// get memory
	cdewJugglers = j;
	cdewHands = 2;
	cdewEntries = e;
	
	if( cdewJugglers == 0 || cdewHands == 0 || cdewEntries == 0)
		return false;
	
	initCL();
	/*      if( jugglerDelayList )
		free( jugglerDelayList );
	jugglerDelayList = allocDelayList( cdewJugglers );
	delayableJugglers = cdewJugglers;
	*/
	
	// read content
	j = 0;
	e = 0;
	maxE = 0;
	
	
	// read delays
	t = p;
	
	if( (t = strstr(t, "#d")) != NULL )
	{
		int dj, dh = 2;
		float v;
		
		while( !isspace( *t ) )
			t++;
		while( isspace( *t ) )
			t++;
		
		dj = atoi( t ) - 1;
		
		while( !isspace( *t ) )
			t++;
		while( isspace( *t ) )
			t++;
		
		if( *t == 'r' )
		{
			dh = 0;
			t++;
			
		}
		if( *t == 'l' )
		{
			dh = 1;
			t++;
		}
		
		while( isspace( *t ) )
			t++;
		
		v = atof( t );
		
		if( dh == 2 )
		{
			*getDelayedJuggler( dj, 0 ) = v;
			*getDelayedJuggler( dj, 1 ) = v;
		}
		else
			*getDelayedJuggler( dj, dh ) = v;
		
		
		
	}
	
	// get line after last command
	t = strrchr( p, '#');
	if( t )
	{
		while( *t && *t != '\r'  && *t != '\n' )
			t++;
		
		p = t;
	}
	
	
	do
	{
		// skip whistespace
		while( isspace( *p ) )
			p++;
		
		// skip line comments
		if( *p == '!' )
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
			maxE = e;
		}
		
		if( *p == '|' )
		{       j++;
			e = maxE;
		}
		
		numberFound = false;
		if( isdigit( *p ) )
		{       int h = e%2?leftHand:rightHand;
			cell = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
			temp[0] = *p;
			cell->length =  atoi(temp) - 2;
			cell->destHand = cell->length%2? switchHand(h):h;
			numberFound = true;
		}
		if( *p == 'p' )
		{       if( j == 0 )
			cell->destJuggler = 1;
			else
				cell->destJuggler = 0;
		}
		if( *p == ':' )
		{       p++;
			temp[0] = *p;
			cell->destJuggler =     atoi(temp)-1;
		}
		
		if( *p )
			p++;
		if( numberFound )
		{       e++;
		}
		
	}while( *p );
	
	// make pattern symetric
	if( e%2 )
		addSymPattern();
	
	// add rest of pattern
	if( end )
		*end = tmp;
	
	return true;
}
// ------------------------------- glut stuff ------------------------------------
/*****
#pragma mark --------------------- io routines
*****/
//----------------------------- file stuff ----------------------------
wwwBoolean oglwin::readDiagram( void )
{
	char *p;
	int w, h;
	int ret;
	//      char tempPatternTitle[MAX_PATTERN_TITLE];
	int temp;
	
	//      strcpy( tempPatternTitle, thePatternTitle);
	p = cdewSelectAndLoadFile();
	if( p == NULL )
	{       return false;
	}
	
	temp = glutGetWindow();
	glutSetWindow( id);
	
	setSaveFullFileName(cdewPathAndFileName);
	//_myPrintf("ffn = %s \n", fullPatternPathName);
	
	getActualCellSize(&w, &h);
	
	addToUndoList();
	windowX = windowY = windowW = windowH = -1;
	
	ret = _readDiagram(p );
	
	free( p );
	
	
	if(ret )
		setCdewWindowTitle(cdewFileName);
#if !CDEW_STANDALONE
	else
	{       int i;
		
		//              strcpy( thePatternTitle, tempPatternTitle);
		
		i = glutGetWindow();
		glutSetWindow(animationWin);
		setCdewWindowTitle( cdewFileName );
		setSaveFullFileName(cdewPathAndFileName);
		//_myPrintf("ffn = %s \n", fullPatternPathName);
		
		if( i )
			glutSetWindow( i );
		
		
	}
#endif
	{
		int sw = glutGet(GLUT_SCREEN_WIDTH );
		int sh = glutGet(GLUT_SCREEN_HEIGHT );
		
        if( windowW > sw)
		{
			cdewCellWidth = sw / cdewEntries ;
			windowW = sw;
#ifdef VerboseCdewOutput
			printf("recalculating cdew window width\n");
#endif
		}
		
        if( windowH > sh)
		{
			cdewCellHeight = sh / cdewJugglers ;
			windowH = sh;
#ifdef VerboseCdewOutput
			printf("recalculating cdew window height\n");
#endif
		}
		
    }
	
	if( windowW > 0 && windowH >0 )
		glutReshapeWindow( windowW, windowH);
	else
		setActualCellSize(&w,&h);
	
	
	glutPostRedisplay();
	
	glutSetWindow(temp);
	
	return true;
}

wwwBoolean oglwin::_readDiagram( char *p )
{
	wwwBoolean ret = false;
	christophesNotation = false;
	
	// read header ans save it in memory
	clearHeader();
	
	header = readHeaderFromString( p );
	
	ret =  readMhnh( p );
	ret |= readCdew( p );
	
	if( ret == false )
	{
		return false;
	}else
		
        if( cdewJugglers == 0 || cdewEntries == 0 || cdewHands == 0)
        {
			cdewJugglers = DefJugglers;
			cdewHands       = Hands;
			cdewEntries = DefEntries;
			initCL();
			ret = false;
			printf("wrong cdew parameters. deleting pattern\n");
        }
	
	if( ret == false )
		printf("file %s can't be read by causal editor\n", cdewFileName);
	
	// one of the many hacks to make christophes notation work.
	// here: change 2 to 1x if another throw follows. Not interpreted as a hurry
	if( christophesNotation )
		change2to1x();
	
	autoStartRestartFlag = (ret == true);
	
	return ret;
	
}


wwwBoolean oglwin::saveDiagram( void )
{
	return _saveDiagram(patternSavedByUser == false);
	
	
}

wwwBoolean oglwin::saveDiagramAs( void )
{
	wwwBoolean ret;
	
	ret =  _saveDiagram(true );
	
	return ret;
}

wwwBoolean oglwin::_saveDiagram( wwwBoolean callFileSelector )
{
	//_myPrintf("fileChangedFlag = %d\n", fileChangedFlag);
	if( fileChangedFlag)
	{
		char *p;
		
		if(!callFileSelector && cdewPathAndFileName[0] != '\0' )
		{
			char d[1000];
			getcwd( d, 1000);
			
#ifdef VerboseCdewOutput
			printf("saving from cdew\n");
			printf("path = %s\n", d );
			printf("file = %s\n",  cdewPathAndFileName );
#endif
			return saveDiagramWithName( cdewPathAndFileName );
			
		}
		saveDiagramWithName( cdewSelectPath );
		
		
		p = readFile( cdewSelectPath, "r"       );
		if( p ){
			
			wwwBoolean fileChangedFlagBackup = fileChangedFlag;
			wwwBoolean ret = cdewSaveTextFileAs( p, cdewFileName );
			
			if( ret ){
				setCdewWindowTitle( cdewFileName );
				setSaveFullFileName(cdewPathAndFileName);
			}
			free(p);
			
			
			if( ret == false )
			{       fileChangedFlag = fileChangedFlagBackup;
				return false ;
			}else
				patternSavedByUser = true;
			
//			setSaveFullFileName( fullPatternPathName );
			
		}
	}
	
	patternSaved = true;
	fileChangedFlag = false;
	
	return true;
}

wwwBoolean oglwin::_saveDiagramWithDefaultPathName( wwwBoolean callFileSelector, char *defaultPathName )
{
	//_myPrintf("fileChangedFlag = %d\n", fileChangedFlag);
	if( fileChangedFlag)
	{
		char *p;
		
		if(!callFileSelector && cdewPathAndFileName )
		{
			char d[1000];
			getcwd( d, 1000);
			
#ifdef VerboseCdewOutput
			printf("saving from cdew\n");
#endif
			printf("path = %s\n", d );
			printf("file = %s\n",  cdewPathAndFileName );
			return saveDiagramWithName( cdewPathAndFileName );
			
		}
		saveDiagramWithName( defaultPathName );
		
		
		p = readFile( defaultPathName, "r"      );
		if( p )
		{       wwwBoolean fileChangedFlagBackup = fileChangedFlag;
			wwwBoolean ret = cdewSaveTextFileAs( p, cdewFileName );
			
			setCdewWindowTitle( cdewFileName );
			setSaveFullFileName(cdewPathAndFileName);
			
			free(p);
			
			
			if( ret == false )
			{       fileChangedFlag = fileChangedFlagBackup;
				return false ;
			}else
				patternSavedByUser = true;
			
			setSaveFullFileName( cdewPathAndFileName );
			
		}
	}
	
	patternSaved = true;
	fileChangedFlag = false;
	
	return true;
}
void    oglwin::setSaveFullFileName( char *name )
{
	sprintf(cdewPathAndFileName, "%s", name);
	
}

void    oglwin::setPatternSavedByUser( wwwBoolean value)
{
	patternSavedByUser = value;
}

void    setPatternSavedByUser( GLint id, wwwBoolean value)
{
	
	oglwin *p = getOglwinFromID(id);
	p->setPatternSavedByUser( value );
	
}


wwwBoolean	oglwin::saveDiagramWithName( char *name )
{
	FILE *fp;
	
#if 0
	_myPrintf("save as %s\n", name);
	_myPrintf("save as %s\n", thePatternTitle);
	_myPrintf("save as %s\n", fullPatternPathName);
#endif
	
	if( (fp = fopen(name, "w")) == NULL )
		return false;
	
	_saveDiagramWithFP( fp );
	if( fp )
		fclose( fp );
	
	return true;
	
}
void oglwin::_saveDiagramWithFP( FILE *fp )
{
	time_t systime;
	char *now;
	
	
	if( fp == NULL )
		return;
	
	
	hasHandDelay = false;
	
	allCells(       _causalCell *p;
					
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					if( p->length != absorb )
					{
						if( p->delay != 0.f )
							
							hasHandDelay = true;
					}
					)
		
		
        systime = time ( NULL );
	now = ctime(&systime);
	
	fprintf(fp, "! JoePass %s file, created %s", __JOEPASS_VERSION_STRING__, now);
#if 0
	fprintf(fp, "! %s %s"NL, CDEWSIGNATURE, sig );
	fprintf(fp, "! %s %s"NL, CDEWLASTCHANGED, lastChanged );
#endif
	

#if 01
	if( header)
	{
		fprintf(fp, "%s\n", header);
    }
#else
	if( header)
	{
		fprintf(fp, "%s\n", HeaderTag);
		fprintf(fp, "%s\n", header);
		fprintf(fp, "%s\n", EndHeaderTag);
    }
#endif
	
	saveMhnh( fp );
	saveCdew( fp );
	
}

// reads a mhn* file
// no error check is provided. If something is wrong in the mhn* file,
// this routine may crash
wwwBoolean oglwin::readMhnh( char *p )
{       char *t;
	_causalCell *cr, *cl;
	int j;
	int h = 2;
	int e;
	wwwBoolean symFlag;
	char *firstPassTag;
	wwwBoolean forMe = false;
	wwwBoolean ret =false;
	
	
	if( strstr( p, cdewVersionString ) != NULL )
		return false;
	
	// check for mhn* pattern
	if( strstr( p, MhnTag ) )
		forMe = true;
	
	
	//  check for Christophes Notation
	if( strstr( p, CMhnTag )  )
	{       christophesNotation = true;
		forMe = true;
	}
	
	if( !forMe )
	{
		ret= readSiteswap(p);
		return ret;
	}
	
	
	// number of cdewJugglers
	j = 1;
	if( (firstPassTag = t = strchr(p, '<')) != NULL )
	{
		while(t && (t = strchr(t, '|')) != NULL )
		{       t++;
			j++;
		}
	}
	
	// number of throws
	t = p;
	e = 0;
	// and check for symetric tag '%'
	symFlag = false;
	while( *t && *t != '|' )
	{       if( *t == '(' )
	{
		
		/*                      int c = 1;
		
		do{
			t++;
			if( *t == '(' )
				c++;
			if( *t == ')' )
				c--;
		}while( *t && c > 0   );
		*/
		e++;
	}
		if( *t == symTag )
		{       symFlag = true;
		}
		t++;
	}
	
	// release old memory and ask for new
	free( causalCells );
	cdewJugglers = j;
	cdewHands = h;
	cdewEntries = e;
	initCL();
	if( jugglerDelayList )
		free( jugglerDelayList );
	jugglerDelayList = allocDelayList( cdewJugglers );
	delayableJugglers = cdewJugglers;
	
	
	
	/* 
	 if there is a # behind the first pass tag or a { in front of it, I dont handle it here
	 user made additions to the pattern, too many posibilities
	*/
	if( firstPassTag)
	{       if( strchr( p, '#' ) > firstPassTag )
		return  false;
		if( strchr( p, '(' ) < firstPassTag )
			return false;
	}else
	{
		// mhn files may not contain control commands.
		// Set pointer to character behind the last control identifier
		if( (t = strrchr(p, '#')) != NULL )
			p = t+1;
	}
	
	t = p;
	// read mhn* throw descriptions
	for( j = 0; j < cdewJugglers; j++ )
	{       actualReadJuggler = j;
		
		for( e = 0; e < cdewEntries; e++ )
		{
			t = strchr( t, '(' );
			if( t == NULL )
			{       
#ifdef VerboseCdewOutput
				printf("error while reading mhn* file\n");
#endif
				return false;
			}
			cl = getCausalCell(causalCells, j,leftHand,e, cdewJugglers,cdewHands,cdewEntries);
			cr = getCausalCell(causalCells, j,rightHand,e, cdewJugglers,cdewHands,cdewEntries);
			
			t = readMhnhContentFromBuffer(cl, t, leftHand );
			t = readMhnhContentFromBuffer(cr, t, rightHand );
			if( t == NULL )
				goto bail;
			
		}
	}
bail:
        if( symFlag )
			addSymPattern();
	
	return true;
	}
// save diagram in mhn* format
void oglwin::saveMhnh( FILE *fp )
{
	int j, e;
	int numberOfObjects;
	
	fprintf(fp,     "! cdew siteswap output: " NL);
	fprintf(fp,     "#sx" NL);
	
	numberOfObjects = (int )calcNumberOfObjects();
	
	fprintf(fp,"%s" NL, MhnTag);
	if( christophesNotation )
		fprintf(fp,"! %s" NL, CMhnTag);
	
	fprintf(fp,"%s%d" NL, ObjectCountTag, numberOfObjects);
	
	
	
	// print delay mode
	for( j = 0; j < cdewJugglers; j++)
	{
		int h;
		for( h = 0; h < 2; h++ )
		{
			if( *getDelayedJuggler( j,h ) != 0.)
			{
				fprintf(fp, "%s " NL, DelayMode);
				fprintf(fp,"!add hand information here" NL);
				break;
			}
		}
	}
	
	// print delays
	for( j = 0; j < cdewJugglers; j++)
	{
		int h;
		for( h = 0; h < 2; h++ )
		{
			float v;
			v = *getDelayedJuggler( j,h );
			if( v != 0.)
				fprintf(fp, "%s %d %c %f" NL, DelayTag, j+1, h==leftHand?'l':'r', v);
		}
	}
	
	// open passing tag
	if( cdewJugglers > 1 )
		fprintf(fp, "<");
	
	
	// all throws are marked as no short holds
	// I need it when printing mhn values:
	// for me short holds are not hurried
	_causalCell *p;
	allCells(       p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					p->isShortHold = false;
					)
		
		
        // for all cdewJugglers
        for( j = 0; j < cdewJugglers; j++)
        {
			// all of his throws
			for( e = 0; e < cdewEntries; e++ )
			{
				// print content
				fprintf(fp,"( ");
				
				wwwBoolean muxMode = false;
				
				if( muxMode )
				{       fprintf(fp,"[ ");
					printMhnhThrow( fp, 0,leftHand,e);
					fprintf(fp," ");
				}
				printMhnhThrow( fp, j,leftHand,e);
				if( muxMode )
					fprintf(fp,"] ");
				
				fprintf(fp,", ");
				printMhnhThrow( fp, j,rightHand,e);
				fprintf(fp,") ");
			}
			if( j != cdewJugglers-1 )
				fprintf(fp, "|" NL);
        }
	
	// close passing tag
	if( cdewJugglers > 1 )
		fprintf(fp, ">" NL);
	
}


void oglwin::saveCdew( FILE *fp)
{       int j,e;
	int x,y,w,h;
	x = glutGet(GLUT_WINDOW_X);
	y = glutGet(GLUT_WINDOW_Y);
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	fprintf(fp, NL NL NL "\"-------------------");
	fprintf(fp, CDEW_INFORMATION_STRING NL NL);
	fprintf(fp, "%s" NL NL, STARTCDEWSTRING);
	
	fprintf(fp, "%s %s" NL, CDEWSIGNATURE, sig );
	fprintf(fp, "%s %s" NL NL, CDEWLASTCHANGED, lastChanged );
	
	fprintf(fp, "#%s %f" NL NL,  cdewVersionString, cdewVersionNumber);
	fprintf(fp, "%s %d %d %d" NL, CDEWSIZE, cdewJugglers, cdewHands, cdewEntries);
	fprintf(fp, "%s %d" NL, CDEWDISPLAYMODE, displayMode);
	fprintf(fp, "%s %d" NL, CDEWCOLORMODE, cdewColorMode);
	fprintf(fp, "%s %d" NL, CDEWCELLCOLOR, cellColor);
	fprintf(fp, "%s %d" NL, CDEWJUGGLERLINES, jugglerLines);
	fprintf(fp, "%s %d" NL, CDEWHANDDELAY, hasHandDelay);
	fprintf(fp, "%s %d" NL, CDEWSHOWNUMBERS, showNumbers);
	fprintf(fp, "%s %d" NL, CDEWSHOWHURRIES, showHurries);
	fprintf(fp, "%s %d" NL, CDEWHIDECROSSES, showCrosses);
	fprintf(fp, "%s %d" NL, CDEWHURRIESFORBIDDEN, unhurriedDiagram);
	fprintf(fp, "%s %d" NL, CDEWPRINTSITESWAPMODE, printSiteswap);
	
	/*      fprintf(fp, "%s %d" NL, TEMPWINDOWX, x);
	fprintf(fp, "%s %d" NL, TEMPWINDOWY, y);
	fprintf(fp, "%s %d" NL, TEMPWINDOWW, w);
	fprintf(fp, "%s %d" NL, TEMPWINDOWH, h);
	*/
	// print delays
	for( j = 0; j < cdewJugglers; j++)
	{       float v;
		int h;
		
		for( h = 0; h < 2; h++ )
		{
			v = *getDelayedJuggler( j ,h);
			if( v != 0.)
				fprintf(fp, "%s %d %d %f" NL, cde2HandDelayTag, j+1, h, v);
		}
	}
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			printCausalCellToFile( fp, j, leftHand, e );
			printCausalCellToFile( fp, j, rightHand, e );
		}
	}
	fprintf(fp, "end of cdew output" NL);
	fprintf(fp, "-------------------\"\n");
	
}

void oglwin::printMhnhThrow( FILE *fp, int j, int h, int e )
{
	int j2, h2, e2;
	float l;
	_causalCell *p, *p2, *pLast, *pNext;
	int t;
	wwwBoolean cross;
	wwwBoolean frictionalSiteswap;
	wwwBoolean cmhnPrinted;
	wwwBoolean s = false;
	
	
	p = getCausalCell( causalCells, j,h, e, cdewJugglers, cdewHands, cdewEntries);
	
	if( p->length == absorb )
	{       fprintf(fp, "-");
		return;
	}
	
	
	e2 = p->length + e;
	h2 = p->destHand;
	j2 = p->destJuggler;
	
	p2 = getCausalCell(causalCells, j2,h2,e2, cdewJugglers, cdewHands, cdewEntries);
	pLast = getCausalCell( causalCells, j,h, e-1, cdewJugglers, cdewHands, cdewEntries);
	pNext = getCausalCell( causalCells, j,h, e+1, cdewJugglers, cdewHands, cdewEntries);
	

	l = p->length + *getDelayedJuggler(j2,h2) - *getDelayedJuggler( j,h)   + p2->delay - p->delay;
	t = (int )l;
	
	// correction for rounding error
	// throw would reach wrong hand othrwise
	if( l-(int)l >= .99999)
		t++;
	
	frictionalSiteswap = ( t != l );
	cmhnPrinted = false;
	
	if( frictionalSiteswap )
	{
		// print throw value
		if(l < 0 )
		{
			// in ChristophesNotation, a 0 has a length of -1
			if(christophesNotation )
			{       if(p->length == -1 )
			{       if( p->c1x )
				fprintf(fp, "1x");
				else //if(p->length == -1 && p->destJuggler == j && p->destHand == h)
					fprintf(fp, "%.2f",0.);
			}
				if( p->length == 0 )
				{       if( pNext->length != absorb )
				{       fprintf(fp, "1x");
					cmhnPrinted = true;
					p->isShortHold = true;
				}
				}
			}
			
			
			//                      l = max( minTime, l);
			//      _myPrintf("setting cross-flag\n");
			/* for some reason, for causal values < 0 direction must be toggled.
			here the flag is set.
			*/
			s = true;
			if(!cmhnPrinted )
				fprintf(fp, "%.2f",l+2);
			l = max(.2, l+.5);
        }else
		{
			l+= dwellTime;
			l = max( minTime, l );
			fprintf(fp, "%.2f",l+1.5);
		}
	}else
	{
		if(christophesNotation )
		{
			if( p->length == -1 )
			{       if( p->c1x )
				fprintf(fp, "1x");
				else //if( p->length == -1 && p->destJuggler == j && p->destHand == h)
					fprintf(fp, "%d",0);
				cmhnPrinted = true;
			}
			if( p->length == 0 )
			{
				if( pNext->length != absorb )
				{       fprintf(fp, "1x");
					cmhnPrinted = true;
					p->isShortHold = true;
				}
			}
		}
		if( !cmhnPrinted)
			fprintf(fp, "%d", t+2);
	}
	
	// add tags for crosses, passes and hurries here:
	if( p->destJuggler != j )
		if( p->destJuggler < 9 )
			fprintf(fp, ":%d", p->destJuggler+1);
		else
			fprintf(fp, ":%c", p->destJuggler+'A'-9);
	
	cross = false;
	
	if(christophesNotation )
	{
        //xx this is not working
		if( !(christophesNotation && p->length == -1 && p->destJuggler == j && p->destHand == h))
		{       if( (t % 2 && h == p->destHand)  || (!(t%2) && h != p->destHand ))
			cross = true;
		}
	}else
	{
		if( (t % 2 && h == p->destHand)  || (!(t%2) && h != p->destHand ))
			cross = true;
	}
	
	// here we switch direction as mentioned above
	if( s )
	{
        //      _myPrintf("changing cross-value\n");
		toggle(cross);
	}
	
	if( cross )
		fprintf(fp, "x");
	
	
	if( p->beep)
		fprintf(fp, "m");
	
	if( p->bounce)
		fprintf(fp, "b");
	
	
	
	// in ChristophesNotation, a 0 has a length of -1 and is not hurried
	if( !( christophesNotation && p->length == -1) )
	{        if( pLast->length != absorb && !pLast->isShortHold )
		fprintf(fp,"*");
	}
	
	
	if( p->delay != 0. )
		fprintf(fp,"d%.2f", p->delay);
	
	
	if( frictionalSiteswap )
	{       fprintf(fp,     "e%.2f", l);
		if( l >01.f && l < 2.f)
			fprintf(fp,     "@1");
        else
			fprintf(fp,     "@%d", max(0,t) );
	}

	/*
	 // overwrites sin settings made by code right above
	if( p->halfspin){
		float d = l-t;
		float s = (int)l+(d>0?0.5f:-0.5f);
		if( p->destJuggler != j )
			s += 1;
		fprintf(fp, "@%.1f", s);
	}
	*/
	if( p->halfspin){
		float s;
		if( l >01.f && l < 2.f)
			s = 0.5f;
		else
			s = max(0,t)-0.5f;
		if( p->destJuggler != j )
			s += 1;
		fprintf(fp,     "@%f", s );
	}
	
}




char *oglwin::readMhnhContentFromBuffer( _causalCell *c, char *p, int hand )
{       char *end, t;
	char *h;
	
	while( *p == ' ' || *p == '(' || *p == ',')
		p++;
	
	if( *p == '-' )
	{       c->length = absorb;
		end = p;
		goto bail;
	}
	
	// read value
	c->length = atoi(p )-2;
	while(isdigit( *p ))
		p++;
	end = strpbrk( p, ",)" );
	if( end )
	{
		t = *end;
		*end = '\0';
	}
	// pass to
	if( (h = strchr( p, ':' )) != NULL )
		c->destJuggler = atoi( h+1 )-1;
	
	// pass
	if( (h = strchr( p, 'p' )) != NULL )
	{       if (actualReadJuggler <= 1 )
		c->destJuggler = 1 - actualReadJuggler;
	}
	
	// half spin
	if( (h = strchr( p, '@' )) != NULL )
	{  
		c->halfspin = true;
	}
	// default hand
	if(c->length%2 )
	{       if( hand == leftHand)
		c->destHand = rightHand;
		else
			c->destHand = leftHand;
	}else
		c->destHand = hand;
	
	// cross
	if( (h = strchr( p, 'x' )) != NULL )
	{       if( c->destHand == leftHand )
		c->destHand = rightHand;
		else
			c->destHand = leftHand;
	}
	
	
	*end = t;
bail:
        if( *end )
			end++;
	// one of the many changes (i.e _hacks_ that are neccessary to use christophes notation)
	if( christophesNotation )
	{
		
        // change length of 0: -2 -> -1
        if( c->length == -2 )
			c->length = -1;
        // HACK! mark short holds
        else if (c->length == -1 && c->destJuggler == actualReadJuggler && c->destHand == hand )
			c->c1x = true;
		
	}
	return end;
	
}



void oglwin::printCausalCellToFile( FILE *fp, int j, int h, int e )
{
	_causalCell *p;
	
	p = getCausalCell( causalCells, j,h,e, cdewJugglers, cdewHands, cdewEntries);
	
	// file version 1.0 content
	fprintf(fp, "%s %d %d %d %s "
			"%d %d %d %d %d %d %d ",
			CELLNAME, j,h,e,CELLDIVIDER,
			p->length, p->destJuggler, p->destHand, p->userDefArrowColor, p->width, p->dash, p->dot);
	// file version 1.1 additions
	fprintf(fp, "%d ",
			p->marked);
	
	// file version 1.2 additions
	fprintf(fp, "%f ",
			p->delay);
	
	// file version 1.4 additions
	fprintf(fp, "%d ",
			p->invertHandHeightOffset);
	
	// file version 1.5 additions
	fprintf(fp, "%d ",
			p->showIfEmpty);
	
	// file version 1.6 additions
	fprintf(fp, "%f ",
			p->factor);
	
	// file version 1.7 additions
	fprintf(fp, "%d ",
			p->beep);
	fprintf(fp, "%d ",
			p->bounce);
	// file version 1.8 additions
	fprintf(fp, "%d ",
			p->repeatIn);
	fprintf(fp, "%d ",
			p->repeatOut);
	// file version 1.9 additions
	fprintf(fp, "%d ",
			p->userDefBackgroundColor);
	// file version 1.10 additions
	fprintf(fp, "%d ",
			p->halfspin);
	
	// add newline
	fprintf(fp, NL);
}

char *oglwin::readCausalCellFromString( char *p, _causalCell *c, int mj, int mh, int me )
{
	int j,h,e;
	_causalCell *thisCell;
	//------------------------
	//      define wwwBoolean values as integers to read them correct
	//------------------------
	int l, dh, dj, col, w, dot, dash, marked, offset = false, showIfEmpty = false;
	float delay, factor;
	int beepFlag = false, bounce = false, repeatIn = false, repeatOut = false, bcol;
	int halfspin = false;
	
	
	
	sscanf(p, "%d %d %d", &j, &h, &e );
	thisCell = getCausalCell( c, j,h,e, mj,mh,me);
	
	p = strstr( p, CELLDIVIDER);
	if( p )
	{   
		p += strlen( CELLDIVIDER );
		// version 2.0
		if(versionNumber > 1.9f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d %f %d %d %d %d %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty, &factor, &beepFlag, &bounce, &repeatIn, &repeatOut, &bcol, &halfspin );
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			thisCell->factor = factor;
			thisCell->beep = beepFlag;
			thisCell->bounce = bounce;
			thisCell->repeatIn = repeatIn;
			thisCell->repeatOut = repeatOut;
			thisCell->userDefBackgroundColor = bcol;
			thisCell->halfspin = halfspin;
			//_myPrintf("cell( %d %d %d) beep = %d, bounce = %d\n", j, h, e, beepFlag, bounce);
			
		}else 
		// version 1.9
		if(versionNumber > 1.8f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d %f %d %d %d %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty, &factor, &beepFlag, &bounce, &repeatIn, &repeatOut, &bcol);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			thisCell->factor = factor;
			thisCell->beep = beepFlag;
			thisCell->bounce = bounce;
			thisCell->repeatIn = repeatIn;
			thisCell->repeatOut = repeatOut;
			thisCell->userDefBackgroundColor = bcol;
			//_myPrintf("cell( %d %d %d) beep = %d, bounce = %d\n", j, h, e, beepFlag, bounce);
			
		}else 
		// version 1.8
		if(versionNumber > 1.7f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d %f %d %d %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty, &factor, &beepFlag, &bounce, &repeatIn, &repeatOut );
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			thisCell->factor = factor;
			thisCell->beep = beepFlag;
			thisCell->bounce = bounce;
			thisCell->repeatIn = repeatIn;
			thisCell->repeatOut = repeatOut;
			//_myPrintf("cell( %d %d %d) beep = %d, bounce = %d\n", j, h, e, beepFlag, bounce);
			
		}else
		// version 1.7
		if(versionNumber > 1.6f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d %f %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty, &factor, &beepFlag, &bounce);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			thisCell->factor = factor;
			thisCell->beep = beepFlag;
			thisCell->bounce = bounce;
			//_myPrintf("cell( %d %d %d) beep = %d, bounce = %d\n", j, h, e, beepFlag, bounce);
			
		}else
		// version 1.6
		if(versionNumber > 1.5f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d %f", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty, &factor);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			thisCell->factor = factor;
			
		}else
			
		// version 1.5
		if( versionNumber > 1.4f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset, &showIfEmpty);
			//                      thisCell = getCausalCell( c, j,h,e, mj,mh,me);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			//                      thisCell->factor = 0.;
			
		}else
		// version 1.4
		if( versionNumber > 1.3f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay, &offset);
			//                      thisCell = getCausalCell( c, j,h,e, mj,mh,me);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
		}else
		// version 1.2 - 1.3
		if( versionNumber > 1.1f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d %f", &l, &dj, &dh, &col, &w, &dash, &dot, &marked, &delay);
			//                      thisCell = getCausalCell( c, j,h,e, mj,mh,me);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->delay = delay;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
			
		}else 
		if( versionNumber > 1.0f)
		{
			sscanf(p, "%d %d %d %d %d %d %d %d", &l, &dj, &dh, &col, &w, &dash, &dot, &marked);
			//                      thisCell = getCausalCell( c, j,h,e, mj,mh,me);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->marked = marked;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
		}else
		// pre version numbers ( == 1.0)
		if( p )
		{
			//                      p += strlen( CELLDIVIDER );
			sscanf(p, "%d %d %d %d %d %d %d", &l, &dj, &dh, &col, &w, &dash, &dot);
			//                      thisCell = getCausalCell( c, j,h,e, mj,mh,me);
			thisCell->length = l;
			thisCell->destJuggler = mod( dj, mj);
			thisCell->destHand = mod(dh, mh);
			thisCell->userDefArrowColor = col;
			thisCell->width = w;
			thisCell->dash = dash;
			thisCell->dot = dot;
			thisCell->invertHandHeightOffset = offset;
			thisCell->showIfEmpty = showIfEmpty;
		}
	}
	return p;
}


void markCdewPatternSaved( int id )
{
	oglwin *o = getOglwinFromID( id );
	o->fileChangedFlag= false;
}


/*****
#pragma mark --------------------- transfer
*****/
void oglwin::postUserInputAction( void )
{
	
	setAllWindowsUnused();
	redisplayAllCDEWWindows();
	
	imTheLastUsedWindow = true;
	
	setLastChanged( ownerSignature);
	
#if !CDEW_STANDALONE
	{
		if( animationActive )
		{
			if( cdewAutoStart )
				transferToAnimation();
			if( cdewAutoOpen )
				transferToTextEditor();
		}
	}
#endif
}

void oglwin::transferToAnimation( void )
{
#if !CDEW_STANDALONE
	int id;
	
	id = glutGetWindow( );
    if( id<1)
		return;
	
	
	recalcFrameRate();
	
	if( autoStartRestartFlag )
	{
		autoStartRestartFlag = false;
		if( allLoopsClosed() )
		{
			transferToJoePass();
			cdewStartedPattern = true;
		}
		
	}
#endif
}


void oglwin::transferToTextEditor( void )
{
#if !CDEW_STANDALONE
    if( startingUp || disableTransferToTextEditor)
        return;
	
	/*
	 muster mit eigenem Namen abspeichern und diesen als EditorNamen eintragen 
	=> editor name als gültig eintragen
	=> beide Editoren bearbeiten das gleiche File
	*/
	/*
	 causal editor text speichern
	 wenn name von text editor und causal editor gleich
	 => editor inhalt 
	 sonst
	 => text editor schließen, ggf speichern, ggf namen vergeben 
	
	 wenn text editor geschlossen
	 => neuen Editor erzeugen mit file aus causal editor
	 */
	
	saveDiagram( );
	
#if 0
	if( editWin )
		closeTextEditor( );
	
	createTextEditor();
#else
	if( editWin )
		saveTextEditorContent();
#endif
	
	openFileInTextEditor( cdewPathAndFileName );
	

	
#endif
}

#if !CDEW_STANDALONE
void oglwin::transferToJoePass( void )
{
	
	int theWin;
	class oglwin *t = firstCdewWindow;
	
	// get last window
	while( t != NULL)
	{
		t->imTheAnimatedWindow = false;
		t = t->next;
	}
	imTheAnimatedWindow = true;
	glutPostRedisplay();
	
	saveDiagramWithName( cdewStartPath);
	readAndStartPattern( cdewStartPath );
	
	theWin = glutGetWindow( );
	
	char *p = (char *)malloc( strlen( causalContentTitle ) + 20 + 20);
	if( p )
	{
		sprintf(p, "%s%3d", causalContentTitle, theWin);
		setWorldTitle( p );
		
		free( p );
		
	}else
		setWorldTitle( causalContentTitle );
	
}
#endif

/*****
#pragma mark ---------------------undo
*****/
// save pattern to list of undoable files
void initUndo( void )
{
	makeUndoDirectory();
}


void makeUndoDirectory( void )
{
	
	getcwd( curFolder, FSIZE );
	undoFolder = makeAbsoluteFilename( curFolder, tempFolderName);
	createFolder( tempFolderName);
	
}
void removeUndoDirectory( void )
{
	int ret;
	
	
	ret = rmdir(undoFolder);
	
#ifdef VerboseCdewOutput
	printf("removing undo directory ...");
	printResult( ret == 0);
#endif
}

void oglwin::undo( void)
{
	if( undoCounter == 0)
		return;
	if( undoCounter == maxUndo )
	{       // save latest changes
		addToUndoList();
		undoCounter--;
	}
	undoCounter--;
	undoOneAction(undoCounter);
}

void oglwin::redo( void )
{
	if( undoCounter >= maxUndo-1)
		return;
	undoCounter++;
	undoOneAction(undoCounter);
}


void oglwin::addToUndoList( void )
{
	char *n;
	FILE *fp;
		
	if( oglwinUndoState == undoDeactivated)
		return;
	
	getcwd( curFolder, FSIZE );
	chdir( undoFolder );
	
	n = generateUndoFileName( undoCounter );
	
	fp = fopen(n,"w");
	
	if( fp == 0 )
		return;
	
	
	undoCounter++;
	undoCounter = maxUndo = max( undoCounter, maxUndo );
	highestUndoNumber = max(highestUndoNumber, undoCounter);
	
	_saveDiagramWithFP( fp );
	
	fclose( fp );
	
	chdir(curFolder);
	
	saveDiagramWithName( cdewCrashPath );
}
// load one element of undoable list back into ram
void oglwin::undoOneAction( int c )
{
	char *n, *p;
	int w,h;
	
	getActualCellSize(&w, &h);
	
	
	getcwd( curFolder, FSIZE );
	chdir( undoFolder );
	
	n = generateUndoFileName( c );
	
	p = readFile(n,"r");
	
	if( p == NULL )
		return;
	
	
	_readDiagram( p );
	
	free( p );
	setActualCellSize(&w,&h);
	chdir(curFolder);
}
void oglwin::removeUndoFiles( void )
{
	int c;
	char *n;
	
	int r;
#ifdef VerboseCdewOutput
	printf("removing undo files\n");
#endif
	
	getcwd( curFolder, FSIZE );
	chdir( undoFolder );
	
	for( c = 0; c < highestUndoNumber; c++ )
	{
		n = generateUndoFileName( c );
		
		r = remove( n );
		
	}
	if( patternSaved )
		remove( cdewSelectPath );
	
	removePostscriptFile(  );
	
	remove( cdewStartPath);
	chdir(curFolder);
#ifdef VerboseCdewOutput
	printf("removing undo files ... done\n");
#endif
	
}
// always get the name of the undo file from here
char *oglwin::generateUndoFileName( int c )
{
	sprintf(undoFileName, "%s_%d_%d", UNDO_FILE_NAME,id, c);
	
	return undoFileName;
}



// -------------------------- callbacks and menu stuff ---------------------------
/*****
#pragma mark ----------prefs
*****/
extern bool				noArrowMFFlag;

#include "prefs.h"
#define CDEW_PREF_LIST \
{ \
	{"cdewStaggerMode:= ",							readInt,				writeInt,						&cdewStaggerMode},\
	{"cellBackgroundFlag:= ",						readBoolean,			writeBoolean,					&cellBackgroundFlag},\
	{"inactiveBackgroundMode:= ",					readInt,                writeInt,                       &inactiveBackgroundMode},\
	{"noArrowMFFlag:= ",							readBoolean,			writeBoolean,					&noArrowMFFlag},\
	{"showErrors:= ",                               readBoolean,			writeBoolean,					&showErrors},\
	{"displayMode:= ",                              readInt,                writeInt,                       &displayMode},\
	{"defaultThickness:= ",                         readInt,				writeInt,						&defaultThickness},\
	{"jugglerLines:= ",                             readInt,                writeInt,                       &jugglerLines},\
	{"showNumbers:= ",                              readBoolean,			writeBoolean,					&showNumbers},\
	{"showDisplayMode:= ",                          readBoolean,			writeBoolean,					&showDisplayMode},\
	{"showColorMode:= ",							readBoolean,			writeBoolean,					&showColorMode},\
	{"defaultThrow:= ",                             readInt,                writeInt,                       &defaultThrow},\
	{"showDelays:= ",                               readBoolean,			writeBoolean,					&showDelays},\
	{"changeBowfactor:= ",							readBoolean,			writeBoolean,					&changeBowfactor},\
	{"bowFactor:= ",                                readFloat,              writeFloat,                     &bowFactor},\
	{"arrowheadFactor:= ",							readFloat,              writeFloat,                     &arrowheadFactor},\
	{"colorPostscript:= ",							readBoolean,			writeBoolean,					&colorPostscript},\
	{"showObjects:= ",                              readBoolean,			writeBoolean,					&showObjects},\
	{"cdewJugglers:= ",                             readInt,                writeInt,                       &cdewJugglers},\
	{"cdewHands:= ",                                readInt,                writeInt,                       &cdewHands},\
	{"cdewEntries:= ",                              readInt,                writeInt,                       &cdewEntries},\
	{"cdewAutoStart:= ",                            readBoolean,			writeBoolean,					&cdewAutoStart},\
	{"cdewAutoOpen:= ",                             readBoolean,			writeBoolean,					&cdewAutoOpen},\
	\
	{"cdewWindowX:= ",                              readInt,                writeInt,                       &cdewWindowX},\
	{"cdewWindowY:= ",                              readInt,                writeInt,                       &cdewWindowY},\
	{"cdewCellWidth:= ",							readFloat,              writeFloat,                     &cdewCellWidth},\
	{"cdewCellHeight:= ",							readFloat,              writeFloat,                     &cdewCellHeight},\
	\
	{"autoFixConnections:= ",						readBoolean,			writeBoolean,					&autoFixConnections},\
	{"timeCursorOffset:= ",							readFloat,              writeFloat,                     &timeCursorOffset},\
	{"timeCursorWidth:= ",							readFloat,              writeFloat,                     &timeCursorWidth},\
	{"timeCursorColor:= ",							readVektor3,			writeVektor3,					&timeCursorColor},\
	{"inactiveColor:= ",							readVektor3,			writeVektor3,					&inactiveColor},\
	{"activeColor:= ",                              readVektor3,			writeVektor3,					&activeColor},\
	{"cdewSnap:= ",                                 readBoolean,			writeBoolean,					&cdewSnap},\
	{"diffHeightFlag:= ",							readBoolean,			writeBoolean,					&diffHeightFlag},\
	\
	{"color0:= ",									readVektor3,			writeVektor3,					&colors[0]},\
	{"color1:= ",									readVektor3,			writeVektor3,					&colors[1]},\
	{"color2:= ",									readVektor3,			writeVektor3,					&colors[2]},\
	{"color3:= ",									readVektor3,			writeVektor3,					&colors[3]},\
	{"color4:= ",									readVektor3,			writeVektor3,					&colors[4]},\
	{"color5:= ",									readVektor3,			writeVektor3,					&colors[5]},\
	{"color6:= ",									readVektor3,			writeVektor3,					&colors[6]},\
	{"color7:= ",									readVektor3,			writeVektor3,					&colors[7]},\
	{"color8:= ",									readVektor3,			writeVektor3,					&colors[8]},\
	{"color9:= ",									readVektor3,			writeVektor3,					&colors[9]},\
	{"color10:= ",									readVektor3,			writeVektor3,					&colors[10]},\
	{"color11:= ",									readVektor3,			writeVektor3,					&colors[11]},\
	{"color12:= ",									readVektor3,			writeVektor3,					&colors[12]},\
	{"cdewDefaultWidth:= ",							readInt,				writeInt,						&cdewDefaultWidth},\
	\
	{"drawAsBowFlag:= ",							readBoolean,			writeBoolean,					&drawAsBowFlag},\
	/*	{"showMarkFlag:= ",								readBoolean,			writeBoolean,					&showMarkFlag},\
	{"dontSetMeToTrueUnlessYouAreWWFlag:= ",		readBoolean,			writeBoolean,					&dontSetMeToTrueUnlessYouAreWWFlag},\
	*/	\
	{NULL,											NULL,                   NULL,							NULL} }


/* Joe Pass environment initialisieren */
void oglwin::readCDEWPrefs(void)
{	float xf = 1., yf = 1.;	

	prefAtom cdewPrefList[] =       CDEW_PREF_LIST;
	wwwBoolean readCdewPrefsFlag = true;
#ifdef VerboseCdewOutput
	printf("read cdewPrefs \n");
#endif
	
#if MAC_CLASSIC_VERSION
	getMacModifiers();
	if (!ckMacMode)
#endif
		
		
        cdewWindowX = -1;
	
#if MAC_CLASSIC_VERSION
	getMacModifiers();
	if (ckMacMode)
		readCdewPrefsFlag = false;
#endif
	
	if( readCdewPrefsFlag )
		readTextPrefs(CDEW_PREF_NAME, cdewPrefList);
	
	//_myPrintf("read cdew window size to %d %d %.f %.f \n",cdewWindowX,cdewWindowY, cdewCellWidth, cdewCellHeight);
	
	// window size undefined: use default
	if( cdewWindowX == -1 )
	{
		cdewWindowX = CDEW_DEF_X;
		cdewWindowY = CDEW_DEF_Y;
		cdewCellWidth = DefCellW;
		cdewCellHeight = DefCellH;
	}
	
	if( cdewWindowX <= 1 ){
		xf = glutGet( GLUT_SCREEN_WIDTH);
		yf = glutGet( GLUT_SCREEN_HEIGHT);

		cdewWindowX *= xf;
		cdewWindowY *= yf;
		cdewCellWidth *= xf;
		cdewCellHeight *= yf;
	}
		
#ifdef VerboseCdewOutput
	printf("... done\n");
#endif
}

void oglwin::writeCDEWPrefs( void )
{
	float xf = 1., yf = 1.;	
	GLint w,h;
	
	int tmp;
#ifdef VerboseCdewOutput
	printf("writing prefs\n");
#endif
	
	tmp = glutGetWindow( );
	glutSetWindow( id);
	
	prefAtom cdewPrefList[] =       CDEW_PREF_LIST;
	
	// scale window size to 0...1
	w = glutGet( GLUT_SCREEN_WIDTH);
	h = glutGet( GLUT_SCREEN_HEIGHT);
	xf = 1.f / w;
	yf = 1.f / h;
	
	cdewWindowX *= xf;
	cdewWindowY *= yf;
	cdewCellWidth *= xf;
	cdewCellHeight *= yf;
	

#ifdef VerboseCdewOutput
    _myPrintf("writing cdew window size: %d %d %.f %.f \n",cdewWindowX,cdewWindowY, cdewCellWidth, cdewCellHeight);
#endif
	saveTextPrefs(CDEW_PREF_NAME, cdewPrefList);
	
	glutSetWindow( tmp);
#ifdef VerboseCdewOutput
	printf("writing prefs ... done\n");
#endif
}

#define CDEW_TEMP_NAME "cdew_template.txt"
#define CDEW_TEMP_LIST \
{ \
	{"templateCellWidth:= ",						readFloat,              writeFloat,                     &templateCellWidth},\
	{"templateCellHeight:= ",                       readFloat,              writeFloat,                     &templateCellHeight},\
	{NULL,											NULL,                   NULL,							NULL }\
}

/* Joe Pass environment initialisieren */
void oglwin::readTemplatePrefs(void)
{
	prefAtom cdewPrefList[] =       CDEW_TEMP_LIST;
	
	readTextPrefs(CDEW_TEMP_NAME, cdewPrefList);
}

void oglwin::writeTemplatePrefs( void )
{
	prefAtom cdewPrefList[] =       CDEW_TEMP_LIST;
	
	templateCellWidth =     glutGet( GLUT_WINDOW_WIDTH ) / cdewEntries;
	templateCellHeight =    glutGet( GLUT_WINDOW_HEIGHT ) / cdewJugglers;
	
	saveTextPrefs(CDEW_TEMP_NAME, cdewPrefList);
}


/*****
#pragma mark ---------- header
*****/

char *oglwin::readHeaderFromString( char *p )
{
	char *a, *b, t;
	char *h = 0;
	size_t l;
	
#if 01
	a = strstr( p, HeaderTag );
	b = strstr( p, EndHeaderTag ) + strlen( EndHeaderTag);
	if( a == 0 || b == 0 || a >= b )
		return NULL;
#else
	a = strstr( p, HeaderTag );
	b = strstr( p, EndHeaderTag );
	if( a == 0 || b == 0 || a >= b )
		return NULL;
	
	a += strlen( HeaderTag);
	
    // dont read end of line
	if( *a == '\n' )
		a++;
	if( *a == '\r' )
		a++;
	
	if( a > b )
	{
		if( *(b-1) == '\n' )
			b--;
		if( *(b-1) == '\r' )
			b--;
	}
	
#endif
	
	
	
	t = *b;
	*b = '\0';
	l = strlen( a) ;
	h = (char *)malloc( l  + 1);
	
	if( h )
	{       strcpy( h, a );
		*(h + l) = '\0';
	}
	
	// remove content of header information from file
	size_t c;
	for( c = 0; c< l ; c++ )
		*(a+c) = ' ';
	
	*b = t;
	
	return h;
}

void oglwin::readHeaderFromFile( void )
{
	char *p;
	
	p = selectAndLoadFile("read a header file");
	if( p == NULL )
		return;
	clearHeader();
	header = readHeaderFromString(p);
	free( p );
}

void oglwin::clearHeader( void )
{
	if( header )
	{       free( header );
		header = 0;
	}
}

void oglwin::writeHeaderToFile(  void )
{
	if( header == NULL )
		return;
	
	char *name, *dot;
	name = (char *)malloc( strlen(cdewFileName) + strlen(headerEnding) + 1);
	if( name == NULL )
		return;
	
	strcpy(name, cdewFileName);
	
	dot = strrchr( name, '.');
	if( dot )
		*dot = '\0';
	strcat(name, headerEnding);
	
	
	saveTextFileAs( header, name, "write a header file" );
	
	free(name);
}


/*****
#pragma mark ---------------------cutting line
*****/
/* if the compiler causes problems on big files (as metrowerks codewarrior may do)
cut the file in 2 peaces and add the 2nd to your project
*/
/*****
#pragma mark ---------------------glut routines
*****/
void oglwin::initGL(void)
{
	glClearColor (1.0, 1.0, 1.0, 0.0);
	glShadeModel (GL_FLAT);
}






void oglwin::setSiteswapInput (wwwBoolean mode )
{       static int oldPrintState;
	siteswapInputMode = mode;
	if( mode == true )
	{       oldPrintState = printSiteswap;
		if( printSiteswap == LetterDisplayMode )
			printSiteswap = OnePersonNumberDisplayMode;
	}else
		printSiteswap = oldPrintState;
	
	siteswapInputJuggler = min(siteswapInputJuggler, cdewJugglers -1);
	siteswapInputEntry = min(siteswapInputEntry, cdewEntries -1);
}

void oglwin::siteswapInputModeSpecialKey( int key )
{
	_causalCell *p;
	wwwBoolean sync;
	p  = getCausalCell( causalCells, siteswapInputJuggler, siteswapInputHand, siteswapInputEntry, cdewJugglers, cdewHands, cdewEntries );
	
	sync = isSyncThrow( p );
	
	nextSiteswapInputCell = false;
	
	if( displayMode == syncLineMode )
	{       switch (key)
	{               case GLUT_KEY_UP:
		if(  siteswapInputHand == rightHand )
		{       siteswapInputJuggler--;
			if( siteswapInputJuggler < 0 )
				siteswapInputJuggler += cdewJugglers;
			siteswapInputHand = leftHand;
		}else
			siteswapInputHand = rightHand;
		
		break;
	case GLUT_KEY_DOWN:
		if(  siteswapInputHand == leftHand )
		{       siteswapInputJuggler++;
			if( siteswapInputJuggler >= cdewJugglers )
				siteswapInputJuggler -= cdewJugglers;
			siteswapInputHand = rightHand;
		}else
			siteswapInputHand = leftHand;
		
		break;
	default:
		break;
	}
	}else
	{
		switch (key)
		{               case GLUT_KEY_UP:
			siteswapInputJuggler--;
			if( siteswapInputJuggler < 0 )
				siteswapInputJuggler += cdewJugglers;
				break;
		case GLUT_KEY_DOWN:
			siteswapInputJuggler++;
			if( siteswapInputJuggler >= cdewJugglers )
				siteswapInputJuggler -= cdewJugglers;
				break;
		default:
			break;
		}
	}
	switch (key)
	{               case GLUT_KEY_LEFT:
		siteswapInputEntry--;
		if( siteswapInputEntry < 0 )
			siteswapInputEntry += cdewEntries;
			break;
	case GLUT_KEY_RIGHT:
		siteswapInputEntry++;
		if( siteswapInputEntry >= cdewEntries )
			siteswapInputEntry -= cdewEntries;
			break;
	default:
		break;
	}
	getSiteswapInputHand();
	
	glutPostRedisplay();
}

wwwBoolean oglwin::siteswapInput( char n)
{
	_causalCell *p;
	wwwBoolean ret = true;
	int hnd;
	wwwBoolean numberInput = false;
	float length;
	
	autoStartRestartFlag = fileChangedFlag = true;
	
	addToUndoList();
	
	// numbers
	if( isdigit ( n ) )
	{    char str[2] = " ";
		*str = n;
		length = atoi(str);
		numberInput = true;
	}		 
	
	// upper case letters
	if( isupper( n ) )
	{	
		length = (n-'A' + 10);
		numberInput = true;
	}
	
	
	// action			
	if(  numberInput == true)
	{      
		if( nextSiteswapInputCell)
		{       siteswapInputEntry++;
			if( siteswapInputEntry >= cdewEntries )
				siteswapInputEntry -= cdewEntries;
		}
	}
	hnd = getSiteswapInputHand( );
	p = getCausalCell( causalCells, siteswapInputJuggler, hnd, siteswapInputEntry, cdewJugglers, cdewHands, cdewEntries);
	
	
#if 01
	if( numberInput == true)
	{
		if( printSiteswap == MultiPersonNumberDisplayMode && cdewJugglers > 1)
		{
			p->destJuggler = siteswapInputJuggler;
       		
			// check for passes
			// add a passing tag if neccessary
			if( (int)length % cdewJugglers != 0)
				siteswapInput( 'p'); 
			
			length /=cdewJugglers;
			
			length = (int)length;	 
		}
	}		
#endif
//	_myPrintf("length = %2.2f\n", length);
	
	if( numberInput)
	{    
		float d1, d2, delay;
		
		nextSiteswapInputCell = true;
		
		p->destHand = hnd;
		p->destHand = p->length%2?switchHand(p->destHand):p->destHand;
		
		d1 = *getDelayedJuggler( siteswapInputJuggler, hnd);
		d2 = *getDelayedJuggler(p->destJuggler, p->destHand);
		delay = d1 - d2;
		//	delay *=2;
		_myPrintf("delay = %2.2f\n", delay);
		//				_myPrintf("delayOffset = %2.2f -  %2.2f = %2.2f => %2.2f\n", d1, d2, delay, length - 2 + delay);        		
		_myPrintf("delayOffset = %2.2f -  %2.2f = %2.2f | length = %2.2f => %2.2f\n", d1, d2, delay, length, length - 2 + delay);        		
		
		if( delay )
			p->length = length - 1 + delay;
		else
			p->length = length - 2;
		
		_myPrintf("total length = %2.2f\n",  p->length);
		
		if( siteswapInputEntry >= cdewEntries )
			siteswapInputEntry -= cdewEntries;
	}else
	{
		switch( n )
		{       case 'x':
			p->destHand = switchHand( p->destHand );
			break;
		case 'p':
			p->destJuggler ++;
			if( p->destJuggler>= cdewJugglers)
				p->destJuggler -= cdewJugglers;
				break;
		case '-':
			if(p->length > absorb )
				p->length --;
			nextSiteswapInputCell = false;
			break;
		case '+':
			p->length++;
			nextSiteswapInputCell = false;
			break;
		case ' ':
		case '=':
			// cdewDisplay mode switch does not affect input state
			switchDisplayMode ();
			break;
			/*                      case '%':
			siteswapInputForceOtherHand = siteswapInputForceOtherHand == false;
			break;
		*/                      case '\r':
			//siteswapInputModeSpecialKey(GLUT_KEY_RIGHT);
			if(     p->length == absorb || p->destJuggler < 0
					||      p->destJuggler >= cdewJugglers || p->destHand < 0
					||      p->destHand > 1 )
				break;
			siteswapInputEntry += p->length;
			while( siteswapInputEntry >= cdewEntries )
				siteswapInputEntry -= cdewEntries;
				siteswapInputJuggler = p->destJuggler;
			siteswapInputHand = p->destHand;
			nextSiteswapInputCell = false;
			break;
		default:
			ret = false;
			nextSiteswapInputCell = false;
			break;
		}
	}
	// get next hand
	hnd = getSiteswapInputHand( );
	
	glutPostRedisplay();
	
	return ret;
}


// calc number of objects by
// objects = length of all arrows / patternLength + cdewJugglers * numberOfJugglerHands
// this is necessary if we want to allow the user _not_ to draw all holds, whats a bit annoying
float oglwin::calcNumberOfObjects( void )
{
	int totalLengthOfArrows, h, e, j;
	_causalCell *pl, *pr;
	float numberOfObjects;
	totalLengthOfArrows = 0;
	for( j = 0; j < cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{       pl = getCausalCell( causalCells, j,leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			pr = getCausalCell( causalCells, j,rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			
			if( pl->length != absorb)
				totalLengthOfArrows += pl->length;
			if( pr->length != absorb)
				totalLengthOfArrows += pr->length;
			
		}
	}
	h = 0;
	for( j = 0; j < cdewJugglers; j++)
	{
		wwwBoolean rightDoesThrow = false;
		wwwBoolean leftDoesThrow = false;
		
		// all of his throws
		for( e = 0; e < cdewEntries; e++ )
		{       pl = getCausalCell( causalCells, j,leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			pr = getCausalCell( causalCells, j,rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			if( pl->length != absorb )
				leftDoesThrow = true;
			if( pr->length != absorb )
				rightDoesThrow = true;
		}
		if( !leftDoesThrow )
			h++;
		if( !rightDoesThrow )
			h++;
	}
	
	numberOfObjects = (float)totalLengthOfArrows / cdewEntries + cdewJugglers*cdewHands - h;
	
	return numberOfObjects;
}
// ------------------------------- in work ------------------------------------



void oglwin::cdewKeyboard(unsigned char key, int x, int y)
{
#if MAC_CLASSIC_VERSION
#pragma unused( x,y )
#endif
	wwwBoolean done = false;
	
	cdewKeyboardX = x;
	cdewkeyboardY = y;
	

	if( pasteInProgressFlag)
	{
		if(key == '\033')
			abortPasteLocation();
		return;
	}
	
	if( markedDragInProgressFlag)
	{
		if(key == '\033')
			abortMarkedDrag();
		return;
	}
	
	if( dontSetMeToTrueUnlessYouAreWWFlag )
	{
		if( key == 'M' )
		{
			toggle(showMarkFlag);
			glutPostRedisplay();
			return;
		}
		if( key == ';' )
		{
			toggle(drawAsBowFlag);
			glutPostRedisplay();
			return;
		}
	}
	
	//yyy Bug? markDrag ist immer aktiv, wenn Drag aktiv ist => z.Zt ist die Reihenfolge wichtig.
	if( userDoesMarkDrag  )
	{
		//_myPrintf("userDoesMarkedDrag called\n");
		if( key >= '0' && key <= '9')
		{	int c;
			
			c = key - '0'; // i.e. 0 == unset
			if( c == 0)
				c--;
			setMarkedArrowColor( causalCells, c, userColorMode );
			glutPostRedisplay();
			return;
		}
 		if( key >='a' && key <= 'd')
		{	int theWidth = key - 'a' + 1;
			
			setMarkedWidth( theWidth );
			glutPostRedisplay();
			return;
		}
	}
	
	
	
	if( userDoesDrag )
	{
		//		_myPrintf("userDoesDrag called\n");
		if( key >= '0' && key <= '9')
		{
			int c;
			
			c = key - '0'; // i.e. 0 == unset
			
			if( c == 0)
				c--;
			
			startCell->userDefArrowColor = c;
			//			_myPrintf("color = %d\n",  startCell->userDefArrowColor);
			glutPostRedisplay();
			return;
			
			
		}
		
		if( key >='a' && key <= 'd')
		{	startCell->width = key - 'a' + 1;
			//_myPrintf("width = %d\n",  startCell->width);
			glutPostRedisplay();
			return;
		}
		
	}
	
	if( key == '\033' )
		setSiteswapInput( siteswapInputMode == false) ;
	
	
	
	if( siteswapInputMode )
		done = siteswapInput( key);
	if( !done)
		stdInput( key);
	
}


void oglwin::cdewSpecialKey(int key, int x, int y)
{
	
#if MAC_CLASSIC_VERSION
#pragma unused( x,y )
#endif
	if( siteswapInputMode )
	{       siteswapInputModeSpecialKey( key );
		return;
	}
	switch (key)
	{
		case GLUT_KEY_F1:
			printArrowLength ^= 1;
			break;
		case GLUT_KEY_F2:
			nextMouseInputMode();
			break;
			/*              case GLUT_KEY_F4:
			if( theModifiers & GLUT_ACTIVE_ALT)
		{       closeWin();
				return;
		}
			break;
			*/
#if SUPPORTS_CMHN
		case GLUT_KEY_F5:
			toggle(christophesNotation);
			break;
#endif
			// undo one action
		case GLUT_KEY_LEFT:
			undo();
			break;
			// redo one action
		case GLUT_KEY_RIGHT:
			redo();
			break;
			
		default:
			break;
	}
	
	glutPostRedisplay();
	
#if !CDEW_STANDALONE
	postUserInputAction();
#endif
}

Vector3D oglwin::getCdewBackgroundColor( void )
{
	Vector3D c;
	if(inactiveBackgroundMode == cdewBackgroundGrey ){
		if( !imTheAnimatedWindow )
			c = inactiveColor;
		else
			c = activeColor;
	}else
		c = Vector3D(1,1,1);
	
	return c;
}


void oglwin::_display( void )
{
	//_myPrintf("cdewwin #%d\n", id);
	Vector3D c;
	c = getCdewBackgroundColor();
	glClearColor( c.x, c.y, c.z, 0);
	
	glClear (GL_COLOR_BUFFER_BIT);
	
	drawEditorContent( );
	drawOnscreenHelp();
	
	if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
		glutSwapBuffers();
	else
		glFlush();
}


void oglwin::cdewDisplay(void)
{
	getWindowPosition( &cdewWindowX, &cdewWindowY);
	getWindowSize( &cdewWindowW, &cdewWindowH);
	_display();
}

		


void exitCausalEditor( void )
{
	// function may be called by different modules.
	// make sure that window flag is set correct
	static bool quitCdewCalled = false;

	if( quitCdewCalled )
		return;
	quitCdewCalled = true;
	

	
	jpPrefs.causalEditorHasOpenWindows = (firstCdewWindow != NULL);
	
	closeAllWin();
}

void closeAllWin( void )
{       oglwin *t = firstCdewWindow;
	// get last window
	while( (t = firstCdewWindow ) != NULL)
	{
		while( t->next )
			t = t->next;
		t->closeWin();
		t = firstCdewWindow;
	}
}

/*****
#pragma mark ---------------------main
*****/
// ------------------------------- main stuff ------------------------------------
// skip a word: leading spaces and all following nonSpaces
char *skipWord( char *p )
{
	if( p == NULL )
		return p;
	
	while( p && *p && *p == ' ')
		p++;
	
	while( p && *p && *p != ' ')
		p++;
	
	return p;
}

// glut callbacks:
// look up wwindow
// get callback
// do it
void _cdewDisplay(void)
{
	oglwin *t;
	if( globalOpenGLMenuInUse )
		return;
	
	t = getOglwinFromID( glutGetWindow() );
	if( t == NULL )
		return;
	t->cdewDisplay();
	
}
void _cdewReshape(int w, int h)
{
	oglwin *t;
	
	t = getOglwinFromID( glutGetWindow() );
	if( t == NULL )
		return;
	t->cdewReshape(w,h);
}
void _cdewKeyboard(unsigned char key, int x, int y)
{
	oglwin *t;
	
	t = getOglwinFromID( glutGetWindow() );
	if( t == NULL )
		return;
	t->cdewKeyboard(key,x,y);
}
void _cdewSpecialKey(int key, int x, int y)
{
	oglwin *t;
	
	t = getOglwinFromID( glutGetWindow() );
	if( t == NULL )
		return;
	t->cdewSpecialKey(key,x,y);
}
void _cdewMouse(int button, int state, int x, int y)
{
	oglwin *t;
	
#if 1 | !TARGET_API_MAC_CARBON
	int winID;
	winID = glutGetWindow();
	t = getOglwinFromID(winID );
#else
	//      _myPrintf("glutGetWindow() = %d\n", glutGetWindow());
	t = getOglwinFromMacWindow( FrontWindow());
	glutSetWindow( t->id );
	//      _myPrintf("t->id = %d, glutGetWindow() = %d\n", t->id, glutGetWindow());
#endif
	if( t == NULL )
		return;
	t->cdewMouse(button, state,x,y);
}
void _cdewMotion( int x, int y)
{
	oglwin *t;
#if 1 |  !TARGET_API_MAC_CARBON
	int winID;
	winID = glutGetWindow();
	t = getOglwinFromID(winID );
#else
	//      _myPrintf("glutGetWindow() = %d\n", glutGetWindow());
	t = getOglwinFromMacWindow( FrontWindow());
	glutSetWindow( t->id );
	//      _myPrintf("t->id = %d, glutGetWindow() = %d\n", t->id, glutGetWindow());
#endif
	
	if( t == NULL )
		return;
	
	t->cdewMotion(x,y);
}
void _cdewPassiveMotion( int x, int y)
{
	oglwin *t;
	
	t = getOglwinFromID( glutGetWindow() );
	
	if( t == NULL )
		return;
	
	t->cdewPassiveMotion(x,y);
}



void _cdewMenu( int id)
{
	oglwin *t;
	
#if !MAC_CLASSIC_VERSION
	int winID;
	winID = glutGetWindow();
	t = getOglwinFromID(winID );
	if( t == NULL )
		return;
#else
	//      _myPrintf("glutGetWindow() = %d\n", glutGetWindow());
	t = getOglwinFromMacWindow( FrontWindow());
	
	if( t == NULL )
		return;
	
	glutSetWindow( t->id );
	//      _myPrintf("t->id = %d, glutGetWindow() = %d\n", t->id, glutGetWindow());
#endif
	
	
	t->cdewMenu(id);
}

void generateCdewPathNames( const char *sd )
{
	/*****
#pragma unused (sd)
	*****/
#if USE_ABS_PATH
	cdew1StdPath    = makeAbsoluteFilename( sd, cdew1StdFileName);
	cdewTemplatePath= makeAbsoluteFilename( sd, cdewTemplateFileName);
	cdewPrefPath    = makeAbsoluteFilename( sd, cdewPrefFileName);
	cdewStartPath   = makeAbsoluteFilename( sd, cdewStartFileName );
	cdewCrashPath   = makeAbsoluteFilename( sd, cdewCrashFileName );
	cdewSelectPath  = makeAbsoluteFilename( sd, cdewSelectFileName );
	cdewSigPath     = makeAbsoluteFilename( sd, cdewSigFileName );
#else
	cdew1StdPath    =cdew1StdFileName;
	cdewTemplatePath = cdewTemplateFileName;
	cdewPrefPath    = cdewPrefFileName;
	cdewStartPath   = cdewStartFileName;
	cdewCrashPath   = cdewCrashFileName;
	cdewSelectPath  = cdewSelectFileName;
	cdewSigPath     = cdewSigFileName;
#endif
	
}

oglwin *initCausalEditor( char *title )
{
	const char *sd;	
	
#ifdef VerboseCdewOutput
	printf("\ninitialize causal editor\n");
#endif
	sd = getSysDirName();
	generateCdewPathNames( sd );
	chdir(sd);
	
	atexit( exitCausalEditor );
	
	readOwnerSignature();
	
	if( title )
		firstCdewWindow = new oglwin(title);
	
	initUndo();
	
#ifdef VerboseCdewOutput
	printf("initializing causal editor....done\n");
#endif
	return firstCdewWindow;
}


#if CDEW_STANDALONE
int main(int argc, char** argv)
{
	const char *cdewSysDir, *cdewAppDir;
	char p[1666];
	initIO();
	
	glutInit( &argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	
#ifdef USE_CONSOLE
	DoInitConsole();
#endif
	
#if 01
	printf( "cdew\nCopyright (c) 2000 Wolfgang Westerboer. All rights reserved.\n");
#endif
	createFolder(SYS_FOLDER_NAME);
	
	cdewSysDir = getAppFolder(argv[0]);
	cdewAppDir = getAppDirName();
	
	initCausalEditor( CDEW_DEFAULT_TITLE);
	
	if( argc > 1 )
		firstCdewWindow->readInputFile( argv[1]);
	
	
	glutMainLoop();
	return 0;
}

#ifdef USE_CONSOLE
static void DoInitConsole(void)
{
	
	SIOUXSettings.initializeTB       = FALSE;
	SIOUXSettings.standalone         = FALSE;
	SIOUXSettings.setupmenus         = FALSE;
	
	// change the following two lines as needed (see Metrowerks docs for full details)
	SIOUXSettings.autocloseonquit    = TRUE;
	SIOUXSettings.asktosaveonclose   = FALSE;
	
	SIOUXSettings.showstatusline     = TRUE;
	SIOUXSettings.tabspaces          = 4;
	SIOUXSettings.columns            = 100;
	SIOUXSettings.rows               = 40;
	SIOUXSettings.toppixel           = 100;
	SIOUXSettings.leftpixel          = 40;
	SIOUXSettings.fontid             = kFontIDMonaco;
	SIOUXSettings.fontsize           = 9;
	SIOUXSettings.fontface           = normal;
}
#endif

#endif

/*****
#pragma mark -------------------------
#pragma mark ---------------------work
#pragma mark -------------------------
*****/



/*      give the user some feedback:
...

since
a) I do not usethe feedback flag
b) the function corrupts the memory (fix see below )
and crashed the program when an arrow was draged outside the window (upwards)

its curently disabled
(i.e. returns without doing anything)

(f*cking bug that kept me busy for one week, at last)


*/


/*****
#pragma mark -------------------------
#pragma mark ---------------------more work
#pragma mark -------------------------
*****/





/*
 void oglwin::stringToScreen( int x, int y, float size, char *s )
 {
	 size_t i;
	 pushMatrix();
	 glTranslatef(x, y, 0);
	 glScalef(size, size, size);
	 for( i = 0; i <strlen(s);i++)
		 glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	 popMatrix();
 }*/




// ------------------------------- draw stuff ------------------------------------
wwwBoolean oglwin::readCdew( char *p )
{
	char *t;
	int j,h,e;
	int ji,hi,ei;
	char comp[50];
	_causalCell *c;
    char *s;
	
	
	//------------------------
	//      define wwwBoolean values as integers to read them correct
	//------------------------
#ifdef VerboseCdewOutput
	printf("reading cdew pattern\n");
#endif
	
	//check file type
	if( (t  = strstr( p, cdewVersionString) ) == NULL )
		return false;
	
	// get version number
	t += strlen(cdewVersionString);
	if( t == NULL )
	{       // no cdew file
		return false;
	}
	readFloat(t, &versionNumber);
	//sscanf(t, "%f", &versionNumber);
#ifdef VerboseCdewOutput
	printf("version is %f\n", versionNumber);
#endif
	
	
	// cdewDisplay Mode
	t = strstr( p, CDEWDISPLAYMODE);
	t = skipWord( t );
	if( t )
		readInt( t, &displayMode);
	//              sscanf(t, "%d", &displayMode );
#ifdef VerboseCdewOutput
	printf("display mode is %d\n", displayMode );
#endif
	
	// cdewSignature
	t = strstr( p, CDEWSIGNATURE);
	t = skipWord( t );
	if( t )
	{       readRestOfLine( t, &s);
		if( *s )
        {    setSignature( s );
            free( s );
        }
    }else
        setSignature( AnonSig);
	
	// cdewLastChanged
	t = strstr( p, CDEWLASTCHANGED);
	t = skipWord( t );
	if( t )
	{       readRestOfLine( t, &s);
		if( *s )
        {
			setLastChanged( s );
			free( s );
		}
    }else
        setLastChanged( AnonSig);
	/*
	 _myPrintf("file signed by  %s\n", sig );
	 _myPrintf("file changed by %s\n", lastChanged );
	 */
	
	
#ifdef VerboseCdewOutput
	printf("display mode is %d\n", displayMode );
#endif  // color pattern Mode
	t = strstr( p, CDEWCOLORMODE);
	t = skipWord( t );
	if( t )
		readInt(t, &cdewColorMode );
#ifdef VerboseCdewOutput
	printf("color mode is %d\n", cdewColorMode );
#endif
	
	// seperator lines
	t = strstr( p, CDEWJUGGLERLINES);
	t = skipWord( t );
	if( t )
		readBoolean( t, &jugglerLines );
#ifdef VerboseCdewOutput
	printf("jugglerLines is %d\n", jugglerLines );
#endif
	
	// cell color
	t = strstr( p, CDEWCELLCOLOR);
	t = skipWord( t );
	if( t )
		readInt( t, &cellColor);
	//              sscanf(t, "%d", &cellColor );
#ifdef VerboseCdewOutput
	printf("cellColor is %d\n", cellColor );
#endif
	
	// read hand delay
	hasHandDelay = false;
	t = strstr( p, CDEWHANDDELAY);
	t = skipWord( t );
	if( t != NULL )
		readBoolean( t, &hasHandDelay);
#ifdef VerboseCdewOutput
	printf("hasHandDelay is %d\n", hasHandDelay );
#endif
	
	// read beat number
	t = strstr( p, CDEWSHOWNUMBERS);
	t = skipWord( t );
	if( t != NULL )
		readBoolean( t, &showNumbers);
#ifdef VerboseCdewOutput
	printf("showNumbers is %d\n", showNumbers );
#endif
	
	
	// read hurried possible flag
	t = strstr( p, CDEWHURRIESFORBIDDEN);
	t = skipWord( t );
	if( t != NULL )
		readBoolean( t, &unhurriedDiagram);
#ifdef VerboseCdewOutput
	printf("unhurriedDiagram is %d\n", unhurriedDiagram );
#endif
	// read hurried visiblits flag
	t = strstr( p, CDEWSHOWHURRIES);
	t = skipWord( t );
	if( t != NULL )
		readBoolean( t, &showHurries);
#ifdef VerboseCdewOutput
	printf("showHurries is %d\n", showHurries );
#endif
	// read crosses visiblity flag
	t = strstr( p, CDEWHIDECROSSES);
	t = skipWord( t );
	if( t != NULL )
		readBoolean( t, &showCrosses);
#ifdef VerboseCdewOutput
	printf("showCrosses is %d\n", showCrosses );
#endif
	
	// read printSiteswapMode
	t = strstr( p, CDEWPRINTSITESWAPMODE);
	t = skipWord( t );
	if( t != NULL )
		readInt( t, &printSiteswap);
#ifdef VerboseCdewOutput
	printf("printSiteswap is %d\n", printSiteswap );
#endif
	// read sizes
	t       = strstr( p, CDEWSIZE);
	t = skipWord( t );
	if( t == NULL )
		return false;
	sscanf(t, "%d %d %d", &j, &h, &e);
	
#ifdef VerboseCdewOutput
	printf("cdew pattern parameters: %d %d %d\n", j,h,e);
#endif
	
	if( j <= 0 || h <= 0 || e <= 0)
	{
#ifdef VerboseCdewOutput
		printf("wrong pattern parameters: %d %d %d\n", j,h,e);
#endif
		return false;
	}
	
	
	// get memory
	c =  allocCausalCells( j,h,e);
	if( c == 0 )
	{
		printf("can't allocate memory\n");
		return false;
	}
	
	for(ji = 0; ji < j; ji++)
		for(hi = 0; hi < h; hi++)
			for( ei = 0; ei < e; ei++ )
				_initCC(c, ji, hi, ei, j, h, e );
	
	
	if( jugglerDelayList )
		free( jugglerDelayList );
	jugglerDelayList = allocDelayList( j );
	delayableJugglers = j;
	
	
	// why does c contain garbage
	free( causalCells );
	causalCells = c;
	cdewJugglers = j;
	cdewHands = h;
	cdewEntries = e;
	
#if 01
	//read old delay value to 2 hands
	{       char *tmp = p;
		if( strstr(tmp, cde2HandDelayTag) )
		{
			while( (tmp = strstr(tmp, cde2HandDelayTag)) != NULL )
			{
				int jd;
				float value;
				int h;
				
				tmp = skipWord( tmp );
				if( tmp == NULL )
					return false;
				
				
				tmp += sscanf( tmp, "%d %d %f", &jd, &h, &value);
				//      _myPrintf("juggler %d, hand %d, delay %.2f\n", jd, h, value);
				
				
				*getDelayedJuggler(jd-1, h ) = value;
				
			}
		}else
			//// old delay handling
		{
			
			while( (tmp = strstr(tmp, cdeDelayTag)) != NULL )
			{
				int jd;
				float value;
				int h;
				
				tmp = skipWord( tmp );
				if( tmp == NULL )
					return false;
				
				
#if 01
				// bug handling: cdeDelayTag was used for old style (both hand) delay
				char *p;
				wwwBoolean handleBugFlag;
				
				p = tmp;
				
				// skip juggler
				p = skipWord( p);
				
				// skip spaces
				while( *p && *p == ' ' )
					p++;
				// oups
				if( *p == '\0' )
					return false;
				
				// if the next string is a floating point value, we ok,
				// otherwise, we have to fix things
				while (*p && isdigit( *p ))
				{
					p++;
				}
				// oups
				if( *p == '\0' )
					return false;
				
				if( *p == '.' )
					handleBugFlag = false;
				else
					handleBugFlag = true;
				
				if( handleBugFlag )
				{
					//_myPrintf("handle bug is ON\n");
					tmp += sscanf( tmp, "%d %d %f", &jd, &h, &value);
					_myPrintf("juggler %d, hand %d, delay %.2f\n", jd, h, value);
					
					*getDelayedJuggler(jd-1, h ) = value;
					
				}else
				{
					//_myPrintf("handle bug is OFF\n");
					tmp += sscanf( tmp, "%d %f", &jd, &value);
					_myPrintf("juggler %d, delay %.2f\n", jd, value);
					*getDelayedJuggler(jd-1, leftHand ) = value;
					*getDelayedJuggler(jd-1, rightHand ) = value;
				}
#else
				tmp += sscanf( tmp, "%d %f", &jd,  &value);
				_myPrintf("juggler %d, both hands: delay %.2f\n", jd, value);
				*getDelayedJuggler(jd-1, leftHand ) = value;
				*getDelayedJuggler(jd-1, rightHand ) = value;
				
#endif
				
			}
		}
	}
	
#endif
	
	
	// walk through all cell descriptions
	t = p;
	sprintf( comp, "%s", CELLNAME);
	
	while((t = strstr( t, comp)) != NULL )
	{
		t += strlen( comp );
		t = readCausalCellFromString( t, c, j, h, e );
	}
	
	return true;
}


void oglwin::printCausalCells( _causalCell *l, int mj, int mh, int me )
{
	int j,h,e;
	
	printf("causal content of (%d %d %d) is \n", mj, mh, me);
	
	
	for( e = 0; e < me; e++ )
		for(h = 0; h < mh; h++ )
			for( j = 0; j < mj; j++ )
				printOneCausalCell( l, j, h, e, mj, mh, me);
	
	printf("causal content is ... done\n");
}

void oglwin::printOneCausalCell( _causalCell *l, int j, int h, int e,  int mj, int mh, int me  )
{
	_causalCell *p;
	p = getCausalCell( l, j, h, e, mj, mh, me );
	
	printf("\ncell %d %d %d = \n", j,h,e);
	printf("%d %d %d \n", p->length, p->destJuggler, p->destHand);
	printf("%d %d %d %d %d %f \n", p->userDefArrowColor, p->width, p->dash, p->dot, p->marked, p->delay);
	printf("%d %d %d \n", p->invertHandHeightOffset, p->showIfEmpty, p->feedbackFlag);
	
	
}


void oglwin::addToFactor( _causalCell *p, float f)
{
	
	p->factor += f;
    
    if( fabs(p->factor) <  .2 )
        p->factor = 0.;
    
}

void oglwin::cdewPassiveMotion(int x, int y)
{
	mousePassiveX = x;
	mousePassiveY = y;
	
	if( pasteInProgressFlag )
	{
		if( lookForPasteLocation() )
			glutPostRedisplay();
		return;
	}
	
	setMotionFeedback( x,y);
}





void oglwin::setDelay( _causalCell *p, float v )
{
	p->delay = v;
}

void oglwin::resetBowFactor( void )
{
	int j, h, e;
	_causalCell *p;
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				p->factor = 0;
			}
		}
	}
}

void oglwin::cdewMenu( int id)
{
	_menuSelection( id );
}


void oglwin::cycleMarksLeft( void )
{
	_causalCell *p, *s, *d, *d2;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	// copy content
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( p, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
					)
		
        // shift marks
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						d = getCausalCell( p, jAll, hAll, eAll-1, cdewJugglers, cdewHands, cdewEntries);
						d2 = getCausalCell( p, jAll, switchHand(hAll), eAll-1, cdewJugglers, cdewHands, cdewEntries);
						/*                              if( d->length == absorb && d2->length != absorb )
						d = d2;
						*/                              d->marked = s->marked;
                        )
        free( causalCells );
	causalCells = p;
}
void oglwin::cycleMarksRight( void )
{
	_causalCell *p, *s, *d, *d2;
	
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	addToUndoList();
	// copy content
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( p, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
					)
		
        // shift marks
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						d = getCausalCell( p, jAll, hAll, eAll+1, cdewJugglers, cdewHands, cdewEntries);
						d2 = getCausalCell( p, jAll, switchHand(hAll), eAll+1, cdewJugglers, cdewHands, cdewEntries);
						/*                              if( d->length == absorb && d2->length != absorb )
						d = d2;
						*/                              d->marked = s->marked;
                        )
        free( causalCells );
	causalCells = p;
}

#endif

void removeTempFolder( void )
{
	int ret;
	char s[1000];
	
	getcwd(s, 1000);
	
#ifdef VerboseCdewOutput
	printf("removing folder %s ... ", TempFolderName);
#endif
	ret = remove(TempFolderName);
	printResult(ret==0);
	
}


void oglwin::cdewReshape (int w, int h)
{
	int w1;
	
	/*
	 window width must be multiplyer of 4
	 needed by screenshot routines
	 */
	w1 = w & ~0x3;
	
	if( w1 != w )
		glutReshapeWindow( w1, h );
	
	windowWidth = w*linesPerJuggler;
	windowHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	
	cdewWindowX = glutGet( GLUT_WINDOW_X);
	cdewWindowY = glutGet( GLUT_WINDOW_Y);
	cdewWindowW = glutGet( GLUT_WINDOW_WIDTH);
	cdewWindowH = glutGet( GLUT_WINDOW_HEIGHT);
	
	cdewCellWidth = cdewWindowW / cdewEntries;
	cdewCellHeight = cdewWindowH / cdewJugglers;	
	
}

void oglwin::toggleBeepOnMarks( void )
{
	addToUndoList();
	
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					if( p->marked )
					toggle(p->beep);
					
					
					)
		
}

void oglwin::beepOff( void )
{
	
	
	addToUndoList();
	
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					p->beep = false;
					)
		
}
void oglwin::toggleBounceOnMarks( void )
{
	
	
	addToUndoList();
	
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					if( p->marked )
					toggle(p->bounce);
					)
		
}
void oglwin::toggleHalfspinOnMarks( void )
{
	fileChangedFlag = true;
	
	addToUndoList();
	
	allCells(       _causalCell *p;
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
			 if( p->marked )
			 toggle(p->halfspin);
			 )
	
}
void oglwin::tossAll( void )
{
	
	
	addToUndoList();
	
	allCells(       _causalCell *p;
					p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries );
					p->bounce = false;
					)
		
}



void oglwin::setMarkedDelay( float v )
{
	int j,h,e;
	_causalCell *p;
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( p->marked )
					setDelay( p, v );
				
			}
		}
	}
}



void oglwin::cdewMotion(int x, int y)
{
	_causalCell *p;
	float   cellWidth = windowWidth/cdewEntries;
	mouseX = x;
	mouseY = y;
	static int thisY;

	//      setMotionFeedback( x,y);
	
	if( userDoesFactorDrag )
	{
		float f;
		
		f = (thisY - y)/(float)lineHeight * DragFactor;
		if( startFactorDragNow )
		{       thisY = startMouseY;
			startFactorDragNow = false;
		}
		
		thisY = y;
		
		p = getCausalCell(causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
		
		if( p->marked )
		{
			_causalCell *t;
			allCells(
					 t = getCausalCell(causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					 if( t->marked )
					 addToFactor( t, f);
					 
					 )
		}else
			addToFactor(p, f);
		
		
		
	}
	
	if( userDoesThrowDelayDrag )
	{       float d;
		int i;
		
		dragDelay = d = (x - startMouseX)/cellWidth + initMarkedDelay;
		
		i = (int)(d * DragResolution);
		d = i;
		d = d / DragResolution;
		
		
		p = getCausalCell( causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
		
		
		
		if( p->marked )
			setMarkedDelay( d );
		else
		{
			setDelay( p, d );
		}
		
	}
	
	if( userDoesJugglerDelayDrag )
	{
		float d, tl,tr, t;
		float f = 1.;
		int i;

		getCellFromMousePosition( x,  y, &globalJ, &globalH, &globalE);
		destJ = globalJ;
		destH = globalH;
		destE = globalE;

		if( dragJugglerFlag == jugglerDelayDrag || dragJugglerFlag == jugglerNoDrag){
			if( destJ == startJ ){ 
				d = (x - startMouseX)/cellWidth + initDelayValue;
				if( d < 0 )
					f = -1.;
				
				d = fabs( d );
				i = (int)(d * DragResolution);
				
				
				if(cdewSnap ==  true)
				{
					int s;
					int lim;
					int delta;
					
					s = DragResolution / cdewJugglers;
					
					lim = s / 4;
					delta = i % s;
					
					// snap to beat mod jugglers
					if( delta <= lim )
						d = (i / s) * s;
					// snap to beat mod 2
					else  if( i%(DragResolution/2) < lim )
						d = (i / (DragResolution/2))*DragResolution/2;
					else
						d = i;
					
				} else
					d = i;
				
				d = d / DragResolution;
				
				d *= f;
				
				if( ceil(d)-d < .01)
					d = ceil(d);
				
				delayValue =  d;
				if( fabs( initDelayValue - delayValue) > 0.3)
					dragJugglerFlag = jugglerDelayDrag;
				
				
				tl = *getDelayedJuggler( delayedJuggler, leftHand );
				tr = *getDelayedJuggler( delayedJuggler, rightHand );
				
				t = *getDelayedJuggler( delayedJuggler, delayedHand );
				
				*getDelayedJuggler(delayedJuggler, leftHand ) =  d;
				*getDelayedJuggler(delayedJuggler, rightHand ) =  d;

				// check if the delay is ok (dont generate arrows < -2 etc )
				if(!checkDelay( d )){
					*getDelayedJuggler(delayedJuggler, leftHand ) =  tl;
					*getDelayedJuggler(delayedJuggler, rightHand ) =  tr;
					
					delayValue = t;
					
				}
			}
		}
		
		if( dragJugglerFlag == jugglerSwapDrag || dragJugglerFlag == jugglerNoDrag){
			if( destJ != startJ ){
					// move juggler startJ to line destJ
				dragMouseY = y;
				if( lastDestJ == -1 ){
					sourceJ = startJ;
					addToUndoList();
				}
				if( lastDestJ != destJ ){
					lastDestJ = destJ;
	#ifndef DRAG_MODE_2
					// reset delay to starting value, undo unwanted user input
					*getDelayedJuggler(sourceJ, leftHand ) = delayValue = initDelayValue;
					*getDelayedJuggler(sourceJ, rightHand ) = delayValue = initDelayValue;

					
					swapNAndM( sourceJ, destJ );
					sourceJ = destJ;
					
	#endif
					dragJugglerFlag = jugglerSwapDrag;
				}
			}
		}
	}
	
	if( userDoesJugglerDelayOneHandDrag )
	{
		float d, t;
		d = (x - startMouseX)/cellWidth + initDelayValue;
		d *= DragResolution;
		d = (int)d;
		d /= DragResolution;
		t = *getDelayedJuggler( delayedJuggler, delayedHand );
		delayValue = *getDelayedJuggler(delayedJuggler, delayedHand ) =  d;
		// check if the delay is ok (dont generate arrows < -2 etc )
		if(!checkDelay( d ) )
		{
			delayValue = *getDelayedJuggler(delayedJuggler, delayedHand ) =  t;
		}
	}
	if( userDoesDrag)
	{
		
		getCellFromMousePosition( x,  y, &globalJ, &globalH, &globalE);
		destJ = globalJ;
		destH = globalH;
		destE = globalE;
		if( displayMode == singleLineMode )
		{
			_getDesiredCell( &destJ, &destH, &destE );
		}
		// one of the many hacks to make christophes notation work.
		// here: change 2 to 1x if another throw follows. Not interpreted as a hurry
		if( christophesNotation )
			change2to1x();
		
		changeCellContentByDrag();
	}
	
	
	if( userDoesMarkDrag )
	{
		getCellFromMousePosition( x,  y, &globalJ, &globalH, &globalE);
		destJ = globalJ;
		destH = globalH;
		destE = globalE;
		
		changeCellContentByMarkDrag( );
	}
	
	glutPostRedisplay();
}






void oglwin::cdewMouse(int button, int state, int x, int y)
{
	int     j,h,e;
	
	
	theModifiers = getModifiers();
	
	mouseButton = button;
	mouseState = state;
	startMouseX = mouseX = x;
	dragMouseY = startMouseY = mouseY = y;
	
	if( mouseState == GLUT_DOWN && pasteInProgressFlag)
		return;
	
	if( mouseState == GLUT_UP)
	{
		if( pasteInProgressFlag )
		{
			setPasteLocation();
			return;
		}
		
		if( markedDragInProgressFlag )
		{
			setMarkedDrag();
			return;
		}
		getDesiredCell(&endJ, &endH, &endE);
		endCell = getCausalCell(causalCells, endJ, endH, endE, cdewJugglers, cdewHands, cdewEntries);
		
		if( userDoesThrowDelayDrag )
		{
			setNewPositionFromDelayValue( );   
		}
		
		if( userDoesDrag )
		{
			userDoesDrag = false;
			
			if( !(theModifiers == GLUT_ACTIVE_SHIFT) )
			{//     allCells( fixConnections(jAll, eAll);)
				
				getCellFromMousePosition( x,  y, &j, &h, &e);
				
				fixConnections( j,e);
				fixConnections( startJ, startE);
				
			}
			autoStartRestartFlag = fileChangedFlag = true;
		}
		if( userDoesMarkDrag )
		{
			userDoesMarkDrag = false;
			allCells( fixConnections(jAll, eAll);)
		}
		if(     userDoesThrowDelayDrag )
		{
			userDoesThrowDelayDrag= false;
			autoStartRestartFlag = fileChangedFlag = true;
		}
		if( userDoesJugglerDelayDrag || userDoesJugglerDelayOneHandDrag)
		{
			
			if( dragJugglerFlag != jugglerNoDrag ){
#ifdef DRAG_MODE_2
				int i, f, t, o;
				if( startJ < destJ ){
					f = startJ + 1;
					t = destJ ;
					o = -1;
				}else{
					f = startJ - 1;
					t = destJ;
					o = 1;
				}
				
				for( i = f; i != t; i += o)	
					swapNAndM( f, t);
				dragJugglerFlag = jugglerNoDrag;
#endif
			}else
				*getDelayedJuggler(delayedJuggler, delayedHand) = delayValue;
			userDoesJugglerDelayDrag = false;
			userDoesJugglerDelayOneHandDrag = false;
			fixDelay();
			autoStartRestartFlag = fileChangedFlag = true;
		}
		if( userDoesFactorDrag )
		{
			userDoesFactorDrag = false;
			
		}
		
		
		delayedJuggler = -1;
		glutPostRedisplay();
		
		postUserInputAction();
		
		goto bail;
	}else
	{       addToUndoList();
	}
	
	userDoesDrag = false;
	dragJugglerFlag = jugglerNoDrag;
	userDoesJugglerDelayDrag = false;
	userDoesMarkDrag = false;
	userDoesFactorDrag = false;

	
	/* remap modifiers if requested */
#define MofifierDrag            (0					|	0					|	0				)
	
#define ModifierDragMark		(0					|	GLUT_ACTIVE_SHIFT	|	0				)
#define ModifierMark            (GLUT_ACTIVE_ALT	|	0					|	0				)
#define ModiferDelayMark        (GLUT_ACTIVE_ALT	|	0					|	GLUT_ACTIVE_CTRL)
#define ModifierDelay			(0					|	0					|	GLUT_ACTIVE_CTRL)
#define ModifierHandDelay       (0					|	GLUT_ACTIVE_SHIFT	|	GLUT_ACTIVE_CTRL)
#define ModifierFactor          (GLUT_ACTIVE_ALT	|	GLUT_ACTIVE_SHIFT	|	0				)
#define ModifierJugglerDrag     (GLUT_ACTIVE_ALT	|	GLUT_ACTIVE_SHIFT	|	GLUT_ACTIVE_CTRL)
	
	
	if( theModifiers == 0 )
	{
		switch(theMouseInputMode )
		{   case mimDrag:
				theModifiers = MofifierDrag;
				break;
			case mimMark:
				theModifiers = ModifierMark;
				break;
			case mimDragMark:
				theModifiers = ModifierDragMark;
				break;
			case mimDelay:
				theModifiers = ModifierDelay;
				break;
			case mimHandDelay:
				theModifiers = ModifierHandDelay;
				break;
			case mimDelayMark:
				theModifiers = ModiferDelayMark;
				break;
			case mimFactor:
				theModifiers = ModifierFactor;
				break;
			case mimJuggler:
				theModifiers = ModifierJugglerDrag;
				break;
		}
	}
	
	//_myPrintf("theModifiers = %x\n", theModifiers);
	getCellFromMousePosition( x,  y, &j, &h, &e);
	getDesiredCell(&startJ, &startH, &startE);
	startCell = getCausalCell(causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
	syncCell = getCausalCell(causalCells, startJ, switchHand(startH), startE, cdewJugglers, cdewHands, cdewEntries);
	
	
	// action!
	switch (theModifiers)
	{
		case( ModifierFactor ):
			userDoesFactorDrag = true;
			startFactorDragNow = true;
			break;
			
			
		case ( ModiferDelayMark ):
		{
			userDoesThrowDelayDrag = true;
			initMarkedDelay = startCell->delay;
			break;
		}
			
			// can be done more elegant
		case ModifierDelay:
			delayedJuggler = startJ;
			delayedHand = startH;
			lastDestJ = -1;
			delayValue = initDelayValue = *getDelayedJuggler( delayedJuggler, delayedHand );
			userDoesJugglerDelayDrag = true;
			break;
			
		case ModifierHandDelay:
			delayedJuggler = startJ;
			delayedHand = startH;
			delayValue = initDelayValue = *getDelayedJuggler( delayedJuggler, delayedHand );
			userDoesJugglerDelayOneHandDrag = true;
			break;
			
		case ModifierMark:
			int old;
			old = startCell->marked;
			toggle(startCell->marked);
			break;
			
		case ModifierDragMark:
			if( !startCell->marked )
				break;
			initMarkedDrag();
			userDoesMarkDrag = true;
			//no break here!
			
		case MofifierDrag:
			userDoesDrag = true;
			break;
			
		default:
			break;
	}
	
	if( userDoesMarkDrag )
		_myPrintf("Marked drag\n");
	
bail:
	// tell cdewMotion function about start positions
	// brings up cells with the first click, without drag
	cdewMotion( x,y);
		
	
}



void oglwin::distributeThrowsPerHand( void )
{
	_causalCell *s, *d;
	_causalCell *p;
	int *l;
	int j,h,e;
	int h1,e1;
	int *firstUsedCell;
	
	addToUndoList();
	numberThrows();
	l = (int*)malloc( sizeof(int ) * cdewJugglers);
	if( l == 0 )
		return;
	for( j = 0; j < cdewJugglers; j++)
		*(l+j) = 0;
	
	
	// calc number of throws for each juggler
	for( j = 0; j < cdewJugglers; j++)
	{   
		for( e = 0; e < cdewEntries; e++ )
		{   
			for( h = 0; h <  cdewHands; h++)
			{ 
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb || s->marked )
					*(l+j)+=1;
			}
		}
	}
	firstUsedCell = (int *)malloc( sizeof(int ) * cdewJugglers);
	if( firstUsedCell == NULL )
	{
		free( l );
		return;
	}
	for( j = 0; j < cdewJugglers; j++)
		*(firstUsedCell+j) = -1;
	
	for( j = 0; j < cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h <  cdewHands; h++)
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb )//|| s->marked )
					if( *(firstUsedCell+j) == -1 )
						*(firstUsedCell+j) = e;
                        }
                }
        }
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
	{
		free( firstUsedCell);
		free( l );
		return;
	}
	allCells(       initCC( p, jAll, hAll, eAll); )
#define getNewCdewEntry( a, b ) ((float)(a) *  cdewEntries) / (b )
        // shift cells
        for( j = 0; j < cdewJugglers; j++)
        {       int nr = 0;
			for( e = 0; e < cdewEntries; e++ )
			{
				for( h = 0; h <  cdewHands; h++)
				{
					
					// get the cell with the next number for this juggler
					s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					// ok, this is the good one
					if( s->nr == nr )
					{
						float newCdewEntry;
						// put it into proper newCdewEntry:
						newCdewEntry = getNewCdewEntry( nr, *(l+j));
						d = getCausalCell( p, j, h, (int)newCdewEntry, cdewJugglers, cdewHands, cdewEntries);
						s->delay = newCdewEntry-(int)newCdewEntry;
						*d = *s;
						// research through all cells
						// makes it hand independent
						h = e = -1;
						
						// search for next number
						nr++;
					}
				}
			}
        }
	numberThrows();
	// connect throw to destination number
	for( j = 0; j <= cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h < cdewHands; h++)
			{
				// take next cell
				s = getCausalCell( p, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb || s->marked )
				{
					
					for( e1 = 0; e1 < cdewEntries ; e1++ )
					{
						h1 = 0;//for( h1 = 0; h1 < cdewHands; h1++ )
					{       int nr;
						
						// look for the cell that we are pointing to (d->nr == s->destNr)
						d = getCausalCell( p, s->destJuggler, h1, e1, cdewJugglers, cdewHands, cdewEntries);
						// number found
						if( d->length != absorb || d->marked )
						{       int destNr = s->destNr;
							
							// number of dest cell
							nr = d->nr;
							// if we are at the end of the pattern, go to the begin
							while( destNr >= *(l+s->destJuggler) )
								destNr -= *(l+s->destJuggler);
							
							if(s->length != absorb && destNr == nr )
							{       int length;
								
								length = e1-e;
								
								if( s->destNr >=  *(l+s->destJuggler) )
									length += cdewEntries;
								{
									// dont let throws disappear (length may not turn into absorb)
									if(length <= absorb )
										if( s->length == absorb )
											length = absorb;
										else
											length = s->length;
								}
								// dont let throws become invalid ('hold' to other hand)
								if(0&& h != h1 &&  length < -1)
									length = -1;
								
								s->length = length;
								
								
							}
						}
					}
					}
				}
			}
		}
	}
	//bail:
	free( causalCells );
	causalCells = p;
	free(firstUsedCell);
	free(l);
	
	
	allCells(       fixConnections(jAll, eAll); )
	{       int tempUndoState = oglwinUndoState;
		oglwinUndoState = undoDeactivated;
		fixDelay();
		
		oglwinUndoState = tempUndoState;
	}
	autoStartRestartFlag = fileChangedFlag = true;
}


void oglwin::distributeThrows( void )
{
	_causalCell *s, *d;
	_causalCell *p;
	int *l;
	int j,h,e;
	int h1,e1;
	int *firstUsedCell;
	
	addToUndoList();
	numberThrows();
	l = (int*)malloc( sizeof(int ) * cdewJugglers);
	if( l == 0 )
		return;
	for( j = 0; j < cdewJugglers; j++)
		*(l+j) = 0;
	// calc number of throws for each juggler
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					if( s->length != absorb || s->marked )
					*(l+jAll)+=1;
					)
        firstUsedCell = (int *)malloc( sizeof(int ) * cdewJugglers);
	if( firstUsedCell == NULL )
	{
		free( l );
		return;
	}
	for( j = 0; j < cdewJugglers; j++)
		*(firstUsedCell+j) = -1;
	
	for( j = 0; j < cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h <  cdewHands; h++)
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb )//|| s->marked )
					if( *(firstUsedCell+j) == -1 )
						*(firstUsedCell+j) = e;
                        }
                }
        }
	p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
	{
		free( firstUsedCell);
		free( l );
		return;
	}
	allCells(       initCC( p, jAll, hAll, eAll); )
#define getNewCdewEntry( a, b ) ((float)(a) *  cdewEntries) / (b )
        // shift cells
        for( j = 0; j < cdewJugglers; j++)
        {       int nr = 0;
			for( e = 0; e < cdewEntries; e++ )
			{
				for( h = 0; h <  cdewHands; h++)
				{
					
					// get the cell with the next number for this juggler
					s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					// ok, this is the good one
					if( s->nr == nr )
					{
						float newCdewEntry;
						// put it into proper newCdewEntry:
						newCdewEntry = getNewCdewEntry( nr, *(l+j));
						d = getCausalCell( p, j, h, (int)newCdewEntry, cdewJugglers, cdewHands, cdewEntries);
						s->delay = newCdewEntry-(int)newCdewEntry;
						*d = *s;
						// research through all cells
						// makes it hand independent
						h = e = -1;
						
						// search for next number
						nr++;
					}
				}
			}
        }
	numberThrows();
	// connect throw to destination number
	for( j = 0; j <= cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h < cdewHands; h++)
			{
				// take next cell
				s = getCausalCell( p, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb || s->marked )
				{
					
					for( e1 = 0; e1 < cdewEntries ; e1++ )
					{
						for( h1 = 0; h1 < cdewHands; h1++ )
						{       int nr;
							
							// look for the cell that we are pointing to (d->nr == s->destNr)
							d = getCausalCell( p, s->destJuggler, h1, e1, cdewJugglers, cdewHands, cdewEntries);
							// number found
							if( d->length != absorb || d->marked )
							{       int destNr = s->destNr;
								
								// number of dest cell
								nr = d->nr;
								// if we are at the end of the pattern, go to the begin
								while( destNr >= *(l+s->destJuggler) )
									destNr -= *(l+s->destJuggler);
								
								if(s->length != absorb && destNr == nr )
								{       int length;
									
									length = e1-e;
									
									if( s->destNr >=  *(l+s->destJuggler) )
										length += cdewEntries;
									{
										// dont let throws disappear (length may not turn into absorb)
										if(length <= absorb )
											if( s->length == absorb )
												length = absorb;
											else
												length = s->length;
									}
									// dont let throws become invalid ('hold' to other hand)
									if( h != h1 &&  length < -1)
										length = -1;
									
									s->length = length;
									
									
								}
							}
						}
					}
				}
			}
		}
	}
	//bail:
	free( causalCells );
	causalCells = p;
	free(firstUsedCell);
	free(l);
	
	
	allCells(       fixConnections(jAll, eAll); )
	{       int tempUndoState = oglwinUndoState;
		oglwinUndoState = undoDeactivated;
		fixDelay();
		
		oglwinUndoState = tempUndoState;
	}
	autoStartRestartFlag = fileChangedFlag = true;
}

/*
 called by distribute throws
 */
void oglwin::numberThrows( void )
{
	_causalCell *s, *d;
	int c;
	int *l;
	int j,h,e;
	
	
	l = (int *)malloc( sizeof(int ) * cdewJugglers);
	if( l == 0 )
		return;
	for( c = 0; c < cdewJugglers; c++)
		*(l+c) = 0;
	// calc number of throws for each juggler
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					if( s->length != absorb || s->marked )
					*(l+jAll)+=1;
					s->nr = s->destNr = CELL_NOT_USED;
					)
        // number used or marked cells
        for( j = 0; j < cdewJugglers; j++)
        {       int nr = 0;
			for( e = 0; e < cdewEntries; e++ )
			{       _causalCell *s1, *s2;
				s1 = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
				s2 = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
				s1->nr = s2->nr = -1;
				
				// if s1 is later than s1, switch pointers
				if( s1->delay > s2->delay )
				{       _causalCell *t;
					t = s1;
					s1 = s2;
					s2 = t;
				}
				// now s1 is earlier than s2
				// cell found (s1)
				if( s1->length != absorb || s1->marked )
				{
					s1->nr = nr;
					nr++;
				}
				// cell found (s2)
				if( s2->length != absorb || s2->marked )
				{
					s2->nr = nr;
					nr++;
				}
			}
        }
	// fill in destination number
	for( j = 0; j < cdewJugglers; j++)
	{
		for( e = 0; e < cdewEntries; e++ )
		{
			for( h = 0; h < cdewHands; h++)
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->length != absorb || s->marked )
				{
					d = getCausalCell( causalCells, s->destJuggler, s->destHand, e + s->length, cdewJugglers, cdewHands, cdewEntries);
					s->destNr = d->nr;
					if( s->length + e >= cdewEntries )
						s->destNr += *(l + s->destJuggler);
				}
			}
		}
	}
	free( l );
}





// walk through all cells
// check if the passing arrows will get less than MinPassValue
// return true if not, false otherwise
wwwBoolean oglwin::checkDelay( float delayValue )
{
#if MAC_CLASSIC_VERSION
#pragma unused( delayValue )
#endif
	
	int j, h, e;
	_causalCell *p, *pd;
	float delayDif;
	
	
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < 2; h++ )
		{
			
			for( e = 0; e < cdewEntries; e++ )
			{
				float v;
				
				p = getCausalCell(causalCells, j,h,e, cdewJugglers, cdewHands, cdewEntries);
				
				if( p->length != absorb )
				{
					pd = getCausalCell(causalCells, p->destJuggler,p->destHand,e+p->length, cdewJugglers, cdewHands, cdewEntries);
					delayDif = *getDelayedJuggler( j, h ) - *getDelayedJuggler( p->destJuggler, p->destHand ) ;
					
					if(p->destJuggler == j )
					{
						if( (v =  p->length - delayDif) < MinSelfValue )
							return false;
						
					}else
					{
						if( (v =  p->length - delayDif) < MinPassValue )
							return false;
					}
				}
			}
		}
	}
	return true;
}






void oglwin::fixDelay( void )
{
	/*      hack:  call coutine twice.
	first time negative delays are shifted to values bigger than .8
	fixed with 2nd call
	ok, this is _dirty_, but im tired.
	*/
	wwwBoolean undoStateTemp;
	
	//      _fixDelay( );
	
	undoStateTemp =  oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	
	_fixDelay( );
	
	oglwinUndoState = undoStateTemp;
	
	
}





/*
 rearange cell content according to delay:
 shift cells to postion that makes delay < 1
 */


void oglwin::_fixDelay( void )
{
	_causalCell     *s,*d;
	_causalCell *p;
	int c = 0;
	int j,h,e;
	
	
	addToUndoList();
	
	// set cell number
	allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
					s->nr = c++;
					)
		
		
        // set dest number
        allCells(       s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
						if( s->length != absorb )
						{
							d = getDestinationCell( s );
							s->destNr = d->nr;
						}else
						s->destNr = absorb;
						
                        )
		
        /*
		 get mem
		 fill in values
		 recalc arrow length according to delay
		 swap mem
		 */
        p = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( p == NULL )
		return;
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			int jDelayOffset;
			
			jDelayOffset = (int)floor( *getDelayedJuggler (j, h));
			
			
			for( e = 0; e < cdewEntries; e++ )
			{
				int deltaDelay;
				
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				d = getCausalCell( p, j, h, e + jDelayOffset, cdewJugglers, cdewHands, cdewEntries);
				*d = *s;
				
				if( d->length != absorb )
				{
					deltaDelay = jDelayOffset - (int)floor(*getDelayedJuggler(d->destJuggler, d->destHand));
					d->length -= deltaDelay;
				}
				
			}
		}
	}
	
	free( causalCells );
	causalCells = p;
	
	/*
	 calculate delay parameters
	 save new delay values
	 */
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for( h = 0; h < cdewHands; h++ )
		{
			float jDelay, jDelayRest;
			int jDelayOffset;
			
			jDelay = *getDelayedJuggler (j, h);
			jDelayOffset = (int)floor( jDelay );
			jDelayRest = jDelay - jDelayOffset;
			
			*getDelayedJuggler (j, h) = jDelayRest;
		}
	}
}

oglwin *checkMySize = 0;

oglwin *getOrCreateOglwinOnePersonNotation( char *buf )
{
	oglwin *w;
	int id = getOglwinOnePersonNotationID();
	
	
	if(id == 0 || generateTextToCausalInSameWindow == false)
	{
		id = _edit2NewCdew( buf );
		w = getOglwinFromID(id );
		glutSetWindow( id );
	}
	else{
		checkMySize = w = getOglwinFromID(id );
		glutSetWindow( id );
		id = _edit2Cdew( buf );
	}
	
	setAllWindowsUnused();
	setAllWindowsInactive();
	
	w->imTheOnePersonNotationWin = true;
	w->imTheAnimatedWindow = true;
	w->imTheLastUsedWindow = true;
	
	w->cdewDisplay(  ) ;
	
	return w;
}

void transferOnePersonNotationToCdewAndAutoSave( char *buf )
{
	int oldID;
	oglwin *w;
	
	disableTransferToTextEditor = true;
	
	
	oldID = glutGetWindow();
	
	w = getOrCreateOglwinOnePersonNotation( buf );
	if( jpPrefs.autosaveCausalFromTextEditor )
		w->saveDiagramAuto( );

	w->saveDiagramWithName( cdew1StdPath );
	
	if( oldID)
		glutSetWindow( oldID);
	
	
	disableTransferToTextEditor = false;
	
}



wwwBoolean oglwin::saveDiagramAuto( void )
{
	wwwBoolean ret = true;
	int jMax, maxPassCount = 0, thisPassCount;
	int j, h, e;
	_causalCell *cell;
	char filename[1000] = "";
	size_t count = 0;
	
	jMax = 0;
	
	
	/*
	 get juggler with most passes
	 */
	for( j = 0; j < cdewJugglers; j++ )
	{
		thisPassCount = 0;
		for( h = 0; h < cdewHands; h++ )
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				cell = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( cell->length != absorb )
					if( cell->destJuggler != j )
						thisPassCount++;
			}
		}
		if( thisPassCount > maxPassCount )
		{
			maxPassCount = thisPassCount;
			jMax = j;
		}
		
	}
	//_myPrintf("juggler %d has most passes\n", jMax);
	
	
	/*
	 print his siteswap to buffer
	 */
	for( e = 0; e < cdewEntries; e++ )
	{
		_causalCell *l, *r, *cell;
		wwwBoolean sync;
		
		l = getCausalCell( causalCells, jMax, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
		r = getCausalCell( causalCells, jMax, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
		
		sync = false;
		
		if( l->length != absorb && r->length != absorb )
			sync = true;
		else if( l->length != absorb )
			cell = l;
		else if( r->length != absorb )
			cell = r;
		else
			cell = 0;
		
		if( sync )
			count += sprintf(filename + count, "(%s,%s)", printPass(l), printPass(r));
		else if( cell )
			count += sprintf(filename + count, "%s",printPass(cell));
		else
			count += sprintf(filename + count, "-");
		
		
	}
	
	/*
	 Check if name is printed several times
	 (brute force)
	 */
	{
		size_t l, s;
		wwwBoolean match;
		
		//              _myPrintf("checking for substrings\n");
		
		l = strlen( filename );
		
		for( s = 1; s < l; s++ )
		{
			size_t d;
			char subname[1000];
			
			// cant be possible
			if( l%s )
				continue;
			
			sprintf(subname, "%s", filename );
			subname[s] = '\0';
			//                      _myPrintf("substring = %s (%d)\n", subname, s);
			
			
			/*
			 does filename contains only multipliers of substring?
			 -> found
			 */
			
			match = true;
			
			for( d = 0; d < l; d += strlen( subname) )
			{
				if( strncmp( &filename[d], subname, strlen(subname )) != 0 )
				{
					match = false;
					break;
				}
			}
			
			if( match )
			{
				sprintf(filename, "%s", subname);
				break;
			}
		}
		
	}
	/*
	 add file extension
	 */
	strcat(filename, ".pass");
	setCdewWindowTitle( filename );
	_myPrintf("%\n", filename);
	
	if( strchr( filename, '(' ))
	{
		_myBeep();
		myPrintf("wrong auto name: %s\n", filename);
	}
	
	
	
	/*
	 build complete path
	 */
	{
		if(askForSaveLocation ){
			char i[100];
			sprintf(i, "saving cdew window %d", id);
			saveTextFileAs("", filename, i);
//			sprintf(filename, "%s", thePatternTitle);
			
			setCdewWindowTitle(cdewFileName);
			saveDiagramWithName( cdewPathAndFileName );
		}
	}
	
	
	patternSaved = true;
	fileChangedFlag = false;
	
	return ret;
}

char *oglwin::printPass( _causalCell *cell )
{
	static char ret[1000] = "";
	size_t l = 0;
	int j,h,e;
	float absLength;
	_causalCell *p2;
	
	if( cell->length == absorb )
		return ret;
	
	
	getIndicesFromCellPointer( cell, &j, &h, &e );
	
	p2 = getCausalCell( causalCells, cell->destJuggler ,cell->destHand, e + cell->length, cdewJugglers, cdewHands, cdewEntries);
	
	absLength = 2. + cell->length - *getDelayedJuggler(j,h) + *getDelayedJuggler( cell->destJuggler, cell->destHand)  - cell->delay + p2->delay;
	
	/*      _myPrintf("%d,   %.1f, %.1f,    %.1f, %.1f\n", 2 + cell->length, *getDelayedJuggler(j), *getDelayedJuggler( cell->destJuggler), cell->delay, p2->delay);
	_myPrintf("absLength = %f\n", absLength);
	*/
	
	if( absLength == int(absLength) )
		l = sprintf(ret, "%.0f", absLength);
	else
		l = sprintf(ret, "%.1f", absLength);
	
	
	if( cell->destJuggler != j )
		sprintf(ret+l,"%c", 'p');
	
	return ret;
}


extern wwwBoolean changeCellSizeFlag;


void oglwin::addSymPattern( void )
{
	int e1, c;
	int j;
	_causalCell *ps, *pd;
	wwwBoolean tempUndoState;
	int w,h;
	
	getActualCellSize(&w, &h);
	
	dontChangeActualCellSizeFlag = true;
	
	e1 = cdewEntries;
	
	addToUndoList();
	tempUndoState = oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	// add cells
	for( c = 0; c < e1; c++ )
	{
		addThrow();
		
		// all cdewJugglers
		for( j = 0; j < cdewJugglers; j++ )
		{
			// read cells and switch cdewHands
			ps = getCausalCell( causalCells, j, leftHand, c, cdewJugglers, cdewHands, cdewEntries);
			pd = getCausalCell( causalCells, j, rightHand, e1 + c, cdewJugglers, cdewHands, cdewEntries);
			*pd = *ps;
			if( pd->length != absorb )
				pd->destHand = switchHand( pd->destHand);
			
			ps = getCausalCell( causalCells, j, rightHand, c, cdewJugglers, cdewHands, cdewEntries);
			pd = getCausalCell( causalCells, j, leftHand, e1 + c, cdewJugglers, cdewHands, cdewEntries);
			*pd = *ps;
			if( pd->length != absorb )
				pd->destHand = switchHand( pd->destHand);
			
		}
	}
	for( j = 0; j < cdewJugglers; j++ )
	{       int temp = autoFixConnections;
		
		autoFixConnections = true;
		fixConnections( j, 0 );
		fixConnections( j, cdewEntries / 2 );
		autoFixConnections = temp;
	}
	oglwinUndoState = tempUndoState;
	
	dontChangeActualCellSizeFlag = false;
	
	if( changeCellSizeFlag )
		setActualCellSize(&w,&h);
	
	autoStartRestartFlag = fileChangedFlag = true;
}
void oglwin::addSamePattern( void )
{
	int e1, c, h;
	int j;
	_causalCell *ps, *pd;
	wwwBoolean tempUndoState;
	int cw, ch;
	
	
	getActualCellSize(&cw,&ch);
	dontChangeActualCellSizeFlag = true;
	
	e1 = cdewEntries;
	
	addToUndoList();
	tempUndoState = oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	// add cells
	for( c = 0; c < e1; c++ )
	{
		addThrow();
		
		// all cdewJugglers
		for( j = 0; j < cdewJugglers; j++ )
		{
			for( h = 0; h < cdewHands; h++)
			{
				// read and copy cells
				ps = getCausalCell( causalCells, j, h, c, cdewJugglers, cdewHands, cdewEntries);
				pd = getCausalCell( causalCells, j, h, e1 + c, cdewJugglers, cdewHands, cdewEntries);
				*pd = *ps;
			}
		}
	}
	oglwinUndoState = tempUndoState;
	
	dontChangeActualCellSizeFlag = false;
	
	if( changeCellSizeFlag )
		setActualCellSize(&cw,&ch);
	
	autoStartRestartFlag = fileChangedFlag = true;
}

void oglwin::resetRepeat( void )
{
	
	_causalCell *p;
	
	addToUndoList();
	allCells(
			 p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 p->repeatIn = p->repeatOut = false;
			 
			 )        
        
}


void oglwin::toggleRepeatIn( void )
{
	
	_causalCell *p;
	addToUndoList();
	
	allCells(
			 p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 if(p->marked)
			 toggle(p->repeatIn);
			 
			 )        
        
}

void oglwin::toggleRepeatOut( void )
{
	
	_causalCell *p;
	addToUndoList();
	
	allCells(
			 p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 if(p->marked)
			 toggle(p->repeatOut);
			 
			 )        
        
}


// call after every input
// (release old mem)
// get mem
// calc club distribution
// save it
void oglwin::calcStart( void )
{
    _causalCell *p, *d;
    int ij, ih, ie;
	//    int j, h,e;
    int l, r;
	
	
    // list is empty, no Problems in start phase
	allCells( 
			  p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			  p->startConflict = false;
			  p->clubCount = 0;
			  *getStartHandFromList( jAll, hAll, startHandList) = 0;
			  )
		
		
		// place one club in every used hand
		allCells( 
				  p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
				  
				  // if( *getStartHandFromList( jAll, hAll, startHandList) == 0 )
				  {
					  
					  if( p->length != absorb && p->length != empty)
						  *getStartHandFromList( jAll, hAll, startHandList) = 1;
				  }        
				  )
		
#if 0
		_myPrintf("1--------------\n");
    allCells( 
			  //                p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			  
			  myPrintf("%d %d\n", *getStartHandFromList( jAll, leftHand, startHandList), *getStartHandFromList( jAll, rightHand, startHandList));
			  )
		_myPrintf("--------------\n");
#endif
	
    allCells(                
							 
							 p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
							 
							 if( p ->length != absorb )//&& p->length != empty)
							 {
								 
								 
								 // source is right from actual pointer && s has positive length -> arrow is entering pattern
								 if( p->length > 0 )
								 {
									 
									 // arrow leaves pattern to the right side <=> will enter at the left side                    
									 l = p->length + eAll;
									 /* 
									 if( l  >= cdewEntries )
									  (*getStartHandFromList( p->destJuggler, p->destHand, startHandList)) ++;
									  */
									 // calculate amount of circlulations
									 r = l / cdewEntries;
									 if( r )
										 (*getStartHandFromList( p->destJuggler, p->destHand, startHandList)) += r;
									 
								 }    
								 
								 // every arrows that leave to the left side reduces the number by one
								 if( p->length < 0 )
								 {         
									 // dest is right from actual pointer && p has negative length -> arrow is leaving pattern to the left
									 d = getDestinationCell( p );
									 
									 if( d )
									 {         
										 getIndicesFromCellPointer( d, &ij, &ih, &ie);
										 if( ie > eAll)
											 
										 {
											 if( *getStartHandFromList( jAll, hAll, startHandList) > 0 )
												 (*getStartHandFromList( jAll, hAll, startHandList))--;
										 }    
									 }  
								 }
							 }     
							 )
		
		
#if 01
		/* Unmöglichen Start markieren:
		Hand ist besetzt und erhält eine Keule
		Hand wirft -> shl --
		Hand erhält eine Keule und shl > 0
		=> fehler
		*/    
		
		
#if 01
		
		// copy list
		int *shl;
	//    _myPrintf("j,h,e = %d,%d,%d\n", cdewJugglers, cdewHands, cdewEntries);
    
    shl = allocStartHandList(cdewJugglers);
    
    if( shl )
    {   
        int j,h;
        
        for( j = 0; j < cdewJugglers; j++ )
            for( h = 0; h < cdewHands; h++ )
                *getStartHandFromList(j,h,shl) = *getStartHandFromList( j,h, startHandList );
		
    }        
#if 0
	_myPrintf("2--------------\n");
    allCells( 
			  //                p = getCausalCell( causalCells, jAll,  hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			  
			  myPrintf("%d %d\n", *getStartHandFromList( jAll, leftHand, shl), *getStartHandFromList( jAll, rightHand, shl));
			  )
		_myPrintf("--------------\n");
#endif
    
    {
        int j3,h3,e3;
        
        for( j3 = 0; j3 < cdewJugglers; j3++ )
        {
            for( h3 = 0; h3 < cdewHands; h3++ )
            {
                int *i;
                i = getStartHandFromList( j3,h3, shl);
				
                for( e3 = 0; e3 < cdewEntries; e3++)
                {
					
                    p = getCausalCell( causalCells, j3,  h3, e3, cdewJugglers, cdewHands, cdewEntries);
					
                    // active cell
                    if(p->length != absorb)
                    {
						p->clubCount = *i;
						// hand makes a throw
						if(*i > 1 ) // && *i < cdewEntries) // *i < cdewEntries == workarround for bugfix
						{
							_causalCell *p1;
							int j4,h4,e4;
							
							// throw is part of the pattern (not the start)
							p1 = getCellToCell( p );
							getIndicesFromCellPointer(p1, &j4,&h4,&e4);
							
#if 0                                
							if( p1->length < 0  // zipps are a problem if the hand is not empty, as well as occupied hands that should be empty 
								|| e3 >= e4 )
							{
								p1->startConflict = true; // marc throw that causes the conflict
								
							}    
#else
							// all throws that enter the pattern are fine
							// the rest is a problem
							if( p1->length < 0  // zipps are a problem if the hand is not empty, as well as occupied hands that should be empty 
								|| e4 + p1->length < cdewEntries ) // e4 + p1->length >= cdewEntries means that the arrow leaves the pattern, i.e. enters it 
							{
								p1->startConflict = true; // marc throw that causes the conflict
								
							}    
							
							
							
							
#endif
						}
						(*i) --;
						
						
                    }        
                }
            }  
        }    
		
    }       
    free( shl);
#endif            
	
	
#endif
    
    
	
}

void oglwin::setNewPositionFromDelayValue( void )
{
	//    if( p->marked )
    int j,h,e;
    _causalCell *p, *s;
    int offset = 0;
    
    offset = floor(startCell->delay);
    
    j = startJ;
    h = startH;
    e = (float)startE + offset;
//    _myPrintf("%d %d\n", e, offset);
	
    p = getCausalCell( causalCells, startJ, startH, e, cdewJugglers, cdewHands, cdewEntries);
    
    
    // not allowed when destination cell is used already
    // or if arrow length would be less than ss 0 
    if( p->length == absorb && startCell->length - offset > absorb )
    {
        *p  = *startCell;
        // don´t correct holds (non-causal throws need a special treatment)
        if( p->length != 0 )
            p->length -= offset;
        p->delay -= offset;
        
        s = getCellToCell( startCell );
        if( s )
            if(  s->length != absorb)
                s->length += offset; 
		
        startCell->length = absorb;
        startCell->delay = 0.;
        startCell->marked = false;
        
    }
	
    
} 

void oglwin::cutMarks( void )
{
	_causalCell *s;
	int j,h,e;
	
	copyMarks();
	addToUndoList();
	
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->marked)
					initCC(causalCells, j, h, e );
			}
}


void oglwin::copyMarks( void )
{
	_causalCell *s, *d;
	int j,h,e;
	
	
	
	if( cdewCopyBuffer )
	{
		free( cdewCopyBuffer);
		cdewCopyBuffer = NULL;
	}	
	
	cdewCopyBuffer = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( cdewCopyBuffer == NULL )
	{	
		cbJugglers = cbHands = cbEntries = 0;
		return;
	}
	
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
				initCC(cdewCopyBuffer, j, h, e );
	
	for(j = 0; j < cdewJugglers; j++)
	{
		bool markedCellFlag = false;
		firstMarkedJuggler = 0;
		firstMarkedEntry = 0;
		
		for( h = 0; h < cdewHands; h++)
		{
			for( e = 0; e < cdewEntries; e++ )
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( s->marked)
				{
					d = getCausalCell( cdewCopyBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
					if( markedCellFlag == false)
					{	
						firstMarkedJuggler = j;
						firstMarkedEntry = e;
						_myPrintf("firstMarkedCell: %d %d\n", firstMarkedJuggler, firstMarkedEntry);
						
						markedCellFlag = true;
					}
				}
			}
		}
	}
	cbJugglers = cdewJugglers;
	cbHands = cdewHands;
	cbEntries = cdewEntries;
}

void oglwin::startPaste( void )
{
	addToUndoList();
	pasteInProgressFlag = true;
	firstPosition = true;
	
	mousePassiveX = cdewKeyboardX;
	mousePassiveY = cdewkeyboardY;
	
	lookForPasteLocation();
}

bool oglwin::lookForPasteLocation( void )
{
	int ji,hi,ei;
	static int jl = -1,hl = -1, el = -1;
	
	_causalCell *s, *d, *p;
	
	
	getCellFromMousePosition(mousePassiveX, mousePassiveY, &ji, &hi, &ei );
	hi = switchHand( hi );
	
	if( ji == jl && hi == hl && ei == el)
		return false;
	
	jl = ji, hl = hi, el = ei;
	
	// outside window
	if( ji<0 || ei<0 || ji>=cdewJugglers || ei >= cdewEntries)
		return false;
	
	p = getCausalCell( causalCells, ji, hi, ei, cdewJugglers, cdewHands, cdewEntries);
	
	// firstPosition => don´t undo old position
	if( firstPosition  )
	{	
		int j, h, e;
		
		if( pasteTempBuffer )
			free( pasteTempBuffer );
		
		pasteTempBuffer = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
		
		for(j = 0; j < cdewJugglers; j++)
			for( h = 0; h < cdewHands; h++)
				for( e = 0; e < cdewEntries; e++ )
				{
					s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( pasteTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
				}	
					firstPosition = false;	
	}
	else
	{
		int j, h, e;
		
		for(j = 0; j < cdewJugglers; j++)
			for( h = 0; h < cdewHands; h++)
				for( e = 0; e < cdewEntries; e++ )
				{
					s = getCausalCell( pasteTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
				}	
	}
		
		// pasteMarks at cells the mouse is pointing to	
		pasteMarksAtCell( ji, hi, ei);
		
		
		return true;
}


void oglwin::abortPasteLocation( void )
{
	int j, h, e;
	_causalCell *s, *d;
	
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
			{
				s = getCausalCell( pasteTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				d = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				*d = *s;
			}	
				pasteInProgressFlag	= false;
	
	glutPostRedisplay();
}

void oglwin::setPasteLocation( void )
{
	pasteInProgressFlag	= false;
	
	glutPostRedisplay();
}

void oglwin::pasteMarksAtCell( int oj, int oh, int oe )
{
	_causalCell *s, *d;
	int j,h,e;
	
	if( cdewCopyBuffer == NULL )
		return;
	
	
	for( j = 0; j < cbJugglers; j++ )
	{
		for( h = 0; h < cbHands; h++ )
		{
			for( e = 0; e < cbEntries; e++ )
			{
				s = getCausalCell( cdewCopyBuffer, j, h, e, cbJugglers, cbHands, cbEntries);
				
				if( s->marked)
				{
					int dj, dh, de;
					
					// map to destination cells
					dj = (j + oj) % cdewJugglers;
					dh = (h + oh) % cdewHands;
					de = (e + oe) % cdewEntries;
					d = getCausalCell( causalCells, dj, dh, de, cdewJugglers, cdewHands, cdewEntries);
					
					// copy content, fix pass destination
					*d = *s;
					d->destJuggler = (d->destJuggler+oj)%cdewJugglers;
					d->destHand = (d->destHand+oh)%cdewHands;
					
				}
			}
		}
	}
}





// mark all used throws done at this beat
void oglwin::markPass( void )
{
	int j,h,e;
	int h1, e1;
	_causalCell *p, *s, *d, *markTempBuffer;
	static int currentPassReceiver = -1;
	int passReceiverAtCallup = currentPassReceiver;
	bool doItAgain;
	int moreThanOneDestinationJugglerHelper = -1;
	bool moreThanOneDestinationJuggerFlag = false;
	
	addToUndoList();
	
	
	markTempBuffer = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	
	{
		int j,h,e;
		
		for(j = 0; j < cdewJugglers; j++)
			for( h = 0; h < cdewHands; h++)
				for( e = 0; e < cdewEntries; e++ )
				{
					s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( markTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
				}	
	}
		
		do
		{
			bool markedFlag = false;
			for( j = 0; j < cdewJugglers; j++ )
			{
				for( h = 0; h < cdewHands; h++ )
				{
					for( e = 0; e < cdewEntries; e++ )
					{
						p = getCausalCell( markTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
						
						
						if( p->marked )
						{
							for( h1 = 0; h1 < cdewHands; h1++ )
							{
								for( e1 = 0; e1 < cdewEntries; e1++ )
								{
									d = getCausalCell( causalCells, j, h1, e1, cdewJugglers, cdewHands, cdewEntries);
									if( d->length != absorb )
									{
										if( d->destJuggler != j && currentPassReceiver == -1 ) // mark all passes
										{
											d->marked = true;
											markedFlag = true;
											
											// check if there are passes to different jugglers
											if( moreThanOneDestinationJugglerHelper == -1 )
												moreThanOneDestinationJugglerHelper = d->destJuggler;
											
											if( moreThanOneDestinationJugglerHelper != d->destJuggler)
												moreThanOneDestinationJuggerFlag = true;
											
										}
										else if( currentPassReceiver == d->destJuggler ) // mark passes to one juggler
										{
											d->marked = true;
											markedFlag = true;
										}
										else
											d->marked = false;
									}
								}
							}
							// no matching throw found: restore buffer, as all throws are unmarked now
							if( markedFlag == false )
							{
								int j,h,e;
								
								for(j = 0; j < cdewJugglers; j++)
									for( h = 0; h < cdewHands; h++)
										for( e = 0; e < cdewEntries; e++ )
										{
											s = getCausalCell( markTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
											d = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
											*d = *s;
										}	
							}
								
						}
					}
				}
			}
				
				// skip this pass when no fitting throw was found.
				// in search for all passes (currentPassReceiver == -1) only one destination juggler was found;
				// no mark was set at all
				doItAgain = (currentPassReceiver == -1 && moreThanOneDestinationJuggerFlag == false); // only passes to one juggler in multiPersonSearch
				doItAgain |= (markedFlag == false);// nothing was marked
					
					if( currentPassReceiver == cdewJugglers )
						currentPassReceiver = -1;
				else
					currentPassReceiver++;
				
				
				
		}while( doItAgain && currentPassReceiver != passReceiverAtCallup );
			
			
		free( markTempBuffer);
			
}


void oglwin::initMarkedDrag( void )
{
	int j,h,e;
	_causalCell *s, *d;
	
	markedDragTempBuffer = allocCausalCells( cdewJugglers, cdewHands, cdewEntries);
	if( markedDragTempBuffer == NULL )
		return;
	
	addToUndoList();
	
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
			{
				s = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				d = getCausalCell( markedDragTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				*d = *s;
			}	
}

void oglwin::abortMarkedDrag( void )
{
	int j, h, e;
	_causalCell *s, *d;
	
	for(j = 0; j < cdewJugglers; j++)
		for( h = 0; h < cdewHands; h++)
			for( e = 0; e < cdewEntries; e++ )
			{
				s = getCausalCell( markedDragTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				d = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				*d = *s;
			}	
				
				free( markedDragTempBuffer );
	markedDragInProgressFlag = false;
	glutPostRedisplay();
}

void oglwin::setMarkedDrag( void )
{
	free( markedDragTempBuffer );
	
	markedDragInProgressFlag = false;
	glutPostRedisplay();
}

void oglwin::changeCellContentByMarkDrag( void  )
{
	int dJ, dE;
	_causalCell *p, *pm, *s, *d;
	int j,h,e;
	
	dJ = destJ - startJ;
	dE = destE - startE;
	
	{
		int j,h,e;
		
		for(j = 0; j < cdewJugglers; j++)
			for( h = 0; h < cdewHands; h++)
				for( e = 0; e < cdewEntries; e++ )
				{
					s = getCausalCell( markedDragTempBuffer, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					d = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					*d = *s;
				}	
	}
		
		p = getCausalCell( causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
		
		for( j = 0; j < cdewJugglers; j++ )
		{
			for( h = 0; h < cdewHands; h++ )
			{
				for( e = 0; e < cdewEntries; e++ )
				{    pm = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
					if( pm->marked )
					{
						
						if( h == p->destHand )
							pm->destHand = h;
						else
							pm->destHand = switchHand( h );
						
						pm->destJuggler = mod( j+dJ, cdewJugglers);
						pm->length += dE;
						
						if( pm->destJuggler == startJ && pm->destHand == startH && pm->length < empty)
							pm->length = absorb;
						if( (pm->destJuggler != startJ || pm->destHand != startH) && pm->length < zip)
							pm->length = absorb;
					}
				}
			}
		}
		allCells( _fixConnections(jAll, eAll);)
}	


void oglwin::swap1And2( void )
{
	addToUndoList();	
	swapNAndM( 0, 1);
	
	autoStartRestartFlag = fileChangedFlag = true;
}

void oglwin::swapNAndM( int n, int m)
{
 _causalCell *c1, *c2, tc;
	int j,e, h ;
	float *d1, *d2, td;
		
	// Swap cell content of juggler 1 and 2
	for( e = 0; e < cdewEntries; e++ ){
		for( h = 0; h < cdewHands; h++ ){
			c1 = getCausalCell( causalCells, n, h, e, cdewJugglers, cdewHands, cdewEntries);
			c2 = getCausalCell( causalCells, m, h, e, cdewJugglers, cdewHands, cdewEntries);
			tc = *c1;
			*c1 = *c2;
			*c2 = tc;
		}
	}
	//swap delays for juggler 1 and 2
	if( 1 )
		for( h = 0; h < 2; h ++ ){
			d1 = getDelayedJuggler( n,  h );
			d2 = getDelayedJuggler( m,  h );
		
			td = *d1;
			*d1 = *d2;
			*d2 = td;
		}
	
	// Swap passes to juggler 1 and 2
	
	// 1. 
	// set all passes to high numbers (> cdewJugglers)
	for( j = 0; j < cdewJugglers; j++ ){
		for( e = 0; e < cdewEntries; e++ ){
			for( h = 0; h < cdewHands; h++ ){
				c1 = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( c1->destJuggler == n || c1->destJuggler == m)
					c1->destJuggler += cdewJugglers;
			}
		}
	}
	// 2. 
	// swap all passes to jn and jm 
	for( j = 0; j < cdewJugglers; j++ ){
		for( e = 0; e < cdewEntries; e++ ){
			for( h = 0; h < cdewHands; h++ ){
				c1 = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
				if( c1->destJuggler == n + cdewJugglers)
					c1->destJuggler = m;
				if( c1->destJuggler == m + cdewJugglers)
					c1->destJuggler = n;
			}
		}
	}

	autoStartRestartFlag = fileChangedFlag = true;
}

void setDefaultCdewWindowSize( int x, int y, int w, int h )
{
	cdewWindowDefaultX = x;
	cdewWindowDefaultY = y;
	cdewWindowDefaultW = w;
	cdewWindowDefaultH = h;
}	

void getDefaultCdewWindowSize( int *x, int *y, int *w, int *h )
{
	*x = cdewWindowDefaultX;
	*y = cdewWindowDefaultY;
	*w = cdewWindowDefaultW;
	*h = cdewWindowDefaultH;
}	

void oglwin::cdewSplitPathName( char *in , char *p, char *n, char *pn)
{
	splitPathName( in, p, n, pn);
}


bool oglwin::cdewSaveTextFileAs( char *in, char *filename )
{	
	char d[2000];
	getcwd( d, 2000);
	char i[100];
	sprintf(i, "saving cdew window %d", id);
	
	chdir( cdewPathName );
	_myPrintf("filename = %s\n", filename);
	_myPrintf("%s + %s = %s\n", cdewPathName, cdewFileName, cdewPathAndFileName);
	bool ret = saveTextFileAs( in, filename, i );
	if( ret )
		cdewSplitPathName( getFileSelectorPathAndName(), cdewPathName, cdewFileName, cdewPathAndFileName);
	
	chdir( d );
	return ret;
}


char * oglwin::cdewSelectAndLoadFile(  void )
{
	char *buf;
	char i[100];
	sprintf(i, "loading pattern into cdew window %d", id);
	buf = selectAndLoadFile( i);
	if( buf )
		cdewSplitPathName( getFileSelectorPathAndName(), cdewPathName, cdewFileName, cdewPathAndFileName);
	return buf;
	
}
