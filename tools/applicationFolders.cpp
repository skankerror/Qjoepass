/*
 *  applicationFolders.cpp
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "jpversion.h"
#include "systemswitch.h"

#include "pathAccess.h"
#include "applicationFolders.h"
#include "fileIO.h"
#include "myPrintf.h"
#include "movie.h"
#include "pathAccess.h"
#include "appnames.h"

#if MAC_CLASSIC_VERSION
#endif



/*
 #include "sysDep.h"
 #include "movie.h"
 
 #ifdef OPEN_GL_VERSION
 #include "editor.h"
 #include "causal_editor.h"
 #endif
 
 #include "prefDef.h"
 #include "parseSiteswap.h"
 #include "fileIO.h"
 #include "pattern.h"
 #include "myPrintf.h"
 */

#ifdef OPEN_GL_VERSION

void    restoreApplicationFlags( void );
void    saveApplicationFlags( void );
bool	editorWindowFlag,
infoWindowFlag,
causalWindowFlag;

void    restoreApplicationFlags( void );
void    saveApplicationFlags( void );
int		siteswapModeFlag;
#endif


char	*proFile = NULL;

/*#define PATH_SIZE 1024
 char applicationDir[PATH_SIZE];
 */


/*
 char *appDir;
 char *sysDir;
 */

char *workPath;
char *profilePath;
char *defprefsPath;
char *prefsPath;
char *languagePath;
char *errPath;
char *historyPath;
char *consolePath;
char *stylePath;
char *editPath;
char *helpPath;
char *j2Path;
char *tipPath;
char *mediaPath;
char *logfilePath;
char *actualTempPath;
char *workspacePath;
char *edit2CdewTempPath;
char *edit2NewCdewTempPath;
char *notation2CdewPath;
char *notation2AnimationPath;
char *tmpNotationPath;
char *cmdLinePath;
char *hotkeyPath;
char *watchfileDownloadPathMac;
char *watchfileDownloadPathWin;
char *watchfileSystemPath;
char *dndfilePath;
char *dndAppPath;
char *soundPath;
char *texturePath;
char *externalTextEditorPath;

#define LOGFILE_NAME                    "logfile.txt"
#define JP_PREFS_NAME                   "jpPrefs.txt"
#define JP_LANGUAGE_NAME                "jpLanguage.txt"
#define JP_DEFAULT_PREFS_NAME           "jpDefaultPrefs.txt"
#define JP_EDIT_NAME                    "edit.pass"
#define DEFAULT_NAME                    "default.pass"
#define WORK_NAME                       "work.pass"
#define CONSOLE_NAME                    "console.pass"
#define STYLES_NAME                     "styles.txt"
#define PROFILE_NAME                    "profile.txt"
#define ERRORS_NAME                     "errors.txt"
#define HISTORY_NAME                    "history.txt"
#define ACTUAL_TEMP_NAME                "actualTemp.pass"
#define EDIT2NEWCDEW_TEMP_NAME          "edit2NewCdewTemp.pass"
#define EDIT2CDEW_TEMP_NAME             "edit2CdewTemp.pass"
#define NOTATION_TO_CDEW_FILE_NAME      "notation2Cdew.pass"
#define NOTATION_TO_ANIMATION_FILE_NAME "notation2Animation.pass"
#define TMP_NOTATION_NAME				"tmpNotationFile.pass"
#define CMD_LINE_NAME					"cmdLine.pass"
#define HOTKEY_FILE						"menuHotkeys.txt"
#define CDEW_WORKSPACE_FILE				"openCdewWindows.txt"
#define WATCHFILE_DOWNLAOD_FILE_MAC		"watchfileMac.txt"
#define WATCHFILE_DOWNLAOD_FILE_WIN     "watchfileWin.txt"
#define WATCHFILE_SYSTEM_FILE			"joe.pass"
#define	SOUND_FOLDER					"snd/"
#define	TEXTURE_FOLDER					"texture/"
#define EXTERNAL_EDITOR_FILE_NAME		"editor.txt"

char defaultPatternName[]               = DEFAULT_NAME;
char workPatternName[]                  = WORK_NAME;
char consolePatternName[]               = CONSOLE_NAME;
char styleFileName[]                    = STYLES_NAME;
char prefFileName[]                     = JP_PREFS_NAME;
char languageFileName[]                 = JP_LANGUAGE_NAME;
char defprefFileName[]                  = JP_DEFAULT_PREFS_NAME;
char editFileName[]                     = JP_EDIT_NAME;
char helpFileName[]                     = HELP_NAME;
char helpJ2FileName[]                   = J2_HELP_NAME;
char tipFileName[]                      = ESC_NAME;
char profilePatternName[]               = PROFILE_NAME;
char errorFileName[]                    = ERRORS_NAME;
char historyFileName[]                  = HISTORY_NAME;
char mediaFolderName[]                  = MEDIA_FOLDER_NAME;
char logfileName[]                      = LOGFILE_NAME;
char actualTempFileName[]               = ACTUAL_TEMP_NAME;
char edit2CdewFileName[]                = EDIT2CDEW_TEMP_NAME;
char edit2NewCdewFileName[]             = EDIT2NEWCDEW_TEMP_NAME;
char notation2CdewFileName[]            = NOTATION_TO_CDEW_FILE_NAME;
char notation2AnimationFileName[]       = NOTATION_TO_ANIMATION_FILE_NAME;
char tmpNotationFileName[]				= TMP_NOTATION_NAME;
char cmdLineFileName[]					= CMD_LINE_NAME;
char hotkeyFileName[]					= HOTKEY_FILE;
char workspaceFileName[]				= CDEW_WORKSPACE_FILE;
char watchfileDownloadFileNameMac[]     = WATCHFILE_DOWNLAOD_FILE_MAC;
char watchfileDownloadFileNameWin[]     = WATCHFILE_DOWNLAOD_FILE_WIN;
char watchfileSystemName[]              = WATCHFILE_SYSTEM_FILE;
char dndfileName[]						= JP__HELPER_FILENAME;
//char dndAppName[]						= JP__HELPER_NAME;
char dndAppName[]						= J2_HELP_NAME;
char soundFolderName[]					= SOUND_FOLDER;
char textureFolderName[]				= TEXTURE_FOLDER;
char externalEditorFileName[]			= EXTERNAL_EDITOR_FILE_NAME;

// get content of file pre.pass
// content of this file is parsed before the actual pattern
char *readProfile( void )
{
	
	if( proFile )
		free( proFile );
	
	proFile = readFile( profilePath, "r");
	return proFile;
}


void generatePathNames( const char *sysDir )
{
	
	// get absolute paths to files
	workPath = makeAbsoluteFilename( sysDir, workPatternName);
	profilePath = makeAbsoluteFilename( sysDir, profilePatternName);
	errPath = makeAbsoluteFilename( sysDir, errorFileName);
	consolePath = makeAbsoluteFilename( sysDir, consolePatternName);
	stylePath = makeAbsoluteFilename( sysDir, styleFileName);
	prefsPath = makeAbsoluteFilename( sysDir, prefFileName);
	languagePath = makeAbsoluteFilename( sysDir, languageFileName);
	defprefsPath = makeAbsoluteFilename( sysDir, defprefFileName);
	historyPath = makeAbsoluteFilename( sysDir, historyFileName);
	editPath = makeAbsoluteFilename( sysDir, editFileName);
	helpPath = makeAbsoluteFilename( sysDir, helpFileName);
	j2Path = makeAbsoluteFilename( sysDir, helpJ2FileName);
	tipPath = makeAbsoluteFilename( sysDir, tipFileName);
	logfilePath = makeAbsoluteFilename( sysDir, logfileName);
	mediaPath = makeAbsolutePath( appDir, mediaFolderName);
	
	actualTempPath = makeAbsoluteFilename( sysDir, actualTempFileName);
	workspacePath = makeAbsoluteFilename( sysDir, workspaceFileName);
	edit2CdewTempPath = makeAbsoluteFilename( sysDir, edit2CdewFileName);
	edit2NewCdewTempPath = makeAbsoluteFilename( sysDir, edit2NewCdewFileName);
	notation2CdewPath = makeAbsoluteFilename( sysDir, notation2CdewFileName);
	notation2AnimationPath = makeAbsoluteFilename( sysDir, notation2AnimationFileName);
	tmpNotationPath = makeAbsoluteFilename( sysDir, tmpNotationFileName);
	cmdLinePath = makeAbsoluteFilename( sysDir, cmdLineFileName);
	hotkeyPath = makeAbsoluteFilename( appDir, hotkeyFileName);
	
	watchfileDownloadPathMac= makeAbsoluteFilename( sysDir, watchfileDownloadFileNameMac );
	watchfileDownloadPathWin= makeAbsoluteFilename( sysDir, watchfileDownloadFileNameWin );
    
    // if the watchfile location is not set, fill in system default
    if( !fexists( watchfileDownloadPathMac )){
        char t[PATH_SIZE];
        sprintf(t, "%s/Downloads/%s", getenv("HOME"), "joe.pass");
        writeFile( watchfileDownloadPathMac, "w", t , strlen( t ));
     }
	watchfileSystemPath= makeAbsoluteFilename( sysDir, watchfileSystemName );
	dndfilePath= makeAbsoluteFilename( sysDir, dndfileName );
	dndAppPath= makeAbsoluteFilename( sysDir, dndAppName );
	
	soundPath= makeAbsoluteFilename( sysDir, soundFolderName );
	texturePath= makeAbsoluteFilename( sysDir, textureFolderName );

	externalTextEditorPath= makeAbsoluteFilename( sysDir, externalEditorFileName);
	
	
	printPath();
}

void printPath( void )
{
	printf("\napplication files and directories:\n");
	printf("application path=: %s\n", appDir);
	printf("sys path=: %s\n", sysDir);
	printf("mediaPath:= %s\n", mediaPath);
	printf("\n");
	
	printf("log file= %s\n", logfilePath);
	printf("actual temp file:= %s\n", actualTempPath);
	printf("edit2Cdew temp file:= %s\n", edit2CdewTempPath);
	printf("edit2NewCdewTempPath:= %s\n", edit2NewCdewTempPath);
	printf("\n");
	
	printf("work file:= %s\n", workPath);
	printf("pro file:= %s\n", profilePath);
	printf("err file:= %s\n", errPath);
	printf("input file:= %s\n", consolePath);
	printf("style file:= %s\n", stylePath);
	printf("pref file:= %s\n", prefsPath);
	printf("language file:= %s\n", languagePath);
	printf("history file:= %s\n", historyPath);
	printf("edit file:= %s\n", editPath);
	printf("help file:= %s\n", helpPath);
	printf("j2 help file:= %s\n", j2Path);
	printf("tip file:= %s\n", tipPath);
	
	printf("\n\n");
}

