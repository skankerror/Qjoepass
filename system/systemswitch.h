/*
 *  systemswitch.h
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

#ifndef __SYS_DEP__
#define __SYS_DEP__

//#define MAC_OS_X_VERSION 1

//************************
// Select operating system
//************************
#if 0
        //************************
        // UNIX
        //************************
        // define in makefile later
                #define UNIX_VERSION	1
#else
        //************************
        // WINDOWS
        //************************
        // defined in VisualC settings
        #ifdef WIN32
                #define WINDOWS_VERSION 1

// no #endif here as quicktime for windows sdk also makes the following defines
        #else
                //************************
                // Mac OS X
                //************************
                // defined by UniversalHeaders
                #if __APPLE__
					#define MAC_OS_X_VERSION 1
					#define MAC_OS_X_VERSION_WINDOW_SWITCH 1
                
                //************************
                // Mac OS 9 / X 
                // Carbon
                // Code Warrior
                //************************
                // defined by UniversalHeaders
                #else 
                	#ifdef TARGET_API_MAC_CARBON
                        #define MAC_CLASSIC_VERSION 1
                	#endif
        		#endif
		#endif

#endif

#ifndef UNIX_VERSION
 #ifndef WINDOWS_VERSION
  #ifndef MAC_OS_X_VERSION
   #ifndef MAC_CLASSIC_VERSION

	#define UNDEFINED_VERSION 1

   #endif
  #endif
 #endif
#endif

// defined, if system dependent functions are implemented

#define USE_ABS_PATH 01

#ifndef MAC_VERSION
 #define OPEN_GL_VERSION
#endif



#ifdef MAC_OS_X_VERSION
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#define SdSelectFile            true    // _very_ useful
#define SdPlaySound                     false   // nice
#define SdMakeMovie                     true    // nice
#define SYSDEP_VERSION
#define MAKE_QUICKTIME_MOVIE	
#define PATH_DIVIDER            '/'
//#define MACINTOSH_VERSION     1
#endif



#ifdef MAC_CLASSIC_VERSION
#include <unistd.h>                                     // std directory access
#include <stat.h>
#define SdSelectFile            true    // _very_ useful
#define SdPlaySound                     true    // nice
#define SdMakeMovie                     true    // nice
#define MAKE_QUICKTIME_MOVIE	
#define PATH_DIVIDER            ':'
#define MACINTOSH_VERSION       1

#define SYSDEP_VERSION
#endif

#ifdef WINDOWS_VERSION
#include <Direct.h>                         // std directory access
#define SdSelectFile            true    // _very_ useful
#define SdPlaySound                     false   // nice
#define SdMakeMovie                     true    // nice
#define PATH_DIVIDER            '\\'
#define SYSDEP_VERSION
#endif

#ifdef UNIX_VERSION
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#define PATH_DIVIDER '/'
#define SdSelectFile            true    // _very_ useful
#define SdPlaySound                     false   // nice
#define SdMakeMovie                     true    // nice
#define SYSDEP_VERSION
#endif


// default settings

#ifndef SYSDEP_VERSION
#include <unistd.h>                                     // std directory access
//#define SdSelectFile          true    // _very_ useful
//#define SdPlaySound       true        // nice
#define SdMakeMovie                     true    // nice
//#define MAKE_QUICKTIME_MOVIE
#define PATH_DIVIDER            ':'
#define NO_SYSTEM_VERSION 1
#endif

#endif


