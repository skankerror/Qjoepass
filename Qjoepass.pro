QT       += core gui 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
  src/animation \
  src/gui \
  src/siteswap \
  translations

SOURCES += \
  src/3d/3dwindow.cpp \
  src/animation/animation.cpp \
  src/animation/curve.cpp \
  src/3d/ground.cpp \
  src/3d/juggler.cpp \
  src/3d/jugglingball.cpp \
  src/3d/jugglingring.cpp \
  src/3d/light.cpp \
  src/main.cpp \
  src/gui/mainwindow.cpp \
  src/3d/pirouette.cpp \
  src/gui/preferences.cpp \
  src/gui/settings.cpp \
  src/siteswap/siteswap.cpp \
  src/siteswap/siteswapevent.cpp

HEADERS += \
  src/3d/3dwindow.h \
  src/animation/animation.h \
  src/animation/curve.h \
  src/3d/ground.h \
  src/3d/juggler.h \
  src/3d/jugglingball.h \
  src/3d/jugglingring.h \
  src/3d/light.h \
  src/gui/mainwindow.h \
  src/3d/pirouette.h \
  src/gui/preferences.h \
  src/qjoepass.h \
  src/gui/settings.h \
  src/siteswap/siteswap.h \
  src/siteswap/siteswapevent.h

TRANSLATIONS += \
    Qjoepass_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \

RESOURCES += \
  application.qrc \

DISTFILES += \
  HACKING \
  INSTALL.md \
  LICENCE.md \
  README.md \
  TODO

