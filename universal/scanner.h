/*
 *  scanner.h
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
 
typedef struct {
					char 	*parseStart;			// Zeiger auf TextStart
					long 	size;					// Anzahl der Zeichen
					long 	readPos;				// Aktuelle Leseposition
					char 	*parsePointer;			// Zeiger auf das aktuelle Zeichen
					char	*prBuffer;	
					long 	prCount;
					wwwBoolean	skip;					// Leerzeichen überspringen ist erlaubt.
					wwwBoolean newWord;				// Gerade beginnt ein neues Word
					wwwBoolean mayReplace;				// Ersatztext wird eingelesen, keine Umwandlung erlaubt
					wwwBoolean inReplacement;			// Ersatz wird ausgegeben
			  }ScannerState;

extern ScannerState st;						// Was macht der Parser?

typedef struct _ReplaceUnit{
				 struct _ReplaceUnit *next;
				 char *wordStart;
				 long wordLength;
				 char *replaceStart;
				 long replaceLength;
			   }ReplaceUnit;

extern ReplaceUnit *theFirstRU;
	  
// Struktur und Speicherbereich für Ersatztexte

extern wwwBoolean parameterError;


wwwBoolean initScanner( char *thePattern, long theSize );
void disposeScanner( void );
wwwBoolean setReplacement( void );

void	setScanLineComment(const char commentCode );// Was ist ein Kommentar?
void	unsetScanLineComment( void );				// Gibt keine Kommentare;
void	setScanBlockComment(const char commentCode );// Was ist ein Kommentar?
void	unsetScanBlockComment( void );				// Gibt keine Kommentare;

void setEDC( char *theString );					// String mit unwichtigen Zeichen definieren
void unsetEDC( void );


char 	getChar( void );						// gültiges Zeichen einlesen. Leerzeichen / Kommentare
char 	peekChar(void );						// gültiges Zeichen abfragen, ohne Pointer zu verändern

void 				getString( char *s, int c );
unsigned int 		getInt( void );							// Daten einlesen
int 				getSignedInt( void );
float				getFloat( void );
float				getSignedFloat( void );
char				getCharacter( void );
char 				*getWord( void );
char				*peekWord( void );

ScannerState getScannerState();
ScannerState setScannerState( ScannerState theState );

#if defined(__cplusplus)
}
#endif
