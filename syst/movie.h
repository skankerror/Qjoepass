/*
 *  movie.h
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

#ifndef __MOVIE__
#define __MOVIE__


#if defined(__cplusplus)
extern "C"
{
#endif
	
#include "mydefs.h"
	
enum	{screenshotMode, movieMode};
	
void	makeOneScreenshot( void );
void	initMovie( void );

	
typedef class movie{
	bool		theMovieScreenshotMode;	
	int			movieFrameCounter;
	bool		movieSetupFinished;
	int			screenshotMovieCounter;
	
public:
	movie( bool mode );						
	~movie( void );
	


	/* ----------------------------------------------------
	 call these routines to 
	 -   start / end movie creation  
	 -   add frames to your movie
	 -   get movie information
	 ----------------------------------------------------*/
	
	//  start / end movie recording
	bool	startMovie( bool mode );
	void 	endMovie( void );
	bool	getCompressionInfo( void );
	inline bool	isMovieSetupFinished( void ) {return movieSetupFinished; };
	// add one frame to your movie
	bool	addFrameToMovie( void );
	bool	addFrameToMovieHelper( void );
	
	void	makeOneScreenshot( void );

	// get movie length info
	long    getFramesInMovie( void);
		
	// each serie of screenshots has its own folder
	void makeNewScreenshotFolder( char *name );
	
	// tell me where the actual movie screenshots are saved
	char	*getMediaSubfolder( void );	
	
	void	saveObjectPosition( void );
	bool	compareObjectPosition( void );
	

}movie;
	
movie *getMyMovie( void );
	
/* ----------------------------------------------------
    Add thefollowing routines to you code, 
    if you want to add system dependend movie routines
    or use the routines in the folder ansi 
    to create a serie of pictures in the folder "media"
 ----------------------------------------------------*/    

// system dependend setup routines
void    _initMovie( void );
void    _exitMovie( void );
bool	getSysdepCompressionInfo( void );

// system dependend stuff for start / end movie recording 
bool	_startMovie( void );
void 	_endMovie( void );

bool	_addFrameToMovie( char *filename );

#if defined(__cplusplus)
}
#endif

#endif
