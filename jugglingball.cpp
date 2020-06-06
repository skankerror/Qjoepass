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

#include "jugglingball.h"

JugglingBall::JugglingBall(QEntity *aRootEntity,
                           QSphereMesh *aSphereMesh,
                           QEffect *aEffect,
                           QColor &aColor)
  :sphereMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    sphereTransform(new Qt3DCore::QTransform()),
    color(aColor)
{
    sphereTransform->setScale(BALL_SCALE);

    diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
    shininessParameter->setName(QLatin1String(SHININESS));
    sphereMaterial->addParameter(diffuseColorParameter);
    diffuseColorParameter->setValue(QVariant::fromValue(color));
    sphereMaterial->addParameter(shininessParameter);
    shininessParameter->setValue(QVariant::fromValue(BALL_SHININESS));
    sphereMaterial->setEffect(aEffect);

    QEntity::setParent(aRootEntity);
    addComponent(aSphereMesh);
    addComponent(sphereMaterial);
    addComponent(sphereTransform);

}

void JugglingBall::setPosition(QVector3D position)
{
  if (m_position == position)
    return;

  m_position = position;
  emit positionChanged(position);
  updateTransForm();
}

void JugglingBall::updateTransForm()
{
  sphereTransform->setTranslation(m_position);
}
