/*
 *  styleScanner.cpp
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

//#define StyleScannerInfos

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
/*
 scans a file for style-descriptions
 uses scanner.c and parsePoint() from parseSiteswap.c
 */
#include "globals.h"
#include "applicationFolders.h"
#include "scanner.h"
#include "styleScanner.h"
#include "parseSiteswap.h"
#include "fileIO.h"
#include "myutil.h"
#include "systemswitch.h"

//#define FILE_NAME     "styles.txt"

#if 01
#define USE_MY_STRCMP
#endif

#ifdef USE_MY_STRCMP
// (defined( MAC_CLASSIC_VERSION) || defined (UNIX_VERSION) || defined(USE_MY_STRCMP))

int             strnicmp( const char *p1, const char *p2, unsigned long c);
int             stricmp( const char *p1, const char *p2);
#endif



#define spinFaktorCode 'f'
#define lineCode '\n'
const char wordStart[] = " \t";                 // Skip vor Beginn von Worten
const char wordEnd[] = ",;.:=\n\r\t ";  // Beendet Worte
extern const char pointStart ;                  // punkt einlesen

#undef EOF
#define EOF '\0'

#define styleComment  '!'
#define styleCommand  '#'
#define styleEOScanner  '@'
const char styleEOThrow = ',';
const char styleEOCatch = ';';

#define NAME_KEY        "Name"  // Es folgt eine Stil-Definition
#define INFO_KEY        "Info"  // Trennzeile fr Stil-Menu
#define EOCMD ':'       // Mu am Ende von Kommandos stehen

#define KEY_LENGTH (sizeof(NAME_KEY)-1)

size_t  mySize, myP;
char *tp = NULL;

styleEl defMove ;

static style    *read1Info(style *p );
static style    *read1Style(style *first, style *p );
static wwwBoolean       read1Move(styleEl *t );

static size_t   readCommand(char *str, size_t s);
//static char           readNoSpace();
static char     peekStyleChar();
static char             readStyleChar();




// Eine Zeile aus einem File einlesen, max s zeichen,
// Leerzeichen bleiben erhalten!
// Wenn str != NULL, in str speichern
// str < 0 -> bis zeilenEnde
// Ergebnis Anzahl der gelesenen Zeichen
//                      (string in str, terminiert);
size_t readLine(char *str, size_t s)
{
	size_t l = 0;
	char c;
	wwwBoolean t;
	
	t = st.skip;
	st.skip = false;
	
	while( s && (c = readStyleChar()) != EOF && c != lineCode)
	{
		if( str )
			*str++ = c;
		s--;
		l++;
	}
	
	*str = '\0';    // Terminieren
	
	
	st.skip = t;
	return l;
}

// Ein Wort aus einem File einlesen, max s zeichen,
// Wenn str != NULL, in str speichern
// str < 0 -> bis zeilenEnde
// Ergebnis Anzahl der gelesenen Zeichen
//                      (string in str, terminiert);
size_t readWord(char *str, size_t s)
{
	size_t l = 0;
	char c;
	wwwBoolean t;
	
	t = st.skip;
	st.skip = false;
	
	// Alle fhrenden Leerzeichen berspringen
	while( s && strchr( wordStart, peekStyleChar() ))
		readStyleChar();
	
	while( s && (c = peekStyleChar()) != EOF && !strchr( wordEnd, c ) )
	{       readStyleChar();
		if( str )
			*str++ = c;
		s--;
		l++;
	}
	
	
	*str = '\0';    // Terminieren
	st.skip = t;
	
	return l;
}

// Words ending with a :
size_t readCommand(char *str, size_t s)
{
	size_t l = 0;
	char c;
	wwwBoolean t;
	
	t = st.skip;
	st.skip = false;
	
	// Alle fhrenden Leerzeichen berspringen
	while( s && strchr( wordStart, peekStyleChar() ))
		readStyleChar();
	
	while( s && (c = peekStyleChar()) != EOF && !strchr( wordEnd, c ) && c!= EOCMD)
	{       readStyleChar();
		if( str )
			*str++ = c;
		s--;
		l++;
	}
	
	if( c == EOCMD )
		readStyleChar();
	
	
	*str = '\0';    // Terminieren
	st.skip = t;
	
	return l;
}

#if 0

char readStyleChar( )
{
	char c;
	
	if( myP == mySize )
		return EOF;
	
	c = *tp++;
	myP++;
	
	return c;
}

void unreadStyleChar( char c )
{
#pragma unused ( c )
	
	if( myP )
	{       myP--;
		tp--;
	}
}
char peekStyleChar()
{
	return *tp;
}
#else

char readStyleChar()
{
	return getChar( );
}
char peekStyleChar()
{
	return peekChar();
}

#endif

// Alle Muster einlesen
void readStyles(style *first )
{
	
	char buffer[256];
	style *pat;
	char c;
	
	pat = first;
	
	
	st.skip = true;
	while( (c = readStyleChar()) != EOF )
	{
		switch( c )
		{       case styleComment:
				readLine( NULL, ~0 ); // File, buffer, count
				break;
			case styleCommand:
				readCommand(buffer, 255 );
				if(stricmp( buffer, NAME_KEY ) == 0 )
				{       if( (pat = read1Style(first, pat ) ) == NULL )
				{       beep( );
					return;
				}
				}else
					if(stricmp( buffer, INFO_KEY ) == 0 )
					{       if( (pat = read1Info( pat )) == NULL )
					{       beep( );
						return;
					}
					}
#ifdef StyleScannerInfos
					else
						printf("Unbekanntes Schlsselwort bei Position %08l\n", st.readPos );
#endif
				break;
			case styleEOScanner:    // Wir sind fertig...
				return;
				break;
			default:
#ifdef StyleScannerInfos
				printf("Falsches Zeichen bei Position %l\n", st.readPos );
#endif
				beep( );
				return;
				break;
		}
	}
	
}

// ein Muster einlesen
// Ausgabe: Zeiger auf neues Muster
style *read1Style(style *first, style *p )
{
	style *t;
	
	if( (t = (style *)malloc(sizeof( style)))==NULL)
	{
#ifdef StyleScannerInfos
		printf("out of memory while scanning styles\n");
#endif
		return NULL;                                            // kein Speicher mehr frei, weg
	}
	
	t->first = NULL;                                                // initialisieren
	t->next = NULL;
	readWord(t->name, 255 );
	
	if( readMoves( t ) == false )           // einlesen
	{       free( t );                                              // klappet nicht
		return  NULL;
	}
	p->next = t;
	first->count++;                                         // Zhlen
	
	return t;
}

style *read1Info( style *p )
{
	
	if( (p -> next = (style *)malloc(sizeof( style)))==NULL)
	{
#ifdef StyleScannerInfos
		printf("out of memory while scanning styles\n");
#endif
		return NULL;                                            // kein Speicher mehr frei, weg
	}
	
	p = p->next;                                            // einlesen
	p->next = NULL;
	
	readLine(p->name, 255 );
	
	
	p->first = NULL;                                                // initialisieren
	
	return p;
}
/*
 Werte eines Musters einlesen
 Punkt
 ,
 ;
 */
wwwBoolean readMoves(style *f )
{
	
	styleEl *t, *o;
	char r;
	char c;
	
	// Teilmuster lesen
	do
	{
		
		if( ( t = (styleEl *)malloc(sizeof( styleEl)))==NULL    ) // Speicher anfragen
		{
#ifdef StyleScannerInfos
			printf("out of memory while scanning movement\n");
#endif
			return false;
		}else
		{
			if( !f->first )                 // erster Eintrag: verankern
			{       o = f->first = t;
			}
			else
			{       o->next = t;            // weitere Eintrge: mit altem verknpfen
				o = t;                          // als neuen alten merken
			}
			
			t->next = f->first;             // den Kreis schlieen
		}
		
		r = read1Move(t);       // einlesen
		
		
	}while( r && (c = peekStyleChar()) != EOF && c != styleCommand);        // Bis Fehler oder neues Kommando folgt
	
	return r;       // Fehlerzustand zurck...
}

/*
 Ein Teilmuster einlesen.
 Rckgabe <=> letztes gelesenes Zeichen
 */
wwwBoolean read1Move(styleEl *t )
{
	wwwBoolean ret = true;
	dPunkt p;
	char c;
	wwwBoolean s;
	t->v = defVPosition;    // Default eingeben
	t->n = defNPosition;
	
	s = st.skip;
	st.skip = true;
	
	/*
	 t->f = 1;
	 if( peekChar() == spinFaktorCode )      // Punkt lesen
	 {       getChar();
	 t->f = getSignedFloat( );
	 }
	 */
	// leere Angabe ist zulssig
	if( (c = peekStyleChar( ) ) != styleEOThrow && c!= styleEOCatch)
	{       // erster Punkt
		p = getDPoint(&defVPosition);
		if(parameterError)              // Fehler
			return false;
		
		t->v = p;                               // Eintragen
	}
	
	
	// Es muss ein Trenzeichen
	if( (c = peekStyleChar( ) ) != styleEOThrow && c!= styleEOCatch)
	{       ret = false;                    // kein Trennzeichen->Fehler
		goto bail;
	}
	if( c == styleEOThrow ) // Trenner berspringen
		getChar();
	
	// leere Angabe ist zulssig
	if( (c = peekStyleChar( ) ) != styleEOCatch )
	{       // zweiter Punkt
		p = getDPoint(&defNPosition);
		if( !parameterError )                           //Wenn vorhanden:
			t->n = p;                                               // Eintragen
	}
	
	// Es mu Endzeichen folgen
	if( getChar( ) != styleEOCatch )
	{       ret = false;                    // kein Endzeichen->Fehler
	}
	
	
bail:
	st.skip = s;
	
	return ret;
}

// Musterdefinition abfragen
// Eingabe: Mustername
// Ausgabe: Beschreibung oder default-Muster
style *getStyle( char *name, style *f )
{
	style *p, *t;
	
	p = t = f;      // Zeiger initialisieren, t = default
	
	while( p )
	{       if( stricmp( name, p->name) == 0 )
	{       t = p;  // gefunden->dies verwenden
		break;
	}
		p = p->next;
	}
	
	return t;
}

//Speicher des eingelesenen Textes wieder freigeben
void releaseFileMemory( void )
{
	if( tp )
		free( tp );
	
	tp = NULL;
}

// Alle Muster wieder freigeben
//yyy gibt nicht first->first frei!
#if 1
void releaseStyles( style *first )
{
	style *p, *t;
	
	p = first->next;
	
	while( p )
	{       if( p->first)
		releaseMoves( p->first );
		t = p->next;
		free( p );
		p = t;
	}
//	free( first );
}
#else
// ok
void releaseStyles( style *first )
{
	style *p, *t;
	
	p = first;
	
	while( p )
	{       if( p->first)
		releaseMoves( p->first );
		t = p->next;
		
		if( p != first )
			free( p );
		
		p = t;
	}
}
#endif

// Alle Teilmuster freigeben
void releaseMoves( styleEl *f )
{
	styleEl *p, *t;
	
	if( (p = f) == NULL )
		return;
	
	do
	{       t = p->next;
		free( p );
		p = t;
	}while( p != f);
}

// File einlesen, untersuchen, Speicher des Files wegwerfen
wwwBoolean scanStyleFile( char *name, style *f )
{
	wwwBoolean ret = false;
	// Einlesen
	if( readStyleFile(name))
	{
		extern  ScannerState st;
		
		// scanner initialisieren
		initScanner( tp, mySize);
		st.skip = false;
		setScanLineComment( '!' );
		
		// Analysieren
		readStyles( f );
		
		// Speicher des Files kann schon weg
		releaseFileMemory();
		
		ret = true;
	}
	
	return ret;
}

style *getStyleFromIndex( int i )
{       style *dP;
	dP = &stdStyle;
	
	while( i && dP->next )
	{       dP = dP->next;
		i--;
	}
	return dP;
}

//-------------------------------------------------------------

void getDefaultStyles( void )
{
	extern char hPName[];
	
	// defaultMuster initialisieren
	sprintf(stdStyle.name, hPName);
	stdStyle.next = NULL;
	stdStyle.first = &defMove;
	stdStyle.count = 0;
	stdStyle.first->v = defVPosition;
	stdStyle.first->n = defNPosition;
	stdStyle.first->next = &defMove;
	scanStyleFile(stylePath, &stdStyle);
	
	//      free( stylePath);
}

void releaseDefaultStyles( void )
{       releaseStyles( &stdStyle);
}

//-------------------------------------------------------------


wwwBoolean readStyleFile( char *name )
{
	wwwBoolean ret = false;
	size_t l = 0;
	myP = 0L;
	if( tp )
		free( tp );
	tp = NULL;
	
#if 1
	if( (tp = _readFile( name, "rb", &l)) != NULL)
	{
		mySize = l;
		ret = true;
	}
	
#else
	
	if( (fp = fopen( name, "r")) != NULL )
	{
		mySize = getFileSize( fp );
		if( (tp = malloc( mySize )) == NULL )
		{       exit(1);
		}
		mySize = fread( tp, 1, mySize * sizeof( char ), fp );
		
		fclose( fp );
		return ret = true;
	}
	
#endif
	return ret;
}

#if 1

#ifdef USE_MY_STRCMP
// (defined( MAC_CLASSIC_VERSION) || defined (UNIX_VERSION) ||�efined(USE_MY_STRCMP))
// Zeichenketten vergleichen,
// Groschreibung ignorieren,
// max c Zeichen
//
// Ergebnis: neg,       p1 < p2
//                       0              p1 == p2
//...
int strnicmp( const char *p1, const char *p2, unsigned long c)
{
	
	while( --c && *p1 && *p2 && (tolower(*p1) == tolower(*p2)) )
	{       p1++;
		p2++;
	}
	
	
	
	return *p1 - *p2;
}


// Zeichenketten vergleichen,
// Groschreibung ignorieren,
//
// Ergebnis: neg,       p1 < p2
//                       0              p1 == p2
//...
int stricmp( const char *p1, const char *p2)
{
	
	while( *p1 && *p2 && (tolower(*p1) == tolower(*p2)) )
	{       p1++;
		p2++;
	}
	
	
	
	return *p1 - *p2;
}

#endif
#endif
