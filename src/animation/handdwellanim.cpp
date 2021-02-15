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

#include "handdwellanim.h"
#include "curve.h"

HandDwellAnim::HandDwellAnim(Juggler *t_juggler,
                             hand t_hand,
                             JugglingProp *t_prop,
                             QVector3D t_initialPos,
                             QVector3D t_finalPos,
                             int t_launch,
                             QObject *parent)
  : QParallelAnimationGroup(parent),
    m_juggler(t_juggler),
    m_hand(t_hand),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch)
{
  (m_hand == leftHand) ?
        m_propertyName = QByteArrayLiteral("m_leftHandPosition"):
      m_propertyName = QByteArrayLiteral("m_rightHandPosition");

  selfAnim();
}

HandDwellAnim::HandDwellAnim(Juggler *t_juggler,
                             hand t_hand,
                             JugglingProp *t_prop,
                             QVector3D t_initialPos,
                             QVector3D t_medPos,
                             QVector3D t_finalPos,
                             int t_launch,
                             QObject *parent)
  : QParallelAnimationGroup(parent),
    m_juggler(t_juggler),
    m_hand(t_hand),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_medPos(t_medPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch)
{
  (m_hand == leftHand) ?
        m_propertyName = QByteArrayLiteral("m_leftHandPosition"):
      m_propertyName = QByteArrayLiteral("m_rightHandPosition");

  passingAnim();
}

void HandDwellAnim::passingAnim()
{
//  auto translationAnimGroup = new QSequentialAnimationGroup();

  // determine number of frames
  int frameCount = (int)((DWELL_TIME / DELTA_TIME));
  // if it's a passing launch 1 ? NOTE: is it realistic
  if (m_launch == 1)
  {
    frameCount = (int)(DWELL_TIME_LAUNCH1 / DELTA_TIME);
  }

  // faire anim arc de cercle jusqu'à ce point
  // puis jusqu'au point de lancement
  auto v_twoQuarterCircular = Curves::curveTwoQuarterCircular(m_initialPos,
                                                              m_medPos,
                                                              m_finalPos,
                                                              frameCount);

//  auto translationPropGroup = new QSequentialAnimationGroup();

//  // TODO: verify time
//  // loop creating our tranlation anim
//  for (int i = 0; i < frameCount; i++)
//  {
//    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
//    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
//    translationAnim->setStartValue(v_twoQuarterCircular.at(i));
//    translationAnim->setEndValue(v_twoQuarterCircular.at(i + 1));
//    translationAnim->setLoopCount(ONE_LOOP);
//    translationPropGroup->addAnimation(translationAnim);
//  }
//  addAnimation(translationPropGroup);

  auto translationHandGroup = new QSequentialAnimationGroup();

  // TODO: verify time
  // loop creating our tranlation anim
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_juggler, m_propertyName);
    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_twoQuarterCircular.at(i));
    translationAnim->setEndValue(v_twoQuarterCircular.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationHandGroup->addAnimation(translationAnim);
  }
  addAnimation(translationHandGroup);
}

void HandDwellAnim::selfAnim()
{
//  auto translationPropGroup = new QSequentialAnimationGroup();


  if (m_launch == 1) // dwell is a linear move
  {
//    auto propTranslation = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
//    propTranslation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
//    propTranslation->setStartValue(m_initialPos);
//    propTranslation->setEndValue(m_finalPos);
//    propTranslation->setLoopCount(ONE_LOOP);

//    addAnimation(propTranslation);

    auto handTranslation = new QPropertyAnimation(m_juggler, m_propertyName);
    handTranslation->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
    handTranslation->setStartValue(m_initialPos);
    handTranslation->setEndValue(m_finalPos);
    handTranslation->setLoopCount(ONE_LOOP);

    addAnimation(handTranslation);
  }
  else
  {
    // determine number of frames
    int frameCount = (int)((DWELL_TIME / DELTA_TIME));

    // we create our curve
    QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(m_initialPos,
                                                                  m_finalPos,
                                                                  frameCount);

//    auto translationPropGroup = new QSequentialAnimationGroup();
//    // loop creating our tranlation anim
//    for (int i = 0; i < frameCount; i++)
//    {
//      auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
//      translationAnim->setDuration(qRound(DELTA_TIME * S_TO_MS));
//      translationAnim->setStartValue(v_semiCircular.at(i));
//      translationAnim->setEndValue(v_semiCircular.at(i + 1));
//      translationAnim->setLoopCount(ONE_LOOP);

//      translationPropGroup->addAnimation(translationAnim);
//     }
//    addAnimation(translationPropGroup);

    auto translationHandGroup = new QSequentialAnimationGroup();
    // loop creating our tranlation anim
    for (int i = 0; i < frameCount; i++)
    {
      auto translationAnim = new QPropertyAnimation(m_juggler, m_propertyName);
      translationAnim->setDuration(qRound(DELTA_TIME * S_TO_MS));
      translationAnim->setStartValue(v_semiCircular.at(i));
      translationAnim->setEndValue(v_semiCircular.at(i + 1));
      translationAnim->setLoopCount(ONE_LOOP);

      translationHandGroup->addAnimation(translationAnim);
     }
    addAnimation(translationHandGroup);

  }

}
