QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut \
    -lGLU \
    -lGL

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    cdew \
    dialog \
    languageSupport \
    openGLversion \
    refactoring \
    runaround \
    syst \
    syst/ansi \
    syst/mac10 \
    syst/mac10mm \
    syst/unix \
    syst/win \
    tools \
    universal \
    headers \
    j2

SOURCES += \
    main.cpp \
    mymainwindow.cpp \
    cdew/causal_editor.cpp \
    cdew/cdewDraw.cpp \
    cdew/cdewMem.cpp \
    cdew/cdewMenu.cpp \
    cdew/cdewtools.cpp \
    cdew/cdewWork.cpp \
    dialog/dialog.cpp \
    j2/j2.cpp \
    languageSupport/languageSupport.cpp \
    openGLversion/animationView.cpp \
    openGLversion/animationWin.cpp \
    openGLversion/dxf.cpp \
    openGLversion/editor.cpp \
    openGLversion/farbwerte.cpp \
    openGLversion/fs.cpp \
    openGLversion/grafik.cpp \
    openGLversion/info.cpp \
    openGLversion/mainGL.cpp \
    openGLversion/opengltools.cpp \
    openGLversion/screenshot.cpp \
    openGLversion/stringtoscreen.cpp \
    openGLversion/texture.cpp \
    openGLversion/world.cpp \
    refactoring/hand.cpp \
    refactoring/hcurve.cpp \
    refactoring/juggler.cpp \
    refactoring/vector3d.cpp \
    runaround/inputWindow.cpp \
    runaround/listWindow.cpp \
    runaround/runaround.cpp \
    runaround/runaroundMenu.cpp \
    syst/ansi/ansiMovie.cpp \
    syst/ansi/ansiPlaySound.cpp \
    syst/ansi/ansifileaccess.cpp \
    syst/fileaccess.cpp \
    syst/mac10/MoreFilesX/MoreFilesX.c \
    syst/mac10/appleEvents.cpp \
    syst/mac10/macTypeCreator.cpp \
    syst/mac10/macfileaccess.cpp \
    syst/mac10/nav.c \
    syst/mac10mm/macMovie.cpp \
    syst/mac10mm/macSound.cpp \
    syst/mac10mm/newMovieRoutines.cpp \
    syst/mac10mm/quicktime.cpp \
    syst/movie.cpp \
    syst/myutil.cpp \
    syst/pathAccess.cpp \
    syst/sdInit.cpp \
    syst/unix/unix.cpp \
    syst/unix/unixMovie.cpp \
    syst/unix/unixPlaySound.cpp \
    syst/win/opensave.cpp \
    syst/win/winFileaccess.cpp \
    syst/win/winMovie.cpp \
    syst/win/winPlaySound.cpp \
    tools/applicationFolders.cpp \
    tools/cmdLine.cpp \
    tools/exitModules.cpp \
    tools/fileIO.cpp \
    tools/free.cpp \
    tools/myPrintf.cpp \
    tools/openglmenu.cpp \
    tools/prefs.cpp \
    tools/vmath.cpp \
    tools/windowsSize.cpp \
    universal/cameraControl.cpp \
    universal/fileHandler.cpp \
    universal/jpPrefs.cpp \
    universal/jugglers.cpp \
    universal/mem.cpp \
    universal/notation.cpp \
    universal/parseSiteswap.cpp \
    universal/pattern.cpp \
    universal/prefDef.cpp \
    universal/preprocess.cpp \
    universal/scanner.cpp \
    universal/siteswap.cpp \
    universal/styleScanner.cpp \
    universal/workspace.cpp

HEADERS += \
    mymainwindow.h \
    cdew/causal_editor.h \
    cdew/cdewClass.h \
    cdew/cdewDefs.h \
    cdew/cdewDraw.h \
    cdew/cdewMem.h \
    cdew/cdewMenu.h \
    cdew/cdew_switch.h \
    cdew/test_defines.h \
    dialog/dialog.h \
    dialog/dialogClass.h \
    graphics/graphics.h \
    headers/appnames.h \
    headers/definitions.h \
    headers/globals.h \
    headers/gpl.h \
    headers/graphics.h \
    headers/headers.h \
    headers/jpversion.h \
    j2/j2.h \
    languageSupport/languageSupport.h \
    openGLversion/animationClass.h \
    openGLversion/animationView.h \
    openGLversion/animationWin.h \
    openGLversion/dxf.h \
    openGLversion/editor.h \
    openGLversion/editorClass.h \
    openGLversion/farbwerte.h \
    openGLversion/fs.h \
    openGLversion/grafik.h \
    openGLversion/info.h \
    openGLversion/infoClass.h \
    openGLversion/mainGL.h \
    openGLversion/opengltools.h \
    openGLversion/screenshot.h \
    openGLversion/stringtoscreen.h \
    openGLversion/texture.h \
    openGLversion/world.h \
    refactoring/constants.h \
    refactoring/hand.h \
    refactoring/hcurve.h \
    refactoring/juggler.h \
    refactoring/vector3d.h \
    runaround/inputWindow.h \
    runaround/listWindow.h \
    runaround/runaround.h \
    runaround/runaroundClass.h \
    runaround/runaroundMenu.h \
    syst/JPResources.h \
    syst/fileSelector.h \
    syst/fileaccess.h \
    syst/mac10/MoreFilesX/MoreFilesX.h \
    syst/mac10/appleEvents.h \
    syst/mac10/fileSelectorCocoa.h \
    syst/mac10/macTypeCreator.h \
    syst/mac10/macfileaccess.h \
    syst/mac10/macio.h \
    syst/mac10/nav.h \
    syst/mac10mm/AudioPlayerViewController.h \
    syst/mac10mm/macMovie.h \
    syst/mac10mm/macSound.h \
    syst/mac10mm/newMovieRoutines.h \
    syst/mac10mm/quicktime.h \
    syst/movie.h \
    syst/myutil.h \
    syst/pathAccess.h \
    syst/playSound.h \
    syst/systemswitch.h \
    syst/win/opensave.h \
    tools/applicationFolders.h \
    tools/cmdLine.h \
    tools/exitModules.h \
    tools/fileIO.h \
    tools/free.h \
    tools/myPrintf.h \
    tools/mydefs.h \
    tools/openglmenu.h \
    tools/patternTitle.h \
    tools/prefs.h \
    tools/vmath.h \
    tools/vtypes.h \
    universal/cameraControl.h \
    universal/fileHandler.h \
    universal/jpPrefs.h \
    universal/jugglers.h \
    universal/keyDefs.h \
    universal/mem.h \
    universal/notation.h \
    universal/parseSiteswap.h \
    universal/pattern.h \
    universal/prefDef.h \
    universal/prefDefinitionValues.h \
    universal/preprocess.h \
    universal/scanner.h \
    universal/siteswap.h \
    universal/styleScanner.h \
    universal/workspace.h

TRANSLATIONS += \
    Qjoepass_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  AUTHORS \
  ChangeLog \
  NEWS \
  README \
  STAMP-H1 \
  aclocal.m4 \
  configure.ac \
  gpl-3.0.txt \
  jp1.icns \
  sys/Earth.jpg \
  sys/actualTemp.pass \
  sys/ball.sgi \
  sys/causal.ps \
  sys/cdewLast.pass \
  sys/cdew_template.txt \
  sys/club.sgi \
  sys/cmdLine.pass \
  sys/editor.txt \
  sys/errors.txt \
  sys/esccodes.txt \
  sys/floor.sgi \
  sys/head-2.sgi \
  sys/head.sgi \
  sys/help.txt \
  sys/j2.out \
  sys/jpLanguage.txt \
  sys/jpPrefs.txt \
  sys/logfile.txt \
  sys/openCdewWindows.txt \
  sys/profile.txt \
  sys/readme.txt \
  sys/siteswapSound00.mp3 \
  sys/siteswapSound01.mp3 \
  sys/siteswapSound02.mp3 \
  sys/siteswapSound03.mp3 \
  sys/siteswapSound04.mp3 \
  sys/siteswapSound05.mp3 \
  sys/siteswapSound06.mp3 \
  sys/siteswapSound07.mp3 \
  sys/siteswapSound08.mp3 \
  sys/siteswapSound09.mp3 \
  sys/siteswapSound10.mp3 \
  sys/siteswapSound11.mp3 \
  sys/siteswapSound12.mp3 \
  sys/siteswapSound13.mp3 \
  sys/siteswapSound14.mp3 \
  sys/sky.sgi \
  sys/sound0.mp3 \
  sys/sound1.mp3 \
  sys/sound2.mp3 \
  sys/sound3.mp3 \
  sys/sound4.mp3 \
  sys/sound5.mp3 \
  sys/stage.sgi \
  sys/styles.txt \
  sys/tmpNotationFile.pass \
  sys/watchfile.txt \
  sys/watchfileMac.txt \
  syst/mac10/MoreFilesX/MoreFilesXReadMe.txt \
  syst/mac10/fileSelectorCocoa.m \
  syst/mac10mm/AudioPlayerViewController.m \
  syst/mac10mm/audiofile.mp3
