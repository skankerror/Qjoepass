#ifndef JPPREF_H
#define JPPREF_H

#include <QSettings>

#include "parseSiteswap.h"
#include "prefDefinitionValues.h"
#include "world.h"

class jpPref
{

public:
    explicit jpPref();
    virtual ~jpPref();

private:
void   loadSettings();

};

#endif // JPPREF_H
