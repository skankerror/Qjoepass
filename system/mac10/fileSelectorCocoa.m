#if 0
//
//  fileSelectorFromCocoa.m
//  demonstrates how to call cocoa functions from cpp 
//	calls the current file selector used by NS...
//  
//  Currently not used.
// just in case the deprecated NavChooseFile function will not longer work on some day
//
//  Created by Wolfgang on 23.02.13.
//  Copyright (c) 2013 Wolfgang. All rights reserved.
//
#import <Cocoa/Cocoa.h>


#include "fileSelectorCocoa.h"
#include "fileaccess.h"
#include "macfileaccess.h"
#include "applicationFolders.h"


#ifdef UseNSOpenSavePanel

char *getPathFromNSString( NSString *path);
char *getPathAndFilenameFromNSString( NSString *pathname);


void _initIO( void )
{ // nothing to do here
}

bool fid( char *infoString)
{
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseDirectories:YES];
	[panel setCanChooseFiles:NO];
	NSString *message = [NSString stringWithUTF8String:infoString];
	[panel	setMessage: message];
	
	if ([panel runModal] != NSFileHandlingPanelOKButton) return false;
	
	NSArray *files = [panel URLs];
	getPathAndFilenameFromNSString( [[files objectAtIndex:0] path]);
	//return [[panel URLs] lastObject];
	return true;
}


bool fio( char *infoString, char *filetypes )
{
#pragma unused (filetypes)
	return callFileSelectorOpen( infoString,  fullPatternPathName  );
}

bool fis(char *infoString, char *name  ){
	return callFileSelectorSave( infoString, name);
}



// use the named path and file as default



bool callFileSelectorOpen( char *infoString, char *filePathAndName  )
{
	
	// get values
	bool ret = false;
	
    // Set-up code here.
    NSOpenPanel *panel = [NSOpenPanel openPanel];
	
	// save defaults
	// works in Mac OS 10.6 and higher
#if 0
	NSString *oss = [[NSProcessInfo processInfo] operatingSystemVersionString];
	NSLog(@"OS-Version: %@",  oss);
	char pn[MAX_FULL_FILE_NAME], p[MAX_FULL_FILE_NAME], n[MAX_FULL_FILE_NAME];
	splitPathName( filePathAndName, p, n, pn);
	NSString *name = [NSString stringWithUTF8String:n];
 	NSString *path = [NSString stringWithUTF8String:p];
	[panel setDirectory:[path stringByExpandingTildeInPath]];
    [panel setNameFieldStringValue: name];
#endif
	
	
	// not working
	NSArray *fileTypesArray;
	fileTypesArray = [NSArray arrayWithObjects:@"pass", @"main", @"txt", nil];
	//	[panel setAllowedFileTypes:fileTypesArray];
	
    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO]; // yes if more than one dir is allowed
	NSString *message = [NSString stringWithUTF8String:infoString];
	[panel	setMessage: message];
    
	//    int clicked = [panel runModal];
	int clicked = [panel runModalForTypes:fileTypesArray];
    
	if (clicked == NSFileHandlingPanelOKButton) {
		// Gets list of all files selected
		NSArray *files = [panel URLs];
		getPathAndFilenameFromNSString( [[files objectAtIndex:0] path]);
		ret = true;
	}
	
	return ret;
}

//	NSString *path = @"~/Documents";
bool callFileSelectorSave( char *infoString, char *filePathAndName  )
{
	// get values
	char pn[MAX_FULL_FILE_NAME], p[MAX_FULL_FILE_NAME], n[MAX_FULL_FILE_NAME];
	bool ret;
	NSString *path;
	
	splitPathName( filePathAndName, p, n, pn);
	
	// save defaults
	NSString *name = [NSString stringWithUTF8String:n];
	if( *p == '.' )
		path = [[[NSUserDefaults standardUserDefaults] objectForKey:@"NSNavLastRootDirectory"] stringByExpandingTildeInPath] ;
	else 
		path = [NSString stringWithUTF8String:p];
	
	
	// create panel and set values
	NSSavePanel *panel = [NSSavePanel savePanel]; 
	NSString *message = [NSString stringWithUTF8String:infoString];
	[panel	setMessage: message];
	
	[panel setDirectory:[path stringByExpandingTildeInPath]];
    [panel setNameFieldStringValue: name];
	//    [panel messageTextField: @"my new info string"];
	//	[panel setRequiredFileType:@"pass"];
	
	// bring up fileSelector
	if ([panel runModal] == NSOKButton) {
		//		NSLog(@"File + path: %@",  [panel filename]);
		char *r;
		r = [[panel filename] UTF8String];
		printf("cstring: %s\n", r );
		getPathAndFilenameFromNSString( [panel filename]);
		ret = true;
	}
	
	return ret;
}

char *getPathFromNSString( NSString *path)
{
	char *pd;
	char *r;
	r = [path UTF8String];
	sprintf( fullPatternPathName, "%s", r); 
	*pd = PATH_DIVIDER;
	
	return fullPatternPathName;
}


char *getPathAndFilenameFromNSString( NSString *pathname)
{
	char *pd;
	char *r;
	r = [pathname UTF8String];
	
	sprintf(fileSelectorPathAndName, "%s",r);
	pd = strrchr( fileSelectorPathAndName, PATH_DIVIDER);
	if( pd == NULL )
		return NULL;
	
	*pd = '\0';
	sprintf( thePatternTitle, "%s", pd+1); 
	sprintf( fullPatternPathName, "%s", fileSelectorPathAndName); 
	*pd = PATH_DIVIDER;
	
	return thePatternTitle;
}

#endif
#else
//
//  fileSelectorFromCocoa.m
//  demonstrates how to call cocoa functions from cpp 
//	calls the current file selector used by NS...
//  
//  Currently not used.
// just in case the deprecated NavChooseFile function will not longer work on some day
//
//  Created by Wolfgang on 23.02.13.
//  Copyright (c) 2013 Wolfgang. All rights reserved.
//
#import <Cocoa/Cocoa.h>


#include "fileSelectorCocoa.h"
#include "fileaccess.h"
#include "macfileaccess.h"
#include "applicationFolders.h"


#ifdef UseNSOpenSavePanel

char *getPathAndFilenameFromNSString( NSString *pathname);


void _initIO( void )
{ // nothing to do here
}

bool fio( char *infoString, char *filetypes)
{
#pragma unused (filetypes)
	return callFileSelectorOpen(  fullPatternPathName);
}

bool fis( char *infoString, char *name ){
	return callFileSelectorSave( infoString, name);
}



// use the named path and file as default



bool callFileSelectorOpen(  char *filePathAndName, bool selectOnlyDirectory )
{
	
	// get values
	bool ret = false;
	
    // Set-up code here.
    NSOpenPanel *panel = [NSOpenPanel openPanel];
	
	// save defaults
	// works in Mac OS 10.6 and higher
#if 0
	NSString *oss = [[NSProcessInfo processInfo] operatingSystemVersionString];
	NSLog(@"OS-Version: %@",  oss);
	char pn[MAX_FULL_FILE_NAME], p[MAX_FULL_FILE_NAME], n[MAX_FULL_FILE_NAME];
	splitPathName( filePathAndName, p, n, pn);
	NSString *name = [NSString stringWithUTF8String:n];
 	NSString *path = [NSString stringWithUTF8String:p];
	[panel setDirectory:[path stringByExpandingTildeInPath]];
    [panel setNameFieldStringValue: name];
#endif
	
//xxx Default runaround file can be activated. 	
	// not working
	NSArray *fileTypesArray;
	fileTypesArray = [NSArray arrayWithObjects:@"pass", @"main", @"txt", nil];
	//	[panel setAllowedFileTypes:fileTypesArray];
	
	// if user may choose not a file, he may create a directory, 
	if( selectOnlyDirectory == true ){
		[panel setCanChooseFiles:FALSE];
		[panel setCanCreateDirectories:YES];
	}
	
    [panel setCanChooseDirectories:YES];
    [panel setAllowsMultipleSelection:NO]; // yes if more than one dir is allowed
    
//    int clicked = [panel runModal];
	int clicked = [panel runModalForTypes:fileTypesArray];
    
	if (clicked == NSFileHandlingPanelOKButton) {
		// Gets list of all files selected
		NSArray *files = [panel URLs];
		getPathAndFilenameFromNSString( [[files objectAtIndex:0] path]);
		ret = true;
	}
	
	return ret;
 }

//	NSString *path = @"~/Documents";
bool callFileSelectorSave( char *infoString, char *filePathAndName)
{
	// get values
	char pn[MAX_FULL_FILE_NAME], p[MAX_FULL_FILE_NAME], n[MAX_FULL_FILE_NAME];
	bool ret;
	NSString *path;
	
	splitPathName( filePathAndName, p, n, pn);

#if 0
	// save defaults
	NSString *name = [NSString stringWithUTF8String:n];
	if( *p == '.' )
		 path = [[[NSUserDefaults standardUserDefaults] objectForKey:@"NSNavLastRootDirectory"] stringByExpandingTildeInPath] ;
	else 
		path = [NSString stringWithUTF8String:p];
	
	NSString *message = [NSString stringWithUTF8String:"my info String"];
   
	// create panel and set values
	NSSavePanel *panel = [NSSavePanel savePanel]; 

	[panel setDirectory:[path stringByExpandingTildeInPath]];
    [panel setNameFieldStringValue: name];
	[panel setCanChooseFiles:NO];
#else
	
	
		NSOpenPanel *panel = [NSOpenPanel openPanel];
		[panel setAllowsMultipleSelection:NO];
		[panel setCanChooseDirectories:YES];
		[panel setCanCreateDirectories:YES];
//		[panel setCanChooseFiles:YES];
		[panel setResolvesAliases:YES];
		
		NSString *panelTitle = NSLocalizedString(@"Choose a folder", @"Title for the open panel");
		[panel setTitle:panelTitle];
		
		NSString *promptString = NSLocalizedString(@"Choose", @"Prompt for the open panel prompt");
		[panel setPrompt:promptString];
#endif		
	//    [panel messageTextField: @"my new info string"];
//	[panel setRequiredFileType:@"pass"];
	
	// bring up fileSelector
	if ([panel runModal] == NSOKButton) {
//		NSLog(@"File + path: %@",  [panel filename]);
		char *r;
		r = [[panel filename] UTF8String];
		printf("cstring: %s\n", r );
		getPathAndFilenameFromNSString( [panel filename]);
		ret = true;
	}
	
	return ret;
}


char *getPathAndFilenameFromNSString( NSString *pathname)
{
	char *pd;
	char *r;
	r = [pathname UTF8String];
	
	sprintf(fileSelectorPathAndName, "%s",r);
	pd = strrchr( fileSelectorPathAndName, PATH_DIVIDER);
	if( pd == NULL )
		return NULL;
	
	*pd = '\0';
	sprintf( thePatternTitle, "%s", pd+1); 
	sprintf( fullPatternPathName, "%s", fileSelectorPathAndName); 
	*pd = PATH_DIVIDER;
	
	return thePatternTitle;
}

#endif
#endif
