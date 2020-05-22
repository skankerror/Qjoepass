#include "settings.h"

MySettings::MySettings(QString fileName, QSettings::Format format)
{
    if( fileName.isEmpty() )
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        fileName = path + "/Settings.ini";
    }

    pSettings_ = new QSettings(fileName, format);
}

MySettings::~MySettings()
{
    delete pSettings_;
}

bool MySettings::isValue(const QString &key)
{
   return pSettings_->contains(key);
}

void MySettings::setValue(const QString &key, const QVariant &value)
{
    pSettings_->setValue(key, value);
}

QVariant MySettings::value(const QString &key, const QVariant &defaultValue)
{
    return pSettings_->value(key, defaultValue);
}

void MySettings::beginGroup(const QString &prefix)
{
    pSettings_->beginGroup(prefix);
}

void MySettings::endGroup()
{
    pSettings_->endGroup();
}

