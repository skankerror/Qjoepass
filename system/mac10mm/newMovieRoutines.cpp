// use module
#if 0
// not working, 29.07.2015

// select code version a / b

#if 01
/*
 *  newMovieRoutines.cpp
 *  joe2
 *
 *  created by wolfgang on 05.12.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>

#include "macMovie.h"
#include "movie.h"
#include "screenshot.h"

//#include "quicktimeUtil.h"
#include "quicktime.h"
#include "prefDef.h"
#include "string.h"
#include "macfileaccess.h"
#include "pathAccess.h"
#include "patternTitle.h"
#include "nav.h"
#include "myPrintf.h"


FSSpec					flatFSSpec;
CodecType				theCodec        = kAnimationCodecType;
CodecQ					theQuality      = codecNormalQuality;     // for use with other codecs
short					theDepth        = codecInfoDepth16;
bool					quicktimeCompressorSelected;

OSErr					startQuicktimeMovie(void);
void					endQuicktimeMovie( void );
bool					_addFrameToMovieFromFile( char *filename); 
bool                    getQuicktimeCompressionInfo( void );
OSStatus WriteMovieWithDependenciesToFile(Movie jpMovie, CFStringRef inPath);


char                    theMovieName[256];
rect                    movieSize;





Movie		jpMovie;
Handle      dataRef = NULL;
OSType      dataRefType;
DataHandler dataHandler = 0;
OSStatus    err;



//xxxxx make movie neu schreiben





bool getSysdepCompressionInfo( void )
{
	return getQuicktimeCompressionInfo();
}


// contains code snipplets from compression examples (apple source example)
bool getQuicktimeCompressionInfo( void )
{
#if 01
	OSErr result;
	ComponentInstance ci;
	SCSpatialSettings ss;
	CGrafPtr        g;
	CGrafPtr        actWindow;
	GDHandle        dummy;
	ci = OpenDefaultComponent(StandardCompressionType, StandardCompressionSubType);
	GetGWorld( &actWindow, &dummy );
	g = (CGrafPtr)actWindow;
	result = SCSetTestImagePixMap(ci,GetGWorldPixMap(g),nil,0);
	{
		long flags;
		
		SCGetInfo(ci,scPreferenceFlagsType,&flags);
		flags &= ~scShowBestDepth;
		//              flags |= scAllowZeroFrameRate;
		SCSetInfo(ci,scPreferenceFlagsType,&flags);
	}
	
	ss.depth                = theDepth;
	ss.spatialQuality       = theQuality;
	ss.codecType            = jpPrefs.theCodec;
	
	SCSetInfo(ci,scSpatialSettingsType, &ss);
	result = SCRequestImageSettings(ci);
	SCGetInfo(ci,scSpatialSettingsType, &ss);
	
	jpPrefs.theCodec        = ss.codecType;
	theQuality      = ss.spatialQuality;
	theDepth        = ss.depth;
	
	if( result == noErr )
		quicktimeCompressorSelected = true;
	
	return result == noErr;
#else
	jpPrefs.theCodec    = StandardCompressionType;
	theQuality          = mediaQualityNormal;
	theDepth            = 32;
    quicktimeCompressorSelected = true;
    return true;
#endif
}






void _initMovie( void )
{
	initQuicktime( );
	if( !qtInstalled )
		return;
	printf("Quicktime aviable: %s\n", qtInstalled?"yes":"no");
	
	
}
void _exitMovie( void )
{
}


bool _startMovie( void )
{
	char *t;
	
	
	// glutSetWindow( worldWin);
	// add ending to name
	sprintf( theMovieName, "%s", thePatternTitle);
	t = strstr( theMovieName, ".pass");
	if( t != NULL)
		*t = '\0';
	
	// set movie size to display size
	movieSize.left = movieSize.top = 0;
	movieSize.right = glutGet( GLUT_WINDOW_WIDTH );
	movieSize.bottom = glutGet( GLUT_WINDOW_HEIGHT);
	
	
    return startQuicktimeMovie( ) == noErr; //&jpMovie, jpPrefs.movieFrameRate 
	
	
    return true;
	
}


void    _endMovie( void )
{
	endQuicktimeMovie( );
}


//bool _addFrameToMovie( void )
bool _addFrameToMovie( char *lastScreenshotName )
{
	
	GLint w,h;
	char sname[2000];

    //	extern char *lastScreenshotName;
	
	
	const char *sdn;
	char od[2000];
	
	getcwd(od, 2000);
	sdn = getSysDirName();
	
	chdir( sdn);
	
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	
	sprintf(sname, "%s%s", sdn, "temp");
	if( screenShot( sname, w,h) == false )
	{
		_endMovie();
		return false;
	}

	_addFrameToMovieFromFile(lastScreenshotName); 
	
	remove( lastScreenshotName);
	
	chdir( od);
	

	return true;
}

bool _addFrameToMovieFromFile( char *filename) 
{
	
	return true;
	
}

/* --------------------------------------------------------------------------------- */
OSErr startQuicktimeMovie( void )
{
	
	
	
	
	OSErr		err = noErr;
	FSSpec      sfFile;

	char  p[2000];
	char r[] = "a.mov";
	
	Handle	dataRefH		= nil;
	OSType	dataRefType;
	
	
	if( !quicktimeCompressorSelected){
		getQuicktimeCompressionInfo();
		quicktimeCompressorSelected = true;
	}

	strcat(theMovieName, ".mov");
	
	// Start up
	if( !IsQuickTimeInstalled())
		return ~noErr;
	if( (err = EnterMovies())!= noErr)
		return err;
#if !TARGET_API_MAC_CARBON
	c2pstr( theMovieName);
#else
	CopyCStringToPascal(theMovieName, (unsigned char *)theMovieName);
#endif
	
	
	Str255 i2;
	CopyCStringToPascal("save movie as...", i2);
	
	if(navSelectForSave( &flatFSSpec, theMovieName, ".mov", (char *)i2))
		;
	else
		return ~noErr;

	
	getcwd(p, 2000);
	sprintf(p,"%s/%s", p,r);	
	printf("%s\n", p);

	
	// create a file data reference for our movie   
	err = QTNewDataReferenceFromFullPathCFString((CFStringRef )p, 
												 kQTNativeDefaultPathStyle,
												 0,
												 &dataRefH,
												 &dataRefType);	// create the data refenrece
    if (err != noErr) goto bail;
	
	// create a file
    err = CreateMovieStorage(dataRef, dataRefType, FOUR_CHAR_CODE('TVOD'),
                             smSystemScript, createMovieFileDeleteCurFile,
                             &dataHandler, NULL);
    if (err != noErr) goto bail;
bail:	
	return err;
}

void endQuicktimeMovie( void )
{
	// add the Movie
    err = AddMovieToStorage(jpMovie, dataHandler);
    if (err != noErr) {
        DataHDeleteFile(dataHandler);
    }
	
bail:
		// close the storage
		if (dataHandler) CloseMovieStorage(dataHandler);
	
    // remember to dispose the data reference handle
    if (dataRef) DisposeHandle(dataRef);
	
	
	ExitMovies();
}


OSStatus WriteMovieWithDependenciesToFile(Movie jpMovie, CFStringRef inPath)
{
    Handle      dataRef = NULL;
    OSType      dataRefType;
    DataHandler dataHandler = 0;
    OSStatus    err;
	
   
	
    // create a file
    err = CreateMovieStorage(dataRef, dataRefType, FOUR_CHAR_CODE('TVOD'),
                             smSystemScript, createMovieFileDeleteCurFile,
                             &dataHandler, NULL);
    if (err != noErr) goto bail;
	
    // add the Movie
    err = AddMovieToStorage(jpMovie, dataHandler);
    if (err != noErr) {
        DataHDeleteFile(dataHandler);
    }
	
bail:
		// close the storage
		if (dataHandler) CloseMovieStorage(dataHandler);
	
    // remember to dispose the data reference handle
    if (dataRef) DisposeHandle(dataRef);
	
    return err;
}

#else


// inspired by apples quicktimeExample: QTShell

#include "systemswitch.h"

#if MAC_OS_X_VERSION

#include <stdio.h>
#include <string.h>

#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>

#include "quicktime.h"
#include "movie.h"
#include "screenshot.h"
#include "fileaccess.h"
#include "MoreFilesX.h"
#include "prefDef.h"
#include "applicationFolders.h"

#include "nav.h"
#include "myutil.h"
#include "cmdLine.h"

#include "MoreFilesX.h"


#define qtTempName "temp"

char			theMovieName[1000];
rect			movieSize;

extern char		fullPatternPathName[];
extern char		thePatternTitle[];

// ------------------------ quicktime globals -----------------------
typedef struct {FSSpec  movieFile;
	Movie   movie;
	short   resRef;
	short   resId;
	Track   theTrack;
	Media   theMedia;
	rect    size;
}MovieStruct;

enum {
	// Custom result codes for CopyBufferToWindow
	kPixelsPurged = 1,
	kNoGWorld
};

// jpData
MovieStruct     jpMovie;
FSSpec			tempFSSpec;
FSSpec          flatFSSpec;
CodecType       theCodec        = kAnimationCodecType;
CodecQ          theQuality      = codecNormalQuality;     // for use with other codecs
short           theDepth        = codecInfoDepth16;

/* ------------------------- public routines ----------------------------------------- */
// declared in movie.h
// defined here

/* ------------------------- private, i.e. quicktime routines ----------------------------------------- */

bool			getSysdepCompressionInfo( void );

OSErr			DrawPictureIntoGWorld(FSSpec *myFSSpec, GWorldPtr *theGWorld, rect r, short depth);

bool			getQuicktimeCompressionInfoFromGWorld(ComponentInstance *ci, SCSpatialSettings *ss, GWorldPtr theGWorld);

OSErr			lockGWorldPixMap (GWorldPtr buffer, GWorldFlags *oldPixState);
OSErr			unlockGWorldPixMap (GWorldPtr buffer, GWorldFlags oldPixState);
OSErr			copyGWorldToGWorld( GWorldPtr theSource,  GWorldPtr theDest, Rect *drawRect, RgnHandle theRgn);
static OSErr	startQuicktimeMovie(MovieStruct * movieFile, short theFrameRate );
static OSErr	flattenQuicktimeMovie(MovieStruct * movieFile );

bool			addFrameToQuicktimeMovie( char *name );
void			endQuicktimeMovie( void );
static void		addGWorldToMovieSamples( Media theMedia, GWorldPtr master);
OSErr			saveFlattenedMovie (Movie theMovie, FSSpec *sfFile);
OSErr			QTUtils_SetMovieFileLoopingInfo (Movie theMovie, long theLoopInfo);
OSErr			QTUtils_AddMovieInfoToMovie (Movie theMovie, char *theText);
OSErr			QTUtils_AddUserDataTextToMovie (Movie theMovie, char *theText, OSType theType);



#pragma mark ------------------------- public: movie setup routines ----------------------------------------- */
void _initMovie( void ){
	if( !qtInitialized )
		initQuicktime();
}

void _exitMovie( void ){
}


#pragma mark ------------------------- public: movie creation routines ----------------------------------------- */

wwwBoolean _startMovie( void )
{
	char *t;
	
	if( !qtInitialized )
		initQuicktime();
	
	// printf("starting movie\n");
	// add ending to name
	strcpy( theMovieName, thePatternTitle);
	t = strstr( theMovieName, ".pass");
	if( t != NULL)
		*t = '\0';
	
	// set movie size to display size
	movieSize.left = movieSize.top = 0;
	movieSize.right = glutGet( GLUT_WINDOW_WIDTH );
	movieSize.bottom = glutGet( GLUT_WINDOW_HEIGHT);
	
	
    return startQuicktimeMovie( &jpMovie, jpPrefs.movieFrameRate ) == noErr;
}


void    _endMovie( void )
{
	endQuicktimeMovie( );
}




bool _addFrameToMovie( char *filename )
{
	bool ret;
	
	if( filename)
		ret = addFrameToQuicktimeMovie( filename );
	
	
	return ret;
}



#pragma mark ------------------------- movie helper routines ----------------------------------------- */

bool getSysdepCompressionInfo( void )
{
	
	GWorldPtr theGWorld = NULL;
	FSSpec spec;
	GLint w,h;
	char *tmpName;
	extern int animationWin;
	rect r;
	bool ret = true;
	extern bool cmdLineInitMovie;
	
	ComponentInstance ci;
	SCSpatialSettings ss;
	long	flags;
	
	const char *sdn;
	char od[2000];
	
	getcwd(od, 2000);
	sdn = getSysDirName();
	
	chdir( sdn);
	
	glutSetWindow( animationWin);
	r.bottom = w = glutGet(GLUT_WINDOW_WIDTH);
	r.right  = h = glutGet(GLUT_WINDOW_HEIGHT);
	r.left = r.top = 0;
	
	
	ci = OpenDefaultComponent(StandardCompressionType, StandardCompressionSubType);
	if( ci ){
		
		SCGetInfo(ci,scPreferenceFlagsType,&flags);
		flags &= ~scShowBestDepth;
		SCSetInfo(ci,scPreferenceFlagsType,&flags);
		
		SCGetInfo(ci,scSpatialSettingsType, &ss);
		ss.depth          = theDepth;
		ss.spatialQuality = theQuality;
		ss.codecType      = kAnimationCodecType;//jpPrefs.theCodec;
		SCSetInfo(ci,scSpatialSettingsType, &ss);
		
		if( cmdLineInitMovie == false ){
			
			// make a screenshot and draw the pict into a gworld 
			screenShot( qtTempName, w,h);
			tmpName = getLastScreenshotFilename( );
			if( tmpName){
				ret = FSPathMakeFSSpec((const UInt8 *)tmpName,  &spec, NULL) == noErr;
				DrawPictureIntoGWorld(&spec, &theGWorld, r, 0);
				
				ret = getQuicktimeCompressionInfoFromGWorld(&ci, &ss, theGWorld);
				
				DisposeGWorld( theGWorld );
				
				remove( tmpName);
			}else
				ret = false;
		}
		CloseComponent( ci );
		//		ret = true;
	}else
		ret = false;
	
	
	chdir( od);
	return ret;
}




OSErr startQuicktimeMovie(MovieStruct *theMovie, short theFrameRate)
{
	OSErr		err = noErr;
	Str255		i2 = "\p";
	CopyCStringToPascal("save quicktime movie", i2);
	CFStringRef	mn;
	// Start up
	if(!qtInstalled)
		return ~noErr;
	
	strcat(theMovieName, ".mov");
	mn = CFStringCreateWithCString( NULL, theMovieName, kCFStringEncodingMacRoman);
	CopyCStringToPascal(theMovieName, (unsigned char *)theMovieName);
	if(!navSelectForSave( &flatFSSpec, theMovieName, ".mov", (char *)i2))
		return ~noErr;			
	
	// Make an FSSpec for a temp movie file
	// which will later be flattened
	// FSMakeFSRefUnicode
		
	FSMakeFSSpec (0, 0L, (const unsigned char *)"\ptemp.mov", &tempFSSpec);
	
	// Fill in information
	jpMovie.movieFile = tempFSSpec;
	err = CreateMovieFile( &theMovie->movieFile, 'TVOD', smCurrentScript, createMovieFileDeleteCurFile,
						  &theMovie->resRef, &theMovie->movie );
	QTUtils_SetMovieFileLoopingInfo (theMovie->movie, 0);
	
	//      Make a video track in desired size
	theMovie->theTrack = NewMovieTrack( theMovie->movie, FixRatio(movieSize.right, 1), FixRatio(movieSize.bottom,1),0);
	err = GetMoviesError();
	
	theMovie->theMedia = NewTrackMedia( theMovie->theTrack, VideoMediaType, theFrameRate, NULL, 0 );
	err = GetMoviesError();
	
	err = BeginMediaEdits( theMovie->theMedia );
	return err;
}

void endQuicktimeMovie( void )
{       
	OSErr err = noErr;
	
	err = InsertMediaIntoTrack( jpMovie.theTrack, 0, 0, GetMediaDuration( jpMovie.theMedia ), 0x00010000L );
	err = EndMediaEdits( jpMovie.theMedia );
	err = AddMovieResource( jpMovie.movie, jpMovie.resRef, &jpMovie.resId, jpMovie.movieFile.name );
	if (jpMovie.resRef)
		CloseMovieFile( jpMovie.resRef );
	
	// close movie, clean up
	flattenQuicktimeMovie( &jpMovie);
	DisposeMovie( jpMovie.movie );
	DeleteMovieFile ((const FSSpec *) &jpMovie.movieFile);
	
}



OSErr flattenQuicktimeMovie( MovieStruct *theMovie)
{
	OSErr err = noErr;
	// save temp file as flattened movie and delete it
	err = saveFlattenedMovie(theMovie->movie, &flatFSSpec);
	if( err )
		printf("movie creation error code: %0x\n", err);
	
	return err;
}


OSErr saveFlattenedMovie (Movie theMovie, FSSpec *sfFile)
{
	OSErr   myErr = noErr;
	short   flags;
	
	if (theMovie == NULL)
		return(invalidMovie);
	
	flags = flattenAddMovieToDataFork;
	FlattenMovieData(theMovie, flags, sfFile, FOUR_CHAR_CODE('TVOD'), smSystemScript, createMovieFileDeleteCurFile);
	myErr = GetMoviesError();
	return(myErr);
}




OSErr QTUtils_SetMovieFileLoopingInfo (Movie theMovie, long theLoopInfo)
{
    UserData            myUserData;
    short                       myCount = 0;
	OSErr                   myErr = paramErr;
	// get the movie's user data
    myUserData = GetMovieUserData(theMovie);
	if (myUserData == NULL)
		goto bail;
	// we want to end up with at most one user data item of type 'LOOP',
	// so let's remove any existing ones
    myCount = CountUserDataType(myUserData, FOUR_CHAR_CODE('LOOP'));
    while (myCount--)
        RemoveUserData(myUserData, FOUR_CHAR_CODE('LOOP'), 1);
	
	myErr = SetUserDataItem(myUserData, &theLoopInfo, sizeof(long), FOUR_CHAR_CODE('LOOP'), 0);
bail:
	return(myErr);
}



bool getQuicktimeCompressionInfoFromGWorld(ComponentInstance *ci, SCSpatialSettings *ss, GWorldPtr theGWorld)
{
	
	Rect		myRect;
	GWorldFlags	oldPixState;
	bool		ret = true;
	OSErr		result;
	
	
	GetPortBounds( theGWorld, &myRect);
	lockGWorldPixMap ( theGWorld, &oldPixState);
	
	
	
	// set image
	result = SCSetTestImagePixMap(*ci,GetGWorldPixMap(theGWorld), &myRect, 0);
	
	
	// bring up the dialog
	result = SCRequestImageSettings(*ci);
	if( result != noErr)
		return false;
	
	SCGetInfo(*ci,scSpatialSettingsType, ss);
	jpPrefs.theCodec  = ss->codecType;
	theQuality        = ss->spatialQuality;
	theDepth          = ss->depth;
	
	if(jpPrefs.theCodec == kPixletCodecType ){
		ret = false;
		_myBeep();
	}
	
	
	unlockGWorldPixMap ( theGWorld, oldPixState);
	
	return ret;
}

bool addFrameToQuicktimeMovie( char *filename )
{
	
	GWorldPtr theGWorld = NULL;
	FSSpec spec;
	OSErr ret;
	
	ret = FSPathMakeFSSpec((const UInt8 *)filename,  &spec, NULL);
	DrawPictureIntoGWorld(&spec, &theGWorld, movieSize, 0);
	
	// add frame
	addGWorldToMovieSamples(jpMovie.theMedia, theGWorld);
	DisposeGWorld( theGWorld );
	return ret == noErr;
}

/**************** DRAW PICTURE INTO GWORLD ***********************/
//
// Uses Quicktime to load any kind of picture format file and draws
// it into the GWorld
//
//
// INPUT: myFSSpec = spec of image file
//

OSErr DrawPictureIntoGWorld(FSSpec *myFSSpec, GWorldPtr *theGWorld, rect mr, short unused)
{
#pragma unused( unused)
	
    OSErr                       iErr;
    GraphicsImportComponent     gi;
    Rect                        r1;
    ComponentResult             result;
    GWorldFlags                 oldPixState;
	Rect						myRect;
	
	
	myRect.left		= mr.left;
	myRect.right	= mr.right;
	myRect.top		= mr.top;
	myRect.bottom	= mr.bottom;
	
    result = GetGraphicsImporterForFile(myFSSpec, &gi);        // load importer for this image file
    if (result != noErr)
        return(result);
    GraphicsImportGetBoundsRect(gi, &r1);                    // get dimensions of image
	
	if (*theGWorld != nil)
        DisposeGWorld(*theGWorld);
	
	iErr = NewGWorld( theGWorld, 0, &myRect , nil, nil, (GWorldFlags) 0 );
    if (iErr)
    {
        printf("can't create movie gworld (Add memory!)\n");
        return iErr;
    }
	
	
	/* DRAW INTO THE GWORLD */
	
	//    DoLockPixels(*theGWorld);
    lockGWorldPixMap (*theGWorld, &oldPixState);
	
    GraphicsImportSetGWorld(gi, *theGWorld, nil);            // set the gworld to draw image into
    GraphicsImportSetQuality(gi,codecLosslessQuality);        // set import quality
	
    {
        RGBColor    cc;
        GraphicsImportSetGraphicsMode(gi, srcCopy, &cc);    // set copy mode (no dither!)
    }
	
    result = GraphicsImportDraw(gi);                        // draw into gworld
    CloseComponent(gi);                                        // cleanup
    if (result != noErr)
    {
        DisposeGWorld (*theGWorld);
        return(result);
    }
    unlockGWorldPixMap (*theGWorld, oldPixState);
	
    return(noErr);
}
// ---------------------- advanced -------------------------------------
OSErr lockGWorldPixMap (GWorldPtr buffer, GWorldFlags *oldPixState)
{
	PixMapHandle    bufferPixMap;
	OSErr                   err = noErr;
	
	if (buffer) {
		/* Lock down our off-screen image */
		bufferPixMap = GetGWorldPixMap(buffer);
		*oldPixState = GetPixelsState(bufferPixMap);
		if (!LockPixels(bufferPixMap))
			err = kPixelsPurged;
	}
	return err;
}
OSErr unlockGWorldPixMap (GWorldPtr buffer, GWorldFlags oldPixState)
{
	PixMapHandle    bufferPixMap;
	/* Undo the effects of "lockGWorldPixMap" */
	if (buffer) {
		bufferPixMap = GetGWorldPixMap(buffer);
		SetPixelsState(bufferPixMap, oldPixState);
	}
	return noErr;
}

static void addGWorldToMovieSamples( Media theMedia, GWorldPtr master)
{
	long            maxCompressedSize;
	GWorldPtr       theGWorld;
	Handle          compressedData = nil;
	Ptr                     compressedDataPtr;
	ImageDescriptionHandle  imageDesc = nil;
	CGrafPtr        oldPort;
	GDHandle        oldGDeviceH;
	OSErr           err = noErr;
	Rect        myRect;
	GWorldFlags oldPixState;
	
	GetGWorld( &oldPort, &oldGDeviceH );
	SetGWorld( master, nil );
	GetPortBounds(master, &myRect);
	
	err = NewGWorld( &theGWorld, 0, &myRect , nil, nil, (GWorldFlags) 0 );
	lockGWorldPixMap (theGWorld, &oldPixState);
	
	err = GetMaxCompressionSize( GetGWorldPixMap(theGWorld), &myRect, 0,
								theQuality, theCodec, (CompressorComponent) bestCompressionCodec,
								&maxCompressedSize );
	
	compressedData = NewHandle(maxCompressedSize);
	if (compressedData == NULL)
		goto bail;
	
	HLockHi(compressedData);
	compressedDataPtr = *compressedData;
	
	imageDesc = (ImageDescriptionHandle)NewHandle(4);
	if (imageDesc == NULL)
		goto bail;
	
	// copy content of given GWorld into mine
	copyGWorldToGWorld( master,  theGWorld, &myRect, nil);
	
	// Add it to movie
	err = CompressImage( 
						GetGWorldPixMap(theGWorld), 
						&myRect, 
						theQuality,
						jpPrefs.theCodec, 
						imageDesc, 
						compressedDataPtr );
	
	
	err = AddMediaSample( theMedia, compressedData, 0, (**imageDesc).dataSize,
						 1, (SampleDescriptionHandle) imageDesc, 1, 0, nil );
	
bail:
	if (imageDesc != NULL)
		DisposeHandle( (Handle) imageDesc );
	if (compressedData != NULL)
		DisposeHandle( compressedData );
	if (theGWorld != NULL)
		DisposeGWorld( theGWorld );
	SetGWorld( oldPort, oldGDeviceH );
	
	if( err )
		printf("error detected\n");
}

OSErr copyGWorldToGWorld( GWorldPtr theSource,  GWorldPtr theDest, Rect *drawRect, RgnHandle theRgn)
{
	/* Copy the image from the offscreen buffer to the window */
	CGrafPtr                savedCPort;
	GDHandle                savedGDevice;
	OSErr                   err;
	Rect                    sourceRect, portRect;
	GWorldFlags             oldPixState;
	GWorldFlags             oldPixState2;
	PixMapHandle    thePixMap;
	PixMapHandle    thePixMap2;
	
	// Nur kopieren, wenn ein Buffer da ist!
	if( theSource == nil )
		return noErr;
	
	GetGWorld(&savedCPort, &savedGDevice);
	SetGWorld(theDest, nil);
	
	err = lockGWorldPixMap(theSource, &oldPixState);
	err = lockGWorldPixMap(theDest, &oldPixState2);
	if (err)
		goto done;
	
	thePixMap =  GetGWorldPixMap(theSource);
	thePixMap2 = GetGWorldPixMap(theDest);
	
	GetPortBounds( theSource, &sourceRect);
	GetPortBounds( theDest,   &portRect);
	
	if(drawRect )
		sourceRect = portRect = *drawRect;
	
	
	/* Draw, podnah */
	SetGWorld(theDest, nil);
	ForeColor( blackColor );
	BackColor( whiteColor );
	
	CopyBits(
			 (BitMap*)*thePixMap,
			 (BitMap*)*thePixMap2,
			 &sourceRect,
			 &portRect,
			 srcCopy, theRgn );
	
	unlockGWorldPixMap(theDest, oldPixState2);
	unlockGWorldPixMap(theSource, oldPixState);
done:
	SetGWorld(savedCPort, savedGDevice);
	return err;
}


#endif

// Select code version a / b
#endif

// use module
#endif