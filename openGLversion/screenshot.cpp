/*
 *  screenshot.cpp
 *
 * 
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
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "screenshot.h"

char	*lastScreenshotName = NULL;
char	*fType = ".tga";

char	*makeScreenshotFilename( char *fName);


char *makeScreenshotFilename( char *fName){
	
	if( lastScreenshotName )
		free(lastScreenshotName);
	if( (lastScreenshotName = (char *) malloc( strlen( fName ) + strlen(fType)+1 )) == NULL )
		return NULL;
	sprintf(lastScreenshotName,"%s%s", fName, fType);
	
	return lastScreenshotName;
}

char *getLastScreenshotFilename( void ){
	return lastScreenshotName;
}

//this will save a TGA in the screenshots
//folder under incrementally numbered files
bool screenShot (char *fName, int winW, int winH)
{
	char *n;
	FILE *fp;
	
    int nSize = winW * winH * 3;

	// make my filename
	if( (n = makeScreenshotFilename( fName )) == NULL )
		return false;

    fp = fopen(n,"wb");
    
    GLubyte *pixels = new GLubyte [nSize];
    if (pixels == NULL) return false;    
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadBuffer (GL_BACK_LEFT);
	glReadPixels(0, 0, winW, winH, GL_RGB, 
				 GL_UNSIGNED_BYTE, pixels);
    // convert to BGR format
	// exchange red and blue
    unsigned char temp;
    int i = 0;
    while (i < nSize){
        temp = pixels[i];			// exchange
        pixels[i] = pixels[i+2];	// red to blue
        pixels[i+2] = temp;			// blue to red
        i += 3;						// jump to next blue
    }
	
    unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
    unsigned char header[6] = { (unsigned char) (winW%256),
                                (unsigned char) (winW/256),
                                (unsigned char) (winH%256),
                                (unsigned char) (winH/256),
                                24,0};
    
    fwrite(TGAheader, sizeof(unsigned char), 12, fp);
    fwrite(header, sizeof(unsigned char), 6, fp);
    fwrite(pixels, sizeof(GLubyte), nSize, fp);
    delete [] pixels;   
	fclose(fp);
    
    return true;
}


