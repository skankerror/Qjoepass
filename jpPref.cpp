#include "jpPref.h"


jpPref::jpPref()
{
  loadSettings();

}

void jpPref::loadSettings()
{
QSettings settings("skankSoft", "Joe Pass");

settings.beginGroup("jpPref");
settings.setValue("saved", false);
settings.setValue("sm", xMode);
settings.setValue("animationSpeed", defSpeed);
settings.setValue("fpsInMovie", defFpsMovie);
settings.setValue("p", false);
settings.setValue("f", false);
settings.setValue("i", false);
settings.setValue("e", true);
settings.setValue("jugglerColor", multiColorJugglers);

settings.endGroup();

}
