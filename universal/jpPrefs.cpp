/*
 *  jpPrefs.cpp
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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#include "systemswitch.h"
#include "prefDef.h"
#include "applicationFolders.h"
#include "jpPrefs.h"
#include "prefs.h"
#include "animationView.h"
#include "world.h"
#include "pathAccess.h"
#include "fileIO.h"

#include "globals.h"
#include "editor.h"
#include "animationWin.h"
#include "info.h"
#include "grafik.h"

#include "causal_editor.h"

#include "movie.h"

#include "myPrintf.h"
#include "myutil.h"
#include "runaround.h"
#include "playSound.h"

#include "prefDefinitionValues.h"
#include "workspace.h"

#if MAC_CLASSIC_VERSION
#include "macWin.h"
#endif


#define USE_REL_SCREEN_DIMENSION true


void                    useDefaultPrefs( void );

wwwBoolean              correctHeight = true;
extern wwwBoolean       projectSunFlag;


prefAtom prefList[] =   {
	{"---------- new sorting:  --------------------------", NULL, NULL, NULL},
	{"---------- was grouped by same gl setting call:  --", NULL, NULL, NULL},
	{"---------- now grouped by object effects:        --", NULL, NULL, NULL},

	{"", NULL, NULL, NULL},
	{"========== obsolete: remove ===================",     NULL, NULL, NULL},
	{"---------- environment definitions: ----------",      NULL, NULL, NULL},
	{"environment:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.env},
	{"showAbstracts:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.showAbstracts},
	//{"projectSun:=",                                      readBoolean,			writeBoolean,           &projectSunFlag},

	{"", NULL, NULL, NULL},
	{"========== to do ==============================",     NULL, NULL, NULL},
	{"---------- fog definitions: -------------------",     NULL, NULL, NULL},
	{"fog:= ",                                              readBoolean,            writeBoolean,           &jpPrefs.fog},
	{"fogStart:= ",                                         readFloat,              writeFloat,             &jpPrefs.fogStart},
	{"fogEnd:= ",                                           readFloat,              writeFloat,             &jpPrefs.fogEnd},
	{"fogDensity:= ",                                       readFloat,              writeFloat,             &jpPrefs.fogDensity},
	{"fieldOfViewY:= ",                                     readFloat,              writeFloat,             &jpPrefs.fieldOfViewY},
	{"fog_color:= ",                                        readVektor3,            writeVektor3,           &jpPrefs.fog_color},

	{"", NULL, NULL, NULL},
	{"---------- bugfix for crashes under some GLUT implementations: ----------", NULL, NULL, NULL},
	{"menuMarkers:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.menuMarkers},

	{"", NULL, NULL, NULL},
	{"========== behavious settings ================",      NULL, NULL, NULL},
	{"useCdewWorkspace:= ",									readBoolean,			writeBoolean,			&useCdewWorkspace},	
	
	{"", NULL, NULL, NULL},
	{"========== windows settings ==================",      NULL, NULL, NULL},
	{"---------- window positions & visability ------",		NULL, NULL, NULL},
	{"worldWinSizeX:= ",                                    readFloat,              writeFloat,             &jpPrefs.worldWinSizeX},
	{"worldWinSizeY:= ",                                    readFloat,              writeFloat,             &jpPrefs.worldWinSizeY},
	{"worldWinSizeW:= ",                                    readFloat,              writeFloat,             &jpPrefs.worldWinSizeW},
	{"worldWinSizeH:= ",                                    readFloat,              writeFloat,             &jpPrefs.worldWinSizeH},
	{"infoWinSizeX:= ",                                     readFloat,              writeFloat,             &jpPrefs.infoWinSizeX},
	{"infoWinSizeY:= ",                                     readFloat,              writeFloat,             &jpPrefs.infoWinSizeY},
	{"infoWinSizeW:= ",                                     readFloat,              writeFloat,             &jpPrefs.infoWinSizeW},
	{"infoWinSizeH:= ",                                     readFloat,              writeFloat,             &jpPrefs.infoWinSizeH},
	{"patternWinSizeX:= ",                                  readFloat,              writeFloat,             &jpPrefs.patternWinSizeX},
	{"patternWinSizeY:= ",                                  readFloat,              writeFloat,             &jpPrefs.patternWinSizeY},
	{"patternWinSizeW:= ",                                  readFloat,              writeFloat,             &jpPrefs.patternWinSizeW},
	{"patternWinSizeH:= ",                                  readFloat,              writeFloat,             &jpPrefs.patternWinSizeH},
	{"runaroundInputWinSizeX:= ",							readFloat,              writeFloat,             &runaroundInputWinSizeX},
	{"runaroundInputWinSizeY:= ",							readFloat,              writeFloat,             &runaroundInputWinSizeY},
	{"runaroundInputWinSizeW:= ",							readFloat,              writeFloat,             &runaroundInputWinSizeW},
	{"runaroundInputWinSizeH:= ",							readFloat,              writeFloat,             &runaroundInputWinSizeH},
	{"runaroundListWinSizeX:= ",							readFloat,              writeFloat,             &runaroundListWinSizeX},
	{"runaroundListWinSizeY:= ",							readFloat,              writeFloat,             &runaroundListWinSizeY},
	{"runaroundListWinSizeW:= ",							readFloat,              writeFloat,             &runaroundListWinSizeW},
	{"runaroundListWinSizeH:= ",							readFloat,              writeFloat,             &runaroundListWinSizeH},
	{"showInfoWindow:= ",                                   readBoolean,            writeBoolean,           &jpPrefs.showInfoWindow},
	{"showEditorWindow:= ",                                 readBoolean,            writeBoolean,           &jpPrefs.showEditorWindow},
	{"showMessageWindow:= ",                                readBoolean,            writeBoolean,           &jpPrefs.showMessageWindow},
	{"showScenesWindow:= ",									readBoolean,            writeBoolean,           &jpPrefs.showScenesWindow},
	{"showPositionWindow:= ",								readBoolean,            writeBoolean,           &jpPrefs.showPositionWindow},

		
	{"", NULL, NULL, NULL},
	{"========== text editor settings ===================", NULL, NULL, NULL},
	{"editorTextColor:= ",                                  readVektor3,            writeVektor3,           &jpPrefs.editorTextColor},
	{"editorErrColor:= ",                                   readVektor3,            writeVektor3,           &jpPrefs.editorErrColor},
	{"useFullCursor:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.useFullCursor},
	{"editorCursorTextColor:= ",                            readVektor3,            writeVektor3,           &jpPrefs.editorCursorTextColor},
	{"editorCursorBgColor:= ",                              readVektor3,            writeVektor3,           &jpPrefs.editorCursorBgColor},
	{"idleCursorWidth:= ",                                  readInt,                writeInt,               &jpPrefs.idleCursorWidth},
	{"flashTime:= ",                                        readInt,                writeInt,               &jpPrefs.flashTime},
	{"jumpToErrPosition:= ",                                readBoolean,            writeBoolean,           &jpPrefs.jumpToErrPosition},
	{"showCmdLineFileInTextEditor:= ",                      readBoolean,            writeBoolean,           &jpPrefs.showCmdLineFileInTextEditor},
	{"helpActive:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.helpActive},
	{"generateCausalFromTextEditorOnStart:= ",              readBoolean,            writeBoolean,           &jpPrefs.generateCausalFromTextEditorOnStart},
	{"autosaveCausalFromTextEditor:= ",                     readBoolean,            writeBoolean,           &jpPrefs.autosaveCausalFromTextEditor},
	{"generateTextToCausalInSameWindow:= ",                 readBoolean,            writeBoolean,           &generateTextToCausalInSameWindow},
	{"editorDisplaySwitch:= ",								readInt,				writeInt,				&editorDisplaySwitch},

	{"", NULL, NULL, NULL},
	{"========== animation window settings ==============", NULL, NULL, NULL},
	{"---------- general --------------------------------", NULL, NULL, NULL},
	{"useTapAnimationValues:=",                             readBoolean,            writeBoolean,           &jpPrefs.useTapAnimationValues},

	{"", NULL, NULL, NULL},
	{"---------- siteswap -------------------------------", NULL, NULL, NULL},
	{"jpAnimationFPS:= ",                                   readInt,                writeInt,               &jpAnimationFPS},
	{"movement:= ",                                         readBoolean,            writeBoolean,           &jpPrefs.mov},
	{"walkFactor:= ",                                       readFloat,              writeFloat,             &jpPrefs.walkFactor},
	{"gravity:= ",                                          readFloat,              writeFloat,             &jpPrefs.gravity},
	{"emptyBeats:= ",                                       readInt,                writeInt,               &jpPrefs.emptyBeats},
	{"maxDwell:= ",                                         readFloat,              writeFloat,             &jpPrefs.maxDwell},
	{"minActionTime:= ",                                    readFloat,              writeFloat,             &jpPrefs.minActionTime},
	{"animationSpeed:= ",                                   readFloat,              writeFloat,             &jpPrefs.animationSpeed},
	{"selfDwellElevation:= ",								readFloat,              writeFloat,             &jpPrefs.selfDwellElevation},
//	{"dwellElevation:= ",									readFloat,              writeFloat,             &jpPrefs.dwellElevation},
	{"emphaseShortDistanceFlag:=",                          readBoolean,            writeBoolean,           &jpPrefs.emphaseShortDistanceFlag},
	{"minObjectDistance:= ",                                readFloat,              writeFloat,             &jpPrefs.minObjectDistance},
	{"positionMode:= ",										readInt,                writeInt,               &positionMode},
	
	{"", NULL, NULL, NULL},
	{"---------- Light ------------------------------", NULL, NULL, NULL},
	{"light_position:= ",                                   readf4,                 writef4,                &light_position},
	{"specular0:= ",										readf4,                 writef4,                &specular0},
	{"diffuse0:= ",											readf4,                 writef4,                &diffuse0},
	{"ambient0:= ",											readf4,                 writef4,                &ambient0},
	{"lmodel_ambient:= ",									readf4,                 writef4,                &lmodel_ambient},
	{"luminance:= ",                                        readFloat,              writeFloat,             &jpPrefs.luminance},

	{"", NULL, NULL, NULL},
	{"---------- render mode ------------------------",     NULL, NULL, NULL},
	{"solidWorld:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.solidWorld},
	{"shaderMode:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.shaderMode},
	{"levelOfDetail:= ",                                    readInt,                writeInt,               &jpPrefs.levelOfDetail},
	{"smooth:= ",                                           readBoolean,            writeBoolean,           &jpPrefs.shaderMode},
	{"antialias:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.antialias},

	{"", NULL, NULL, NULL},
	{"---------- camera -----------------------------",     NULL, NULL, NULL},
	{"magnification:= ",                                    readFloat,              writeFloat,             &jpPrefs.mag},
	{"matrix:= ",                                           readMatrix4,            writeMatrix4,           jpPrefs.matrix},
	{"rotationAroundCenter:= ",                             readFloat,              writeFloat,             &jpPrefs.rotationAroundCenter},
	{"bendAngle:= ",                                        readFloat,              writeFloat,             &jpPrefs.bendAngle},
	{"modelScale:= ",                                       readFloat,              writeFloat,             &jpPrefs.modelScale},
	{"showCameraPath:= ",                                   readBoolean,            writeBoolean,           &jpPrefs.showCameraPath},
	{"showHandPath:= ",										readBoolean,            writeBoolean,           &jpPrefs.showHandPath},

	{"", NULL, NULL, NULL},
	{"---------- Textures -------------------------------", NULL, NULL, NULL},
	{"texturedClub:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.texturedClub},
	{"texturedBall:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.texturedBall},
	{"texturedSky:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.texturedSky},
	{"texturedFloor:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.texturedFloor},
	{"texturedJuggler:= ",                                  readBoolean,            writeBoolean,           &jpPrefs.texturedJuggler},
	{"texturedHead:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.texturedHead},
	
	{"", NULL, NULL, NULL},
	{"---------- switches -------------------------------", NULL, NULL, NULL},
	{"axis:= ",                                             readBoolean,            writeBoolean,           &jpPrefs.a},
	{"sky:= ",                                              readBoolean,            writeBoolean,           &jpPrefs.sky},
	{"floor:= ",                                            readBoolean,            writeBoolean,           &jpPrefs.floor},
	{"showScene:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.showScene},	
	{"showLogo:= ",                                         readBoolean,            writeBoolean,           &jpPrefs.showLogo},	
	{"showJugglerNumbers:= ",								readBoolean,            writeBoolean,           &jpPrefs.showJugglerNumbers},
	{"numbersColor:= ",                                     readVektor3,            writeVektor3,           &jpPrefs.numbersColor},

	{"", NULL, NULL, NULL},
	{"---------- color definitions: -----------------", NULL, NULL, NULL},
	{"backgroundColor:= ",                                  readVektor3,            writeVektor3,           &jpPrefs.backgroundColor},
	{"skyColor:= ",                                         readVektor3,            writeVektor3,           &jpPrefs.skyColor},	
		
	{"", NULL, NULL, NULL},
	{"---------- Juggler --------------------------------", NULL, NULL, NULL},
	{"jugglerMaterialSpecular:= ",							readf4,                 writef4,                &jugglerMaterialSpecular},
	{"jugglerMaterialShininess:= ",							readFloat,				writeFloat,				&jugglerMaterialShininess},
	{"jugglerMaterialEmission:= ",							readf4,                 writef4,                &jugglerMaterialEmission},
	{"jugglerColor:= ",                                     readVektor3,            writeVektor3,           &jpPrefs.bd},
	{"bodyFaktor:= ",                                       readFloat,              writeFloat,             &jpPrefs.bodyFaktor},
	{"jNumbers:= ",                                         readBoolean,            writeBoolean,           &jpPrefs.jNumbers},
	{"unicycles:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.unicycles},
	{"jointObjects:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.jointObjects},
	{"blindfolded:=",                                       readBoolean,            writeBoolean,           &jpPrefs.blindfolded},
	{"showJugglerPos:= ",                                  readBoolean,            writeBoolean,           &jpPrefs.showJugglerPos},
	{"showJugglerPoi:= ",                                   readBoolean,            writeBoolean,           &jpPrefs.showJugglerPoi},
	{"showShortTimeMovementPaths:= ",						readBoolean,            writeBoolean,           &showShortTimeMovementPaths},
	{"showJugglerModifiers:= ",                             readBoolean,            writeBoolean,           &jpPrefs.showJugglerModifiers},
	
	{"", NULL, NULL, NULL},
	{"---------- Material--------------------------------", NULL, NULL, NULL},
	{"objMaterialSpecular:= ",								readf4,                 writef4,                &objMaterialSpecular},
	{"objMaterialShininess:= ",								readFloat,				writeFloat,				&objMaterialShininess},
	{"objMaterialEmission:= ",								readf4,                 writef4,                &objMaterialEmission},
	{"objectFaktor:= ",                                     readFloat,              writeFloat,             &jpPrefs.objectFaktor},
	{"objectSize:= ",                                       readFloat,              writeFloat,             &jpPrefs.objectSize},
	{"objectColor:= ",                                      readVektor3,            writeVektor3,           &jpPrefs.objectColor},
	{"colorOffset:= ",                                      readInt,                writeInt,               &jpPrefs.colorOffset},
	{"objectStyle:= ",                                      readInt,                writeInt,               &jpPrefs.os},
	{"objectColorMode:= ",                                  readInt,                writeInt,               &jpPrefs.objectColorMode},
	{"showObjectVektors:= ",                                readBoolean,            writeBoolean,           &jpPrefs.showObjectVektors},
	{"showObjectNumbers:= ",								readBoolean,            writeBoolean,           &jpPrefs.showObjectNumbers},
	{"showSiteswapValue:= ",								readBoolean,            writeBoolean,           &jpPrefs.showSiteswapValue},
	
	{"", NULL, NULL, NULL},
	{"---------- Floor ----------------------------------", NULL, NULL, NULL},
	{"floorColor:= ",                                       readVektor3,            writeVektor3,           &jpPrefs.floorColor},
	{"floorGridColorLight:= ",                              readVektor3,            writeVektor3,           &jpPrefs.floorGridColorLight},
	{"floorGridColorDark:= ",								readVektor3,            writeVektor3,           &jpPrefs.floorGridColorDark},
	{"floorSize:= ",                                        readFloat,              writeFloat,             &jpPrefs.floorSize},
	{"solidFloor:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.solidFloor},
	{"showFloorGrid:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.showFloorGrid},
	{"showFrame:= ",										readBoolean,            writeBoolean,           &showFrameFlag},
	{"repeatFloor:= ",                                      readInt,                writeInt,               &jpPrefs.repeatFloor},
	{"lookThroughFloor:= ",                                 readBoolean,            writeBoolean,           &jpPrefs.lookThroughFloor},

	{"", NULL, NULL, NULL},
	{"---------- Levels ---------------------------------", NULL, NULL, NULL},
	{"niveauBlendValue:= ",                                 readFloat,              writeFloat,             &jpPrefs.niveauBlendValue},
	{"niveauColor:= ",                                      readVektor3,            writeVektor3,           &jpPrefs.niveauColor},
	{"niveauSize:= ",                                       readFloat,              writeFloat,             &jpPrefs.niveauSize},
	{"niveauLines:= ",                                      readInt,                writeInt,               &jpPrefs.niveauLines},
	{"showNivieaus:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.showNiveaus},
	{"solidNiveaus:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.solidNiveaus},

	{"", NULL, NULL, NULL},
	{"---------- stage description ----------", NULL, NULL, NULL},
	{"showStage:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.showStage},
	{"stageColor:= ",                                       readVektor3,            writeVektor3,           &jpPrefs.stageColor},
	{"stageLowerLeft:= ",                                   readVektor3,            writeVektor3,           &jpPrefs.lowerLeftStage},
	{"stageUpperRight:= ",                                  readVektor3,            writeVektor3,           &jpPrefs.upperRightStage},
	
	{"", NULL, NULL, NULL},
	{"---------- Sound ----------------------------------", NULL, NULL, NULL},
	{"throwSound:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.t},
	{"dynSoundHeight:= ",                                   readBoolean,            writeBoolean,           &jpPrefs.dt},
	{"dynSoundPos:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.dp},
	{"playDropSound:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.playDropSound},
	{"sound:= ",                                            readBoolean,            writeBoolean,           &jpPrefs.s},
	//~ {"soundKind:= ",                                        readInt,				writeInt,				&soundKind},
	
	{"", NULL, NULL, NULL},
	{"========== runaround ===========================", NULL, NULL, NULL},
	{"runaroundRotateAllScenesTogetherFlag:= ",						readBoolean,            writeBoolean,           &runaroundRotateAllScenesTogetherFlag},

	
	{"", NULL, NULL, NULL},
	{"========== info ===========================", NULL, NULL, NULL},
	{"fpsInMovieInfo:= ",                                   readInt,                writeInt,               &jpPrefs.fpsInMovie},
	{"patternInfo:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.p},
	{"runaroundInfo:= ",									readBoolean,            writeBoolean,           &showRunaroundInfoFlag},
	{"frameRateInfo:= ",                                    readBoolean,            writeBoolean,           &jpPrefs.f},
	{"infoStringInfo:= ",                                   readBoolean,            writeBoolean,           &jpPrefs.i},
	{"eventInfo:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.e},
	{"movieFrameRate:= ",                                   readInt,                writeInt,               &jpPrefs.movieFrameRate},
	{"fadeEvents:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.fadeEvents},
	{"fadeFactor:= ",                                       readFloat,              writeFloat,             &jpPrefs.fadeFactor},
	{"numberOfAdditionalEvents:= ",                         readInt,                writeInt,               &jpPrefs.numberOfAdditionalEvents},
	{"movieInfo:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.movieInfo},
	{"showPatternTime:= ",                                  readBoolean,            writeBoolean,           &showPatternTimeFlag},
	{"showObjectColorFlag:= ",                              readBoolean,            writeBoolean,           &showObjectColorFlag},

	{"", NULL, NULL, NULL},
	{"========== appl stuff =========================", NULL, NULL, NULL},
	{"---------- command line ---------------------------", NULL, NULL, NULL},
	
	{"", NULL, NULL, NULL},
	{"---------- siteswap ---------------------------", NULL, NULL, NULL},
	{"siteswapMode:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.sm},
	{"holdTwo:= ",                                          readBoolean,            writeBoolean,           &jpPrefs.h2},
	{"syncMode:= ",                                         readInt,                writeInt,               &jpPrefs.syncMode},
	{"oneTime:= ",                                          readFloat,              writeFloat,             &jpPrefs.oneTime},
	{"switch1x:= ",                                         readBoolean,            writeBoolean,           &jpPrefs.switch1x},

	{"", NULL, NULL, NULL},
	{"---------- juggling ---------------------------", NULL, NULL, NULL},
	{"defhandDwell:= ",                                     readFloat,              writeFloat,             &jpPrefs.defhandDwell},
	{"jugglerStyle:= ",                                     readInt,				writeInt,				&jpPrefs.jugglerColor},
	{"delayMode:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.obeyDelay},
	{"autoHurried:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.autoHurriedMode},

	{"", NULL, NULL, NULL},
	{"---------- hand movement ---------------------", NULL, NULL, NULL},
	{"vertHandMovement:= ",                                 readFloat,              writeFloat,             &jpPrefs.vertHandMovement},
	{"dwell:= ",                                            readFloat,              NULL,					&jpPrefs.deflection},
	{"deflection:= ",                                       readFloat,              writeFloat,             &jpPrefs.deflection},
	{"armForeFactor:= ",                                    readFloat,              writeFloat,             &jpPrefs.armForeFactor},
	{"armSideFactor:= ",                                    readFloat,              writeFloat,             &jpPrefs.armSideFactor},
	{"ellbowDeflection:= ",                                 readFloat,              writeFloat,             &jpPrefs.ellbowDeflection},
	{"deflectionPassFactor:= ",                             readFloat,              writeFloat,             &jpPrefs.deflectionPassFactor},
	{"throwAngle:= ",                                       readFloat,              writeFloat,             &jpPrefs.throwAngle},
	{"passCorrectionVector:= ",                             readVektor3,            writeVektor3,           &jpPrefs.passCorrectionVector},
	{"catchCorrectionVector:= ",                            readVektor3,            writeVektor3,           &jpPrefs.catchCorrectionVector},

	{"", NULL, NULL, NULL},
	{"---------- misc  ------------------------------", NULL, NULL, NULL},
	{"saved:= ",                                            readBoolean,            writeBoolean,           &jpPrefs.saved},
	{"errorOut:= ",                                         readBoolean,            writeBoolean,           &jpPrefs.es},
	{"mouseControl:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.animationView_mouseControl},
	{"inputPatternCode:= ",                                 readPrintableChar,      writePrintableChar,     &jpPrefs.modeSwitchCode},
	{"textSize:= ",                                         readFloat,              writeFloat,             &jpPrefs.textSize},
	{"soundOutput:= ",                                      readBoolean,            writeBoolean,           &jpPrefs.soundOutput},
	{"textEditorAutoStartFlag:= ",                          readBoolean,            writeBoolean,           &jpPrefs.textEditorAutoStartFlag},
	{"screenWidth:= ",                                      readFloat,              writeFloat,             &jpPrefs.screenWidth},
	{"screenHeight:= ",                                     readFloat,              writeFloat,             &jpPrefs.screenHeight},
	{"newGiraffeModel:= ",                                  readBoolean,            writeBoolean,           &jpPrefs.newGiraffeModel},
#ifdef USE_CDEW
	{"causalEditorHasOpenWindows:= ",                       readBoolean,            writeBoolean,           &jpPrefs.causalEditorHasOpenWindows},
#endif
	{"askForSave:= ",                                       readBoolean,            writeBoolean,           &jpPrefs.askForSaveFlag},
	//{"fullPatternPath:= ",                                readString,             writeString,            &fullPatternPath},
	{"", NULL, NULL, NULL},
	{"---------- prefs  -----------------------------", NULL, NULL, NULL},
	{"writeLogfile:= ",                                     readBoolean,            writeBoolean,           &jpPrefs.writeLogfile},
	{"conOutput:= ",                                        readBoolean,            writeBoolean,           &jpPrefs.conOutput},
	{"showMenuHotkeys:= ",                                  readBoolean,            writeBoolean,           &jpPrefs.showMenuHotkeys},
	{"showMenuSelectors:= ",                                readBoolean,            writeBoolean,           &jpPrefs.showMenuSelectors},
	{"waitForUserInputOnQuit:= ",                           readBoolean,            writeBoolean,           &jpPrefs.waitForUserInput},
	
#if 0
	{"", NULL, NULL, NULL},
	// value was damaged once. took me weeks before I realized that it was just the prefs value. 
	// As this is a vey bad user experience, no value is saved and the default value is used as initial value (mpeg currently).
#ifdef MAKE_QUICKTIME_MOVIE
	{"---------- movie codec definitions (mac only): ----------", NULL, NULL, NULL},
	{"theCodec:= ",                                         readLong,               writeLong,              &jpPrefs.theCodec},
#endif
#endif
	
	{"", NULL, NULL, NULL},
	{"---------- experimental: --------------------------------", NULL, NULL, NULL},
	
	// no more entries
	{NULL,                  NULL,                   NULL,                   NULL}};


bool readPrefsIfChanged( void )
{
	struct tm* tNow;				// create a time structure
	static tm tLast;
	bool ret = false;
	struct stat attrib;					// create a file attribute structure
	
	if( strlen( prefsPath) == 0 )
		return false;
	if( !fexists(prefsPath) )
		return false;

	stat(prefsPath, &attrib);
	tNow = gmtime(&(attrib.st_mtime));	
	if( compareTime( &tLast, tNow) ){
		readJPPrefs( false );
	}
	tLast = *tNow;	
	return ret;
}



/* Joe Pass environment initialisieren */
void readJPPrefs( bool initialiseWithDefaults)
{
	wwwBoolean readPrefs = true;
	float xf = 1., yf = 1.;	
	
	if( initialiseWithDefaults )
		useDefaultPrefs();
	
	
	if(readPrefs ){
		readTextPrefs(prefsPath, prefList);
		prefsValid = true;
	}else
		_myBeep();
	
	if( jpPrefs.worldWinSizeX <= 1 ){
		xf = glutGet( GLUT_SCREEN_WIDTH);
		yf = glutGet( GLUT_SCREEN_HEIGHT);
	}

	jpPrefs.worldWinSizeX *= xf;
	jpPrefs.worldWinSizeY *= yf;
	jpPrefs.worldWinSizeW *= xf;
	jpPrefs.worldWinSizeH *= yf;
		
	jpPrefs.patternWinSizeX *= xf;
	jpPrefs.patternWinSizeY *= yf;
	jpPrefs.patternWinSizeW *= xf;
	jpPrefs.patternWinSizeH *= yf;
	
	jpPrefs.infoWinSizeX *= xf;
	jpPrefs.infoWinSizeY *= yf;
	jpPrefs.infoWinSizeW *= xf;
	jpPrefs.infoWinSizeH *= yf;
	
	// set the value of certain menu markers
	showGridFlag = jpPrefs.showFloorGrid;
	showPosFlag = jpPrefs.showJugglerPos;
	showPoiFlag = jpPrefs.showJugglerPoi;
	runaroundShowSceneIndexFlag = showSceneIndexFlag;
	runaroundShowFrameFlag = showFrameFlag;

}

void writeJPPrefs( void )
{
	wwwBoolean writePrefs = true;
	float xf = 1., yf = 1.;
	GLint w,h;
	
#ifdef OPEN_GL_VERSION
	animationGetMatrix(jpPrefs.matrix);	
#endif
	
	
#if MAC_CLASSIC_VERSION
q	fixWindowPositionForMac();
#endif

	// scale window size to 0...1
	w = glutGet( GLUT_SCREEN_WIDTH);
	h = glutGet( GLUT_SCREEN_HEIGHT);
	xf = 1.f / w;
	yf = 1.f / h;

	
	jpPrefs.worldWinSizeX *= xf;
	jpPrefs.worldWinSizeY *= yf;
	jpPrefs.worldWinSizeW *= xf;
	jpPrefs.worldWinSizeH *= yf;
	
	jpPrefs.patternWinSizeX *= xf;
	jpPrefs.patternWinSizeY *= yf;
	jpPrefs.patternWinSizeW *= xf;
	jpPrefs.patternWinSizeH *= yf;
	
	jpPrefs.infoWinSizeX *= xf;
	jpPrefs.infoWinSizeY *= yf;
	jpPrefs.infoWinSizeW *= xf;
	jpPrefs.infoWinSizeH *= yf;
	
	
	
#ifdef MAC_VERSION
	getMacModifiers();
	writePrefs = !skMacMode;
#endif
	if( writePrefs )
	{
		jpPrefs.saved = true;
		saveTextPrefs(prefsPath, prefList);
	}
}


void useDefaultPrefs( void )
{       jpPrefs = defaultPrefs;
	readTextPrefs(defprefsPath, prefList);
	prefsValid = true;
}


