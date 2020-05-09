/*
 *  scanner.cpp
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
/*
Einfacher Scanner

- Liest einen Text zeichenweise ein.
- Stellt Funktionen zum Einlesen von Textersatz zur Verfügung.
  (Wort wird durch Ersatztext ersetzt)
- Überspringt Rest der Zeile nach definierbarem Kommentarzeichen
- Stellt in Verbindung mit Dokument-Routinen von DragText AusgabeText in einem eigenen Fenster dar.
- Muß einen Header mit den Resource-IDs des aktuellen Files einbinden
*/

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "globals.h"
#include "myutil.h"

#ifdef MAC_VERSION
#include "stdUtil.h"
#else
#include "systemswitch.h"
#endif
#include "scanner.h"

#if 1
#define DEBUG
#endif

#if 0
#define TESTREPLACE	
#endif

#ifdef TESTREPLACE
wwwBoolean peek = false;
#endif

ReplaceUnit *theFirstRU = NULL;

// Braucht der scanner
char *allBlanks =  " \t\n\r";		// Alles, was ich als  Leerzeichen betrachte. 
char *wordBlanks = " \t\n\r";		// Wort- und Zeilenende
char *lineBlanks = "\n\r";	
char *lineCode	= "\r\n";			// zeilenende
char *EDCString = 0l;				// Extern Definierte Zeichenkette
char *replacementWordBlank = " \t";	// damit endet der erste definierte Begriff.
ScannerState st;		  
	

wwwBoolean knowsLineComments = false;
wwwBoolean knowsBlockComments = false;
char 	theLineCommentCode;
char 	theBlockCommentCode;

wwwBoolean parameterError;


char lastReadChar = '\0';
char lastPeekedChar = '\0';


int 	cInt;					// Anzahl der Ziffern des letzten int-Wertes. Validität in getInt() und getFloat() hat´s leichter
int 	fInt;					// Anzahl der Ziffern des letzten float-Wertes. Validität in getFloat()

long 	skipInval( void );						// Unwichtiges überspringen
long	skipBlanks( void );						
long	skipLineComment( void );
long	skipBlockComment( void );
wwwBoolean	inString( char c, char *theBlanks );
long	skipEDC( void );

wwwBoolean isBlank( char c );


wwwBoolean		initReplacementZone( void );
void		disposeReplacementZone( void );
wwwBoolean		growReplacementZone( void );
ReplaceUnit *getReplacementUnit( int c );
wwwBoolean		doReplacement( void );
wwwBoolean		getReplacementWord( char **theStart, long *count );
wwwBoolean		getReplacementLine( char **theStart, long *count );


char 	*getTextStart( void );					// und abfragen					
char 	*getTextPointer( void );				// Aktuellen Pointer

char 	_readBuffer( void );					// nächstes Zeichen aus Textbereich lesen, Ersatz beachten
char 	_peekBuffer( void );					// nächstes Zeichen abfragen

// ------------------------ Text scannen --------------------------------------------------
// 
// Text liegt als char-Array vor.
// Einfacher Textersatz wird unterstützt
wwwBoolean initScanner( char *thePattern, long count )
{	
	st.parseStart = st.parsePointer = thePattern;		
	st.size = count;
	st.readPos = 0l;
	st.prBuffer = NULL;
	st.prCount =0l;
	st.skip = true;
	st.newWord = true;
	st.inReplacement = false;
	
	if( initReplacementZone() == false )			// Speicherbereich für Ersatztexte abfragen
		return false;

	parameterError = false;
	
	return true;
}
	
// Textersatz-Routinen initialisieren
wwwBoolean initReplacementZone( void )
{	
	wwwBoolean ret = true;
	
#ifdef TESTREPLACE	
	printf("Beginne Textersatz\n");
#endif		
	
	disposeReplacementZone( );
		
	return ret;
}	

void disposeScanner( void )
{	disposeReplacementZone();
}

void disposeReplacementZone( void )
{
	ReplaceUnit *theRU, *t;

	theRU = theFirstRU;
	
	if( (t = theRU) != NULL )
	{	
		theRU = theRU->next;
//		printf("%s -> %s\n", t->wordStart, t->replaceStart);
		free( t );
	}
	theFirstRU = NULL;

}

// Ersatz-Text-Bereich vergrößern
wwwBoolean growReplacementZone( void )
{
	wwwBoolean ret = true;
	ReplaceUnit *t, *l;
	
	l = getReplacementUnit( -1 );						// Zeiger auf letzte Einheit holen
	t = (ReplaceUnit*)malloc( sizeof( ReplaceUnit));	// Speicher für neue Einheit
	if( t == NULL )
	{	
#ifdef MAC_VERSION
		errorHandler(jpWarningStringsID, wIReplaceMemory, false );			// Fehler
#endif
		ret = false;
	}	
	else
	{	t->next = NULL;
		if( l == NULL )
		{	theFirstRU = t;	// Erste Einheit
		}else
		{	l->next = t;	// Alle anderen
		}		
		ret = true;
	}		

	return ret;	
}	

// Zeiger auf Einheit nr c holen
// c <= 0: Zeiger auf letzte Einheit
// Ausgabe: Zeiger auf Einheit 
//          NULL, wenn noch keine Einheit alloziert
ReplaceUnit *getReplacementUnit( int c )
{
	ReplaceUnit *t;
	
	t = theFirstRU;

	while( --c && t && t->next)
	{	t = t->next;
	}
	
	return t;

}

// Word und Ersatztext einlesen
wwwBoolean setReplacement( void )
{
	ReplaceUnit *theUnit;
	wwwBoolean ret = false;
	
	if( growReplacementZone() == false )
	{	
#ifdef MAC_VERSION
		errorHandler(jpWarningStringsID, jpErrorALRT, false );
#endif
		return false;
	}	
	
	st.mayReplace = false;

	theUnit = getReplacementUnit(-1);			// letzte Einheit

	if( getReplacementWord( &theUnit->wordStart, &theUnit->wordLength ) == false )
		goto PointOfReturn;
	
	if( getReplacementLine( &theUnit->replaceStart, &theUnit->replaceLength ) == false )
		goto PointOfReturn;
		
	ret = true;

PointOfReturn:		
	st.mayReplace = true;	

	return ret;	
}	

	

#if 0
// ein Worte einlesen für Textreplacement. 
// Daher ist ein leeres Wort ein Fehler
wwwBoolean getReplacementWord( char **theStart,long *theCount )
{
	char theChar;
	char *theEnd;
	wwwBoolean skipState;

	

	theChar = getChar();					// Beginne mit einem Buchstaben
	if( !isalpha( theChar ) )	
	{	*theStart = NULL;
		return false;
	}
	*theStart = getTextPointer() - 1;		// Anfang eintragen
	
	skipState = st.skip;					// keine Leerzeichen mehr überspringen
	st.skip = false;

	while( isalnum( theChar = peekChar() ) )	// Nur Zahlen und Buchstaben erlaubt.
	{	getChar();
	}
	
	theEnd = getTextPointer();				// Wir sind am Ende
	*theCount = theEnd - *theStart;

	if( !inString( theChar, wordBlanks ) )	// Am Ende muß ein Leerzeichen folgen.
		return false;	

	st.skip = skipState;
		
	return true;							// mindestens ein Zeichen!
}
#else
	
// ein Worte einlesen für Textreplacement. 
// Daher ist ein leeres Wort ein Fehler
wwwBoolean getReplacementWord( char **theStart,long *theCount )
{
	char theChar;
	char  *theEnd;
	wwwBoolean skipState;

	
	
	// leerzeichen überspringen
	theChar = _peekBuffer();
	while( inString( theChar, replacementWordBlank))
	{
		_readBuffer();
		theChar = _peekBuffer();
	}
	
	*theStart = getTextPointer();			// Anfang eintragen
	
	skipState = st.skip;					// keine Leerzeichen mehr überspringen
	st.skip = false;

	while( !isBlank( theChar = peekChar() ) )	// Nur Zahlen und Buchstaben erlaubt.
		getChar();
	
	
	// get one space to force delimeted replacement (mails with martin frost 5.07)
//	getChar();
	
	theEnd = getTextPointer();				// Wir sind am Ende
	*theCount = theEnd - *theStart;

	if( !inString( theChar, wordBlanks ) )	// Am Ende muß ein Leerzeichen folgen.
		return false;						// Zeilenende ist ein Fehler!
			

	st.skip = skipState;
		
	return true;							// mindestens ein Zeichen!
}
#endif

wwwBoolean getReplacementLine( char **theStart, long *theCount )
{	
	char theChar;
	char *theEnd;
	wwwBoolean skipState;
	char t[1000];

	skipInval();	// Keine führenden Leerzeichen!
	
	skipState = st.skip;
	st.skip = false;
	
	*theStart = getTextPointer();
	while( (theChar = peekChar()) != '\0' && theChar != theLineCommentCode && strchr( lineBlanks, theChar ) == NULL  )	// alles bis zum Zeilenende
	{	theChar = getChar();
	}
	theEnd = getTextPointer();
	*theCount = theEnd - *theStart; 
	strncpy(t, *theStart, *theCount );
	 
	st.skip = skipState;

	if( theChar == theLineCommentCode )
		skipLineComment();
	if( theChar == theBlockCommentCode )
		skipBlockComment();
	
	return theChar != '\0';
}			

void getString( char *s, int c )
{
	wwwBoolean skipState;
	char theChar;
	
	skipInval();	// Keine führenden Leerzeichen!
	
	skipState = st.skip;
	st.skip = false;
	
	while( --c && (theChar = peekChar()) != '\0' && theChar != theLineCommentCode && strchr( lineBlanks, theChar ) == NULL  )	// alles bis zum Zeilenende
	{	*s++ = getChar();
	}
	*s = '\0';

	st.skip = skipState;
}	
// Textersatz wird unterstützt.
// Daher ist nicht mehr gesichert, 
// daß die Zeiger auf den Textbereich vorwärts bewegen 
//
// Alle Abfragen über diese Routinen, mit Ausnahme der Textersatz-Routinen. 
// Verschachtelter Textersatz wird nicht unterstützt.
// ggf. Ersatztext ausgeben
// Eingabe: wird das Zeichen gelesen?
// RetCode == true, wenn in Ersatzmode
wwwBoolean doReplacement( void )
{
	ReplaceUnit *theUnit;
	
	
	// Ich darf nix tun!
	if( st.mayReplace == false )
		return false;
		
	// Ich darf nicht nochmal ersetzen
	if( st.inReplacement == true )
		return true;
		
		
	// Ersatztext-Liste durchsuchen und ggf. Pointer neu setzen	

	theUnit = theFirstRU;
	
	while( theUnit )
	{
#if 0		
		char p[1000];
		
		sprintf(p, " %s ", theUnit->wordStart);
		if( strncmp( st.parsePointer, p, strlen( p ) ) == 0)
		{	st.prBuffer = st.parsePointer + theUnit->wordLength;	// Da machen wir später weiter
			st.readPos += theUnit->wordLength;						// dann haben wir soviel Zeichen gelesen
			st.parsePointer = theUnit->replaceStart;				// jetzt erstmal hier
			st.prCount = theUnit->replaceLength;					// Das erste Zeichen wird sofort ausgelesen
			st.inReplacement = true;
			break;
		}	
#else
		if( strncmp( st.parsePointer, theUnit->wordStart, theUnit->wordLength) == 0)
		{	st.prBuffer = st.parsePointer + theUnit->wordLength;	// Da machen wir später weiter
			st.readPos += theUnit->wordLength;						// dann haben wir soviel Zeichen gelesen
			st.parsePointer = theUnit->replaceStart;				// jetzt erstmal hier
			st.prCount = theUnit->replaceLength;					// Das erste Zeichen wird sofort ausgelesen
			st.inReplacement = true;
			break;
		}	
#endif
		theUnit = theUnit->next;
	}

	
	return st.inReplacement;
}	

// Pointer setzen, abfragen 
char *getTextStart( void )
{	return st.parseStart;	// Zeiger auf Text-Anfang
}

char *getTextPointer( void )
{	return st.parsePointer;
}

// Was ist ein Kommentar?
void setScanLineComment(const char commentCode )
{
	theLineCommentCode = commentCode;
	knowsLineComments = true;
}

// Gibt keine Kommentare;
void unsetScanLineComment( void )
{	knowsLineComments = false;
}

// Was ist ein Kommentar?
void setScanBlockComment(const char commentCode )
{
	theBlockCommentCode = commentCode;
	knowsBlockComments = true;
}

// Gibt keine Kommentare;
void unsetScanBlockComment( void )
{	knowsBlockComments = false;
}

// Ist das übergebene Zeichen ein Leerzeichen?
wwwBoolean inString( char c, char *theBlanks )
{
	return strchr( theBlanks, c ) != NULL;
}

// Unwichtige Zeichen
// Werden beim Scannen übersprungen
void setEDC( char *theString )
{	EDCString = theString;
}

void unsetEDC( void )
{	EDCString = 0l;
}


// word : nur Buchstaben

char *getWord( void )
{	
	wwwBoolean oldState;
	static char theWord[255];
	char c;
	skipInval();
	
	cInt = 0;
	oldState = st.skip;
	st.skip = false;

	while(	(c = peekChar()) != '\0' && c != ';' /*&& c != ',' */&&
			isgraph(c)) 
		// Alle Ziffern einlesen
	{	theWord[cInt] = getChar();						
		cInt++;
	}
	
	if( cInt == 0 )								// Keine Eingabe ist ein Fehler
		parameterError = true;
	else
		theWord[cInt] = '\0';
		
	st.skip = oldState;
	
	return theWord;
}	

char *peekWord( void )
{	
	wwwBoolean oldState;
	static char theWord[255];
	ScannerState theState;
	
	theState = st;	

	skipInval();
	
	cInt = 0;
	oldState = st.skip;
	st.skip = false;

	while(peekChar() && isgraph( peekChar()) && cInt < 255)	// Alle Ziffern einlesen
	{	theWord[cInt] = getChar();						
		cInt++;
	}
	
	if( cInt == 0 )								// Keine Eingabe ist ein Fehler
		parameterError = true;
	else
		theWord[cInt] = '\0';
		
	st.skip = oldState;

	st = theState;	
	return theWord;
}	

// int : nur Zahlen.
unsigned int getInt( void )
{	
	unsigned int u;
	wwwBoolean oldState;
	char theNumber[255];

	skipInval();
	
	cInt = 0;
	oldState = st.skip;
	st.skip = false;

	while(peekChar() && isdigit( peekChar()) )	// Alle Ziffern einlesen
	{	theNumber[cInt] = getChar();						
		cInt++;
	}
	
	if( cInt == 0 )								// Keine Ziffer ist ein Fehler
		parameterError = true;
	else
		theNumber[cInt] = '\0';
		
	st.skip = oldState;
	
	u = atoi(theNumber);
	
	return u;
}	

// int : Mit Vorzeichen.
int getSignedInt( void )
{
	int i;
	char vz;
	wwwBoolean oldState;

	skipInval();

	oldState = st.skip;
	st.skip = false;

	vz = '+';
	
	if( peekChar() == '+' || peekChar() == '-' )
		vz = getChar();

	i = getInt();	

	st.skip = oldState;

	return i*(vz=='+'?1:-1);
}	
	
#if 0
// Lese eine Fließkommazahl ein.	
float getFloat( void )
{	float f,fk;
//	wwwBoolean err = false;
	wwwBoolean oldState;
		
	skipInval();

	oldState = st.skip;
	st.skip = false;
	
	fInt = 0;
	
		
	f = fk = 0.;
	
	// Überprüfung
	if( isdigit(peekChar() ) )				// Zahl beginnt
	{	f = (float)getInt( );				
		fInt += cInt;
	}	
	if( peekChar() == '.' )					// punkt folgt
	{	getChar();
		if( isdigit( peekChar() ) )		// Und es kommt noch was!
		{	fk = (float)getInt();
			fk *= pow( 10.f, -cInt);
			fInt +=cInt;
		}
		f += fk;
	}
	
	if( fInt == 0 )		// garkeine Angabe ist ein Fehler
		parameterError = true;
		
	st.skip = oldState;
	
	return f;
}		
#else
#define MAX_FLOAT_CHARS 1024 
float getFloat(void )
{
	float f;
	char s[MAX_FLOAT_CHARS];
	int p = 0;
	wwwBoolean oldState;
	
	skipInval();
	
	oldState = st.skip;
	st.skip = false;
	
	
	while(p < MAX_FLOAT_CHARS-2 && isdigit(peekChar()))
		s[p++] = getChar();
	if(p < MAX_FLOAT_CHARS-2 && peekChar() == '.' ) 	
		s[p++] = getChar();
	while(p < MAX_FLOAT_CHARS-2 && isdigit(peekChar()))
		s[p++] = getChar();

	s[p] = '\0';
	sscanf(s, "%f", &f);		  

	st.skip = oldState;

	return f;
}
#endif

// float : Mit Vorzeichen.
float getSignedFloat( void )
{	
	float f;
	char vz;
	wwwBoolean oldState;

	skipInval();

	oldState = st.skip;
	st.skip = false;

	vz = '+';
	
		
	if( peekChar() == '+' || peekChar() == '-' )
		vz = getChar();

	f = getFloat();	

	st.skip = oldState;

	return f*(vz=='+'?1:-1);
}	

// character : ein Zeichen
char getCharacter( void )
{	
	wwwBoolean oldState;
	char c;
		
	skipInval();
	
	cInt = 0;
	oldState = st.skip;
	st.skip = false;

	c = getChar();
	
	st.skip = oldState;
	return c;
}	

// Suche an aktueller Textstelle nach Textersatz
// Rückgabe:
// NULL oder Zeiger auf ErsatzUnit;
// ErsatzUnit wird gelockt -> am Ende des Textersatzes wieder freigeben.

//Definiertes Zeichen, Leerzeichen und Kommentare überspringen
// Anzahl übersprungener Zeichen zurückgeben
long skipInval( void )
{
	long r1=0 , r2=0, r3=0;
	long ret = 0l;
	
	while( (r1 = skipEDC())!=0L ||  
		   (r1 = skipBlanks())!= 0l || 
		   (r2 = skipLineComment()) != 0l ||
		   (r3 = skipBlockComment()) != 0l 
		   ) 
		ret += r1+r2;
		
	return ret;	
}	

// extern definierte Zeichenkette überspringen
long skipEDC( void )
{	wwwBoolean doIt, ret;
	char c;
	
	doIt = true;
	ret = 0l;
	
	if( EDCString == 0l )
		return 0l;
		
	if( !st.skip )
		return 0l;
		
	while( ( c = _peekBuffer()) != '\0' && doIt )			// Solange wir noch was zu tun hatten
	{
		doIt = false;
		if( inString( c, EDCString)  )						// Leerzeichen überspringen
		{	doIt = true;
			_readBuffer();
			ret++;
		}
		
	}
	
	return ret;
}	

// 	alle Leerzeichen überspringen
// Zurück: Anzahl der übersprungenen Zeichen
long skipBlanks( void )
{	wwwBoolean doIt, ret;
	char c;
	
	doIt = true;
	ret = 0l;
	
	if( !st.skip )
		return 0l;
		
	while( ( c = _peekBuffer()) != '\0' && doIt )			// Solange wir noch was zu tun hatten
	{
		doIt = false;
		if( inString( c, allBlanks)  )						// Leerzeichen überspringen
		{	doIt = true;
			_readBuffer();
			ret++;
		}
		
	}
	
	return ret;
}	

wwwBoolean isBlank( char c )
{	
	return inString( c, allBlanks);
}	

// dito Kommentare
long skipLineComment( void )
{
	long ret = 0l;
	char c;
	
	if( knowsLineComments && ( c = _peekBuffer()) == theLineCommentCode )					// Kommentar: bis zum Zeilenende überspringen
	{	
		while( ( c = _peekBuffer()) != '\0' &&  !strchr(lineCode, c ) )
		{	_readBuffer();
			ret++;
		}	
	}
	return ret;
}
	
// dito Kommentare
long skipBlockComment( void )
{
	long ret = 0l;
	char c;
	
	if( !knowsBlockComments)
		return ret;
		
	if( (c = _peekBuffer()) == theBlockCommentCode )					
	{	_readBuffer();
		while( ( c = _peekBuffer()) != '\0' )
		{	_readBuffer();
			ret++;
			if(  c == theBlockCommentCode )
				break;
		}	
	}
	return ret;
}


// Zeichen einlesen, abfragen
char getChar( void )
{	char c;
	long inSkip;
	
	do
	{	inSkip = skipInval();			// unwichtiges überspringen
		doReplacement();				// Ersatztexte überprüfen			
			
	}while( inSkip );
			
	c = _readBuffer();
	lastReadChar = c;
		
	return c;	
}

// Nächstes Zeichen abfragen. 
// Nicht einlesen!
// NB: aus peekBlank überspringt Leerzeichen und Kommentare 
// uns setzt u.U. neue Pointer (Textersatz)
char peekChar(void )
{
	char c;
	ScannerState ost;
	long inSkip;
		
		
#ifdef TESTREPLACE
	peek= true;		
#endif
	ost = st;

	do
	{	inSkip = skipInval();			// unwichtiges überspringen
		doReplacement();				// Ersatztexte überprüfen			
			
	}while( inSkip );

	c = getChar();
	lastPeekedChar = c;
	
	st = ost;
	
#ifdef TESTREPLACE		
	peek= false;
#endif	
	return c;
}


// aktuelles Zeichen aus Buffer lesen
// Pointer neu setzen
// evtl. Textersatz-Pointer anpassen
char _readBuffer( void )
{	char c;

	// in Ersatzmodus
	if( st.mayReplace == true && st.inReplacement == true )	
	{	
		// Ein Zeichen abholen
		c = *st.parsePointer;
#ifdef DEBUG
		if( c == '\0')
		{	beep();
			return '\0';
		}	
#endif				
		st.prCount--;
		
		// wenn fertig, alles zurücksetzen
		if( st.prCount == 0 )		
		{	st.parsePointer = st.prBuffer;
			st.prBuffer = NULL;
			st.inReplacement = false;
			// Jetzt zutück, Position wurde schon gezählt.
			return c;	
		}
	}else
	{	// Schluß ist
		if( st.readPos == st.size)
			return '\0';
	
	// Nächstes Zeichen
	st.readPos++;		
	c = *st.parsePointer;
	}
	st.parsePointer++;	

			
	if( c == '\0' )	// Sollte nicht vorkommen!
	{
#ifdef DEBUG
		beep( );
#else
		;
#endif
	}
	
	return c;	
}	


// aktuelles Zeichen aus Buffer abfragen
// Es werden keine Zeichen übersprungen
char _peekBuffer( void )
{	
	// Schluß
	if( st.inReplacement == false )
	{	if( st.readPos == st.size)
			return '\0';
	}
	
	// Inhalt
	return *st.parsePointer;
}

ScannerState getScannerState()
{
	return st;
}	

ScannerState setScannerState( ScannerState theState )
{	return st = theState;
}

