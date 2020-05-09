/*
 *  movie.cpp
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

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
using namespace std;

#include "movie.h"
#include "fileIO.h"
#include "vtypes.h"
#include "prefDef.h"
#include "myPrintf.h"
#include "mem.h"
#include "siteswap.h"
#include "screenshot.h"
#include "animationWin.h"
#include "info.h"
#include "globals.h"
#include "world.h"

#include "pathAccess.h"
#include "applicationFolders.h"



#define			DirSize 1000
char			mediaSubFolder[DirSize] = ".";
char			fullMediaSubFolder[DirSize] = ".";
#define			mediaTempName "screen"
int				movieCounter = 0;


static  bool	addToMovie = false;

float			oldtimeStep;
bool			startCol;
int				minFrames;
Vector3D		*objectPositions = NULL;
long			movieObjectStyle;
long			framesToDraw;
long			framesInMovie;
prefsSettings	movieTempSettings;

static bool		movieCreated = false;
static int		screenshotCounter;

void initMovie( void )
{	
	char	curDir[DirSize];	
	
	getcwd( curDir, DirSize );
	chdir(appDir);
	createFolder(mediaPath);
	chdir( curDir );	
}

movie::movie( bool mode )
{
	movieSetupFinished = false;
	screenshotMovieCounter = 0;
	
	//~ _initMovie();
	startMovie( mode );
}

movie::~movie( void )
{
	//_exitMovie();
}


bool movie::getCompressionInfo( void )
{
	//return getSysdepCompressionInfo();
	return false;
}



// initializes movie creation
bool movie::startMovie( bool mode )
{
	char n[DirSize];
	sprintf( n, "%s%d%c", "movie", movieCounter, PATH_DIVIDER);

	// only one movie at a time
    if( addToMovie ){
		_myPrintf("allready in Movie Creation Process\n");
		return false;
	}
    
	theMovieScreenshotMode = mode;
	
	if( theMovieScreenshotMode == screenshotMode ){	
		makeNewScreenshotFolder( n );
		movieFrameCounter = 0;
	}else{
		if( getCompressionInfo() == false )
				return false;
		
		//~ if( _startMovie() == false )
			return false;
	}
	
	
	// save settings
	movieTempSettings = jpPrefs;
	// set object style
	movieObjectStyle = jpPrefs.os;
	// calc movie frames and timestep
	framesToDraw = framesInMovie = getFramesInMovie( );
	oldtimeStep = timeStep;
	timeStep = jpPrefs.animationSpeed/jpPrefs.movieFrameRate;
	
	
	glutSetWindow( animationWin );
	saveObjectPosition();
	addToMovie = true;
	
	movieSetupFinished = true;
    return true;
}


void movie::endMovie( void )
{
	
	if( addToMovie )
	{
		//~ if( theMovieScreenshotMode == movieMode)
			//~ _endMovie();
		
		movieCounter++;
		
		// clean up for Joe
		jpPrefs = movieTempSettings;
		timeStep = oldtimeStep;
		
		jpPrefs.os = movieObjectStyle;
		glutSetWindowTitle( AnimationWindowTitle);
		
		addToMovie = false;
		
	}
}

/* -------------------------------------------------------------------------------------- */
// calculate framesInMovie depending on object style
long movie::getFramesInMovie( void )
{
	float throwsInPattern;
	float fim;
	
	// calulate number of framesToDraw In Movie
	if( jpPrefs.objectColorMode == multiColorObj)
		throwsInPattern = patternLength * jpPrefs.movieFrameRate;
	else
		throwsInPattern = patternTime * jpPrefs.movieFrameRate;
	throwsInPattern--;
	
	fim = throwsInPattern * 1./jpPrefs.animationSpeed * 2.;
	/*      if (ash)
	 fim *= 2;
	 */
	return ceil(fim);
}





/* calculation of movie length (i.e. throws in pattern)
 does not work in hurried patterns
 heres a hack: on movie start, save all objects positions
 then compare them with the actual positions
 do this at least for one complete pattern cycle (otherwise 3333342 would stop too early)
 stop if every object is on the (close to the) starting position
 */

void movie::saveObjectPosition( void )
{
	int c;
	
	if( objectPositions )
		free(objectPositions);
	objectPositions = (Vector3D *)malloc( ballCount * sizeof( Vector3D ));
	
	for( c = 0; c < ballCount; c++ )
		*(objectPositions+c) = getObject( c ) -> a;
	
	startCol = col;
	minFrames = patternTime * jpPrefs.movieFrameRate;
}



// does not work with patterns containing zeroes, like 120
// as objects reach same positions before pattern repeats
bool movie::compareObjectPosition( void )
{
	int c;
	Vector3D d;
	float delta = 20;
	
	minFrames--;
	
	
	if( minFrames > 0 )
		return false;
	
	for( c = 0; c < ballCount; c++ )
	{
		d =  (objectPositions[c]) - (getObject( c ) -> a);
		if( d.betrag() > delta )
			return false;
		
	}
	//return true;
	return false;
}



// add one frame to the movie.
bool movie::addFrameToMovie( void )
{
	
	const	char *sdn;
	char	od[2000];
	GLint	w,h;
	char	*tmpName;
	bool	ret = true;
	int		c;
	char	dMovieTitle[255];
	long	percent;
	
	
	getcwd(od, 2000);
	sdn = getSysDirName();
	
	chdir( sdn);
	
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

#if 01
	// make a screenshot
	if( !screenShot( mediaTempName, w,h) ){
		chdir( od );
		return false;
	}
	if( (tmpName = getLastScreenshotFilename( ))== NULL ){
		chdir( od );
		return false;
	}
#else	
	
	char n[2000];
	static int cnt = 0;
	
	sprintf(n, "%s%08d", mediaTempName, cnt++);
	
	// make a screenshot
	if( !screenShot( n, w,h) ){
		chdir( od );
		return false;
	}
	if( (tmpName = getLastScreenshotFilename( ))== NULL ){
		chdir( od );
		return false;
	}
	
	copyFile( mediaTempName, n );
	
#endif
			
	if(theMovieScreenshotMode == screenshotMode){
		char *p = (char *)malloc(strlen( mediaPath) + strlen(ScreenShotFileName) + strlen(mediaSubFolder) + strlen(fType) + 100 );
		sprintf(p, "%s%s%08d%s", fullMediaSubFolder, ScreenShotFileName, screenshotMovieCounter++, fType);
		copyFile( p, tmpName);
		free( p );
	}/*else{
		ret = _addFrameToMovie( tmpName );
	}*/
	remove( tmpName);
	chdir( od );
	if( ret == false )
		return ret;
	
	movieCreated = true;
	
	
	// count down
	framesToDraw--;
	// show information in windowTitle
	percent = 100-(100*framesToDraw)/framesInMovie;
	c = sprintf(dMovieTitle, "%ld%% completed",percent);
	dMovieTitle[c] = '\0';
	glutSetWindowTitle(dMovieTitle);
	// look if we are through with the movie
	if( framesToDraw == 0l || compareObjectPosition()  )
		return false;
	
	
    return true;
}






void makeOneScreenshot( void )
{
	GLint w,h;
	char od[DirSize];
	char *p = (char *)malloc(strlen( mediaPath) + strlen(ScreenShotFileName) + strlen(mediaSubFolder) + 10 );
	
	getcwd(od, DirSize);
	
	
	chdir(mediaPath);
	sprintf(p, "%s%d", ScreenShotFileName, screenshotCounter++);
	
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	
	screenShot (p, w, h);
	
	
	chdir( od );	
	
}



void movie::makeNewScreenshotFolder( char *name )
{
	char dir[DirSize];
	int ret;
	
	getcwd(dir, DirSize);
	
	chdir(mediaPath);
	
	sprintf(mediaSubFolder, "%s", name);
	ret  = createFolder( mediaSubFolder);
	
	chdir(mediaSubFolder);	
	getcwd(fullMediaSubFolder, DirSize);
	fixDirName( fullMediaSubFolder);
	//	_myPrintf("fullMediaSubFolder = %s\n", fullMediaSubFolder);
	
	chdir(dir);
}	

char *movie::getMediaSubfolder( void )
{
	return fullMediaSubFolder;
}

