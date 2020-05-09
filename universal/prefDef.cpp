/*
 *  prefDef.cpp
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
#include "globals.h"

#include "systemswitch.h"

#include "prefDef.h"
#include "parseSiteswap.h"
#include "world.h"

#ifdef OPEN_GL_VERSION
#include "animationWin.h"
#endif
#ifdef MAKE_QUICKTIME_MOVIE
#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>
#endif

#ifdef MAC_VERSION
#include "macgraf.h"
#else
#include "animationView.h"
#include "grafik.h"
#endif

#include "prefDefinitionValues.h"


#define			kFMFPSDefault				6
#define			kBOptimized					1
#define			PrefVersion					200


prefsSettings jpPrefs, defaultPrefs;


sysColor        textColor				=	DefTextColor;
sysColor        ballColor				=	DefObjectColor;
sysColor        lineColor				=	DefLineColor;
sysColor        eventColor1				=	DefEvent1Color;
sysColor        eventColor2				=	DefEvent2Color;

int				prefVersion				=	PrefVersion;

float			worldMatrix[4][4]		=	DefMatrix;			// Darstellungsmatrix
float			light_position[4]		=  { 1000,3000,500,0};

bool			showShortTimeMovementPaths = true;

void initPrefs( void )
{
	defaultPrefs.saved	 							=	false;                    
	defaultPrefs.sm 								=	xMode;                       
	defaultPrefs.animationSpeed 					=	defSpeed;           
	defaultPrefs.fpsInMovie 						=	defFpsMovie;               
	defaultPrefs.p 									=	false;
	defaultPrefs.f 									=	false;	
	defaultPrefs.i	 								=	false; 
	defaultPrefs.e 									=	true;                  
	defaultPrefs.jugglerColor						=	multiColorJugglers; 
	defaultPrefs.h2 								=	true;
	defaultPrefs.mov 								=	false;
	defaultPrefs.s			 						=	false;               
	defaultPrefs.a 									=	false; 
	defaultPrefs.floor 								=	false;                  
	defaultPrefs.deflection							=	1.f;                    
	defaultPrefs.gravity						 	=	defGravity;                  
	defaultPrefs.objectFaktor 						=	1.f;
	defaultPrefs.objectSize 						=	1.f;               
	defaultPrefs.bodyFaktor 						=	1.f;               
	defaultPrefs.t 									=	false;                        
	defaultPrefs.dt 								=	false;
	defaultPrefs.dp 								=	false;                   
	defaultPrefs.es 								=	false;                       
	defaultPrefs.syncMode 							=	syncLeft;                 
	defaultPrefs.jNumbers 							=	false;                 
	defaultPrefs.obeyDelay 							=	true;                
	defaultPrefs.switch1x 							=	true;                 
	defaultPrefs.showNiveaus 						=	false;              
	defaultPrefs.floorSize 							=	512.f;                
	defaultPrefs.niveauSize 						=	512.f;               
	defaultPrefs.niveauLines 						=	6;              
	defaultPrefs.emptyBeats 						=	1;               
	defaultPrefs.backgroundColor 					=	DefBgColor;
	defaultPrefs.gr 								=	DefGridColor;
	defaultPrefs.bd 								=	DefBodyColor;               
	defaultPrefs.objectColor 						=	DefObjectColor;
	defaultPrefs.oneTime 							=	0.5f;                  
	defaultPrefs.maxDwell 							=	-1.f;                 
	defaultPrefs.movieInfo 							=	false;                
	
	// open gl stuff
	defaultPrefs.mag								=	DefMag;				
	defaultPrefs.objectColorMode					=	greyObj;				
	//defaultPrefs.matrix[4][4]						=	defMatrix;			// Darstellungsmatrix	
	defaultPrefs.os									=	clubs;				// object-Style
	defaultPrefs.solidWorld							=	true;				// solid rendering
	defaultPrefs.shaderMode							=	GL_SMOOTH;				// smooth rendering
	defaultPrefs.env								=	true;				// alles environment ein
	defaultPrefs.fog								=	false;				// fog        
	defaultPrefs.unicycles							=	false;				// unicycles        
	defaultPrefs.sky								=	false;				// sky
	defaultPrefs.skyColor							=	DefSkyColor;
	defaultPrefs.jointObjects							=	true;				
	defaultPrefs.showStage							=	false;				
	defaultPrefs.stageColor							=	DefStageColor;
	defaultPrefs.lowerLeftStage						=	Vector3D(-400,0,-500);
	defaultPrefs.upperRightStage					=	Vector3D(400,800,-500);
	defaultPrefs.rotationAroundCenter				=	0;					
	defaultPrefs.bendAngle							=	0;					
	defaultPrefs.colorOffset						=	0;					
	defaultPrefs.menuMarkers						=	true;				
	defaultPrefs.fogStart							=	0.0f;				
	defaultPrefs.fogEnd								=	50;					
	defaultPrefs.fogDensity							=	0.03f;				
	defaultPrefs.fieldOfViewY						=	60. ;	
	defaultPrefs.levelOfDetail						=	DefLevelOfDetail;	
	defaultPrefs.niveauColor						=	DefRed; 
	defaultPrefs.fog_color							=	sysColor(0.5f, 0.5f, 0.5f);	
	defaultPrefs.floorColor							=	sysColor(0.f,0.6f,0.f);
	defaultPrefs.floorGridColorLight				=	sysColor(0.9f,.9f,0.9f);	
	defaultPrefs.floorGridColorDark					=	sysColor(0.3f,.3f,0.3f);	
	defaultPrefs.luminance							=	.8f ;				
	defaultPrefs.niveauBlendValue					=	.3f ;				
	defaultPrefs.worldWinSizeX						=	DefWorldWinSizeX;	
	defaultPrefs.worldWinSizeY						=	DefWorldWinSizeY;	
	defaultPrefs.worldWinSizeW						=	DefWorldWinSizeW;	
	defaultPrefs.worldWinSizeH						=	DefWorldWinSizeH;	
	defaultPrefs.patternWinSizeX					=	defPatternWinSizeX;	
	defaultPrefs.patternWinSizeY					=	defPatternWinSizeY;	
	defaultPrefs.patternWinSizeW					=	defPatternWinSizeW;	
	defaultPrefs.patternWinSizeH					=	defPatternWinSizeH;	
	defaultPrefs.infoWinSizeX						=	defInfoWinSizeX;	
	defaultPrefs.infoWinSizeY						=	defInfoWinSizeY;	
	defaultPrefs.infoWinSizeW						=	defInfoWinSizeW;	
	defaultPrefs.infoWinSizeH						=	defInfoWinSizeH;	
	defaultPrefs.scenesWinSizeX						=	defScenesWinSizeX;	
	defaultPrefs.scenesWinSizeY						=	defScenesWinSizeY;	
	defaultPrefs.scenesWinSizeW						=	defScenesWinSizeW;	
	defaultPrefs.scenesWinSizeH						=	defScenesWinSizeH;	
	defaultPrefs.positionWinSizeX					=	defPositionWinSizeX;	
	defaultPrefs.positionWinSizeY					=	defPositionWinSizeY;	
	defaultPrefs.positionWinSizeW					=	defPositionWinSizeW;	
	defaultPrefs.positionWinSizeH					=	defPositionWinSizeH;	
	defaultPrefs.showInfoWindow						=	true;				
	defaultPrefs.showEditorWindow					=	true;					
	defaultPrefs.showScenesWindow					=	false;				
	defaultPrefs.showPositionWindow					=	false;					
	defaultPrefs.editorTextColor					=	sysColor(.0f,0.f,.0f);	
	defaultPrefs.editorErrColor						=	sysColor(1.f,.3f,.3f);	
	defaultPrefs.useFullCursor						=	true;					
	defaultPrefs.editorCursorTextColor				=	sysColor(.5f,0.f,.5f);	
	defaultPrefs.editorCursorBgColor				=	sysColor(.6f,.6f,.6f);	
	defaultPrefs.idleCursorWidth					=	1;						
	defaultPrefs.flashTime							=	100;					
	defaultPrefs.jumpToErrPosition					=	true;					
	defaultPrefs.showCmdLineFileInTextEditor 		=	false;					
	defaultPrefs.helpActive							=	true;					
	defaultPrefs.generateCausalFromTextEditorOnStart=	false;						
	defaultPrefs.autosaveCausalFromTextEditor 		=	false;					
	defaultPrefs.walkFactor							=	5.f;					
	defaultPrefs.modelScale							=	100;					
	defaultPrefs.playDropSound						=	false;                
	defaultPrefs.minActionTime						=	.2f;                  
	defaultPrefs.showLogo							=	false;                
	defaultPrefs.showCameraPath						=	false;                
	defaultPrefs.showJugglerPos						=	true;                
	defaultPrefs.showJugglerPoi						=	false;                
	defaultPrefs.showJugglerModifiers				=	false;                
	defaultPrefs.showAbstracts						=	false;                
	defaultPrefs.texturedClub						=	false;                
	defaultPrefs.texturedBall						=	false;                
	defaultPrefs.texturedSky						=	false;                
	defaultPrefs.texturedFloor						=	false;                
	defaultPrefs.texturedJuggler					=	false;                
	defaultPrefs.texturedHead						=	false;                
	defaultPrefs.showScene							=	false;                
	defaultPrefs.solidNiveaus						=	false;                
	defaultPrefs.solidFloor							=	false;                
	defaultPrefs.showFloorGrid						=	true;                 
	defaultPrefs.antialias							=	false;                
	defaultPrefs.showObjectVektors					=	false;                
	defaultPrefs.repeatFloor						=	1;                    
	defaultPrefs.lookThroughFloor					=	true;                 
	defaultPrefs.showJugglerNumbers					=	false;                
	defaultPrefs.showObjectNumbers					=	false;                
	defaultPrefs.showSiteswapValue					=	false;                
	defaultPrefs.numbersColor						=	sysColor(.6, .6, .6); 
	defaultPrefs.blindfolded						=	false;                
	defaultPrefs.emphaseShortDistanceFlag			=	false;                
	defaultPrefs.emphaseShortDistanceFlagForSelfs 	=	true;                 	
	defaultPrefs.minObjectDistance					=	50;                   
	defaultPrefs.movieFrameRate						=	15;                   
	defaultPrefs.fadeEvents							=	false;                
	defaultPrefs.fadeFactor							=	1.f;                  
	defaultPrefs.numberOfAdditionalEvents			=	5;                    
	defaultPrefs.defhandDwell						=	.5f;                  
	defaultPrefs.autoHurriedMode					=	true;                 
	defaultPrefs.vertHandMovement					=	8.0;                  
	defaultPrefs.armForeFactor						=	.5;                   
	defaultPrefs.armSideFactor						=	.5;                   
	defaultPrefs.ellbowDeflection					=	.15f;                  
	defaultPrefs.deflectionPassFactor				=	1.5;                  
	defaultPrefs.throwAngle							=	10;                   
	defaultPrefs.passCorrectionVector				=	Vector3D( SideOffset, UpOffset, BackOffset); 
	defaultPrefs.catchCorrectionVector				=	Vector3D( CatchSideOffset, CatchUpOffset, CatchBackOffset); 
	defaultPrefs.showHandPath						=	false;
	defaultPrefs.animationView_mouseControl			=	true;                 
	defaultPrefs.modeSwitchCode						=	VanillaCode;          
	defaultPrefs.textSize							=	.8f;                  
	defaultPrefs.soundOutput						=	true;                 
	defaultPrefs.textEditorAutoStartFlag			=	false;                
	defaultPrefs.screenWidth						=	640;                  
	defaultPrefs.screenHeight						=	480;                  
	defaultPrefs.newGiraffeModel					=	true;                 
	defaultPrefs.causalEditorHasOpenWindows			=	true;                 
	defaultPrefs.askForSaveFlag						=	true;                 
	defaultPrefs.writeLogfile						=	true;                 
	defaultPrefs.conOutput							=	false;                 
	defaultPrefs.showMenuHotkeys					=	true;                 
	defaultPrefs.showMenuSelectors					=	true;                 
	defaultPrefs.waitForUserInput					=	false;                
#ifdef MAKE_QUICKTIME_MOVIE
	defaultPrefs.theCodec							=	kAnimationCodecType;  
#endif
	defaultPrefs.selfDwellElevation					=	1.;                  
	defaultPrefs.dwellElevation						=	15.;                  
	defaultPrefs.useTapAnimationValues				=	false;                
	defaultPrefs.showMessageWindow					=	false;       	
}

