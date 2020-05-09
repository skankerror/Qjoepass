/*
 *  winplaysound.cpp
 *  joe2
 *
 *
 * Dummy routines for sound output.
 * if you want to add sound support for your system, make a copy of this file, and add code for your sysem
 * Replace switch UNDEFINED_VERSION (defined in systemswitch.h, more infos there)
 * See folder content of mac10mm for a working example.
 *
 * created by wolfgang 
 *//*-----------------------------------------------------------------------
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

#include "systemswitch.h"
#include "fileaccess.h"
#include "pathAccess.h"
#include "headers.h"
#include "playSound.h"
#include "myutil.h"
#include "applicationFolders.h"
#include "fileIO.h"

#if WINDOWS_VERSION

#include <windows.h>

// some old mac os relicts: resource numbers
#define bellID						128
#define dropID						129
#define airID						130
#define throwID						131
#define catchID						132
#define multiCatchID				133


enum {singleThrowSound, singleCatchSound, multiThrowSound, multiCatchSound, dropSound, errorSound, bgSound, soundCount};
char	bgSoundName[PATH_SIZE];

#define MaxSsSound 9


bool	soundToPlay;
int		soundKind;	

int		ret;
void	printMciString( char *h, int r );

#define MySoundDevice	"MySoundDevice"
void	loadSoundFiles( void );

void 	initSounds( void )
{
	char helper[DIR_SIZE];
	
	sprintf(helper, "open mpegvideo alias %s", MySoundDevice);
	ret = mciSendString(helper, NULL, NULL, NULL);
	printMciString(helper, ret );
	
	loadSoundFiles( );
	
}
			
void loadSoundFiles( void )
{
	char helper1[DIR_SIZE], helper2[DIR_SIZE];
	char channel[100];
	int c = 0;
	const char *d;
	
	d = soundPath;
	
	

	for( c = 0; c < MaxSsSound; c++ ){
		sprintf( helper1, "\"%s%s%02d%s\"", d, "siteswapSound", c,".mp3");
		sprintf(channel, "channel_0%d", c);
		sprintf(helper2,"open %s type mpegvideo alias %s", helper1, channel);
		ret = mciSendString(helper2, NULL, 0, NULL);
		printMciString(helper2, ret );

	}
}
void 	doSound(short id, _side who, int val) 						
{	
	int ret;
	char helper[PATH_SIZE], channel[100];
	//printf("playing id %d\n", id);
	if( val < 128 ){	
		sprintf(channel, "channel_0%d", val);
	}else {
		int translatedID = 128;
		switch( val ){
			case bellID:
				translatedID = errorSound;
				break;
			case dropID:
				translatedID = dropSound;
				break;
			case airID:
				translatedID = errorSound;
				break;
			case throwID:
				translatedID = singleThrowSound;
				break;
			case catchID:
				translatedID = singleCatchSound;
				break;
			case multiCatchID:
				translatedID = multiCatchSound;
				break;
			default:
				translatedID = errorSound;
				break;
		}
		sprintf(channel, "channel_0%d", translatedID);
	}		
	
	/*	sprintf(helper, "stop %s", channel);
	 ret = mciSendString(helper,NULL, NULL, NULL);
	sprintf(helper, "pause %s", channel);
	ret = mciSendString(helper,NULL, 0, NULL);
	printMciString( helper, ret );
	 */	

	sprintf(helper, "play %s from 0", channel);
	ret = mciSendString(helper,NULL, 0, NULL);
	printMciString( helper, ret );
	
}
void 	idleSounds(void)											{				}




// play bgSound
// create a new channel with every new sound played 
// (working on some bugs)
#define BG_ALIAS	"myBGSound"
int bgCounter = 0;
char bgChannel[100];

static char	helper[PATH_SIZE*2];
static int	bgMusicStarted = false;
bool	backgroundSoundDefined = false;


bool isBackgroundSoundDefined( void )
{
	return backgroundSoundDefined;
}

void initBackgroundSound( char *name )
{
	char bgSoundPath[PATH_SIZE];
	char d[PATH_SIZE];
	
	sprintf( bgChannel, "%s%d", BG_ALIAS, bgCounter);
	// clean up if we were playing already
	stopBackgroundSound();
	
	// get directory
	if( fullPatternPath[0] == '.' )
		getcwd( d, PATH_SIZE );
	else {
		sprintf(d, "%s", fullPatternPath);
	}
	fixDirName( d );
	
	
	// get the sound file
	sprintf( bgSoundName, "%s", name );
	sprintf( bgSoundPath, "%s%s", d, bgSoundName ); // just filename
	if(!fexists( bgSoundPath ))
		return;
	sprintf( bgSoundPath, "\"%s%s\"", d, bgSoundName ); // command: with paranthesis

	backgroundSoundDefined = true;
	
	
	//load audio and (in short) name it 'myFile'
	sprintf(helper,"open %s type mpegvideo alias %s", bgSoundPath, bgChannel);
	ret = mciSendString( helper,NULL, 0, NULL); 
	printMciString( helper, ret );
	
}

void resetBackgroundSound( void )
{
//	sprintf( bgSoundName, "%s", "" );
	backgroundSoundDefined = false;
}


void stopBackgroundSound( void )
{
	if( bgMusicStarted ){
		sprintf(helper,"pause %s", bgChannel);		
		ret = mciSendString(helper,NULL, 0, NULL);
		printMciString( helper, ret );

		bgCounter++;

	}
}

void	playBackgroundSound( void )									
{
	//play audio
	sprintf(helper,"play %s from 0", bgChannel);
	ret = mciSendString(helper,NULL, 0, NULL);
	printMciString( helper, ret );

	if( ret == 0 )
		bgMusicStarted = true;
	
}

void printMciString( char *h, int r )
{
	char p[PATH_SIZE];
return;
	printf( "%s\n", h);

	mciGetErrorString( r, p, PATH_SIZE);
	printf("%s\n", p );
}	


#endif
