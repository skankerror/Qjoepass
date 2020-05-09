/*
 *  languageSupport.cpp
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

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "languageSupport.h"
#include "prefs.h"
#include "applicationFolders.h"
#include "myPrintf.h"

#include "openglmenu.h"


wwwBoolean	makeLanguageDefaultStrings( void );
void dummyFunction (FILE *fp, void *d);
void writeLanguageFile( void );

#define WriteLanguageFile 1

// default menu strings
// donst change order, hard wired with entries in languageSupport.h -> enum
// first index is 0, last is languageEnd
const char *languageDefaultStrings[languageEnd] =
{
	// dialogs
	// runaround
	"close existing runaround projekt first",
	"press any key or mouse button",
	
	"open documentation",
	"quit",
	
	// runaround
	"File",
	"New",
	"Open project ...",
	"Save project",	
	"Save project as ...",
	"Select pattern ...",
	"Close",
	
	
	
	"Default Positions",				
	"Side to Side",			
	"Back to Back",			
	"Line",					
	"Dropback Line",			
	"Circle",					
	"Feed",					
	"Gorilla Feed",		
	"Rotate Juggler Positions in All Scenes",
	
	"Edit Scenes List",	
	"Copy to next field",
	"Add position at end",
	"Add position before selection",
	"Delete position at end",
	"Delete position at selection",
	"Cycle scenes clockwise",
	"Cycle scenes counterclockwise",
	"Add juggler",		
	"Remove juggler",
	"Newline",	
	"Double scenes",	
	"Halve scenes",	
	
	"Selection",
	"Select first scene",
	"Select second scene",
	"Remove scene selection",
	"Rotate Juggler Positions Clockwise",		
	"Rotate Juggler Positions Counter-Clockwise",	
	//"Edit Current Position",
	
	"Edit Selected Scene",	
	"Flip horizontal",	
	"Flip vertical",	
	//	"Exchange positions", 	// currently not used
	"Edit pattern in causal editor", 
	"Edit pattern in text editor", 
	
	"View",
	"Animated Scene", 
	"Grid",
	"Position",
	"Point of Interest",
	"Frame",	
	"Index",
	"More corners in frame",		
	"Less corners in frame",	
	"Rotate frame clockwise",
	"Rotate frame counterclockwise",
	"Scale up",		
	"Scale down",
	
	"Experts",
	"remove unused files (see doc!)",		
	"attach default pattern to all positions (see doc!)",
	
	"Run",	
	
	/*
	 "AUFRÄUMEN"	
	 "Generate default pattern",
	 */
	
	
	// info
	"Movie",
	"Movie info",
	"More frames",
	"Less frames",
	"Throws",
	"Pattern",
	"Runaround status (if available)",
	"Framerate",
	"Time",
	"Information Text (if available)",
	"Object Color",
	"Hide window",
	"Show animation window",
	"Emphase short object distance",
	
	
	// animation    
	"MainMenu",
	"Animation",
	"Toggle Animation",
	"Step Through Animation",
	"Until Next Throw",
	"Hide Window",
	"Bigger Scene",
	"Smaller Scene",
	"Bigger Objects",
	"Smaller Objects",
	"Faster",
	"Slower",
	"Higher",
	"Lower",
	"Drop",
	"Table juggling",
	"Default speed",
	"Tap speed",
	"Shorter Warning Distance",
	"Wider Warning Distance",
	"Emphase short object distance",
	"Emphase only passes",
	
	"File",
	"Edit ...",
	"Animate ...",
	"Edit and animate ...",
	"Save as ...",
	"Animation to editor",
	"Animation to external editor",
	"Save changed files",
	
	"Workspace",
	"open...",
	"save as...",
	
	"Objects",
	"Random",
	"Clubs",
	"Fishes",
	"Balls",
	"Rings",
	"Pancakes",
	"Nothing",
	"More spin",
	"Less spin",
	"Reset spin",
	
	"Color",
	"Jugglers",
	"Objects",
	
	"Environment",
	"Sky",
	"Floor",
	"Fog",
	"Unicycles",
	"Blindfolded",
	"Scene",
	"Logo",
	"Juggler ID",
	"Object ID",
	"Siteswap",
	
	"Texture",
	"Head",
	"Sky",
	"Floor",
	"Balls",
	"Clubs",
	"All textures",
	
	"Camera",
	"Front",
	"Top",
	"Bird",
	"Next",
	"Move",
	"Vomit",
	"Tess",
	"Object",
	"Reset magnification",
	
	"Abstract",
	"Axis",
	"Niveaus",
	"Camera path",
	"Hand path",
	
	"Walk",
	"Floor grid",
	"Position",	
	"Point of interest",
	"show just current movements",
	"Modifiers",
	
	"View",
	
	"Renderer",
	"Solid",
	"Solid niveaus",
	"Solid floor",
	"Smooth",
	"Antialias",
	"Look through floor",
	"toggle level of detail",
	
	"Fullscreen",	
	
	"Sound",
	"Throw",
	"Catch",
	"Kind",
	"Dynamic height",
	"Dynamic position",
	"Faults",
	
	"Movies",
	"Make movie",
	"Make screenshot series",
	"Select compressor",
	"Make screenshot",
	
	
	"Windows",
	"Info window",
	"Editor window",
	"Causal window",
	"Causal window with animation pattern",
	"Runaround window",
	"previous window",
	
	"Styles",
	
	"Positions",
	
	"Configure",
	"Markers",
	"Hotkeys",
	"Select download folder...",
    "Logfile outout",
	"Select external editor...",
	
	"Restart",
	"Open PrechacThis",
	"About",
	
	
	
	// editor
	"File",
	"Open ...",
	"Close ",
	"Save ",
	"Save as ...",
	
	"Edit",
	"Clear",
	"Capitalize",
	"Block cursor",
	"Vanilla",
	"Extended",
	
	"Animate",
	"Start pattern",
	"Start pattern 4 handed",
	"Start current line",
	"Start current line 4 handed",
	
	"Causal",
	"Generate causal on start",
	"Generate causal now",
	"Always in same window",
	"Suggest name when saving",
	
	"J2 generator",
	"Call j2",
	"J2 help page",
	
	"Show help text",
	
	// cdew
	//file
	//file:postscript
	"Postscript",
	"Save postscript as ...", 
	"Add color to postscript",
	// file:header
	"Header",
	"Open ...",
	"Save as ...",
	"Clear",
	// file:transfer
	"Transfer",
	"Start animation",
	"Always start animation",
	"Open in text editor",
	"Always open in text editor",
	// file
	"File",
	"New",
	"Duplicate",
	"Open ...",
	"Close",
	"Save",		
	"Save as ...",	
	"Save auto name",
	"Add ...",
	"Merge ...",
	"Save as template",	
	"Screenshot",    
	"Sign pattern", 
	"Show animation window",
	
	"Fix",
	"Fix connections now",	    
	"Always fix connections ",	
	"No hurries ",
	
	"Markers",
	
	//"change",
	"Background Color",
	"Reset",		
	"Arrow Color",
	"Reset",		
	"dark blue",
	"green",
	"red",
	"yellow",
	"light blue",
	"violet",
	"grey",
	"rose",
	
	"Thickness",	
	"Reset",
	"1",
	"2",
	"3",
	"4",
	
	"Repeat",
	"Start",	
	"End",
	"Discard",
	
	"defaults",
	
	
	"Modifiers",
	"Toggle beep",
	"Toggle bounce",
	"Remove all beeps",
	"Remove all bounces",
	"Half spin",
	
	"Mark",
	"Path",				
	"Pass",				
	"Right side",
	"Beat",
	"Juggler",
	"Hand",
	"Invert marks",
	
	"Action",
	"Invert height",
	"Shift left",
	"Shift right",
	"Exchange hands",
	
	"Unmark",
	"Path",
	"Beat",
	"Juggler",	
	"Hand",
	"All",
	
	"Cycle",
	"Left",
	"Right",
	"Up",
	"Down",
	"Swap 1 and 2",
	
	"Content",
	"Clear",
	"Default throw",
	"distribute throws per hand",
	"Distribute throws",
	"Rotate",
	"Reset arrow height",
	
	"Size",
	"Add juggler",	
	"Remove juggler",
	"Add beat",	
	"Remove beat",	
	"Add reverse diagram",	
	"Add symetric diagram",
	"Add same diagram",
	
	"Edit",
	"Undo",
	"Redo",
	"Cut",
	"Copy",
	"Paste",
	
	"Display",
	"Single line mode",
	"Sync line mode",
	"Color line mode",
	"Next display mode",
	"show display mode",
	"show color mode",
	
	"Color",
	"Reset user definitions",
	"Arrow length colors",
	"Causal path colors",
	"User defined color",
	"Juggler color",
	"Next color mode",
	
	"View",
	"Throws",
	"Hurries",
	"Crosses",
	"Faults",
	"Start",
	"Pattern",
	"Delays",
	"Line seperators",
	"Cell color",
	"Siteswap Display",
	"Object count",
	"Beat number",
	"Unused hands",
	"Time cursor",
	
	"General",
	"background for inactive windows",
	"Snap",
	
	"windows",
	"animation",
	"next causal"
};


char *languageStrings[languageEnd];

prefAtom languageList[] =	{
	{"---------- dialogs ---------------------", NULL, NULL, NULL}, 
	{"languageRunaroundProjectCloseDialog:= ",	readString,	writeString,	&languageStrings[languageRunaroundProjectCloseDialog]},
	{"languageHintDialog:= ",					readString,	writeString,	&languageStrings[languageHintDialog]},
	
	{"---------- All Windows ---------------------", NULL, NULL, NULL}, 
	{"languageAllMenusOpenDoc:= ",				readString,	writeString,	&languageStrings[languageAllMenusOpenDoc]},
	{"languageAllMenusQuit:= ",					readString,	writeString,	&languageStrings[languageAllMenusQuit]},
	
	
	{"---------- Runaround Window ---------------------", NULL, NULL, NULL}, 
	{"languageRunaroundMenuPositions:= ",		readString,	writeString,	&languageStrings[languageRunaroundMenuPositions]},
	{"languageRunaroundMenuFile:= ",			readString,	writeString,	&languageStrings[languageRunaroundMenuFile]},
	{"languageRunaroundFileNew:= ",				readString,	writeString,	&languageStrings[languageRunaroundFileNew]},
	{"languageRunaroundFileClose:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileClose]},
	{"languageRunaroundFileOpen:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileOpen]},
	{"languageRunaroundFileSave:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileSave]},
	{"languageRunaroundFileSaveAs:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileSaveAs]},
	{"languageRunaroundFileSelect:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileSelect]},
	//{"languageRunaroundFileDefault:= ",			readString,	writeString,	&languageStrings[languageRunaroundFileDefault]},
	//{"languageRunaroundFileEditCausal:= ",		readString,	writeString,	&languageStrings[languageRunaroundFileEditCausal]},
	//{"languageRunaroundFileEditText:= ",		readString,	writeString,	&languageStrings[languageRunaroundFileEditText]},
	
	{"languageRunaroundMenuEditScenesList:= ",		readString,	writeString,	&languageStrings[languageRunaroundMenuEditScenesList]},
	{"languageRunaroundEditCopyNext:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditCopyNext]},
	{"languageRunaroundEditAddPositionEnd:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditAddPositionEnd]},
	{"languageRunaroundEditAddPositionSelection:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditAddPositionSelection]},
	{"languageRunaroundEditDeletePositionEnd:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditDeletePositionEnd]},
	{"languageRunaroundEditDeletePositionSelection:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditDeletePositionSelection]},
	{"languageRunaroundEditAddJuggler:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditAddJuggler]},
	{"languageRunaroundEditRemoveJuggler:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditRemoveJuggler]},
	{"languageRunaroundEditNewline:= ",			readString,	writeString,	&languageStrings[languageRunaroundEditNewline]},
	{"languageRunaroundEditDouble:= ",			readString,	writeString,	&languageStrings[languageRunaroundEditDouble]},
	{"languageRunaroundEditHalve:= ",			readString,	writeString,	&languageStrings[languageRunaroundEditHalve]},
	
	//{"languageRunaroundMenuEditCurrentScene:= ", readString,	writeString,	&languageStrings[languageRunaroundMenuEditCurrentScene]},
	//{"languageRunaroundEditHorizontal:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditHorizontal]},
	//{"languageRunaroundEditVertical:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditVertical]},
	//{"languageRunaroundEditXpositions:= ",		readString,	writeString,	&languageStrings[languageRunaroundEditXpositions]},
	{"languageRunaroundEditCycleClockwise:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditCycleClockwise]},
	{"languageRunaroundEditCycleCounterclockwise:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditCycleCounterclockwise]},
	
	
	{"languageRunaroundMenuView:= ",			readString,	writeString,	&languageStrings[languageRunaroundMenuView]},
	{"languageRunaroundViewGrid:= ",			readString,	writeString,	&languageStrings[languageRunaroundViewGrid]},
	{"languageRunaroundViewPos:= ",				readString,	writeString,	&languageStrings[languageRunaroundViewPos]},
	{"languageRunaroundViewPoi:= ",				readString,	writeString,	&languageStrings[languageRunaroundViewPoi]},
	{"languageRunaroundViewShowFrame:= ",		readString,	writeString,	&languageStrings[languageRunaroundViewShowFrame]},
	{"languageRunaroundViewShowIndex:= ",		readString,	writeString,	&languageStrings[languageRunaroundViewShowIndex]},
	{"languageRunaroundViewMoreCorners:= ",		readString,	writeString,	&languageStrings[languageRunaroundViewMoreCorners]},
	{"languageRunaroundViewLessCorners:= ",		readString,	writeString,	&languageStrings[languageRunaroundViewLessCorners]},
	{"languageRunaroundViewRotateFrameCW:= ",	readString,	writeString,	&languageStrings[languageRunaroundViewRotateFrameCW]},
	{"languageRunaroundViewRotateFrameCCW:= ",	readString,	writeString,	&languageStrings[languageRunaroundViewRotateFrameCCW]},
	{"languageRunaroundViewScaleUp:= ",			readString,	writeString,	&languageStrings[languageRunaroundViewScaleUp]},
	{"languageRunaroundViewScaleDown:= ",		readString,	writeString,	&languageStrings[languageRunaroundViewScaleDown]},
	{"languageRunaroundMenuRemoveUnused:= ",	readString,	writeString,	&languageStrings[languageRunaroundMenuRemoveUnused]},
	
	{"languageRunaroundMenuPositions:= ",		readString,	writeString,	&languageStrings[languageRunaroundMenuPositions]},
	{"languageRunaroundPositionsSideToSide:= ",	readString,	writeString,	&languageStrings[languageRunaroundPositionsSideToSide]},
	{"languageRunaroundPositionsBackToBack:= ",	readString,	writeString,	&languageStrings[languageRunaroundPositionsBackToBack]},
	{"languageRunaroundPositionsLine:= ",		readString,	writeString,	&languageStrings[languageRunaroundPositionsLine]},
	{"languageRunaroundPositionsDropbackLine:= ",	readString,	writeString,	&languageStrings[languageRunaroundPositionsDropbackLine]},
	{"languageRunaroundPositionsCircle:= ",		readString,	writeString,	&languageStrings[languageRunaroundPositionsCircle]},
	{"languageRunaroundPositionsFeed:= ",		readString,	writeString,	&languageStrings[languageRunaroundPositionsFeed]},
	{"languageRunaroundPositionsGorillaFeed:= ",readString,	writeString,	&languageStrings[languageRunaroundPositionsGorillaFeed]},
	{"languageRunaroundPositionsRotateAllPositions:= ",readString,	writeString,	&languageStrings[languageRunaroundPositionsRotateAllPositions]},
	{"languageRunaroundEditRotateClockwise:= ",readString,	writeString,	&languageStrings[languageRunaroundEditRotateClockwise]},
	{"languageRunaroundEditRotateCounterClockwise:= ",	readString,	writeString,	&languageStrings[languageRunaroundEditRotateCounterClockwise]},
	
	{"languageRunaroundMenuRun:= ",				readString,	writeString,	&languageStrings[languageRunaroundMenuRun]},
	
	
	{"---------- Information Window ---------------------", NULL, NULL, NULL}, 
	{"languageInfoMenuMovie:= ",				readString,	writeString,	&languageStrings[languageInfoMenuMovie]},
	{"languageInfoMovieShowInfo:= ",			readString,	writeString,	&languageStrings[languageInfoMovieShowInfo]},
	{"languageInfoMovieMoreFrames:= ",			readString,	writeString,	&languageStrings[languageInfoMovieMoreFrames]},
	{"languageInfoMovieLessFrames:= ",			readString,	writeString,	&languageStrings[languageInfoMovieLessFrames]},
	
	{"languageInfoThrows:= ",					readString,	writeString,	&languageStrings[languageInfoThrows]},
	{"languageInfoPattern:= ",					readString,	writeString,	&languageStrings[languageInfoPattern]},
	{"languageInfoRunaround:= ",				readString,	writeString,	&languageStrings[languageInfoRunaround]},
	{"languageInfoFramerate:= ",				readString,	writeString,	&languageStrings[languageInfoFramerate]},
	{"languageInfoTime:= ",						readString,	writeString,	&languageStrings[languageInfoTime]},
	{"languageInfoText:= ",						readString,	writeString,	&languageStrings[languageInfoText]},
	{"languageInfohideWindow:= ",				readString,	writeString,	&languageStrings[languageInfohideWindow]},
	{"languageInfoShowAnimation:= ",			readString,	writeString,	&languageStrings[languageInfoShowAnimation]},
	
	{"---------- Animation Window ---------------------", NULL, NULL, NULL}, 
	{"languageAnimMenu:= ",						readString,	writeString,	&languageStrings[languageAnimMenu]},
	{"languageAnimMenuAnim:= ",					readString,	writeString,	&languageStrings[languageAnimMenuAnim]},
	{"languageAnimAnimAnimate:= ",				readString,	writeString,	&languageStrings[languageAnimAnimAnimate]},
	{"languageAnimAnimStep:= ",					readString,	writeString,	&languageStrings[languageAnimAnimStep]},
	{"languageAnimAnimThrow:= ",				readString,	writeString,	&languageStrings[languageAnimAnimThrow]},
	{"languageAnimAnimShow:= ",					readString,	writeString,	&languageStrings[languageAnimAnimShow]},
	{"languageAnimAnimBigger:= ",				readString,	writeString,	&languageStrings[languageAnimAnimBigger]},
	{"languageAnimAnimSmaller:= ",				readString,	writeString,	&languageStrings[languageAnimAnimSmaller]},
	{"languageAnimAnimFaster:= ",				readString,	writeString,	&languageStrings[languageAnimAnimFaster]},
	{"languageAnimAnimSlower:= ",				readString,	writeString,	&languageStrings[languageAnimAnimSlower]},
	{"languageAnimAnimHigher:= ",				readString,	writeString,	&languageStrings[languageAnimAnimHigher]},
	{"languageAnimAnimLower:= ",				readString,	writeString,	&languageStrings[languageAnimAnimLower]},
	{"languageAnimAnimDrop:= ",					readString,	writeString,	&languageStrings[languageAnimAnimDrop]},
	{"languageAnimAnimTableJuggling:= ",		readString,	writeString,	&languageStrings[languageAnimAnimTableJuggling]},
	{"languageAnimAnimDefault:= ",				readString,	writeString,	&languageStrings[languageAnimAnimDefault]},
	{"languageAnimAnimTap:= ",					readString,	writeString,	&languageStrings[languageAnimAnimDefault]},
	{"languageAnimAnimWarningNarrow:= ",		readString,	writeString,	&languageStrings[languageAnimAnimWarningNarrow]},
	{"languageAnimAnimWarningWide:= ",			readString,	writeString,	&languageStrings[languageAnimAnimWarningWide]},
	{"languageAnimAnimShortDistance:= ",		readString,	writeString,	&languageStrings[languageAnimAnimShortDistance]},
	{"languageAnimAnimShortDistanceForSelfs:= ",readString,	writeString,	&languageStrings[languageAnimAnimShortDistanceForSelfs]},
	
	{"languageAnimMenuFile:= ",					readString,	writeString,	&languageStrings[languageAnimMenuFile]},
	{"languageAnimFileEdit:= ",					readString,	writeString,	&languageStrings[languageAnimFileEdit]},
	{"languageAnimFileAnimate:= ",				readString,	writeString,	&languageStrings[languageAnimFileAnimate]},
	{"languageAnimFileEditPlus:= ",				readString,	writeString,	&languageStrings[languageAnimFileEditPlus]},
	{"languageAnimFileSaveAs:= ",				readString,	writeString,	&languageStrings[languageAnimFileSaveAs]},
	{"languageAnimFileToEditor:= ",				readString,	writeString,	&languageStrings[languageAnimFileToEditor]},
	{"languageAnimFileToExternalEditor:= ",		readString,	writeString,	&languageStrings[languageAnimFileToExternalEditor]},
	{"languageAnimFileSaveChanged:= ",			readString,	writeString,	&languageStrings[languageAnimFileSaveChanged]},
	
	{"languageAnimMenuWorkspace:= ",			readString,	writeString,	&languageStrings[languageAnimMenuWorkspace]},
	{"languageAnimWorkspaceOpen:= ",			readString,	writeString,	&languageStrings[languageAnimWorkspaceOpen]},
	{"languageAnimWorkspaceSaveAs:= ",			readString,	writeString,	&languageStrings[languageAnimWorkspaceSaveAs]},
	
	
	{"languageAnimMenuObject:= ",				readString,	writeString,	&languageStrings[languageAnimMenuObject]},
	{"languageAnimObjectRandom:= ",				readString,	writeString,	&languageStrings[languageAnimObjectRandom]},
	{"languageAnimObjectClubs:= ",				readString,	writeString,	&languageStrings[languageAnimObjectClubs]},
	{"languageAnimObjectFishes:= ",				readString,	writeString,	&languageStrings[languageAnimObjectFishes]},
	{"languageAnimObjectBalls:= ",				readString,	writeString,	&languageStrings[languageAnimObjectBalls]},
	{"languageAnimObjectRings:= ",				readString,	writeString,	&languageStrings[languageAnimObjectRings]},
	{"languageAnimObjectPancakes:= ",			readString,	writeString,	&languageStrings[languageAnimObjectPancakes]},
	{"languageAnimObjectNothing:= ",			readString,	writeString,	&languageStrings[languageAnimObjectNothing]},
	{"languageAnimObjectMoreSpin:= ",			readString,	writeString,	&languageStrings[languageAnimObjectMoreSpin]},
	{"languageAnimObjectLessSpin:= ",			readString,	writeString,	&languageStrings[languageAnimObjectLessSpin]},
	{"languageAnimObjectResetSpin:= ",			readString,	writeString,	&languageStrings[languageAnimObjectResetSpin]},
	
	{"languageAnimMenuColor:= ",				readString,	writeString,	&languageStrings[languageAnimMenuColor]},
	{"languageAnimColorJugglers:= ",			readString,	writeString,	&languageStrings[languageAnimColorJugglers]},
	{"languageAnimColorObjects:= ",				readString,	writeString,	&languageStrings[languageAnimColorObjects]},
	
	{"languageAnimMenuEnvironment:= ",			readString,	writeString,	&languageStrings[languageAnimMenuEnvironment]},
	{"languageAnimEnvironmentSky:= ",			readString,	writeString,	&languageStrings[languageAnimEnvironmentSky]},
	{"languageAnimEnvironmentFloor:= ",			readString,	writeString,	&languageStrings[languageAnimEnvironmentFloor]},
	{"languageAnimEnvironmentFog:= ",			readString,	writeString,	&languageStrings[languageAnimEnvironmentFog]},
	{"languageAnimEnvironmentUnicycles:= ",		readString,	writeString,	&languageStrings[languageAnimEnvironmentUnicycles]},
	{"languageAnimEnvironmentBlindfolded:= ",	readString,	writeString,	&languageStrings[languageAnimEnvironmentBlindfolded]},
	{"languageAnimEnvironmentScene:= ",			readString,	writeString,	&languageStrings[languageAnimEnvironmentScene]},
	{"languageAnimEnvironmentLogo:= ",			readString,	writeString,	&languageStrings[languageAnimEnvironmentLogo]},
	{"languageAnimEnvironmentJugglerId:= ",		readString,	writeString,	&languageStrings[languageAnimEnvironmentJugglerId]},
	{"languageAnimEnvironmentObjectId:= ",		readString,	writeString,	&languageStrings[languageAnimEnvironmentObjectId]},
	
	{"languageAnimMenuTexture:= ",				readString,	writeString,	&languageStrings[languageAnimMenuTexture]},
	{"languageAnimTextureHead:= ",				readString,	writeString,	&languageStrings[languageAnimTextureHead]},
	{"languageAnimTextureSky:= ",				readString,	writeString,	&languageStrings[languageAnimTextureSky]},
	{"languageAnimTextureFloor:= ",				readString,	writeString,	&languageStrings[languageAnimTextureFloor]},
	{"languageAnimTextureBalls:= ",				readString,	writeString,	&languageStrings[languageAnimTextureBalls]},
	{"languageAnimTextureClubs:= ",				readString,	writeString,	&languageStrings[languageAnimTextureClubs]},
	{"languageAnimTextureAllTextures:= ",		readString,	writeString,	&languageStrings[languageAnimTextureAllTextures]},
	
	{"languageAnimMenuCamera:= ",				readString,	writeString,	&languageStrings[languageAnimMenuCamera]},
	{"languageAnimCameraFront:= ",				readString,	writeString,	&languageStrings[languageAnimCameraFront]},
	{"languageAnimCameraTop:= ",				readString,	writeString,	&languageStrings[languageAnimCameraTop]},
	{"languageAnimCameraBird:= ",				readString,	writeString,	&languageStrings[languageAnimCameraBird]},
	{"languageAnimCameraNext:= ",				readString,	writeString,	&languageStrings[languageAnimCameraNext]},
	{"languageAnimCameraMove:= ",				readString,	writeString,	&languageStrings[languageAnimCameraMove]},
	{"languageAnimCameraVomit:= ",				readString,	writeString,	&languageStrings[languageAnimCameraVomit]},
	{"languageAnimCameraTess:= ",				readString,	writeString,	&languageStrings[languageAnimCameraTess]},
	{"languageAnimCameraObject:= ",				readString,	writeString,	&languageStrings[languageAnimCameraObject]},
	{"languageAnimCameraResetMag:= ",			readString,	writeString,	&languageStrings[languageAnimCameraResetMag]},
	
	{"languageAnimMenuAbstract:= ",				readString,	writeString,	&languageStrings[languageAnimMenuAbstract]},
	{"languageAnimAbstractAxis:= ",				readString,	writeString,	&languageStrings[languageAnimAbstractAxis]},
	{"languageAnimAbstractNiveaus:= ",			readString,	writeString,	&languageStrings[languageAnimAbstractNiveaus]},
	{"languageAnimAbstractCameraPath:= ",		readString,	writeString,	&languageStrings[languageAnimAbstractCameraPath]},
	{"languageAnimAbstractHandPath:= ",			readString,	writeString,	&languageStrings[languageAnimAbstractHandPath]},
	
	{"languageAnimMenuWalk:= ",					readString,	writeString,	&languageStrings[languageAnimMenuWalk]},
	{"languageAnimWalkGrid:= ",					readString,	writeString,	&languageStrings[languageAnimWalkGrid]},
	{"languageAnimWalkPos:= ",					readString,	writeString,	&languageStrings[languageAnimWalkPos]},
	{"languageAnimWalkPoi:= ",					readString,	writeString,	&languageStrings[languageAnimWalkPoi]},
	{"languageAnimWalkAllPoints:= ",			readString,	writeString,	&languageStrings[languageAnimWalkAllPoints]},
	{"languageAnimWalkModifiers:= ",			readString,	writeString,	&languageStrings[languageAnimWalkModifiers]},
	
	{"languageAnimMenuView:= ",					readString,	writeString,	&languageStrings[languageAnimMenuView]},
	
	{"languageAnimMenuRenderer:= ",				readString,	writeString,	&languageStrings[languageAnimMenuRenderer]},
	{"languageAnimRendererSolid:= ",			readString,	writeString,	&languageStrings[languageAnimRendererSolid]},
	{"languageAnimRendererSolidNiveaus:= ",		readString,	writeString,	&languageStrings[languageAnimRendererSolidNiveaus]},
	{"languageAnimRendererSolidFloor:= ",		readString,	writeString,	&languageStrings[languageAnimRendererSolidFloor]},
	{"languageAnimRendererSmooth:= ",			readString,	writeString,	&languageStrings[languageAnimRendererSmooth]},
	{"languageAnimRendererAntialias:= ",		readString,	writeString,	&languageStrings[languageAnimRendererAntialias]},
	{"languageAnimRendererLookThroughFloor:= ",	readString,	writeString,	&languageStrings[languageAnimRendererLookThroughFloor]},
	{"languageAnimRendererDetails:= ",			readString,	writeString,	&languageStrings[languageAnimRendererDetails]},
	
	{"languageAnimMenuFullScreen:= ",			readString,	writeString,	&languageStrings[languageAnimMenuFullScreen]},
	
	{"languageAnimMenuSound:= ",				readString,	writeString,	&languageStrings[languageAnimMenuSound]},
	{"languageAnimSoundThrow:= ",				readString,	writeString,	&languageStrings[languageAnimSoundThrow]},
	{"languageAnimSoundCatch:= ",				readString,	writeString,	&languageStrings[languageAnimSoundCatch]},
	{"languageAnimSoundKind:= ",				readString,	writeString,	&languageStrings[languageAnimSoundKind]},
	{"languageAnimSoundDynamicHeight:= ",		readString,	writeString,	&languageStrings[languageAnimSoundDynamicHeight]},
	{"languageAnimSoundDynamicPosition:= ",		readString,	writeString,	&languageStrings[languageAnimSoundDynamicPosition]},
	{"languageAnimSoundFaults:= ",				readString,	writeString,	&languageStrings[languageAnimSoundFaults]},
	
	{"languageAnimMenuMedia:= ",				readString,	writeString,	&languageStrings[languageAnimMenuMedia]},
	{"languageAnimMediaMakeMovie:= ",			readString,	writeString,	&languageStrings[languageAnimMediaMakeMovie]},
	{"languageAnimMediaSelectCompressor:= ",	readString,	writeString,	&languageStrings[languageAnimMediaSelectCompressor]},
	{"languageAnimMediaMakeScreenshot:= ",		readString,	writeString,	&languageStrings[languageAnimMediaMakeScreenshot]},
	
	{"languageAnimMenuWindows:= ",				readString,	writeString,	&languageStrings[languageAnimMenuWindows]},
	{"languageAnimWindowsInfoWindow:= ",		readString,	writeString,	&languageStrings[languageAnimWindowsInfoWindow]},
	{"languageAnimWindowsEditorWindow:= ",		readString,	writeString,	&languageStrings[languageAnimWindowsEditorWindow]},
	{"languageAnimWindowsCausalWindow:= ",		readString,	writeString,	&languageStrings[languageAnimWindowsCausalWindow]},
	{"languageAnimWindowsCausalDuplicate:= ",	readString,	writeString,	&languageStrings[languageAnimWindowsCausalDuplicate]},
	{"languageAnimWindowsRunaroundWindow:= ",	readString,	writeString,	&languageStrings[languageAnimWindowsRunaroundWindow]},
	{"languageAnimWindowsPreviousWindow:= ",	readString,	writeString,	&languageStrings[languageAnimWindowsPreviousWindow]},
	
	{"languageAnimMenuStyles:= ",				readString,	writeString,	&languageStrings[languageAnimMenuStyles]},
	{"languageAnimMenuPositions:= ",			readString,	writeString,	&languageStrings[languageAnimMenuPositions]},
	
	{"languageAnimMenuConfigure:= ",			readString,	writeString,	&languageStrings[languageAnimMenuConfigure]},
	{"languageAnimMenuMarkers:= ",				readString,	writeString,	&languageStrings[languageAnimMenuMarkers]},
	{"languageAnimMenuHotkeys:= ",				readString,	writeString,	&languageStrings[languageAnimMenuHotkeys]},
	{"languageAnimMenuWatchfile:= ",			readString,	writeString,	&languageStrings[languageAnimMenuWatchfile]},
	{"languageAnimMenuLogfile:= ",              readString,	writeString,	&languageStrings[languageAnimMenuLogfile]},
	{"languageAnimMenuExternalEditor:= ",		readString,	writeString,	&languageStrings[languageAnimMenuExternalEditor]},
	
	{"languageAnimFileOpenPrechacthis:= ",		readString,	writeString,	&languageStrings[languageAnimFileOpenPrechacthis]},
	{"languageAnimMenuAbout:= ",				readString,	writeString,	&languageStrings[languageAnimMenuAbout]},
	
	{"----------Editor Window ---------------------", NULL, NULL, NULL}, 
	{"languageEditMenuFile:= ",					readString,	writeString,	&languageStrings[languageEditMenuFile]},
	{"languageEditFileOpen:= ",					readString,	writeString,	&languageStrings[languageEditFileOpen]},
	{"languageEditFileClose:= ",				readString,	writeString,	&languageStrings[languageEditFileClose]},
	{"languageEditFileSave:= ",					readString,	writeString,	&languageStrings[languageEditFileSave]},
	{"languageEditFileSaveAs:= ",				readString,	writeString,	&languageStrings[languageEditFileSaveAs]},
	
	
	{"languageEditMenuEdit:= ",					readString,	writeString,	&languageStrings[languageEditMenuEdit]},
	{"languageEditClear:= ",					readString,	writeString,	&languageStrings[languageEditClear]},
	{"languageEditCapitalize:= ",				readString,	writeString,	&languageStrings[languageEditCapitalize]},
	{"languageEditBlockCursor:= ",				readString,	writeString,	&languageStrings[languageEditBlockCursor]},
	{"languageEditVanilla:= ",					readString,	writeString,	&languageStrings[languageEditVanilla]},
	{"languageEditExtended:= ",					readString,	writeString,	&languageStrings[languageEditExtended]},
	
	{"languageEditMenuSiteswap:= ",				readString,	writeString,	&languageStrings[languageEditMenuSiteswap]},
	{"languageEditPattern:= ",					readString,	writeString,	&languageStrings[languageEditPattern]},
	{"languageEditFourHanded:= ",				readString,	writeString,	&languageStrings[languageEditFourHanded]},
	{"languageEditCurentLine:= ",				readString,	writeString,	&languageStrings[languageEditCurentLine]},
	{"languageEditCurentLineFour:= ",			readString,	writeString,	&languageStrings[languageEditCurentLineFour]},
	
	{"languageEditMenuCausal:= ",				readString,	writeString,	&languageStrings[languageEditMenuCausal]},
	{"languageEditGenerateCausalOnStart:= ",	readString,	writeString,	&languageStrings[languageEditGenerateCausalOnStart]},
	{"languageEditGenerateCausalNow:= ",		readString,	writeString,	&languageStrings[languageEditGenerateCausalNow]},
	{"languageEditCausalSameWindow:= ",			readString,	writeString,	&languageStrings[languageEditCausalSameWindow]},
	{"languageEditAutoSaveCausal:= ",			readString,	writeString,	&languageStrings[languageEditAutoSaveCausal]},
	
	{"languageEditMenuJ2:= ",					readString,	writeString,	&languageStrings[languageEditMenuJ2]},
	{"languageEditCallJ2:= ",					readString,	writeString,	&languageStrings[languageEditCallJ2]},
	{"languageEditJ2Help:= ",					readString,	writeString,	&languageStrings[languageEditJ2Help]},
	
	{"languageEditMenuHelp:= ",					readString,	writeString,	&languageStrings[languageEditMenuHelp]},
	
	
	{"---------- Causal Window ---------------------", NULL, NULL, NULL}, 
	{"languageCdewMenuPS:= ",					readString,	writeString,	&languageStrings[languageCdewMenuPS]},
	{"languageCdewPSSaveAs:= ",					readString,	writeString,	&languageStrings[languageCdewPSSaveAs]},
	{"languageCdewPSColor:= ",					readString,	writeString,	&languageStrings[languageCdewPSColor]},
	
	{"languageCdewMenuHeader:= ",				readString,	writeString,	&languageStrings[languageCdewMenuHeader]},
	{"languageCdewHeaderOpen:= ",				readString,	writeString,	&languageStrings[languageCdewHeaderOpen]},
	{"languageCdewHeaderSaveAs:= ",				readString,	writeString,	&languageStrings[languageCdewHeaderSaveAs]},
	{"languageCdewHeaderClear:= ",				readString,	writeString,	&languageStrings[languageCdewHeaderClear]},
	
	{"languageCdewMenuTransfer:= ",				readString,	writeString,	&languageStrings[languageCdewMenuTransfer]},
	{"languageCdewStart:= ",					readString,	writeString,	&languageStrings[languageCdewStart]},
	{"languageCdewAlwaysStart:= ",				readString,	writeString,	&languageStrings[languageCdewAlwaysStart]},
	{"languageCdewOpenText:= ",					readString,	writeString,	&languageStrings[languageCdewOpenText]},
	{"languageCdewAlwaysOpenText:= ",			readString,	writeString,	&languageStrings[languageCdewAlwaysOpenText]},
	
	{"languageCdewMenuFile:= ",					readString,	writeString,	&languageStrings[languageCdewMenuFile]},
	{"languageCdewFileNew:= ",					readString,	writeString,	&languageStrings[languageCdewFileNew]},
	{"languageCdewFileDuplicate:= ",			readString,	writeString,	&languageStrings[languageCdewFileDuplicate]},
	{"languageCdewFileOpen:= ",					readString,	writeString,	&languageStrings[languageCdewFileOpen]},
	{"languageCdewFileClose:= ",				readString,	writeString,	&languageStrings[languageCdewFileClose]},
	{"languageCdewFileSave:= ",					readString,	writeString,	&languageStrings[languageCdewFileSave]},
	{"languageCdewFileSaveAs:= ",				readString,	writeString,	&languageStrings[languageCdewFileSaveAs]},
	{"languageCdewFileSaveAutoName:= ",			readString,	writeString,	&languageStrings[languageCdewFileSaveAutoName]},
	{"languageCdewFileAdd:= ",					readString,	writeString,	&languageStrings[languageCdewFileAdd]},
	{"languageCdewFileMerge:= ",				readString,	writeString,	&languageStrings[languageCdewFileMerge]},
	{"languageCdewFileSaveTemplate:= ",			readString,	writeString,	&languageStrings[languageCdewFileSaveTemplate]},
	{"languageCdewFileScreenshot:= ",			readString,	writeString,	&languageStrings[languageCdewFileScreenshot]},
	{"languageCdewFileSign:= ",					readString,	writeString,	&languageStrings[languageCdewFileSign]},
	{"languageCdewFileShow:= ",					readString,	writeString,	&languageStrings[languageCdewFileShow]},
	
	{"languageCdewMenuMarkers:= ",				readString,	writeString,	&languageStrings[languageCdewMenuMarkers]},
	
	//{"languageCdewMenuMarkArrowStyle:= ",		readString,	writeString,	&languageStrings[languageCdewMenuMarkArrowStyle]},
	{"languageCdewMenuMarkDefaults:= ",			readString,	writeString,	&languageStrings[languageCdewMarkArrowDefaults]},
	
	{"languageCdewMarkArrowColor:= ",			readString,	writeString,	&languageStrings[languageCdewMarkArrowColor]},
	{"languageCdewMarkArrowThickness:= ",		readString,	writeString,	&languageStrings[languageCdewMarkArrowThickness]},
	
	{"languageCdewMarkRepeat:= ",	        	readString,	writeString,	&languageStrings[languageCdewMarkRepeat]},
	{"languageCdewMarkRepeatStart:= ",		    readString,	writeString,	&languageStrings[languageCdewMarkRepeatStart]},
	{"languageCdewMarkRepeatEnd:= ",	    	readString,	writeString,	&languageStrings[languageCdewMarkRepeatEnd]},
	{"languageCdewMarkRepeatDelete:= ",	    	readString,	writeString,	&languageStrings[languageCdewMarkRepeatDelete]},
	
	{"languageCdewMenuFix:= ",					readString,	writeString,	&languageStrings[languageCdewMenuFix]},
	{"languageCdewFixNow:= ",					readString,	writeString,	&languageStrings[languageCdewFixNow]},
	{"languageCdewFixAlways:= ",				readString,	writeString,	&languageStrings[languageCdewFixAlways]},
	{"languageCdewFixNoHurries:= ",				readString,	writeString,	&languageStrings[languageCdewFixNoHurries]},
	
	{"languageCdewMenuModifier:= ",				readString,	writeString,	&languageStrings[languageCdewMenuModifier]},
	{"languageCdewModifierBeep:= ",				readString,	writeString,	&languageStrings[languageCdewModifierBeep]},
	{"languageCdewModifierBounce:= ",			readString,	writeString,	&languageStrings[languageCdewModifierBounce]},
	{"languageCdewModifierNoBeep:= ",			readString,	writeString,	&languageStrings[languageCdewModifierNoBeep]},
	{"languageCdewModifierNoBounce:= ",			readString,	writeString,	&languageStrings[languageCdewModifierNoBounce]},
	{"languageCdewModifierHalfspin:= ",			readString,	writeString,	&languageStrings[languageCdewModifierHalfspin]},
	
	{"languageCdewMenuMark:= ",					readString,	writeString,	&languageStrings[languageCdewMenuMark]},
	{"languageCdewMarkPath:= ",					readString,	writeString,	&languageStrings[languageCdewMarkPath]},
	{"languageCdewMarkPass:= ",					readString,	writeString,	&languageStrings[languageCdewMarkPass]},
	{"languageCdewMarkRight:= ",				readString,	writeString,	&languageStrings[languageCdewMarkRight]},
	{"languageCdewMarkBeat:= ",					readString,	writeString,	&languageStrings[languageCdewMarkBeat]},
	{"languageCdewMarkJuggler:= ",				readString,	writeString,	&languageStrings[languageCdewMarkJuggler]},
	{"languageCdewMarkHand:= ",					readString,	writeString,	&languageStrings[languageCdewMarkHand]},
	{"languageCdewMarkInvert:= ",				readString,	writeString,	&languageStrings[languageCdewMarkInvert]},
	
	{"languageCdewMenuAction:= ",				readString,	writeString,	&languageStrings[languageCdewMenuAction]},
	{"languageCdewMarkInvertHeight:= ",			readString,	writeString,	&languageStrings[languageCdewMarkInvertHeight]},
	{"languageCdewMarkShiftLeft:= ",			readString,	writeString,	&languageStrings[languageCdewMarkShiftLeft]},
	{"languageCdewMarkShiftRight:= ",			readString,	writeString,	&languageStrings[languageCdewMarkShiftRight]},
	{"languageCdewMarkXHands:= ",				readString,	writeString,	&languageStrings[languageCdewMarkXHands]},
	
	{"languageCdewMenuUnmark:= ",				readString,	writeString,	&languageStrings[languageCdewMenuUnmark]},
	{"languageCdewUnmarkPath:= ",				readString,	writeString,	&languageStrings[languageCdewUnmarkPath]},
	{"languageCdewUnmarkBeat:= ",				readString,	writeString,	&languageStrings[languageCdewUnmarkBeat]},
	{"languageCdewUnmarkJuggler:= ",			readString,	writeString,	&languageStrings[languageCdewUnmarkJuggler]},
	{"languageCdewUnmarkHand:= ",				readString,	writeString,	&languageStrings[languageCdewUnmarkHand]},
	{"languageCdewUnmarkAll:= ",				readString,	writeString,	&languageStrings[languageCdewUnmarkAll]},
	
	{"languageCdewMenuCycle:= ",				readString,	writeString,	&languageStrings[languageCdewMenuCycle]},
	{"languageCdewCycleLeft:= ",				readString,	writeString,	&languageStrings[languageCdewCycleLeft]},
	{"languageCdewCycleRight:= ",				readString,	writeString,	&languageStrings[languageCdewCycleRight]},
	{"languageCdewCycleUp:= ",					readString,	writeString,	&languageStrings[languageCdewCycleUp]},
	{"languageCdewCycleDown:= ",				readString,	writeString,	&languageStrings[languageCdewCycleDown]},
	{"languageCdewSwap1And2:= ",				readString,	writeString,	&languageStrings[languageCdewSwap1And2]},
	
	{"languageCdewMenuContent:= ",				readString,	writeString,	&languageStrings[languageCdewMenuContent]},
	{"languageCdewContentClear:= ",				readString,	writeString,	&languageStrings[languageCdewContentClear]},
	{"languageCdewContentDefault:= ",			readString,	writeString,	&languageStrings[languageCdewContentDefault]},
	{"languageCdewHandsDistribute:= ",			readString,	writeString,	&languageStrings[languageCdewHandsDistribute]},
	{"languageCdewContentDistribute:= ",		readString,	writeString,	&languageStrings[languageCdewContentDistribute]},
	{"languageCdewContentRotate:= ",			readString,	writeString,	&languageStrings[languageCdewContentRotate]},
	{"languageCdewContentResetArrows:= ",		readString,	writeString,	&languageStrings[languageCdewContentResetArrows]},
	
	{"languageCdewMenuSize:= ",					readString,	writeString,	&languageStrings[languageCdewMenuSize]},
	{"languageCdewSizeAddJuggler:= ",			readString,	writeString,	&languageStrings[languageCdewSizeAddJuggler]},
	{"languageCdewSizeRemoveJuggler:= ",		readString,	writeString,	&languageStrings[languageCdewSizeRemoveJuggler]},
	{"languageCdewSizeAddBeat:= ",				readString,	writeString,	&languageStrings[languageCdewSizeAddBeat]},
	{"languageCdewSizeRemoveBeat:= ",			readString,	writeString,	&languageStrings[languageCdewSizeRemoveBeat]},
	{"languageCdewSizeAddReverse:= ",			readString,	writeString,	&languageStrings[languageCdewSizeAddReverse]},
	{"languageCdewSizeAddSymetric:= ",			readString,	writeString,	&languageStrings[languageCdewSizeAddSymetric]},
	{"languageCdewSizeAddSame:= ",				readString,	writeString,	&languageStrings[languageCdewSizeAddSame]},
	
	{"languageCdewMenuEdit:= ",					readString,	writeString,	&languageStrings[languageCdewMenuEdit]},
	{"languageCdewEditUndo:= ",					readString,	writeString,	&languageStrings[languageCdewEditUndo]},
	{"languageCdewEditRedo:= ",					readString,	writeString,	&languageStrings[languageCdewEditRedo]},
	{"languageCdewEditCut:= ",					readString,	writeString,	&languageStrings[languageCdewEditCut]},
	{"languageCdewEditCopy:= ",					readString,	writeString,	&languageStrings[languageCdewEditCopy]},
	{"languageCdewEditPaste:= ",				readString,	writeString,	&languageStrings[languageCdewEditPaste]},
	
	{"languageCdewMenuDisplay:= ",				readString,	writeString,	&languageStrings[languageCdewMenuDisplay]},
	{"languageCdewDisplaySingle:= ",			readString,	writeString,	&languageStrings[languageCdewDisplaySingle]},
	{"languageCdewDisplaySync:= ",				readString,	writeString,	&languageStrings[languageCdewDisplaySync]},
	{"languageCdewDisplayColor:= ",				readString,	writeString,	&languageStrings[languageCdewDisplayColor]},
	{"languageCdewDisplayNext:= ",				readString,	writeString,	&languageStrings[languageCdewDisplayNext]},
	{"languageCdewDisplayModePrint:= ",			readString,	writeString,	&languageStrings[languageCdewDisplayModePrint]},
	{"languageCdewColorModePrint:= ",			readString,	writeString,	&languageStrings[languageCdewColorModePrint]},
	
	{"languageCdewMenuColor:= ",				readString,	writeString,	&languageStrings[languageCdewMenuColor]},
	{"languageCdewColorReset:= ",				readString,	writeString,	&languageStrings[languageCdewColorReset]},
	{"languageCdewColorLength:= ",				readString,	writeString,	&languageStrings[languageCdewColorLength]},
	{"languageCdewColorPath:= ",				readString,	writeString,	&languageStrings[languageCdewColorPath]},
	{"languageCdewColorUser:= ",				readString,	writeString,	&languageStrings[languageCdewColorUser]},
	{"languageCdewColorJuggler:= ",				readString,	writeString,	&languageStrings[languageCdewColorJuggler]},
	{"languageCdewColorNext:= ",				readString,	writeString,	&languageStrings[languageCdewColorNext]},
	
	{"languageCdewMenuView:= ",					readString,	writeString,	&languageStrings[languageCdewMenuView]},
	{"languageCdewMenuViewThrow:= ",			readString,	writeString,	&languageStrings[languageCdewMenuViewThrow]},
	{"languageCdewViewHurries:= ",				readString,	writeString,	&languageStrings[languageCdewViewHurries]},
	{"languageCdewViewCrosses:= ",				readString,	writeString,	&languageStrings[languageCdewViewCrosses]},
	{"languageCdewViewDelays:= ",				readString,	writeString,	&languageStrings[languageCdewViewDelays]},
	{"languageCdewMenuViewPattern:= ",			readString,	writeString,	&languageStrings[languageCdewMenuViewPattern]},
	{"languageCdewViewLines:= ",				readString,	writeString,	&languageStrings[languageCdewViewLines]},
	{"languageCdewViewCellCol:= ",				readString,	writeString,	&languageStrings[languageCdewViewCellCol]},
	{"languageCdewViewFaults:= ",				readString,	writeString,	&languageStrings[languageCdewViewFaults]},
	{"languageCdewViewStart:= ",				readString,	writeString,	&languageStrings[languageCdewViewStart]},
	{"languageCdewViewSiteswap:= ",				readString,	writeString,	&languageStrings[languageCdewViewSiteswap]},
	{"languageCdewViewObjects:= ",				readString,	writeString,	&languageStrings[languageCdewViewObjects]},
	{"languageCdewViewBeats:= ",				readString,	writeString,	&languageStrings[languageCdewViewBeats]},
	{"languageCdewViewAllHands:= ",				readString,	writeString,	&languageStrings[languageCdewViewAllHands]},
	{"languageCdewViewTimeC:= ",				readString,	writeString,	&languageStrings[languageCdewViewTimeC]},
	
	{"languageCdewMenuGeneral:= ",				readString,	writeString,	&languageStrings[languageCdewMenuGeneral]},
	{"languageCdewGeneralBackground:= ",		readString,	writeString,	&languageStrings[languageCdewGeneralBackground]},
	{"languageCdewGeneralSnap:= ",				readString,	writeString,	&languageStrings[languageCdewGeneralSnap]},
	
	{"languageCdewMenuWindow:= ",				readString,	writeString,	&languageStrings[languageCdewMenuWindow]},
	{"languageCdewWindowAnimation:= ",			readString,	writeString,	&languageStrings[languageCdewWindowAnimation]},
	{"languageCdewWindowNextCdew:= ",			readString,	writeString,	&languageStrings[languageCdewWindowNextCdew]},
	
	// no more entries
	{NULL,								NULL,		NULL,		NULL}};	


void initLanguage( void )
{
	makeLanguageDefaultStrings();
	readLanguageFile();
	
	// disable while working on the language routines
	// enable to create default language strings if there is no entry in the language file 	
#if WriteLanguageFile
	atexit( writeLanguageFile);
#endif	
}



void dummyFunction (FILE *fp, void *d)
{
#pragma unused( fp, d)
}

wwwBoolean makeLanguageDefaultStrings( void )
{
	int c;
	
	
	for( c = 0; c < languageEnd; c++)
	{
		char *p;
		
		p = (char *)malloc( strlen( languageDefaultStrings[c] ) + 1 );
		if( p == 0 )
			return false;
		
		sprintf(p, "%s", languageDefaultStrings[c]);
		printf("%d: %s\n", c, p);
		
		
		languageStrings[c] = p;
		
	}
	
	return true;				
	
}






/* Joe Pass environment initialisieren */
wwwBoolean readLanguageFile(void)
{
	wwwBoolean ret = false;
	
	ret = readTextPrefs(languagePath, languageList);
	
	if( ret == false )
		_myPrintf("error while reading language file\n");
	
	return ret;
}

#if WriteLanguageFile
void writeLanguageFile( void )
{
	saveTextPrefs(languagePath, languageList);
}
#endif



