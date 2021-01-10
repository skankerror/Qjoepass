/*
 * (c) 2020 Pat Co / M. C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settings.h"
#include <QtWidgets>

MySettings::MySettings(QString fileName, QSettings::Format format)
{
    if( fileName.isEmpty() )
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        fileName = path + "/Settings.ini";
    }
//    qDebug() << "Standard path : " << QStandardPaths::ConfigLocation;
    m_settings = new QSettings(fileName, format);
}

MySettings::~MySettings()
{
    delete m_settings;
}

bool MySettings::isValue(const QString &key)
{
   return m_settings->contains(key);
}

void MySettings::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

QVariant MySettings::value(const QString &key, const QVariant &defaultValue)
{
    return m_settings->value(key, defaultValue);
}

void MySettings::beginGroup(const QString &prefix)
{
    m_settings->beginGroup(prefix);
}

void MySettings::endGroup()
{
    m_settings->endGroup();
}

