/*
 *  info.cpp
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
#define DEBUG_ON 01


#include "systemswitch.h"
#include "graphics.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "mydefs.h"
#include "globals.h"
#include "myPrintf.h"
#include "languageSupport.h"

#include "infoClass.h"

#if MAC_CLASSIC_VERSION
#include "macWin.h"
#endif
#include "prefDef.h"
#include "info.h"
#include "grafik.h"
#include "siteswap.h"
#include "world.h"
#include "editor.h"
#include "cameraControl.h"
#include "animationWin.h"
#include "parseSiteswap.h"
#include "vtypes.h"
#include "prefs.h"
#include "mem.h"
#include "openglmenu.h"
#include "stringtoscreen.h"
#include "inputWindow.h"
#include "listWindow.h"
#include "myutil.h"

#if SdMakeMovie
#include "movie.h"
#endif



#define         PatternTextSize         .8f
#define         InfoTextSize            .8f
#define         MovieTextSize           .8f
#define         EventTextSize           .8f
#define         FrameRateTextSize       .8f
#define         PatternTimeTextSize     .8f
#define         DebugTextSize           .8f
#define         InfoStringTextSize      .8f
#define			ObjectColorTextSize		.8f


bool 	showPatternTimeFlag = false;
bool	showObjectColorFlag = false;
bool	showRunaroundInfoFlag = false;

void	showRunaroundInfo( void );
void	showPatternTime( void );
void	showDistanceInfo( void );
void	showObjectColorInfo( void );
void    infoMenuSelection( int r );
void    infoKey(unsigned char theKey, int x, int y);
void    moreFrames( int dummy );
void    lessFrames( int dummy );
void    infoQuit( int dummy );

void    infoWinFunc( int dummy ),
quitFunc( int dummy);

//#define SWITCH_MENU_NAME "switches"
int movieMenu;

#define MOVIE_MENU_NAME "movie settings"

int				infoWin = 0;
_menuList       *infoMenuList;


char			*externBuf;

sysColor		fadeColor( sysColor *in, wwwBoolean fadeThis, float d1 );
//int			numberOfAdditionalEvents = 5;
//float			fadeFactor = 1.f;

//float			*debugMatrix44 = NULL;


// positions in the info-window
#define	POST_EVENT_INDEX (JUGGLER_MAX*2)
int smartAtLine( int l);

//enum {  eventLine,  errorLine = eventLine+POST_EVENT_INDEX, timeLine, patternLine , runaroundPatternLine,infoLine, objColorLine, speedLine, movieLine, distanceLineCollision, distanceLineShortestDistance, debugLine00=30, debugLine01, debugLine10, debugLine11};

enum {  errorLine, runaroundPatternLine, infoLine,   movieLine= infoLine + 2, objColorLine=  movieLine+2, distanceLineCollision=objColorLine + 2, timeLine = distanceLineCollision+2, animationFramerateLine, patternLine, eventLine, debugLine00=30, debugLine01, debugLine10, debugLine11};


// draw the information at the correct position:
// only used jugglers, no placeholders
int smartAtLine( int i )
{
	if( i > POST_EVENT_INDEX )
		i = i - POST_EVENT_INDEX + jugglerCount;
	
	return atLine( i );	
}


static char     buf[10000],
infoString[1000];

int            theGlutModifiers = 0;


_object           *eventInfoList[JUGGLER_MAX];

sysColor        nextEventColor;
wwwBoolean      col = false;
extern          wwwBoolean hasSync, hasAsync;

wwwBoolean      redrawInfoWin = true;

void            showMovieInfo( void );
void            showObjectColorInfo( void );
void            showInfos( void );
void            showSyntaxErrorInfo( void );
void            showSiteswapFaultInfo( void );
void            showPatternInfo( void );
void            showEvent( sse *e);
void            showInfoString( void );
void            showDebugInfo( void );
void            infoMenuInit(void);
void            info_menu_select( int id);
void            infoSpecialKey(int theKey, int x, int y);


void redisplayInfoWindow( void ) {
	
    if (infoWin) {
        glutSetWindow(infoWin);
		
        //myPrintf("info win dimensions = %d %d %d %d\n", jpPrefs.infoWinSizeX, jpPrefs.infoWinSizeY, jpPrefs.infoWinSizeW, jpPrefs.infoWinSizeH);
		
        glutPostRedisplay();
    }
}


void showPatternInfo( void ) {
    int c;
    char *p;
	
    if( !jpPrefs.p )
        return;
	
    c  = 0;
    p = buf;
	
    c += sprintf( p + c, "length: %d, objects: %d (%-3.2f)",
				 momentCount, ballCount, siteswapCount / momentCount);
	
    stringToScreen(p,centered, 0,smartAtLine(patternLine), PatternTextSize, &black);
}


#ifdef __MOVIE__
void showMovieInfo( void ) {
	
	
    long fim = myMovie->getFramesInMovie();
	
    if( !jpPrefs.movieInfo )
        return;
	
    sprintf(buf,"movie: %ld fps, %ld frames, %f sec", jpPrefs.movieFrameRate, fim ,(float)(fim)/ jpPrefs.movieFrameRate );
    stringToScreen( buf, centered, 0,smartAtLine(movieLine), MovieTextSize, &black);
}

void moreFrames( int dummy ) {
//#pragma unused (dummy )
    jpPrefs.movieFrameRate ++;
}

void lessFrames( int dummy ) {
//#pragma unused (dummy )
    jpPrefs.movieFrameRate--;
    jpPrefs.movieFrameRate = max( jpPrefs.movieFrameRate, 3);
}

void infoQuit( int dummy ) {
//#pragma unused (dummy )
    exit( 0 );
}

#endif

void showFrameRate( void ) {
    if( !jpPrefs.f )
        return;
	
    sprintf(buf,"frames/sec = %d, throws/sec = %2.3f",theFrameRate, jpPrefs.animationSpeed); 
	stringToScreen( buf, centered, 0,smartAtLine(animationFramerateLine), FrameRateTextSize, &black);
}


void showSyntaxErrorInfo( void ) {
	
    sprintf(buf,"Syntax error at red colored text");
    
    stringToScreen( buf, centered, 0,smartAtLine(animationFramerateLine), 1.2, &red);
}

void showSiteswapFaultInfo( void ) {
	
    sprintf(buf,"Not a valid siteswap. Animation may look strange");    
    
    stringToScreen( buf, centered, 0,smartAtLine(errorLine), 1.2, &red);
}

void showRunaroundInfo( void ){
	if(listID == 0 || !showRunaroundInfoFlag)
		return;
	
    sprintf(buf,"selected runaround scene: %3d, associated pattern: %s", selectedRunaroundIndex, selectedRunaroundPattern);
	stringToScreen( buf, centered, 0,smartAtLine(runaroundPatternLine), PatternTimeTextSize, &black);
}
	
	

void showPatternTime( void ) {
    float t;
	
    if( !showPatternTimeFlag )
        return;
	
    t = aTime;
    while( t >= momentCount )
        t -= momentCount;
	
    sprintf(buf,"time = %2.3f",t);
    stringToScreen( buf, centered, 0,smartAtLine(timeLine), PatternTimeTextSize, &black);
}


void showObjectColorInfo( void ) {

    if( !showObjectColorFlag )
        return;
	
	
    sprintf(buf,"objectColor: %s", objInfoStr[jpPrefs.objectColorMode]);
    stringToScreen( buf, centered, 0,smartAtLine(objColorLine), ObjectColorTextSize, &black);
}

void showDistanceInfo( ) {
	
    if( !jpPrefs.emphaseShortDistanceFlag )
        return;
	
 /*
  sprintf(buf,"collision warning at %-2.3f cm",jpPrefs.minObjectDistance);
    stringToScreen( buf, centered, 0,smartAtLine(distanceLineCollision), PatternTimeTextSize, &black);
    sprintf(buf,"shortest distance is %-2.3f cm", minDistanceValue);
    stringToScreen( buf, centered, 0,smartAtLine(distanceLineShortestDistance), PatternTimeTextSize, &black);
  */
	if( minDistanceValue >= 0 )
		sprintf(buf,"short distance warning: less than %3.0f cm - shortest distance is %3.0f cm",jpPrefs.minObjectDistance, minDistanceValue);
	else {
		sprintf(buf,"short distance warning: less than %3.0f cm - shortest distance is undefined",jpPrefs.minObjectDistance);
		
	}

    stringToScreen( buf, centered, 0,smartAtLine(distanceLineCollision), PatternTimeTextSize, &black);
}


void showDebugInfo( void ) {
#if  DEBUG_ON

	int	   y,l = debugLine00 + 10;
	char  *s;
	GLdouble matrix[16];
	glGetDoublev(GL_PROJECTION, matrix);	
	
	s = &buf[0];
	if( matrix){	
		for (y = 0; y < 4; y++){	
			sprintf(s,"(% f/ % f/ % f/ % f)", matrix[0+4*y],matrix[1+4*y],matrix[2+4*y],matrix[3+4*y] );
			stringToScreen( s, centered, 0, smartAtLine(l++), DebugTextSize, &black);
		}
	}

	
	sprintf(buf,"debugPoint = (% f/ % f/ % f)",debugPoint.x, debugPoint.y, debugPoint.z);
	stringToScreen( buf, centered, 0, smartAtLine(debugLine00), DebugTextSize, &black);
	sprintf(buf,"debugValue1 = %f",debugValue1);
	stringToScreen( buf, centered, 0,smartAtLine(debugLine01), DebugTextSize, &black);
	sprintf(buf,"debugValue0 = %f",debugValue0);
	stringToScreen( buf, centered, 0,smartAtLine(debugLine10), DebugTextSize, &black);
	sprintf(buf,"debugPoint2 = (%.4f/ %.4f/ %.4f)",debugPoint2.x, debugPoint2.y, debugPoint2.z);
	stringToScreen( buf, centered, 0,smartAtLine(debugLine11), DebugTextSize, &black);	
#endif
	
}

// Was geschieht in einem Event?
void showEvent( sse *e) {
#ifdef MAC_CLASSIC_VERSION
//#pragma unused( e )
#endif
	
    char    *p;
    int     c;
    sse     *se, *pe, *me, *ee;
    wwwBoolean      sim, mux;
    int     j, d1;
    sysColor theColor;
    wwwBoolean otherHand;
    wwwBoolean fadeThis;
    int xPosition = 5;
    int maxOut = 0;
    int charsOut = 0;
    sysColor bracketColor;
    float weight;
	
    if( !jpPrefs.e )
        return;
    if( validPattern == false ) {
        return;
    }
	
    //_myPrintf("ss = %f, sim = %lx\n", e->ss, e->sim);
	
    p = buf;
	
	
    /*
	 von e an
	 alle events durchsuchen und ausgeben, wenn:
	 derselben Jongleur
	 topLevelEvent == true
	 maximal n folge-events, wg
	 besichtlichkeit der Ausgabe
	 Speicherplatz (wird nicht auf berlauf getestet)
	 */
    for( d1 = 0; d1 < 1 + jpPrefs.numberOfAdditionalEvents; d1++) {
        sse *e;
		
        xPosition  += maxOut;
        maxOut = 0;
		
		
        for( j = 0; j < jugglerCount;j++) {
            eventListEntry *el;
            int simOffset = 0;
			
            charsOut = 0;
			
            c = 0;
            *p = '\0';
			
            el = getTopLevelEventForJuggler( j, topLevelEventNumber + d1 );
			
            if( el == NULL ) {
                c+= sprintf(p+c, "-");
                stringToScreen( p, left, xPosition , smartAtLine(eventLine + jugglerCount - j), EventTextSize, &black);
                charsOut += c;
                goto bail;
				
            }
			
            e = el->p;
			
            if( e == NULL ) {
                continue;
            }
			
            pe  = e;
			
            sim = (pe->sim != NULL  ) ;                                     // Simultan
            se = pe;
			
            if(     d1 == 0 || (0 && sim && d1 == 1))
                fadeThis = false;
            else
                fadeThis = true;
			
            bracketColor = fadeColor( &black, fadeThis, d1);
			
            if( sim ) {
                c += sprintf( p+c, "(" );
                stringToScreen( p, left, xPosition , smartAtLine(eventLine + jugglerCount - j), EventTextSize, &bracketColor);
                simOffset += 1;
				
                otherHand = true;
            } else
                otherHand = false;
			
            do {
                wwwBoolean ci = (el->hnd == leftHand);
				
                c = 0;
				
                if( otherHand ) {
                    ci = toggle(ci );
                    otherHand = false;
                }
				
                if( ci )
                    //                              if( se->handUsedThisTime == leftHand)
                    theColor = eventColor1;
                else
                    theColor = eventColor2;
				
                theColor = fadeColor( &theColor, fadeThis, d1);
				
                if( (mux = (se->mux != NULL))==true )   // Multiplex
                    c += sprintf( p+c, "[" );
                me = se;
                do                                                                              // Einzel
                {
                    ee = me;
                    if( ee->ss == SS_EMPTY ) {
                        c += sprintf( p+c,"-");
                        goto bailSingle;
                    }
                    {
                        _object *o;
						
                        o = (_object *)ee->o;
                        if( o ) {
                            if( obeyDelay )
                                weight = ee->ss  - ee->resultingDelay;
                            else
                                weight = ee->ss;
							
                            if( weight - (int)weight == 0.)
                                c += sprintf( p+c,"%d",(int)weight);            // ss-Wert int
                            else
                                c += sprintf( p+c,"%1.2f",weight);              // float
                        } else
                            c += sprintf( p+c,"%d",0);              // float
                    }
					
                    if( e->isMhnHurried )
                        c += sprintf( p+c,"*");
					
					
                    if( ee->sameHand == true )                              // dieselbe Hand
                        c += sprintf( p+c,"h");
					
                    if(ee->who != ee->to)                                   // pass
					{       if( jugglerCount == 2 )
						c += sprintf( p+c,"%c", 'p');
					else
						c += sprintf( p+c,"%c%d", ':', ee->to+1 );
                    }
					
                    {
                        wwwBoolean cross = false;
                        int h1, h2;
						
                        h1 = getJuggler(ee->who)->start;
                        h2 = getJuggler(ee->to)->start;
						
                        cross = (int)weight % 2;
						
                        cross ^= ee->cross;
						
                        if (h1 != h2)
							
                            toggle( cross);
						
                        if( cross )     // cross
                            c += sprintf( p+c,"x");
                    }
					
                    if( ee->bounce )
                        c += sprintf( p+c,"%c", 'b' );
					
                } while( (me = me->mux) != NULL );                       // Multiplex
			bailSingle:
                if( mux )
                    c += sprintf( p+c, "]" );
				
                if( se->sim != NULL ) {
                    c += sprintf( p+c, "," );
                    ci = toggle(ci);
                }
				
				stringToScreen( p, left, xPosition + simOffset, smartAtLine(eventLine + jugglerCount - j), EventTextSize, &theColor);
				
                charsOut += c;
                simOffset = c+1;
            } while((se = se->sim) != NULL  );                                       // Simultan-Ereignisse
            c = 0;
            *p = '\0';
			
            if( pe->sim ) {
                c += sprintf( p+c, ")");
                stringToScreen( p, left, xPosition + charsOut + 1, smartAtLine(eventLine + jugglerCount - j), EventTextSize, &bracketColor);
                charsOut += c;
            }
			
		bail:
            charsOut += 3;							// add some extra spaces after last output
            maxOut = max( charsOut, maxOut);        // get longest entry to align the upcomming throws
        }       // end for( jugglers)
	}       // end for( eventNumbers)
}

sysColor fadeColor( sysColor *in, wwwBoolean fadeThis, float d1 ) {
    sysColor fadeOffset;
    sysColor theColor = *in;
    if( jpPrefs.fadeEvents ) {
		
        if( fadeThis ) {
            fadeOffset.x = fadeOffset.y = fadeOffset.z = d1;
            theColor = theColor + fadeOffset;
            theColor = theColor * ( 1./(d1+1)*jpPrefs.fadeFactor);
        }
    } else {
        if( fadeThis ) {
            fadeOffset.x = fadeOffset.y = fadeOffset.z = 1;
            theColor = theColor + fadeOffset;
            theColor = theColor * .5;
        }
    }
	
    return theColor;
}




/* ---------------- info ------------------- */



void resetInfo( void ) {
    int c;
    for( c = 0; c < JUGGLER_MAX; c++ )
        eventInfoList[c] = NULL;			 
	debugMatrix44 = NULL;
}

void setNextObjectInfo( _object *o) {
	
    eventInfoList[o->thrower.juggler->getJugglerNr()] = o;
}



void nextEventInfo( void ) {
    col = (col == false);
    if(col)
        nextEventColor = eventColor1;
    else
        nextEventColor = eventColor2;
}

void resetEventInfo( void ) {
    col = false;
    nextEventColor = eventColor1;
	
}

void setInfoString( char *i ) {
    if( *i != NIS ) {
        sprintf( infoString,"%s",i );
    } else {
        strcpy( infoString,"");//no text defined" );
    }
}

void showInfoString( void ) {
	
	char *p;
	
    if( !jpPrefs.i )
        return;
	
    sprintf( infoString, "%s", infoString );
    stringToScreen( infoString, centered, 0, smartAtLine(infoLine),InfoStringTextSize,&black);// + jugglerCount),InfoStringTextSize,&black);
	
	if( listID != 0){
		GLint id;
		
		// if the string contains infos about a runaround, extract the scene index.
		if( strstr( infoString, AnimatedRunaroundSceneInfoString) != NULL ){
			
			 p = infoString + strlen( AnimatedRunaroundSceneInfoString );
			if( *p && !isdigit(*p))
			   p++;
			   
			animatedRunaroundSceneIndex = atoi( p );
			id = glutGetWindow();
			glutSetWindow(listID);
			glutPostRedisplay();
			glutSetWindow(id);
		}	   
	}
}


void clearInfoString( void ) {
    infoString[0] = '\0';
	
    RedisplayAppWindows();
}

void drawInfo( void ) {
	
    if(!infoWin )
        return;
	
    glutSetWindow( infoWin);
    getWindowPosition( &jpPrefs.infoWinSizeX, &jpPrefs.infoWinSizeY);
	
    glClearColor(1, 1, 1, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    showInfos();
    if( glutGet( GLUT_WINDOW_DOUBLEBUFFER ) )
        glutSwapBuffers();
    else
        glFlush();
}

void showInfos( void ) {
    //myPrintf("drawing infos\n");
    if( !validPattern )
    {   showSyntaxErrorInfo();
        return;
    }    
	
    if( 0 && siteswapError )
        showSiteswapFaultInfo();
    showPatternTime();
	showRunaroundInfo();
	showObjectColorInfo();
    showDistanceInfo();
    showEvent( eventPointer );
    showFrameRate();
    showInfoString();
    showPatternInfo( );
    if( !validPattern )
		showSyntaxErrorInfo();
#ifdef __MOVIE__
	
    showMovieInfo( );
#endif
	
    showDebugInfo();
}


void reshapeInfo(int width, int height) {
	
    if(!infoWin )
        return;
	
    glutSetWindow( infoWin);
    jpPrefs.infoWinSizeW = width;
    jpPrefs.infoWinSizeH = height;
    glViewport(0, 0, width, height);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,1.0 *  (GLfloat)width/(GLfloat)height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    RedisplayAppWindows();
	
    if (editWin) {
        glutSetWindow(editWin);
        glutPostRedisplay();
    }
	
}
/*
 void infoMenuInit( void )
 {
 glut_menu[infoID] = glutCreateMenu( info_menu_select );
 glutAddMenuEntry(" event ", toggleEvent);
 glutAddMenuEntry(" pattern ", togglePattern);
 glutAddMenuEntry(" infoString ", toggleInfo);
 glutAddMenuEntry(" animation ", toggleFramerate);
 #if SdMakeMovie
 glutAddMenuEntry(" movie ", toggleMovieInfo);
 #endif
 glutAddMenuEntry(" hide window ", hideID);
 glutAddMenuEntry(" quit ", quitSelect);
 
 #if MAC_CLASSIC_VERSION && 0
 glutAttachMenuName(GLUT_RIGHT_BUTTON, " Info ");
 #else
 glutAttachMenu(GLUT_RIGHT_BUTTON);
 #endif
 }
 
 
 void info_menu_select( int id)
 {
 
 switch( id )
 {
 // info
 case hideID:
 createInfoWindow(false);
 break;
 case toggleMovieInfo:
 toggle( jpPrefs.movieInfo );
 break;
 case toggleFramerate:
 toggle( jpPrefs.f );
 break;
 case togglePattern:
 toggle( jpPrefs.p );
 break;
 case toggleInfo:
 toggle( jpPrefs.i );
 break;
 case toggleEvent:
 toggle( jpPrefs.e );
 break;
 case quitSelect:
 exit( 0 );
 break;
 }
 }
 */
// ----------------- special key handling -------------------
void infoSpecialKey(int theKey, int x, int y) {
	
    switch( theKey) {
			/*              // close win key
			 case GLUT_KEY_F4:
			 if( theModifiers & GLUT_ACTIVE_ALT)
			 {       createInfoWindow(false);
			 return;
			 }
			 break;
			 */
			// pass to animation window
		default:
			worldSpecialKey( theKey, x,y);
			break;
    }
	
}


void createInfoWindow( wwwBoolean mode ) {
    printf("\ncreating information window\n");
	
    jpPrefs.showInfoWindow = mode;
	
    if( mode ) {
        if( !infoWin ) {
            glutInitWindowPosition(jpPrefs.infoWinSizeX, jpPrefs.infoWinSizeY);
            glutInitWindowSize(jpPrefs.infoWinSizeW, jpPrefs.infoWinSizeH);
            infoWin = glutCreateWindow("info");
			
            glutDisplayFunc( drawInfo );
            glutReshapeFunc(reshapeInfo);
            glutMouseFunc(NULL);
            glutMotionFunc(NULL);
            glutVisibilityFunc(NULL);
            glutKeyboardFunc(infoKey);      // one for all
            glutSpecialFunc(infoSpecialKey);
			
            //                      infoMenuInit(); // init menus
            initMenu(infoMenuList, true);
			
            recalcFrameRate();
        }
    } else {
        if( infoWin ) {
#if MAC_CLASSIC_VERSION
            fixInfoWindowPositionForMac();
#endif
			
            glutDestroyWindow( infoWin );
            infoWin = 0;
        }
    }
    _myPrintf("infoWinID = %d\n", infoWin);
}

void infoMenuSelection( int id ) {
    _menuSelect( infoMenuList, id );
	
}


void infoKey(unsigned char theKey, int x, int y) {
//#pragma unused (x,y)
	if( !_menuHotkeySelect( infoMenuList, theKey))
		_menuHotkeySelect(worldMenuList, theKey);
	
	
	
}

void infoWinFunc( int dummy) {
//#pragma unused (dummy)
    createInfoWindow(false);
}

void quitFunc( int dummy) {
//#pragma unused (dummy)
    exit(0);
}


info::info( void ) {
	
    _menuList       *l,
    _infoMenuList[] = {
		{"information window",									menuTitle},
		{"mainmenu",                                            newMenu,                        0,      NULL,           infoMenuSelection},
		{languageStrings[languageInfoThrows],                   menuEntry,                      'e',    NULL,           NULL,           &jpPrefs.e},
		{languageStrings[languageInfoPattern],                  menuEntry,                      'p',    NULL,           NULL,           &jpPrefs.p },
		{languageStrings[languageInfoFramerate],                menuEntry,                      ' ',    NULL,           NULL,           &jpPrefs.f },
		{languageStrings[languageInfoTime],                     menuEntry,                      't',    NULL,           NULL,           &showPatternTimeFlag },
		{languageStrings[languageInfoShowCollisionWarning],		menuEntry,                      'P',    NULL,           NULL,           &jpPrefs.emphaseShortDistanceFlag},
		{languageStrings[languageObjectColorText],              menuEntry,                      'C',    NULL,           NULL,           &showObjectColorFlag},
		{languageStrings[languageInfoMovieShowInfo],            menuEntry,                      'm',    NULL,           NULL,           &jpPrefs.movieInfo },
		{languageStrings[languageInfoText],                     menuEntry,                      'T',    NULL,           NULL,           &jpPrefs.i },
		{languageStrings[languageInfoRunaround],				menuEntry,                      'R',    NULL,           NULL,           &showRunaroundInfoFlag },

		{languageStrings[languageInfohideWindow],               menuEntry,                      'w',    NULL,           infoWinFunc},
		{languageStrings[languageInfoShowAnimation],            menuEntry,                      '=',    NULL,           toggleAnimationActive},
		{languageStrings[languageAllMenusOpenDoc],				menuEntry,                      '?',	NULL,			openDocFunc},
		{languageStrings[languageAllMenusQuit],					menuEntry,                      'q',    NULL,           animationQuit},
		{NULL,                                                  menuListTerminator,             0,      NULL,           0}     // last entry
	};
	
	
    // alloc memory for menuList
    l = (_menuList *)malloc( sizeof( _infoMenuList ));
    // copy content
    memcpy( l, _infoMenuList, sizeof( _infoMenuList ));
	
    infoMenuList = l;
}


info::~info() {}


void initInfo( void ) {
	
    new info();
}

