/*
 *  myPrintf.cpp
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
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "prefDef.h"
#include "applicationFolders.h"

#if 0
#define NO_OUTPUT
#endif


#include "myPrintf.h"

#undef printf

//wwwBoolean    writeLogfile    = true;
//wwwBoolean conOutput          = true;


//#define LOGFILENAME           "jpLogfile.txt"
static  FILE *logfile   = NULL;
int     printLogTimeStampNow = 1;
extern char *logfilePath;


void deleteLogfile( void )
{
	remove( logfilePath );
	
}



void printResult( int r )
{
#ifdef NO_OUTPUT
	return;
#endif
	
	myPrintf("%s\n", r==false ?"failed":"ok");
}


int myPrintf( const char *format, ... )
{       va_list args;
	
#ifdef NO_OUTPUT
	return 0;
#endif
	
	
	/*#include <windows.h>
		FrontWindow();
	*/
	if(jpPrefs.writeLogfile )
	{
		
		logfile = fopen( logfilePath, "a+");
		if( logfile == NULL )
			printf( "cant open logfile \n");
		else
		{
			if( printLogTimeStampNow )
			{
				time_t  t;
				
				t = time( NULL );
				fprintf(logfile, "logfile created on %s\n", ctime( &t ));
				printLogTimeStampNow = 0;
			}
			
			// print to logfile
			va_start( args, format);
			vfprintf(logfile, format, args);
			va_end(args);
		}
	}
	
	
	if( jpPrefs.conOutput )
	{
		// print to console
		va_start( args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
	
	if( logfile && logfile != stdout )
		fclose( logfile );
	
	
	
	return 0;
}

int printToFile( const char *name, const char *format, ... )
{
#ifdef MAC_CLASSIC_VERSION
#pragma unused( name )
#endif
	
	va_list nextArg;
	
#ifdef NO_OUTPUT
	return 0;
#endif
	
#if 0
	
	va_list args;
	FILE *fp = NULL;
	wwwBoolean newLogfile = false;
	
	if( fopen( name, "r") )
		newLogfile = false;
	else
		newLogfile = true;
	
	if( newLogfile )
	{
		fp = fopen( name, "w");
		
	}else
		fp = fopen (name, "a");
	
	
	
	if( fp )
	{
		// print to logfile
		va_start( args, format);
		vfprintf(fp, format, args);
		va_end(args);
		fflush(fp);
		
		fclose( fp );
	}
	
	
#else
	
	va_start( nextArg, format);
	vfprintf(stdout, format, nextArg);
	va_end(nextArg);
#endif
	
	return 0;
	
}


int _myPrintf( const
              char *arg, ... )
{
	va_list nextArg;
	
#ifdef NO_OUTPUT
	return 0;
#endif
	
	
	va_start( nextArg, arg);
	vfprintf(stdout, arg, nextArg);
	va_end(nextArg);
	fflush(stdout);
	return 0;
}


