#include "settings.h"

MySettings::MySettings(QString fileName, QSettings::Format format)
{
    if( fileName.isEmpty() )
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        fileName = path + "/Settings.ini";
    }

    settings = new QSettings(fileName, format);
}

MySettings::~MySettings()
{
    delete settings;
}

bool MySettings::isValue(const QString &key)
{
   return settings->contains(key);
}

void MySettings::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

QVariant MySettings::value(const QString &key, const QVariant &defaultValue)
{
    return settings->value(key, defaultValue);
}

void MySettings::beginGroup(const QString &prefix)
{
    settings->beginGroup(prefix);
}

void MySettings::endGroup()
{
    settings->endGroup();
}

