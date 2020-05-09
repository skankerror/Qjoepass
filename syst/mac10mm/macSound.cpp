//xxxxx
#if 01
/*
 *  macSound.cpp
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright ( C ) <1997---2011>  Wolfgang Westerboer
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 ( at your option ) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------*/

#include "systemswitch.h"

#ifdef MAC_OS_X_VERSION

#include "MoreFilesX.h"

#ifdef MAC_OS_X_VERSION
#include <QuickTime/Movies.h>
#else
#include <Movies.h>
#endif

// objective c #include <NSSound.h>

#include "quicktime.h"
#include "playSound.h"
#include "pathAccess.h"
#include "fileaccess.h"
#include "myutil.h"
#include "applicationFolders.h"


#define bellID						128
#define dropID						129
#define airID						130
#define throwID						131
#define catchID						132
#define multiCatchID				133




enum {singleThrowSound, singleCatchSound, multiThrowSound, multiCatchSound, dropSound, errorSound, bgSound, soundCount};

int soundKind = siteswapNotes;

FSSpec	mySoundFSSpec[soundCount];
Movie	theSound[soundCount];
bool	soundBusy[soundCount];

#define MaxSsSound 9
FSSpec	mySsSoundFSSpec[MaxSsSound];
Movie	theSsSound[MaxSsSound];
bool	ssSoundBusy[MaxSsSound];
char	bgSoundName[PATH_SIZE];

short	bgFileRefNum;
static	bool	bgMusicStarted = false;
bool	backgroundSoundDefined = false;

// init quicktime
// load sounds
void initSounds( void )
{
	OSStatus err;
	int c;
	char mySoundPath[PATH_SIZE];
	const char *d;
	short fileRefNum;

	initQuicktime( );
	if( !qtInstalled )
		return;
	
	d = soundPath;
		
	for( c = 0; c < soundCount; c++ )
	{
		soundBusy[c] = false;
		
		// bgSound is not handled here
		if( c != bgSound ){
			
			sprintf( mySoundPath, "%s%s%d%s", d, "sound", c,".mp3" );
			err = FSPathMakeFSSpec( ( const UInt8 * ) mySoundPath, &mySoundFSSpec[c], NULL );
			err = OpenMovieFile ( &mySoundFSSpec[c], &fileRefNum, fsRdPerm );

			if ( noErr == err ) {
				  err = NewMovieFromFile ( &theSound[c], fileRefNum, 0, nil,
					 newMovieActive, nil );
			}
			if ( noErr == err ) {
				GoToBeginningOfMovie ( theSound[c] );
			}
			
			if ( noErr == err ) {
				CloseMovieFile(fileRefNum);
			}
		}

	}
	
	for( c = 0; c < MaxSsSound; c++ )
	{
		
		sprintf( mySoundPath, "%s%s%02d%s", d, "siteswapSound", c,".mp3" );

		err = FSPathMakeFSSpec( ( const UInt8 * ) mySoundPath, &mySsSoundFSSpec[c], NULL );
		err = OpenMovieFile ( &mySsSoundFSSpec[c], &fileRefNum, fsRdPerm );
		if ( noErr == err ) {
			err = NewMovieFromFile( &theSsSound[c], fileRefNum, 0, nil,
									newMovieActive, nil );
		}
		if ( noErr == err ) {
			GoToBeginningOfMovie ( theSsSound[c] );
		}
		if ( noErr == err ) {
			CloseMovieFile(fileRefNum);
		}
		
	}
	
}

void moreSounds( void );

// play a sound, translate old resID to actual sound id
void doSound( short id, _side who, int toneID)
{
	if( !qtInstalled ){
		_myBeep( );
		return;
	}
	
	if( soundKind == siteswapNotes ){
	
		int i = max( 0,toneID );

		GoToBeginningOfMovie ( theSsSound[i] );
		StartMovie ( theSsSound[i] );
	}
	else if ( soundKind == soundNoises ){
	
		int translatedID = 128;
		switch( id ){
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
		
		
		if( !soundBusy[translatedID] ){
			GoToBeginningOfMovie ( theSound[translatedID] );
			StartMovie ( theSound[translatedID] );
		}
			
		
		if( jpPrefs.dt ){
			toneID++;
			toneID = toneID*toneID*10;
		}else
			toneID = 1000;
		
		SetMovieVolume( theSound[translatedID], toneID );
	}	
	
	moreSounds();
}


//http://stackoverflow.com/questions/9022079/how-do-you-play-an-mp3-in-a-mac-application-in-xcode-4-2
void moreSounds( void )
{
#if 0	
	NSString *resourcePath = [[NSBundle mainBundle] pathForResource:@"fn" ofType:@"mp3"];
	NSSound *sound = [[NSSound alloc] initWithContentsOfFile:resourcePath byReference:YES];
	[sound play];
#endif
}


// check if sounds are still playing
void idleSounds( void )
{
	int c;
	
	if( !qtInstalled )
		return;
	
	for( c = 0; c < soundCount; c++ )	{
		if( soundBusy[c] )
			if ( !IsMovieDone ( theSound[c] ) ){
				MoviesTask ( theSound[c], 0 );
			}else
				soundBusy[c] = false;
	}


	for( c = 0; c < MaxSsSound; c++ ){
		if( ssSoundBusy[c] )
			if ( !IsMovieDone ( theSsSound[c] ) ){
				MoviesTask ( theSsSound[c], 0 );
			}else
				ssSoundBusy[c] = false;
	}
}


bool isBackgroundSoundDefined( void )
{
	return backgroundSoundDefined;
}

void initBackgroundSound( char *name )
{
	sprintf( bgSoundName, "%s", name );
	backgroundSoundDefined = true;
}

void resetBackgroundSound( void )
{
	sprintf( bgSoundName, "%s", "" );
	backgroundSoundDefined = false;
	stopBackgroundSound( );
}
	
void playBackgroundSound( void )
{
	OSStatus err;
	char bgSoundPath[PATH_SIZE];
	char d[PATH_SIZE];
	 
	if( !qtInstalled )
		return;

	// clean up if we were playing already
	stopBackgroundSound( );
	
	// get directory
	if( fullPatternPath[0] == '.' )
		getcwd( d, PATH_SIZE );
	else {
		sprintf(d, "%s", fullPatternPath);
	}
	fixDirName( d );

	
	// get the sound file
	sprintf( bgSoundPath, "%s%s", d, bgSoundName );
	if( noErr == FSPathMakeFSSpec( ( const UInt8 * ) bgSoundPath, &mySoundFSSpec[bgSound], NULL ))
		err = OpenMovieFile ( &mySoundFSSpec[bgSound], &bgFileRefNum, fsRdPerm );
	
	// play
	if ( noErr == err )
		err = NewMovieFromFile ( &theSound[bgSound], bgFileRefNum, 0, nil,
								newMovieActive, nil );
	if ( noErr == err ) {
		GoToBeginningOfMovie ( theSound[bgSound] );
		StartMovie ( theSound[bgSound] );
		bgMusicStarted = true;
	}else
		bgMusicStarted = false;
}


void stopBackgroundSound( void )
{
	if( bgMusicStarted ){
		StopMovie(theSound[bgSound] );
		CloseMovieFile(bgFileRefNum);
		bgMusicStarted = false;
	}
}



#endif
#endif
