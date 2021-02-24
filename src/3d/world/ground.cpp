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

Ground::Ground(QEntity *t_rootEntity, QColor &t_color)
  : planeMesh(new Qt3DExtras::QPlaneMesh()),
    planeTransform(new Qt3DCore::QTransform()),
    color(t_color),
    groundMetalRoughnessMaterial(new Qt3DExtras::QMetalRoughMaterial()),
    groundBaseColor(new Qt3DRender::QTextureLoader()),
    groundMetalness(new Qt3DRender::QTextureLoader()),
    groundRoughness(new Qt3DRender::QTextureLoader()),
    groundNormal(new Qt3DRender::QTextureLoader()),
    groundAmbientOcclusion(new Qt3DRender::QTextureLoader())
{
  // Plane shape data
  planeMesh->setWidth(GROUND_SIZE);
  planeMesh->setHeight(GROUND_SIZE);

  // Plane mesh transform
  planeTransform->setTranslation(QVector3D(0.0f, GROUND_POSY, 0.0f));

  // Plane texture
  groundBaseColor->setSource(QUrl(QStringLiteral(GROUND_BASE_COLOR)));
  groundBaseColor->setFormat(Qt3DRender::QAbstractTexture::SRGB8_Alpha8);
  groundBaseColor->setGenerateMipMaps(true);
  groundNormal->setSource(QUrl(QStringLiteral(GROUND_NORMAL)));
  groundNormal->setGenerateMipMaps(true);
  groundRoughness->setSource(QUrl(QStringLiteral(GROUND_ROUGHNESS)));
  groundRoughness->setGenerateMipMaps(true);
  groundAmbientOcclusion->setSource(QUrl(QStringLiteral(GROUND_AMBIENT_OCCLUSION)));
  groundMetalness->setSource(QUrl(QStringLiteral(GROUND_METALNESS)));

  groundMetalRoughnessMaterial->setBaseColor(QVariant::fromValue(groundBaseColor));
  groundMetalRoughnessMaterial->setNormal(QVariant::fromValue(groundNormal));
  groundMetalRoughnessMaterial->setMetalness(QVariant::fromValue(groundMetalness));
  groundMetalRoughnessMaterial->setRoughness(QVariant::fromValue(groundRoughness));
  groundMetalRoughnessMaterial->setAmbientOcclusion(QVariant::fromValue(groundAmbientOcclusion));

  QEntity::setParent(t_rootEntity);
  addComponent(planeMesh);
  addComponent(groundMetalRoughnessMaterial);
  addComponent(planeTransform);
  setEnabled(enabled);

}

void Ground::setColor(QColor t_color)
{
  color = t_color;
}
