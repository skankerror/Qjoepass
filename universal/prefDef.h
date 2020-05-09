/*
 *  prefDef.h
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
#ifndef __PREF_DEF__
#define __PREF_DEF__


#if defined(__cplusplus)
extern "C" {
#endif



#ifdef MAC_VERSION
#include "camera.h"
#endif
#include"vtypes.h"
#include "graphics.h"

#define VanillaCode             'b'
#define ExtendedCode            'x'
	
	
	
extern int			prefVersion;
extern GLfloat		light_position[];
extern bool			showShortTimeMovementPaths;
	
	
void initPrefs( void );

	

    typedef struct {
//        prefsHeader ph;							// Enthlt Version, Gre der Struktur. obsolet

        bool        saved;                          // have prefs been saved or are they default?
        bool        sm;                             // SiteswapMode
        float       animationSpeed;                 // Animationsgeschwindigkeit
        int         fpsInMovie;                     // Frames per second in movie creation
        bool        p,f,i,e;                        // Ausgaben fr Standpunkt, frameRate Info, Events
        int         jugglerColor;					// figures
        bool        h2, mov, s;						// 2, bewegung, sound
        bool        a,floor;						// Achsen, Boden

        float		deflection;							// Handauslenkung (horizontal)
        float		gravity;						// Schwerkraft
        float		objectFaktor;
        float		objectSize;						// ObjektGre
        float		bodyFaktor;						// StiftGre

        bool       t;								// Werfen: Sound,
        bool       dt, dp;							// Dynamischer Sound Hhe, Position
        bool       es;								// Fehlerausgabe
        int			syncMode;						// Was geschieht in synchronen Wrfen?
        bool       jNumbers;						// JongleurNummern
        bool       obeyDelay;						// delay bercksichtigen
        bool       switch1x;						// switch hands automatically after an 1x
        bool       showNiveaus;						// show Niveaus
        float		floorSize;						// floor size
        float		niveauSize;						// niveau size
        int			niveauLines;					// Lines in niveau
        int			emptyBeats;						// empty beats after a throw
        sysColor	backgroundColor, gr, bd;						// Hintergrund-, Boden- & Jongleurfarbe
        sysColor	objectColor;
        float		oneTime;						// so lange dauert durchgeben
        float		maxDwell;						// absolute maximum dwell time;
        bool		movieInfo;						// showMovieInfo
#ifdef MAC_VERSION
        // mac stuff
        float		timeStep;						// timeStep
        float		floorLines;
        float		penSize;                         // StiftGre

        int			os;								// object-Style


        float		mag;							// stdMagnification
        int			objectColorMode;				// object color
        bool		dd;								// 3dMode
        sysColor	le,re;							// Farben fr die Augen
        float		dAuge, zeroPlain;				// 3d-Parameter

        Camera		camera;							// Cameraposition
        rect		screenRect;						// Fensterposition
        rect		infoRect;						// Info-Fenster
        rect		siouxRect;						// Sioux-Fenster

        int			fpsEntry;						// What entry in tick List?
        bool		durationFixed;					// movie will have constant duration
        bool		openAnimDialog;					// open animation dialog whencreating a movie
        bool		tempValues;						// use values only for generating a movie
        bool		movieObjectStyle;				// defines what for balls are drawn in movie

        bool		noOther;						// kooperativ sein?
#else
        // open gl stuff
        float		mag;							// stdMagnification
        int			objectColorMode;				// object color
        int worldWinW, worldWinH;					// Fenstergre
        GLfloat		matrix[4][4];					// Cameraposition
        int			os;								// object-Style
        bool		solidWorld;						// draws lines instead of 3d objects
        bool		shaderMode;						// flat / smooth shading
        bool		env;							// hide all environment
        bool		fog;							// show fog
        bool		unicycles;						// show / hide unicycles
        bool		sky;
        sysColor	skyColor;
        bool       jointObjects;
        bool       showStage;
        sysColor	stageColor;
        Vector3D	lowerLeftStage;
        Vector3D	upperRightStage;
        float		rotationAroundCenter;
        float		bendAngle;
        int			colorOffset;
        bool		menuMarkers;
        float		fogStart;
        float		fogEnd;
        float		fogDensity;
        float		fieldOfViewY;
        int			levelOfDetail;
        Vector3D	niveauColor;
        sysColor	fog_color;
        Vector3D	floorColor;
        Vector3D	floorGridColorLight;
        Vector3D	floorGridColorDark;
        float		luminance;
        float		niveauBlendValue;
        float		worldWinSizeX;
        float		worldWinSizeY;
        float		worldWinSizeW;
        float		worldWinSizeH;
        float		patternWinSizeX;
        float		patternWinSizeY;
        float		patternWinSizeW;
        float		patternWinSizeH;
        float		infoWinSizeX;
        float		infoWinSizeY;
        float		infoWinSizeW;
        float		infoWinSizeH;
        float		scenesWinSizeX;
        float		scenesWinSizeY;
        float		scenesWinSizeW;
        float		scenesWinSizeH;
        float		positionWinSizeX;
        float		positionWinSizeY;
        float		positionWinSizeW;
        float		positionWinSizeH;
        bool		showInfoWindow;
        bool		showEditorWindow;
        bool		showScenesWindow;
        bool		showPositionWindow;
        sysColor	editorTextColor;
        sysColor	editorErrColor;
        bool		useFullCursor;
        sysColor	editorCursorTextColor;
        sysColor	editorCursorBgColor;
        int			idleCursorWidth;
        int			flashTime;
        bool		jumpToErrPosition;
        bool		showCmdLineFileInTextEditor;
        bool		helpActive;
        bool		generateCausalFromTextEditorOnStart;
        bool		autosaveCausalFromTextEditor;
        float		walkFactor;
        float		modelScale;
        bool		playDropSound;
        float		minActionTime;
        bool		showLogo;
        bool		showCameraPath;
		bool		showHandPath;
        bool		showJugglerPos;
        bool		showJugglerPoi;
        bool		showJugglerModifiers;
        bool		showAbstracts;
        bool		texturedClub;
        bool		texturedBall;
        bool		texturedSky;
        bool		texturedFloor;
        bool		texturedJuggler;
        bool		texturedHead;
        bool		showScene;
        bool		solidNiveaus;
        bool		solidFloor;
        bool		showFloorGrid;
        bool		antialias;
        bool		showObjectVektors;
        int			repeatFloor;
        bool		lookThroughFloor;
        bool		showJugglerNumbers;
        bool		showObjectNumbers;
        bool		showSiteswapValue;
        Vector3D	numbersColor;
        bool		blindfolded;
        bool		emphaseShortDistanceFlag;
        bool		emphaseShortDistanceFlagForSelfs;
        float		minObjectDistance;
        long		movieFrameRate;
        bool		fadeEvents;
        float		fadeFactor;
        int			numberOfAdditionalEvents;
        float		defhandDwell;
        bool		autoHurriedMode;
        float		vertHandMovement;
        float		armForeFactor;
        float		armSideFactor;
        float		ellbowDeflection;
        float		deflectionPassFactor;
        float		throwAngle;
        Vector3D	passCorrectionVector;
        Vector3D	catchCorrectionVector;
        bool		animationView_mouseControl;
        char		modeSwitchCode;
        float		textSize;
        bool		soundOutput;
        bool		textEditorAutoStartFlag;
        float		screenWidth;
        float		screenHeight;
        bool		newGiraffeModel;
        bool		causalEditorHasOpenWindows;
        bool		askForSaveFlag;
        bool		writeLogfile;
        bool		conOutput;
        bool		showMenuHotkeys;
        bool		showMenuSelectors;
        bool		waitForUserInput;
        unsigned long theCodec;
        float		dwellElevation;
		float		selfDwellElevation;
		bool		useTapAnimationValues;
        bool		showMessageWindow;
#endif

    }
    prefsSettings;


    extern prefsSettings defaultPrefs, jpPrefs;




#if defined(__cplusplus)
}
#endif

#endif
