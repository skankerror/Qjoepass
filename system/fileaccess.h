// --------------------- selectFile --------------------------
/*  place system dependent io routines here
 three routines are needed by JoePass:
 
 void initIO( void )                                                             {                                       }
 do whatever you need to set up the io-system
 
 char *selectAndLoadFile( void )                         {       return NULL;    }
 call a file selector and
 return a buffer with the content of the selected file or NULL
 
 wwwBoolean saveTextFileAs (char *buf, char *name, char *infoString)       {       return false;   }
 call a files selector with default name set to "name"
 save content of buffer in selected file,
 or do nothing if user hits cancel
 return true if file was saved, false otherwise
 
 
 create a file that contains these routines
 and include it here
 dont forget to set a precompiler switch to activate your additions
 (see dysDep.h)
 
 wolfgang westerboer
 westwolf@netcologne.de
 
 */
#ifndef __FILE_ACCESS__
#define __FILE_ACCESS__

#include "systemswitch.h"
#ifdef MAC_OS_X_VERSION
#include <sys/syslimits.h>
#define MAX_PATTERN_TITLE 	FILENAME_MAX
#define DIR_SIZE			PATH_MAX
#define MAX_FULL_FILE_NAME	(PATH_MAX + FILENAME_MAX)
#else 
#ifdef WINDOWS_VERSION
#include <stdlib.h>
#define MAX_PATTERN_TITLE 	_MAX_FNAME
#define DIR_SIZE			_MAX_PATH
#define MAX_FULL_FILE_NAME	(_MAX_PATH + _MAX_FNAME)
#endif
#endif
// rajout
#include <stdlib.h>
#include <linux/limits.h>
#define MAX_PATTERN_TITLE 	FILENAME_MAX
#define DIR_SIZE			PATH_MAX
#define MAX_FULL_FILE_NAME	(PATH_MAX + FILENAME_MAX)
// fin rajout
#include "mydefs.h"

#if defined(__cplusplus)
extern "C"
{
#endif
#include "mydefs.h"
#include "systemswitch.h"
	
	//extern char             fileSelectorPathAndName[];
	
	
	
	extern	char thePatternTitle[];
	extern	char fullPatternPathName[];
	extern	char fullPatternPath[];
	extern	char fileSelectorPathAndName[];
	
	
	/*      routines called by JoePass      */
	void                    initIO( void );
	char					*selectFolder( char *infoString );
	char                    *selectAndLoadFile( char *infoString );
	bool					saveTextFileAs (char *buf, char *name, char *infoString);
	char					*getFileSelectorPathAndName( void );
	void					splitPathName( char *in, char *path, char *name, char *pathname);
	void					saveNameAndPath( char *pn);
	
	
	/*
	 define this in your code:
	 
	 fio and fis will
	 + call the file selector,
	 + save complete path into fileSelectorPathAndName
	 + return 0 if user hit cancel, NOT 0 otherwise
	 */
	
	void                    _initIO( void );        // initialize file selector
	bool					fid( char *infoString );            // call file selector to select a folder
	bool					fio( char *infoString, char *fileTypes );            // call file selector to open file
	bool					fis( char *infoString, char *name  );      // call file selector to save file
	
	
	
	
#if defined(__cplusplus)
}
#endif

#endif
