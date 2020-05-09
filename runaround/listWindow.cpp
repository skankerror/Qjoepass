/*
 *  listWindow.cpp
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
#if 0
#define OPEN_GL_ROUTINES
#endif

/*
 list window for runaround editor.
 
 wolfgang westerboer
 westwolf@necologne.de
 cologne, 30.12.01
 */
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>


#include "globals.h"

#include "stringtoscreen.h"
#include "grafik.h"
#include "info.h"

#include "fileIO.h"

#include "cdewDraw.h"

#include "animationWin.h"

#include "inputWindow.h"
#include "listWindow.h"
#include "runaround.h"
#include "myPrintf.h"
#include "mydefs.h"
#include "myutil.h"
#include "causal_editor.h"

#include "runaroundMenu.h"

#include "exitModules.h"
#include "prefDef.h"
//#include "Dialog.h"



// JoePass routines
#include "vtypes.h"
#include "jpPrefs.h"

#ifndef false
#define true (1==1)
#define false (!true)
#endif


bool askForSaveInRunaround = false;

int numberOfRunaroundJugglers = 0;
int _numberOfRunaroundJugglers = 0;


void                    getScreenPositionOfField( _fieldContent *thisP, int *x, int *y, int *w, int *h);
void                    drawFieldInListWindow( _fieldContent *p);
int						getFieldIndexAtPosition( int x, int y);
void                    getIndicesOfField( _fieldContent *p, int *row, int *col );
_fieldContent			*getFieldWithIndices( int row, int col );
// the colum in which the marker is.
// reset to -1 when the user selects a new cell by mouse or left / right key.
// if the user selects a cell with up / down key, the curent column index is saved in here and will be used for up/down cell selection
// insted of the index of the selected field.
// this makes the cursor to jump to the desired column if possible, and to the last possible field in a row otherwise.
int						desiredColumn = -1; 


bool					checkForRunaroundDelayWalkChanged( void );
void					drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size );
void					_drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size );
void					__drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size );


wwwBoolean numberOfJugglersHasChanged;

#pragma mark ------ defines


#ifndef min
#define min( a, b) ((a)<(b)?(a):(b))
#define max( a, b) ((a)>(b)?(a):(b))
#endif


#define drawOneLine2D(x1,y1,x2,y2)  {glBegin(GL_LINES);  \
glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2) ); glEnd();}


#define defFieldsInLine                 3
#define defTotalFields                  2
#define defJugglersInPattern			3

#define listCursorColor					0.f,.5f,0.f
#define listSourceColor                 0.f,1.f,0.f
#define listDestinationColor1           1.f,0.f,.5f
#define listDestinationColor2			.75f,0.f,.35f
#define listInDragColor                 .5f,.5f,.5f
#define listAnimatedSceneColor          1.f,0.f,0.f
#define RunaroundFilesUsed				"runaroundFilesUsed.txt"
int						fieldsSelectedFlag = false;

#pragma mark ------ prototypes


int						thisX, thisY;
int						selectIndex = -1;
#define cursorIndex selectIndex
//int                     cursorIndex = -1;
int						dragIndex   = -1;
int						xDragInit, yDragInit;
int						xMouseListWin, yMouseListWin;
int						xInit, yInit;

int						runaroundScene_NoModifier_ID = -1;
int						runaroundScene_ShiftModifier_ID = -1;

int						runaroundScene_FirstSelectedScene_ID = -1;
int						runaroundScene_LastSelectedScene_ID = -1;

void					markSelectedScenes( void );
void                    setSelectedScene_IDs( void );

static void				 mouse(int button, int state, int x, int y);
static void				 motion( int x, int y );
static void				 reshape(int w, int h);
static void				 listDraw( void );

int                      initField( _fieldContent *list, int entries, _fieldContent *f, int jugglers );
void                     disposeFieldContent( _fieldContent *f );
void                     defaultContentToField( _fieldContent *f);

_fieldContent			*getPointerToSelectedField( void );



_fieldContent			*newField( void );
void                     addFieldToList( void );
void                     removeFieldFromList( void );

int                      getFieldSizeOnListScreen( void );



void                     drawFilledRect( int x, int y, int w, int h);
void                     drawEmptyRect( int x, int y, int w, int h);


void                     copyFieldToField( _fieldContent *dest, _fieldContent *source);

void					 _RemoveUnusedFilesOnFileList( void );
char					*getNextLineFromBuffer( char *buffer, char *r, int l );


#pragma mark ------ globals
_fieldContent   *fieldList = NULL;
int              totalFields     = defTotalFields;
_fieldContent   *theSelectedField = 0;

GLint			 listID = 0;



#pragma mark ------ functions
// initialise module
void initListModul( void )
{
	makeDefaultPattern( defJugglersInPattern );
	
	createDefaultRunaroundDelayFile( );
	
	totalFields = defTotalFields;
	fieldList = initFieldList(totalFields, defJugglersInPattern, true );
	
	setSelectedFieldByPointer( fieldList );
}




/*	the runaround editor logs all files it created. 
	unused files can be removed to clean up
*/

// remove file with list of created filed
void removeListFile( void )
{
	char n1[MAX_FULL_FILE_NAME];
	
	sprintf(n1, "%s%s", runaroundProjectPath, RunaroundFilesCreated);
	removeFile( n1 );
}

// add a name to list of created filed
void addNameToFileList( char *name )
{
	FILE *fp;
	char n1[MAX_FULL_FILE_NAME];	
	
	sprintf(n1, "%s%s", runaroundProjectPath, RunaroundFilesCreated);
	fp = fopen(n1, "a+");
		
	if( fp ){
		fprintf(fp, "%s\n", name);
		fclose(fp);
	}
	
}

// removes all files that have been created from but not used in current runaund project
// list a: files created in this runaround pattern
// list b: lies actually used by runaround
// currently no spaces allowed in file names

// NB: if list file is not saved,  ALL files created will be deleted
void RemoveUnusedFilesOnFileList( int dummy )
{
#pragma unused (dummy )
	_RemoveUnusedFilesOnFileList();
}

// see above
void _RemoveUnusedFilesOnFileList( void )
{
	char *a, *b;
	FILE *fp;
	char n1[MAX_FULL_FILE_NAME];	
	char n2[MAX_FULL_FILE_NAME];	
	char o[MAX_FULL_FILE_NAME], *t;
	char deleteMe[MAX_FULL_FILE_NAME];
	_fieldContent *s;
    int c;
	
	if( fieldList == NULL )
		return;
	
	// read the file with the list pattern files created
	sprintf(n1, "%s%s", runaroundProjectPath, RunaroundFilesCreated);
	a = readFile(n1, "r");
	if( a == 0 )
		return;
	
	// make a list of pattern files used by current runaround
	sprintf(n2, "%s%s", runaroundProjectPath, RunaroundFilesUsed);
	fp = fopen(n2, "w");
	if( fp ){
		for( c = 0; c < totalFields; c++)
		{
			s = getField( fieldList, c);
			if( s->pattern)
				fprintf(fp, "%s\n", s->pattern);
		}
		
		fclose(fp);
	}
	
	// read the created file
//	sprintf(n2, "%s%s", runaroundProjectPath, RunaroundFilesUsed);
	b = readFile(n2, "r");
	
	
	t = a;
	
	_myPrintf("--------------------------\n"
			  "files created, but not used: deleting...\n");
	while( t && *t ){
		t = getNextLineFromBuffer( t, o, MAX_FULL_FILE_NAME);
		if (strstr( b, o) == NULL) {
			sprintf(deleteMe, "%s%s", runaroundProjectPath, o);
			removeFile( deleteMe);
			_myPrintf("deleting %s\n", deleteMe);
		}
	}
	_myPrintf("deleting... done\n"
			  "--------------------------\n");
	
	
	// clead up
	removeFile(n2);
	if( b == 0 )
		free( b );
	if( a )
		free( a );
}

// reads everything from buffer to endOfLine (\n or \r) into r 
// returns address of next char that is not endOfLine
// stop if *address == \0
char *getNextLineFromBuffer( char *buffer, char *r, int l )
{
	char *t = buffer;
	
	if( t == NULL )
		return NULL;
	
	while( *t && *t != '\n' && *t != '\r' && l--)
		*r++ = *t++;
	
	*r = '\0';
	
	while( *t && (*t == '\n' || *t == '\r') )
		t++;
	
	return t;
}
	

// creates the list window if not already created
void listWindow( void )
{
	float screenW, screenH;
	float lx, ly, lw, lh;
	
	char s[100];
	if( listID == 0)
	{
		screenW = glutGet( GLUT_SCREEN_WIDTH );
		screenH = glutGet( GLUT_SCREEN_HEIGHT );
		
		lx = runaroundListWinSizeX * screenW;
		ly = runaroundListWinSizeY * screenH;
		lw = runaroundListWinSizeW * screenW;
		lh = runaroundListWinSizeH * screenH;
		glutInitWindowPosition( lx, ly);
		glutInitWindowSize( lw, lh);
		
		sprintf(s, "Scenes" );
		listID = glutCreateWindow( s );
		_myPrintf("listID = %d\n", listID);
		
		glutReshapeFunc( reshape);
		glutDisplayFunc( listDraw );
		glutKeyboardFunc( runaroundListKey );
		glutMouseFunc( mouse );
		glutMotionFunc( motion );
		glutSpecialFunc( runaroundListSpecialKey );

		runaroundInitMenu( );
		glutPostRedisplay();
		
		animatedRunaroundSceneIndex = -1;
	}
}


// closes the list window
void closeListWindow( void )
{
	if( listID == 0)
		return;
	
	glutDestroyWindow( listID );
	listID = 0;
	
//	RemoveUnusedFilesOnFileList( );
	removeListFile();
	
	_myPrintf("scenes window closed\n");
	
}

// glut callbacks
void mouse(int button, int state, int x, int y)
{
#ifdef TARGET_OS_MAC
#pragma unused( button )
#endif
	
	xMouseListWin = x;
	yMouseListWin = y;
	
	if( state == GLUT_UP )
	{
		
		if( selectIndex >= 0 && dragIndex >= 0)
		{
			int deltaIndex;
			
			deltaIndex = dragIndex - selectIndex;
//			_myPrintf("drag endet: selectIndex = %3d, dragIndex = %3d\n", selectIndex, dragIndex);
//			_myPrintf("drag offset = %3d\n", deltaIndex);

			
			_fieldContent *s, *d;
			
			// One scene: just a drag, no selection
			// copy content of source to dest
			s = getField( fieldList, selectIndex );
			d = getField( fieldList, dragIndex );
			copyFieldToField( d, s);

			setSelectedFieldByPointer(d);
			// Hier nicht nur das ausgewählte Feld, sondern alle markierten Felder kopieren
			// Kopie der Liste anlegen und füllen
			//	durch liste laufen
			// offset in Liste = indexD - indexS
			// wenn Feld markiert:
			// Inhalt von lfd.Index + Offset aus Kopie in Liste einfügen.
			// Auf Bereichsüber und unterschreitungen achten
			int c, anzahl, sourceIndex, destIndex;
			int start, end, offset;
			
			anzahl = 0;
			sourceIndex = -1;
			
			for( c = 0; c < totalFields; c++ ){
				s = getField( fieldList, c );
				if( s->selected){
					anzahl++;
					if( sourceIndex < 0 )
						sourceIndex = c;
				}
			}
//			_myPrintf("copying %3d scenes, first Scene is %3d\n", anzahl, sourceIndex);
			
			
			if( deltaIndex > 0 ){
				offset = -1;
				start = sourceIndex + anzahl-1;				
				end = sourceIndex-1;
				dragIndex += anzahl-1;
			}else{
				//ok
				offset = 1;
				start = sourceIndex;	
				end = sourceIndex + anzahl;
			}
			
//			_myPrintf("copying from %3d to %3d with offset %3d\n", start, end, offset);
			destIndex = dragIndex;
			
			for( c = start; c != end; c += offset ){
				s = getField( fieldList, c );
				if( s->selected){

					if( destIndex < 0 )	
						destIndex += totalFields;
					if( destIndex >= totalFields ) 
						destIndex -= totalFields;
					
					d = getField( fieldList, destIndex );

					copyFieldToField( d, s);
//					_myPrintf("copy %d to %d\n", c, destIndex);
					
					destIndex += offset ;
				}	
			}
			
			// the first scene may not have a newline tag. May happen if a scene with newline tag was dragged to the first scene
			d = getField( fieldList, 0 );
			d->showInNewLine = false;
		}
		
		
		dragIndex = -1;
		
		goto bail;
	}else
		// GLUT_DOWN
	{	_fieldContent *p;
		int m;
        int t;
        int i;
       
		xInit = xMouseListWin;
		yInit = yMouseListWin;
		
		desiredColumn = -1;	// see notes at declaration

#if 0
		selectIndex = getFieldIndexAtPosition( x,y);
		p = getField(fieldList, selectIndex);
		if( selectIndex < 0)
		{
			dragIndex = -1;
			goto bail;
		}
		
        beep();
#endif
        
		m = getModifiers();
        i = getFieldIndexAtPosition( x,y);
		
        // only proceed if a field is selected
        if( i >= 0 ){
            
            // klick with no modifiers
            // set cursor to klicked field
            if( m == 0  ){
                
                p = getField(fieldList, i);
                
                
                // field is not selected:
                // unmark everything,
                // set the cursor to this field
                if (p->selected == false){
                    unselectAllScenes( 0 );
                }
				
				// marks the first field
				runaroundScene_NoModifier_ID = i;
                selectIndex = runaroundScene_NoModifier_ID;
            
            }else if ( m == GLUT_ACTIVE_SHIFT ){
                // marks the 2nd field
				runaroundScene_ShiftModifier_ID = i;
                //selectIndex = runaroundScene_ShiftModifier_ID;
            }
            
bail2:
            printf("A: %-3d   B: %-3d\n",runaroundScene_NoModifier_ID, runaroundScene_ShiftModifier_ID);

            markSelectedScenes();
        }
		
	}
	 

	motion( x, y );
	
bail:
	redrawRunaroundWindows();
}





void runaroundSelectSceneA( int dummy){
#pragma unused(dummy)
    
    runaroundScene_NoModifier_ID = getIndexOfSelectedField();
    markSelectedScenes();
}

void runaroundSelectSceneB( int dummy){
#pragma unused(dummy)
    int t = selectIndex;
    runaroundScene_ShiftModifier_ID = getIndexOfSelectedField();
    markSelectedScenes();
    selectIndex = t;
    
}

void setSelectedScene_IDs(void){

    // at least one marker is set
    if( runaroundScene_NoModifier_ID >= 0 || runaroundScene_ShiftModifier_ID >= 0){
        
        
        // just one marker => select just this scene
        // only scene 1
        if(runaroundScene_ShiftModifier_ID < 0 ){
            selectIndex = runaroundScene_FirstSelectedScene_ID = runaroundScene_LastSelectedScene_ID = runaroundScene_NoModifier_ID;
            if ( selectIndex < 0)
                selectIndex = 0;
        }else
        // only scene2
        if(runaroundScene_NoModifier_ID < 0 ){
            selectIndex = runaroundScene_FirstSelectedScene_ID = runaroundScene_LastSelectedScene_ID = runaroundScene_ShiftModifier_ID;
            if ( selectIndex < 0)
                selectIndex = 0;
        }
        // both markers are set => switch id of first / last if last < first
        // cursor stayd the same
        else{
            if(runaroundScene_NoModifier_ID <= runaroundScene_ShiftModifier_ID ){
                runaroundScene_FirstSelectedScene_ID = runaroundScene_NoModifier_ID;
                runaroundScene_LastSelectedScene_ID = runaroundScene_ShiftModifier_ID;
            }else{
				runaroundScene_FirstSelectedScene_ID = runaroundScene_ShiftModifier_ID;
				runaroundScene_LastSelectedScene_ID = runaroundScene_NoModifier_ID;
            }
            
        }
        
    }
	printf("%d %d\n", runaroundScene_FirstSelectedScene_ID, runaroundScene_LastSelectedScene_ID);
}

void unselectAllScenes( int dummy )
{
#pragma unused(dummy)

    
    int c;
    _fieldContent *p;
    
    runaroundScene_NoModifier_ID = runaroundScene_ShiftModifier_ID = -1;
    runaroundScene_FirstSelectedScene_ID = runaroundScene_LastSelectedScene_ID =  -1;
    
    for( c = 0; c < totalFields; c++ ){
        p = getField( fieldList, c);
        p->selected = false;
    }
    
    
}



/*
 xxxxx
 
Vereinheitlichen:

modifiersNo
ModifiersShift

zu	FirstId		
lastId

und dann nur noch die verwenden	
wie programmiert man, dass da keine Fehler möglich sind?


*/
void markSelectedScenes( void ){
    
    int c;
    _fieldContent *p;
    
    int t;
    
    // sets the entries for runaroundScene_FirstSelectedScene_ID and runaroundScene_LastSelectedScene_ID
    // sorts them = >runaroundScene_FirstSelectedScene_ID <= runaroundScene_LastSelectedScene_ID
    setSelectedScene_IDs();
#if 01
	// nach aktueller programmierung ist einer von beiden immer 0,
	// da beide variablen bei jedem klic auf -1 gesetzt werden, aber nur eine danach eut gesetzt wird.
	// xxxxx
    if(runaroundScene_FirstSelectedScene_ID < 0 || runaroundScene_LastSelectedScene_ID < 0 )
        return;
#endif
    
    
    for( c = 0; c < totalFields; c++ ){
        p = getField( fieldList, c);
        p->selected = (c >= runaroundScene_FirstSelectedScene_ID && c <= runaroundScene_LastSelectedScene_ID);
    }
    
}






/*	draws the whole list.
 position and size is calculated for every field
 calls drawing routine for every field with this parameters
 */

void listDraw( void )
{
	int c;
	int s;
	_fieldContent *f;
	static int last = -1;
	int now;
	static bool redrawWindowFlag = true;
	
	// check if window still exists, crashes otherwise
	if( listID == 0)// || inputID == 0)
		return;
	if( fieldList == 0 )
		return;

	
	if( askForSaveInRunaround)
	{
		
		int now = glutGet(GLUT_ELAPSED_TIME);
		float fadeFactor;
		
		
		fadeFactor = (float)(1.f+sin(myPi/180.f*((now/10)%360)))*.25f + .5f;
		glClearColor( 0.8f*fadeFactor,0.8f*fadeFactor,1.0f*fadeFactor,0.f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLineWidth( 1.f);
		stringToScreen( "_S_ave, _D_iscard, _R_eview", centered, 0, atLine(2),defaultTextSize,&black);
		glutPostRedisplay();

		redrawWindowFlag = true;
	}else
	{
		glClearColor( 1.f,1.f,1.f,1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	setColor( 0.6f,0.6f,0.6f);
	s = getFieldSizeOnListScreen();
	
	selectedRunaroundPattern = NULL;
	
	// draw all scenes
    for( c = 0; c < totalFields; c++ ){
		
        f = getField(fieldList, c );
		
		if( c == selectIndex ){
			selectedRunaroundIndex = selectIndex;
			selectedRunaroundPattern = f->pattern;
		}
		
			
        drawFieldInListWindow(f );
		
		
    }
	
#define DELTA_LIST_MOVEMENT	10
	
	// user does a drag
	if( dragIndex>=0 )
	{
		static int lastx = -1, lasty = -1;
		
		_fieldContent *p;
		int sx,sy,sw,sh;
		int	h;
		int xo, yo;

		if(01){// || (fabs(xMouseListWin - lastx) > DELTA_LIST_MOVEMENT) || (fabs(yMouseListWin - lasty) > DELTA_LIST_MOVEMENT)){
			h = glutGet( GLUT_WINDOW_HEIGHT);
			// calculation, was in motion() before. 
			// moved here as we want to be able to do multiple
			p = getField( fieldList, selectIndex);
			setSelectedFieldByPointer( p );
			
			getScreenPositionOfField( p, &sx, &sy, &sw, &sh);
			
			// get Mouse position relative to selected image position
			xDragInit = sx - xInit; 
			yDragInit = sy - (h-yInit);
			
			xo = xMouseListWin + xDragInit;
			yo = yMouseListWin + yDragInit;

			drawDraggedField( p, xo, yo, s );
			
			for( c = 0; c < totalFields; c++ ){
				_fieldContent *f2;
				f2 = getField(fieldList, c );
				if( f2->selected ){
					
					getScreenPositionOfField( f2, &sx, &sy, &sw, &sh);

					// get Mouse position relative to selected image position
					xDragInit = sx - xInit; 
					yDragInit = sy - (h-yInit);
					
					xo = xMouseListWin + xDragInit;
					yo = yMouseListWin + yDragInit;
					
					drawDraggedField(f2, xo, yo, s );
				}
			}
			lastx = xMouseListWin;
			lasty = yMouseListWin;
			

		}
	}
	
	if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
		glutSwapBuffers();
	else
		glFlush();
	
}

void drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size )
{
	int w,h;
	
	
	
	w = glutGet( GLUT_WINDOW_WIDTH);
	h = glutGet( GLUT_WINDOW_HEIGHT);

	// field fits on screen, at least parts of it
	_drawDraggedField( f, xDragPosition, yDragPosition, size );
	

	
	if( xDragPosition < 0 ){
		xDragPosition += w;
		yDragPosition += size;
		_drawDraggedField( f, xDragPosition, yDragPosition, size );
	}
	if( xDragPosition + size > w){
		xDragPosition -= w;
		yDragPosition -= size;
		_drawDraggedField( f, xDragPosition, yDragPosition, size );
	}

	if( yDragPosition < 0 ){
		yDragPosition += h;
		_drawDraggedField( f, xDragPosition, yDragPosition, size );
	}
	if( yDragPosition + size > h){
		yDragPosition -= h;
		_drawDraggedField( f, xDragPosition, yDragPosition, size );
	}
	_drawDraggedField( f, xDragPosition, yDragPosition, size );
	
}

void _drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size )
{	
	int w,h;
	
	w = glutGet( GLUT_WINDOW_WIDTH);
	h = glutGet( GLUT_WINDOW_HEIGHT);
	
	__drawDraggedField( f, xDragPosition, yDragPosition, size );

	
}

void __drawDraggedField( _fieldContent *f, int xDragPosition, int yDragPosition, int size )
{	setColor( listInDragColor);
	drawEmptyRect( xDragPosition, yDragPosition, size, size);
	drawContentInRect(f, xDragPosition, yDragPosition, size, size );
}	

void motion( int x, int y )
{
	//	static int      lx = -1,ly;
	
	int h = glutGet(	GLUT_WINDOW_HEIGHT);
	
    if( selectIndex < 0 )
        return;
	
	
	if( xInit == x && yInit == y)
        return;
	
	/*	lx = x;
	 ly = y;
	 */	
	xMouseListWin = x;
	yMouseListWin = h-y;
	
	dragIndex = getFieldIndexAtPosition( x, y );
	
	glutPostRedisplay();
	
}


void runaroundListSpecialKey(int runaroundListKey, int x, int y)
{
#ifdef TARGET_OS_MAC
#pragma unused ( x,y )
#endif
	switch( runaroundListKey )
	{
#if 01
		case GLUT_KEY_UP:
			moveOneFieldUp();
			break;
		case GLUT_KEY_DOWN:
			moveOneFieldDown();
			break;
#endif
		case GLUT_KEY_LEFT:
			moveOneFieldLeft();
			break;
		case GLUT_KEY_RIGHT:
			moveOneFieldRight();
			break;
			
			
		case GLUT_KEY_PAGE_UP:
		case GLUT_KEY_PAGE_DOWN:
		case GLUT_KEY_HOME:
		case GLUT_KEY_END:
		case GLUT_KEY_INSERT:
		default:
			break;
	}
	redrawRunaroundWindows();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// drawList war hier


/*	this does the actual call for the preceeding routine
	draws also the frame for the source and dest frame in case of a drag
*/

void drawFieldInListWindow( _fieldContent *p)
{
	
	int x,y,w,h;
	float lw;
	int i;
	
	getScreenPositionOfField( p, &x, &y, &w, &h);
	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;
	
#if 0
	if(dragIndex >= 0 && fieldList + dragIndex == p ){
		glGetFloatv(GL_LINE_WIDTH, &lw);
		setLineWidth( 2 );
        setColor(listDestinationColor1);
		drawEmptyRect(x,y,w,h);
		setLineWidth( lw);
	}
#endif
    
// JA!
#if 01
//	wir brauchen hier nicht die Position des gewählten Feldes sondern die, des dazu gehörenden Zielfeldes
	if(dragIndex >= 0 && p->selected ){
		int d, o;
		int i, i2;
		int x,y,w,h;
		_fieldContent *p2;
		d = dragIndex - selectIndex;
		i = getFieldIndex(fieldList, totalFields, p);
		
		o = i+d;
		if( o < 0 )	
			o += totalFields;
		if( o >= totalFields) 
			o -= totalFields;
		
		p2 = getField( fieldList, o );
		i2 = getFieldIndex(fieldList, totalFields, p2);
        
#if 0
		if(i2 == dragIndex)
			setColor(listDestinationColor1);
		else
			setColor(listDestinationColor2);
#else
        if(i2 == runaroundScene_FirstSelectedScene_ID)
            setColor(listDestinationColor2);
        if( i2 == runaroundScene_LastSelectedScene_ID)
            setColor(listDestinationColor2);
        
        
#endif
		glGetFloatv(GL_LINE_WIDTH, &lw);
		setLineWidth( 2 );
		getScreenPositionOfField( p2, &x, &y, &w, &h);
		drawEmptyRect(x,y,w,h);
		setLineWidth( lw);
	}
#endif
    
#if 01
// draws the cursor (cursorIndex war selectIndex)
	if(cursorIndex >= 0 && fieldList + cursorIndex == p )
	{
		glGetFloatv(GL_LINE_WIDTH, &lw);
		setLineWidth( 2 );
		setColor(listCursorColor);
		drawEmptyRect(x,y,w,h);
		setLineWidth( lw);
	}
#endif
    
#if 01
// draws the rect around the fields with the marker selected
 
	if(p->selected)
	{
		glGetFloatv(GL_LINE_WIDTH, &lw);
		setLineWidth( 2 );
		setColor(listSourceColor);
		drawEmptyRect(x+2,y+2,w-4,h-4);
		setLineWidth( lw);
	}
#endif
    
#if 01
// draws the marker for the field that is curently in animation	
	if( runaroundShowAnimatedSceneFlag ){
		i = getFieldIndex(fieldList, totalFields, p);
	
		if( i == animatedRunaroundSceneIndex ){
			glGetFloatv(GL_LINE_WIDTH, &lw);
			setLineWidth( 2 );
			setColor( listAnimatedSceneColor);
			drawEmptyRect( p->x, p->y, p->w, p->h);
			setLineWidth( lw);
		}
	}
#endif
    
	drawContentInRect( p, x,y,w,h);
	
	
}


/*	execute the drag:
	copy content of one field to another
	used by drag N drop copy
*/
void copyFieldToField( _fieldContent *dest, _fieldContent *source)
{
	int c;
	int jugglers;
	
	if( source == dest )
		return;
	
    jugglers = min( source->c, dest->c);	
	
	for( c = 0; c < jugglers; c++ )
		*getRunaroundJuggler( dest, c) = *getRunaroundJuggler( source, c);
	
//	dest->showInNewLine = source->showInNewLine;
	if(0 && getFieldIndex(fieldList, totalFields, source ) == 0 )
		dest->showInNewLine = true;
		
			if( 0 ){
	if( dest->pattern )
		free( dest->pattern );
	
	dest->pattern = (char *)malloc(strlen(source->pattern) + 1);
	sprintf(dest->pattern, "%s", source->pattern);
	}else {
		dest->pattern = source->pattern;
	}

	setRunaroundPatternChanged( true );
}


// adds another field to the end of the list
void    addFieldToList( void )
{
	totalFields++;
	
}

// removes field from the list
void    removeFieldFromList( void )
{
	if( totalFields > 1)
		totalFields--;
	
}








/*	tags a pointer to a field
	sets the index of the field as the one selected
*/
void setSelectedFieldByPointer( _fieldContent *p)
{
	
	int d;
	_fieldContent *t;
	
	if( p == NULL )
	{
		selectIndex = -1;
		return;
	}
	
	
	// get row
	for( d = 0; d < totalFields; d++ )
	{
		
		t = getField( fieldList, d );
		
		if( t == p )
		{
			selectIndex = d;
			theSelectedField = t;
			return;
		}
	}
	
	selectIndex = -1;
	printf("Waring: setSelectedFieldByPointer(): pointer not in list\n");
	
}

/*	returns the pointer to the selected field 
 */
_fieldContent *getPointerToSelectedField( void )
{
    if( selectIndex == -1 )
        return NULL;
    else
        return 	getField( fieldList, selectIndex);
;
	
}

int getIndexOfSelectedField( void )
{
	return selectIndex;
}


/*	takes pointer to field list, number of members and index of a field
	returns pointer to the named field
 */
_fieldContent *getField(_fieldContent *list, int i)
{
	
	if( i == -1 )
		return NULL;
	else
		return list + i;
}



/*	takes pointer to field list, number of members and pointer to a field
	returns index the named field
 */
int getFieldIndex(_fieldContent *list, int entries, _fieldContent *f)
{
	
	int i;
	
	for( i = 0; i < entries; i++)
	{
		if( getField( list, i ) == f )
			return i;
	}
	
	return NULL;
	
}

/*	takes pointer to field list, number of members and index of a juggler
	returns pointer to the juggler
*/
_runaroundJuggler *getRunaroundJuggler( _fieldContent *f, int i)
{
	int c = f->c;
	
	return f->j + c - i - 1;
}

/*	fills field with default content
*/
int initField( _fieldContent *list, int entries, _fieldContent *f, int jugglers)
{
#pragma unused (list, entries )
	
	int c;
	punkt pos = punkt(250,0,0), poi = punkt(50,0,0);
	
	f->c = jugglers;
	f->j = (_runaroundJuggler*)malloc(sizeof(_runaroundJuggler) * f->c );
	f->showInNewLine = false;
	f->selected = false;
	
	if( f->j == 0 )
	{
		f = NULL;
		return 0;
	}
	
	for( c = 0; c < f->c; c++ )
		_runaroundCirclePosition( f );	
	
	f->pattern = NULL;
	
	return 1;
	
	
}



/*
 dummy: will be the number of jugglers in one line later
 */
_fieldContent *initFieldList( int  count, int jugglers, bool addPatternToFieldList )
{
#pragma unused (jugglers)
	
	int c;
	_fieldContent *newFieldList, *p;
	int rm, rc = 0;
	
	_numberOfRunaroundJugglers = jugglers;
	
	if( fieldList != NULL ){
		getMaxFieldsOnScreen( &rm, &rc);
	}
	if( rc < 2 )
		rc = defFieldsInLine;
	
	
	newFieldList = (_fieldContent *)malloc( sizeof( _fieldContent ) * count);
	
	for( c = 0; c < count; c++ )
	{
		p = getField( newFieldList, c);
		initField( newFieldList, count, p, jugglers );
		
		if( addPatternToFieldList )
			addDefaultPattern(newFieldList, count, p );
		
		//		if( c && c % defFieldsInLine == 0 )
		if( c && c % rc == 0 )
			p->showInNewLine = true;
		
		
	}
	
	return newFieldList;
	
}

// Overwrites all patterns used in this runaround with the content of the default pattern file, whatever it is 
// Yes, this destroys all the previous content.
// Be careful whith what you are doing.
void attachDefaultPatternToAllPositions( int dummy)
{
	extern char *stdRunaroundSubPatternName;
	char d[DIR_SIZE];
	int c;
	_fieldContent *f;
	char *p;
	FILE *fp;
	getcwd( d, DIR_SIZE );
	chdir(runaroundProjectPath);
	p = readFile( stdRunaroundSubPatternName, "r" );
	if( p ){
		for( c = 0; c < totalFields; c++ )
		{
			f = getField( fieldList, c);
			fp = fopen(f->pattern, "w");
			fprintf(fp, "%s", p);
			fclose( fp );
		}
		free( p );
		
	}
	chdir( d);
}



/*
 positionen 
in allen scenen rotieren
nur in der aktuellen Reihe
nur in der aktuellen Scene
*/

int getFieldSizeOnListScreen( void )
{
	int w,h;
	int s, sw, sh;
	int fieldCount;
	int rows, columns;
	
	
	getMaxFieldsOnScreen( &rows, &columns);
	
	
	fieldCount = max( rows, columns);
	
	
	h = glutGet( GLUT_WINDOW_HEIGHT);
	w = glutGet( GLUT_WINDOW_WIDTH);
	
	sw = w / columns;
	sh = h / rows;
	
	s = min( sw, sh);
	
	if( fieldCount == 0 )
		s = min( w, h);
	else
		s = min( sw, sh);
	
	//printf("rows = %d, columns = %d, fieldCount = %d, max = %d, s = %d\n", row, maxCol, fieldCount, m, s);
	
	return s;
}


void getMaxFieldsOnScreen( int *rMax, int *cMax)
{
	int row = 1,
	col = 0,
	maxCol = 1;
	int c;
	_fieldContent *p;
	
	
	// number of rows
	for( c = 0; c < totalFields; c++ )
	{
		
		p = getField( fieldList, c );
		
		if( p ->showInNewLine ){
			row++;
			maxCol = max(maxCol, col);
			col = 1;
		}else
			col++;
	
	}
	
#if 0
	// number of comumns
	for( c = 0; c < totalFields; c++ )
	{
		
		p = getField( fieldList, c );
		
		if( p ->showInNewLine )
		{
			maxCol = max(maxCol, col);
			col = 1;
		}else
			col++;
	}
#endif
	
	maxCol = max(maxCol, col) ;//- 1;
	
	*rMax = row;
	*cMax = maxCol;
	
}




int getFieldIndexAtPosition( int x, int y)
{
	int c;
	_fieldContent *p;
	int xp, yp, wp,hp;
	int h = glutGet( GLUT_WINDOW_HEIGHT);
	
	y = h - y;
	for( c = 0; c < totalFields; c++ )
	{
		p = getField( fieldList, c );
		getScreenPositionOfField( p, &xp, &yp, &wp, &hp);
		if( xp < x && x < xp+wp && yp < y && y < yp + hp)
		{
			return c;
		}
		
		
	}
	return -1;
}

#if 0
// removes a pattern file if the runaround position is deleted by user 
// and the pattern file is not used by another position
// THIS DOES NOT WORK:
// it removes filed in the lifetime of a pattern and the user may decide not to save the edited pattern,
// leaving a corrupt runaround behind.

bool _runaroundRemovePatternIfPossible( _fieldContent *l, int n, _fieldContent *t )
{
	int c;
	char fn[MAX_PATTERN_TITLE];
	_fieldContent *p;
	
	return false;
	
	// get the name
    c = getField(l, 0)->c;
	
	// check if name is used by other position
    for( c = 0; c < n-1; c++)
    {
        p = getField( l, c);
		
		// if the name is used, don´t remove file
		if( t != p )
			if( strcmp( p->pattern, t->pattern) == 0 ){
				_myPrintf("can't remove %s\n", t->pattern );
				return false;
			}
     }
	
	sprintf(fn, "%s%s",runaroundProjectPath, t->pattern);
	removeFile( fn);
	
	return true;
}	
#endif



void _runaroundRemovePositionAtEnd( void )
{
	
    _fieldContent *s, *d, *newList, *p;
    int c, ii;

	
    if ( totalFields == 1)
        return;
	
	ii = getIndexOfSelectedField();
	
    c = getField(fieldList, 0)->c;
	s = getField(fieldList, totalFields-1);
//	_runaroundRemovePatternIfPossible( fieldList, c, s);
	
	
    newList = initFieldList( totalFields-1, c, false);
    for( c = 0; c < totalFields-1; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
    free( fieldList);
    fieldList = newList;
    totalFields--;
	
//    selectIndex = dragIndex = -1;
//    setSelectedFieldByPointer( NULL );
	
	if( selectIndex == totalFields)
		ii--;
	selectIndex = ii;
	p = getField(fieldList, ii);
	setSelectedFieldByPointer(p);
	
	
	
}


void _runaroundRemoveSelectedPosition( void )
{
	
    _fieldContent *s, *d, *newList;
    int c;
    _fieldContent *f, *t;
    int i;
	
	
	
    if ( totalFields == 1 )
        return;
	
	f = getPointerToSelectedField();
	if( f == 0 )
		return;
	
	i = getIndexFromFieldPointer(fieldList, f);
	
	
    t = getField(fieldList, 0);
	c = t->c;
//	_runaroundRemovePatternIfPossible( fieldList, c, f);
	
    newList = initFieldList( totalFields-1, c, false);
	
    for( c = 0; c < i; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
	for( c = i + 1; c < totalFields ; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c - 1);
        copyFieldToField( d,s);
    }
	
    free( fieldList);
    fieldList = newList;
    totalFields--;
	
    selectIndex = dragIndex = -1;
    setSelectedFieldByPointer( NULL );
	
	i = min( i, totalFields);
	f = getField( fieldList, i);
	setSelectedFieldByPointer( f );
	
	setRunaroundPatternChanged( true );
	
}


void _runaroundAddPosition( void )
{
    _fieldContent *s, *d, *newList, *p;
    int c, ii;
	
    c = getField(fieldList, 0)->c;
	
    newList = initFieldList( totalFields+1, c, false);
	ii = getIndexOfSelectedField();
							 
	
    for( c = 0; c < totalFields; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
    free( fieldList );
    fieldList = newList;
	totalFields++;
	
	d = getField( fieldList, totalFields - 1 );
	addDefaultPattern( fieldList, totalFields, d );
	
	p = getField(fieldList, ii);
	setSelectedFieldByPointer(p);
	setRunaroundPatternChanged( true );
	
}

void _runaroundAddBeforeSelectedPosition( void )
{
    _fieldContent *s, *d, *newList, *p;
    int c;
	_fieldContent *f;
	int i, ii;
	
	
	if( (f = getPointerToSelectedField()) == 0)
		return;
	ii = getIndexOfSelectedField();
	
	i = getIndexFromFieldPointer( fieldList, f );
	
	
    c = getField(fieldList, 0)->c;
	
    newList = initFieldList( totalFields+1, c, false);
	
    for( c = 0; c <= i; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
    for( c = i; c < totalFields; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c+1);
        copyFieldToField( d,s);
    }
	
    d = getField( fieldList, i );
    copyFieldToField( d,f);
	
    free( fieldList );
    fieldList = newList;
	totalFields++;
	
//	setSelectedFieldByPointer(NULL);
	
//	i = min( i+1, totalFields-1);
//	f = getField( fieldList, i);
//	setSelectedFieldByPointer( f );

	p = getField(fieldList, ii+1);
	setSelectedFieldByPointer(p);

	setRunaroundPatternChanged( true );
	
}



void _runaroundAddJuggler( void )
{
    _fieldContent *s, *d, *newList, *p;
    int c, i;
	
	numberOfJugglersHasChanged = true;
	i = getIndexOfSelectedField();
	
	c = getField(fieldList, 0)->c;
	
	makeDefaultPattern ( c+1 );
	
    newList = initFieldList( totalFields, c+1, false);
    for( c = 0; c < totalFields; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
    free( fieldList );
    fieldList = newList;
	
	p = getField(fieldList, i);
    setSelectedFieldByPointer(p);	

	setRunaroundPatternChanged( true );
	
}

void _runaroundRemoveJuggler( void )
{
	
    _fieldContent *s, *d, *newList, *p;
    int c, i;
	
    c = getField(fieldList, 0)->c;
	
    if (c == 1 )
        return;
	
	numberOfJugglersHasChanged = true;
	i = getIndexOfSelectedField();
	
	makeDefaultPattern( c-1 );
	
    newList = initFieldList( totalFields, c-1, false);
    for( c = 0; c < totalFields; c++)
    {
        s = getField( fieldList, c);
        d = getField( newList, c);
        copyFieldToField( d,s);
    }
	
    free( fieldList);
    fieldList = newList;
 
	p = getField(fieldList, i);
    setSelectedFieldByPointer(p);	

	setRunaroundPatternChanged( true );
}



void moveOneFieldUp( void )
{
	int rows, columns;
	int row, col;
	_fieldContent *p;
	
		
	   
	//      printf("\n\nmoving up\n");
	if( selectIndex < 0 ){
		printf("invalid selectIndex\n");
		return;
	}
	
	getMaxFieldsOnScreen( &rows, &columns);
	getIndicesOfField( p = getPointerToSelectedField(), &row, &col );
//	_myPrintf("old field: r = %3d, c = %3d\n", row, col);
	

	// see notes at declaration
	if( desiredColumn == -1)
		desiredColumn = col;
	else
		col = max( desiredColumn, col );
	
			 

	if( row == 0 )
		row = rows-1;
	else 
		row--;


	p = getFieldWithIndices( row, col );
//	_myPrintf("new field: r = %3d, c = %3d\n", row, col);
	
	setSelectedFieldByPointer( p );
	
bail:
	redrawRunaroundWindows();
}


void moveOneFieldDown( void )
{
	
	int rows, columns;
	int row, col;
	_fieldContent *p;
	
	//      printf("\n\nmoving down\n");
	if( selectIndex < 0 ){
		printf("invalid selectIndex\n");
		return;
	}
	
	getMaxFieldsOnScreen( &rows, &columns);
	getIndicesOfField( p = getPointerToSelectedField(), &row, &col );
//	_myPrintf("old field: r = %3d, c = %3d\n", row, col);

	
	// see notes at declaration
	if( desiredColumn == -1)
		desiredColumn = col;
	else
		col = max( desiredColumn, col );

	
	if( row == rows-1 )
		row = 0;
	else
		row++;
	
	p = getFieldWithIndices( row, col );
//	_myPrintf("new field: r = %3d, c = %3d\n", row, col);
	
	setSelectedFieldByPointer( p );
	
	redrawRunaroundWindows();
	
}


void moveOneFieldLeft( void )
{
	_fieldContent *p;
	int i;
	
	desiredColumn = -1;	// see notes at declaration
	
	p = getPointerToSelectedField();

	if( p == 0 )
		i = 0;
	else{		
		i = getIndexFromFieldPointer( fieldList, p );
		
		if( i == 0)
			i = totalFields;
		i--;
	}
	
	p = getField( fieldList, i);
	setSelectedFieldByPointer( p );
	
	redrawRunaroundWindows();
}


void moveOneFieldRight( void )
{
	
	_fieldContent *p;
	int i;
	
	desiredColumn = -1;	// see notes at declaration

	p = getPointerToSelectedField();

	if( p == 0 )
		i = 0;
	else{
		i = getIndexFromFieldPointer( fieldList, p );
		
		if( i + 1 < totalFields )
			i++;
		else
			i = 0;
	}
	p = getField( fieldList, i);
	setSelectedFieldByPointer( p );
	
	redrawRunaroundWindows();
	
}




int getIndexFromFieldPointer(_fieldContent *l, _fieldContent *p )
{
	int c;
	_fieldContent *t;
	
	
	// get row
	for( c = 0; c < totalFields; c++ )
	{
		
		t = getField( l, c );
		
		if( t == p )
			return c;
		
	}
	
	
	return -1;
}



void disposeFieldContent( _fieldContent *f )
{
	
	if( f == NULL )
		return;
	
	free( f->j );
	releasePatternName( f->pattern);
	
}

void disposeFieldList( _fieldContent *l, int count )
{
	int c;
	_fieldContent *p;
	
	for( c = 0; c < count; c++ )
	{
		p = getField( l, c );
		disposeFieldContent( p );
	}
	
	free( l );
	
	
}



void copyToNextField( int dummy )
{
#pragma unused (dummy )
	
	_fieldContent *s, *d;
	int i;
	int nlState;
	char *p;
	
	
	s = getPointerToSelectedField();
	if( s == NULL )
		return;
	
	
	i = getIndexFromFieldPointer( fieldList, s );
	
	if( i+1 == totalFields )
	{
		
		_runaroundAddPosition( );
		s = getField( fieldList, i);
		setSelectedFieldByPointer( s );
		
	}
	
	s = getField( fieldList, i);
	d = getField( fieldList, i+1);
	
	nlState = d->showInNewLine;
	
	p = (char *)malloc( strlen (d->pattern ) + 1 );
	sprintf(p, "%s", d->pattern );
	
	copyFieldToField( d, s);
	
	free( d->pattern );
	d->pattern = p;
	
	
	
	d->showInNewLine = nlState;
	
	setSelectedFieldByPointer( d );
	
	setRunaroundPatternChanged( true );
	
}

void startList( int dummy )
{
#pragma unused (dummy )
	
	numberOfRunaroundJugglers = _numberOfRunaroundJugglers;	
	writeAndStartRunaroundPattern();
	setWorldTitle( "runaround editor");
}

void runaroundSelectPattern( int dummy )
{
#pragma unused (dummy )
	
	_selectPattern();
}
void runaroundRemoveJuggler( int dummy )
{
#pragma unused (dummy )
	
	_runaroundRemoveJuggler();
}
void runaroundAddJuggler( int dummy )
{
#pragma unused (dummy )
	
	_runaroundAddJuggler();
}
void runaroundAddPosition( int dummy )
{
#pragma unused (dummy )
	
	_runaroundAddPosition();
}
void runaroundAddBeforeSelectedPosition( int dummy )
{
#pragma unused (dummy )
	
	_runaroundAddBeforeSelectedPosition();
}
void runaroundRemovePositionAtEnd( int dummy )
{
#pragma unused (dummy )
	
	_runaroundRemovePositionAtEnd();
}
void runaroundRemoveSelectedPosition( int dummy )
{
#pragma unused (dummy )
	
	_runaroundRemoveSelectedPosition();
}
void toggleNewLine( int dummy )
{
#pragma unused (dummy )
	
	if( selectIndex > 0)
	{
		_fieldContent *p;
		
		p = getField(fieldList, selectIndex );
		p->showInNewLine = p->showInNewLine == false;
		
	}
	setRunaroundPatternChanged( true );
	
}

void mSaveRunaroundProject( int dummy )
{
#pragma unused (dummy )
	saveRunaroundProject();
	//      saveRunaroundProjectAs();
}

void mSaveRunaroundProjectAs( int dummy )
{
#pragma unused (dummy )
	//      saveRunaroundProject();
	saveRunaroundProjectAs();
}



void closeRunaround( int dummy )
{
#pragma unused (dummy )
	
	_closeRunaround();
	
}



void shrinkToHalve( int dummy )
{
	_shrinkToHalve( );
}


void doublePattern( int dummy )
{
#pragma unused (dummy )
	_doublePattern( );
}

void runaroundShowAnimatedScene( int dummy )
{
#pragma unused (dummy )
	
	setRunaroundPatternChanged( true );
	createInfoWindow( true );
	jpPrefs.i = true;
}

// if a row contains not enough fields, 
// take the last field in the row.


_fieldContent *getFieldWithIndices( int r, int c)
{
	int row = 0, col = 0;
	int d;
	_fieldContent *t, *last;
	
	
	if( r < 0 || c < 0 )
		return NULL;
	
	t = NULL;
	
	// get row
	for( d = 0; d < totalFields; d++ )
	{
		
		last = t;
		t = getField( fieldList, d );
		
		if( t ->showInNewLine ){
			row++;
			col = 0;
		}else
			if ( d )
				col++;
		
		
		// good indices, return pointer
		if( row == r && col == c)
		{
			t = getField( fieldList, d);
			return t;
		}
		// not enough entries in row. Take the last entry instead
		// later: save the desired value. 
		// when a new row is selected, 
		// try if this row  contains enough entries 
		// reset desired value when 
		// user presses left or right 
		// or select a scene with the mouse
		// set if user presses up / down and value was invalid (i.e. reset) befor
		
		else if( row > r ){
			return last;
		}
			
	}
	
	// wrong indices
	return NULL;
}

void getScreenPositionOfField( _fieldContent *thisP, int *x, int *y, int *w, int *h)
{
	_fieldContent *p;
	int row = 0, col = 0;
	int c;
	int sh = glutGet( GLUT_WINDOW_HEIGHT);
	int size;
	
	size = getFieldSizeOnListScreen();
	
	for( c = 0; c < totalFields; c++ )
	{
		
		p = getField( fieldList, c );
		
		if( p ->showInNewLine )
		{
			row++;
			col = 0;
		}else
			col++;
		
		if( p == thisP )
			break;
	}
#if 01
	if( row == 0 )
        col--;
#endif
	
	*x = col * size;
	*y = sh - (row+1)* size;
	*w = *h = size;
	
}

void getIndicesOfField( _fieldContent *p, int *r, int *c)
{
	int row = 0,
	col = -1;
	int d;
	_fieldContent *t;
	
	
	// get row
	for( d = 0; d < totalFields; d++ ){
		
		t = getField( fieldList, d );
		
		if( t ->showInNewLine ){
			col = 0;
			row++;
		}else	
			col++;
		
		if( p == t)
			break;
	}
	
#if 0	
	// get colmum
	for( d = 0; d < totalFields; d++ ){
		
		t = getField( fieldList, d );
		if( t->showInNewLine )
			col = 0;
		else
			col++;
		
		if( p == t )
			break;
		
	}
#endif
	
#if 0
	if( row == 0 )
        col--;
#endif
	
	*r = row;
	*c = col;
	
}








void newProject( int dummy )
{
#pragma unused (dummy )
	
	newRunaroundProject();
	return;
}





#ifdef OPEN_GL_ROUTINES
void drawFilledRect( int x, int y, int w, int h)
{
	glBegin(GL_QUADS);
	glVertex2f( x+1,        y+1);
	glVertex2f( x+w-1,      y+1);
	glVertex2f( x+w-1,      y+h-1 );
	glVertex2f( x+1,        y+h-1 );
	glEnd();
}

#else

void drawFilledRect( int x, int y, int w, int h)
{
	drawQuad( x+1,  y+1, x+w-1,     y+1, x+w-1,     y+h-1, x+1,     y+h-1 );
	
}
#endif

#ifdef OPEN_GL_ROUTINES
void drawEmptyRect( int x, int y, int w, int h)
{
	
	drawOneLine2D(x,y, x+w,y);
	drawOneLine2D(x+w,y, x+w,y+h);
	drawOneLine2D(x+w,y+h, x,y+h);
	drawOneLine2D(x,y+h, x,y);
	
}
#else
void drawEmptyRect( int x, int y, int w, int h)
{
	
	drawOneLine(x,y, x+w,y);
	drawOneLine(x+w,y, x+w,y+h);
	drawOneLine(x+w,y+h, x,y+h);
	drawOneLine(x,y+h, x,y);
	
}
#endif

#if 01
bool checkForRunaroundDelayWalkChanged( void )
{
	struct tm* tNow;				// create a time structure
	static tm tLast;
	bool ret = false;
	struct stat attrib;					// create a file attribute structure
	char p[MAX_FULL_FILE_NAME];
	
	sprintf(p, "%s%s", runaroundProjectPath, RunaroundDelayFile);	
	
	if( strlen( p) == 0 )
		return false;
	if( !fexists( p ))
		return false;

	stat(p, &attrib);
	tNow = gmtime(&(attrib.st_mtime));	
	if(compareTime( &tLast, tNow) ){
		ret = true;
	}
	tLast = *tNow;	
	return ret;
	
	
}


void createDefaultRunaroundDelayFile( void )
{
	FILE *fp;
	char p[MAX_FULL_FILE_NAME];
	
	sprintf(p, "%s%s", runaroundProjectPath, RunaroundDelayFile);	
	fp = fopen( p, "w");
	fprintf(fp, "0");
	fclose( fp );
	
	
}


float readRunaroundDelayFileValue( void )
{
	float f = 0;
	FILE *fp;
	char p[MAX_FULL_FILE_NAME];
	
	sprintf(p, "%s%s", runaroundProjectPath, RunaroundDelayFile);	
	if( (fp = fopen( p, "r")) != NULL ){
		fscanf(fp, "%f", &f);
		fclose( fp );
	}
	
	return f;
}
#endif

void listIdle( void )
{
#if 01
	if( runaroundEditorHasOpenWindows()){
		if( checkForRunaroundDelayWalkChanged(  ) ){
			startList( 0 );
			
		}
	}
#endif
}
