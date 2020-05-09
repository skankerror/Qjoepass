 /*
 uses old calls from the pre ppc aera
 not longer working!
 
	handles openDoc event:
 copy the location of file to application dir in file watchfile.txt
*/

// defined in project settings
#if defined DND
#include	<Carbon/Carbon.h>
#include	<unistd.h>
#include	"MoreFilesX.h"
#include	"appnames.h"

//#define ExitToShell() //ExitToShell()
 
		void		InstallAppleEventHandlers( void );
static	OSStatus	OpenDocuments( AEDescList docList );		//  Add call to DisplayMovieInWindow()
		int			main( int argc, char *argv[] );								//  Added call to EnterMovies()



#define				MAX_FULL_FILE_NAME	2000
#define				PATH_DIVIDER		'/'
#define				DEST_NAME			"joe.pass"

// my file and folder names
// I do not need all of them
static char			fullPatternPathName[MAX_FULL_FILE_NAME],
					PathToJoe[MAX_FULL_FILE_NAME],
					PathToWatchFile[MAX_FULL_FILE_NAME]; 


static char			*getFileNameFromFSSpec( FSSpec *sfFile);
void				doAlert( int id, char *error, char *explanation);
static bool			writeFile( char *name, char *mode, char *buffer, size_t size );
void				launchApp( void );


#pragma mark -
#pragma mark ¥ Main ¥

// defined in project settings
#ifdef DND_STANDALONE
int	main( int argc, char *argv[] )
 {
	int i;
	char *p;
	char tmp[MAX_FULL_FILE_NAME];
	
//	doAlert( kAlertStopAlert, "main 1", "nothing" );
	sprintf(tmp, "%s", argv[0]);
	
	// get my bundle directory (actual executable is 4 directory levels deeper than the bundle dir
	for( i = 0; i < 4; i++ ){
		p = tmp;
		p = strrchr(p, PATH_DIVIDER );
		if( p )
			*p = '\0';
	}
	sprintf(PathToWatchFile, "%s%c%s", tmp, PATH_DIVIDER, JP__HELPER_FILENAME);
			
	for( i = 0; i < 1; i++ ){
		p = strrchr(tmp, PATH_DIVIDER );
		if( p )
			*p = '\0';
	}
	
	
	sprintf(PathToJoe, "%s%c%s", tmp, PATH_DIVIDER, JP_NAME);
	
//	doAlert( kAlertStopAlert, "main 2", "nothing" );
	InstallAppleEventHandlers();
	
//	doAlert( kAlertStopAlert, "main 3", "nothing" );
	RunApplicationEventLoop();											//	Call the event loop

	return 0;
	
}
#endif


void doAlert( int id, char *error, char *explanation)
{
	DialogRef	alertDialog;
	CFStringRef cferror = CFStringCreateWithCString (NULL, error, kCFStringEncodingASCII);
	CFStringRef cfexplanation = CFStringCreateWithCString (NULL, explanation, kCFStringEncodingASCII);

	CreateStandardAlert( kAlertStopAlert, cferror, cfexplanation, NULL, &alertDialog );
	RunStandardAlert( alertDialog, NULL, NULL );
	
}	


#pragma mark -
#pragma mark ¥ AppleEvent Handlers ¥

static	pascal	OSErr	HandleAppleEventOapp( const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefcon )
{
	
	ExitToShell( );	
	return( noErr );
}

static	pascal	OSErr	HandleAppleEventRapp( const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefcon )
{
	return( noErr );
}

static	pascal	OSErr	HandleAppleEventOdoc( const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefcon )
{
	AEDescList		docList;
	OSErr			err			= AEGetParamDesc( theAppleEvent, keyDirectObject, typeAEList, &docList );
	require_noerr( err, CantGetDocList );
	
//	doAlert( kAlertStopAlert, "open doc event", "nothing" );
	err	= OpenDocuments( docList );
	AEDisposeDesc( &docList );

CantGetDocList:
  	return( err );
}

static	pascal	OSErr	HandleAppleEventPdoc( const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefcon )
{
	return( errAEEventNotHandled );
}

void	InstallAppleEventHandlers( void )
{
	OSErr		status;

	status	= AEInstallEventHandler( kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(HandleAppleEventOapp), 0, false );	require_noerr( status, CantInstallAppleEventHandler );
	status	= AEInstallEventHandler( kCoreEventClass, kAEReopenApplication, NewAEEventHandlerUPP(HandleAppleEventRapp), 0, false );	require_noerr( status, CantInstallAppleEventHandler );
	status	= AEInstallEventHandler( kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(HandleAppleEventOdoc), 0, false );		require_noerr( status, CantInstallAppleEventHandler );
	status	= AEInstallEventHandler( kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(HandleAppleEventPdoc), 0, false );	require_noerr( status, CantInstallAppleEventHandler );
	//	Note: Since RunApplicationEventLoop installs a Quit AE Handler, there is no need to do it here.

CantInstallAppleEventHandler:
	return;
}



static char *getFileNameFromFSSpec( FSSpec *sfFile)
{
	char *pd;
	short ret;
	char tmp[MAX_FULL_FILE_NAME];
	
	ret = FSMakePath(sfFile->vRefNum, 
					 sfFile->parID, 
					 sfFile->name , 
					 (unsigned char *)tmp, 
					 MAX_FULL_FILE_NAME);
	
	pd = strrchr( tmp, PATH_DIVIDER);
	if( pd == NULL )
		return NULL;
	
	*pd = '\0';
	sprintf( fullPatternPathName, "%s", tmp); 
	*pd = PATH_DIVIDER;
	
	return fullPatternPathName;
}

// writes a file, stdDir is applicationDir
// destroys original file content
// input; name
// return: true for success
static bool writeFile( char *name, char *mode, char *buffer, size_t size )
{
	static FILE *fp;
	size_t w;
	printf("watchfile = %s\n", name );
	if( (fp = fopen( name, mode)) != NULL ){
		
		w = fwrite( buffer, 1, size * sizeof( char ), fp );
		fclose( fp );
		return true;
	}
	return false;
}




static	OSStatus	OpenDocuments( AEDescList docList )
{
	long				index;
	FSRef				fsRef;
	long				count			= 0;
	OSStatus			status			= AECountItems( &docList, &count );

	for( index = 1; index <= count; index++ )
	{
		if ( (status = AEGetNthPtr( &docList, index, typeFSRef, NULL, NULL, &fsRef, sizeof(FSRef), NULL) ) == noErr )
		{
			FSRefMakePath( &fsRef, (unsigned char *)fullPatternPathName, MAX_FULL_FILE_NAME-1 );
			writeFile( PathToWatchFile, "w", fullPatternPathName, strlen(fullPatternPathName) );
//			doAlert( kAlertStopAlert, PathToWatchFile, fullPatternPathName );
			launchApp();
			ExitToShell();
		}else {
			ExitToShell();
			return status;
		} 

	}
	ExitToShell();
	return( status );
}




void launchApp( void )
{
	FSRef appRef;
	LSApplicationParameters     appParams;
	OSErr err;
	OSStatus	result;
	ProcessSerialNumber outPSN;

	// Do it the easy way on 10.4 and later.
//	doAlert( kAlertStopAlert, "launch app", "nothing" );
	
	/* convert the POSIX path to an FSRef */
	result = FSPathMakeRef((unsigned char *)PathToJoe, &appRef, NULL);

	memset(&appParams, 0, sizeof(appParams));
	appParams.version = 0;
	appParams.flags = kLSLaunchDefaults;
	appParams.application = &appRef;
	
	err = LSOpenApplication(&appParams, &outPSN);
	


}

#endif


