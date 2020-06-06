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

#include "jugglingring.h"


JugglingRing::JugglingRing(QEntity *aRootEntity,
                           QTorusMesh *torusMesh,
                           QEffect *aEffect,
                           QColor aColor)
  :ringMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    ringTransform(new Qt3DCore::QTransform()),
    color(aColor)

{
  ringTransform->setScale3D(QVector3D(RING_SCALE_X, RING_SCALE_Y, RING_SCALE_Z));

  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  ringMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  ringMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(RING_SHININESS));
  ringMaterial->setEffect(aEffect);

  QEntity::setParent(aRootEntity);
  addComponent(torusMesh);
  addComponent(ringTransform);
  addComponent(ringMaterial);
  setEnabled(enabled);

  //animation test
//  QAnimationClip animClip;
//  QAnimationClipData clipData;
//  QChannelMapper channelMapper;
//  QChannelMapping channelMapping;

//  QClipAnimator clipAnimator;
//  clipAnimator.setClip(&animClip);
//  animClip.setClipData(clipData);
//  addComponent(&clipAnimator);


}
