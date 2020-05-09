/*
 *  appleEvents.cpp
 *  joe2
 *
 * created by Wolfgang Westerboer on 21.12.07.
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---TODAY>  Wolfgang Westerboer
 
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
#include "appleEvents.h"
#include "myPrintf.h"
#include "myutil.h"
#include "prefDef.h"
#include "fileIO.h"
#include "animationWin.h"

#if 0
#ifdef MAC_OS_X_VERSION
#define USE_APPLE_EVENTS
#endif
#endif

#ifdef USE_APPLE_EVENTS
#include <CarbonCore/MacTypes.h>
OSErr InitAppleEvents( void );
#endif

#ifdef MAC_OS_X_VERSION
void InitMac( void )
{
#ifdef USE_APPLE_EVENTS
	InitAppleEvents();
#endif
	
}



#endif




#ifdef USE_APPLE_EVENTS

OSErr InitAppleEvents( void );

typedef wwwBoolean(*AEOAR)(const AppleEvent *messageIn, AppleEvent *reply,UInt32 handlerRefCon);
typedef wwwBoolean(*AEODR)(const AppleEvent *messageIn, AppleEvent *reply,UInt32 handlerRefCon);
typedef wwwBoolean(*AECAR)(const AppleEvent *messageIn, AppleEvent *reply,UInt32 handlerRefCon);

AEEventHandlerUPP gOpenAppUPP, gOpenDocUPP, gCloseAppUPP;

AEOAR   openAppRoutine = nil;
AEODR   openDocRoutine = nil;
AECAR   closeAppRoutine = nil;

pascal OSErr aeOpenApp (const AppleEvent *messageIn, AppleEvent *reply, long handlerRefCon);
pascal OSErr aeCloseApp (const AppleEvent *messageIn, AppleEvent *reply, long handlerRefCon);
pascal OSErr aeOpenDocs (const AppleEvent *messageIn, AppleEvent *reply, long handlerRefCon); // war UInt32
#endif



#ifdef USE_APPLE_EVENTS

OSErr InitAppleEvents( void )
{
	OSErr err = noErr;
	jpPrefs.writeLogfile = true;


	/*
	 historical stuff:
	 to use apple events, we need an event loop that handles apple events.
	 As glut doesnt do so, well leave the job to sioux
	 Therefore, we have to make an output to the console.
	 // NOT WORKING IN CARBON!
	 InitSIOUXConsole();
	 */
	myPrintf("activating apple events\n");
	gOpenAppUPP = NewAEEventHandlerUPP (aeOpenApp);
	err = AEInstallEventHandler(kCoreEventClass, kAEOpenApplication,
								gOpenAppUPP, nil, false);
	
	if( err )
		_myPrintf("gOpenAppUPP = %lx => err = %d\n", gOpenAppUPP, err);
	
	gOpenDocUPP = NewAEEventHandlerUPP (aeOpenDocs);
	err = AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
								gOpenDocUPP, nil, false);
	
	if( err )
		_myPrintf("gOpenDocUPP = %lx => err = %d\n", gOpenDocUPP, err);
	
	gCloseAppUPP = NewAEEventHandlerUPP (aeCloseApp);
	err = AEInstallEventHandler(kCoreEventClass, kAEQuitApplication,
								gCloseAppUPP, nil, false);
#if 01
	myPrintf("gOpenDocUPP = %lx => err = %d\n", gCloseAppUPP, err);
	myPrintf("gOpenAppUPP = %lx => err = %d\n", gOpenAppUPP, err);
	myPrintf("gOpenDocUPP = %lx => err = %d\n", gOpenDocUPP, err);
	myPrintf("gCloseAppUPP = %lx => err = %d\n", gCloseAppUPP, err);
#endif
	return err;
}


pascal OSErr aeOpenApp (const AppleEvent *messageIn, AppleEvent *reply,  long handlerRefCon)
{
#pragma unused (messageIn, reply,handlerRefCon)
	
	myPrintf("-----------------------------------------\nopen app\n");
	return noErr;
} /* aeOpenApp */

pascal OSErr aeCloseApp (const AppleEvent *messageIn, AppleEvent *reply,  long handlerRefCon)
{
#pragma unused (messageIn, reply,handlerRefCon)
	
	myPrintf("-----------------------------------------\nclose app\n");
    exit( 0 );
	
	return noErr;
} /* aeOpenApp */


#include "macfileaccess.h"
pascal OSErr aeOpenDocs (const AppleEvent *messageIn, AppleEvent *reply,  long handlerRefCon)
{
#pragma unused (reply,handlerRefCon)
	
	AEDescList              docList;
	AEKeyword               keyword;
	DescType                returnedType;
	FSSpec                  theFSSpec;
	Size                    actualSize;
	long                    itemsInList;
	short                   i;
	OSErr                   result;
	char *p;
	
	myPrintf("-----------------------------------------\nopen doc\n");
	_myBeep();
	//return noErr;
	
	myPrintf("open doc called\n");
	if ((result = AEGetParamDesc(messageIn, keyDirectObject, typeAEList, &docList) )!=0)
		return(result);
	
	if ((result = AECountItems(&docList, &itemsInList))!=0)
		return(result);
	
	for (i = 1; i <= itemsInList; i++)
	{
		if ((result = AEGetNthPtr(&docList, i, typeFSS, &keyword, &returnedType, (Ptr) &theFSSpec, sizeof(FSSpec), &actualSize))!=0)
			return(result);
		
#if 01
		char *n;
		n = getFileNameFromFSSpec( &theFSSpec);
		p = readFile( n, "r" );
		if( p != NULL ){	
			_myPrintf("opening %s\n", p);
			_openInFunc( p );
			free( p );
		}
#else
		p = ioOpenTextFSSpec( theFSSpec );
		if( p )
		{
			_myPrintf("opening %s\n", p);
			
			_openInFunc( p );
			free( p );
			
		}
#endif
		
	}
	result = AEDisposeDesc (&docList);
	return noErr;/* ich MACHE keine Fhler*/
} /* aeOpenDoc */
#endif


