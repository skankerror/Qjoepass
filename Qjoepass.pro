QT       += core gui 3dcore 3drender 3dextras widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#msys2 add
win32 {
DEFINES -= UNICODE
}
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
  src/main.cpp \
  src/gui/mainwindow.cpp \
  src/gui/preferences.cpp \
  src/gui/settings.cpp \
  src/siteswap/handanimevents.cpp \
  src/siteswap/propanimevents.cpp \
  src/siteswap/siteswap.cpp

HEADERS += \
  src/3d/3dwindow.h \
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
  src/gui/mainwindow.h \
  src/gui/preferences.h \
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
  TODO

