/*text editor geschlossen
=> pfad ungültig, sonst wird Inhalt unter alter pfad bei neuem Editor überschrieben
*/
/*
 *  editor.cpp
 *  joe2
 *
 * created by wolfgang on 09.02.2002
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
/************************************
 simple textEditor
 for openGL/glut
 end of Line:
 carriageReturn + Newline
 vers. 0.0
 9.2.2000
 ************************************/
#include "systemswitch.h"
#include "graphics.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>
//#include <unistd.h>

#include "editorClass.h"
#include "prefDef.h"
#include "globals.h"
#include "fileIO.h"

#include "fileaccess.h"

#include "myutil.h"
#include "myPrintf.h"
#ifdef MAC_CLASSIC_VERSION
#include "macWin.h"
#endif
#include "main.h"
#include "info.h"

#include "exitModules.h"

#include "languageSupport.h"


#include "applicationFolders.h"
#include "editor.h"
#include "grafik.h"
#include "pattern.h"
#include "parseSiteswap.h"
#include "scanner.h"
#include "prefs.h"
#include "fileIO.h"
#include "animationWin.h"
#include "jpPrefs.h"
#include "openglmenu.h"
#include "causal_editor.h"

#include "notation.h"

#include "j2.h"
#include "workspace.h"
#include "preprocess.h"

//#include "dialog.h"


void		saveAsTempFile( void );
int         tempCounter = 0;
char        *generateTempFileName( int c );
char        tempFileName[1000];
#define     TEMP_FILE_NAME  "textTempFile"


void        readTipFile( void );
bool		reloadTextEditorPattern( void );
bool		checkForFileChanged( void );
void		forceRestardForUnchangedFile( void );

static char	*tipText;
bool		disableTransferToTextEditor = false;
bool		editorContendChanged = false;
bool		dontSaveTextEditorContentThisTimeWithName = false;
bool		forceRestardFlag = false;

int			jugglerNumberForNHandedPatterns = 2;
bool		jugglerNumberForNHandedPatternsDefined = false;

enum		{ editorShowTipText, editorShowHelpText, editorShowJ2HelpText, editorShowNumberText, editorShowEditorContent };
int			editorDisplaySwitch = editorShowHelpText;
char		numberInputText[] = "enter number (1...9) as default number for multi-person patterns";
char		*getAppropriateTextEditorContent( void );

bool		editorPathValid = false;

//***********************************
// definitions
//***********************************
#pragma mark ------------------- definitions

// id also used by other modules
#define openPatternID				1000
#define TextEditorWorldInfoString	"text editor"		

// window size
#define         IdleCursor 01
#define         LinesBeforeCursor 8
// viewing stuff
#if 0
// stroke font stuff (not used)
#define USE_STROKE_FONTS
#else
// bitmap font stuff
#define USE_BITMAP_FONTS
#endif
#if 0
#define BigFont
#endif
#ifdef USE_BITMAP_FONTS
#ifdef BigFont
#define CHAR_WIDTH 10
#define CHAR_HEIGHT 17
#else
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 15
#endif
#else
#ifdef BigFont
#define CHAR_WIDTH 40
#define CHAR_HEIGHT 60
#else
#define CHAR_WIDTH 11
#define CHAR_HEIGHT 30
#endif
#endif



//***********************************
// enums
//***********************************
#pragma mark ------------------- enums

enum {lineEnd_oneChar, lineEnd_twoChar, lineEnd_unknown};

//***********************************
// external vars
//***********************************
#pragma mark ------------------- external vars



//***********************************
// global vars
//***********************************
#pragma mark ------------------- global vars






int			editWin								= 0;
bool		textEditorStartedPattern			= false;

bool		extendedFlag						= false;



//***********************************
// local vars
//***********************************
#pragma mark ------------------- local vars
static int      lineEnd = lineEnd_oneChar;
static char     lineEndCode[3];

//-----------------------------------
// text buffer
// concept: the first 7 characters of the buffer are not printed.
// heres the mode information stored
// this information is not part of the pattern, and will not be saved to disk
//-----------------------------------
#define                 modeSwitchPosition 3
char					modeSwitchString[modeSwitchPosition+2] = BSTRING;


static bool				sideMove = false;
static int              savedPosInLine;
static int              linesInEditorWin;
static int              internalOffset;
static int              editorLinesInWindow;

// editor content
static char             *helpText, *j2Text;
static int              lastEditX, lastEditY, lastEditW, lastEditH;
// scrolling stuff
static int              patternMouseX,
patternMouseY;
static int              patternButton;
static int              patternState;
static int              lineOffset;
static float			theEditCharWidth        = CHAR_WIDTH;
static float			scale           = .01f;

static bool				cursorVisible = false;

bool					generateTextToCausalInSameWindow = true;

static bool				drawTextNow = true;
bool					skipAnimateCdewPattern	= false;
bool					generateCausalNow		= false;

static float			cursorX,cursorY;
char                    *c						= NULL;
int                     inputPosition           = 0;
static bool				softWrap				= true;   // dont change, not working
char					*textEditorContent		= NULL;

static void				_startEditorPattern( char *p );
void					showMemoryInfo( void );
#define					DefaultTextEditorFileTitle	"untitled.pass"
char					theEditorTitle[MAX_PATTERN_TITLE] = DefaultTextEditorFileTitle;
char					textEditorFullPathAndName[MAX_FULL_FILE_NAME] = "";
char					textEditorPath[MAX_FULL_FILE_NAME] = ".";



//***********************************
// menu definition
//***********************************

void	editMenuSelection( int r ),
editOpenFunc( int dummy ),
editCloseFunc( int dummy ),
editSaveFunc( int dummy ),
editSaveAsFunc( int dummy ),
editStartFunc( int dummy ),
edit2CdewWinmode( int dummy ),
editRedisplayFunc( int dummy ),
editClearFunc( int dummy ),
editCapitalFunc( int dummy ),
vanillaFunc(  int dummy ),
extendedFunc(  int dummy ),
switchModeFunc( int dummy ),
cursorFunc(  int dummy ),
callJ2( int dummy ),
lineJuggle( int dummy ),
lineJuggleNHands( int dummy ),
j2HelpFunc( int dummy ),
editShowHelp( int dummy);


static int
editorMenu,
fileMenu,
siteswapMenu,
causalMenu,
j2Menu,
editMenu;
//modeMenu,
//viewMenu

_menuList *editMenuList;


#pragma mark ------------------- local protorypes
//***********************************
// local prototyles
//***********************************
static  char    *readTextEditorContent( void );
static  void    writeTextEditorContent( void );
static  void    openTextEditorPattern( void);
static  void    saveTextEditorContentWithName(void);

static  void    reshapePatternWin(int width, int height);
static  void    drawPatternWin( void );
static  void    patternKey(unsigned char key, int x, int y);
static  void    patternSpecialKey(int key, int x, int y);
static  void    patternMouse(int button, int state, int x, int y);
static  void    patternMotion(int x, int y);
static  void    clearPattern( void );
static  void    setInputPatternCode( char code );
static  void    showEditorContent( char *s, int h);
static  void    showCursor( void );
static  int     calcLinesInEditorWin( void );
static  int     calcLinesBeforeTextPos( char *pos );
static  void    recalcEditorLine( void );
static  void    readHelpFiles( void );
static  void    readStdHelp( void );
static  void    readJ2Help( void );
static  void    handleAddKey( unsigned char theKey);
static  void    handleBackspaceKey( void );
static  void    handleDelKey( void );
static  void    handleLeftKey( void );
static  void    handleRightKey( void );
static  void    handleInsertKey( void );
static  void    handleUpKey( void );
static  void    handleDownKey( void );
static  void    handlePageUpKey( void );
static  void    handlePageDownKey( void );
static  void    handleHomeKey( void );
static  void    handleEndKey( void );
static  void    _addKey( unsigned char theKey);
static  void    _removeChar( int pos );

char            *currentLineToMem( void );

static  char    *changeLineendings( char *in );
static  bool    writeFileRestoreLineend( char *name, char *mode, char *buffer, size_t size );
static  void    cursorIdle( int i );

#pragma mark ------------------- functions

void createTextEditor( void ) {
	printf("\ncreating text editor\n");
	
	if( !editWin ) {
		readHelpFiles();
		readTipFile();
		resetTextEditor();
		jpPrefs.showEditorWindow = true;
		
		if( joeHasCrashed )
			editorDisplaySwitch = editorShowHelpText;
		
		glutInitWindowPosition(jpPrefs.patternWinSizeX, jpPrefs.patternWinSizeY);
		glutInitWindowSize(jpPrefs.patternWinSizeW, jpPrefs.patternWinSizeH);
		editWin = glutCreateWindow("editor");
		glutReshapeFunc(reshapePatternWin);
		glutDisplayFunc(drawPatternWin);
		glutKeyboardFunc(patternKey);
		glutSpecialFunc(patternSpecialKey);
		glutMouseFunc(patternMouse);
		glutMotionFunc(patternMotion);
		glutTimerFunc( jpPrefs.flashTime, cursorIdle, 1);
		
		glutSetWindowTitle("by ...ww");
		glClearColor(1, 1, 1, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//              initEditorMenu();       // init menus
		initMenu( editMenuList, true);
		
		atexit( _closeTextEditor );
		
		
		if(  jpPrefs.textEditorAutoStartFlag)
			editStartFunc(0);
		
		extendedFlag = jpPrefs.modeSwitchCode == ExtendedCode;
		
		printf("create text editor ... done\n");
	} else
	{
		glutSetWindow( editWin);
	}
	
	_myPrintf("editWinID = %d\n", editWin);
}


void closeTextEditor( void ) {
	_closeTextEditor();
	
	jpPrefs.showEditorWindow = false;
	
	
}


void _closeTextEditor( void ) {
	
	
	if( editWin == 0)
		return;
	

	printf("dispose of text editor (id = %d)\n", editWin);
	if( textEditorContent && jpPrefs.askForSaveFlag	) {
		saveTextEditorContentWithName();
		free( textEditorContent );
		textEditorContent = NULL;
	}
	
	lastEditX = jpPrefs.patternWinSizeX;
	lastEditY = jpPrefs.patternWinSizeY;
	lastEditW = jpPrefs.patternWinSizeW;
	lastEditH = jpPrefs.patternWinSizeH;
	
	
	glutDestroyWindow( editWin );
	editWin = 0;
	
	printf("dispose of text editor ... done\n");

	editorContendChanged = false;
	editorPathValid = false;
	sprintf(theEditorTitle, "%s", DefaultTextEditorFileTitle);
	*textEditorFullPathAndName = '\0';
	
	_myPrintf("editor window closed\n");
}

#pragma mark ------------------- openGL callbacks
void drawPatternWin( void ) {
	char *p;
	
	
	if(!editWin)
		return;
	if( glutGetWindow( ) != editWin )
		return;
	
	glutSetWindow( editWin);
	getWindowPosition( &jpPrefs.patternWinSizeX, &jpPrefs.patternWinSizeY);
	
	p = getAppropriateTextEditorContent();		
	showEditorContent( p, 1 );
	
	showCursor();
	
	showMemoryInfo();
	
	if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
		glutSwapBuffers();
	else
		glFlush();
	
}

char *getAppropriateTextEditorContent( void )
{
	char *p;
	
	switch( editorDisplaySwitch ){
		case editorShowTipText:
			p = tipText;
			break;
		case editorShowHelpText:
			p = helpText;
			break;
		case editorShowJ2HelpText:
			p = j2Text;
			break;
		case editorShowNumberText:
			p = numberInputText;
			break;
		default:
			p = textEditorContent;
			break;
	}
	
	return p;
}


void patternMouse(int button, int state, int x, int y) {
	getModifiers();
	if( ckMode )
		return;
	
	if( state == GLUT_DOWN )
		glutSetCursor(GLUT_CURSOR_UP_DOWN);
	else
		glutSetCursor(GLUT_CURSOR_INHERIT);
	
	patternMouseX = x;
	patternMouseY = y;
	patternButton = button;
	patternState = state;
}

void patternMotion(int x, int y) {
	float   scrollLines;
	float   f,v;
	static int lastLineOffset = -1;
	
	patternMouseX = x;
	patternMouseY = y;
	
	if( patternButton != GLUT_LEFT_BUTTON)
		return;
	if( patternState != GLUT_DOWN)
		return;
	
	// restrict movement to window content
	v = min( jpPrefs.patternWinSizeH, y);
	v = max( 0, v);
	
	
	// number of lines to scroll through
	scrollLines = calcLinesInEditorWin();
	
	f = (float)(jpPrefs.patternWinSizeH-v)/jpPrefs.patternWinSizeH;
	lineOffset = scrollLines * f - 1;
	lineOffset = max(0,lineOffset);
	
	if( lineOffset != lastLineOffset) {
		redrawTextEditor();
		recalcFrameRate();
	}
	lastLineOffset = lineOffset;
}

void reshapePatternWin(int width, int height) {
	if( !editWin)
		return;
	
	glutSetWindow( editWin);
	
	
	jpPrefs.patternWinSizeW = glutGet(GLUT_WINDOW_WIDTH);
	jpPrefs.patternWinSizeH = glutGet(GLUT_WINDOW_HEIGHT);
	jpPrefs.patternWinSizeX = glutGet(GLUT_WINDOW_X);
	jpPrefs.patternWinSizeY = glutGet(GLUT_WINDOW_Y);
	
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	linesInEditorWin = calcLinesInEditorWin( );
	internalOffset = 0;
	drawTextNow = true;
	
	redrawTextEditor();
}


// ----------------- normal key handling -------------------
void patternKey(unsigned char theKey, int x, int y) {
#ifdef MAC_CLASSIC_VERSION
#pragma unused(x,y)
#endif
	
	static char lastKey;
	bool escCode;
	static int originalInputPosition;
	static bool jugglerNumberForNHandedPatternsFlag = false;
		
	jpPrefs.helpActive = false;
	
	editorDisplaySwitch = editorShowEditorContent;
	
	if(jugglerNumberForNHandedPatternsFlag ){
		if( isdigit( theKey ) ){
// ????warum steht hier unter jugglerNumbers die anzahl der Hände????
#if 0
			jugglerNumberForNHandedPatterns = (theKey-'1')*2 + 2;		
#else
			jugglerNumberForNHandedPatterns = (theKey-'1') + 1;
#endif
			
			if( jugglerNumberForNHandedPatterns != 0){
				_myPrintf("jugglerNumberForNHandedPatterns set to %d\n", jugglerNumberForNHandedPatterns);
				jugglerNumberForNHandedPatternsFlag = false;
				jugglerNumberForNHandedPatternsDefined = true;
				redrawTextEditor();
			}else{
				jugglerNumberForNHandedPatternsDefined = false;
			}
		}else{
			jugglerNumberForNHandedPatternsDefined = false;
		}
		
		return;
	}
	
#ifdef MAC_OS_X_VERSION
	
	if(theKey == delCode )
		theKey = backspaceCode;
	else if(theKey == backspaceCode )
		theKey = delCode;
#endif
	
	
	if( lastKey == ESC && theKey == ESC) {
		lastKey = 0;
		goto bail2;
	}
	
	if( theKey == ESC ){
		editorDisplaySwitch = editorShowTipText;
		redrawTextEditor();
	}
	
	if( editorDisplaySwitch == editorShowTipText ) {
		originalInputPosition = inputPosition;
		inputPosition = 0;
		lineOffset = 0;
	}
	
	// handle esc codes
	if( lastKey == ESC ) {
		escCode = true;
		lastKey = 0;
		inputPosition = originalInputPosition;
		if(isdigit( theKey ) &&  theKey != '0')
			nHandedSiteswap( textEditorContent, (theKey-'1'+1)*2);
		else{
			switch( theKey ) {
				case 'R':	
					// reset settings for N jugglers
					jugglerNumberForNHandedPatternsDefined = false;
					jugglerNumberForNHandedPatterns;
					editStartFunc(0);
					break;
				case '?':
					openDocFunc(0);
					break;
				case 'E':
					openFileInExternalEditor( textEditorFullPathAndName );
					break;
				case 'X':
					reloadTextEditorPattern( );
					editorContendChanged = true;
					break;
				case 'z':
					switchToLastOrAnimationWindow(0);
					break;
				case DEL:
					clearPattern();
					break;
				case 'o':
					openTextEditorPattern();
					originalInputPosition = 0;
					break;
				case 's':
					saveTextEditorContent();
					editorContendChanged = false;
					break;
				case 'a':
					editorContendChanged = true;
					saveTextEditorContentWithName();
					break;
				case 'S':
					if( strlen( textEditorFullPathAndName) != 0 )
						if( jpPrefs.helpActive != true && textEditorContent != NULL)
							writeFileRestoreLineend( textEditorFullPathAndName, "w", textEditorContent, strlen(textEditorContent));
					break;
				case 'r':
					editStartFunc(0);
					break;
				case 'I':
					editorDisplaySwitch = editorShowHelpText;
					jpPrefs.helpActive = true;
					break;
				case 'N':
					jugglerNumberForNHandedPatternsFlag = true;
					editorDisplaySwitch = editorShowNumberText;
					return;
					break;
				case 'c':
					_myPrintf("generating causal now, jugglerNumberForNHandedPatterns = %d\n", jugglerNumberForNHandedPatterns);
					generateCausalDiagramFromBuffer( textEditorContent, jugglerNumberForNHandedPatterns);
					break;
				case 'C':
					toggle( jpPrefs.generateCausalFromTextEditorOnStart);
					break;
				case 'j':
					callJ2( 0);
					break;
				case 'l':
					lineJuggle( 0);
					break;
				case ' ':
					lineJuggleNHands( 0);
					break;
				case 'w':
					closeTextEditor( );
					break;
				case 'q':
					// mac os x requires to close windows before calling exit
					// otherwise system services are not aviable any longer (i.e. navigation services);
					
					exitModules();
					
					break;
				default:
					escCode = false;
					jpPrefs.helpActive = false;
					return;
			}	
		}
	} else
		escCode = false;
	
	// ESC escapes from escape mode
	
	lastKey = theKey;
	if( escCode)
		goto bail2;
	
	// handle menu hotkeys
	if( _menuHotkeySelect( editMenuList, theKey) )
		goto bail;
	
	
	// only change editor content
	if( editorDisplaySwitch != editorShowEditorContent)
		goto bail2;
	
	// remapping some codes
	if( theKey == crCode )
		theKey = newlineCode;
	else if( theKey == newlineCode)
		theKey = crCode;
	// remapping the  macnewlineCode code
	if( macnewlineCode == theKey)
		theKey = crCode;
	
	// dispatch input
	switch( theKey ) {
		case backspaceCode:
			editorContendChanged = true;
			handleBackspaceKey();
			break;
		case delCode:
			editorContendChanged = true;
			handleDelKey();
			break;
			/*		case crCode:
			 handled by _menuHotkeySelect();
			 _myPrintf("editor: dispatch input - %d\n", theKey);
			 editStartFunc(0);
			 break;
			 */		
		default:
			editorContendChanged = true;
			handleAddKey(theKey);
			glutPostRedisplay();
			break;
	}
bail:
	
 	recalcEditorLine();
	
bail2:
	redrawTextEditor();
}

// ----------------- special key handling -------------------
void patternSpecialKey(int theKey, int x, int y) {
	
#ifdef MAC_CLASSIC_VERSION
#pragma unused(x,y)
#endif
	
	// dont change help text
	if( editorDisplaySwitch != editorShowEditorContent)
		return;
	
	// dispatch input
	switch( theKey ) {
		case GLUT_KEY_LEFT:
			handleLeftKey();
			break;
		case GLUT_KEY_RIGHT:
			handleRightKey();
			break;
		case GLUT_KEY_INSERT:
			handleInsertKey();
			break;
		case GLUT_KEY_HOME:
			handleHomeKey();
			break;
		case GLUT_KEY_END:
			handleEndKey();
			break;
		case GLUT_KEY_UP:
			handleUpKey();
			break;
		case GLUT_KEY_DOWN:
			handleDownKey();
			break;
		case GLUT_KEY_PAGE_UP:
			handlePageUpKey();
			break;
		case GLUT_KEY_PAGE_DOWN:
			handlePageDownKey();
			break;
		default:
			break;
			
	}
	recalcEditorLine( );
	redrawTextEditor();
}


// timer callback for idle cursor
void cursorIdle( int i ) {
#pragma unused(i)
	static int cnt = 0;

	// keep on fireing timer
	glutTimerFunc(jpPrefs.flashTime, cursorIdle, 1);
	
	// check for editor file change every 1/10 of a second
	checkForFileChanged( );
	readPrefsIfChanged( );
	
	if(        editWin == 0
	   || jpPrefs.helpActive
	   || cursorX < 0 || cursorX > jpPrefs.patternWinSizeW
	   ||  cursorY < 0 || cursorY > jpPrefs.patternWinSizeH
	   )
		return;
	if( cnt++ < 200 )
		return;
	
	if( !jpPrefs.useFullCursor ) {
		toggle(cursorVisible);
		
		redrawTextEditor( );
	}
	
}

#pragma mark ------------------- functions

void readHelpFiles( void ) {
	readStdHelp();
	readJ2Help();
}

void readStdHelp( void ) {
	helpText = readFile( helpPath, "r");
	helpText = changeLineendings( helpText);
	
	if( helpText == NULL )
		helpText = "help.txt not found";
}

void readJ2Help( void ) {
	j2Text = readFile( j2Path, "r");
	j2Text = changeLineendings( j2Text);
	
	if( j2Text == NULL )
		j2Text = "help.txt not found";
}

void readTipFile( void ) {
	tipText = readFile( tipPath, "r");
	tipText = changeLineendings( tipText);
	
	if( tipText == NULL )
		tipText = ESC_NAME" not found";
	
	
}

#pragma mark ------------------- calc display
void recalcEditorLine( void ) {
	lineOffset = calcLinesBeforeTextPos( textEditorContent + inputPosition);
	lineOffset = max( 0, lineOffset - LinesBeforeCursor);
}


int calcLinesInEditorWin( void ) {
	int     lineWidth = jpPrefs.patternWinSizeW * jpPrefs.textSize /theEditCharWidth-1;
	char    *p;
	int     l;
	float   totalLines, additionalLines;
	
	// select text buffer
	p = getAppropriateTextEditorContent();
	if( p == NULL )
		return 0;
	// determinate number of lines
	totalLines = 1;
	if( jpPrefs.helpActive )
		additionalLines = 20;
	else
		additionalLines = 0;
	
	l = 0;
	while( *p ) {
		// count lines
		if(*p++ == newlineCode) {
			totalLines++;
			l = 0;
		}
		
		
		if( softWrap) {
			// if the line reaches over the right border, add a line
			if( l > lineWidth) {
				l = 0;
				additionalLines++;
			}
			l++;
		}
	}
	return totalLines +     additionalLines;
}

int calcLinesBeforeTextPos( char *pos ) {
	int     lineWidth = jpPrefs.patternWinSizeW * jpPrefs.textSize /theEditCharWidth-1;
	char    *p;
	int     l;
	float   totalLines, additionalLines;
	
	p = getAppropriateTextEditorContent();
	if( p == NULL )
		return 0;
	// determinate number of lines
	totalLines = 1;
	additionalLines = 0;
	l = 0;
	while( *p && p < pos) {
		// count lines
		if(*p++ == newlineCode) {
			totalLines++;
			l = 0;
		}
		// if the line reaches over the right border, add a line
		if( l > lineWidth) {
			l = 0;
			additionalLines++;
		}
		l++;
	}
	return totalLines +     additionalLines;
}


#pragma mark ------------------- drawing routines


void showEditorContent( char *s, int h) {
#ifdef MAC_CLASSIC_VERSION
#pragma unused( h)
#endif
	long len;
	char theChar;
	float x,y;
	int line;
	int l;
	int i;
	float charWidth;
	float lineWidth;
	int lHeight;
	int actualPosition, errPos;
	char *p, *t;
	int ll;
	int skipLines;
	
	//myPrintf("drawing text content\n");
	
	
	glClearColor(1, 1, 1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if( s == NULL ) {
		return;
	}
	
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, jpPrefs.patternWinSizeW, 0, jpPrefs.patternWinSizeH, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// calc sizes
	// charWidth = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, ' ') * scale;
	charWidth = theEditCharWidth;
	lineWidth = jpPrefs.patternWinSizeW / charWidth -1 ;
	lHeight = charWidth *2;
	editorLinesInWindow = ll = jpPrefs.patternWinSizeH / lHeight - 1;
	// calc error pos (text will switch to red at that pos)
	if( validPattern == false )
		errPos = getErrPos();
	
	
	// calc maximum number of lines to be displayed
	ll = max( 0, ll + lineOffset);
	t = p = s;
	
	// prepare for display
	t = s;
	len = strlen((char*) t);
	actualPosition = 0;
	line = 1;
	x = charWidth;
	l = 0;
	skipLines = lineOffset;
	cursorX = cursorY = -100;
	
	setColorFromVector(jpPrefs.editorTextColor);
	
	for (i = 0; i <= len; i++) {
		y = jpPrefs.patternWinSizeH - line * lHeight;
		theChar = *(t+i);
		actualPosition++;
		
		if(!jpPrefs.helpActive && actualPosition-1 == inputPosition && skipLines <= 0) {       // save cursor pos
			cursorX = x - charWidth/2;
			cursorY = jpPrefs.patternWinSizeH - line * lHeight;
			
			if( jpPrefs.useFullCursor ) {
				setColorFromVector(jpPrefs.editorCursorBgColor);
				glBegin( GL_QUADS);
				glVertex2f( x, cursorY);
				glVertex2f( x + theEditCharWidth, cursorY);
				glVertex2f( x + theEditCharWidth, cursorY+15);
				glVertex2f( x, cursorY+15);
				glEnd();
				setColorFromVector(jpPrefs.editorCursorTextColor);
			}
		} else {       // set color, according to errors
			if( !jpPrefs.helpActive && validPattern == false && actualPosition >= errPos )
				setColorFromVector(jpPrefs.editorErrColor);
			else
				setColorFromVector(jpPrefs.editorTextColor);
		}
		
		// just ignore newlines
		if( theChar == skipCode)
			continue;
		
		//with every carriage return, go 1 line down
		if( theChar == newlineCode) {
			x = charWidth;
			l = 0;
			if( --skipLines <= 0)
				line ++;
			continue;
			
		}
		
		// if the line reaches over the right border, do the same
		// print at least one character (x != 0)
		if( softWrap ) {
			if( l >= lineWidth) {
				l = 0;
				x = charWidth;
				if( --skipLines <= 0)
					line++;
				else
					continue;
			}
		}
		
		// get character
		if(theChar < 0 ||(!isprint( theChar )
						  && theChar != crCode)) {
			theChar = '.';
		}
		// we are using a monospaced font, character width doesnt change
		l ++;
		// plot char at its position
		glLoadIdentity();
		glRasterPos2f(x,jpPrefs.patternWinSizeH - line * lHeight);
		if( skipLines <= 1)
			if( isgraph( theChar ))
#ifdef USE_BITMAP_FONTS
#ifdef BigFont
				
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, theChar);
#else
		
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, theChar);
#endif
#else
		{   
			glPushMatrix();
			glTranslatef(x,y,0);
			glScalef( scale, scale, scale);
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, theChar);
			glPopMatrix();
		}
#endif
		/*      cursorX = x;
		 cursorY = patternWinSizeH - line * lHeight;
		 */
		x += charWidth;
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	externBuf = s;
	
}


#pragma mark ------------------- input routines

void handleAddKey( unsigned char theKey) {
	
	if( isprint( theKey) || theKey == newlineCode) {
#if 0
		if( theKey == newlineCode )
			myPrintf("<nlc>\n");
		else if( theKey == crCode )
			myPrintf("<nlc>\n");
		else if( theKey >= 32 )
			myPrintf("(%3c)\n", theKey);
		else
			myPrintf("[%3d]\n", theKey);
#endif		
		_addKey( theKey);
	}
	sideMove = true;
}



// delet left char
void handleBackspaceKey( void ) {
	if( inputPosition == 0 )
		return;
	
	inputPosition--;
	_removeChar( inputPosition);
	sideMove = true;
	
}
// delete right char
// == go to the right
//    delete left char
void handleDelKey( void ) {
	_removeChar(inputPosition);
	sideMove = true;
}
void _removeChar( int pos ) {
	char *p;
	int c, len = strlen( textEditorContent) ;
	
	if( len == 0 || pos == len) {
		_myBeep();
		return;
	}
	
	// remove character at given pos
	for( c = pos; c <  len; c++ )
		textEditorContent[c] = textEditorContent[c+1];
	
	if( strlen( textEditorContent) > 0) {
		p = (char *)realloc(textEditorContent, len );
		if( p == NULL ) {
			printf("realloc failed to reduce memory\n");
			_myBeep();
		} else
			textEditorContent = p;
	}
	
}

void handleInsertKey( void ) {
	_addKey( ' ' );
	sideMove = true;
}
void handleLeftKey( void ) {
	if( inputPosition > 0 )
		inputPosition--;
	if( textEditorContent[inputPosition] == crCode )
		inputPosition--;
	sideMove = true;
}
void handleRightKey( void ) {
	if( inputPosition < (int) strlen( textEditorContent)) {
		inputPosition++;
		if( textEditorContent[inputPosition] == newlineCode )
			inputPosition++;
	}
	
	sideMove = true;
}

void handleHomeKey( void ) {
	inputPosition = 0;
	sideMove = true;
	internalOffset = linesInEditorWin;
}
void handleEndKey( void ) {
	inputPosition = strlen( textEditorContent);
	sideMove = true;
	internalOffset = 0;
}
void setInputPatternCode( char code ) {
	
	if( code != ExtendedCode && code != VanillaCode)
		code = VanillaCode;
	
	jpPrefs.modeSwitchCode = modeSwitchString[modeSwitchPosition] = code;
	
}

#pragma mark ------------------- transfer

// as JoePass runs on several operating systems which all handle line endings diffenrently,
// we use an own standard internally, which is 
// one byte long and contains the character saved in 
//    newlineCode
// on save, the line endings are saved in the form the current operating system expects
char *changeLineendings( char *in ) {
	
	char *p = in, *p1, *p2;
	
	lineEnd = lineEnd_oneChar;
	
	lineEndCode[0] = newlineCode;
	lineEndCode[1] = lineEndCode[2] = 0;
	
	if( p == NULL)
		return NULL;
	
	// get endOfLine code
	p1 = strchr( in, newlineCode );
	p2 = strchr( in, crCode );
	
	// no line ending
	if( p1 == NULL && p2 ==NULL)
		return in;
	
	// one char code: A
	if( p1 == NULL && p2) {
		lineEndCode[0] = *p2;
		lineEndCode[1] = '\0';
	} else
		// one char code: B
		if( p1 && p2 == NULL) {
			lineEndCode[0] = *p1;
			lineEndCode[1] = '\0';
		} else
			// multiLineCode
		{
			if( p1 < p2 ) {
				lineEndCode[0] = newlineCode;
				lineEndCode[1] = crCode;
			} else {
				lineEndCode[0] = crCode;
				lineEndCode[1] = newlineCode;
			}
			lineEndCode[2] = '\0';
			
			lineEnd = lineEnd_twoChar;
		}
	
	// multiLine mode: remove all crCode characters
	if(lineEnd == lineEnd_twoChar ) {
		while( (p = strchr( p, crCode )) != NULL )
			strcpy( p, p+1);
		
	} else
		// translate all crCode to newlineCode
	{
		while( (p = strchr( in, crCode )) != NULL )
			*p = newlineCode;
		
	}
	
	return in;
}

bool writeFileRestoreLineend( char *name, char *mode, char *buffer, size_t size ) {
	static FILE *fp;
	size_t pos = 0;
	char *p = buffer;
	
	if( (fp = fopen( name, mode)) != NULL ) {
		while( pos < size ) {
			if( p[pos] == newlineCode && lineEndCode[0] != '\0')
				fwrite( lineEndCode, 1, strlen(lineEndCode) * sizeof( char ), fp );
			else
				fwrite( p+pos , 1, 1 * sizeof( char ), fp );
			pos++;
		}
		
		fclose( fp );
		
		return true;
	}
	return false;
}

#pragma mark ------------------- file io


void openFileInTextEditor( char *n ){
	if( editWin != 0)
		closeTextEditor();
	createTextEditor();

	textEditorContent = _readTextEditorPattern( n );
	
	setTextEditorNames( n );
	setEditorWindowName( );	
	redrawTextEditor();
}


char *readTextEditorContent( void ) {
	char *p = NULL;
	
	if(editorPathValid )
	{
		_resetTextEditor();
		p = _readTextEditorPattern( editPath);
	}
	
	return p;
	
	
}

char *_readTextEditorPattern( char *fileName ) {
	char *p;
	
	_resetTextEditor();
	
	p = readFile( fileName, "r");
	p = changeLineendings( p );
	
	
	return p;
}

void writeTextEditorContent( void ) {
	// dont write help text
	if( jpPrefs.helpActive == true || textEditorContent == NULL)
		return;
	
	writeFileRestoreLineend( editPath, "w", textEditorContent, strlen(textEditorContent));
	editorPathValid = true;
}

void openTextEditorPattern( void) {
	char *p;
	char i[100];
	sprintf(i, "open file in text editor");
	
	writeTextEditorContent();
	
	p = selectAndLoadFile( i );
	p = changeLineendings( p );
	
	//	sprintf(textEditorFullPathAndName, "%s",);
	setTextEditorNames(  getFileSelectorPathAndName() );
	setEditorWindowName();
	
	printf( "theEditorTitle = %s\n", theEditorTitle);
	
	
	if( p != NULL ) {
		jpPrefs.helpActive = false;
		resetTextEditor();
		textEditorContent = p;
		
		redrawTextEditor();
	}
	
}

void saveTextEditorContent( void )
{
	char *p;
	
	if( !editorContendChanged )
		return;
	
	
	writeTextEditorContent();
	

	
	if( *textEditorFullPathAndName == '\0')
		saveTextEditorContentWithName();
	else{
		p = readFile( editPath, "r");
		if( p ){ 
			saveFile(p, textEditorFullPathAndName);
			setEditorWindowName();
		}
		free( p );
	}
}

void saveTextEditorContentWithName(void) {
	char *p;
	char i[100];

	
	if( !editorContendChanged )
		return;
	
	sprintf(i, "save text editor file");
	writeTextEditorContent();
	

	if( dontSaveTextEditorContentThisTimeWithName == true){
		dontSaveTextEditorContentThisTimeWithName = false;
		return;
	}
	
	p = readFile( editPath, "r");
	if( p ) {
		
		if( saveTextFileAs(p, theEditorTitle, i)){
			setTextEditorNames( fullPatternPathName);
			setEditorWindowName();
			
			editorContendChanged = false;
		}
		free( p );
		
		p = 0;
	}	
}


void setEditorWindowName( void )
{
	int oldID;
	oldID = glutGetWindow();
	glutSetWindow( editWin );
	glutSetWindowTitle( theEditorTitle );
	setWorldTitle( TextEditorWorldInfoString );
	glutSetWindow( oldID );
}


void resetTextEditor( void ) {
	//      printf("initializing text editor\n");
	
	if( textEditorContent != NULL ){
		free( textEditorContent );
		textEditorContent = NULL;
	}
	textEditorContent = readTextEditorContent();
	
	if(textEditorContent == NULL ) {
		textEditorContent = (char *)malloc( 1 );
		*textEditorContent = '\0';
		inputPosition = 0;
	}
	
	scale = jpPrefs.textSize * 1./theEditCharWidth;
	//      printf("initializing text editor ... done\n");
	
}

void _resetTextEditor( void ) {
	
	if(textEditorContent )
		free(textEditorContent);
	
	textEditorContent = (char *)malloc( 1 );
	if( textEditorContent )
		*textEditorContent = '\0';
	else {
		_myPrintf("cant allocate memory for text editor\n" );
		getchar();
		exit( 0 );
	}
	
	sideMove = true;
	setInputPatternCode( jpPrefs.modeSwitchCode );
	lineOffset = 0;
	inputPosition = 0;
	
}


void redrawTextEditor( void ) {
	GLint id = glutGetWindow();
	
	if( editWin ) {
		glutSetWindow( editWin);
		glutPostRedisplay();
	}
	
	glutSetWindow(id);
	
	
}


void clearPattern( void ) {
	
	*textEditorFullPathAndName='\0' ;
	editorPathValid = false;
	saveAsTempFile();
	_resetTextEditor();
	redrawTextEditor();
	
}




int getErrPos( void ) {
	int errPos;
	long pd;
	
	
	pd = st.parsePointer - st.parseStart;
	if( proFile )
		errPos = pd - strlen(proFile) - 2;
	else
		errPos = pd - 2;
	
	if( errPos < 0 )
		errPos =0;
	
	if( errPos >= strlen( textEditorContent ) )
		errPos = strlen( textEditorContent );
	
	return  errPos;
}


void edit2CdewWinmode( int dummy ) {
	
	skipAnimateCdewPattern = true;
	generateCausalNow = true;
	nHandedToCausal(textEditorContent, jugglerNumberForNHandedPatterns);
	generateCausalNow = false;
	skipAnimateCdewPattern = false;
	
}

void edit2Cdew( int dummy ) {
#pragma unused( dummy )
	
#if USE_CDEW
	// dont write help text
	if( jpPrefs.helpActive == true )
		return;
	
	_edit2Cdew( textEditorContent );
#endif
	
}

int _edit2Cdew( char *buf ) {
	int id = 0;
#if USE_CDEW
	
	
	writeFile( edit2CdewTempPath, "w", buf, strlen(buf));
	
	//      disableTransferToTextEditor = true;
	
	id = getLastUsedWindow();
	if(id ) {
		writePatternToOglwin( id, edit2CdewTempPath, true );
		glutSetWindow( id );
		setOglwinSize( id );
		glutPostRedisplay();
	} else
		id = _edit2NewCdew( buf );
	
	//      disableTransferToTextEditor = false;
	
#endif
	
	return id;
	
}

void edit2NewCdew( int dummy ) {
#pragma unused( dummy )
	
#if USE_CDEW
	
	// dont write help text
	if( jpPrefs.helpActive == true )
		return;
	
	_edit2NewCdew( textEditorContent );
	
#endif
}

int _edit2NewCdew( char *buf ) {
	int id = 0;
	
#if USE_CDEW
	
	//      disableTransferToTextEditor = true;
	writeFile( edit2NewCdewTempPath, "w", buf, strlen(buf));
	
	id = makeOglwin( "text editor pattern" );
	if( id ) {
		setOglwinSize( id );
		writePatternToOglwin( id, edit2NewCdewTempPath, true );
	}
	//      disableTransferToTextEditor = false;
#endif
	return id;
	
}


void editMenuSelection( int id ) {
	glutSetWindow(editWin);
	_menuSelect( editMenuList, id );
}

void editRedisplayFunc( int dummy ) {
#pragma unused( dummy )
	redrawTextEditor();
	
}



void editOpenFunc( int dummy ) {
#pragma unused( dummy )
	saveAsTempFile();
	
	openTextEditorPattern();
	if( jpPrefs.textEditorAutoStartFlag)
		editStartFunc(0);
}

void editSaveFunc( int dummy ) {
#pragma unused( dummy )
	
	saveTextEditorContent();
	editorContendChanged = false;
}
void editSaveAsFunc( int dummy ) {
#pragma unused( dummy )
	
	saveTextEditorContentWithName();
	editorContendChanged = false;
}

void editClearFunc( int dummy ) {
#pragma unused( dummy )
	clearPattern();
	jpPrefs.helpActive = false;
}

void vanillaFunc( int dummy ) {
#pragma unused( dummy )
	setInputPatternCode(VanillaCode);
	extendedFlag = false;
}
void extendedFunc( int dummy ) {
#pragma unused( dummy )
	setInputPatternCode(ExtendedCode);
	extendedFlag = true;
}
void switchModeFunc( int dummy ) {
#pragma unused( dummy )
	setInputPatternCode(extendedFlag? ExtendedCode:VanillaCode);
	_myPrintf("mode = %d\n", extendedFlag);
}



void cursorFunc( int dummy ) {
#pragma unused( dummy )
	if( jpPrefs.useFullCursor)
		cursorVisible = false;
	
	redrawTextEditor();
}


void editCloseFunc( int dummy ) {
#pragma unused( dummy )
	
	closeTextEditor();
}


void j2HelpFunc( int dummy ) {
#pragma unused( dummy )
	
	jpPrefs.helpActive = true;
	editorDisplaySwitch = editorShowJ2HelpText;
	
	redrawTextEditor();
}

void callJ2( int dummy ) {
#define MAX_J2_ARGS 30
	int argc = 1;
	char *argv[MAX_J2_ARGS];
	char *p;
	char *t;
	char *s = " \n\r\t";
	char me[] = "JoePass";
	
	
	
#pragma unused( dummy )
	//_myPrintf("call j2\n");
	
	
	//      p = textEditorContent;
	p = currentLineToMem();
	//_myPrintf("%s\n", p );
	t = strtok( p, s);
	
	argv[0] = me;
	
	while( t ) {
		if(strstr( t, "j2" ) == NULL ) {
			
			if( argc >= MAX_J2_ARGS ) {
				_myPrintf("j2 called with too many arguments\n");
				return;
			}
			argv[argc] = t;
			argc++;
		}
		t = strtok( NULL, s );
	}
	
#if 0
	for( c = 0; c < argc; c++ )
		_myPrintf(">%s<\n", argv[c]);
#endif
	
	
	j2( argc, argv);
	
	free( p );
	
	free( textEditorContent);
	textEditorContent = NULL;
	
	textEditorContent = _readTextEditorPattern( J2_OUTPUT_FIlE);
	
	recalcEditorLine();
	redrawTextEditor();
}

/*
 // asumes user wants std siteswap
 char *copyOneLine( void )
 {
 
 return p1;
 }
 */

void editShowHelp(  int dummy ) {
#pragma unused( dummy )
	if( jpPrefs.helpActive )
		editorDisplaySwitch = editorShowHelpText;
	else
		editorDisplaySwitch = editorShowEditorContent;

	redrawTextEditor();
}

void lineJuggle( int dummy ) {
#pragma unused( dummy )
	char *p,*p1;
	char *bString = "#sb";
	
	
	p = currentLineToMem( );
	if( p == NULL )
		return;
	
	
	p1 = (char*)malloc( strlen(bString)  + 1 + strlen( p ) + 1 );
	if( p1 == NULL ) {
		free( p );
		return;
	}
	sprintf(p1, "%s %s", bString, p );
	
	if( !startOnePersonPassNotation(p, true))
		startPattern(p1);
	free( p1 );
	free( p );
	
}


void lineJuggleNHands( int dummy ) {
#pragma unused( dummy )
	char *p1;
	
	p1 = currentLineToMem();
	if( p1 == NULL )
		return;
	nHandedSiteswap( p1, jugglerNumberForNHandedPatterns);
	
	free( p1 );
}


char *currentLineToMem( void ) {
	char *p;
	
	char *lineStart;
	int curPos = inputPosition;
	int n;
	
	// find line-endig before cursor position
	while( curPos ) {
		char t;
		
		t = textEditorContent[curPos-1];
		if( t == '\0' || t == newlineCode )
			break;
		else
			curPos--;
		
	}
	
	
	lineStart = textEditorContent + curPos;
	
	// count characters in line
	n = 0;
	
	// find line-ending     after cursorPosition
	while( textEditorContent[curPos] != '\0' ) {
		if( textEditorContent[curPos] == newlineCode )
			break;
		
		curPos++;
		n++;
	}
	
	p = (char *)malloc( n + 1);
	strncpy( p, lineStart, n );
	*(p+n) = '\0';
	
	{
		int c;
		int l = strlen( p );
		
		if( l )
			for( c = l-1; c > 0; c-- ) {
				if( p[c] == ' ')
					p[c] = '\0';
				else
					break;
			}
	}
	
	
	return p;
}

/*
 // compares input with actual size and returns value according to bool operation
 bool hasWindowSizeChanged( int x, int y, int w, int h)
 {       int x1, y1, h1, w1;
 getWindowSize( &x1, &y1, &w1, &h1);
 return !( x1 == x && y1 ==y && w1 == w && h1 == h);
 }
 */





// writes a file specified by a access path to the text editor
bool writeToTextEditor( char *path ) {
	char *buf;
	bool ret;
	
	if(editorContendChanged == true ){
//		showDialog( "please close editor first");
		//\nsecond line\nthird line\n""what shall I do with the current editor content?\n[s]ave, [d]iscard, [a]bort action");
		return false;
	}
		
	buf = readFile( path, "r" );
	if( buf == 0 )
		return false;
	
	ret = saveFile(buf, editPath);
	
	free( buf );
	
	if( !editWin )
		createTextEditor();
	else
		resetTextEditor();

	setTextEditorNames(path);
	setEditorWindowName();
	
	jpPrefs.helpActive = false;
	editorDisplaySwitch = editorShowEditorContent;
	editRedisplayFunc( -1 );
	editorContendChanged = false;
	
	return ret;
}


/*
 save buffer to file
 returns true if succeeded
*/
bool saveFile( char *buf, char *path ) {
	FILE *fp;
	bool ret;
	size_t l, t;
	
	if( buf == 0 )
		return false;
	
	
	
	fp = fopen( path, "w");
	if( fp == 0 )
		return NULL;
	
	l = strlen( buf );
	t = fwrite( buf, 1, l , fp );
	ret = (t == l );
	
	fclose( fp );
	
	return ret;
	
}

char *generateTempFileName( int c ) {
	sprintf(tempFileName, "%s_%d", TEMP_FILE_NAME, c);
	
	return tempFileName;
}






void saveAsTempFile( void ) {
	return;
	
#if 0
	//yyy not working for unknown reason
	char *p;
	
	printf("saveing text editor temp file\n");
	writeTextEditorContent();
	p = readFile( editPath, "r");
	printf("$p = %08x, p = %s\n", p, p);
	getchar();
	if( p ) {
		char *n;
		n = generateTempFileName(tempCounter);
		writeFile( n, "w", p, strlen( p ));
		//              free( p );
		p = 0;
	}
	
	tempCounter++;
#endif
	
}



// draw a simple flashing cursor
void showCursor( void ) {
	//return;
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, jpPrefs.patternWinSizeW, 0, jpPrefs.patternWinSizeH, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glRasterPos2f(cursorX,cursorY);
	setColorFromVector(red);
	
	if( cursorVisible  )
		setColorFromVector(black);
	else
		setColorFromVector(white);
	
#if 01
	
	glBegin( GL_QUADS);
	glVertex2f( cursorX + theEditCharWidth / 2., cursorY);
	glVertex2f( cursorX+jpPrefs.idleCursorWidth + theEditCharWidth / 2., cursorY);
	glVertex2f( cursorX+jpPrefs.idleCursorWidth + theEditCharWidth / 2., cursorY+15);
	glVertex2f( cursorX + theEditCharWidth / 2., cursorY+15);
	glEnd();
#else
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '|');
#endif
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}


// draw a simple flashing cursor
void showMemoryInfo( void ) {
#if 0
	_myPrintf("ip = %03d\n",  inputPosition);
#endif
}


void _addKey( unsigned char theKey) {
	char *p;
	
#if 01
	
	char t;
	
	size_t l = strlen( textEditorContent ) + 1 + 1; // length + new character + terminator
	
	
	p = (char *)malloc( l );
	
	if(p == 0 )
		return;
	
	t = textEditorContent[inputPosition];
	textEditorContent[inputPosition] = '\0';
	sprintf(p, "%s%c", textEditorContent, theKey);
	textEditorContent[inputPosition] = t;
	sprintf(p+inputPosition+1, "%s", textEditorContent + inputPosition);
	
	inputPosition++;
	free( textEditorContent );
	textEditorContent = p;
#else
	
	size_t l = strlen( textEditorContent ) ;
	int c;
	
	p = realloc( textEditorContent, l + 1 + 1);
	
#if 01
	
	for( c = l ; c >= inputPosition; c--)
		*(p+c+1) = *(p+c);
	
#else
	
	strcpy( p+inputPosition+1, p+inputPosition);
#endif
	
	*(p+inputPosition) = theKey;
	*(p+l) = '\0';
	
	textEditorContent = p;
	inputPosition++;
	
#endif
	
}




void handleUpKey( void ) {
	int c;
	int lineNr = 0;
	int posInLine;
	c = inputPosition;
	
	
	for( c = 0; c < inputPosition; c++) {
		if( textEditorContent[c] == newlineCode ) {
			lineNr++;
			posInLine = 0;
		} else
			posInLine++;
	}
	
	lineNr--;
	
	if( lineNr < 0 ) {
		inputPosition = 0;
		return;
	}
	for( c = 0; lineNr > 0; c++ )
		if( textEditorContent[c] == newlineCode )
			lineNr--;
	// always go to the same column if we just walk up/down
	if( sideMove ) {
		savedPosInLine = posInLine;
		sideMove = false;
	} else
		posInLine = savedPosInLine;
	for( inputPosition = c; posInLine > 0; inputPosition++, posInLine-- )
		if( textEditorContent[inputPosition] == newlineCode )
			break;
	
	internalOffset--;
	internalOffset = max( 0 , internalOffset);
}
// comments see handleUpKey()
void handleDownKey( void ) {
	int c;
	int lineNr = 0;
	int posInLine = 0;
	int m = strlen( textEditorContent);
	
	c = inputPosition;
	
	// get actual line
	for( c = 0; c < inputPosition; c++) {
		if( textEditorContent[c] == newlineCode ) {
			lineNr++;
			posInLine = 0;
		} else
			posInLine++;
	}
	
	// fine next line
	lineNr++;
	
	for( c = 0; lineNr > 0 && c < m; c++ )
		if( textEditorContent[c] == newlineCode )
			lineNr--;
	
	if( c == m ) {
		inputPosition = m;
		return;
	}
	
	if( sideMove ) {
		savedPosInLine = posInLine;
		sideMove = false;
	} else
		posInLine = savedPosInLine;
	
	
	for( inputPosition = c; posInLine > 0; inputPosition++, posInLine-- )
		if( textEditorContent[inputPosition] == newlineCode )
			break;
	
	
	internalOffset++;
	internalOffset = min( linesInEditorWin , internalOffset);
	
}



void handlePageUpKey( void ) {
	int c;
	for( c = 0; c < editorLinesInWindow; c++ )
		handleUpKey();
	
}

void handlePageDownKey( void ) {
	int c;
	for( c = 0; c < editorLinesInWindow; c++ )
		handleDownKey();
	
}


void editCapitalFunc( int dummy ) {
	
#pragma unused (dummy)
	
	size_t c, l;
	
	l = strlen( textEditorContent  );
	for( c = 0; c < l; c++)
		textEditorContent[c] = toupper(textEditorContent[c]);
	
	glutPostRedisplay();
}

void initTextEditor( void ) {
	
	
	new editor();
	_resetTextEditor();
}



editor::editor( void ) {
	
	_menuList       *l,
	_editMenuList[]= {
		
		{"editor window",										menuTitle},
		{languageStrings[languageEditMenuFile],                 newMenu,                        0,              &fileMenu,      editMenuSelection},
		{languageStrings[languageEditFileOpen],                 menuEntry,                      0,              NULL,           editOpenFunc},
		{languageStrings[languageEditFileClose],                menuEntry,                      0,              NULL,           editCloseFunc},
		{languageStrings[languageEditFileSave],					menuEntry,                      0,              NULL,           editSaveFunc},
		{languageStrings[languageEditFileSaveAs],               menuEntry,                      0,              NULL,           editSaveAsFunc},
		
		{languageStrings[languageEditMenuEdit],                 newMenu,                        0,              &editMenu,      editMenuSelection},
		{languageStrings[languageEditClear],                    menuEntry,                      0,              NULL,           editClearFunc},
		{languageStrings[languageEditCapitalize],               menuEntry,                      0,              NULL,           editCapitalFunc},
		{languageStrings[languageEditBlockCursor],              menuEntry,                      0,              NULL,           cursorFunc,           &jpPrefs.useFullCursor},
		{languageStrings[languageEditVanilla],                  menuEntry,                      0,              NULL,           vanillaFunc},
		{languageStrings[languageEditExtended],                 menuEntry,                      0,              NULL,           extendedFunc},
		
		{languageStrings[languageEditMenuSiteswap],             newMenu,                        0,              &siteswapMenu,  editMenuSelection},
		{languageStrings[languageEditPattern],                  menuEntry,                      3,              NULL,           editStartFunc},
//		{languageStrings[languageEditFourHanded],               menuEntry,						0,              NULL,           startEditor4HandPattern},
		{languageStrings[languageEditCurentLine],               menuEntry,                      14,             NULL,           lineJuggle},
//		{languageStrings[languageEditCurentLineFour],           menuEntry,                      15,             NULL,           lineJuggleNHands},
		
		{languageStrings[languageEditMenuCausal],               newMenu,                        0,              &causalMenu,    editMenuSelection},
		{languageStrings[languageEditGenerateCausalOnStart],    menuEntry,                      0,              NULL,           NULL,                 &jpPrefs.generateCausalFromTextEditorOnStart},
		{languageStrings[languageEditGenerateCausalNow],        menuEntry,                      0,              NULL,           edit2CdewWinmode},
		{languageStrings[languageEditCausalSameWindow],         menuEntry,                      0,              NULL,           NULL,                 &generateTextToCausalInSameWindow},
		{languageStrings[languageEditAutoSaveCausal],           menuEntry,                      0,              NULL,           NULL,                 &jpPrefs.autosaveCausalFromTextEditor},
		
		{languageStrings[languageEditMenuJ2],                   newMenu,                        0,              &j2Menu,        editMenuSelection},
		{languageStrings[languageEditCallJ2],                   menuEntry,                      2,              NULL,           callJ2},
		{languageStrings[languageEditJ2Help],                   menuEntry,                      0,              NULL,           j2HelpFunc},
		
		
		{"editorMainMenu",                                      newMenu,                        0,              &editorMenu,    editMenuSelection},
		{languageStrings[languageEditMenuFile],                 addAsSubMenu,                   0,              &fileMenu,      editMenuSelection},
		{languageStrings[languageEditMenuEdit],                 addAsSubMenu,                   0,              &editMenu,      editMenuSelection},
		{languageStrings[languageEditMenuSiteswap],             addAsSubMenu,                   0,              &siteswapMenu,  editMenuSelection},
		{languageStrings[languageEditMenuCausal],               addAsSubMenu,                   0,              &causalMenu,    editMenuSelection},
		{languageStrings[languageEditMenuJ2],                   addAsSubMenu,                   0,              &j2Menu,        editMenuSelection},
		{languageStrings[languageEditMenuHelp],                 menuEntry,                      0,              NULL,           editShowHelp,      &jpPrefs.helpActive },
		{languageStrings[languageAllMenusOpenDoc],				menuEntry,                      '?',			NULL,			openDocFunc},

		{languageStrings[languageAllMenusQuit],					menuEntry,                      0,              NULL,           animationQuit},
		{NULL,                                                  menuListTerminator,             0,              NULL,           0}      // last entry
	};
	
	
	// alloc memory for menuList
	l = (_menuList *)malloc( sizeof( _editMenuList ));
	// copy content
	memcpy( l, _editMenuList, sizeof( _editMenuList ));
	
	editMenuList = l;
	
}

editor::~editor(  ) {}

void setTextEditorNames(  char *pathname ) 
{	splitPathName( pathname, textEditorPath, theEditorTitle, textEditorFullPathAndName);
}

void startMusic( char *name)
{
	char *p, *t;
	size_t l;
	
	_myPrintf("playing %s\n", name);	
	
	// file not saved: leave
	if( textEditorFullPathAndName == 0 )
		return;
	
	l = strlen( textEditorFullPathAndName );
	
#if 0
	// play only with saved files 
	if( l == 0 )
		return;
#endif
	
	l += strlen(name );
	p = (char *)malloc( l );
	
	sprintf(p, "%s", textEditorFullPathAndName);
	
	t = strrchr( p, PATH_DIVIDER );
	if( t )
		*(t+1) = '\0';
	
	
	strcat(p, name);
	
	_myPrintf("trying to play song %s\n", p);
	
}

void forceRestardForUnchangedFile( void )
{
	
	forceRestardFlag = true;
}

bool checkForFileChanged( void )
{
	struct tm* tNow;				// create a time structure
	static tm tLast;
	bool ret = false;
	struct stat attrib;					// create a file attribute structure
	
	if( strlen( textEditorFullPathAndName) == 0 )
		return false;
	
	stat(textEditorFullPathAndName, &attrib);
	tNow = gmtime(&(attrib.st_mtime));	
	if(forceRestardFlag || compareTime( &tLast, tNow) ){
		reloadTextEditorPattern();
		
		editStartFunc( 0 );

		forceRestardFlag = false;

		ret = true;
	}
	tLast = *tNow;	
	return ret;
}


bool reloadTextEditorPattern( void )
{
	char *buf;
	bool ret = false;
	
	
	if( strlen( textEditorFullPathAndName ) != 0 ){
		buf = readFile(textEditorFullPathAndName, "r");
		buf = changeLineendings( buf );
		
		if( buf != NULL ) {
			jpPrefs.helpActive = false;
			resetTextEditor();
			
			
			textEditorContent = buf;
			redrawTextEditor();
			ret = true;
		}
	}
	
	return ret;
}

char *copyString( char *in)
{
	char *out = (char *)malloc( strlen(in) + 1);
	sprintf(out, "%s", in);
	return out;
}
	
void editStartFunc( int dummy ) {
#pragma unused( dummy )
	
	_editStartFunc( textEditorContent );
	
	// bring my editor window back to front (mac only)
	glutSetWindow(editWin);
	glutShowWindow();
	
}

#define JugglerNumberFlg1 "#JugglersInPattern"
#define JugglerNumberFlg2 "#jugglersInPattern"
void _editStartFunc( char *in)
{	char *p, *q;
	char *pattern;
	
    
	if( textEditorContent == NULL){
        textEditorContent = (char *)malloc(strlen(in)+100);
        if( textEditorContent == NULL )
            return;
    }
	// expand the pattern
	
	animationActive = false; // we toggle this switch in a moment
	visible(GLUT_VISIBLE);
	_toggleAnimationActive( true );
	
	pattern = copyString( in );

	// check if the number of jugglers is written in the pattern
	q = p = strstr( pattern, JugglerNumberFlg1);
	if( p == NULL )
		q = p = strstr( pattern, JugglerNumberFlg2);
	
	// only if it is not commented out
	while(q && *q != lineCommentCode && *q != newlineCode && q >= pattern)
		q--;
	
	// p points to keyword, q to comment, new line or first char in pattern 
	if( p && *q != lineCommentCode){
		int n;
		
		// find keyword (same length for both keyword strings!)
		p += strlen (JugglerNumberFlg1) ;
		
		while( *p && !isdigit(*p ) && *p != newlineCode)
			p++;
		
		// scan value
		sscanf(p, "%d", &n);
		
		// jump to end of line
		while( *p && *p != newlineCode )
			p++;
		
		// execute pattern
		nHandedSiteswap( p, n*2);		
		
		free( pattern);
		return;
	}
	
	
	// j2 command? single line innput!
	p = currentLineToMem( );
	if( p != NULL ) {
		if(strstr( p, "j2" ) == p ) {
			callJ2( 0 );
			free( p );
			free( pattern );
			return;
		}
		free( p );
	}
	
	
#ifdef USE_CDEW	
	setAllWindowsInactive( );
#endif
	bool ret;
	//falls anzahl der Jongleure gesetzt ist:
	//Muster mit dieser Zahl erzeugen
	if( jugglerNumberForNHandedPatternsDefined )
		ret = nHandedSiteswap( pattern, jugglerNumberForNHandedPatterns);
	else 	
		ret = startOnePersonPassNotation(pattern, true);
//	_myPrintf("ret = %d\n", ret);
	
	if(!ret){
		_startEditorPattern(pattern);
		if(  jpPrefs.generateCausalFromTextEditorOnStart )
			nHandedToCausal(pattern, jugglerNumberForNHandedPatterns);
	}
	
	setWorldTitle( TextEditorWorldInfoString );
	
	free( pattern );
}


void _startEditorPattern( char *in ) {
	char *p;
	
	
	if( strstr(in, modeSwitchString) == NULL )
	{	p = (char *)malloc( strlen(modeSwitchString) + strlen(in) +2+ 1);
		sprintf(p, "%s\n%s", modeSwitchString,in);
	}
	
	startPattern( p);
	
	
	textEditorStartedPattern = true;
	
	if( validPattern)
		setWorldTitle( TextEditorWorldInfoString );
	else if( jpPrefs.jumpToErrPosition )
		inputPosition = getErrPos();
	
	free( p );
	
	redrawTextEditor();
	
}
