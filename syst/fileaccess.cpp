//select download folder in animation menu crashed

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "systemswitch.h"


#include "pathAccess.h"
#include "fileaccess.h"
#include "fileIO.h"
#include "applicationFolders.h"
 


char thePatternTitle[MAX_PATTERN_TITLE] = "untitled.pass";
char fullPatternPathName[MAX_FULL_FILE_NAME] = "";
char fullPatternPath[MAX_FULL_FILE_NAME] = "";
char fileSelectorPathAndName[MAX_FULL_FILE_NAME] = "";

void saveNameAndPath( char *pn);

void setPatternTitle( char *title )
{	sprintf( thePatternTitle, "%s", title);
}

// init FileIO - Services
void initIO( void )					
{

	// make fullPatternPath a valid dir, was "."
	getcwd( fullPatternPath, MAX_FULL_FILE_NAME);
	
	sprintf(fileSelectorPathAndName, "%s", fullPatternPath);   
	//~ _initIO();
	
}	


char * selectFolder( char *infoString )
{
	char 		d[MAX_FULL_FILE_NAME];
	char 		*buf = 0;
	wwwBoolean 	ret;
	
	
	getcwd(d,MAX_FULL_FILE_NAME);

	if( fullPatternPath[0] == '.') 
		getcwd( fullPatternPath, MAX_FULL_FILE_NAME);
	else
		chdir( fullPatternPath );

	//~ ret = fid( infoString);

	chdir( d );

	// user selected a file
	//~ if(	ret )
	//~ {
//		buf = readFile( fileSelectorPathAndName, "r");
//		saveNameAndPath( fileSelectorPathAndName);
//		printf("reading %s\n", fileSelectorPathAndName);
	//~ }


	return buf;
	
}

// select a file, read it and return content in a char pointer
char *selectAndLoadFile( char *infoString )
{

	char 		d[MAX_FULL_FILE_NAME];
	char 		*buf = 0;
	wwwBoolean 	ret;
	
	
	getcwd(d,MAX_FULL_FILE_NAME);

	if( fullPatternPath[0] == '.') 
		getcwd( fullPatternPath, MAX_FULL_FILE_NAME);
	else
		chdir( fullPatternPath );

	//~ ret = fio( infoString, "pass\0*.pass\0txt\0*.txt\0all\0*.*\0\0");

	chdir( d );

	// user selected a file
	//~ if(	ret )
	//~ {
		//~ buf = readFile( fileSelectorPathAndName, "r");
		//~ saveNameAndPath( fileSelectorPathAndName);
//~ //		printf("reading %s\n", fileSelectorPathAndName);
	//~ }


	return buf;

}

bool saveTextFileAs (char *fbuf, char *name, char *infoString)
{
#pragma unused (name )

	char d[MAX_FULL_FILE_NAME];
	wwwBoolean ret = false;
	
	getcwd(d,MAX_FULL_FILE_NAME);
//_myPrintf("dir is %s\n", d);
	chdir( fullPatternPath );
//_myPrintf("dir is %s\n", fullPatternPath);

	//~ ret = fis(infoString, name);

	chdir( d );
	
	

	// user selected a file
	//~ if(	ret )
	//~ {
		//~ ret = writeFile( fileSelectorPathAndName, "w", fbuf, strlen( fbuf));
		//~ saveNameAndPath( fileSelectorPathAndName);
	//~ }


	return ret;

}

char *getFileSelectorPathAndName( void )
{	return fileSelectorPathAndName;
}
	
	

// split fullPathAndName in Path and Name
// buffers for Path and Name must be big enough
void saveNameAndPath( char *pn)
{	splitPathName( pn, fullPatternPath, thePatternTitle, fullPatternPathName);
}

void splitPathName( char *in, char *path, char *name, char *pathname)
{
	char *n;
	char t;
	
	if( in == NULL )
		return;
	
	n = strrchr( in, PATH_DIVIDER);
	if( n == NULL ){
		sprintf(path, "%s", ".");
		n = in;
	}
	else{
		*n = '\0';
		sprintf(path, "%s%c", in, PATH_DIVIDER);
		*n = PATH_DIVIDER;
		n++;
    }
	// save the name
	sprintf( name, "%s", n);
	
	t = *n;
	sprintf( pathname, "%s", in);
	*n = t;
	
	//	printf("%s %s\n", fullPatternPathName, thePatternTitle);
	
}



