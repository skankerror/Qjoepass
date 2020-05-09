/*
 *  animationWin.cpp
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
#include "systemswitch.h"
#include "graphics.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#ifdef WINDOWS_VERSION
#include <windows.h>
#include <ShellAPI.h>
#include <Process.h>
#endif

#include "animationClass.h"
#include "editor.h"
#include "globals.h"
#include "mydefs.h"
#include "cameraControl.h"
#include "animationWin.h"

#include "languageSupport.h"

#include "runaroundMenu.h"
#include "listWindow.h"
#include "myutil.h"
#include "playSound.h"
#include "myPrintf.h"
#include "fileaccess.h"

#include "causal_editor.h"
#include "openglmenu.h"
#include "movie.h"

#include "exitModules.h"
#include "fileaccess.h"


#include "mainGL.h"
#include "info.h"
#include "editor.h"
#include "jugglers.h"
#include "editor.h"
#include "applicationFolders.h"
#include "texture.h"
#include "fileIO.h"
#include "pattern.h"
#include "jpPrefs.h"
#include "mem.h"
#include "causal_editor.h"
#include "parseSiteswap.h"
#include "siteswap.h"
#include "styleScanner.h"

#include "world.h"
#include "definitions.h"
#include "workspace.h"

#include "cdewDefs.h"
#include "runaround.h"
#include "movie.h"
#include "pathAccess.h"
#include "notation.h"
#include "cdewClass.h"
#include "cdewMenu.h"
#include "animationView.h"

#include "cmdLine.h"

#include "fileHandler.h"


int	fixedWindowSizeX = 5000;
int fixedWindowSizeY = 4000;


#define MIN_DISTANCE_FACTOR					1.3f
#define NumberOfHeightValues				10
#define	FPS_MOVIE							10
#define	TRIGGER								60
#define JPANIMATIONDEFAULTFPS               60
#if 01
// use timer function to define framerate
// speed defined in jpPrefs.txt jpAnimationFPS
#define	glutIdleFunc(jpIdle)				glutIdleFunc(NULL)
#else
// use glut idle function, i.e. redraw as often as possible
// uses a lot of system resources
#define glutTimerFunc(jpTimerFireringSpeed , callWorldIdle, a) glutIdleFunc(jpIdle)
#endif


#ifdef WINDOWS_VERSION
HWND windowsWinHandle						= NULL;
#endif


movie	*myMovie							= NULL;
void	finishMovie( void );


int		viewMode							= BIRD_VIEW;

char	*animationWindowTitle				= AnimationWindowTitle;

float fieldOfViewY							= 60.f;
int		lastFullscreenW,
		lastFullscreenH,
		lastFullscreenX,
		lastFullscreenY;

bool	useDefaultAnimationValues			= false;     
int		jpAnimationFPS						= JPANIMATIONDEFAULTFPS;
int		jpTimerFireringSpeed;
bool	jpAnimate							= true;
bool	jpUntilNextThrow					= false;
bool	animationActive						= true;

// material
GLfloat jugglerMaterialSpecular[]			= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat jugglerMaterialShininess[]			= { 10.0f };
GLfloat jugglerMaterialEmission[]			= { 0.f, 0.f, 0.f, 0.0f };
GLfloat objMaterialSpecular[]				= {	1.0f, 1.0f, 1.0f, 0.0f };
GLfloat objMaterialShininess[]				= { 50.0f };
GLfloat objMaterialEmission[]				= { .0f, .0f, .0f, 0.0f };

// light
GLfloat specular0[]							= { 0.93f, .930f, .930f, 1.0f };
GLfloat diffuse0[]							= { 0.830f, 0.830f, 0.830f, 1.0f };
GLfloat ambient0[]							= { 0.530f, 0.530f, 0.530f, 1.0f };
GLfloat lmodel_ambient[]					= { 0.3f, 0.3f, 0.3f, 1.0f };


int		theFrameRate						= 1;
int		mouse_state, mouse_button;

int		frameCount;
int		frameRateState;
int		warnungVorDemMenu					= 0;
bool	redrawWorldWin						= true;


float	defaultSpeedList[NumberOfHeightValues] = {2.00, 2.00, 2.00, 2.90, 3.40, 4.10, 4.25, 5.00, 5.00, 5.50};

int		styleEntries						= 0;
bool	allTextureFlag						= false;

static int
fileMenu,
workspaceMenu,
objectMenu,
mediaMenu,
texMenu,
envMenu,
winMenu,
viewMenu,
cameraMenu,
abstractMenu,
walkMenu,
rendererMenu,
colorMenu,
animMenu,
menuMenu,	
styleMenu,
positionMenu,
soundMenu;

_menuList *worldMenuList;





/* world */
void	jpIdle( void );
void    toggleAnimation( int dummy );
void    callWorldIdle( int dummy );

void    oneStepOfAnimation( int dummy );
void	untilNextThrow( int dummy );

//void  worldFullscreen(void);
void	animationMouse(int button, int state, int x, int y);
void	animationMotion(int x, int y);
void	worldReshape(int w, int h);
void	worldMenuSelection( int r );

/* styles */
void	addStyleMenu( bool remove);
void	style_select(int item);

/* positions */
void	addPositionMenu( bool remove);
void	position_select(int item);

void	setAllTextureFlag( int dummy );

void    setupAnimationWindow( void );
void    setupRenderMode( wwwBoolean mode );
void    setupFog( wwwBoolean showFog );
void    setupShader( int mode );
void    setAntialias( void );
void	setupRenderer( void );
void	setupProjection( void );


void    
editorWindowFunc( int dummy ),
causalWindowFunc( int dummy ),
causalDuplicateFunc( int dummy ),
runaroundWindowFunc( int dummy ),
infoWindowFunc( int dummy ),
animateFunc( int dummy  ),
openInFunc( int dummy  ),
editAnimFunc( int dummy ),
saveFunc( int dummy  ),
restartFunc( int dummy ),
editFunc( int dummy ),
openWorkspaceFunc( int dummy ),
saveWorkspaceAsFunc( int dummy ),
editExternFunc( int dummy ),
biggerWorldFunc( int dummy  ),
smallerWorldFunc( int dummy  ),
biggerObjectFunc( int dummy  ),
smallerObjectFunc( int dummy  ),
//savePrefsFunc( int dummy  ),

#ifdef MAKE_QUICKTIME_MOVIE
quicktimeMovieFunc( int dummy ),
//quicktimeCompressorFunc( int dummy ),
#endif

movieByScreenshots( int dummy ),
screenshotFunc( int dummy ),
randFunc( int dummy ),
clubsFunc( int dummy ),
fishFunc( int dummy ),
ballsFunc( int dummy ),
ringsFunc( int dummy ),
pancacesFunc( int dummy ),
nothingFunc( int dummy ),
addSpinFunc( int dummy ),
subSpinFunc( int dummy ),
defSpinFunc( int dummy ),
fullScreenFunc( int dummy ),
vomitCamFunc( int dummy ),
tessCamFunc( int dummy ),
objecttCamFunc( int dummy ),
resetMagFunc( int dummy ),

setTextureHead( int dummy ),
setTextureSky( int dummy ),
setTextureFloor( int dummy ),
setTextureObject( int dummy ),
setTextureClub( int dummy ),
setAllTextures( int dummy ),

fasterFunc(  int dummy ),
slowerFunc(  int dummy ),
slowMotionFunc( int dummy),
higherFunc(  int dummy ),
lowerFunc(  int dummy ),
dropFunc(  int dummy ),
warningNarrowFunc(  int dummy ),
warningWideFunc(  int dummy ),
jugglerColorFunc(  int dummy ),
objectColorFunc(  int dummy ),
remakeMenu(  int dummy ),
toggleAnimationActive( int dummy ),

setAllTextures( int dummy),
texFloorFunc( int dummy ),
texSkyFunc( int dummy ),
setAnimationToDefaultValues( int dummy ),
setTapAnimationValues( int dummy ),
nextSoundKindFunc( int dummy ),
animationAbout( int dummy ),
flip1p( int dummy ),
selectWatchFile( int dummy ),
selectExternalEditor( int dummy ),
toggleTableJugglingMode( int dummy ),
setDefaultAnimationSpeed( int dummy );

void	makeAnimationMenu( void );
void	setAllObjects( int kind );

void	initTextures(void);
bool	init1Texture( char *textureFilename, GLuint myIndex);

void	calcFrameRate( int now );
void	showFrameRate( void );
void	calcTimeStep( void );
void	initGlutWindow(void);
void	deleteAnimationWindow( void );

void	initMaterial(bool mode);
void	initLight(bool mode);
void	dayLight( bool mode );

void	redisplayWorldWindow( void );

void	checkForScreensaverStillBusy( int now );
void 	passiveMotion( int x, int y );


void	menuFunc( int state, int x, int y );

void	checkGlutError( void );

void check_dndfile( void );



void toggleAnimationActive( int dummy ) {
#pragma unused( dummy )
	_toggleAnimationActive(false);
}


void _toggleAnimationActive( bool force  ) {
	static bool lastInfoState = false;
	static bool lastEditorState = false;
	extern int infoWin, editWin;

	
	if(!force && jpPrefs.causalEditorHasOpenWindows == false )
		return;

	toggle (animationActive);
	
	glutIdleFunc(jpIdle);
	if( animationActive == false ) {
		if( infoWin )
			lastInfoState = true;
		
		if( editWin )
			lastEditorState = true;
		
		
		closeTextEditor();
		createInfoWindow(false);
		
		glutSetWindow( animationWin);
		glutHideWindow();
		
	} else {
//		glutIdleFunc(jpIdle);
		if( lastEditorState )
			createTextEditor();
		
		if( lastInfoState )
			createInfoWindow(true);
		
		
		glutSetWindow( animationWin);
		glutShowWindow();
		
	}
}

void menuFunc( int state, int x, int y ) {
#pragma unused (x,y)
	_myPrintf("state = %d\n", state);
	if( state == GLUT_MENU_IN_USE )
		warnungVorDemMenu = true;
	else
		warnungVorDemMenu = false;
	
	
}


void createAnimationWindow( void ) {
	
	printf("\ncreating animation window\n");
	
	jpTimerFireringSpeed = 1000 / jpAnimationFPS;
	
	// World window
	glutInitWindowPosition(jpPrefs.worldWinSizeX , jpPrefs.worldWinSizeY);
	glutInitWindowSize(jpPrefs.worldWinSizeW, jpPrefs.worldWinSizeH);
	atexit( deleteAnimationWindow);
	
#if 0
	if( screenSaverMode ) {
		glutGameModeString("640x480:16@60");
		glutEnterGameMode();
		//      animationWin = 1;
	}
#else
	animationWin = glutCreateWindow(animationWindowTitle);
#endif
	
	setWorldTitle("untitled");
	
	initGlutWindow();
	glutReshapeFunc(worldReshape);
	glutDisplayFunc(drawScene);
	glutMouseFunc(animationMouse);
	glutMotionFunc(animationMotion);
	glutPassiveMotionFunc( passiveMotion);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(worldKey);
	glutSpecialFunc(worldSpecialKey);
	glutTimerFunc (jpTimerFireringSpeed , callWorldIdle, 1);
	
	//      glutMenuStatusFunc(menuFunc);
	
	setAllTextureFlag( 0 );
	
	initMenu(worldMenuList, true);
	
	addStyleMenu( false );
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	addPositionMenu( false );
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glutSetWindow( animationWin);
	
	initTextures();
	
	_myPrintf("animationWinID = %d\n", animationWin);
}

void deleteAnimationWindow( void ) {
	
	if( animationWin ) {
		glutDestroyWindow(animationWin);
		_myPrintf("animation window closed\n");
		animationWin = 0;
		
	} else
		_myPrintf("animation window was already closed\n");
	
	
}


void initGlutWindow(void) {
	
	setupAnimationWindow();
	initLight(true);
	initMaterial(true);
	setupRenderer( );
	setupProjection( );
}

void setupAnimationWindow( void ) {
	setupFog( jpPrefs.fog);
	setupRenderMode( jpPrefs.solidWorld );
	setupShader( jpPrefs.shaderMode );
	setAntialias();
	glDisable(GL_CULL_FACE);
}

void setupShader( int mode ) {
	glShadeModel(mode);
}
void setAntialias( void ) {
	if( jpPrefs.antialias ) {
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	} else {
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
}
void setupRenderer( void ){
	// set Render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
}
void setupRenderMode( wwwBoolean mode ) {
	if( mode ) {//     glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {//     glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_NORMALIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void setupProjection( void ){
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 1.33, .1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

void setupFog( wwwBoolean showFog ) {
	GLfloat fc[4] = {jpPrefs.fog_color.x, jpPrefs.fog_color.y, jpPrefs.fog_color.z, 1.};
	
	if( showFog ) {
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_DENSITY, jpPrefs.fogDensity);
		glFogfv(GL_FOG_COLOR, fc);
	} else
		glDisable(GL_FOG);
}



void initMaterial(bool mode) {
#pragma unused(mode)
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, objMaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, objMaterialShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,	objMaterialEmission	);
	

}

void initLight( bool mode ) {
#pragma unused (mode )

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void dayLight( bool mode ) {
	
	// night
	if( mode ) {       // day
		GLfloat diffuse[] = { .5f, .5f, .5f, 1.0f };
		GLfloat light_ambient[] = { .2f, .2f, .2f, 0.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		
	} else {
		/*              GLfloat diffuse[] = { .05f, .05f, .05f, 1.0f };
		GLfloat light_ambient[] = { .03f, .03f, .031f, 0.0f };
		*/              GLfloat diffuse[] = { .0f, .0f, .0f, 1.0f };
		GLfloat light_ambient[] = { .0f, .0f, .0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		
	}
}



void passiveMotion(int x, int y ) {
	static bool firstCall = true;
	static int startX, startY;
	
	if( firstCall ) {
		firstCall = false;
		startX = x;
		startY = y;
		return;
	} else if( x == startX && y == startY)
		return;
	
	if(screenSaverMode )
		exit( 0 );
}


void worldReshape(int width, int height) {
	int w,h;
	
	if( !animationActive)
		return;
	
	glutSetWindow( animationWin);
	recalcFrameRate();
	
	w = width & ~3;
	h = height & ~3;
	
	if( 1 ){
		if( w != width || h != height)
		glutReshapeWindow(w,h);
	}else {
		glutReshapeWindow(5000,4000);
	}
	
	jpPrefs.worldWinSizeX = glutGet(GLUT_WINDOW_X);
	jpPrefs.worldWinSizeY = glutGet(GLUT_WINDOW_Y);
	jpPrefs.worldWinSizeW = glutGet(GLUT_WINDOW_WIDTH);
	jpPrefs.worldWinSizeH = glutGet(GLUT_WINDOW_HEIGHT);
	
	
#ifdef __TRACKBALL__
	
	animationViewReshape(width, height);
#endif
	
	glViewport(0, 0, width, height);
	if(!fullscreen) {
		jpPrefs.worldWinSizeW = width;
		jpPrefs.worldWinSizeH = height;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( jpPrefs.fieldOfViewY ,1.0 *  (GLfloat)width/(GLfloat)height, .07, 10e10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

#if 01
//#ifdef __TRACKBALL__


void animationMouse(int button, int mouseState, int x, int y) {
	if(screenSaverMode )
		exit( 0 );
	
	if( jpPrefs.useTapAnimationValues )
	{
		static int d, t;
		
		
		if( mouseState == GLUT_DOWN)
			d = glutGet(GLUT_ELAPSED_TIME);
		
		if( mouseState == GLUT_UP)
		{
			// ignore taps when animationViewMouse button is pressed for more than 1/10 sec. this is most likely a drag or a menu selection
			t = glutGet(GLUT_ELAPSED_TIME) - d;
			//			_myPrintf("t = %d \n",t );
			if( t < 200)
				setTapAnimationValues( 0 );
		}
	}
	
	mouse_state = mouseState;
	mouse_button = button;
	
	// stop film creation
//	if( inMovieCreation )
		finishMovie ();
	
	// stop auto movement
	viewMode = BIRD_VIEW;
	setCameraMode( false );
	
	getModifiers();
	
	if( ckMode )
		return;
	
	animationViewMouse(button, mouseState, x, y);
	
}

void animationMotion(int x, int y) {
/*	if(screenSaverMode )
		exit( 0 );
	recalcFrameRate();
*/
	animationViewMotion(x, y);

}

#endif

void visible(int vis) {
	static int last = GLUT_NOT_VISIBLE;//GLUT_VISIBLE;
	glutSetWindow( animationWin);
	if (vis == GLUT_VISIBLE) {
		glutIdleFunc(jpIdle);
	} else {
		glutIdleFunc(jpIdle);
//		glutIdleFunc(NULL);
	}
	last = vis;
}

void _cdewDisplay(void);


enum {addMovieFrameDo, addMovieFrameDone, addMovieFrameNojob};
int addMovieFrameFlag = addMovieFrameNojob;
void animationAddFrameToMovie( void );


void drawScene(void) {
	int now ;
	bool drawNow = true;
	
	
	if( cmdLinePos){
		_myPrintf("drawScene: setting camera to %d\n", cmdLinePos);
		switch (cmdLinePos ){
			case 1: frontFunc( 0 );	break;
			case 3: nextPosFunc( 0 );	break;
			case 5: moveCamFunc( 0 );	break;
			case 7: topFunc( 0 );		break;
			case 9: birdFunc( 0 );		break;
			default: break;
		}
		cmdLinePos = 0 ;
	}
	
	if( cmdLineToCdew ){
		char *p;
		oglwin *o;
		_myPrintf("generating causal from command line pattern\n");
		if( (p = readFile( cmdLinePath, "rb")) == NULL )
			_myPrintf("-> file not found\n");
		else{
			_myPrintf("input file is\n\n%s\n\n", p);

			if( strstr( p, cdewVersionString) ) {
				int id;
				
				//      writeFile( edit2NewCdewTempPath, "w", textEditorContent, strlen(textEditorContent));
				id = makeOglwin( thePatternTitle );
				if( id ) {
					writePatternToOglwin( id, fullPatternPathName, false );
					setOglwinSize( id );
					cdewJugglerColor( 0 );
				}
			}else
				nHandedToCausal( p, jugglersInCommandLinePattern * 2);	
			free( p );
		}
		o = getActualOglwin();
		if( cmdLineMakePostscript ){
			cdewSavePS( -0 );
			_cdewDisplay();
			cmdLineMakePostscript = false;
		}
		
		o->saveDiagramWithName( "cmdLineToCausal.pass" );
		o->fileChangedFlag = false;
		cmdLineToCdew = false;
	}
	
	
	if( cmdLineInitMovie ){
		if( !myMovie ){
			cmdLineInitMovie = false;
			myMovie = new movie( screenshotMode );
		}
	}
	
	if( !animationActive)
		return;
	
	glutSetWindow( animationWin);
	getWindowPosition(&jpPrefs.worldWinSizeX, &jpPrefs.worldWinSizeY);
	
	if( useDefaultAnimationValues ){
		setAnimationToDefaultValues( 1 );
		useDefaultAnimationValues = false;
	}
	
	
	if(warnungVorDemMenu )
		_myPrintf("drawing animation while menu is active\n");
	
	
	// what time is it?
	now = glutGet(GLUT_ELAPSED_TIME);
	
	checkForScreensaverStillBusy( now );

	// if started by cmd line and quit flag set:
	// exif when pending jobs have been finished
	if( cmdLineDoJobsAndExit ){
		if( !(myMovie			// movie must have been started by cmdLine if we reach this point
		     || cmdLineToCdew			// the causal is already created here
			 || cmdLineMakePostscript	// same for the postscript file 
			 ))
			printf("exiting from cmd-line\n");
			exit( 0 );
	}
	
	// draw content
	if( animationActive )
		glClearColor(jpPrefs.backgroundColor.x, jpPrefs.backgroundColor.y, jpPrefs.backgroundColor.z, 0.0);
	else
		glClearColor( .3f,.3f,.3f,1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();


	// move camera in model
	switch( viewMode ){
		case OBJECT_VIEW:
			objectSetView();
			break;
		case VOMIT_VIEW:
			vomitSetView();
			break;
		case TESS_VIEW:
			tessedSetView();
			break;
		case MOVE_CAMERA_VIEW:
			cameraSetView();
			animationCameraView();
			break;
		case JUGGLER_VIEW:
			setCameraToActualJuggerView();
			animationMatrix();
			break;
		default:
			animationMatrix();
			break;
	}

	glPushMatrix( );

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	if(animationActive ) {
		if( drawNow )
			projectWorld();
	}
		
	glPopMatrix();
	
	//      glPushMatrix();
	
	if((viewMode == JUGGLER_VIEW && jpPrefs.blindfolded) ) {
		punkt p1,p2,p3,p4;
		
		float x = 400.f, y = 400.f, z = -.7001f;
		
		p1 = punkt(-x,-y,z);
		p2 = punkt( x,-y,z);
		p3 = punkt( x, y,z);
		p4 = punkt(-x, y,z);
		
		glColor4f(0.f,0.f,0.f,.85f);
		
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		glBegin(GL_QUADS);
		glNormal3f(0.0, .0, 1.0);
		
		glVertex3f(p1.x,p1.y,p1.z);
		glVertex3f(p2.x,p2.y,p2.z);
		glVertex3f(p3.x,p3.y,p3.z);
		glVertex3f(p4.x,p4.y,p4.z);
		
		
		glEnd();
		glPopMatrix();
		
		glDisable( GL_BLEND);
	}
	
	
	glPopMatrix();
	
#ifdef __MOVIE__	
	if( myMovie ) {
		if( myMovie->isMovieSetupFinished() ){ // compression and file selection dialogs may be non modal (they are under mac os x). We have to wait until the compression info is valid.
			animationAddFrameToMovie();
		}
	} else {
		calcFrameRate( now );
		calcTimeStep();
	}
#else
	calcFrameRate( now );
	calcTimeStep();
#endif	
		

	if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
		glutSwapBuffers();
	else{
		glFlush();
	}

}

void animationAddFrameToMovie( void )
{	
	if (myMovie->addFrameToMovie() == false ){
		// that was the last frame, of another fault happend (disk full?)
		finishMovie();
	}
}

// pass idle information to other windows, too
// world win needs much time, so other windows may not be called
void jpIdle( void ) {
	
	if( animationActive ){
#ifdef WINDOWS_VERSION
		// Obtain the handle of the active window.
		HWND i;
		int l = 256;
		char p[256];
		if( windowsWinHandle == NULL ){
			i = GetForegroundWindow();	
			if( GetWindowText( i, p, 256) > 0 ){
				if( strstr( p, AnimationWindowTitle) > 0 ){
					windowsWinHandle = i;
					
				}
			}
			if( 0 ){
				int pid;
					
				pid = GetWindowThreadProcessId( i,NULL);
				myPrintf("pid = %d\n", pid);
				
			}
		}
		   
#endif
	
		doSiteswap();
		
		redisplayInfoWindow();
		redisplayWorldWindow();
		//idleSounds();
	}	
    checkForWatchfiles( );
	check_dndfile( );
	
	listIdle();
/*	
	if( addMovieFrameFlag == addMovieFrameDo ){
			animationAddFrameToMovie();
		addMovieFrameFlag = addMovieFrameDone;
	}		
*/		
}




// display content of my windows
// only if desired time has passed by
void RedisplayAppWindows(void) {
	return;
	
	redisplayWorldWindow();
	redisplayInfoWindow();
}

void redisplayWorldWindow( void ) {
	if( animationWin) // always true
	{       glutSetWindow(animationWin);
		
		redrawWorldWin = true;
		glutPostRedisplay();
	}
}

/* ---------------------------------------
calculate animationSpeed:
desired speed is in jpPrefs.animationSpeed
this is the time to pass by in one second

calculate actual animationSpeed from

animationSpeed
timeStep = ----------------
fps
-----------------------------------------*/
void calcTimeStep( void ) {
	
	if( jpAnimate == false )
		return;
	if( theFrameRate == 0)
		return;
	
	timeStep = jpPrefs.animationSpeed / theFrameRate;
	timeStep = min(timeStep, .3);
}


// how many frames / sec are drawn, about?
void calcFrameRate( int now ) {
	int dt;
	static int lastTime = initState;
	static int frameRateStartAt;
	//      static int frameCount;
	
	// states
	switch(frameRateState) {
		case defaultState:
			if( now - lastTime > TRIGGER ) {
				recalcFrameRate();
			} else {       // 1 frame up
				frameCount++;
				dt = now - frameRateStartAt;
				if( dt == 0 )
					break;
				// get actual framerate
				theFrameRate = (1000. * frameCount) /dt;
			}
			break;
		case firstState:
			frameRateStartAt = glutGet( GLUT_ELAPSED_TIME);
			frameCount = 0;
			frameRateState = defaultState;
			break;
		case initState:
			// just ask about the time
			frameRateState = firstState;
			break;
	}
	lastTime = now;
	
}

void recalcFrameRate( void ) {
	frameRateState = initState;
}



// make the program be controlable from another application (a screen saver);
// check every second (or so) if a given file exists.
// if so, animationQuit
void checkForScreensaverStillBusy( int now ) {
#define animationQuitScreensaverName "animationQuitScreensaver"
#define CheckScreensaverTime    100
	
	static int lastCheck = -CheckScreensaverTime;
	FILE *fp;
	
	if( !screenSaverMode )
		return;
	
	
	if( now - lastCheck < CheckScreensaverTime)
		return;
	
	lastCheck = now;
	if( (fp = fopen( animationQuitScreensaverName, "r" )) != NULL ) {
		fclose( fp );
		remove
			( animationQuitScreensaverName);
		exit( 0 );
	}
}

void checkGlutError( void ) {
	FILE *fp;
	
	
	return;
#define GLUT_ERR_LOG_FILE "glut_error_log"
	
	if((fp = fopen(GLUT_ERR_LOG_FILE, "r")) != NULL ) {
		printf("problem detected\n");
		fclose( fp );
		remove
			( GLUT_ERR_LOG_FILE );
	}
}

void setWorldTitle( char *t) {
	char *p;
	size_t l;
	char *preName = " (";
	char *postName = ")";
	
	if( animationWin ) {
		
		l = strlen(animationWindowTitle) + strlen(preName) + strlen(t) + strlen(postName) + 1;
		p = (char *) malloc( l );
		
		if( p ) {
			sprintf(p, "%s%s%s%s", animationWindowTitle, preName, t, postName);
			glutSetWindow(animationWin);
			glutSetWindowTitle( p );
			
			free( p );
		} else {
			glutSetWindow(animationWin);
			glutSetWindowTitle( t);
		}
	}
}

#pragma mark ------- menu helper functions
void worldMenuSelection( int id ) {
	
	_menuSelect( worldMenuList, id );
	
}



void worldKey(unsigned char theKey, int x, int y) {
#pragma unused(x,y)
	
	if(screenSaverMode )
		exit( 0 );

	// leave fullscreen mode if a key is pressed (not all keys - see whitelist)
	if( fullscreen && !strchr("LlVvXtT123457890cCsS.,+-", theKey)){
		fullscreen = false;
		fullScreenFunc( 0 );
	}
	
	
	// stop film creation
	if( myMovie ) {
		finishMovie();
		return;
	}
	
	if( !_menuHotkeySelect( worldMenuList, theKey ) )
		_myBeep();
	
	if( !jpAnimate )
		drawScene();
}



void worldSpecialKey(int theKey, int x, int y) {
#pragma unused(x,y)
	// stop film creation
	if( myMovie ) {
		finishMovie();
		return;
	}
	
	
	
	if( theKey  == GLUT_KEY_F1){
		myPrintf("\n\nrenderer = %s\n\n\n", (char*) glGetString(GL_RENDERER));
		goto bail;
	}
	
	
	if( theKey == GLUT_KEY_F2){
		readJPPrefs( false );
		goto bail;
	}
	
	
	if(screenSaverMode )
		exit( 0 );
	
	
	
	if( !_menuHotkeySelect( worldMenuList, theKey ) )
		_myBeep();

	
bail:
	if( !jpAnimate )
		drawScene();
	
}
// style-Menu aktualisieren
// Eingabe: true:  remove old content first
//                      false: only add new content
// Ausgabe: nope
// destroys old submenu
void addPositionMenu( bool remove) {
	int c = 0;
	int cur;
	
	cur = glutGetMenu();
	
	glutSetMenu(positionMenu);
	for( c = PositionSideToSide; c < PositionShortDistance; c++ ){
		glutAddMenuEntry(positionMenuName[c], c);      
	}
	glutSetMenu( cur );
	
}


void position_select(int item){
	char *p;
	int i;
	
	i = item;
	
	if( i == PositionReverseOrder)
		positionMirrorXFlag ^= true;
	else
		positionMode = i;
	
	p = readFile( actualTempPath, "r");
	if( p )	
		initSiteswapPattern( p, strlen( p ));
}

// style-Menu aktualisieren
// Eingabe: true:  remove old content first
//                      false: only add new content
// Ausgabe: nope
// destroys old submenu
void addStyleMenu( bool remove
		   ) {
	int i = 0, c = 0;
	style *p;
	int cur;
	
	cur = glutGetMenu();
	// remove all items from the style menu
	if( remove
	    ) {
		// disabled. doesnt work
		return;
		if( styleMenu > 0 )
			glutSetMenu(styleMenu);
		c = glutGet( GLUT_MENU_NUM_ITEMS);
		printf("removing from %d\n", styleMenu);
		printf("%d, %d\n", c, styleEntries);
		
		{
			while( c > 0 ) {
				printf("removing item %d\n", c--);
				glutRemoveMenuItem( 1);
			}
		}
        }
	
	//      printf("add to %d\n", glut_menu[stylesID]);
	styleEntries = 0;
	
	glutSetMenu(styleMenu);
	p = &stdStyle;
	while( p ) {
		//              printf("adding %s\n",p->name);
		glutAddMenuEntry(p->name, i++);                 // Name
		p = p->next;
		styleEntries++;
	}
	//printf("total styles: %d\n\n", styleEntries);
	defStyleIndex = 0;
	glutSetMenu( cur );
	
}



void style_select(int item) {
	style *s;
	
	s = getStyleFromIndex( item );
	if(s->first) {
		// select style
		defStyleIndex = item;
		// do pattern (if valid)
		if( validPattern ) {
			char *p;
			
			p = readFile( actualTempPath, "r");
			forceTableJugglingMode = true;
			startPattern( p );
			free( p );
		}
	} else
		beep( );
	
}


#pragma mark ------- menu functions
void animationAbout( int dummy ){
#pragma unused (dummy )	
	toggle(jpPrefs.showStage);
}

void animationQuit( int dummy ) {
#pragma unused (dummy )
	//_myPrintf("animationQuit() called by editor window\n");
	
	exitModules();
}

void editorWindowFunc( int dummy ) {
#pragma unused (dummy )
	createTextEditor( );
}

void infoWindowFunc( int dummy ) {
#pragma unused (dummy )
	// toggle value in function to avoid menu marker
	toggle(jpPrefs.showInfoWindow);
	createInfoWindow( jpPrefs.showInfoWindow);
}
void animateFunc( int dummy ) {
#pragma unused (dummy )
	selectAndStartPattern( );
}

void editAnimFunc( int dummy ) {
#pragma unused (dummy )
	char *p;
	char i[100];
	sprintf(i, "read file");
	
	
	p = selectAndLoadFile( i);
	if( p ) {
		_openInFunc( p );
		
		startPattern( p );
		if( validPattern == false ) {
			char *tmp;
#define BSTRINGNL BSTRING"\n"
			
			
			tmp = (char *)malloc( strlen(p) + 1 + strlen(BSTRINGNL));
			
			if( tmp ) {
				sprintf(tmp, "%s%s", BSTRINGNL, p );
				startPattern( tmp );
				free( tmp);
			}
		}
		
		
		free( p );
	}
}


void openInFunc( int dummy ) {
#pragma unused (dummy )
	
	char *p;
	char i[100];
	sprintf(i, "edit pattern");
	
	// xxxxx
//	p = selectFolder( i );
	p = selectAndLoadFile(i);
	_openInFunc( p );
	
}


void saveFunc( int dummy ) {
#pragma unused (dummy )
	
	char *buf;
	char i[100];
	sprintf(i, "write file");
	
	buf = readFile( actualTempPath, "r");
	if( buf ) {
		saveTextFileAs(buf, thePatternTitle, i);
		free( buf );
	}
}

/*
 void savePrefsFunc( int dummy ) {
#pragma unused (dummy )
	 writeJPPrefs();
 }
 */

void openWorkspaceFunc( int dummy ) {
#pragma unused (dummy )
	char t[MAX_PATTERN_TITLE];
	
	//if( fio( "select workspace file", "pass\0*.pass\0txt\0*.txt\0all\0*.*\0") ){
		//sprintf(t, "%s", fileSelectorPathAndName);
		//_loadCdewWorkspace(t);
	//}
}

void saveWorkspaceAsFunc( int dummy ) {
#pragma unused (dummy )
	char t[MAX_PATTERN_TITLE];
	//bool ret;
	bool ret = false;
	
	sprintf( t, "%s", "workspace.pass");
	// select folder
	//ret = fis( "save workspace file", t);

	// cancled by user
	if(	!ret ){
		return;
	}
	
	_saveWorkspaceAs( t );
}




void setAllObjects( int kind ) {
	int c;
	for(c=0;c<ballCount;c++) {
		if( kind == random_object)
			getObject( c )->kind = selectRandomObject();
		else
			getObject( c )->kind = kind;
	}
}

// objects
void randFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = random_object;
	setAllObjects( random_object );
}
void clubsFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = clubs;
	setAllObjects( clubs );
}

void fishFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = fishes;
	setAllObjects( fishes );
}
void ballsFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = balls;
	setAllObjects( balls );
}
void ringsFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = rings;
	setAllObjects( rings );
}
void pancacesFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = pancakes;
	setAllObjects( pancakes );
}
void nothingFunc( int dummy ) {
#pragma unused (dummy )
	jpPrefs.os = nothing;
	setAllObjects( nothing );
}

void addSpinFunc( int dummy ) {
#pragma unused (dummy )
	spinOffset++;
}

void subSpinFunc( int dummy ) {
#pragma unused (dummy )
	spinOffset--;
}

void defSpinFunc( int dummy ) {
#pragma unused (dummy )
	spinOffset = 0;
}


void fullScreenFunc( int dummy ) {
#pragma unused (dummy )
	// saveWindowSizes
	glutSetWindow( animationWin);
	if( fullscreen) {
		lastFullscreenW = glutGet(GLUT_WINDOW_WIDTH);
		lastFullscreenH = glutGet(GLUT_WINDOW_HEIGHT);
		lastFullscreenX = glutGet(GLUT_WINDOW_X);
		lastFullscreenY = glutGet(GLUT_WINDOW_Y);
		
		fullscreen = true;
#if 01
		glutFullScreen();
#else
		glutReshapeWindow(5000,4000);
		
#endif
	} else {
		glutReshapeWindow(lastFullscreenW,lastFullscreenH);
		glutPositionWindow(lastFullscreenX, lastFullscreenY);
		
		fullscreen = false;
	}
}



//1
void frontFunc( int dummy ) {
#pragma unused (dummy )
	viewMode = FRONT_VIEW;
	animationViewSet(FRONT_VIEW);
}
//3
void nextPosFunc( int dummy ) {
#pragma unused (dummy )
	viewMode = JUGGLER_VIEW;
	nextJugglerView(false);
}
//5
void moveCamFunc( int dummy ) {
#pragma unused (dummy )
	static int lastViewMode;
	
	static bool b = false;
	static GLfloat matrix[4][4];
	
	b = getCameraMode();
	toggle(b);
	setCameraMode( b );
	_myPrintf("camera mode = %d\n", b);
	// camera animationMotion now active:
	if( b ){
		// save viewing matrix
		animationGetMatrix( matrix );
		lastViewMode = viewMode;
		viewMode = MOVE_CAMERA_VIEW;
	}
	else{
		// restore matrix
		animationSetMatrix( matrix );
		viewMode = lastViewMode;
	}
}
//7
void topFunc( int dummy ) {
#pragma unused (dummy )
	viewMode = TOP_VIEW;
	animationViewSet(TOP_VIEW);
}
//9
void birdFunc( int dummy ) {
#pragma unused (dummy )
	viewMode = BIRD_VIEW;
	animationViewSet(BIRD_VIEW);
}

void vomitCamFunc( int dummy ) {
#pragma unused (dummy )
	nextVomitObject();
	viewMode = VOMIT_VIEW;
}

void tessCamFunc( int dummy ) {
#pragma unused (dummy )
	nextTessedJuggler();
	// switch off movement vectors
	jpPrefs.showJugglerPos = false;
	
	// switch off floor
	jpPrefs.showFloorGrid = false;
	jpPrefs.floor = false;
	viewMode = TESS_VIEW;
}

void objectCamFunc( int dummy ) {
#pragma unused (dummy )
	nextVomitObject();
	viewMode = OBJECT_VIEW;
}

void resetMagFunc( int dummy ) {
#pragma unused (dummy )
	animationResetMagnification( );
}

void markAllBouncesUndefined( void );

void higherFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.gravity *= FAK_GRAV;
	markAllBouncesUndefined(  );
	
	useDefaultAnimationValues = false;
	
}
void lowerFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.gravity /= FAK_GRAV;
	markAllBouncesUndefined(  );
	
   	useDefaultAnimationValues = false;
	
}
void fasterFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.animationSpeed *= FAK_TIME;
	
	useDefaultAnimationValues = false;
}
void slowerFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.animationSpeed /= FAK_TIME;
	useDefaultAnimationValues = false;
}

void slowMotionFunc(  int dummy ) {
#pragma unused (dummy )
	static bool slowMotionFlag = false;
	
	setAnimationToDefaultValues( 1 );
	toggle( slowMotionFlag );
	
	if( slowMotionFlag )
		jpPrefs.animationSpeed /= 3.;

}

void dropFunc(  int dummy ) {
#pragma unused (dummy )
	reInitAllObjects( );
}
void warningNarrowFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.minObjectDistance /= MIN_DISTANCE_FACTOR;
}
void warningWideFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.minObjectDistance *= MIN_DISTANCE_FACTOR;
}
void biggerWorldFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.mag *= FAK_MAG;
	jpPrefs.mag = min( jpPrefs.mag, DefMag*10.);
}
void smallerWorldFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.mag /= FAK_MAG;
}

void biggerObjectFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.objectFaktor *= FAK_MAG;
}
void smallerObjectFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.objectFaktor /= FAK_MAG;
}

void jugglerColorFunc(  int dummy ) {
#pragma unused (dummy )
	jpPrefs.jugglerColor++;
	jpPrefs.jugglerColor = jpPrefs.jugglerColor%ColorJugEnd;
}


void objectColorFunc(  int dummy ) {
#pragma unused (dummy )
	static int jugglerColorState = -1;
	
	jpPrefs.objectColorMode ++;
	jpPrefs.objectColorMode %= ColorObjEnd;
	
	if( jpPrefs.objectColorMode == destJugglerObj){
		jugglerColorState = jpPrefs.jugglerColor;
		jpPrefs.jugglerColor = multiColorJugglers;
	}else if( jugglerColorState >= 0){
		jpPrefs.jugglerColor = jugglerColorState;
	}
}

#define DetailFactor 2.f;

void detailsFunc(   int dummy ) {
#pragma unused (dummy)
	
	if( jpPrefs.levelOfDetail == 5 )
		jpPrefs.levelOfDetail = 10;
	else
		jpPrefs.levelOfDetail = 5;
}
	
void remakeMenu( int dummy){
#pragma unused (dummy)
	
	if( animationWin )
		makeAnimationMenu();
	if( infoWin )
		reCreateMenu( infoWin, infoMenuList);
	if( editWin )
		reCreateMenu( editWin, editMenuList);
	reCreateCausalEditorMenus();
	if(inputID)
		reCreateMenu( inputID, runaroundListMenuList);
	if(listID)
		reCreateMenu( listID, runaroundListMenuList);
	
	
	
}


void makeAnimationMenu( void ) {
	reCreateMenu( animationWin, worldMenuList);
	addStyleMenu(false);
}


void causalWindowFunc( int dummy ) {
#pragma unused (dummy )
	activateCausalEditor();
}

void runaroundWindowFunc( int dummy ) {
#pragma unused (dummy )

	if( runaroundEditorHasOpenWindows() )	{
		
		glutSetWindow(inputID);
		glutSetWindow(listID);
		
	}else {
		
		newRunaroundProject();
	}
	
}

// -------------------- Texturen ------------------





void initTextures(void) {
	hasSkyTexture =
	hasFloorTexture =
	hasObjectTexture =
	hasRingTexture =
	hasClubTexture =
	hasHeadTexture =
	hasBodyTexture = false;
	
/*	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
*/	
	//    _myPrintf("initalizing textures\n");
	
	hasFloorTexture	= init1Texture("floor.sgi", hasFloorTextureIndex);
	hasStageTexture	= init1Texture("stage.sgi", hasStageTextureIndex);
	hasSkyTexture	= init1Texture("sky.sgi", hasSkyTextureIndex);
	hasObjectTexture	= init1Texture("ball.sgi", hasObjectTextureIndex);
	hasRingTexture	= init1Texture("ring.sgi", hasRingTextureIndex);
	hasClubTexture	= init1Texture("club.sgi", hasClubTextureIndex);
	hasHeadTexture	= init1Texture("head.sgi", hasHeadTextureIndex);
//	hasBodyTexture = init1Texture("body.sgi", hasBodyTextureIndex);

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//    _myPrintf("initalizing textures ... done\n");
	
}
 
bool init1Texture( char *textureFilename, GLuint myIndex) {
	char *p = NULL;
	bool ret;
	size_t l;
	unsigned *buf;
	int width, height, components;
	
	if( textureFilename == NULL)
		return false;
	
	
	l = strlen(texturePath) + strlen(textureFilename) + 2;
	if( (p = (char *)malloc( l) ) == NULL )
		return false;
	
	
	sprintf(p,"%s%s", texturePath, textureFilename);
//	printf("loading texture %s\n", p);
	
	if(!fexists(p)){
//		printf("...failed\n");
		return false;
	}

	
	
	if( (buf = read_texture( p, &width, &height, &components )) == NULL)
			ret = false;

#define BORDER 0
	glBindTexture(GL_TEXTURE_2D, myIndex);
	glTexImage2D( GL_TEXTURE_2D, 0, components, width + BORDER * 2 , height + BORDER* 2, BORDER, GL_RGBA, GL_UNSIGNED_BYTE, buf );
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
*/	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf );
	
	
	free( buf );
	ret = true;

	free(p);
	
	return ret;
}


void animationFunc( int dummy ){
#pragma unused (dummy )
 	setupAnimationWindow();
}

void setTextureHead(  int dummy ) {
#pragma unused (dummy )
	setAllTextureFlag( 0 );
}

void setTextureSky( int dummy ) {
#pragma unused (dummy )
	setAllTextureFlag( 0 );
	jpPrefs.sky = jpPrefs.texturedSky;
}

void setTextureFloor( int dummy ) {
#pragma unused (dummy )
	setAllTextureFlag( 0 );
	jpPrefs.floor = jpPrefs.texturedFloor;
}

void setTextureObject(  int dummy ) {
#pragma unused (dummy )
	setAllTextureFlag( 0 );
}

void setTextureClub(  int dummy ) {
#pragma unused (dummy )
	setAllTextureFlag( 0 );
}


void setAllTextures( int dummy ) {
#pragma unused (dummy )
	
	jpPrefs.texturedClub =
	jpPrefs.texturedBall =
	jpPrefs.texturedSky =
	jpPrefs.texturedFloor =
	jpPrefs.texturedJuggler =
	jpPrefs.texturedHead = 
	allTextureFlag == true;
	
	jpPrefs.sky = jpPrefs.texturedSky;
	jpPrefs.floor = jpPrefs.texturedFloor;
	
	makeAnimationMenu();
}

void setAllTextureFlag(int dummy) {
#pragma unused (dummy )
	
	allTextureFlag = (
			  jpPrefs.texturedSky &&
			  jpPrefs.texturedFloor &&
			  jpPrefs.texturedBall &&
			  jpPrefs.texturedClub &&
			  jpPrefs.texturedHead
			  );								
}


void initAnimation( void ) {
	
	new animation();
}



animation::animation( void ) {
	
	_menuList       *l,
	
	_animationMenuList[]= {
	{"animation window",									menuTitle},				
	{languageStrings[languageAnimMenuAnim],                 newMenu,                        0,      &animMenu,      worldMenuSelection} ,
	{languageStrings[languageAnimAnimAnimate],              menuEntry,                      '4',    NULL,           toggleAnimation }, 
	{languageStrings[languageAnimAnimStep],			        menuEntry,                      '2',    NULL,           oneStepOfAnimation }, 
	{languageStrings[languageAnimAnimThrow],		        menuEntry,                      '8',    NULL,           untilNextThrow }, 
		
	{languageStrings[languageAnimAnimBigger],               menuEntry,                      'S',    NULL,           biggerWorldFunc},
	{languageStrings[languageAnimAnimSmaller],              menuEntry,                      's',    NULL,           smallerWorldFunc},
	{languageStrings[languageAnimObjectBigger],             menuEntry,                      'B',    NULL,           biggerObjectFunc},
	{languageStrings[languageAnimObjectSmaller],            menuEntry,                      'b',    NULL,           smallerObjectFunc},
	{languageStrings[languageAnimAnimFaster],               menuEntry,                      '+',    NULL,           fasterFunc},
	{languageStrings[languageAnimAnimSlower],               menuEntry,                      '-',    NULL,           slowerFunc},
	{"toggle slow motion",									menuEntry,                      '6',    NULL,           slowMotionFunc},
	{languageStrings[languageAnimAnimHigher],               menuEntry,                      'H',    NULL,           higherFunc},
	{languageStrings[languageAnimAnimLower],                menuEntry,                      'h',    NULL,           lowerFunc},
	{languageStrings[languageAnimAnimDrop],                 menuEntry,                      3,      NULL,           dropFunc},
	{languageStrings[languageAnimAnimTableJuggling],        menuEntry,                      '_',    NULL,           toggleTableJugglingMode},
	{languageStrings[languageAnimAnimDefault],				menuEntry,                      '.',    NULL,           setDefaultAnimationSpeed},
	{languageStrings[languageAnimAnimTap],					menuEntry,                      ',',    NULL,           NULL, &jpPrefs.useTapAnimationValues},
	{languageStrings[languageAnimAnimWarningNarrow],        menuEntry,                      '<',    NULL,           warningNarrowFunc},
	{languageStrings[languageAnimAnimWarningWide],          menuEntry,                      '>',    NULL,           warningWideFunc},
	{languageStrings[languageAnimAnimShortDistance],        menuEntry,                      'P',    NULL,           NULL, &jpPrefs.emphaseShortDistanceFlag},
	{languageStrings[languageAnimAnimShortDistanceForSelfs],menuEntry,                      'p',    NULL,           NULL, &jpPrefs.emphaseShortDistanceFlagForSelfs},
		
	{languageStrings[languageAnimMenuFile],                 newMenu,                        0,      &fileMenu,      worldMenuSelection},
	{languageStrings[languageAnimFileEdit],                 menuEntry,                      'o',    NULL,           openInFunc},
	{languageStrings[languageAnimFileAnimate],              menuEntry,                      'a',    NULL,           animateFunc},
	{languageStrings[languageAnimFileEditPlus],             menuEntry,						0,		NULL,           editAnimFunc},
	{languageStrings[languageAnimFileSaveAs],               menuEntry,                      0,      NULL,           saveFunc},
	{languageStrings[languageAnimFileToEditor],				menuEntry,                      'e',	NULL,           editFunc},

	{languageStrings[languageAnimFileToExternalEditor],		menuEntry,                      'x',	NULL,           editExternFunc},
	{languageStrings[languageAnimFileSaveChanged],          menuEntry,                      0,      NULL,           NULL,   &jpPrefs.askForSaveFlag},
	
	{languageStrings[languageAnimMenuWorkspace],			newMenu,						0,		&workspaceMenu,	worldMenuSelection},
	{languageStrings[languageAnimWorkspaceOpen],			menuEntry,                      'w',    NULL,           openWorkspaceFunc},
	{languageStrings[languageAnimWorkspaceSaveAs],			menuEntry,                      0,      NULL,           saveWorkspaceAsFunc},
	
	
	{languageStrings[languageAnimMenuObject],               newMenu,                        0,      &objectMenu,    worldMenuSelection},
	{languageStrings[languageAnimObjectRandom],             menuEntry,                      0,      NULL,           randFunc },
	{languageStrings[languageAnimObjectClubs],              menuEntry,                      0,      NULL,           clubsFunc},
	{languageStrings[languageAnimObjectFishes],             menuEntry,                      0,      NULL,           fishFunc},
	{languageStrings[languageAnimObjectBalls],				menuEntry,                      0,      NULL,           ballsFunc},
	{languageStrings[languageAnimObjectRings],              menuEntry,                      0,      NULL,           ringsFunc},
	{languageStrings[languageAnimObjectPancakes],           menuEntry,                      0,      NULL,           pancacesFunc},
	{languageStrings[languageAnimObjectNothing],            menuEntry,                      0,      NULL,           nothingFunc},
	{languageStrings[languageAnimObjectMoreSpin],           menuEntry,                      0,      NULL,           addSpinFunc},
	{languageStrings[languageAnimObjectLessSpin],           menuEntry,                      0,      NULL,           subSpinFunc},
	{languageStrings[languageAnimObjectResetSpin],          menuEntry,                      0,      NULL,           defSpinFunc},
		
	{languageStrings[languageAnimMenuColor],                newMenu,                        0,      &colorMenu,     worldMenuSelection},
	{languageStrings[languageAnimColorJugglers],            menuEntry,                      'c',    NULL,           jugglerColorFunc },
	{languageStrings[languageAnimColorObjects],             menuEntry,                      'C',    NULL,			objectColorFunc},
		
	{languageStrings[languageAnimMenuEnvironment],          newMenu,                        0,      &envMenu,       worldMenuSelection},
	{languageStrings[languageAnimEnvironmentSky],           menuEntry,                      0,      NULL,           NULL,           &jpPrefs.sky },
	{languageStrings[languageAnimEnvironmentFloor],         menuEntry,                      0,      NULL,           NULL,           &jpPrefs.floor},
	{languageStrings[languageAnimEnvironmentFog],           menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.fog},
	{languageStrings[languageAnimEnvironmentUnicycles],     menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.unicycles},
		
	{languageStrings[languageAnimEnvironmentBlindfolded],   menuEntry,                      0,      NULL,           NULL,           &jpPrefs.blindfolded},
	{languageStrings[languageAnimEnvironmentScene],         menuEntry,                      0,      NULL,           NULL,           &jpPrefs.showScene},
	{languageStrings[languageAnimEnvironmentLogo],          menuEntry,                      0,      NULL,           NULL,           &jpPrefs.showLogo},
	{languageStrings[languageAnimEnvironmentJugglerId],		menuEntry,                      'n',    NULL,           NULL,           &jpPrefs.showJugglerNumbers},
	{languageStrings[languageAnimEnvironmentObjectId],		menuEntry,                      'N',    NULL,           NULL,           &jpPrefs.showObjectNumbers},
	{languageStrings[languageAnimEnvironmentSiteswapId],	menuEntry,                      0,		NULL,           NULL,           &jpPrefs.showSiteswapValue},
		
	{languageStrings[languageAnimMenuTexture],              newMenu,                        0,      &texMenu,       worldMenuSelection},
	{languageStrings[languageAnimTextureHead],              menuEntry,                      0,      NULL,           setTextureHead,	&jpPrefs.texturedHead},
	{languageStrings[languageAnimTextureSky],               menuEntry,                      0,      NULL,           setTextureSky,	&jpPrefs.texturedSky},
	{languageStrings[languageAnimTextureFloor],             menuEntry,                      0,      NULL,			setTextureFloor,&jpPrefs.texturedFloor},
	{languageStrings[languageAnimTextureBalls],             menuEntry,                      0,      NULL,           setTextureObject,	&jpPrefs.texturedBall},
	{languageStrings[languageAnimTextureClubs],             menuEntry,                      0,      NULL,           setTextureClub,	&jpPrefs.texturedClub},
	{languageStrings[languageAnimTextureAllTextures],       menuEntry,                      0,      NULL,           setAllTextures,	&allTextureFlag},
		
	{languageStrings[languageAnimMenuCamera],               newMenu,                        0,      &cameraMenu,    worldMenuSelection},
	{languageStrings[languageAnimCameraFront],              menuEntry,                      '1',    NULL,           frontFunc },
	{languageStrings[languageAnimCameraNext],               menuEntry,                      '3',    NULL,           nextPosFunc},
	{languageStrings[languageAnimCameraMove],               menuEntry,                      '5',    NULL,           moveCamFunc},
	{languageStrings[languageAnimCameraTop],                menuEntry,                      '7',    NULL,           topFunc},
	{languageStrings[languageAnimCameraBird],               menuEntry,                      '9',    NULL,           birdFunc},
	{languageStrings[languageAnimCameraVomit],              menuEntry,                      'v',    NULL,           vomitCamFunc},
	{languageStrings[languageAnimCameraObject],              menuEntry,                     'V',    NULL,           objectCamFunc},
	{languageStrings[languageAnimCameraTess],				menuEntry,                      'J',	NULL,           tessCamFunc},
	{languageStrings[languageAnimCameraResetMag],           menuEntry,                      033,    NULL,           resetMagFunc},
		
	{languageStrings[languageAnimMenuAbstract],             newMenu,                        0,      &abstractMenu,  worldMenuSelection},
	{languageStrings[languageAnimAbstractAxis],             menuEntry,                      '#',    NULL,           NULL,           &jpPrefs.a },
	{languageStrings[languageAnimAbstractNiveaus],          menuEntry,                      'l',    NULL,           NULL,           &jpPrefs.showNiveaus},
	{languageStrings[languageAnimAbstractCameraPath],       menuEntry,                      0,      NULL,           NULL,           &jpPrefs.showCameraPath},
	{languageStrings[languageAnimAbstractHandPath],			menuEntry,                      'j',     NULL,           NULL,			&jpPrefs.showHandPath},
		
	{languageStrings[languageAnimMenuWalk],                 newMenu,                        0,      &walkMenu,      worldMenuSelection},
	{languageStrings[languageAnimWalkGrid],                 menuEntry,                      'g',    NULL,           showGrid,		&showGridFlag },
	{languageStrings[languageAnimWalkPos],                  menuEntry,                      0,      NULL,           showPos,		&showPosFlag },
	{languageStrings[languageAnimWalkPoi],                  menuEntry,                      '!',    NULL,           showPoi,		&showPosFlag },
	{languageStrings[languageAnimWalkAllPoints],			menuEntry,                      0,      NULL,           NULL,           &showShortTimeMovementPaths},
//	{languageStrings[languageAnimWalkModifiers],            menuEntry,                      0,      NULL,           NULL,           &jpPrefs.showJugglerModifiers},
			
	{languageStrings[languageAnimMenuRenderer],             newMenu,                        0,      &rendererMenu,worldMenuSelection},
	{languageStrings[languageAnimRendererSolid],            menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.solidWorld },
	{languageStrings[languageAnimRendererSolidNiveaus],     menuEntry,                      'L',    NULL,			animationFunc,	&jpPrefs.solidNiveaus},
	{languageStrings[languageAnimRendererSolidFloor],       menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.solidFloor},
	{languageStrings[languageAnimRendererSmooth],           menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.shaderMode},
	{languageStrings[languageAnimRendererAntialias],        menuEntry,                      0,      NULL,           animationFunc,	&jpPrefs.antialias},
	{languageStrings[languageAnimRendererLookThroughFloor], menuEntry,                      0,      NULL,           NULL,			&jpPrefs.lookThroughFloor},
	{languageStrings[languageAnimRendererDetails],			menuEntry,                      0,		NULL,           detailsFunc},	
		
	{languageStrings[languageAnimMenuView],                 newMenu,                        0,      &viewMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuAnim],                 addAsSubMenu,                   0,      &animMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuCamera],               addAsSubMenu,                   0,      &cameraMenu,	worldMenuSelection},
	{languageStrings[languageAnimMenuObject],               addAsSubMenu,                   0,      &objectMenu,	worldMenuSelection},
	{languageStrings[languageAnimMenuEnvironment],          addAsSubMenu,                   0,      &envMenu,       worldMenuSelection},
	{languageStrings[languageAnimMenuTexture],              addAsSubMenu,                   0,      &texMenu,       worldMenuSelection},
	{languageStrings[languageAnimMenuAbstract],             addAsSubMenu,                   0,      &abstractMenu,  worldMenuSelection},
	{languageStrings[languageAnimMenuRenderer],             addAsSubMenu,                   0,      &rendererMenu,	worldMenuSelection},
		
	{languageStrings[languageAnimMenuWalk],                 addAsSubMenu,                   0,      &walkMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuColor],                addAsSubMenu,                   0,      &colorMenu,     worldMenuSelection},
		
	{languageStrings[languageAnimMenuFullScreen],           menuEntry,                      'X',    NULL,           fullScreenFunc, &fullscreen},
		
	{languageStrings[languageAnimMenuSound],                newMenu,                        0,      &soundMenu,     worldMenuSelection},
	{languageStrings[languageAnimSoundThrow],               menuEntry,                      't',    NULL,           NULL,           &jpPrefs.t },
	{languageStrings[languageAnimSoundCatch],               menuEntry,                      'T',    NULL,           NULL,           &jpPrefs.s },
	{languageStrings[languageAnimSoundKind],			      menuEntry,                    'k',    NULL,           nextSoundKindFunc},
	{languageStrings[languageAnimSoundDynamicHeight],       menuEntry,                      0,      NULL,           NULL,           &jpPrefs.dt },
	{languageStrings[languageAnimSoundDynamicPosition],     menuEntry,                      0,      NULL,           NULL,           &jpPrefs.dp },
	{languageStrings[languageAnimSoundFaults],              menuEntry,                      0,      NULL,           NULL,           &jpPrefs.es },
		
	{languageStrings[languageAnimMenuMedia],                newMenu,                        0,      &mediaMenu,     worldMenuSelection},
		
		
#ifdef MAKE_QUICKTIME_MOVIE
	{languageStrings[languageAnimMediaMakeMovie],           menuEntry,                      'm',    NULL,           quicktimeMovieFunc},
//	{languageStrings[languageAnimMediaSelectCompressor],    menuEntry,                      '6',      NULL,         quicktimeCompressorFunc},
#endif
	{languageStrings[languageAnimMediaMakeScreenshotSeries],menuEntry,                      'M',    NULL,           movieByScreenshots},
	{languageStrings[languageAnimMediaMakeScreenshot],      menuEntry,                      ' ',    NULL,           screenshotFunc},
	{languageStrings[languageInfoMovieMoreFrames],          menuEntry,                      'F',    NULL,           moreFrames },
	{languageStrings[languageInfoMovieLessFrames],          menuEntry,                      'f',    NULL,           lessFrames },
		
		
	{languageStrings[languageAnimMenuWindows],              newMenu,                        0,      &winMenu,       worldMenuSelection},
	{languageStrings[languageAnimWindowsInfoWindow],        menuEntry,                      'I',    NULL,           infoWindowFunc },
	{languageStrings[languageAnimWindowsEditorWindow],      menuEntry,                      'E',    NULL,           editorWindowFunc },
	{languageStrings[languageAnimWindowsCausalWindow],      menuEntry,                      'W',    NULL,           causalWindowFunc },
	{languageStrings[languageAnimWindowsCausalDuplicate],	menuEntry,                      'D',    NULL,           causalDuplicateFunc },
	{languageStrings[languageAnimWindowsRunaroundWindow],   menuEntry,						'R',     NULL,          runaroundWindowFunc },
#if MAC_OS_X_VERSION
	{languageStrings[languageAnimWindowsPreviousWindow],      menuEntry,					'z',    NULL,           switchToLastOrAnimationWindow },
#endif
		
	{languageStrings[languageAnimMenuStyles],               newMenu,                        0,      &styleMenu,     style_select},
	{languageStrings[languageAnimMenuPositions],			newMenu,                        0,      &positionMenu,  position_select},
		
	{languageStrings[languageAnimMenuConfigure],			newMenu,                        0,      &menuMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuMarkers],				menuEntry,                      0,      NULL,           remakeMenu, &jpPrefs.showMenuSelectors },
	{languageStrings[languageAnimMenuHotkeys],              menuEntry,                      0,      NULL,           remakeMenu, &jpPrefs.showMenuHotkeys },
	{languageStrings[languageAnimMenuWatchfile],			menuEntry,                      0,     NULL,			selectWatchFile },
	{languageStrings[languageAnimMenuLogfile],				menuEntry,                      0,     NULL,			NULL, &jpPrefs.writeLogfile  },
#if WINDOWS_VERSION
	{languageStrings[languageAnimMenuExternalEditor],		menuEntry,                      0,     NULL,			selectExternalEditor },
#endif		
		
	{languageStrings[languageAnimMenu],                     newMenu,                        0,      NULL,           worldMenuSelection},
	{languageStrings[languageAnimMenuFile],                 addAsSubMenu,                   0,      &fileMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuWorkspace],			addAsSubMenu,                   0,      &workspaceMenu,	worldMenuSelection},
	{languageStrings[languageAnimMenuStyles],               addAsSubMenu,                   0,      &styleMenu,     style_select},
	{languageStrings[languageAnimMenuPositions],			addAsSubMenu,                   0,      &positionMenu,  position_select},
	{languageStrings[languageAnimMenuView],                 addAsSubMenu,                   0,      &viewMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuSound],                addAsSubMenu,                   0,      &soundMenu,     worldMenuSelection},
	{languageStrings[languageAnimMenuMedia],                addAsSubMenu,                   0,      &mediaMenu,     worldMenuSelection},
	{languageStrings[languageAnimMenuConfigure],			addAsSubMenu,                   0,      &menuMenu,      worldMenuSelection},
	{languageStrings[languageAnimMenuWindows],              addAsSubMenu,                   0,      &winMenu,       worldMenuSelection},
	{languageStrings[languageAnimMenuRestart],				menuEntry,                      'r',	NULL,           restartFunc},
	{languageStrings[languageAnimMenuAbout],				menuEntry,                      'i',    NULL,           animationAbout},
	{languageStrings[languageAllMenusOpenDoc],				menuEntry,                      '?',	NULL,			openDocFunc},
	{languageStrings[languageAnimFileOpenPrechacthis],		menuEntry,                      16,		NULL,			openPrechacthisFunc},
	{languageStrings[languageAllMenusQuit],					menuEntry,                      'q',    NULL,           animationQuit},
		
		
	{NULL,                                                  menuListTerminator,             0,      NULL,           0}      // last entry
	};
	
	// alloc memory for menuList
	l = (_menuList *)malloc( sizeof( _animationMenuList ));
	// copy content
	memcpy( l, _animationMenuList, sizeof( _animationMenuList ));
	
	worldMenuList = l;
	
}

animation::~animation( ) {}



void setAnimationToDefaultValues( int dummy )
{
#pragma unused( dummy )
	
#define maxDT 1.2
	
	int stdSpeedIndex = ballCount/jugglerCount;
	float tps;
	
	stdSpeedIndex = min( stdSpeedIndex, NumberOfHeightValues - 1);
	if(0 && jugglerCount == 1 )
		tps = defaultSpeedList[stdSpeedIndex] ;	
	else
		tps = 1.7;
	
	
	setAnimationSpeed( tps);
}

void setTapAnimationValues( int dummy )
{	
#pragma unused( dummy )
	
	static int lastTime = -1;
	int now = glutGet( GLUT_ELAPSED_TIME ) ;
	float tps;
	
	if(lastTime == -1 )
		lastTime = now;
	else
	{
		int dt = now - lastTime;
		
	        
		if(dt < 1500)
		{
			tps = 1000./dt;
			
			setAnimationSpeed( tps );
			setGravity( 98.1 * 4 / (tps*tps) ); 
			//	    _myPrintf("as = %.2f, dt = %d\n", tps, dt);
			
			lastTime = now;
			
		}else
			lastTime = -1;
		
	}	    
	
}

void nextSoundKindFunc( int dummy )
{
#pragma unused( dummy)
	//soundKind++;
	//if( soundKind == endOfSoundKinds)
		//soundKind = 0;
}	



/* 
animation is fired by a timer,
	as glutIdle always results in 100% system usage 
	
	these routines reduce the usage of system resources to a given maximum framerate 
	(30 by default, adjustable in the preferences file)
 */	

void toggleAnimation( int dummy )
{
#pragma unused( dummy)
        toggle(jpAnimate);
        
        glutTimerFunc (jpTimerFireringSpeed , callWorldIdle, 1);
}



void callWorldIdle( int dummy )
{
#pragma unused( dummy)
    jpIdle( );
	_drawTimeCursor( 1 );

	if( jpAnimate )
		glutTimerFunc (jpTimerFireringSpeed , callWorldIdle, 1);
}        

void untilNextThrow( int dummy )
{
#pragma unused( dummy)
	jpUntilNextThrow = true;
	jpAnimate = true;
	glutTimerFunc (jpTimerFireringSpeed , callWorldIdle, 1);
}

	
	
void oneStepOfAnimation( int dummy )
{
#pragma unused( dummy)
        jpAnimate = false;
	
        jpIdle( );
}


/*
 durch die Fenster wechseln geht nicht richtig.
animation window aktiv bei erser auswahl: nichts geschieht.
sonst: nur wechsel zwischen anim und letztem aktiven fenster 
*/
 void switchToLastOrAnimationWindow( int dummy )
{
#pragma unused( dummy)
	
	static int savedWinID = -1;
	int tmpWinID = glutGetWindow();
	
	// animation is active
	if( tmpWinID == animationWin )
	{
		// no last window: nothing to do.
		if( savedWinID == -1 )
			return;
		else{
			glutSetWindow( savedWinID);
			glutHideWindow();
			glutShowWindow();
		}
	}
	// other window is active
	else
	{
		savedWinID = glutGetWindow();
		glutSetWindow( animationWin);
		glutHideWindow();
		glutShowWindow();
	}	
}	



#ifdef MAKE_QUICKTIME_MOVIE

#if 0
void quicktimeCompressorFunc( int dummy ) {
#pragma unused (dummy )
	myMovie->getCompressionInfo();
}
#endif


	
void quicktimeMovieFunc( int dummy ) {
#pragma unused (dummy )
	
	if( !myMovie )
		myMovie = new movie ( movieMode );
}
#endif


void finishMovie( void )
{
	myMovie->endMovie();
	delete myMovie;
	myMovie = NULL;
}	
	
void movieByScreenshots( int dummy ) {
#pragma unused (dummy )
	if( !myMovie )
		myMovie = new movie ( screenshotMode);
}



void screenshotFunc( int dummy ) {
#pragma unused (dummy )

	drawScene();
	makeOneScreenshot();
	
}

void flip1p( int wwDummy ){
	Juggler *j;
	
	j = getJuggler( 0 );
	j->position.x *= -1;
	return;
}

void causalDuplicateFunc( int dummy ) 
{
#pragma unused (dummy )
	oglwin *c;
	char *p;
	
	p = readFile( actualTempPath, "r");
	if( p ){
		if( strstr( p, CDEW_INFORMATION_STRING )){
			c = activateCausalEditor();
			c->_readDiagram(p);
		}else {
			openFileInTextEditor(actualTempPath);
		}

		free( p );
	}
}

void restartFunc(  int dummy )
{
	readAndStartPattern( actualTempPath);
}


void editFunc( int dummy )
{
	char *buf;
	buf = readFile( actualTempPath, "r");
	if( buf ) {
		saveNameAndPath( actualTempPath);
		_openInFunc( buf );	
		free(buf);
	}
}

void editExternFunc( int dummy)
{
#pragma unused (dummy )
	openFileInExternalEditor( actualTempPath );
}

void selectWatchFile( int dummy )
{
#pragma unused (dummy)
	// save file names, we will need them
	char tpt[MAX_PATTERN_TITLE];
	char fppn[MAX_FULL_FILE_NAME];
	char fspan[MAX_FULL_FILE_NAME];
	char name[MAX_PATTERN_TITLE];
	
	if( fullPatternPathName[0] == '\0' || fullPatternPathName[0] == '.' )
		getcwd( fullPatternPathName, MAX_FULL_FILE_NAME);
	
	sprintf( tpt, "%s", thePatternTitle);
	sprintf(fppn, "%s", fullPatternPathName);
	sprintf(fspan, "%s", fileSelectorPathAndName);
	
	sprintf(name, "%s", "joe.pass");
	
	// call the file selector
	//if( fis(  "select download folder", name)){
		//// write selected path and filename to file
//#if MAC_OS_X_VERSION
		//writeFile( watchfileDownloadPathMac, "w", fileSelectorPathAndName, strlen( fileSelectorPathAndName));
//#else
		//writeFile( watchfileDownloadPathWin, "w", fileSelectorPathAndName, strlen( fileSelectorPathAndName));
//#endif
		//// fis created an empty file. remove it, we don´t want to use this file for an animation 
		//remove(fileSelectorPathAndName);
	//}
	
	// restore file names
	sprintf( thePatternTitle, "%s", tpt);
	sprintf( fullPatternPathName, "%s", fppn);
	sprintf( fileSelectorPathAndName, "%s", fspan);
	
}


#if 1
void selectExternalEditor( int dummy )
{
#pragma unused (dummy)
	// save file names, we will need them
	char tpt[MAX_PATTERN_TITLE];
	char fppn[MAX_FULL_FILE_NAME];
	char fspan[MAX_FULL_FILE_NAME];
	sprintf( tpt, "%s", thePatternTitle);
	sprintf(fppn, "%s", fullPatternPathName);
	sprintf(fspan, "%s", fileSelectorPathAndName);
	
	// call the file selector
	//if( fio( "select external editor", "exe\0*.exe\0all\0*.*\0")){
		// write selected path and filename to file
		//writeFile( externalTextEditorPath, "w", fileSelectorPathAndName, strlen( fileSelectorPathAndName));
		// fis created an empty file. remove it, we don´t want to use this file for an animation 
//		remove(fileSelectorPathAndName);
	//}
	
	// restore file names
	sprintf( thePatternTitle, "%s", tpt);
	sprintf( fullPatternPathName, "%s", fppn);
	sprintf( fileSelectorPathAndName, "%s", fspan);
	 
}
#endif


void openDocFunc( int dummy )
{
#pragma unused (dummy)
	char cmdLine[10000];
#if MAC_OS_X_VERSION
	// force system to open file in default viewer (option -t)
	sprintf(cmdLine, "/usr/bin/open \"%s%s\"", appDir, "index.html");
	system(cmdLine);
#elif WINDOWS_VERSION
	int r;
	sprintf(cmdLine, "\"%s%s\"", appDir, "index.html");
	r = spawnlp( _P_NOWAIT, "cmd.exe", "/C", cmdLine, NULL, NULL);//"\"C:\\Dokumente und\ Einstellungen\\Administrator\\Desktop\\ww.pass\"", NULL);	
#endif
	myPrintf("cmdLine = %s\n", cmdLine);

//	r = system(cmdLine);
}


void openPrechacthisFunc( int dummy )
{
#pragma unused (dummy)
	char cmdLine[10000];
	char cmdFormat[10000];
    GLint ax,ay,aw,ah, sx,sy,sw,sh,
	prechacWindowWidth;
    
    
    ax = glutGet( GLUT_WINDOW_X);
    ay = glutGet( GLUT_WINDOW_Y);
    aw = glutGet( GLUT_WINDOW_WIDTH) + 50;
    ah = glutGet( GLUT_WINDOW_HEIGHT);

    sx = 0;
    sy = 20;
    sw = glutGet( GLUT_SCREEN_WIDTH);
    sh = glutGet( GLUT_SCREEN_HEIGHT);
	prechacWindowWidth = 500;
    
//fenster in den fordergund
#if MAC_OS_X_VERSION
#define CommandFormat "osascript -e  'tell application \"Finder\" to open application \"Safari\"' -e  'tell application \"Finder\" to set visible of process \"Safari\" to true' -e 'tell application \"Safari\" to open location \"http://www.prechacthis.org\"' -e 'tell application \"Safari\" to set bounds of front window to {%d, %d, %d, %d}'"
	// osascript -e 'tell application "Finder"' -e 'set visible of process \"Safari\" to true' -e 'end tell'
    // force system to open file in default text editor (option -t)
//	sprintf(cmdLine, "/usr/bin/open %s", "http://www.prechacthis.org/");
	sprintf( cmdFormat, "%s",  CommandFormat);
    
    if(ax + aw + prechacWindowWidth < sx + sw)
         sprintf( cmdLine, cmdFormat, ax + aw, ay - sy, sw - (ax + aw)+ prechacWindowWidth, ay + ah - sy);
    else
        sprintf( cmdLine, cmdFormat, 100,100, 1000, 1000);
	_myPrintf("%s\n", cmdLine);
	system(cmdLine);
#elif WINDOWS_VERSION
	int r;
	sprintf(cmdLine, "\"%s %s\"", "explorer", "http://www.prechacthis.org/");
//	sprintf(cmdLine, "\"%s%s\"", appDir, "index.html");
//	r = spawnlp( _P_NOWAIT, "cmd.exe", "/C", cmdLine, NULL, NULL);//"\"C:\\Dokumente und\ Einstellungen\\Administrator\\Desktop\\ww.pass\"", NULL);	
	ShellExecute(NULL, "open", "http://www.prechacthis.org/", NULL, NULL, SW_SHOWNORMAL);
#endif
	myPrintf("cmdLine = %s\n", cmdLine);
	
	//	r = system(cmdLine);
}

void toggleTableJugglingMode( int dummy )
{
#pragma unused (dummy)
	static int previousPositionMode;
	
	toggle( tableJugglingMode );
	if( tableJugglingMode ){
		forceTableJugglingMode = true;
		previousPositionMode = positionMode;
	}else
		positionMode = previousPositionMode;

	setTableJugglingMode( tableJugglingMode );

	char *p = readFile( actualTempPath, "r");
	startPattern( p );
	free( p );
	setTableJugglingMode( tableJugglingMode );
	
}


// sets a flag that forces next drawScene call to read in default speed / height parameters 
void setDefaultAnimationSpeed( int dummy )
{
#pragma unused (dummy)

	useDefaultAnimationValues = true;
}
