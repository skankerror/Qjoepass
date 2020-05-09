/*
 *  languageSupport.h
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
#ifndef __LANGUAGE_SUPPORT__
#define __LANGUAGE_SUPPORT__


#if defined(__cplusplus)
extern "C"
{
#endif

#include "headers.h"


enum 	{
	//Dialogs
	languageRunaroundProjectCloseDialog,
	languageHintDialog,		

	// all menus
	languageAllMenusOpenDoc,
	languageAllMenusQuit,

	languageRunaroundMenuFile,
			languageRunaroundFileNew,
			languageRunaroundFileOpen,
			languageRunaroundFileSave,
			languageRunaroundFileSaveAs,
			languageRunaroundFileSelect,
			languageRunaroundFileClose,

	languageRunaroundMenuPositions,
			languageRunaroundPositionsSideToSide,
			languageRunaroundPositionsBackToBack,
			languageRunaroundPositionsLine,
			languageRunaroundPositionsDropbackLine,
			languageRunaroundPositionsCircle,
			languageRunaroundPositionsFeed,
			languageRunaroundPositionsGorillaFeed,
			languageRunaroundPositionsRotateAllPositions,

	languageRunaroundMenuEditScenesList,
			languageRunaroundEditCopyNext,
			languageRunaroundEditAddPositionEnd,	
			languageRunaroundEditAddPositionSelection,
			languageRunaroundEditDeletePositionEnd,
			languageRunaroundEditDeletePositionSelection,
			languageRunaroundEditCycleClockwise, 	
			languageRunaroundEditCycleCounterclockwise, 	
			languageRunaroundEditAddJuggler,
			languageRunaroundEditRemoveJuggler,	
			languageRunaroundEditNewline,
			languageRunaroundEditDouble,
			languageRunaroundEditHalve,
	
	languageRunaroundMenuSelectScenes,
			languageRunaroundSelectScenesA,
			languageRunaroundSelectScenesB,
			languageRunaroundSelectScenesUnselect,
			languageRunaroundEditRotateClockwise,
			languageRunaroundEditRotateCounterClockwise,
	
	languageRunaroundMenuEditCurrentScene,
			languageRunaroundEditHorizontal,
			languageRunaroundEditVertical,	
			languageRunaroundFileEditCausal,
			languageRunaroundFileEditText,
	
		
	languageRunaroundMenuView,
			languageRunaroundAnimatedScene,
			languageRunaroundViewGrid,
			languageRunaroundViewPos,
			languageRunaroundViewPoi,
			languageRunaroundViewShowFrame,	
			languageRunaroundViewShowIndex,
			languageRunaroundViewMoreCorners,	
			languageRunaroundViewLessCorners,
			languageRunaroundViewRotateFrameCW,
			languageRunaroundViewRotateFrameCCW,
			languageRunaroundViewScaleUp,
			languageRunaroundViewScaleDown,		
	
	languageRunaroundMenuExperts,
			languageRunaroundMenuRemoveUnused,
			languageRunaroundMenuAttachDefault,
			
	languageRunaroundMenuRun,

	/*
AUFRÄUMEN

	languageRunaroundMenuEditInput,
	languageRunaroundEditXpositions, 	
*/
	

	languageInfoMenuMovie,
			languageInfoMovieShowInfo,
			languageInfoMovieMoreFrames,
			languageInfoMovieLessFrames,
			languageInfoThrows,
			languageInfoPattern,
			languageInfoRunaround,
			languageInfoFramerate,
			languageInfoTime,
			languageInfoText,
			languageObjectColorText,
			languageInfohideWindow,
			languageInfoShowAnimation,
			languageInfoShowCollisionWarning,
			

	languageAnimMenu,

		languageAnimMenuAnim,
            languageAnimAnimAnimate,	
			languageAnimAnimStep,
			languageAnimAnimThrow,
			languageAnimAnimShow,	
			languageAnimAnimBigger,
			languageAnimAnimSmaller,
			languageAnimObjectBigger,
			languageAnimObjectSmaller,
			languageAnimAnimFaster,
			languageAnimAnimSlower,
			languageAnimAnimHigher,
			languageAnimAnimLower,
			languageAnimAnimDrop,
			languageAnimAnimTableJuggling,
			languageAnimAnimDefault,
			languageAnimAnimTap,
			languageAnimAnimWarningNarrow,
			languageAnimAnimWarningWide,
			languageAnimAnimShortDistance,
			languageAnimAnimShortDistanceForSelfs,

		languageAnimMenuFile,
			languageAnimFileEdit,
			languageAnimFileAnimate,
			languageAnimFileEditPlus,
			languageAnimFileSaveAs,
			languageAnimFileToEditor,
			languageAnimFileToExternalEditor,
			languageAnimFileSaveChanged,
	
		languageAnimMenuWorkspace,
			languageAnimWorkspaceOpen,
			languageAnimWorkspaceSaveAs,
	
		languageAnimMenuObject,
			languageAnimObjectRandom,
			languageAnimObjectClubs,
			languageAnimObjectFishes,
			languageAnimObjectBalls	,
			languageAnimObjectRings,
			languageAnimObjectPancakes,
			languageAnimObjectNothing,
			languageAnimObjectMoreSpin,
			languageAnimObjectLessSpin,
			languageAnimObjectResetSpin,
			
		languageAnimMenuColor,
			languageAnimColorJugglers,
			languageAnimColorObjects,

		languageAnimMenuEnvironment,
			languageAnimEnvironmentSky,
			languageAnimEnvironmentFloor,
			languageAnimEnvironmentFog,
			languageAnimEnvironmentUnicycles,
			languageAnimEnvironmentBlindfolded,
			languageAnimEnvironmentScene,
			languageAnimEnvironmentLogo,
			languageAnimEnvironmentJugglerId,
			languageAnimEnvironmentObjectId,
			languageAnimEnvironmentSiteswapId,
			
			
		languageAnimMenuTexture,
			languageAnimTextureHead,
			languageAnimTextureSky,
			languageAnimTextureFloor,
			languageAnimTextureBalls,
			languageAnimTextureClubs,
			languageAnimTextureAllTextures,
			
		languageAnimMenuCamera,
			languageAnimCameraFront,
			languageAnimCameraTop,
			languageAnimCameraBird,
			languageAnimCameraNext,
			languageAnimCameraMove,
			languageAnimCameraVomit,
			languageAnimCameraTess,
			languageAnimCameraObject,
			languageAnimCameraResetMag,			
					
		languageAnimMenuAbstract,
			languageAnimAbstractAxis,
			languageAnimAbstractNiveaus,
			languageAnimAbstractCameraPath,
			languageAnimAbstractHandPath,
			
		languageAnimMenuWalk,
			languageAnimWalkGrid,
			languageAnimWalkPos,
			languageAnimWalkPoi,
			languageAnimWalkAllPoints,
			languageAnimWalkModifiers,	
		
		languageAnimMenuView,
		
		languageAnimMenuRenderer,
			languageAnimRendererSolid,
			languageAnimRendererSolidNiveaus,
			languageAnimRendererSolidFloor,
			languageAnimRendererSmooth,
			languageAnimRendererAntialias,
			languageAnimRendererLookThroughFloor,
			languageAnimRendererDetails,
	
		languageAnimMenuFullScreen,
						
		languageAnimMenuSound,
			languageAnimSoundThrow,
			languageAnimSoundCatch,
			languageAnimSoundKind,
			languageAnimSoundDynamicHeight,
			languageAnimSoundDynamicPosition,
			languageAnimSoundFaults,
	
		languageAnimMenuMedia,
			languageAnimMediaMakeMovie,
			languageAnimMediaMakeScreenshotSeries,
			languageAnimMediaSelectCompressor,
			languageAnimMediaMakeScreenshot,
			
		languageAnimMenuWindows,
			languageAnimWindowsInfoWindow,
			languageAnimWindowsEditorWindow,
			languageAnimWindowsCausalWindow,
			languageAnimWindowsCausalDuplicate,
			languageAnimWindowsRunaroundWindow,
			languageAnimWindowsPreviousWindow,
	
		languageAnimMenuStyles,
		languageAnimMenuPositions,
		
		languageAnimMenuConfigure,
			languageAnimMenuMarkers,
			languageAnimMenuHotkeys,
            languageAnimMenuWatchfile,
            languageAnimMenuLogfile,
    
			languageAnimMenuExternalEditor,
	
		languageAnimMenuRestart,
		languageAnimFileOpenPrechacthis,
		languageAnimMenuAbout,				
							
			
			
		languageEditMenuFile,
			languageEditFileOpen,
			languageEditFileClose,
			languageEditFileSave,
			languageEditFileSaveAs,

		languageEditMenuEdit,
			languageEditClear,
			languageEditCapitalize,
			languageEditBlockCursor,
			languageEditVanilla,
			languageEditExtended,

		languageEditMenuSiteswap,
			languageEditPattern,
			languageEditFourHanded,
			languageEditCurentLine,
			languageEditCurentLineFour,

		languageEditMenuCausal,
			languageEditGenerateCausalOnStart,
			languageEditGenerateCausalNow,
			languageEditCausalSameWindow,
			languageEditAutoSaveCausal,

		languageEditMenuJ2,
			languageEditCallJ2,
			languageEditJ2Help,

		languageEditMenuHelp,
		
		
		languageCdewMenuPS,
			languageCdewPSSaveAs, 
			languageCdewPSColor,
			
		languageCdewMenuHeader,
			languageCdewHeaderOpen,
			languageCdewHeaderSaveAs,
			languageCdewHeaderClear,
			
		languageCdewMenuTransfer,
			languageCdewStart,
			languageCdewAlwaysStart,
			languageCdewOpenText,
			languageCdewAlwaysOpenText,

		languageCdewMenuFile,
			languageCdewFileNew,
			languageCdewFileDuplicate,
			languageCdewFileOpen,
			languageCdewFileClose,
			languageCdewFileSave,
			languageCdewFileSaveAs,
			languageCdewFileSaveAutoName,
			languageCdewFileAdd,
			languageCdewFileMerge,
			languageCdewFileSaveTemplate,
			languageCdewFileScreenshot,
			languageCdewFileSign,
			languageCdewFileShow,
			
		languageCdewMenuFix,
			languageCdewFixNow,
			languageCdewFixAlways,
			languageCdewFixNoHurries,
			
		languageCdewMenuMarkers,	
			
//		languageCdewMenuMarkArrowStyle,
		languageCdewMarkBackgroundColor,
			languageCdewMarkArrowColorReset,	
		languageCdewMarkArrowColor,
			languageCdewMarkBackgroundColorReset,
			languageCdewMarkColor1,
			languageCdewMarkColor2,
			languageCdewMarkColor3,
			languageCdewMarkColor4,
			languageCdewMarkColor5,
			languageCdewMarkColor6,
			languageCdewMarkColor7,
			languageCdewMarkColor8,
		
		languageCdewMarkArrowThickness,
			languageCdewMarkArrowThicknessReset,
			languageCdewMarkArrowThickness1,
			languageCdewMarkArrowThickness2,
			languageCdewMarkArrowThickness3,
			languageCdewMarkArrowThickness4,

		languageCdewMarkRepeat,
			languageCdewMarkRepeatStart,
			languageCdewMarkRepeatEnd,
			languageCdewMarkRepeatDelete,
			
		languageCdewMarkArrowDefaults,

		languageCdewMenuModifier,
			languageCdewModifierBeep,
			languageCdewModifierBounce,
			languageCdewModifierNoBeep,
			languageCdewModifierNoBounce,
			languageCdewModifierHalfspin,
	
		languageCdewMenuMark,
			languageCdewMarkPath,
			languageCdewMarkPass,
			languageCdewMarkRight,
			languageCdewMarkBeat,
			languageCdewMarkJuggler,
			languageCdewMarkHand,
			languageCdewMarkInvert,
	
		languageCdewMenuAction,
			languageCdewMarkInvertHeight,
			languageCdewMarkShiftLeft,
			languageCdewMarkShiftRight,
			languageCdewMarkXHands,		
		
		languageCdewMenuUnmark,
			languageCdewUnmarkPath,
			languageCdewUnmarkBeat,
			languageCdewUnmarkJuggler,
			languageCdewUnmarkHand,
			languageCdewUnmarkAll,
			
		languageCdewMenuCycle,
			languageCdewCycleLeft,
			languageCdewCycleRight,
			languageCdewCycleUp,
			languageCdewCycleDown,
			languageCdewSwap1And2,
			
		languageCdewMenuContent,
			languageCdewContentClear,
			languageCdewContentDefault,
			languageCdewHandsDistribute,
			languageCdewContentDistribute,
			languageCdewContentRotate,
			languageCdewContentResetArrows,
			
		languageCdewMenuSize,
			languageCdewSizeAddJuggler,
			languageCdewSizeRemoveJuggler,
			languageCdewSizeAddBeat,
			languageCdewSizeRemoveBeat,
			languageCdewSizeAddReverse,
			languageCdewSizeAddSymetric,
			languageCdewSizeAddSame,
						
		languageCdewMenuEdit,
			languageCdewEditUndo,
			languageCdewEditRedo,
			languageCdewEditCut,
			languageCdewEditCopy,
			languageCdewEditPaste,
	
//		languageCdewMenuMarker,
			
		languageCdewMenuDisplay,
			languageCdewDisplaySingle,
			languageCdewDisplaySync,
			languageCdewDisplayColor,
			languageCdewDisplayNext,
			languageCdewDisplayModePrint,
			languageCdewColorModePrint,
						
		languageCdewMenuColor,
			languageCdewColorReset,
			languageCdewColorLength,
			languageCdewColorPath,
			languageCdewColorUser,
			languageCdewColorJuggler,
			languageCdewColorNext,
			
			
		languageCdewMenuView,
			languageCdewMenuViewThrow,
				languageCdewViewHurries,
				languageCdewViewCrosses,
				languageCdewViewFaults,
				languageCdewViewStart,
			languageCdewMenuViewPattern,
				languageCdewViewDelays,	
				languageCdewViewLines,
				languageCdewViewCellCol,
				languageCdewViewSiteswap,
				languageCdewViewObjects,
				languageCdewViewBeats,
				languageCdewViewAllHands,
			languageCdewViewTimeC,
			
		languageCdewMenuGeneral,
			languageCdewGeneralBackground,
			languageCdewGeneralSnap,
			
		languageCdewMenuWindow,
			languageCdewWindowAnimation,
			languageCdewWindowNextCdew,
				
			
languageEnd
};

	extern		char *languageStrings[languageEnd];



void 		initLanguage( void );
wwwBoolean	readLanguageFile( void );


#if defined(__cplusplus)
}
#endif

#endif
