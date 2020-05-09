/*
 *  fileIO.cpp
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "systemswitch.h"

#include "fileIO.h"
#include "myPrintf.h"
#include "cmdLine.h"

char cwd[1000];

size_t lastReadSize = 0;


size_t getFileSize( FILE* fp )
{
        size_t  a, l;

        a = ftell( fp);                         // aktuelle Position
        fseek( fp, 0, SEEK_END);        // Lnge abfragen
        l = ftell( fp );
        fseek( fp, a, SEEK_SET);        // wieder aktuelle Position

        return l;
}

// read (parts of) a file into a buffer,
// allocate buffer if requested
// input:       name    file name
//                      mode    read mode
//                      size number of caracters to be read, 0 for all
// output:      return value: pointer to buffer,
//                      size:             number of characters read,
//                                                1 at least, 0 for failiure
// allocates memory for buffer.
///doesnt free buffer-memory
char *readFile( char *name, char *mode )
{
        size_t l = 0l;
        getcwd(cwd, 1000 );

        return _readFile(name, mode, &l );
}

char *_readFile( char *name, char *mode, size_t *size )
{
        static FILE *fp;
        size_t  mySize, totalSize;
        char *buffer;

        if( (fp = fopen( name, mode)) != NULL )
        {
//		_myPrintf("%s, %s\n", name, mode );
                // get character count
                if( *size)
                        mySize = *size;
                else
                        mySize = getFileSize( fp );

                lastReadSize = mySize;

                // allocate buffer
                // incl. space for an additional '\0' at end of file
                buffer = (char *)malloc( sizeof( char ) * mySize + 1);
                if( buffer == NULL)
                {       fclose( fp );
                        return 0;
                }
                totalSize = mySize;
                // read file
                mySize = fread( buffer, 1, mySize * sizeof( char )+1, fp );
                //printf("readFile=: %ld bytes read (from total %ld)\n", mySize, totalSize);
                // make it cString alike
                *(buffer + mySize) = '\0';

                // close file
                fclose( fp );

                // save read characters, excl '\0'
                *size = mySize;

                return buffer;
        }
        return NULL;
}


// writes a file, stdDir is applicationDir
// destroys original file content
// input; name
// return: true for success
bool writeFile( char *name, char *mode, char *buffer, size_t size )
{
        static FILE *fp;
        size_t w;

        if( (fp = fopen( name, mode)) != NULL )
        {
                w = fwrite( buffer, 1, size * sizeof( char ), fp );

                fclose( fp );

                return true;
        }
        return false;
}



bool copyFile( char *destName, char *sourceName)
{
	FILE *fd, *fs;
	size_t s;
	char *b;
	
	fs = fopen(sourceName, "rb");
	if( fs == 0 )
		return false;

	s = getFileSize( fs );
	b = (char *)malloc(s);
	
	fread( b, s, 1, fs);
	fclose( fs );
	
	fd = fopen(destName, "wb");
	if( fd == 0 ){
		free( b );
		return false;
	}
	
	fwrite( b, s, 1, fd);
	fclose( fd);
	free( b );
	
	return true;
}

bool moveFile( char *d, char *s)
{	bool ret;
	
	if( ret = copyFile( d, s) )
		removeFile(s);
	return ret;
	
}

// try to remove a file
// print to screen
// return true if successfull
bool removeFile( char *name )
{
	bool ret;
	
	if(0 && verboseOutput)
		printf("removing %s ... ", name);
	ret =   remove( name );
	if(0 && verboseOutput)
		printResult( ret == 0 );
	
	return ret == 0;
}


bool fexists( char *name )
{
	FILE *fp;
	bool ret = false;
	
	if( (fp = fopen( name, "r") ) != NULL )
	{
		fclose(fp);
		ret = true;
	}

        return ret;
}


bool compareTime( tm *t1, tm *t2 )
{
	if( t1 == NULL || t2 == NULL )
		return false;
	else
		return	t1->tm_sec  != t2->tm_sec  ||
				t1->tm_min  != t2->tm_min  ||
				t1->tm_hour != t2->tm_hour ||
				t1->tm_mday != t2->tm_mday ||
				t1->tm_mon  != t2->tm_mon  ||
				t1->tm_year != t2->tm_year;
}

