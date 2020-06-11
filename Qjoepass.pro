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
  translations

SOURCES += \
  src/animsimple.cpp \
  src/ground.cpp \
  src/juggler.cpp \
  src/jugglingball.cpp \
  src/jugglingring.cpp \
  src/light.cpp \
  src/main.cpp \
  src/my3dwindow.cpp \
  src/mycurves.cpp \
  src/mymainwindow.cpp \
  src/pirouette.cpp \
  src/preferences.cpp \
  src/settings.cpp \
  src/siteswap.cpp

HEADERS += \
  src/animsimple.h \
  src/ground.h \
  src/juggler.h \
  src/jugglingball.h \
  src/jugglingring.h \
  src/light.h \
  src/my3dwindow.h \
  src/mycurves.h \
  src/mymainwindow.h \
  src/pirouette.h \
  src/preferences.h \
  src/qjoepass.h \
  src/settings.h \
  src/siteswap.h

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
  LICENCE.md \
  README.md \
  notes.txt

