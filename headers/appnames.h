/*
 *  appnames.h
 *  JoePass
 *
 *  Created by Wolfgang on 22.09.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __APP_NAMES__
#define __APP_NAMES__



#include "systemswitch.h"


#if MAC_OS_X_VERSION
	#define JP_NAME			"JoePass.app"
	#define JP__HELPER_NAME	"JoePass-Helper.app"
#elif WINDOWS_VERSION	
	#define	JP_NAME			"JoePass.exe"
	#define JP__HELPER_NAME	"prog.cmd"
#endif

#define	JP__HELPER_FILENAME	"dndlocation.txt"

#endif