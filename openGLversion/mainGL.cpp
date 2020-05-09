/*
 *  main.cpp
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

#include <iostream> 
using namespace std;

#include "graphics.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "gpl.h"

 
#include "stringtoscreen.h"
#include "languageSupport.h"
#include "jpversion.h"

#define DEF_VARS
#include "globals.h"

#include "dialog.h"

#include "systemswitch.h"

#ifdef MAC_OS_X_VERSION
#include	<Carbon/Carbon.h>
//#include	"MoreFilesX.h"
#include "fileSelectorCocoa.h"
#endif

#if WINDOWS_VERSION
#include <Process.h>
#endif

#include "applicationFolders.h"
#include "pathAccess.h"

#include "fileIO.h"

#include "animationView.h"
#include "animationWin.h"
#include "editor.h"
#include "info.h"

#include "prefs.h"

#include "parseSiteswap.h"
#include "world.h"
#include "pattern.h"
#include "styleScanner.h"
#include "grafik.h"


#include "mainGL.h"
#include "dxf.h"
#include "jpPrefs.h"
#include "cameraControl.h"
#include "openglmenu.h"

#include "playSound.h"
#include "myPrintf.h"
#include "myutil.h"

#include "fileaccess.h"

#include "causal_editor.h"

#include "runaround.h"
//#include "dialog.h"
#include "cmdLine.h"

#include "workspace.h"
#include "appnames.h"

#ifdef MAC_OS_X_VERSION
#if 0
#include "appleEvents.h"
#endif
#endif

#if MAC_CLASSIC_VERSION
#include "macfileaccess.h"
#include "macio.h"
#endif


#define DefaultPattern "<3p 3 3|2@1 3 4p>"
//#define DefaultPattern "< 0 | 0 >"

#define CRASH_FILE_NAME "checkForCrash"
#define FIRST_START_FILE_NAME "restarted"
#define ScreenSaverName "JoePassSaverFlag"
wwwBoolean checkScreensaverMode( );

void    readScreensaverPattern( void );
void    removeTempFiles( void );

wwwBoolean	screenSaverMode = false;
wwwBoolean	screensaverStartedPattern = false;
wwwBoolean	startingUp;
bool		joeHasCrashed;
#define ENVIRONMENT_NAME "model"
#define BODY_NAME "body.dxf"
#define ARM_NAME "arm.dxf"


void    initJoePass( char *p );
void    parseArgs(int argc, char **argv);
void	checkStartup(void );
void	finalizeStartup( void );
void	done( void );
void	initDnd( void );


float	dM[4][4] = DefMatrix;



void initJoePass( char *argv0 )
{
//#pragma unused( argv0 )
	const char *sd, *ad;
	char cd[1000];
	atexit( done );
	
	
	// get my application dir
	getcwd(cd,1000);
	fixDirName( cd );
	
#ifdef WINDOWS_VERSION
//	yyy
//	sd = getAppFolder(cd); 	
	sd = getAppFolder(argv0); 
#else
	sd = getAppFolder(argv0); 
#endif
	ad = getAppDirName();
	
	chdir(ad );

	
	//	checkForWriteProtection();
	{
#define WP_CHECK_FILENAME "checkForWriteProtectionFileName"	
		char p[MAX_FULL_FILE_NAME];
		FILE *fp;
		
		sprintf(p, "%s%s", ad, WP_CHECK_FILENAME);
		fp = fopen(p, "w");
		if( fp == NULL ){
			myPrintf( "JoePass needs write access to it´s home dir");
			exit( 1 );
		}
		fclose( fp );
		remove( p );
	}		
	
	
	{
		FILE *fp;
		time_t t;
		time( &t );
		
		fp = fopen( hotkeyFileName, "w");
		if( fp ){
			fprintf(fp, "JoePass! menu hotkey file, created on %s\n\n", ctime(&t ));
			fclose( fp );
		}else {
			myPrintf( "can´t create file %s\n", hotkeyFileName);
		}

	}
	
	createFolder((char*)sd);	
	chdir( sd);
	
	atexit( removeTempFiles );
	atexit( restorePath);
	
	generatePathNames( sd );
	
	deleteLogfile();
	
	
#ifdef USE_CDEW
	generateCdewPathNames( sd );
#endif
	
	checkStartup();
	atexit( finalizeStartup);
	
	readProfile( );
	getDefaultStyles();             // Muster einlesen vor Aufbau der Menus!
	
	screenSaverMode = checkScreensaverMode();
	
	
	readJPPrefs( true );
	
	if( !screenSaverMode )
		atexit( writeJPPrefs );
	
	initGrafik( );
	//~ initSounds();
	
}

void checkStartup( void )
{
	
	FILE *fp = NULL;
	
	// on the very first start, dont send an error message
	fp = fopen( FIRST_START_FILE_NAME, "r");
	if(fp == NULL )
	{
		fp = fopen( FIRST_START_FILE_NAME, "w");
		if( fp )
			fclose( fp);
		else
			_myPrintf( "this program will not work correctly from write protected drives\n");
		
		return;
	}
	
	
	// try to read my checkfile
	fp = fopen( CRASH_FILE_NAME, "r");
	
	// not there: we had a crash., remove some files
	if(fp == NULL )
	{
		_myPrintf("\n\n\nprefs and pattern files deleted.\n");
		
		removeFile( cdew1StdPath);
		removeFile( cdewSelectPath);
		removeFile( cdewPrefPath);
		removeFile( editPath );
		removeFile( prefsPath);
		removeFile( actualTempPath);
		removeFile( workspacePath);
		removeFile( watchfileSystemPath);
		removeFile( dndfilePath);
		joeHasCrashed = true;
	}else
	{
		fclose( fp );
		remove( CRASH_FILE_NAME);
		joeHasCrashed = false;
	}
}

void finalizeStartup( void )
{
	FILE *fp;
	
	
	//	printf("writing file %s\n", CRASH_FILE_NAME);
	
	// fine. create checkfile (called by atexit())
	if( (fp = fopen( CRASH_FILE_NAME, "w")) != NULL)
		fclose( fp );
}

/*
 just an idea for a screenSaver
 if a sertain flag is set (a file exists) then only the animation window will open,
 and it will be fullscreen
 prefs will not be saved
 */
wwwBoolean checkScreensaverMode( )
{
	FILE *fp;
	wwwBoolean ret;
	char d[1000];
	
	// try to read my checkfile
	fp = fopen( ScreenSaverName, "r");
	
	ret = (fp != NULL );
	if( fp )
	{
		fclose( fp );
		remove( ScreenSaverName );
		_myPrintf( "\n\n\n"
				   "----------------------------\n"
				   "starting in screensaver mode\n"
				   "----------------------------\n");
		
		getcwd( d, 1000);
		_myPrintf("dir = %s\n", d);
	}
	
	return ret;
	
}

void 	InstallAppleEventHandlers( void );
//-------------------- Start ------------------
int mainGL(int argc, char **argv)
{
/*
_myPrintf("argc = %d\n", argc);
	
	for( c = 0; c < argc; c++ )
		_myPrintf("argv[%d] = %s\n", c, argv[c]);
*/
	glutInit(&argc, argv);

	_myPrintf(	"LICENSE:\n%s\nVERSION:\n%s\n\nBuild on %s, %s\n\n\n\n------------ STARTING UP ------------\n\n\n", 
				GPL_TEXT,
			   __JOEPASS_VERSION_STRING__,
			   __DATE__, __TIME__);
	

	
	initPrefs();
	initIO();
	
	srand(time(NULL));
	
	
	// init OpenGL

	glutInitDisplayMode(	GLUT_DOUBLE
							|GLUT_RGB   
							|GLUT_DEPTH 
							);
	
	
	
	
	// init Joe Pass
	initJoePass(argv[0]);
	initLanguage();

	
	initMovie();

	// hier Zugriff verweigert unter windows
	//~ initDnd();
	
/*
 MAC OS X
 open my drag and drop helper to let
 mac os x know what files it can handle (neccessary only once!)
 LSOpenApplication
 Launches the specified application.
 
 OSStatus LSOpenApplication (
 const LSApplicationParameters *appParams,
 ProcessSerialNumber *outPSN
 );
 
 NB:
 dnd will not work 
*/ 
	
	
	initTextEditor();
	
	initAnimation();
	initInfo();
	initRunaround( );
	initDialog();
	
	startingUp = true;
//	callFileSelectorSave( "infoString", workspacePath);
/*	 
	if( 1 ){
		if( screenSaverMode == false){
			
			if( !comandLineStartedPattern )
				createInfoWindow( jpPrefs.showInfoWindow );

			if(jpPrefs.showEditorWindow || cmdLineToTextEditor)
				createTextEditor( );

			if(useCdewWorkspace){
				initCausalEditor( NULL );
				loadDefaultWorkspace();
			}else{
			if(jpPrefs.causalEditorHasOpenWindows )
				initCausalEditor( "untitled");
			}

		}
			
	}

*/
			if( !comandLineStartedPattern )
				createInfoWindow( jpPrefs.showInfoWindow );

			if(jpPrefs.showEditorWindow || cmdLineToTextEditor)
				createTextEditor( );
			
	createAnimationWindow();
	
	parseArgs(argc, argv);
	
	// and load the prefs matrix as default
	animationViewInit(GLUT_LEFT_BUTTON);
	animationSetMatrix(jpPrefs.matrix);
	
	initMyWorld();
	
	
	if(screenSaverMode )
		readScreensaverPattern( );
	
	if( !comandLineStartedPattern ) 
		readAndStartPattern(actualTempPath);
	
	if( !validPattern){
		wwwBoolean ret;
		char *p;
		printf("\nstarting default pattern %s\n", DefaultPattern);
		
		p = (char *)malloc( strlen(DefaultPattern) + 1 );
		if( p )
		{
			sprintf(p, "%s", DefaultPattern);
			ret = startPattern( p );
			printf("starting default pattern ... ");
			printResult( ret == true);
			free( p );
		}else
			printf("not enougth memory aviable\n");
	}
	
	
	if(jpPrefs.saved == false )
		animationViewSet( BIRD_VIEW);
	
	
	// read my models.
	if( makeGlListFromDxfFile( body_list, BODY_NAME ) )
		hasBodyModel = true;
	
	if( makeGlListFromDxfFile( arm_list, ARM_NAME ) )
		hasArmModel = true;
	
	if( makeGlListFromDxfFile( demo_list, ENVIRONMENT_NAME ) )
		hasEnvModel = true;
	
	if(screenSaverMode ){
		glutSetWindow( animationWin );
		glutFullScreen();
		setCameraMode( true );
	}
	
	if( joeHasCrashed == true)
		jpPrefs.helpActive = true;
	
	startingUp = false;

	_myPrintf("\n\n\n");
	

	glutMainLoop();
	
  cout << "Output message" << endl;
  cerr << "Error message" << endl;
	
	return 0;             /* ANSI C requires main to return int. */
}




void done( void )
{
	printf("\nJoePass! ... done\n");
	if( jpPrefs.waitForUserInput )
	{
		_myPrintf("hit enter\n");
		getchar();
	}
}



void readScreensaverPattern( void )
{
#define ScreensaverPattern "screensaver.pass"
	char *p;
	
	
	p = readFile( ScreensaverPattern, "r" );
	_myPrintf("starting screensaver pattern %s ...", ScreensaverPattern);
	printResult( p != NULL );
	
	if( p )
	{
		startPattern( p );
		free( p );
		screensaverStartedPattern = true;
		
	}
	setCameraMode( 1 );
}

void removeTempFiles( void )
{
	char d[1000];
	getcwd( d, 1000);
	
	if( 0 && verboseOutput)
		printf("current path is %s\n", d);
#ifdef USE_CDEW
	removeFile( cdewSelectPath );
#endif
	//        removeFile( actualTempPath);
	removeFile( edit2NewCdewTempPath);
	removeFile( edit2CdewTempPath);
	
}



#ifdef MAC_OS_X_VERSION
void initDnd( void )
{
//	myPrintf("%s\n", dndAppPath);
		FSRef appRef;
		LSApplicationParameters     appParams;
		OSErr err;
		OSStatus	result;
		ProcessSerialNumber outPSN;
		
		// Do it the easy way on 10.4 and later.
		
		/* convert the POSIX path to an FSRef */
		result = FSPathMakeRef((unsigned char *)dndAppPath, &appRef, NULL);
		
		memset(&appParams, 0, sizeof(appParams));
		appParams.version = 0;
		appParams.flags = kLSLaunchDefaults;
		appParams.application = &appRef;
		
		err = LSOpenApplication(&appParams, &outPSN);
	if(err != 0){
		char msg[MAX_FULL_FILE_NAME];
		sprintf(msg, "%s can not be found.No double click on pattern file!", JP__HELPER_NAME);
		showDialog(msg);
	}
	
	myPrintf("err = %d\n", err);
}
#endif

#ifdef WINDOWS_VERSION
char *makeCmdLineArg( char *in);

void initDnd( void )
{
#define INIT_CMD	"install.cmd"
	char p[MAX_FULL_FILE_NAME];

	sprintf( p, "%s%s", sysDir, INIT_CMD);
	char *pn  = makeCmdLineArg( p);
	char *me  = makeCmdLineArg(appDir);

	int r;

	// xxxxx
	r = spawnlp( _P_WAIT, "cmd.exe", "/C", pn, NULL, NULL);//"\"C:\\Dokumente und\ Einstellungen\\Administrator\\Desktop\\ww.pass\"", NULL);
	
	if(r)
		myPrintf("call dndhelper as: %s\nreturns %d", pn, r);

	free( me );
	free( pn );
}
#endif






