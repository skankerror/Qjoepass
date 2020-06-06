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

#include "ground.h"

Ground::Ground(QEntity *aRootEntity, QEffect *aEffect, QColor &aColor)
  :planeMesh(new QPlaneMesh()),
    planeTransform(new Qt3DCore::QTransform()),
    planeMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    color(aColor),
    groundTextureMaterial(new QTextureMaterial()),
    groundTextureLoader(new QTextureLoader())
{
  // Plane shape data
  planeMesh->setWidth(GROUND_WIDTH);
  planeMesh->setHeight(GROUND_HEIGHT);

  // Plane mesh transform
  planeTransform->setTranslation(QVector3D(0.0f, GROUND_POSY, 0.0f));
  // Plane Material
  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  planeMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  planeMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(GROUND_SHININESS));
  planeMaterial->setEffect(aEffect);

  // Plane texture
  groundTextureLoader->setSource(QUrl(QStringLiteral("qrc:/texture/images/texture/parquet.png")));
  groundTextureLoader->setMirrored(false);
  groundTextureMaterial->setTexture(groundTextureLoader);

  QEntity::setParent(aRootEntity);
  addComponent(planeMesh);
//  addComponent(planeMaterial);
  addComponent(groundTextureMaterial);
  addComponent(planeTransform);
  setEnabled(enabled);

}

void Ground::setColor(QColor aColor)
{
  color = aColor;
  diffuseColorParameter->setValue(color);

}
