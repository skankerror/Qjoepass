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

#INCLUDEPATH += \

SOURCES += \
  animsimple.cpp \
  camera.cpp \
  ground.cpp \
  juggler.cpp \
  jugglingball.cpp \
  jugglingring.cpp \
  light.cpp \
  main.cpp \
  my3dwindow.cpp \
  mycurves.cpp \
  mymainwindow.cpp \
  pirouette.cpp \
  preferences.cpp \
  settings.cpp \
  siteswap.cpp

HEADERS += \
  animsimple.h \
  camera.h \
  ground.h \
  juggler.h \
  jugglingball.h \
  jugglingring.h \
  light.h \
  my3dwindow.h \
  mycurves.h \
  mymainwindow.h \
  pirouette.h \
  preferences.h \
  qjoepass.h \
  settings.h \
  siteswap.h

TRANSLATIONS += \
    Qjoepass_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \

RESOURCES += \
    application.qrc \
    mesh/J_club2.qrc

DISTFILES += \
  LICENCE.md \
  README.md

