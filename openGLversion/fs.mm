/*
 *  fs.cpp
 *  JoePass
 *
 *  Created by Wolfgang on 23.02.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#if 0
#import <Cocoa/Cocoa.h>
#import "fs.h"

@implementation fs
//-----------------
//NSOpenPanel: Displaying a File Open Dialog in OS X 10.7
//-----------------

// Any ole method
- (void)checkOpenDialog {
	
	// Loop counter.
	int i;
	
	// Create a File Open Dialog class.
	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
	
	// Set array of file types
	NSArray *fileTypesArray;
	fileTypesArray = [NSArray arrayWithObjects:@"jpg", @"gif", @"png", nil];
	
	// Enable options in the dialog.
	[openDlg setCanChooseFiles:YES];   
	[openDlg setAllowedFileTypes:fileTypesArray];
	[openDlg setAllowsMultipleSelection:TRUE];
	
	// Display the dialog box.  If the OK pressed,
	// process the files.
	if ( [openDlg runModal] == NSOKButton ) {
		
		// Gets list of all files selected
		NSArray *files = [openDlg URLs];
		
		// Loop through the files and process them.
		for( i = 0; i < [files count]; i++ ) {
			
			// Do something with the filename.
			NSLog(@"File path: %@", [[files objectAtIndex:i] path]);
			
		}
		
	}
	
}

@end

#endif
