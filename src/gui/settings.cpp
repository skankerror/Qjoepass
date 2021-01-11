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

MySettings::MySettings(QString t_fileName, QSettings::Format t_format)
{
    if( t_fileName.isEmpty() )
    {
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        t_fileName = path + "/Settings.ini";
    }
//    qDebug() << "Standard path : " << QStandardPaths::ConfigLocation;
    m_settings = new QSettings(t_fileName, t_format);
}

MySettings::~MySettings()
{
    delete m_settings;
}

bool MySettings::isValue(const QString &t_key)
{
   return m_settings->contains(t_key);
}

void MySettings::setValue(const QString &t_key, const QVariant &t_value)
{
    m_settings->setValue(t_key, t_value);
}

QVariant MySettings::value(const QString &t_key, const QVariant &t_defaultValue)
{
    return m_settings->value(t_key, t_defaultValue);
}

void MySettings::beginGroup(const QString &t_prefix)
{
    m_settings->beginGroup(t_prefix);
}

void MySettings::endGroup()
{
    m_settings->endGroup();
}

