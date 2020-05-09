/*
 *  cmdLine.cpp
 *  joe2
 *
 *  created by Wolfgang Westerboer on 25.10.07.
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

#include <string.h>
#include <ctype.h>

#include "gpl.h"

#include "globals.h"
#include "cmdLine.h"
#include "myPrintf.h"
#include "fileIO.h"

#include "notation.h"
#include "prefDef.h"
#include "applicationFolders.h"
#include "parseSiteswap.h"
#include "causal_editor.h"
#include "animationWin.h"
#include "world.h"

int			jugglersInCommandLinePattern = 1;
int			cmdLinePos = 0;

bool		cmdLineQuitFlag;
bool		cmdLineDoJobsAndExit;
bool		cmdLineInitMovie;
bool		cmdLineToCdew = false;
bool		verboseOutput = true;
bool		cmdLineToTextEditor = false;
bool		cmdLineCanonicalPatternDetected;
bool		cmdLineMakePostscript = false;

bool		showCmdLineFileNow;
bool		comandLineStartedPattern;
bool		commandLineDisabled = false;

bool		commandLineTriesToStartPattern = false;


bool		parseFlags( char *p );

void		printfCmdLineHelpScreen( void );

bool parseFlags( char *p )
{
	char opt;
	char c[200];
	
	if( *p++ != '-' )
		return false;
	
	// pattern
	if( isdigit( *p )){
		sprintf(c, "%c", *p);							
		jugglersInCommandLinePattern = atoi( c );
		if( verboseOutput)
			_myPrintf("generating pattern with %d juggler%s\n", jugglersInCommandLinePattern, jugglersInCommandLinePattern==1?"":"s");
		return true;
	}
	
	switch ( opt = *p++ ){
		// general
		case 'h':
		case 'H':
		case '?':
			printfCmdLineHelpScreen();
			break;
		case 'v':
			toggle(verboseOutput);
			if( verboseOutput )
				_myPrintf("verbose output = %d\n", verboseOutput);
				break;
		case 'q':
			if( verboseOutput)
				_myPrintf("quitting after cmdline execution\n");
			cmdLineQuitFlag = true;
			break;
			
			// movie	
		case 'm':
			if( verboseOutput )
				_myPrintf("creating movie from command line\n");			
			cmdLineInitMovie = true;
			break;
		case 'f':
			sprintf(c, "%.190s", p);
			jpPrefs.movieFrameRate = atol( c);
			break;
			
			// animation
		case 's':
			sprintf(c, "%.190s", p);
			jpPrefs.animationSpeed = atof( c);
			break;
		case 'g':
			float g;
			sprintf(c, "%.190s", p);
			g = atof( c);
			jpPrefs.gravity = g * g;
			break;
		case 'o':
			sprintf(c, "%.190s", p);
			jpPrefs.objectColorMode = atoi( c ) % ColorObjEnd;
			break;
		case 't':
			cmdLineToTextEditor = true;
			break;
		case 'p':
			sprintf(c, "%c", *p);							
			cmdLinePos = atoi( c );
			
			if( verboseOutput)
				_myPrintf("viewer position is %d \n", cmdLinePos);
				break;
		case 'c':
			cmdLineToCdew = true;
			if( verboseOutput)
				_myPrintf("generating causal diagram from pattern\n");
				break;
		case 'w':
			_myPrintf("%s\n", GPL_SHOW_W);
			break;
		case 'r':
			_myPrintf("%s\n", GPL_SHOW_R);
			break;
#if 01
		case 'P':
//			cmdLineToTextEditor = true;
			cmdLineMakePostscript = true;
			if( verboseOutput)
				_myPrintf("generating postscript file from causal diagram\n");
				break;
			
			
#endif				
		default:
			if( verboseOutput)
				_myPrintf("unknown option -%c\n", opt);
			break;
	}
	
	return true;
}

void parseArgs(int argc, char **argv)
{
	char *p;
	char *i;
	int c = 0;
	size_t l;
	
	
	cmdLineQuitFlag = false;
	cmdLineToCdew = false;
	verboseOutput = false;
	cmdLineToTextEditor = false;
	cmdLineCanonicalPatternDetected = false;
	
	/*
	 // dummy args for cmdLine debugging reasons
	 cmdLineQuitFlag = true;
	 cmdLineToCdew = true;
	 cmdLineMakePostscript = true;
	 cmdLineDoJobsAndExit = true;	
	 char atmp[] = "345";
	 p = atmp;
	 for (c = 1; c < 2; c++ ){
		 */
	// walk through all args higher than 0 (which contains the application name)
	for (c = 1; c < argc; c++ ){
		p = argv[c];
		
		
		if(0 && verboseOutput)
			printf("argv[%d] = %s\n",c,p);
		
		if( parseFlags( p ) )
			continue;
		
		// its a file
		i = readFile( p, "rb");
		if( i )
		{
			p = i;
			if( verboseOutput)
				_myPrintf("loading file %s\n", p);
			strncpy( thePatternTitle, p, min(strlen(p),MAX_PATTERN_TITLE) );
			writeFile( cmdLinePath, "w", i, strlen(i));
		}else{
			if(verboseOutput)
				_myPrintf("starting pattern from command line: %s\n", p);
			writeFile( cmdLinePath, "w", p, strlen(p));
		}
		
		
		// handle command line pattern
		
		commandLineTriesToStartPattern = true;
		
		
		// transcribe pattern
		// generate causal diagram
		if( jugglersInCommandLinePattern != 1 ){
			if(verboseOutput)
				_myPrintf("transcribing pattern with %d jugglers: %s\n", jugglersInCommandLinePattern, p);
			
			nHandedToAnimation( p, jugglersInCommandLinePattern*2);
			
		}else if( startOnePersonPassNotationFlag( p, true, startAnimationMode ) ){
			if(verboseOutput)
				_myPrintf("transcribing canonical pattern: %s\n",  p);
			
			cmdLineCanonicalPatternDetected = true;
			i = readFile( editPath, "rb");
			if( verboseOutput)
				_myPrintf("canonical pattern transcribed ->\n%s\n",  i);
			
			
		}else {
			char simpleModeString[] = BSTRING"\n";
			l =strlen( simpleModeString ) +  strlen( p ) + 1;
			
			if( verboseOutput)
				_myPrintf("starting vanilla siteswap pattern\n");
			
			i = (char *)malloc( l );
			if( i )
				sprintf(i, "%s%s", simpleModeString, p);
		}
		
		// start pattern
		if(i)
		{
			// default: simple mode
			initSiteswapPattern( i, strlen( i ));
			showCmdLineFileNow = false;
			//				writeFile( cmdLinePath, "w", p, strlen(p));
			
			if( !validPattern )
				showCmdLineFileNow = true;
			if(jpPrefs.showCmdLineFileInTextEditor || showCmdLineFileNow)
				writeFile( editPath, "w", p, strlen( p ));
			
			free( i );
			
			if( cmdLineToTextEditor )
				writeFile( editPath, "w", argv[c], strlen(argv[c]));
		}
		
		comandLineStartedPattern = true;
	}
	
	// some tasks can´t be handled here.
	// set flag and quit when done
	if( cmdLineQuitFlag)
		if( cmdLineInitMovie || cmdLineToCdew || cmdLineMakePostscript)	
			cmdLineDoJobsAndExit = true;
		else
			exit( 0 );
}


void printfCmdLineHelpScreen( void )
{
_myPrintf("\n\n\nJoePass! command line parameters\n"
		  "./joe -[option1]+ -[option2 parameter]+ pattern\n"
		  
		  "\noption1 are:\n"
		  "-?	this page\n"
		  "-v	verbose output\n"
		  "-q	quit after processing command line\n"
		  
		  "-m	generate movie\n"
		  "-t	open pattern in text editor\n"
		  "-c	generate causal diagram \n"
		  "-P	generate postscript, implies -c \n"
		  
		  "\noption2 are:\n"
		  "-f	i set movie frame rate to i\n"
		  "-s	f set animation speed to f\n"
		  "-g	f set gravity to f\n"
		  "-o	i object type to i\n"
		  "-p	i set viewer position to i, see menu view in animation winwdow\n"
		  "(i == integer value, f == floating point value)\n"
		  
		  "\npattern:\n"
		  "can be a file or a siteswap\n"
"\n\n\n"
		  );
}
