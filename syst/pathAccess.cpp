/*
 *  pathAccess.cpp
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
 access to folders
 home directory
 system directory
 
 handles absolute and relative path
 */
#include "systemswitch.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <math.h>

#include "pathAccess.h"
#include "fileaccess.h"
#include "myPrintf.h"
#include "cmdLine.h"

#define         SYS_FOLDER_NAME "sys"

char            *sysDir;
char            applicationDir[PATH_SIZE];
char            *appDir;



const char *getSysDirName(void )
{       return sysDir;
}

const char *getAppDirName( void )
{       return applicationDir;
}

char *getAppFolder( const char *path )
{
	char *p0, *p;
	
	
	sprintf(applicationDir, "%s",path );
	//	_myPrintf("applicationDir = %s\n", applicationDir);
	
	
	// if I am an application bundle under mac os x
	if( strstr( applicationDir, "MacOS" ))
	{
		char *p1;
		
#define PATH_UP 4
		int c;
		
		for( c = 0; c < PATH_UP; c++ ){
			p1 = strrchr(applicationDir, PATH_DIVIDER);
			if(p1 == 0 )
				break;
			else
				*p1 = '\0';
			
		}
		sprintf(applicationDir,"%s%c", applicationDir,  PATH_DIVIDER);
	}
	
#ifdef WINDOWS_VERSION
	applicationDir[0] = toupper(applicationDir[0]);
#endif

	p0 = applicationDir;
	p = strrchr(p0, PATH_DIVIDER);
	if( p )
		*(p+1)='\0';
	
	_myPrintf("applicationDir = %s\n", applicationDir);
	if( p0 && *p0 != '.')
	{
		sysDir = (char *)malloc( strlen( p0 ) + strlen(SYS_FOLDER_NAME) + 3); //3 für zweimal Divider und einmal \0
		strcpy ( sysDir, p0 );
		p = strrchr(sysDir, PATH_DIVIDER);
		if( p )
			*(p)='\0';
		sprintf(sysDir, "%s%c%s%c", sysDir, PATH_DIVIDER, SYS_FOLDER_NAME, PATH_DIVIDER);
		
	}else
	{
		// for some strange reason, the last character in a path is a divider (: reps \\) or not
		// yes under MacOS
		// no under Windows
		// dunno under whatsoever
		// so well check the last character, and if its no divider, well add one.
		if( *(applicationDir+strlen(applicationDir)-1) != PATH_DIVIDER)
		{       *(applicationDir+strlen(applicationDir)) = PATH_DIVIDER;
			*(applicationDir+strlen(applicationDir)+1) ='\0';
		}
		sysDir = (char *)malloc( strlen(applicationDir) + strlen(SYS_FOLDER_NAME) + 1);
		sprintf(sysDir, "%s%s%c", applicationDir, SYS_FOLDER_NAME, PATH_DIVIDER);
	}
	
	appDir = applicationDir;
	return sysDir;
}


// name may contain a subfolder.
// use / to seperate folder and filename.
// / will be translated into PATH_DIVIDER
char *makeAbsoluteFilename( const char *path, const char *name)
{
	char *absFilename;
	char t[DIR_SIZE];
	char *p;
	size_t	l;
	
	l =  strlen(path) + strlen(name) + 1 + 1 +1;

	absFilename= (char *)malloc( l );        // add extra space for possible path divider	
	sprintf(t, "%s", path);
	fixDirName( t );
	sprintf(absFilename, "%s%s", t, name);
	
	p = absFilename;
	while(p && *p){
		p = strchr(p, '/');
		if( p ){
			*p = PATH_DIVIDER;
			p++;
		}
	}
		
	return absFilename;
}


char *makeAbsolutePath( const char *path, const char *folder)
{
	char *absPath;
	
	absPath= (char *)malloc( strlen(path) + strlen(folder) + 2 + 1);
	
	sprintf(absPath, "%s", path);
	if( *(path+strlen(path)-1) == PATH_DIVIDER)
		sprintf(absPath, "%s%s%c", path, folder, PATH_DIVIDER );
	else
		sprintf(absPath, "%s%c%s%c", path, PATH_DIVIDER, folder, PATH_DIVIDER);
	
	return absPath;
}




void printcwd( void )
{
	char s[1000];
	
	getcwd(s,1000);
	printf("dir is %s\n", s);
	
	
}
int createFolder( char *name)
{
	int ret;
	
#if WINDOWS_VERSION
	ret = mkdir( name);
#elif MAC_CLASSIC_VERSION
	ret = mkdir( name, 0);
#elif MAC_OS_X_VERSION
	ret = mkdir( name, 0777);
#else
	// todo !! ret = mkdir( name, 0777);
#endif
	return ret;
}
void restorePath( void )
{
	if( verboseOutput)
		printf("resetting application directory\n");
	chdir(applicationDir);
}


void fixDirName( char *d)
{
	size_t l;
	
	l = strlen(d);
	if( l )
		l--;
	
	//      printf("%c (%d)\n", d[l],d[l]);
	
	if( d[l] != PATH_DIVIDER)
		sprintf( d, "%s%c", d,PATH_DIVIDER);
}







