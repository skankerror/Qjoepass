/*
 *  preprocess.cpp
 *  joe2
 *
 * include a file 
 * to make it possible to 
 * a) add styles to patterns which are located in the
 *    directory "include" in the applications folder)
 *    syntax: #include <file>
 * b) concentrate on the really important part of the pattern 
 *    (place working siteswap in an extra file)
 *    all files have to be in the same location on the disk  
 *    Maybe this is too error prone to make it public
 *    ... to be checked
 *
 *
 *  created by wolfgang on 10.12.06.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "systemswitch.h"

#include "preprocess.h"
#include "pathAccess.h"
#include "fileIO.h"
#include "fileaccess.h"
#include "editor.h"

#include "parseSiteswap.h"

#include "myPrintf.h"

#define PositionFolder		"positions"
#define IncludeCommand		"#include"

#define IncludeDir			"include"
#define IncludeSysStart		'<'
#define IncludeSysEnd		'>'

#define IncludeLocalStart	'"'
#define IncludeLocalEnd		'"'

char tmpName[DIR_SIZE];

typedef struct _commentEntry { char *start, *end;} commentEntry;
commentEntry ce[10000];
int ceCounter;

void printCommentList( void )
{
	int c;
	
	_myPrintf("----------------------------\n");
	_myPrintf("---- commented areas -------\n");
	_myPrintf("----------------------------\n");

	for( c = 0; c < ceCounter; c++)
		_myPrintf("comment from %08x to %08x\n", ce[c].start, ce[c].end);
	_myPrintf("----------------------------\n\n\n");
	
}

void generateCommentList( char *p )
{
	char *t;
	
	ceCounter = 0;
	t = p;
	
	while( *t ){
		if( *t == '#' ){
			while(*t && *t != newlineCode )
				t++;
#if 0
		}else if(*t == '\"' ){
			ce[ceCounter].start = t;
			do{
				t++;
			}while(*t && *t != '\"');
			ce[ceCounter++].end = t;
#endif
		}else if(*t == '!' ){
			ce[ceCounter].start = t;
			do{
				t++;
			}while(*t && *t != newlineCode);
			ce[ceCounter++].end = t;
		}
		if( *t )
			t++;
	}
//	printCommentList( );
}

bool inCommentList( char *p )
{
	int c;
	bool ret = false;
/*	_myPrintf("----------------------------\n");
	_myPrintf("-- checking for comments ---\n");
	_myPrintf("-------- at %08x -------\n", p);
	_myPrintf("----------------------------\n");
*/	
	for( c = 0; c < ceCounter && ret == false; c++){
//		_myPrintf("checking from %08x to %08x", ce[c].start, ce[c].end);
		if( ce[c].start <=p && ce[c].end >= p ){
			ret = true;
		}
//		_myPrintf(" => %s \n", ret? "yes": "no");
		
	}
/*
	_myPrintf("------------ %s ------------\n", ret? "yes": "no");
	_myPrintf("----------------------------\n\n\n");
*/	
	return ret;
}



// at this time just #include
// requests memory, released at next call 
// => can´t be called multiple times
char *preprocess( char *inText){
	// preprocess the text file
	// currently only file inclusion
	// input: text file with pattern description
	// output: text file with expanded pattern description
	//		   or NULL in case of error
	
	static char *pBuf1 = NULL, *pBuf2;
	char *buf1, *buf2, *iBuf;
	char *p1, *p2;
	char t;
	

	char d[DIR_SIZE];
	
#if 0
	if( pBuf1 ){
		free( pBuf1);
		pBuf1 = NULL;
	}
#endif
	// push dir	
	getcwd(d, DIR_SIZE);	
//	chdir( textEditorPath );
	chdir( fullPatternPath );
	
	pBuf1 = (char *)malloc( strlen(inText)+1);
	sprintf(pBuf1, "%s", inText);
	
#define IncludeTag	"#include"
	
	p1 = pBuf1;
	
	// look for IncludeTag in input string
	while( (p2 = strstr( p1, IncludeTag)) != NULL ){
		
		// make a list of areas which are commented out
		generateCommentList( pBuf1 );
		
		// ignore commands that are in comments
		if( inCommentList( p2 ) ){
			p1 = p2+1;
			continue;
		}
		// p2 contains all from start to IncludeTag (excluding the tag)
		*p2 = '\0';
		
		// save part 1 to buffer 1;
		buf1 = (char *)malloc( strlen( p1 )+1);
		sprintf( buf1, "%s", p1 );
		
		// restore p1
		*p2 = '#';
		p1 = p2 + strlen(IncludeTag);
		
		
		// skip to first hyphon
		while( *p1 && *p1 != '\"' )
			p1++;

		if( *p1 )
			p1++;
		
		p2 = p1;
		// scan all until next hyphon
		while( *p2 && *p2 != '\"')
				p2++;
		
		// get the name of the include file
		t = *p2;
		*p2 = '\0';
		iBuf = readFile(p1, "r");
//		_myPrintf("found include command: >>>>%s<<<\n", p1);
		*p2 = t;		
		if( *p2 )
			p2++;
		

		buf2 = (char *)malloc( strlen( p2 )+1);
		sprintf( buf2, "%s", p2 );

		if(iBuf){
			// put result in my buffer
			pBuf2 = (char *)malloc( strlen(buf1) + strlen( iBuf) + strlen( buf2 ) +1 );
			sprintf(pBuf2, "%s%s%s", buf1, iBuf, buf2 );
		}else {
			pBuf2 = (char *)malloc( strlen(buf1) + strlen( buf2 ) +1 );
			sprintf(pBuf2, "%s%s", buf1, buf2 );
		}

#if 0
		
		_myPrintf("\n"
				  "-----------------------------------------------------------------------\n"
				  "current dir:\n"
				  "-----------------------------------------------------------------------\n"
				  "%s\n"
				  "-----------------------------------------------------------------------\n", fullPatternPath);
		
		_myPrintf( "\n"
				  "-----------------------------------------------------------------------\n"
				  "before include:\n"
				  "-----------------------------------------------------------------------\n"
				  "%s\n"
				  "-----------------------------------------------------------------------\n", buf1);
		if( iBuf ){
			_myPrintf("\n"
					  "-----------------------------------------------------------------------\n"
					  "to be included:\n"
					  "-----------------------------------------------------------------------\n"
					  "%s\n"
					  "-----------------------------------------------------------------------\n", iBuf);
		}
		_myPrintf("\n"
				  "-----------------------------------------------------------------------\n"
				  "after include:\n"
				  "-----------------------------------------------------------------------\n"
				  "%s\n"
				  "-----------------------------------------------------------------------\n", buf2);
		_myPrintf("\n"
				  "-----------------------------------------------------------------------\n"
				  "new pattern file:\n"
				  "-----------------------------------------------------------------------\n"
				  "%s\n"
				  "-----------------------------------------------------------------------\n", pBuf2);
#endif
		// release memory
		if( buf1 )
			free( buf1 );
		if( buf2 )
			free( buf2 );
		if( iBuf )
			free( iBuf );
		if( pBuf1 )
			free( pBuf1 );
		// switch to new memory
		pBuf1 = pBuf2;
		p1 = pBuf1;
	}
	
	// restore path
	chdir( d);
	return pBuf1;
}

