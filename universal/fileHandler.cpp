/*
 *  fileHandler.cpp
 *  JoePass
 *
 *  Created by Wolfgang on 25.04.12.
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
#include "systemswitch.h"

#ifdef WINDOWS_VERSION
#include <windows.h>
#include <Process.h>
#endif

#include <string.h>

#include "myPrintf.h"
#include "fileIO.h"
#include "fileHandler.h"

#include "headers.h"
#include "globals.h"

#include "applicationFolders.h"

#include "workspace.h"
#include "runaround.h"
#include "pattern.h"
#include "causal_editor.h"
#include "cdewMenu.h"

#include "editor.h"

#ifdef WINDOWS_VERSION
extern HWND windowsWinHandle;
#endif


void _openInFunc( char *p) {
	if( p == 0 )
		return;

	if( strstr( p, RunaroundInfoString)) {
		openRunaroundProjectFromBuffer( p );
	} else{
		// this is nonsense. we dont want to start a pattern, we want to edit it.
		if(0 && strstr( p, "runaround"))
			startPattern( p );
		else{			
			if( strstr( p, cdewVersionString) ) {
				int id;
				
				//      writeFile( edit2NewCdewTempPath, "w", textEditorContent, strlen(textEditorContent));
				id = makeOglwin( thePatternTitle );
				if( id ) {
					writePatternToOglwin( id, fullPatternPathName, false );
					setOglwinSize( id );
					cdewJugglerColor( 0 );
					oglwin *w;
					w = getOglwinFromID( id  );
					w->transferToAnimation();
				}
				
			} else {
				editorPathValid = true;
				writeToTextEditor( fullPatternPathName );
				if( editWin ) {
					int id;
					
					id = glutGetWindow( );
					glutSetWindow( editWin );
					glutPostRedisplay();
					glutSetWindow( id );
					
				}
			}
		}
	}
	//      free( p );
	
	
}



// ------------------------------
// read and execute watch files
// 1. joe.pass in ./sys/
// 2. joe.pass in download folder
// 3. dnd support:
//		if file dndlocation.txt exists:
//		+ read content (path to original file) 
//		+ open specified file
//		+ delete dndlocation.txt


// ------------------------------
void checkForWatchfiles( void )
{
	// sys folder
    readWatchFile( watchfileSystemPath);
	char t[MAX_FULL_FILE_NAME];
	
#if MAC_OS_X_VERSION
    sprintf(t, "%s", watchfileDownloadPathMac);
#else
    sprintf(t, "%s", watchfileDownloadPathWin);
#endif
    
	char *p;
	// download folder
	p = readFile( t, "r" );
	if( p != NULL ){
		readWatchFile( p);
		free (p );
	}
}



// ------------------------------
// try to read a specified file
// if existing, start animation
// then delete the file
// ------------------------------
bool readWatchFile( char *p)
{
	char *f;
	f = readFile(p, "r");
	if( f != NULL ){
#if 01
        startPattern( f );
#else
		bool t;
		t = jpPrefs.generateCausalFromTextEditorOnStart;
		jpPrefs.generateCausalFromTextEditorOnStart = false;
        _editStartFunc(f);
		jpPrefs.generateCausalFromTextEditorOnStart = t;
#endif
		removeFile(p);
	}
	return f != NULL;
}



void check_dndfile( void )
{
	char *p;
	
	// dndfile
	p = readFile( dndfilePath, "r" );
	
	if( p != NULL ){
		char *q, *t;
		// remove " character from string
		t = p;
		
		// remove first "
		if( (t=strchr(p, '\"') ) != NULL )
			sprintf(t, t+1 );
		// ignore everything from 2nd, including itself
		if( (t=strchr(p, '\"') ) != NULL )
			*t = '\0';
		//		*(p+strlen(p)-1) = '\0';
		q = readFile( p, "r");
		
		if( q ){
			
			// open a workspace
			if(strstr(q, WORKSPACE_MAGIC_TEXT)!= NULL){
				_loadCdewWorkspace( p );
				
			}else{
				
				// should not use the text editor path and name but the system path and name
				saveNameAndPath( p );
				
				// runaround project file
				if( strstr( q, RunaroundInfoString ) ){
					openRunaroundProjectFromBuffer( q );
					goto bail;
					
				}
				if( strstr( q, cdewMagicString ) ){
					writePatternToNewOglwin( p, true );
					cdewAnimate( 0 );
					goto bail;
					
				}
				{
					openFileInTextEditor( p );
	//				_editStartFunc(q);
					
				}
#ifdef WINDOWS_VERSION
				if( windowsWinHandle != NULL){
					SetForegroundWindow(windowsWinHandle);
				}
#endif		
bail:
				;
			}
		}
		
		
		removeFile( dndfilePath ) ;
		free (p );
		free( q );
	}
}

void check_dndfile_old( void )
{
	char *p;
	
	// dndfile
	p = readFile( dndfilePath, "r" );
	
	if( p != NULL ){
		char *q, *t;
		// remove " character from string
		t = p;
		
		// remove first "
		if( (t=strchr(p, '\"') ) != NULL )
			sprintf(t, t+1 );
		// ignore everything from 2nd, including itself
		if( (t=strchr(p, '\"') ) != NULL )
			*t = '\0';
		//		*(p+strlen(p)-1) = '\0';
		q = readFile( p, "r");
		
		if( q ){
			
			if(strstr(q, WORKSPACE_MAGIC_TEXT)!= NULL){
				_loadCdewWorkspace( p );
				
			}else{
				// should not use the text editor path and name but the system path and name
				//			setTextEditorNames( p );
				saveNameAndPath( p );
				
				if( strstr( q, RunaroundInfoString ) ){
					openRunaroundProjectFromBuffer( q );
				}else{
					_editStartFunc(q);
					
				}
#ifdef WINDOWS_VERSION
				if( windowsWinHandle != NULL){
					SetForegroundWindow(windowsWinHandle);
				}
#endif		
			}
		}
		
		
		removeFile( dndfilePath ) ;
		free (p );
		free( q );
	}
}
