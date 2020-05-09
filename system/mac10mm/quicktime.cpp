//xxxxx
#if 01

/*
 *  quicktime.cpp
 *  joe
 *
 *  created by Wolfgang Westerboer on 19.07.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "systemswitch.h"
#ifdef MAC_OS_X_VERSION
#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>

#include "quicktime.h"
#include "movie.h"
#include "cmdLine.h"
#include "myPrintf.h"

bool		qtInstalled		= false;
bool		qtInitialized	= false;

#if 01
#define USE_QUICKTIME
#endif

static bool     QTUtils_IsQuickTimeInstalled (void);


void initQuicktime( void )
{
	if( qtInitialized )
		return;
#ifdef USE_QUICKTIME
	EnterMovies();
	qtInstalled =QTUtils_IsQuickTimeInstalled ();
	if( verboseOutput )
		printf("Quicktime aviable: %s\n", qtInstalled?"yes":"no");
	qtInitialized = true;
#endif
	atexit( exitQuicktime );
}

void exitQuicktime( void ){	
#ifdef USE_QUICKTIME
	ExitMovies();
#endif
}

bool IsQuickTimeInstalled( void )
{
    return QTUtils_IsQuickTimeInstalled();
}

//////////
//
// QTUtils_IsQuickTimeInstalled
// Is QuickTime installed?
//
//////////
bool QTUtils_IsQuickTimeInstalled (void)
{
	wwwBoolean      myQTAvail = false;
	long            myAttrs;
	OSErr           myErr = noErr;
#ifdef USE_QUICKTIME
	myErr = Gestalt(gestaltQuickTime, &myAttrs);
	if (myErr == noErr)
		myQTAvail = true;
	return(myQTAvail);
#endif

}
#endif

#endif