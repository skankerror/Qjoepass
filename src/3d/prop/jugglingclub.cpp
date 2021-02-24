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

#include "jugglingclub.h"
#include <Qt3DRender>


JugglingClub::JugglingClub(QEntity *t_rootEntity,
                           QColor &t_color,
                           launchTypeClub t_launchType)
  : JugglingProp(t_rootEntity, t_color),
    m_launchType(t_launchType)
{
  auto clubMesh = new Qt3DRender::QMesh();
  clubMesh->setSource(QUrl(CLUB_MESH_SRC));
  m_propTransform->setScale(CLUB_SCALE);
  addComponent(clubMesh);
}

