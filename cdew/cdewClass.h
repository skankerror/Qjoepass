/*
 * cdewWork.cpp
 * joe2
 * class definintion for cdew
 * for internal use of cdew routines
 *
 * Wolfgang Westerboer, 24.09.2001
 * westwolf@netcologne.de
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
#ifndef CDEW_CLASS
#define CDEW_CLASS

#include <stdio.h>

#include "cdewDefs.h"
#include "openglmenu.h"
#include "vtypes.h"
#include "fileaccess.h"
/*****
#if defined(__cplusplus)
extern "C"
{
#endif
	*****/
#if 0
#define cdew_standalone
#endif

enum {singleLineMode, syncLineMode, colorLineMode, maxDisplayMode};

enum CausalColorModes{ LengthColorMode=0, LineColorMode, JugglerColorMode, EmphaseCrossColorMode, EndOfColorMode};
//enum CausalAutoMode{ autoColorMode, userColorMode, markedColorMode, allColorMode, EndOfAutoMode};
enum CausalAutoMode{ autoColorMode, userColorMode, markedColorMode, allColorMode, EndOfAutoMode};

enum SiteswapDisplayModes{LetterDisplayMode = 0, OnePersonNumberDisplayMode, MultiPersonNumberDisplayMode, noHandDisplayMode, EndThrowDisplayMode};

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*****
#pragma mark ---------------------defines
*****/
#define lineHeight                      ( windowHeight/(cdewJugglers * linesPerJuggler ))
#define lineHeight2             (lineHeight/2)


#define allCells( commands )    {       int jAll,hAll,eAll;\
for( jAll = 0; jAll < cdewJugglers; jAll++ )\
{\
	for( hAll = 0; hAll < cdewHands; hAll++ )\
	{       \
		for( eAll = 0; eAll < cdewEntries; eAll++)\
		{       \
			commands\
		}\
	}\
}\
}

/*****
*****/
/*****
#pragma mark ---------------------defines
*****/




// default content
#define         DefJugglers             2
#define         Hands                           2
#define         DefEntries                      6
#define         DefContent                      absorb




#define                 XOFFSET 35
#define                 YOFFSET 50

#define _rand() ((float)rand()/RAND_MAX)

#define     RadiusOFHandCircle  10.f
#define     MinPassValue                -1
#define     MinSelfValue                -2
#define     CELL_NOT_USED               -3
#define     MAX_COLOR 12 /* don´t change value to smaller number as the first 12 colors are saved in the prefs file as color vectors*/
#define         COLOR1_NAME "black"
#define         COLOR2_NAME "blue"
#define         COLOR3_NAME "green"
#define         COLOR4_NAME "red"
#define         COLOR5_NAME "beige"
#define         COLOR6_NAME "turquoise"
#define         COLOR7_NAME "violet"
#define         COLOR8_NAME "light blue"
#define         NO_COLOR        -1


#define DragResolution                  100
#define DragFactor                              .1
#define UpFactor                                .025

//#define cdewVersionInfo                         "cdew 1.3"
#define cdewVersionNumber                       2.0
#define SUPPORTS_CMHN                           01
#define PRINT_ENTRIES                           0
#ifdef cdew_standalone
#define CDEW_STANDALONE                         01
#else
#define CDEW_STANDALONE                         0
#endif

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*****
#pragma mark ---------------------enums
*****/

// change read and write routines when changing content of structure
enum marked { unmarked = 0, marked = true, markedThisTime};
enum content {absorb = -3, empty = -2, zip = -1, hold = 0, single = 1};
//enum hand {leftHand = 0, rightHand };
enum { undoDeactivated, undoActivated };
enum mouseinputMode { mimDrag, mimMark, mimDragMark, mimDelay, mimHandDelay,mimDelayMark, mimFactor, mimJuggler, mimEnd};


/*****
#pragma mark -------------------- type definition
*****/
typedef struct {                      
	int				length;
	int				destJuggler;
	int				destHand;
	
	int				userDefArrowColor;
	int				userDefBackgroundColor;
	int				autoColor;
	int				width;
	int				dash;
	int				dot;
	int				marked;
	int				markedHelperFlag;
	int				beep;
	int				bounce;
	int				halfspin;
	float			delay;
	// For internal use - not patternSaved
	int				nr;                 // reconnecting cells
	int				destNr;
	
	int				notOutgoing;                // marking vacant cells
	int				notReached;
	int				reached;
	// one of the many changes (i.e _hacks_ that are neccessary to use christophes notation)
	wwwBoolean		c1x;
	wwwBoolean		isShortHold;
	wwwBoolean		invertHandHeightOffset;
	wwwBoolean		showIfEmpty;
	wwwBoolean		feedbackFlag;
	float			factor;
	
	int				test;
	wwwBoolean		switched;
	wwwBoolean		repeatIn, repeatOut;
	bool			startConflict;
	int				clubCount;
}_causalCell;



#define MAX_UNDO_FILE_CHARS     255

/*****
#pragma mark -------------------- class definition
*****/
typedef class oglwin{
	
public:
	int id;
	oglwin *next;
	oglwin(char *name);
	~oglwin();
	
	char					*sig;
	char					*lastChanged;
	wwwBoolean              autoSaveFlag;
	
	void					cdewSplitPathName( char *in , char *p, char *n, char *pn);
	void					screenshot( void );      
	void                    setSignature(char *sig);
	void                    setLastChanged(char *sig);
	
	void                    reCreateCausalMenu( void );
	
	void                    cdewMenu( int id);
	void                    cdewDisplay(void);
	void                    cdewReshape (int w, int h);
	void                    cdewKeyboard(unsigned char key, int x, int y);
	void                    cdewSpecialKey(int key, int x, int y);
	void                    cdewMouse(int button, int state, int x, int y);
	void                    cdewMotion(int x, int y);
	void                    cdewPassiveMotion(int x, int y);
	void                    _menuSelection( int r );
	wwwBoolean              readInputFile( char *name);
	void                    setColorPattern( void );
	void                    setPatternSavedByUser( wwwBoolean value);
	
	void                    initPattern( void );
	void                    initVars( void );
	
	void					cutMarks( void );
	void					copyMarks( void );
	
	bool					pasteInProgressFlag;
	bool					firstPosition;
	void					startPaste( void );
	bool					lookForPasteLocation( void );
	void					setPasteLocation( void );
	void					abortPasteLocation( void );
	void					pasteMarksAtCell( int j, int h, int e );
	_causalCell				*pasteTempBuffer;
	int						firstMarkedJuggler;
	int						firstMarkedEntry;
	
	bool					markedDragInProgressFlag;
	_causalCell				*markedDragTempBuffer;
	void					initMarkedDrag( void );
	void					setMarkedDrag( void );
	void					abortMarkedDrag( void );
	void                    changeCellContentByMarkDrag( void );
	
	
	
	wwwBoolean              closeWin( );
	
#if MAC_CLASSIC_VERSION
	/* "$% as opengl 1.2.x doesnt set the active window correctly, we have to get the active (topmost) window
		from toolbox calls.
		otherwise menu clicks cant be handled propperly
		*/
	WindowPtr				macWindow;
	
#endif
	
	
	
	_causalCell				*getCausalCell( _causalCell *p, int j, int h, int e, int maxJ, int maxH, int maxE);
	_causalCell				*_getCausalCell( _causalCell *p, int j, int h, int e, int maxJ, int maxH, int maxE);
	_causalCell				*getCellToCell( _causalCell *p );
	void					freeCausalCells( _causalCell *p);
	_causalCell				*allocCausalCells( int j, int h, int e);
	_causalCell				*getCellWithNumber( _causalCell *list, int mj, int mh, int me, int i );
	_causalCell				*getDestinationCell( _causalCell *p );
	
	
	wwwBoolean              imTheAnimatedWindow;
	wwwBoolean              imTheLastUsedWindow;
	wwwBoolean              imTheOnePersonNotationWin;
	
	float                   cdewCellWidth;
	float                   cdewCellHeight;
	
	int                     cdewJugglers;
	int                     cdewHands;
	int                     cdewEntries;
	
	int                     displayMode;
	int                     cellColor;
	
	int                     cdewColorMode;

	wwwBoolean              autoStartRestartFlag;
	int                     printSiteswap;
	wwwBoolean              fileChangedFlag;
	wwwBoolean				print4HandedSiteswapFlag;
	
	
	wwwBoolean              _readDiagram( char *p );
	wwwBoolean              saveDiagram( void );
	wwwBoolean              saveDiagramAs( void );
	wwwBoolean              saveDiagramAuto( void );
	char *                  printPass( _causalCell *cell );
	wwwBoolean              _saveDiagram( wwwBoolean mode );
	wwwBoolean              _saveDiagramWithDefaultPathName( wwwBoolean callFileSelector, char *defaultPathName );
	void                    setSaveFullFileName( char *name  );
	wwwBoolean              readDiagram( void );
	wwwBoolean              saveDiagramWithName( char *name );
	void                    writeTemplatePrefs(void);
	void                    fixAllConnections( void );
	void                    colorPattern( bool mode );
	void                    clearDiagram(void );
	void                    readHeaderFromFile( void );
	void                    clearHeader( void  );
	void                    writeHeaderToFile( void );
	void                    undo();
	void                    redo();
	void                    transferToAnimation( void );
	void                    transferToTextEditor( void );
	void                    addDiagram( void );
	void                    mergeDiagram( void );
	void                    markPath( void );
	void                    markJuggler( void );
	void                    markBeat( void );
	void                    markPass( void );
	void                    markJugglersHand( void );
	void                    markRight( void );
	void                    invertMarked( void );
	void                    unmarkPath( void );
	void                    unmarkJuggler( void );
	void                    unmarkBeat( void );
	void                    unmarkJugglersHand( void );
	void                    unmarkAll( void );
	void                    shiftMarksRight( void );
	
	void                    distributeThrowsPerHand( void );
	void                    distributeThrows( void );
	void                    connectToDestNumber( _causalCell *p,  int cdewJugglers, int cdewHands, int cdewEntries);
	void                    addThrow( void );
	void                    removeThrow( void );
	void                    addJuggler( void );
	void                    removeJuggler( void );
	void                    addSymPattern( void );
	void                    addSamePattern( void );
	void                    setDefaultHand( void );
	void                    invertDiagram( void );
	void                    resetBowFactor( void );
	
	void                    swapJuggler1And2( void );
	void                    cyclePatternRight( void );
	void                    cyclePatternLeft( void );
	void                    cyclePatternUp( void );
	void                    cyclePatternDown( void );
	void                    swapNAndM( int n, int m );
	void                    swap1And2( void );
	void                    cycleMarksRight( void );
	void                    cycleMarksLeft( void );
	void                    fixDelay( void );
	void                    invertHeightForChristophesVisualisationModeFunc ( void );
	void                    rotateDiagram( void );
	void                    switchDisplayMode( void );
	void                    exchangeMarkedHands( void );
	void                    toggleBeepOnMarks( void );
	void                    beepOff( void );
	void                    toggleBounceOnMarks( void );
	void                    toggleHalfspinOnMarks( void );
	void                    tossAll( void );
	
	void                    setMarkedWidth( int w );
	void                    setMarkedArrowColor(_causalCell *list, int i, int mode );
	void                    setMarkedBackgroundColor(_causalCell *list, int i);
	void                    nextColor();
	void                    nextThickness();
	void                    arrowDefault();                
	void                    toggleRepeatIn( void );
	void                    toggleRepeatOut( void );
	void                    resetRepeat( void );
	Vector3D				getCdewBackgroundColor( void );

	
private:
		//                  _menuList               *l;
	_menuList               *cdewMenuList;
	
	bool        	        unhurriedDiagram;
	
	
	
	// functions are called by universal menu routines (not part of class)
	// file stuff
	void                    postInputAction( void );
	void                    _saveDiagramWithFP( FILE *fp );
	void                    saveMhnh( FILE *fp );
	wwwBoolean              readMhnh( char *p );
	char                    *readMhnhContentFromBuffer( _causalCell *c, char *p, int hand );
	void                    printCausalCellToFile( FILE *fp, int j, int h, int e );
	char                    *readCausalCellFromString( char *p, _causalCell *c, int tmj, int mh, int me );
	void                    saveCdew( FILE *fp);
	wwwBoolean              readCdew( char *p );
	void                    printMhnhThrow( FILE *fp, int j, int h, int e );
	wwwBoolean              readSiteswap( char *p );
	void                    _addDiagram( char *p );
	void                    writeCDEWPrefs(void);
	void                    readCDEWPrefs(void);
	void                    readTemplatePrefs(void);
	
	
	void                    fixConnections( int sJ, int sE );
	
	
	
	int                     relMoveCells(  );
	void                    setMarkedHands( int hand );
	void                    setMarkedDelay( float markedDelayValue );
	void                    setDelay( _causalCell *p, float v );
	
	void                    drawRepeatIn( int x );
	void                    drawRepeatOut( int x);
	
	void					drawStart( void );
	
	
	//debug
	void					checkmem( void );
	
	// div stuff
	void					printCausalCells( _causalCell *l, int mj, int mh, int me );
	void					printOneCausalCell( _causalCell *l, int j, int h, int e, int mj, int mh, int me  );
	
	
	
	
#if !CDEW_STANDALONE
	void					transferToJoePass( void );
#endif
	
	
public:
		
	bool			showTimeCursor;
	bool			jugglerLines;
	bool            showNumbers;
	bool            showHurries;
	bool			showCrosses;
//	bool			showErrors;
	bool            showObjects;
	bool            showDelays;
	bool            showDisplayMode;
	bool            showColorMode;
	
	bool            hasHandDelay;
	
	int winX,winY,winW,winH;
	
	int		width;
	// undo stuff
	void            addToUndoList( void );
	void            undoOneAction( int c );
	char            *generateUndoFileName( int c );
	void            removeUndoFiles( void );
	void            key( unsigned char key, int x, int y);
	void            reshape( int x, int y );
	
	// global stuff
	int             defaultThrow;
	int		defaultWidth;

	// user input stuff
	int             globalJ, globalH, globalE;
	int             startJ, startH, startE;
	int             destJ, destH, destE;
	int				lastDestJ, sourceJ;
	int             endJ, endH, endE;
	_causalCell	*startCell;
	_causalCell *syncCell;
	_causalCell	*endCell;
	
	// openGL stuff
	int             windowWidth;
	int		windowHeight;
	int		mouseButton;
	int		mouseState;
	int		mouseX, mouseY;
	int		mousePassiveX, mousePassiveY;
	int		startMouseX, startMouseY;
	int		dragMouseY;
	int		cdewWindowW;
	int		cdewWindowH;
	int		cdewWindowX;
	int		cdewWindowY;
	
	// cdewDisplay stuff
	wwwBoolean	releaseMouse;
	float		radius;
	float		*jugglerDelayList;
	int		theModifiers;
	wwwBoolean	userDoesJugglerDelayDrag;
	wwwBoolean	userDoesJugglerDelayOneHandDrag;
	float		delayValue;
	float		initDelayValue;
	int			delayedJuggler;
	int			delayedHand;
	int			delayableJugglers;
	wwwBoolean	changeBowfactor;
	float		bowFactor;                      // factor for causals drawn as bows
	float		arrowAngleFactor;       // factor for arrows on bows
	float		arrowheadFactor;        // size factor for arrowheads
	wwwBoolean	printArrowLength;
	Vector3D	arrow1,                 // arrowwhead
				arrow2,
				arrow3,
				arrow4;
	
	// memory stuff
	_causalCell     *causalCells;
	_causalCell     *muxCells;
	
	// undo stuff
	int		undoCounter;
	int		maxUndo;
	int		highestUndoNumber;
	wwwBoolean	oglwinUndoState;
	// causal input stuff
	float		initMarkedDelay;
	wwwBoolean	userDoesThrowDelayDrag;
	wwwBoolean	userDoesMarkDrag;
	wwwBoolean	userDoesDrag;
	wwwBoolean	userDoesFactorDrag;
	int			cdewKeyboardX;
	int			cdewkeyboardY;
	
	// settings, saved in pattern file
	// bool defined as int, see prefs.c
	
	float		versionNumber;
	
	char		undoFileName[MAX_UNDO_FILE_CHARS];
	// siteswap input stuff
	wwwBoolean	nextSiteswapInputCell;
	int			siteswapInputEntry;
	int			siteswapInputJuggler;
	int			siteswapInputHand;
	int			siteswapInputMode;
	int			windowX, windowY, windowW, windowH;
	wwwBoolean	christophesNotation;
	wwwBoolean	siteswapInputForceOtherHand;
	
	// file stuff
	int			actualReadJuggler;
	wwwBoolean	patternSavedByUser;
	wwwBoolean	patternSaved;
	char		cdewFileName[MAX_PATTERN_TITLE];
	char		cdewPathAndFileName[MAX_FULL_FILE_NAME];
	char		cdewPathName[MAX_FULL_FILE_NAME];

	bool		cdewSaveTextFileAs( char *in, char *filename );
	char		*cdewSelectAndLoadFile(  void );
	
	
	// openGL stuff
	void		initGL(void) ;
	void		_display( void );
	void		getActualCellSize( int *cellWidth, int *cellHeight );
	void		setActualCellSize(int *cellWidth, int *cellHeight );
	void		cdewReshapeWindow( int w, int h );

	// postscript stuff
	void		initPostscript( int w, int h );
	void		endPostscript( void );
	void		printThisWindowPS( void );
	void		removePostscriptFile( void );
	
	void		setCdewWindowTitle( char *name );
	// memory stuff
	void		initCL( void );
	void		initCC( _causalCell *p,  int j, int h , int e);
	void		_initCC( _causalCell *p,  int j, int h, int e, int jMax, int hMax, int eMax );
	
	void		copyCausalCells(        _causalCell* s, _causalCell *d,
						int js, int hs, int es,
						int jd, int hd, int ed);
	_causalCell     *copyDiagram( void );
	float		*allocDelayList( int j );
	float		*getDelayedJuggler( int i, int h );
	int		switchHand( int h);
	wwwBoolean	isSyncThrow( _causalCell *p );
	void		numberThrows( void );
	void		showWindowList( void );
	float		getCellBackgroundColor( int i );
	void		calcCenterOfCell( int j, int h, int e, float *destCenterX, float *destCenterY);
	void		_calcCenterOfCell( int j, int h, int e, float *destCenterX, float *destCenterY);
	void		getCellFromMousePosition( int x, int y, int *j, int *hnd, int *e);
	void		changeCellContentByDrag( void );
	void		getDesiredCell( int *j, int *h, int *e );
	void		_getDesiredCell( int *j, int *h, int *e );
	wwwBoolean	checkDelay( float delayValue );
	// keaboard input stuff
	void		stdInput( unsigned char key);
	wwwBoolean	siteswapInput( char n);
	// causal input stuff
	void		markFaults( void );
	void		_fixConnections( int sJ, int sE );
	void		showMarkedWhenEmpty( wwwBoolean value );
	void		_mergeDiagram( char *p );
	void		_fixDelay(void);
	wwwBoolean	allLoopsClosed( void );
	void		postUserInputAction( void );
	
	
	// siteswap input stuff
	void		setSiteswapInput (wwwBoolean mode );
	void		siteswapInputModeSpecialKey( int key );
	wwwBoolean	getIndicesFromCellPointer( _causalCell *in, int *j, int *h, int *e );
	int		getSiteswapInputHand( void );
	
	
	// cdewDisplay stuff
	void		drawEditorContent( void );
	void		drawNumberOfBeats( void );
	void		drawCellBackground( int j, int h, int e);
	void		drawCellContent( int j, int hnd, int e);
	void		_drawCellContent( int j, int hnd, int e);
	wwwBoolean	drawAsBow( _causalCell *ps );
	void		drawDisplayMode( void );
	void		drawColorMode( void );
	void		cdewDrawCursor( void );
	void		drawDelayValue(void );
	void		toggleHorizontalLine( void );
	float		calcNumberOfObjects( void );
	void		drawNumberOfObjects( void );
	void		drawChristophesNotationInfo( void );
	void		drawOnscreenHelp( void );
	
	void		cdewInitMenu(void);
	
	//cmhn stuff
	void		change2to1x( void );
	
	// work
	void		setMotionFeedback( int x, int y);
	void		drawTimeCursor( void );
	void		setTimeCursorDiagram( bool mode );
	
	wwwBoolean	patternHasOpenLoops;
	void		nextMouseInputMode( void );
	void		drawMimMode( void );
	wwwBoolean	patternHasFaults;
	
	char		*readHeaderFromString( char *p );
	
	
	char		*header;
	
	void		initGLwin( char *name );
	
	
	void		rotatePosition( int *j, int *h);
	
	void		addToFactor( _causalCell *p, float f);
	wwwBoolean	startFactorDragNow;
	
	void		calcStart( void );
	
	int		*allocStartHandList( int j );
	int		*getStartHandFromList( int j, int h, int *l );
	
	int		*startHandList; 
	void		drawStartDistribution( void );
	bool		showStart;
	
	void		setNewPositionFromDelayValue( void );
	
   }oglwin;

oglwin	*getOglwinFromID( int id );
oglwin	*getOrCreateOglwinOnePersonNotation( char *buf );
oglwin	*getActualOglwin( void );



extern	int		theMouseInputMode;

extern	int		linesPerJuggler;
extern	Vector3D	colors[];


/*****
#if defined(__cplusplus)
}
#endif
*****/

#endif
