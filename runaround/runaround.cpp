/*
 *  runaround.cpp
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
#include <assert.h>

#include "globals.h"
#include "systemswitch.h"

#include "runaround.h"
#include "runaroundMenu.h"
#include "inputWindow.h"
#include "listWindow.h"
#include "fileIO.h"
#include "fileaccess.h"
#include "pathAccess.h"
#include "parseSiteswap.h"
#include "prefDef.h"
#include "causal_editor.h"
#include "editor.h"

#include "pattern.h"

//#include "dialog.h"

#include "myPrintf.h"
//#include "Dialog.h"
#include "languageSupport.h"

#include "exitModules.h"
#if 0
#define DEBUG_RUNAROUND_READING
#endif

#define RUNAROUND_TEMP_FILE "runaroundTempFile"

int						 runaroundJugglers = 0;

char					*getPathFromFullName( char *fullPatternPathName);
char                    *getNameFromFullName( char *fullPatternPathName);
bool					 askForRunaroundProjectName( void );
void				     printRunaroundContent( void );

bool					runaroundContentHasChanged;
bool					runaroundEditorHasOpenWindows(void );

char                     fullSubPatternName[DIR_SIZE];

float					 runaroundDelayWalk = 0.f;

//const char			*homeDir;
const char              *systemDir;


char                     runaroundDefaultPattern[DIR_SIZE];
char                     runaroundProjectPath[DIR_SIZE] = "";


int						defaultThrowForRunaroundPatterns = 3;
int						animatedRunaroundSceneIndex;


#define                  StdRunaroundCompletePatternTitle   "runaround_complete.pass"
#define                  StdRunaroundDefaultPatternTitle	"runaround_default_pattern.pass"
#define                  StdRunaroundMovementOnlyTitle		"movement_only.pass"

wwwBoolean				 askForRunaroundProjectName( void );

char					*runaroundProjectName				= NULL;
char                    *runaroundStartPaternName			= NULL;
char                    *stdRunaroundSubPatternName			= NULL;
char                    *stdRunaroundCompletePatternName	= NULL;
char                    *stdRunaroundMovementOnlyName		= NULL;

#define					 RunaroundJugglers				"runaroundJugglers"
#define					 RunaroundEntries				"runaroundEntries"
#define					 RunaroundNewLine				"runaroundNewLine"
#define					 RunaroundCorners				"runaroundCorners"
#define					 RunaroundRotationOfFrame		"runaroundRotationOfFrame"
#define					 RunaroundMetersOnScreen		"runaroundMetersOnScreen"
#define					 RunaroundPattern				"runaroundPattern"
#define					 RunaroundPos					"runaroundPos"
#define					 RunaroundPoi					"runaroundPoi"
#define					 RunaroundPoiIn					"runaroundPoiIn"
#define					 RunaroundPoiOut				"runaroundPoiOut"
#define					 RunaroundIn					"runaroundIn"
#define					 RunaroundOut					"runaroundOut"
#define					 RunaroundDrawPos				"runaroundDrawPath"
#define					 RunaroundDrawPoi				"runaroundDrawPoi"
#define					 RunaroundSelectionIndex		"RunaroundSelectionIndex"
#define					 RunaroundShowIndex				"RunaroundShowIndex"
#define					 RunaroundListWindow			"RunaroundListWindow"
#define					 RunaroundInputWindow			"RunaroundInputWindow"

// obsolete
#define					 sizeString						"cdewsize"


char                    *readNewLine( char *buf, _fieldContent *l, int maxE);
char                    *readPatternName( char *buf, _fieldContent *l, int maxE);
char                    *readPos( char *buf, _fieldContent *l, int maxE, int maxJ);
char                    *readPoi( char *buf, _fieldContent *l, int maxE, int maxJ);
char                    *readPoiIn( char *buf, _fieldContent *l, int maxE, int maxJ);
char                    *readPoiOut( char *buf, _fieldContent *l, int maxE, int maxJ);
char                    *readPosIn( char *buf, _fieldContent *l, int maxE, int maxJ);
char                    *readPosOut( char *buf, _fieldContent *l, int maxE, int maxJ);



void					 writeRunaroundPattern( _fieldContent *p, int count );
void					 _writeRunaroundPattern( char *dir, char *name, _fieldContent *p, int count, bool includeMode );

void					 _saveRunaroundProject( char *dir, char *name );
bool					 _openRunaroundProjectFromDirectory( char *dir );


void					 getRunaroundDirAndName();


void initRunaroundModule( void ) {
	
    printf("\ninitialising runaround editor\n");
	
    systemDir  = (const char *)getSysDirName();
	
    runaroundProjectName = (char *)malloc( strlen(StdRunaroundProjectTitle) + 1);
    sprintf(runaroundProjectName,"%s", StdRunaroundProjectTitle);
	
    stdRunaroundSubPatternName = (char *)malloc(strlen(StdRunaroundDefaultPatternTitle) + 1);
    sprintf(stdRunaroundSubPatternName,"%s", StdRunaroundDefaultPatternTitle);
	
	stdRunaroundCompletePatternName = (char *)malloc(strlen(StdRunaroundCompletePatternTitle) + 1);
    sprintf(stdRunaroundCompletePatternName,"%s", StdRunaroundCompletePatternTitle);
	
	stdRunaroundMovementOnlyName = (char *)malloc(strlen(StdRunaroundMovementOnlyTitle) + 1);
    sprintf(stdRunaroundMovementOnlyName,"%s", StdRunaroundMovementOnlyTitle);
	
	closeInputWindow();
	closeListWindow();		
//    runaroundEditorHasOpenWindows = false;
	
	//    atexit( _closeRunaround );
    printf("initialising runaround editor... done\n");
}

bool runaroundEditorHasOpenWindows( void )
{
	if( (listID == 0 && inputID != 0) || (listID == 0 && inputID != 0)){
		_myPrintf("just one runaround window open!\n");
		assert(  false );
	}
	
	return listID || inputID ;
}





void makeRunaroundEditorWindows( void ) {
	
    listWindow();
    inputWindow();
	
//    runaroundEditorHasOpenWindows = true;
}


void initRunaround( void )
{
	initRunaroundModule();
	initRunaroundList();
//	initRunaroundInput();
	
}



void newRunaroundProject( void ) {
	
	if( runaroundEditorHasOpenWindows()){
		
//		showDialog( languageStrings[languageRunaroundProjectCloseDialog]);
		return;
	}
	
	if( !askForRunaroundProjectName() )
        return;
	
	
    getRunaroundDirAndName();
    initListModul();
	
    makeRunaroundEditorWindows();
	
    saveRunaroundProject();
	
}


void _closeRunaround( void ) {
	
    if( !runaroundEditorHasOpenWindows())
        return;
	
    if( getRunaroundPatternChanged() ){
		 
		if(exitModulesFlag)	{
	        saveRunaroundProjectAs();	
			closeRunaroundNow();
		}
		else
			askForSaveInRunaround = true;
	}else
		closeRunaroundNow();
	
	if( !askForSaveInRunaround )
		setRunaroundPatternChanged( false );
}

void closeRunaroundNow( void )
{
	
    if( 0 && getRunaroundPatternChanged() )
        saveRunaroundProjectAs();
	
    closeListWindow();
    closeInputWindow();
	
    //      free( runaroundProjectName);
    //      free( stdRunaroundSubPatternName); // this is initialized at startup and never released
	
    if( fieldList )
        disposeFieldList( fieldList, totalFields);
    fieldList = 0;
	
//    runaroundEditorHasOpenWindows = false;
	
#ifdef useWwMalloc
	
    wwFreeAll();
    wwCheckMem();
#endif
}




//yyy erst erzeugen, wenn projekt erzeugt wird

void generateDefaultPattern( int dummy ) {
#if MAC_CLASSIC_VERSION
#pragma unused (dummy)
#endif
	
	if( !fexists( runaroundDefaultPattern ) )
		_generateDefaultPattern( 0 );
}

void _generateDefaultPattern( int dummy ) {
#if MAC_CLASSIC_VERSION
#pragma unused (dummy)
#endif
	
	makeDefaultPattern( fieldList->c );
}


void makeDefaultPattern(int jugglers ) {
    int c;
    FILE *fp;
	
    printf("generating default pattern for %d jugglers\n", jugglers);
	
	
	// initialize pattern
	//    sprintf(runaroundDefaultPattern, "! %s %d 2 2\n", sizeString, jugglers);
	sprintf(runaroundDefaultPattern, "");
    if( jugglers > 1 )
        sprintf(runaroundDefaultPattern, "%s\n< ", runaroundDefaultPattern);
	
    for( c = 0; c < jugglers - 1; c++) {
        sprintf(runaroundDefaultPattern, "%s %d %d|", runaroundDefaultPattern, defaultThrowForRunaroundPatterns, defaultThrowForRunaroundPatterns);
    }
	
    sprintf(runaroundDefaultPattern, "%s %d %d", runaroundDefaultPattern, defaultThrowForRunaroundPatterns, defaultThrowForRunaroundPatterns);
	
    if( jugglers > 1 )
        sprintf(runaroundDefaultPattern, "%s >\n", runaroundDefaultPattern);
	
	
    sprintf(fullSubPatternName, "%s%s", runaroundProjectPath, stdRunaroundSubPatternName);
    fp = fopen( fullSubPatternName, "w");
    if( fp == 0) {
        printf("can't write runaround default pattern %s\n", fullSubPatternName);
        getchar();
        exit( 1);
    }
	
    fwrite( runaroundDefaultPattern, 1, strlen( runaroundDefaultPattern), fp);
    fclose( fp );
	
    _myPrintf("default pattern generated as %s\n", fullSubPatternName);
	
}



void addDefaultPattern( _fieldContent *list, int entries, _fieldContent *f ) {
    int i;
    char *fullPathName;
    char *name;
	
    if( *fullSubPatternName == '\0' )
        makeDefaultPattern(list->c);
	
    i = getFieldIndex( list, entries, f );
    name = (char *)malloc( 50 );
	
    sprintf(name, "p%d.pass", i);
	
    if( !*runaroundProjectPath )
        printf("darf nicht!\n");
	
#if 01
    fullPathName = (char *)malloc( 1 + strlen( runaroundProjectPath) + strlen(name) + 1 );
    sprintf(fullPathName, "%s%s",runaroundProjectPath, name);
	
    if( fexists( fullPathName )) {
        extern wwwBoolean numberOfJugglersHasChanged;
		
        if( numberOfJugglersHasChanged)
            _myPrintf("overwriting existing default pattern\n");
		
        else
            _myPrintf("%s already exists\n", fullPathName);
		
    } else {
        _myPrintf("copy default pattern to %s\n", fullPathName);
        _myPrintf("copyFile returns %d\n",
		copyFile( fullPathName, fullSubPatternName));
    }
	
#else
    fullPathName = (char *)malloc( strlen(fullSubPatternName) + 1 );
    sprintf(p, "%s", fullSubPatternName);
#endif
    free( fullPathName );
	
	
	addNameToFileList(name);
    f->pattern = name;
}






#if 01
void  runaroundTextOutput(GLfloat x, GLfloat y, float w, char *text) {
    char *p;
    float s = .2f;
    GLfloat lw;
	
    glGetFloatv(GL_LINE_WIDTH, &lw);
	
    glLineWidth(w);
    glPushMatrix();
    glTranslatef(x-10, y-10, 0);
    glScalef( s,s,s);
    for (p = text; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
    glLineWidth(lw);
}
#endif



void _selectPattern( void ) {
    _fieldContent *p;
    char *buf;
    FILE *fp;
    char aktDir[DIR_SIZE];
	char i[MAX_PATTERN_TITLE];
	sprintf(i, "select pattern file (now: %s)", thePatternTitle);
	
    p = getPointerToSelectedField();
	
    if( p == 0 )
        return;
	
    // nothing selected
    if( (buf = selectAndLoadFile(i)) == NULL ) {
        printf("no pattern selected\n");
		
        return;
    }
	
    if( p->pattern )
        free( p->pattern );
	
    p->pattern = (char *)malloc( strlen( thePatternTitle) + 1 );
    sprintf(p->pattern, "%s", thePatternTitle);
	
    // save file to patDir
    getcwd( aktDir, DIR_SIZE );
	chdir( runaroundProjectPath );
	
    printf("saving file to:\n");
    printf("dir     = %s\n", runaroundProjectPath);
    printf("pattern = %s\n", p->pattern);
	
    fp = fopen( p->pattern, "w");
    if( fp ) {
        fwrite( buf, 1, strlen( buf), fp);
        fclose( fp );
        //              printf("%s added\n", p->pattern);
    } else
        printf("can't write file %s\n", p->pattern);
	
    chdir( aktDir);
	
	
}




wwwBoolean askForRunaroundProjectName( void ) {
    char *buf = "\0";
    char *path, *name;
    wwwBoolean ret;
	char i[100];
	sprintf(i, "save runaround project");
	
    ret = saveTextFileAs(buf, runaroundProjectName, i);
    if( ret == false )
        return false;
	
    // get new path and name
    path = getPathFromFullName( fullPatternPathName);
    name = getNameFromFullName( fullPatternPathName);
	
    printf("projectPath = %s\n", path);
    printf("projectName = %s\n", name);
	
    free( path);
    free( name);
	
    return true;
}



void saveRunaroundProjectAs( void ) {
    char *path, *name;
    char *buf;
	char tmp[MAX_PATTERN_TITLE];
	
    path = (char *)malloc( strlen( runaroundProjectPath) + strlen( runaroundProjectName ) + 1);
    sprintf(path, "%s%s", runaroundProjectPath, runaroundProjectName );
	
    _myPrintf("saving as %s\n", path);
	
    buf = readFile( path, "r" );
    free( path );
	
    if( buf == NULL ) {
        sprintf(tmp, "can't find project file %s\n", path);
//		showDialog( tmp );
        return;
    }
	
    if( !askForRunaroundProjectName( ))
        return;
	
	
    // get new path and name
    path = getPathFromFullName( fullPatternPathName);
    name = getNameFromFullName( fullPatternPathName);
	
    printf("projectPath = %s\n", path);
    printf("projectName = %s\n", name);
	
    _saveRunaroundProject( path, name);
	
    free( name );
    free( path);
    free( buf );
	
	
    getRunaroundDirAndName();
	
}


void saveRunaroundProject( void ) {
	
    printf("changed\n");
    _saveRunaroundProject( runaroundProjectPath, runaroundProjectName);
	
}

bool getRunaroundPatternChanged( void )
{
	return runaroundContentHasChanged;
}

void setRunaroundPatternChanged( wwwBoolean value ) {
	runaroundContentHasChanged = value;
}


void _saveRunaroundProject( char *dir, char *name ) {
    FILE *fp;
    char aktDir [DIR_SIZE];
    char s[DIR_SIZE * 2 ], d[DIR_SIZE * 2 ];
    int c;
	
	
    writeRunaroundPattern( fieldList, totalFields );
	
	
    getcwd(aktDir, DIR_SIZE);
    chdir( dir );
	
    printf("saving runaround project as >%s<\n>%s<\n", dir, name);
    if( runaroundProjectPath[0] == '\0')
        printf("undefined project path\n");
	
    // collect patterns
    for( c = 0; c < totalFields; c++ ) {
        char *buf;
        _fieldContent *f;
		
        f = getField( fieldList, c);
		
        sprintf(s, "%s%s", runaroundProjectPath, f->pattern);
        sprintf(d, "%s%s", dir, f->pattern);
        printf("copy %s to %s\n", s, d);
		
		
        buf = readFile( s, "r");
        if( buf ) {
            writeFile( d, "w", buf, strlen(buf));
			
            free( buf );
        } else
            printf("cant find %s\n", s);
		
    }
	
	
    fp = fopen( name, "w");
    if( fp ) {
        int c;
        //printf("\nwriting global information\n");
        // print general information
        fprintf(fp,"%s\n", RunaroundInfoString);
        // print pattern information
        fprintf(fp,"%s %d\n", RunaroundJugglers, fieldList->c);
        fprintf(fp,"%s %d\n", RunaroundEntries, totalFields);
        fprintf(fp,"%s %.2f\n", RunaroundMetersOnScreen, metersOnScreen);
//        fprintf(fp,"%s %d\n", RunaroundDrawPos, jpPrefs.showJugglerPos);
//        fprintf(fp,"%s %d\n", RunaroundDrawPoi, jpPrefs.showJugglerPoi);
		fprintf(fp,"%s %d\n", RunaroundCorners, cornersInRunaroundView);
		fprintf(fp,"%s %d\n", RunaroundRotationOfFrame, startingAngleOfFrame);
		fprintf(fp,"%s %d\n", RunaroundSelectionIndex, selectIndex);
		fprintf(fp,"%s %d\n", RunaroundShowIndex, runaroundShowSceneIndexFlag);
		
        //printf("\nwriting global information ...done\n");
		
		// runaround windows positions
		// scale window size to 0...1
		float xf = 1., yf = 1.;
		GLint w,h;
		GLfloat lx, ly, lw, lh;
		GLfloat ix, iy, iw, ih;
		
		w = glutGet( GLUT_SCREEN_WIDTH);
		h = glutGet( GLUT_SCREEN_HEIGHT);
		xf = 1.f / w;
		yf = 1.f / h;
		
		glutSetWindow( inputID);
		ix = glutGet( GLUT_WINDOW_X) * xf;
		iy = glutGet( GLUT_WINDOW_Y) * yf;
		iw = glutGet( GLUT_WINDOW_WIDTH) * xf;
		ih = glutGet( GLUT_WINDOW_HEIGHT) * yf;

		glutSetWindow( listID);
		lx = glutGet( GLUT_WINDOW_X) * xf;
		ly = glutGet( GLUT_WINDOW_Y) * yf;
		lw = glutGet( GLUT_WINDOW_WIDTH) * xf;
		lh = glutGet( GLUT_WINDOW_HEIGHT) * yf;
		
		fprintf(fp,"%s %2.2f %2.2f %2.2f %2.2f\n", RunaroundListWindow, lx, ly, lw, lh);
		fprintf(fp,"%s %2.2f %2.2f %2.2f %2.2f\n", RunaroundInputWindow, ix, iy, iw, ih);
		
        // print pictures and subpatterns
        for( c = 0; c < totalFields; c++ ) {
            char *t;
			
            _fieldContent *p;
			// get field
            p = getField( fieldList, c );

            t = strrchr( p->pattern, PATH_DIVIDER);
            if( t == 0 )
                t = p->pattern;
            else
                t++;
			
            fprintf( fp, "\n%s %d %d\n", RunaroundNewLine, c, p->showInNewLine);
            fprintf( fp, "\n%s %d %s\n", RunaroundPattern, c, t);
			
            //print field content
			printRunaroundFieldContentToFile( fp, p, c);
			
		}
		
        fclose( fp );
    } else
        printf("...failed\n");
	
    chdir( aktDir );
	
	setRunaroundPatternChanged( false );
}




void openRunaroundProjectFromBuffer( char *p ) {
	if( runaroundEditorHasOpenWindows())
	{
//		showDialog( languageStrings[languageRunaroundProjectCloseDialog]);
		return;
	}

	//	else
	makeRunaroundEditorWindows();
	
	if( !_openRunaroundProjectFromBuffer( p ) ){
		   closeInputWindow();
		   closeListWindow();
	   }
	
	
}

void openRunaroundProject( int dummy )
{
#pragma unused (dummy )
	
	if( runaroundEditorHasOpenWindows())
//		showDialog( languageStrings[languageRunaroundProjectCloseDialog]);
		return;
	else
		_openRunaroundProject();
}



bool _openRunaroundProject( void ) {
    char aktDir[DIR_SIZE];
    char *buf;
	char i[100];
	bool ret = true;
	
	sprintf(i, "open runaround project");
	
	
    getcwd(aktDir, DIR_SIZE);
    chdir( runaroundProjectPath);
	
    buf = selectAndLoadFile(i);
	_myPrintf("buf = %s\n", buf);
	
	
    if( buf == NULL ) {
        printf("can't read file\n");
        chdir (aktDir);
        ret = false;
    }
	
    if( ret ){
		_openRunaroundProjectFromBuffer(buf );
		free( buf );
	}
    chdir( aktDir);
	
	return ret;
}

bool openRunaroundProjectFromDirectory( char *p ) {
	bool ret;
	
	if( runaroundEditorHasOpenWindows())
	{
//		showDialog( languageStrings[languageRunaroundProjectCloseDialog]);
		return false;
	}

	//	else
	makeRunaroundEditorWindows();
	
    ret = _openRunaroundProjectFromDirectory( p );
	if( ret == false ){
		closeInputWindow();
		closeListWindow();
//		runaroundEditorHasOpenWindows = false;
	}
	
	return ret;
		
}

bool _openRunaroundProjectFromDirectory( char *dir ){
	char filename[DIR_SIZE];
	char *buf;
	bool ret = true;
	

	sprintf(filename, "%s%s", dir, runaroundProjectName );

	if( (buf = readFile(filename, "r")) == NULL ) {
		printf("can't read file\n");
		ret = false;
	}
	   
	if( ret == true )
		ret = _openRunaroundProjectFromBuffer(buf );

	if( buf )
		free( buf );

	
	return ret;
}


bool _openRunaroundProjectFromBuffer( char *buf ) {
    _fieldContent			*newList;
    char                    *p;
    int                     entries, jugglers;
	
	
	
    getRunaroundDirAndName();
	removeListFile( );
	
//    printf("reading pattern from %s%s\n", runaroundProjectPath, runaroundProjectName);
	

    // read jugglers
    p = strstr( buf, RunaroundJugglers);
    if( p == 0 ) {
        printf("no entry for jugglers\n");
        return false;
		
    }
    p += strlen( RunaroundJugglers );
    sscanf(p, "%d", &jugglers);
	runaroundJugglers = jugglers;
	
    // read entries
    p = strstr( buf, RunaroundEntries);
    if( p == 0 ) {
        printf("no entry for Positions\n");
        return false;
		
    }
    p += strlen( RunaroundEntries );
    sscanf(p, "%d", &entries);
	
	
	
//    printf("entries = %d, jugglers = %d\n", entries, jugglers);
	
	
    // read scale
    if( (p = strstr( buf, RunaroundMetersOnScreen)) != NULL){
        p += strlen( RunaroundMetersOnScreen );
        sscanf(p, "%f", &metersOnScreen);
//        printf("metersOnScreen scaled to %f\n", metersOnScreen);
    }
	
#if 0
    // read pos flag
    if( (p = strstr( buf, RunaroundDrawPos)) != NULL){
        p += strlen( RunaroundDrawPos );
        sscanf(p, "%d", &jpPrefs.showJugglerPos);
//        printf("RunaroundDrawPos is %d\n", drawPosInRunaroundEditorFlag);
    }


	// read poi flag
    if( (p = strstr( buf, RunaroundDrawPoi)) != NULL){
        p += strlen( RunaroundDrawPoi );
        sscanf(p, "%d", &jpPrefs.showJugglerPoi);
//        printf("RunaroundDrawPoi is %d\n", drawPoiInRunaroundEditorFlag);
    }
#endif
	
	
	// read corners
    if( (p = strstr( buf, RunaroundCorners)) != NULL ){
        p += strlen( RunaroundCorners );
        sscanf(p, "%d", &cornersInRunaroundView);
//        printf("RunaroundCorners is %d\n", cornersInRunaroundView);
    }

	// read angle
    if( (p = strstr( buf, RunaroundRotationOfFrame)) != NULL ){
        p += strlen( RunaroundRotationOfFrame );
        sscanf(p, "%d", &startingAngleOfFrame);
//        printf("RunaroundRotationOfFrame is %d\n", startingAngleOfFrame);
    }
	
	// read window positions
	float xf = 1., yf = 1.;	
	xf = glutGet( GLUT_SCREEN_WIDTH);
	yf = glutGet( GLUT_SCREEN_HEIGHT);

	
	if( (p = strstr( buf, RunaroundListWindow)) != NULL ){
		GLfloat x, y, w, h;
        p += strlen( RunaroundListWindow );
        sscanf(p, "%f %f %f %f", &x, &y, &w, &h);
		glutSetWindow( listID );
		glutPositionWindow( x * xf, y * yf );
		glutReshapeWindow( w * xf, h *yf );
	}

	if( (p = strstr( buf, RunaroundInputWindow)) != NULL ){
		GLfloat x, y, w, h;
        p += strlen( RunaroundInputWindow );
        sscanf(p, "%f %f %f %f", &x, &y, &w, &h);
		glutSetWindow( inputID );
		glutPositionWindow( x * xf, y * yf );
		glutReshapeWindow( w * xf, h *yf );
	}
	
	newList = initFieldList(entries, jugglers, false);
	
	
    if(newList ) {
        // read content to new list
        p = buf;
        while( p )
            p = readPatternName( p, newList, entries);

		p = buf;
        while( p )
            p = readNewLine( p, newList, entries);

        p = buf;
        while( p )
            p = readPos( p, newList, entries, jugglers);
		
        p = buf;
        while( p )
            p = readPoi( p, newList, entries, jugglers);
		
        p = buf;
        while( p )
            p = readPoiIn( p, newList, entries, jugglers);
		
        p = buf;
        while( p )
            p = readPoiOut( p, newList, entries, jugglers);
		
        p = buf;
        while( p )
            p = readPosIn( p, newList, entries, jugglers);
		
        p = buf;
        while( p )
            p = readPosOut( p, newList, entries, jugglers);
		
        if( fieldList )
            disposeFieldList( fieldList, totalFields);
        fieldList = 0;
		
        fieldList = newList;
        totalFields = entries;
		
		printRunaroundContent();
		
		setRunaroundPatternChanged( false );
		printf("done\n");
		
		
    } else
        printf("can't create new field list\n");

	selectIndex = -1;
	setSelectedFieldByPointer( NULL);
	// read input pattern index
    if((p = strstr( buf, RunaroundSelectionIndex)) != NULL ){
		_fieldContent *t;
        
		p += strlen( RunaroundSelectionIndex );
        sscanf(p, "%d", &selectIndex);
		
		t = getField( fieldList, selectIndex);
		setSelectedFieldByPointer( t );	
    }
	
	// read  show index flag
    if((p = strstr( buf, RunaroundShowIndex)) != NULL ){
		_fieldContent *t;
        
		p += strlen( RunaroundShowIndex );
        sscanf(p, "%d", &runaroundShowSceneIndexFlag);
    }

	return true;
}




char *readPatternName( char *buf, _fieldContent *l, int maxE) {
    char *t;
    _fieldContent *f;
    int e;
    char name[DIR_SIZE * 2 ];
	
	
    t = strstr( buf, RunaroundPattern);
    if( t ) {
        t += strlen( RunaroundPattern);
		
//        sscanf( t, "%d %s", &e, &name);
// read field number and whole name (until end of line)
          sscanf( t, "%d %[^\n]", &e, &name);
		
        if( e > maxE) {
            printf("wrong index %d\n", e);
            exit( 0 );
			
        } else {
            char *namep;
            size_t namel;
			
            namel = strlen( name);
            namep = (char *)malloc( namel + 1 );
			
            //              printf("pattern name is %s, length is %ld\n", name, namel);
			
			
            if( namep == 0) {
                printf("can't get memory to save pattern name %s\n", name);
                exit( 0 );
            }
            sprintf(namep, "%s", name);
			
            f = getField( l, e);
			
            if( f->pattern )
                free( f->pattern);
			
            f->pattern = namep;
#ifdef DEBUG_RUNAROUND_READING
			printf("f->patternName = %s\n", namep);
#endif
        }
    }
	
    return t;
	
}

void releasePatternName( char *p) {
    free(  p );
}

char *readNewLine( char *buf, _fieldContent *l, int maxE) {
    char *t;
    _fieldContent *f;
    int e;
    int v = 0;
	
    t = strstr( buf, RunaroundNewLine);
    if( t ) {
        t += strlen( RunaroundNewLine);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d", &e, &v);
		
        if( e > maxE) {
            printf("wrong index %d\n", e);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            f->showInNewLine = v;
#ifdef DEBUG_RUNAROUND_READING
			printf("newLine in  %d %d\n", e,v);
#endif
        }
    }
	
    return t;
	
}



char *readPos( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundPos);
    if( t ) {
        t += strlen( RunaroundPos);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
			return t; // war exit(0);
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->pos.x = x;
            jp->pos.y = y;
            jp->pos.z = z;
#ifdef DEBUG_RUNAROUND_READING
			printf("readPos(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}




char *readPoi( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundPoi);
    if( t ) {
        t += strlen( RunaroundPoi);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->poi.x = x;
            jp->poi.y = y;
            jp->poi.z = z;
			
			jp->poiIn = jp->poiOut = jp->poi;
			
#ifdef DEBUG_RUNAROUND_READING
			printf("readPoi(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}

char *readPosIn( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundIn);
    if( t ) {
        t += strlen( RunaroundIn);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->posIn.x = x;
            jp->posIn.y = y;
            jp->posIn.z = z;
#ifdef DEBUG_RUNAROUND_READING
			printf("readIn(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}

char *readPosOut( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundOut);
    if( t ) {
        t += strlen( RunaroundOut);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->posOut.x = x;
            jp->posOut.y = y;
            jp->posOut.z = z;
#ifdef DEBUG_RUNAROUND_READING
			printf("readOut(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}

char *readPoiIn( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundPoiIn);
    if( t ) {
        t += strlen( RunaroundPoiIn);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->poiIn.x = x;
            jp->poiIn.y = y;
            jp->poiIn.z = z;
#ifdef DEBUG_RUNAROUND_READING
			printf("readIn(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}

char *readPoiOut( char *buf, _fieldContent *l, int maxE, int maxJ) {
    char *t;
    _fieldContent *f;
    _runaroundJuggler *jp;
    int e,j;
    float x,y,z;
	
    t = strstr( buf, RunaroundPoiOut);
    if( t ) {
        t += strlen( RunaroundPoiOut);
        //              printf("%.20s\n", t);
		
        sscanf( t, "%d %d %f %f %f", &e, &j, &x, &y, &z);
		
        //printf("%d %d = %.2f %.2f %.2f\n", e, j, x, y, z);
		
        if( e > maxE || j > maxJ ) {
            printf("wrong indices %d %d\n", e,j);
            exit( 0 );
			
        } else {
            f = getField( l, e);
            jp = getRunaroundJuggler( f, j);
            //printf("save %d %d\n", e, j);
            jp->poiOut.x = x;
            jp->poiOut.y = y;
            jp->poiOut.z = z;
#ifdef DEBUG_RUNAROUND_READING
			printf("readOut(%d,%d) = %.2f %.2f %.2f\n", e, j, x, y, z);
#endif
        }
    }
	
    //      printRunaroundFieldContentToScreen(l, f );
	
    return t;
	
}


void writeAndStartRunaroundPattern( void ) {
    char *fullName;
	int i;
	_fieldContent *s;
	bool v;
	char msg[1000];
	char *p;
	char d[MAX_PATTERN_TITLE];
	
	
#ifdef DEBUG_RUNAROUND_READING
    printf("write and start runaround ");
#endif	
	
    writeRunaroundPattern( fieldList, totalFields );
	
    fullName = (char *)malloc( strlen( runaroundProjectPath) + strlen(StdRunaroundCompletePatternTitle) + 1 );
    sprintf(fullName, "%s%s", runaroundProjectPath, StdRunaroundCompletePatternTitle);
#ifdef DEBUG_RUNAROUND_READING
	printf("starting runaround from file %s", fullName);
#endif
	
	
	
	// check if every sub pattern is valid
	// bring up a dialog otherwise
	v = true;
	for( i = 0; i < totalFields; i++ )
	{
		s = getField( fieldList, i);

		sprintf( d, "%s%s", runaroundProjectPath, s->pattern );
		p = readFile( d, "r" );
		if( p == NULL || *p == '\0'){
			v = false;
		}else{
			initSiteswapPattern(p, strlen( p ) );
			if( validPattern == false )
				v = false;
		}
		free( p );
		if( v == false )
			break;
	}
	if( v == false ){
		sprintf(msg, "pattern %s invalid at runaround position %i", s->pattern, i );
//		showDialog( msg);
		
	}
		
	readAndStartPattern(fullName);
}

void writeRunaroundPattern( _fieldContent *f, int count ) {
	
     // write only positions (without siteswap to make it includable
	_writeRunaroundPattern( runaroundProjectPath, stdRunaroundMovementOnlyName, f, count, false);

	 // write whole pattern
	_writeRunaroundPattern( runaroundProjectPath, stdRunaroundCompletePatternName, f, count, true);
	
}

void _writeRunaroundPattern( char *path, char *name, _fieldContent *p, int count, bool printSiteswapInformation ) {
    int				c, i;
    char			*t, *u;
    _fieldContent	*f;
    int				dummy, jc, l;
    float			totalTime, partTime, tt;
    FILE			*rfp;
    char			aktDir[DIR_SIZE];
    char			tDir[DIR_SIZE];
	
    getcwd( aktDir, DIR_SIZE);
    chdir( path);
    getcwd( tDir, DIR_SIZE);

	//	fprintf( rfp, "#include \"%s\"\n", RunaroundDelayFile);

	if( !fexists( RunaroundDelayFile ))
		createDefaultRunaroundDelayFile();
	
	runaroundDelayWalk = readRunaroundDelayFileValue( );
//	_myPrintf("runaroundDelayWalk = %f\n", runaroundDelayWalk);
	
    // get total time
    totalTime = 0.;
	
    for( c  = 0; c < count; c++ ) {
        f = getField( p, c);
        t = readFile( f->pattern, "r" );
        if( t == 0 ) {
            printf("can't read pattern >%s<\n...sorry\n", f->pattern);
            chdir( aktDir);
            return;
			
        }
		
        u = strstr( t, sizeString);
        if( u ) {
            u += strlen( sizeString);
            sscanf(u, "%d %d %d", &jc, &dummy, &l);
        } else {
            //                              printf("not a cdew pattern. Trying to read as siteswap\n");
            if( !calculateSizeOfPlainSiteswap( t, &jc, &l)) {
                printf("failed\n");
                goto bail2;
            }
			
			
        }
		
		
        totalTime += l;
		
        free( t );
    }
//    printf("totalTime = %.2f\n", totalTime);
	
    {
        char d[DIR_SIZE];
        getcwd(d, DIR_SIZE);
        fixDirName( d );
		
//        printf("\n\nwriting runaround output pattern to %s%s\n", d, name);
    }
	

	
   rfp = fopen( name, "w");
    if( rfp == 0 ) {
        printf("can't create runaround pattern as %s\n", name);
        getchar();
        exit( 1);
    }
	fprintf(rfp, "!pattern time is %03.2f\n", totalTime);
	if(((int)totalTime)%2){
		fprintf(rfp,	"! Please note: the lenght of this pattern is odd.\n"
						"! This means that you will end (at least in siteswap) on the \"wrong\" hand.\n"
						"! JoePass! handles this issue by reading the pattern twice,\n"
						"! to return to the \"right\" hand at the end of the pattern\n"
						"! Please refer to the documentation of the runarond editor.\n ");
		
		
	}

	
	
    // write pattern, add times
    partTime = 0;
	
    for( i  = 0; i < count; i++ ) {
        float thisTime;
        c = i;//(i + count-1) % count;
        //xprintf("writing part %d as %d\n", i, c);
		
		
        f = getField( p, c);
		
        t = readFile( f->pattern, "r" );
        //printf("%s\n", t);
		
        if( t == 0 ) {
            printf("can't read pattern\n...sorry\n");
            goto bail;
        }
		

		
        u = strstr( t, sizeString);
        if( u ) {
            u += strlen( sizeString);
            sscanf(u, "%d %d %d", &jc, &dummy, &l);
        } else {
            //                      printf("not a cdew pattern. Trying to read as siteswap\n");
            if( !calculateSizeOfPlainSiteswap( t, &jc, &l)) {
                printf("failed\n");
                goto bail2;
            }
        }

 
		// write pattern
        thisTime = (float)partTime / totalTime;
        partTime += l;
        /*              while( thisTime >= 1.f )
		 thisTime -= 1.f;
		 */
		
        //x             printf("writing part %d of %d, time = %.2f, pattern is %s\n", c + 1, count, thisTime, f->pattern);
		
        fprintf(rfp, "\n!runaround part %d\n", c);
		
        for( jc = 0; jc < f->c; jc++ ) {
            _runaroundJuggler *j;
            punkt pos, poi, poiIn, poiOut, posIn, posOut;
			
			
			
            j = getRunaroundJuggler( f, jc);
			
 
#if 0
			// make view identical in runaround editor and animation window
            // rotate all points by 180 degrees
            // multiply x by -1. (in fprintf command)
			
			pos = j->pos.drehungZ(0.f);
            poi = j->poi.drehungZ(0.f);
            poiIn =  j->poiIn.drehungZ(0.f);
            poiOut = j->poiOut.drehungZ(0.f);
            posIn =  j->posIn.drehungZ(0.f);
            posOut = j->posOut.drehungZ(0.f);
#else
			pos = j->pos;
            poi = j->poi;
            poiIn =  j->poiIn;
            poiOut = j->poiOut;
            posIn =  j->posIn;
            posOut = j->posOut;
#endif
			
			tt = (thisTime + runaroundDelayWalk/totalTime ) * 100.;
#if 01
			while( tt >=  100. )
				tt -= 100.;
#endif
//			_myPrintf("totalTime = %2.2f, runaroundDelayWalk = %2.2f, thisTime = %2.2f, t = %2.2f\n", totalTime, runaroundDelayWalk, thisTime, tt);
			
            fprintf(rfp, "#walk %d %.2f "
                    "(%.2f, 0, %.2f) (%.2f, 0, %.2f) "
                    "li(%.2f, 0, %.2f) "
                    "lo(%.2f, 0, %.2f) "
                    "i(%.2f, 0, %.2f) "
                    "o(%.2f, 0, %.2f)"
                    "\n",
                    jc + 1, tt,
                    pos.x, -pos.y,
                    poi.x, -poi.y,
                    poiIn.x, -poiIn.y,
                    poiOut.x, -poiOut.y,
                    posIn.x, -posIn.y,
                    posOut.x, -posOut.y
					
					);
			
			
        }
		
        // print information string
        fprintf(rfp, "#i %s: %3d", AnimatedRunaroundSceneInfoString, i + 1);
        if( printSiteswapInformation )
			fprintf(rfp, ", associated pattern: %s", f->pattern);
        fprintf(rfp, "\n");
		
		// if includeMode is true, only the positions and the duration will be saved.
		// you can add the result to the include directory without changes.
		if( printSiteswapInformation  ){
			// print siteswap
			fprintf(rfp, "%s\n", t);
		}
		
        free( t );
    }
	
bail:
    fclose( rfp);
bail2:
    chdir( aktDir);
	
}





void _editPatternInCDEW( void ) {
	
#if USE_CDEW
    int id;
    _fieldContent *f;
    char p[DIR_SIZE * 2];
	
    f = getPointerToSelectedField( );
    if( f == 0 )
        return;
	
    if( f->pattern == 0 )
        return;
	
	
#if 0
	
    getcwd( aktDir, DIR_SIZE);
    chdir( runaroundProjectPath);
    //      sprintf(p, "%s%s", projectPath, f->pattern);
    printf("transfering pattern to cdew\n");
    printf("path = %s\n", runaroundProjectPath);
    printf("file = %s\n", f->pattern);
	
    //      writeFile( edit2NewCdewTempPath, "w", textEditorContent, strlen(textEditorContent));
    id = makeOglwin( f->pattern );
    if( id ) {
        writePatternToOglwin( id, f->pattern, false );
        markCdewPatternSaved( id );
        setOglwinSize( id );
        cdewJugglerColor( 0 );
    }
	
    chdir( aktDir );
#else
	
    sprintf(p, "%s%s", runaroundProjectPath, f->pattern);
    printf("transfering pattern to cdew\n");
    printf("fullPath = %s\n", p);
	
    //      writeFile( edit2NewCdewTempPath, "w", textEditorContent, strlen(textEditorContent));
    id = makeOglwin( f->pattern );
    if( id ) {
        writePatternToOglwin( id, p, false );
        markCdewPatternSaved( id );
        setOglwinSize( id );
        cdewJugglerColor( 0 );
        setPatternSavedByUser( id, true );
    }
	
#endif
	
	
#endif
}


void _editPatternInTE( void ) {
	
    _fieldContent *f;
    char p[DIR_SIZE * 2];
	
	
	
	
    f = getPointerToSelectedField( );
    if( f == 0 )
        return;
	
	dontSaveTextEditorContentThisTimeWithName = false;
	
    sprintf(p, "%s%s", runaroundProjectPath, f->pattern);
	editorPathValid = true;
    writeToTextEditor( p );
}


void getRunaroundDirAndName() {
	
    char *p;
	
    p = getPathFromFullName( fullPatternPathName );
    sprintf(runaroundProjectPath, "%s", p);
    free( p );
	
    if( runaroundProjectName )
        free( runaroundProjectName );
    runaroundProjectName = getNameFromFullName( fullPatternPathName );
}





char *getPathFromFullName( char *fullPatternPathName) {
    char *t, *path;
    char c;
	
    t = strrchr(fullPatternPathName, PATH_DIVIDER);
    if( t == 0 ) {
        path = (char *)malloc( strlen("")+1);
        sprintf(path, "%s", "");
        return path;
    } else {
        t++;
		
        c = *t;
        *t = '\0';
		
        path = (char *)malloc( strlen( fullPatternPathName)+1 );
        sprintf(path, "%s", fullPatternPathName);
		
        *t = c;
		
		
    }
	
    return path;
}



char *getNameFromFullName( char *fullPatternPathName) {
    char *t;
    char *name;
	
    t = strrchr(fullPatternPathName, PATH_DIVIDER);
    if( t == 0 ) {
        name = (char *)malloc( strlen(fullPatternPathName)+1);
        sprintf(name, "%s", fullPatternPathName);
        return name;
    } else {
        t++;
        name = (char *)malloc( strlen( t )+1 );
        sprintf(name, "%s", t);
		
    }
	
    return name;
}


void    redrawRunaroundWindows() {
    GLint a;
	
	
    a = glutGetWindow( );
	
    if( inputID ) {
        glutSetWindow( inputID );
        glutPostRedisplay( );
    }
    if( listID ) {
        glutSetWindow( listID );
        glutPostRedisplay( );
    }
	
	
    if( a )
        glutSetWindow( a );
	
}


void printRunaroundContent( void )
{
	int e;
	
	
	return;
	
	
	if( fieldList == 0){
		printf("\n-------------------------\n"
			   "no runaround pattern in memory\n");
		return;
	}		
	
	printf("\n----------------------\n"
		   "runaround list content\n"
		   "jugglers: %d\n"
		   "entries:  %d\n", runaroundJugglers, totalFields);
	
	for( e = 0; e < totalFields; e++) {
		_fieldContent *p;
		
		
		p = getField( fieldList, e);
		printf("e = %d von %d, p(j) = %d\n", e, totalFields, p->c);
		
		printRunaroundFieldContentToScreen(fieldList, p );
		printf("\n");
	}
}

void printRunaroundFieldContentToScreen(_fieldContent *l,  _fieldContent *f ) {
    int e, j;
	
    e = getIndexFromFieldPointer(l, f );
	
	printf("field: j = %d, e = %d\n", f->c, e); 
	
    for( j = 0; j < f->c; j++ ) {
        _runaroundJuggler *jp;
		
        jp = getRunaroundJuggler( f, j);
		
		printf("field->j = %d, j = %2d, e = %d\n", f->c, j, e);
        printf("pos    %d %d = %05.2f %5.2f %5.2f\n", e, j, jp->pos.x, jp->pos.y, jp->pos.z);
        printf("poi    %d %d = %05.2f %5.2f %5.2f\n", e, j, jp->poi.x, jp->poi.y, jp->poi.z);
		printf("poiIn  %d %d = %05.2f %5.2f %5.2f\n",  e, j, jp->poiIn.x, jp->poiIn.y, jp->poiIn.z );
		printf("poiOut %d %d = %05.2f %5.2f %5.2f\n",  e, j, jp->poiOut.x, jp->poiOut.y, jp->poiOut.z );
        printf("in     %d %d = %05.2f %5.2f %5.2f\n", e, j, jp->posIn.x, jp->posIn.y, jp->posIn.z);
        printf("out    %d %d = %05.2f %5.2f %5.2f\n", e, j, jp->posOut.x, jp->posOut.y, jp->posOut.z);
    }
}


void printRunaroundFieldContentToFile( FILE *fp, _fieldContent *p, int c)
{
	int j;
	
	for( j = 0; j < p->c; j++ ) {
		_runaroundJuggler *rj = getRunaroundJuggler(p,j);
		
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundPos, c, j, rj->pos.x, rj->pos.y, rj->pos.z );
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundPoi, c, j, rj->poi.x, rj->poi.y, rj->poi.z );
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundPoiIn, c, j, rj->poiIn.x, rj->poiIn.y, rj->poiIn.z );
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundPoiOut, c, j, rj->poiOut.x, rj->poiOut.y, rj->poiOut.z );
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundIn, c, j, rj->posIn.x, rj->posIn.y, rj->posIn.z );
		fprintf(fp, "%s %d %d %.2f %.2f %.2f\n", RunaroundOut, c, j, rj->posOut.x, rj->posOut.y, rj->posOut.z );
	}
}


void readRunaroundFieldContent( _fieldContent *l ) 
{
	char *p, *buf;
	int jugglers = runaroundJugglers;
	char i[100];
	sprintf(i, "read runaround pattern");
	
	buf = selectAndLoadFile(i);
	
	p = buf;
	while( p )
		p = readPos( p, l, 1, jugglers);
	
	p = buf;
	while( p )
		p = readPoi( p, l, 1, jugglers);
	
	p = buf;
	while( p )
		p = readPoiIn( p, l, 1, jugglers);
	
	p = buf;
	while( p )
		p = readPoiOut( p, l, 1, jugglers);	
	
	p = buf;
	while( p )
		p = readPosIn( p, l, 1, jugglers);
	
	p = buf;
	while( p )
		p = readPosOut( p, l, 1, jugglers);	
	
	free( buf );
	
}

void saveCurrentRunaroundField( _fieldContent *currentField )
{
	char d[1000];
	FILE *fp;
	
	getcwd(d,1000);
	chdir( sysDir );
	
	
	fp = fopen( RUNAROUND_TEMP_FILE, "w");
	
	printRunaroundFieldContentToFile( fp, currentField, 1);
	
	fclose( fp);
}


void _shrinkToHalve( void )
{
	int c, t = totalFields/2;
	for( c = 0; c < t; c++ )
		_runaroundRemovePositionAtEnd();
	
	setRunaroundPatternChanged( true );
	
}

void _doublePattern( void )
{
	int c, t = totalFields;
	_fieldContent *s, *d;
	
	for( c = 0; c < t; c++ )
		_runaroundAddPosition();
	
	
	for( c = 0; c < t; c++)
	{
		s = getField(fieldList, c);
		d = getField(fieldList, c + t);
		
		copyFieldToField(d, s );
		if( c == 0)
			d->showInNewLine = true;
		
	}
	setRunaroundPatternChanged( true );
	
}

