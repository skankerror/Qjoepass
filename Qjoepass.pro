QT       += core gui 3dcore 3drender 3dextras widgets

CONFIG += c++11
CONFIG += console

DEFINES += QT_DEPRECATED_WARNINGS

#msys2 add
win32 {
DEFINES -= UNICODE
}

INCLUDEPATH += \
  src \
  src/3d \
  src/3d/juggler \
  src/3d/world \
  src/3d/prop \
  src/animation \
  src/gui \
  src/siteswap \
  translations

SOURCES += \
  src/3d/3dwindow.cpp \
  src/3d/juggler/jugglerhead.cpp \
  src/3d/juggler/jugglerleg.cpp \
  src/3d/juggler/jugglertrunk.cpp \
  src/3d/world/light.cpp \
  src/3d/world/ground.cpp \
  src/3d/juggler/juggler.cpp \
  src/3d/juggler/jugglerarm.cpp \
  src/3d/prop/jugglingprop.cpp \
  src/3d/prop/jugglingball.cpp \
  src/3d/prop/jugglingclub.cpp \
  src/3d/prop/jugglingring.cpp \
  src/animation/animation.cpp \
  src/animation/curve.cpp \
  src/animation/handanim.cpp \
  src/animation/handdwellanim.cpp \
  src/animation/propanim.cpp \
  src/animation/propdwellanim.cpp \
  src/animation/propparabolicanim.cpp \
  src/gui/jugglerpositionwidget.cpp \
  src/gui/scalediagramwidget.cpp \
  src/main.cpp \
  src/gui/mainwindow.cpp \
  src/gui/preferences.cpp \
  src/gui/settings.cpp \
  src/siteswap/handanimevents.cpp \
  src/siteswap/propanimevents.cpp \
  src/siteswap/siteswap.cpp

HEADERS += \
  src/3d/3dwindow.h \
  src/3d/juggler/jugglerhead.h \
  src/3d/juggler/jugglerleg.h \
  src/3d/juggler/jugglertrunk.h \
  src/3d/world/light.h \
  src/3d/world/ground.h \
  src/3d/juggler/juggler.h \
  src/3d/juggler/jugglerarm.h \
  src/3d/prop/jugglingprop.h \
  src/3d/prop/jugglingball.h \
  src/3d/prop/jugglingclub.h \
  src/3d/prop/jugglingring.h \
  src/animation/animation.h \
  src/animation/curve.h \
  src/animation/handanim.h \
  src/animation/handdwellanim.h \
  src/animation/propanim.h \
  src/animation/propdwellanim.h \
  src/animation/propparabolicanim.h \
  src/gui/jugglerpositionwidget.h \
  src/gui/mainwindow.h \
  src/gui/preferences.h \
  src/gui/scalediagramwidget.h \
  src/qjoepass.h \
  src/gui/settings.h \
  src/siteswap/handanimevents.h \
  src/siteswap/propanimevents.h \
  src/siteswap/siteswap.h

TRANSLATIONS += \
    Qjoepass_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  application.qrc \

DISTFILES += \
  HACKING \
  INSTALL.md \
  LICENCE.md \
  NOTES \
  README.md \
  TODO \
  doc/ANIMATION \
  doc/schema_animation.pdf

