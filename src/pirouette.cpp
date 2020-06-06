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

#include "pirouette.h"

Pirouette::Pirouette(QEntity *aRootEntity,
                     QMesh *aPirouetteMesh,
                     QEffect *aEffect,
                     QColor aColor)
  :pirouetteMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    pirouetteTransform(new Qt3DCore::QTransform()),
    color(aColor)
{
  pirouetteTransform->setScale(CLUB_SCALE);

  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  pirouetteMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  pirouetteMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(CLUB_SHININESS));
  pirouetteMaterial->setEffect(aEffect);


  QEntity::setParent(aRootEntity);
  addComponent(pirouetteTransform);
  addComponent(aPirouetteMesh);
  addComponent(pirouetteMaterial);
  setEnabled(enabled);
}

void Pirouette::setPosition(QVector3D aPosition)
{
  position = aPosition;
  pirouetteTransform->setTranslation(position); // A voir...
}
