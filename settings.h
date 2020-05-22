#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>
#include <QSettings>
#include <QStandardPaths>

class MySettings
{
public:
    MySettings(QString fileName = "", QSettings::Format format = QSettings::IniFormat);
    ~MySettings();
//protected:
    bool isValue(const QString &key);
    void setValue(const QString &key, const QVariant &value);
    void beginGroup(const QString &prefix);
    void endGroup();

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
private:
    QSettings *pSettings_;

};

#endif // SETTINGS_H
