/*
 *  quicktime.h
 *  joe
 *
 *  created by Wolfgang Westerboer on 19.07.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemswitch.h"

#if MAC_OS_X_VERSION	


#ifndef __MAC_QUICKTIME__
#define __MAC_QUICKTIME__


#if defined(__cplusplus)
extern "C"
{
#endif
	

	
extern bool		qtInitialized;
extern bool		qtInstalled;

void initQuicktime( void );
void exitQuicktime( void );	
bool IsQuickTimeInstalled( void );


#if defined(__cplusplus)
}
#endif

#endif

#endif
