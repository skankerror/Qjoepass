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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>
#include <QSettings>
#include <QStandardPaths>

class MySettings
{
public:

    MySettings(QString t_fileName = "",
               QSettings::Format t_format = QSettings::IniFormat);
    ~MySettings();

    bool isValue(const QString &t_key);
    void setValue(const QString &t_key, const QVariant &t_value);
    void beginGroup(const QString &t_prefix);
    void endGroup();

    QVariant value(const QString &t_key,
                   const QVariant &t_defaultValue = QVariant());

private:

    QSettings *m_settings;

};

#endif // SETTINGS_H
