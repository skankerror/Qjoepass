/*
 *  applicationFolders.h
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

#if defined(__cplusplus)
extern "C"
{
#endif

#include "mydefs.h"

#define SYS_FOLDER_NAME                 "sys"
#define MEDIA_FOLDER_NAME               "media"
#define ScreenShotFileName              "screenshot"
#define TempFolderName                  "temp"
#define HELP_NAME                       "help.txt"
#define J2_HELP_NAME                    "j2.txt"
#define ESC_NAME                        "esccodes.txt"
extern 	char hotkeyFileName[];
// --------------------- file --------------------------
extern char *proFile;

extern char *workPath;
extern char *prefsPath;
extern char *languagePath;
extern char *defprefsPath;
extern char *profilePath;
extern char *errPath;
extern char *stylePath;
extern char *historyPath;
extern char *editPath;
extern char *helpPath;
extern char *j2Path;
extern char *tipPath;
extern char *mediaPath;
extern char *notation2CdewPath;
extern char *notation2AnimationPath;
extern char *logfilePath;
extern char *actualTempPath;
extern char *edit2CdewTempPath;
extern char *edit2NewCdewTempPath;
extern char *tmpNotationPath;
extern char *cmdLinePath;
extern char *hotkeyPath;
extern char *workspacePath;
extern char workspaceFileName[];
extern char *watchfileDownloadPathMac;
extern char *watchfileDownloadPathWin;
extern char *watchfileSystemPath;
extern char *dndfilePath;
extern char *dndAppPath;
extern char *soundPath;
extern char *texturePath;
	
extern char *externalTextEditorPath;


char *getAppFolder( const char *argv0 );
const char *getSysDirName( void );
const char *getAppDirName( void );


void restorePath( void );

void generatePathNames( const char *sysDir );
char *readProfile( void );

void printPath( void );

int createFolder( char *name);
void removeTempFolder( void );

#if defined(__cplusplus)
}
#endif
