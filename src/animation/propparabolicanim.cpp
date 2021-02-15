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

#include "propparabolicanim.h"
#include "curve.h"


PropParabolicAnim::PropParabolicAnim(JugglingProp *t_prop,
                                     QVector3D t_initialPos,
                                     QVector3D t_finalPos,
                                     int t_launch,
                                     QObject *parent)
  : QParallelAnimationGroup(parent),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch)
{
  m_arcTime = getArcTime();

  // for further time adjustment
  m_duration = qRound(m_arcTime * S_TO_MS);

  auto translatAnim = translationAnim();

  // Time adjustment if necessary
  int intDecay = m_duration - translatAnim->duration();
  if (intDecay)
    translatAnim->addPause(intDecay);
//  qDebug() << "PropAnim::parabolicAnim() duration" << translatAnim->duration();

  addAnimation(translatAnim);
}

PropParabolicAnim::PropParabolicAnim(JugglingProp *t_prop,
                                     QVector3D t_initialPos,
                                     QVector3D t_finalPos,
                                     int t_launch,
                                     float t_initialRotX,
                                     float t_finalRotX,
                                     QObject *parent)
  : QParallelAnimationGroup(parent),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch),
    m_initialRotX(t_initialRotX),
    m_finalRotX(t_finalRotX)
{
  m_arcTime = getArcTime();

  // for further time adjustment
  m_duration = qRound(m_arcTime * S_TO_MS);

  auto translatAnim = translationAnim();

  // Time adjustment if necessary
  int intDecay = m_duration - translatAnim->duration();
//  qDebug() << "decay in translaton" << intDecay;
  if (intDecay)
    translatAnim->addPause(intDecay);
//  qDebug() << "PropAnim::parabolicAnim() duration" << translatAnim->duration();

  addAnimation(translatAnim);

  auto animRotX = rotXAnim();

  addAnimation(animRotX);

}

QSequentialAnimationGroup *PropParabolicAnim::translationAnim()
{
  // we calculate velocity launch
  auto velBall = ((m_finalPos - m_initialPos) - 0.5 *
                  (GRAVITY * qPow(m_arcTime, 2))) / m_arcTime;

  int frameCount = (int)((m_arcTime / (DELTA_TIME)));

  // We create our curve
  auto v_parabolic = Curves::curveParabolic(velBall,
                                            m_initialPos,
                                            frameCount);

  // create sequential for translation
  auto translationAnimGroup = new QSequentialAnimationGroup();

  // this loop creates all animations for translation
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration(qRound(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_parabolic.at(i));
    translationAnim->setEndValue(v_parabolic.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationAnimGroup->addAnimation(translationAnim);
  }
  return translationAnimGroup;
}

QPropertyAnimation *PropParabolicAnim::rotXAnim()
{
  auto animRotX = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
  animRotX->setDuration(m_duration);
  animRotX->setStartValue(m_initialRotX);
  animRotX->setEndValue(m_finalRotX);
  animRotX->setLoopCount(ONE_LOOP);

  return animRotX;
}

QSequentialAnimationGroup *PropParabolicAnim::rotYAnim()
{

}

float PropParabolicAnim::getArcTime() const
{
  float arcTime;

  if (m_launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else // Shannon Theorem
    arcTime = ((HAND_PERIOD) / 2) * (m_launch - (2 * DWELL_RATIO));

  return arcTime;
}
