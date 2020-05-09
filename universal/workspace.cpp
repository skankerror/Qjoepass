
/*
 *  workspace.cpp
 *  joe2
 *
 *  created by Wolfgang Westerboer on 14.09.08.
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
#include <iostream>

#include <stdio.h>
#include <string.h>

#include "globals.h"

#include "workspace.h"
#include "pathAccess.h"

#include "causal_editor.h"
#include "applicationFolders.h"
#include "editor.h"
#include "fileIO.h"
#include "fileaccess.h"
#include "myPrintf.h"
#include "jpPrefs.h"
#include "runaround.h"

#include "myPrintf.h"

#if MAC_OS_X_VERSION
#include <unistd.h>
#endif

#if WINDOWS_VERSION
#include <Process.h>
#endif

char *makeCmdLineArg( char *in);
bool useCdewWorkspace = true;
bool workspaceSavedFlag = false;

char *makeRelDataPath( char *appPath, char *dataPath );
char *makeAbsDataPath( char *appPath, char *dataPath );

#define APP_DIR		"ApplicationDir = "
// base dir is the application dir

// rel to abs path 
// if path is absolute (contains : on win, starts with / on mac) 
// => we´re done
// else 
//	if path starts with ../
//	=> for every< ../ move one dir up in application dir
//     remove ../ from rel dir
//  => connect the application dir and the data dir to an absolute dir



// uses relative path names. 
// makes it possible to have the same workspace on a stick, on mac or win
// tries to read a relative path.
// if not possible, tries to read an absolute path 
#define USE_REL_PATH	true
void setWorkspaceSavedFlag( bool value){
	workspaceSavedFlag = value;
}

bool getWorkspaceSavedFlag( void ){
	return workspaceSavedFlag;
}

void loadDefaultWorkspace( void )
{
	char defWsPath[PATH_SIZE];
	sprintf(defWsPath, "%s%s", workspacePath, workspaceFileName);
	
	_loadCdewWorkspace( workspacePath);
	
}



/*
 char *u;
 u = strrchr( t, PATH_DIVIDER);
 if( u )
 *(u+1) = '\0';
 
 */

void _loadCdewWorkspace( char *in){
	
	char n[PATH_SIZE], w[PATH_SIZE];
	char *np, *p, *pout, *b, *t;
	char cur_path[PATH_SIZE], lastAppDir[PATH_SIZE];
	float wx,wy,ww,wh;
	float xf = 1., yf = 1.;	
	char c;
	char *u;
	char wsp[PATH_SIZE];
	
	sprintf(wsp, "%s", in);
	
	b = readFile(wsp, "r");
	if( b == NULL)
		return;
	
	if( *b == '\0'){
		free( b );
		return;
	}
	
	p = b;
	
	// close all windows
	oglwin *w2, *w1 = firstCdewWindow;
	while( w1 ){
		w2 = w1->next;
		w1->closeWin();
		w1 = w2;
	}
	closeTextEditor();
	_closeRunaround();
	
	xf = glutGet( GLUT_SCREEN_WIDTH);
	yf = glutGet( GLUT_SCREEN_HEIGHT);
	
	
	
	// get workspace directory
	u = strrchr( wsp, PATH_DIVIDER);
	if( u )
		*(u+1) = '\0';
	
	
	// copy the last app dir:
	// first line in file contains the directory where the app was located on last run.
	// we need it if we can´t open a file:
	// Maybe the user copied the app to a new location.
	// this way we may be lucky and are able to open the file by searching relatively to the old application path. 
	t = strstr(b, APP_DIR);
	if(0 && t ){
		t = strpbrk(t, "\n\r");
		if( t ){
			c = *p; 
			*t = '\0';
			sprintf(lastAppDir, "%s", p);
			*t = c;
		}
	}else {
		sprintf(lastAppDir, "%s", applicationDir);
		//		sprintf(lastAppDir, "%s", workspacePath);
	}
	
	getcwd(cur_path, PATH_SIZE);
	chdir(wsp);
	
	if( animationWin > 0 ){
		// get animationWindow size and position, if aviable
		if( (p = strstr( b, ANIMATION_WORKSPACE_WINPOS_ENTRY)) != NULL ){
			// get pointer to keyword
			p += strlen( ANIMATION_WORKSPACE_WINPOS_ENTRY );
			sscanf(p, "%f %f %f %f", &wx, &wy, &ww, &wh);
			// simple test if we use rel sizes
			if( wx <= 1){
				wx *= xf;
				wy *= yf;
				ww *= xf;
				wh *= yf;
			}
			glutSetWindow(animationWin);
			glutPositionWindow( wx, wy);
			glutReshapeWindow( ww, wh);
		}
	}
	
	// try to load runaround project
	// this might create an extra cdew window that we want to try to open later
#if USE_REL_PATH
	if( (p = strstr( b, RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY2)) != NULL ){
		char *s;
		// get pointer to keyword
		p += strlen( RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY2 );
#else
	if( (p = strstr( b, RUNAROUNDEDITOR_WORKSPACE_FILENAME_ENTRY)) != NULL ){
		char *s;
		// get pointer to keyword
		p += strlen( RUNAROUNDEDITOR_WORKSPACE_FILENAME_ENTRY );
		
#endif
		s = p;
		// extract path from textline
		while(*s && *s !='\n' && *s != '\r')		
			s++;
		strncpy(n, p, s-p);
		n[s-p] = '\0';
		
		if( runaroundProjectName )
			free( runaroundProjectName );
		if( *n == '\0' ){
			sprintf(n,"%s",StdRunaroundProjectTitle);
		}
		runaroundProjectName = (char *)malloc(strlen(n)+1);
		sprintf(runaroundProjectName,"%s",n);

	}

#if USE_REL_PATH
	if( (p = strstr( b, RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY)) != NULL ){
		char *s;
		// get pointer to keyword
		p += strlen( RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY );
#else
		if( (p = strstr( b, RUNAROUNDEDITOR_WORKSPACE_FILENAME_ENTRY)) != NULL ){
			char *s;
			// get pointer to keyword
			p += strlen( RUNAROUNDEDITOR_WORKSPACE_FILENAME_ENTRY );
			
#endif
			s = p;
			// extract path from textline
			while(*s && *s !='\n' && *s != '\r')		
				s++;
			strncpy(n, p, s-p);
			n[s-p] = '\0';
			
			if( *n ){
				char p[PATH_SIZE];
				pout = makeAbsDataPath(wsp, n);
				if( !fexists(pout) )
					pout = makeAbsDataPath(lastAppDir, n);
				//				printf("pout = %s\n", pout);
//				sprintf(runaroundProjectName, "%s", StdRunaroundProjectTitle);
				sprintf(p, "%s%s", pout, runaroundProjectName);
				saveNameAndPath( p );
				openRunaroundProjectFromDirectory( pout );
				
			}
		}
		
		
		// try to load text editor pattern
		// this might create an extra cdew window that we want to try to open later
#if USE_REL_PATH
		if( (p = strstr( b, TEXTEDITOR_WORKSPACE_REL_FILENAME_ENTRY)) != NULL ){
			char *s;
			// get pointer to keyword
			p += strlen( TEXTEDITOR_WORKSPACE_REL_FILENAME_ENTRY );
#else
			if( (p = strstr( b, TEXTEDITOR_WORKSPACE_FILENAME_ENTRY)) != NULL ){
				char *s;
				// get pointer to keyword
				p += strlen( TEXTEDITOR_WORKSPACE_FILENAME_ENTRY );
				
#endif
				s = p;
				// extract path from textline
				while(*s && *s !='\n' && *s != '\r')		
					s++;
				strncpy(n, p, s-p);
				n[s-p] = '\0';
				
				if( n && *n ){
					pout = makeAbsDataPath(wsp, n);
					if( !fexists(pout) )
						pout = makeAbsDataPath(lastAppDir, n);
					//			printf("pout = %s\n", pout);
					saveNameAndPath( pout );
					openFileInInternalAndExternalEditor( pout );
					
					// get window size & position
					if( (p = strstr( p, TEXTEDITOR_WORKSPACE_WINPOS_ENTRY)) != NULL ){
						p += strlen( TEXTEDITOR_WORKSPACE_WINPOS_ENTRY );
						s = p;
						while(*s && *s !='\n' && *s != '\r')		
							s++;
						strncpy(w, p, s-p);
						w[s-p] = '\0';
						sscanf(w, "%f %f %f %f", &wx, &wy, &ww, &wh);
						
						// simple test if we use rel sizes
						if( wx <= 1){
							wx *= xf;
							wy *= yf;
							ww *= xf;
							wh *= yf;
						}
						glutSetWindow(editWin);
						glutPositionWindow( wx, wy);
						glutReshapeWindow( ww, wh);
					}
				}
			}
			
			p = b;
			while(p && *p ){
				char *s;
				
				
#if USE_REL_PATH
				// get filename
				if( (p = strstr( p, CDEW_WORKSPACE_REL_FILENAME_ENTRY)) ){
					np = p += strlen( CDEW_WORKSPACE_REL_FILENAME_ENTRY );
				}else
#else
				{
					// get filename
					if( (p = strstr( p, CDEW_WORKSPACE_FILENAME_ENTRY)) == NULL )
						continue;
					np = p += strlen( CDEW_WORKSPACE_FILENAME_ENTRY );
				}
#endif		
				if( p == NULL )
					continue;
				
				s = p;
				while(*s && *s !='\n' && *s != '\r')		
					s++;
				strncpy(n, p, s-p);
				n[s-p] = '\0';
				
				// get window size & position
				if( (p = strstr( p, CDEW_WORKSPACE_WINPOS_ENTRY)) == NULL )
					continue;
				
				p += strlen( CDEW_WORKSPACE_WINPOS_ENTRY );
				s = p;
				while(*s && *s !='\n' && *s != '\r')		
					s++;
				strncpy(w, p, s-p);
				w[s-p] = '\0';
				sscanf(w, "%f %f %f %f", &wx, &wy, &ww, &wh);
				
				// simple test if we use rel sizes
				if( wx <= 1){
					wx *= xf;
					wy *= yf;
					ww *= xf;
					wh *= yf;
				}
				// read terminator to find next entry
				p = strstr( p, CDEW_WORKSPACE_END_ENTRY);
				
				printf("workspace file %s \n", n);
				//	if asked, read a pattern more than once:
				if( strstr(b, n) < np ){
#if 0
					printf("read error\n");
					continue;
#else
					printf("double entry\n");
#endif			
				}
				
				pout = makeAbsDataPath(wsp, n);
				if( !fexists(pout) )
					pout = makeAbsDataPath(lastAppDir, n);
				
				//		printf("pout = %s\n", pout);
				
				// don`t load files from my /sys dir
				// and then: why not?
#if 0
				char sd_rel_name[100];
				sprintf(sd_rel_name, "%s%c", SYS_FOLDER_NAME, PATH_DIVIDER);
				if( strstr(pout, sd_rel_name) )
					continue;
#endif		
				
				// load file into causal editor
				char *p = readFile( pout, "r" );
				
				// ignore deleted or moved files
				if( p ){
					char *n1 = strrchr( n, PATH_DIVIDER );
					char name[2000];
					if( n1 )
						sprintf( name, "%s", n1+1);
					else
						sprintf(name, n);
					
					
					// load file into causal editor
					setDefaultCdewWindowSize( wx ,wy , ww, wh);
					oglwin *o = new oglwin( name );
					setDefaultCdewWindowSize( -1,-1,-1,-1);
					o->_readDiagram( p );
					o->cdewSplitPathName( pout, o->cdewPathName, o->cdewFileName, o->cdewPathAndFileName );
					free( p );
					printf("=> ok\n", n);
				}else
					printf("not found\n", n);
			}
			
			if( b )
				free( b );
			
			chdir(cur_path);
			
		}
		
		
		
		void saveCdewWorkspace( void )
		{	
			workspaceSavedFlag = false;
			_saveCdewWorkspace( sysDir, workspacePath );
		}
		
		
		void _saveCdewWorkspace( char *sdPath, char *wsPath) {
			bool addToSaveList;
			FILE *fp1;
			oglwin *p;
			float x,y,w,h;	
			float xf = 1., yf = 1.;
			
			if( workspaceSavedFlag )
				return;
			else
				workspaceSavedFlag = true;
			
			// scale window size to 0...1
			xf = 1.f / glutGet( GLUT_SCREEN_WIDTH);
			yf = 1.f / glutGet( GLUT_SCREEN_HEIGHT);
			
			
			/*
			 write open pattern list 
			 */
			
			fp1 = fopen(wsPath, "w");
			if( fp1 ){
				char *relDataPath;
				
				fprintf(fp1, "%s\n", WORKSPACE_MAGIC_TEXT );
				fprintf(fp1, "%s\n\n\n", sdPath);
				
				int oldID = glutGetWindow();
				
				// animation window sizes;
				glutSetWindow( animationWin );
				x = glutGet( GLUT_WINDOW_X);
				y = glutGet( GLUT_WINDOW_Y);
				w = glutGet( GLUT_WINDOW_WIDTH);
				h = glutGet( GLUT_WINDOW_HEIGHT);
				x *= xf;
				y *= yf;
				w *= xf;
				h *= yf;
				fprintf(fp1, "%s%f %f %f %f\n", ANIMATION_WORKSPACE_WINPOS_ENTRY, x, y, w, h);
				fprintf(fp1, "%s\n", CDEW_WORKSPACE_END_ENTRY);
				
				
				// causal patterns
				p = firstCdewWindow;;
				while( p ){
					
					if(p->cdewPathAndFileName != NULL && *p->cdewPathAndFileName){
						addToSaveList = strstr(p->cdewPathAndFileName, "causalTemplate.pass") == NULL;// || p->fileChangedFlag || p->autoSaveFlag;
						if( addToSaveList ){
							glutSetWindow( p->id );
							x = glutGet( GLUT_WINDOW_X);
							y = glutGet( GLUT_WINDOW_Y);
							w = glutGet( GLUT_WINDOW_WIDTH);
							h = glutGet( GLUT_WINDOW_HEIGHT);
							x *= xf;
							y *= yf;
							w *= xf;
							h *= yf;
							
							
							relDataPath = makeRelDataPath( sdPath, p->cdewPathAndFileName);
							
							fprintf(fp1, "%s%s\n", CDEW_WORKSPACE_REL_FILENAME_ENTRY, relDataPath);
							fprintf(fp1, "%s%s\n", CDEW_WORKSPACE_FILENAME_ENTRY, p->cdewPathAndFileName);
							fprintf(fp1, "%s%f %f %f %f\n", CDEW_WORKSPACE_WINPOS_ENTRY, x, y, w, h);
							fprintf(fp1, "%s\n", CDEW_WORKSPACE_END_ENTRY);
						}
					}
					p = p->next;
				}
				glutSetWindow( oldID);
				
				// text editor
				if( editWin ){
					glutSetWindow( editWin );
					x = glutGet( GLUT_WINDOW_X);
					y = glutGet( GLUT_WINDOW_Y);
					w = glutGet( GLUT_WINDOW_WIDTH);
					h = glutGet( GLUT_WINDOW_HEIGHT);
					x *= xf;
					y *= yf;
					w *= xf;
					h *= yf;
					
					relDataPath = makeRelDataPath( sdPath, textEditorFullPathAndName);
					fprintf(fp1, "%s%s\n", TEXTEDITOR_WORKSPACE_REL_FILENAME_ENTRY, relDataPath);
					fprintf(fp1, "%s%s\n", TEXTEDITOR_WORKSPACE_FILENAME_ENTRY, relDataPath);
					fprintf(fp1, "%s%f %f %f %f\n", TEXTEDITOR_WORKSPACE_WINPOS_ENTRY, x, y, w, h);
					fprintf(fp1, "%s\n", CDEW_WORKSPACE_END_ENTRY);
				}
				
				
				// runaround editor
				if( runaroundProjectPath[0] != '\0' && runaroundEditorHasOpenWindows()){
					relDataPath = makeRelDataPath( sdPath, runaroundProjectPath);
					fprintf(fp1, "%s%s\n", RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY,relDataPath );
					fprintf(fp1, "%s%s\n", RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY2,runaroundProjectName );
				}
				
				
				fclose( fp1);
			}
		}
		
		
		void _saveWorkspaceAs( char *n ) {
			char workspaceFilename[MAX_PATTERN_TITLE];
			char workspacePath[MAX_PATTERN_TITLE];
			char patternTitle[MAX_PATTERN_TITLE];
			char newFilename[MAX_PATTERN_TITLE];
			char *p, *t;
			
			sprintf(workspaceFilename, "%s", n );
			myPrintf("selected workspace file: %s\n", fileSelectorPathAndName);	
			
			// path to workspace folder. 
			// save all files here
			sprintf( workspaceFilename, "%s", fileSelectorPathAndName);
			sprintf( workspacePath, "%s", fileSelectorPathAndName);
			p = strrchr(workspacePath, PATH_DIVIDER);
			if( p )
				*(p+1) = '\0';
			else 
				sprintf(p, "%s%c", workspacePath, PATH_DIVIDER);
			
			
			
			// collect open files to selected folder, set new file path
			oglwin *w = firstCdewWindow;
			while( w ){
				t = w->cdewPathAndFileName;
				if( t != NULL && *t){
					p = strrchr( t, PATH_DIVIDER);
					if( p )
						sprintf(patternTitle, p+1 );
					else 
						sprintf(patternTitle, t);
					sprintf(newFilename, "%s%s", workspacePath, patternTitle);
					myPrintf("new cdew filename: %s\n", newFilename);	
					w->saveDiagramWithName(newFilename);
					w->setSaveFullFileName(newFilename);
				}
				w = w->next;
			}
			
			if( textEditorFullPathAndName ){
				t = textEditorFullPathAndName;
				
				if( t != NULL && *t){
					p = strrchr( t, PATH_DIVIDER);
					if( p )
						sprintf(patternTitle, p+1 );
					else 
						sprintf(patternTitle, t);
					
					sprintf(newFilename, "%s%s", workspacePath, patternTitle);
					saveFile( textEditorContent, newFilename);
					setTextEditorNames( newFilename);
				}			
			}
			
			// save workspace file
			_saveCdewWorkspace( workspacePath, workspaceFilename );
			
		}
		
		
		/*
		 gleiche Verzeichnisse am Anfang verwerfen in Datei- und Programmpfad
		 relAppPfad = AppPfad mit .. statt verbleibende Verzeichnisse 
		 relDataPfad = DataPfad ohne gleiche Verzeichnisse
		 relPfad = relAppPFad + relDataPfad
		 */
		char *makeRelDataPath( char *appPath, char *dataPath )
		{
			static char out[10000];
			char *a, *a2, *d, *d2;
			
			// path is already relative
			// not saved since we read it from the workspace file 
#ifdef MAC_OS_X_VERSION
			if( *dataPath != '/' )
				return dataPath;
#endif 
#ifdef WINDOWS_VERSION
			if( strstr(dataPath, ":") == NULL)
				return dataPath;	
#endif
			
			
			
			//gleiche Verzeichnisse am Anfang verwerfen in Datei- und Programmpfad
			a = a2 = appPath;
			d = d2 = dataPath;
			while(*a && *d ){
				
				// gleiche Zeichen
				if( *a == *d){
					a++;
					d++;
					// wenn trenner erreicht: teilpfad wird nicht gebraucht
					if( *a == PATH_DIVIDER ){
						a2 = ++a;
						d2 = ++d;
					}
					// zeichen ungleich:	
					// letzter teilpfad ist der gesuchte
					// aber: das trennzeichen überspringen
				}else {
					if( appPath != a2)
						if( *a2 == PATH_DIVIDER)
							a2++;
					if( dataPath != d2)	
						if( *d2 == PATH_DIVIDER)
							d2++;
					break;
				}
			}
			
			// different volumes
			if( dataPath == d2 )
				sprintf(out, "%s", dataPath );
			else {
				sprintf(out, "");
				// a2 enthält die Ordner, um die wir nach oben müssen.
				// für jeden gefunden PATH_DIVIDER => ..PATH_DIVIDER einfügen
				while(*a2 ){
					if( *a2 == PATH_DIVIDER )
						sprintf(out, "%s%s%c", out, "..", PATH_DIVIDER);
					a2++;
				}
				sprintf(out, "%s%s", out, d2);
			}
			
			
			
			
			return out;
		}
		
		// rel to abs path 
		// if path is absolute (contains : on win, starts with / on mac) 
		// => we´re done
		// else 
		//	if path starts with ../
		//	=> for every< ../ move one dir up in application dir
		//     remove ../ from rel dir
		//  => connect the application dir and the data dir to an absolute dir
		// connect appPath and dataPath just the way they are. 
		// Operating system does the rest 
		// ok on mac
		
		char *makeAbsDataPath( char *appPath, char *dataPath ){
			static char out[10000];
			
			// path is already absolute
			// not saved since we read it from the workspace file 
#ifdef MAC_OS_X_VERSION
			if( *dataPath == '/' )
				return dataPath;
#endif 
#ifdef WINDOWS_VERSION
			if( strstr(dataPath, ":") != NULL)
				return dataPath;	
#endif
			
			
			sprintf( out, "%s%s", appPath, dataPath);
			
			/*	alle verzeichnistrenner an system anpassen:
			 zur Zeit / und 
			 */
			char *p = out;
			
			while( p && *p ){
				p = strpbrk( p, "/\\");
				if( p )
					*p++ = PATH_DIVIDER;
			}
			
			return out;
		}
		
		
		// open a text file in joes text editor and in systems default text editor
		// nb: choose one that automatically adds changes to it´s own content
		void openFileInInternalAndExternalEditor( char *pathname)
		{
			char *p;
			
			// init the editor
			createTextEditor();
			resetTextEditor();
			
			// read file into text editor
			if( (p = _readTextEditorPattern(pathname )) != NULL){
				textEditorContent = p;
				
				// save name and path
				setTextEditorNames(pathname );
				setEditorWindowName();
				
				openFileInExternalEditor(pathname);
				
			}
			
		}
		
		void selectExternalEditor( int dummy );
		void openFileInExternalEditor( char *pathname )
		{
			int r;
			
#if MAC_OS_X_VERSION
			// force system to open file in default text editor (option -t)
#define MacCmd "/usr/bin/open -t" 
			char *cmdLine  = (char *)malloc( strlen(MacCmd)+strlen(pathname)+3);
			
			sprintf(cmdLine, "%s \"%s\"", MacCmd, pathname);
			r = system(cmdLine);
			free( cmdLine);
#endif
#if WINDOWS_VERSION
			char *cl  = readFile(externalTextEditorPath, "r");
			if( cl == 0 ){
				selectExternalEditor( 0 );
				cl  = readFile(externalTextEditorPath, "r");
				if( cl == 0)
					return;
			}
			char *pn  = makeCmdLineArg( pathname);
			char *me  = makeCmdLineArg(appDir);
			char *cl2 = makeCmdLineArg(cl);
			
			r = spawnl( _P_NOWAIT, cl2, me, pn, NULL);//"\"C:\\Dokumente und\ Einstellungen\\Administrator\\Desktop\\ww.pass\"", NULL);
			printf("%s, %s, %s => %d\n", cl2, me, pn, r );
			
			free( cl2);
			free( me );
			free( pn );
			free( cl);
#endif
			printf("ret = %d\n", r);
		}
		
		char *makeCmdLineArg( char *in)
		{
			
			char *out = (char *)malloc( strlen( in) + 3);
			if(  strchr( in, ' ') )
				sprintf(out, "\"%s\"", in);
			else
				sprintf(out, "%s", in);
			
			return out;
		}
		
