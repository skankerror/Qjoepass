/*
 *  prefs.cpp
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
 read / write list of variables from / to a file
 NB:
 booleans are declared as unsigned char in mactypes.h
 if you want to read a bool value by scanf,
 its not enough to use the result, but you have to check if the result is true or false
 see readBoolean().
 Another way would be to set the 2nd parameter of readBoolean to wwwBoolean,
 but then its not possible to scan my list of variables.
 Also, it would be possible to use int instead of bool.
 
 
 */
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "prefs.h"

#include "fileIO.h"
#include "myPrintf.h"

#include "vtypes.h"
#include "mydefs.h"

#include "cmdLine.h"

#include <assert.h>


#if 0
#define verbosePrefsOutput
#endif

wwwBoolean prefsValid = false;

char blankList[] = " \r\n\t";
wwwBoolean checkBlank( char c );


void readColorList( char *p,  void *v);
void writeColorList( FILE *fp,  void *v);

/*---------------------------------------------------------*/
void readVektor3(char *p, void *i)
{       int x;
	float f;
	float *v = (float *)i;
	
	if( p == 0 )
		return;
	
	for( x = 0; x < 3; x++ )
	{       while(*p &&  checkBlank(*p))
		p++;
		sscanf(p," %f", &f);
		*(v+x) = f;
		while(*p &&  !checkBlank(*p))
			p++;
	}
}


void readf4(char *p, void  *i)
{       int x;
	float f;
	float *v = (float *)i;
	
	if( p == 0 )
		return;
	
	for( x = 0; x < 4; x++ )
	{       while(*p &&  checkBlank(*p))
		p++;
		sscanf(p," %f", &f);
		*(v+x) = f;
		while(*p &&  !checkBlank(*p))
			p++;
	}
}

#if 0
void readBoolean(char *p, void *i)
{
	int *b = (int *)i;
	
	if( p == 0)
		return;
	
	sscanf(p," %d", b);  
}
#else
void readBoolean(char *p, void *i)
{
	int b;
	
	if( p == 0)
		return;
	
	sscanf(p," %d", &b);  
	*(bool *)i = (bool)b;
}
#endif
void readShort( char *p, void *b)
{       int v;
	if( p == 0 )
		return;
	
	sscanf(p," %d", &v);
	*(short*)b = (short)v;
}

void readInt( char *p, void *i)
{       int b;
	
	if( p == 0)
		return;
	
	sscanf(p," %d", &b);
	*(int *)i = b;
}

void readPrintableChar( char *p, void *i)
{       char *b = (char *)i;
	
	if( p == 0)
		return;
	
	sscanf(p," %c", b);
}

void readLong( char *p, void *i)
{       long *b = (long *)i;
	
	if( p == 0 )
		return;
	
	sscanf(p," %ld", b);
}

void readFloat( char *p, void *i)
{
	float *b = (float *)i;
	
	if( p == 0 ){
		*b = 0;
		return;
	}
	sscanf(p," %f", b);
}

void readRestOfLine( char *p, char ** i )
{
	size_t l = 0;
	char *s, *t;
	
	// skip spaces and tabs
	while(  *(p+l) &&
			(*(p+l) == ' ' || *(p+l) == '\t') &&
			*(p+l) != '\n' &&
			*(p+l) != '\r')
		l++;
	
	t = p+l;
	
	l = 0;
	while(  *(t+l) &&
			*(t+l) != '\n' &&
			*(t+l) != '\r')
		l++;
	
	
	s = (char *)malloc( l + 1 );
	strncpy( s,t, l );
	*(s+l) = '\0';
	
	*i = s;
}

void writeVektor3(FILE *fp, void *i)
{
	Vector3D *v = (Vector3D *)i;
	fprintf(fp, "%f %f %f\n", v->x, v->y, v->z );
}

void writef4(FILE *fp, void *i)
{
	float *v = (float *)i;
	fprintf(fp, "%f %f %f %f\n", *v, *(v+1), *(v+2), *(v+3) );
}
void writeBoolean(FILE *fp,void *i)
{
	
#if 01
	bool t = *(bool *)i;
	
	//		*v = *v?true:false;
	fprintf(fp, "%d\n", t);//(*v!=0));
#else
		writeInt( fp, i);
#endif
		
}

void writeFloat(FILE *fp, void *i)
{       float *v = (float *)i;
	fprintf(fp, "%f\n", *v);
}

void writeInt(FILE *fp, void *i)
{       int *v = (int *)i;
	fprintf(fp, "%d\n", *v);
}

void writePrintableChar(FILE *fp, void *i)
{       char *v = (char *)i;
	fprintf(fp, "%c\n", *v);
}

void writeShort(FILE *fp, void *i)
{       short *v = (short *)i;
	fprintf(fp, "%d\n", *v);
}

void writeLong( FILE *fp, void *i)
{        long *v = (long *)i;
	fprintf(fp, "%ld\n", *v);
}


wwwBoolean checkBlank( char c)
{       char *p;
	
	p = strchr( blankList, c);
	
	return p != NULL;
}

#if 0

void readColorList( char *p, float *l)
{
	int c, d;
	float f;
	
	for( c = 0; c < ColorsInList; c++ )
	{
		for( d = 0; d < d; d++ )
		{       while(*p &&  checkBlank(*p))
			p++;
			sscanf(p," %f", &f);
			*(l+c*3+d) = f;
			while(*p &&  !checkBlank(*p))
				p++;
		}
	}
}

void writeColorList( FILE *fp, float *v)
{
	int c,d;
	
	for( c = 0; c < ColorsInList; c++ )
	{
		for(d = 0; d < 3;d++)
			fprintf(fp, "%f ", v + c * 3 +d);
		fprintf(fp, "\n");
	}
}
#endif

#if 01

void writeMatrix4(FILE *fp, void *i)
{
	int x,y;
	float *matrix = (float *)i;
	
	
	for (y = 0; y < 4; y++)
	{       for( x = 0; x < 4; x++ )
	{
		fprintf(fp," %f", *(matrix+4*x+y));
	}
	}
	fprintf(fp,"\n");
}


void readMatrix4(char *p, void *i)
{       int x,y;
	float f;
	float *matrix = (float *)i;
	
	
	for (y = 0; y < 4; y++)
	{       for( x = 0; x < 4; x++ )
	{       while(*p &&  checkBlank(*p))
		p++;
		sscanf(p," %f", &f);
		*(matrix+x*4+y) = f;
		while(*p &&  !checkBlank(*p))
			p++;
	}
	}
}

#endif

wwwBoolean saveTextPrefs( char *fileName, prefAtom *e )
{
	FILE *fp;
	//      prefAtom *e = prefList;
	time_t t;
	wwwBoolean ret;
	time (&t);
	
	if( cmdLineQuitFlag ){
		static bool showWritePrefsMsg = true;
		if( showWritePrefsMsg && verboseOutput){
			_myPrintf("writing prefs disabled due to cmdLine start\n");
			showWritePrefsMsg = false;
		}		
		return false;
	}
	
#ifdef verbosePrefsOutput
	printf("writing prefs to file: %s\n", fileName);
#endif	
	if( (fp = fopen( fileName, "w")) != NULL ){
		fprintf(fp,"Preference file\ngenerated %s\n\n", ctime(&t ));
		while( e->name != NULL ) {
			// write formatting lines, but none with partly undefined values, as we don´t want to want disabled options be written
			if( e->readFunction == NULL ||  e->writeFunction == NULL ||  e->value == NULL ){
				if( strstr(e->name, ":=") == NULL ) // don´t write obsolete  values (the ones with a read or write function undefined)
					fprintf(fp, "%s\n", e->name);
			}
			else{
				fprintf(fp, "%s", e->name);
				e->writeFunction(fp,e->value);
			}
			
			e++;
		}
		fclose( fp );
		ret = true;
		
	}else
		ret = false;
	
#ifdef verbosePrefsOutput
	printf("writing prefs ... ");
	printResult( fp !=  NULL);
#endif
	
	
	return ret;
}


wwwBoolean readTextPrefs(char *fileName, prefAtom *e)
{
	char *buf,*p;
	//      prefAtom *e = prefList;
	//        _myPrintf("reading prefs from file: %s\n", fileName);
	
	buf = readFile( fileName, "r");
	/*        _myPrintf("reading prefs ... ");
	printResult( buf !=  NULL);
	*/
	if( buf == NULL )
		return false;
	
	
	while( e->name != NULL )
	{
		p = strstr( buf, e->name);
		if( p )
		{
			// keywworls have to start at the beginning of
			// 1) text buffer
			// 2) a line
			if( p == buf || *(p-1) == '\n' || *(p-1) == '\r'  )
			{
				if( e->readFunction)
				{       p += strlen(e->name);
					while( *p &&  checkBlank(*p))
						//                              while( *p &&  *p == ' ')
						p++;
					e->readFunction(p,e->value);
				}
			}
                }
		e++;
        }
	return true;
}

void _readString( char *p, char **h);

void readString( char *p, void *s)
{
	char *b;
	char **h = &b;
	//      _myPrintf("%s\n", s);
	
	_readString( p, h);
	*(char **)s = *h;
}
void _readString( char *p, char **h)
{
	char *b = p, *m;
	char tmp;
	
	if( p == 0 )
		return;
	
	while(*b && *b !='\n' && *b != '\r')
		b++;
	
	tmp = *b;
	*b = '\0';
	
	m = (char *)malloc( strlen( p ) + 1);
	//      sscanf(p, "%s", m);
	sprintf(m, "%s", p);
	
	*b = tmp;
	//_myPrintf(">%s<\n", m);
	
	
	*h = m;
}


void _writeString(FILE *fp, char **h);


void writeString( FILE *fp, void *s)
{
	char **h = (char **)s;
	_writeString( fp, h);
}

void _writeString( FILE *fp, char **h)
{
	//      _myPrintf("%s\n", *h);
	
	fprintf(fp, "%s\n", *h);
	
}
