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

#ifndef JUGGLINGRING_H
#define JUGGLINGRING_H

#include "jugglingprop.h"

class JugglingRing: public JugglingProp
{

  Q_OBJECT

public:

  JugglingRing(QEntity *t_rootEntity,
               QColor &t_color,
               launchTypeRing t_launchType = normalRing);

  launchTypeRing getLaunchType() const { return m_launchType; };

public slots:

  void setLaunchType(launchTypeRing t_launchType) { m_launchType = t_launchType; };

private:

  launchTypeRing m_launchType;
};

#endif // JUGGLINGRING_H
