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

#include "propdwellanim.h"
#include "curve.h"


// cstr for self ball
PropDwellAnim::PropDwellAnim(JugglingProp *t_prop,
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
  // translation
  auto translationAnim = selfTranslationAnim();
  addAnimation(translationAnim);
}

// cstr for passing ball
PropDwellAnim::PropDwellAnim(JugglingProp *t_prop,
                             QVector3D t_initialPos,
                             QVector3D t_medPos,
                             QVector3D t_finalPos,
                             int t_launch,
                             QObject *parent)
  : QParallelAnimationGroup(parent),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_medPos(t_medPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch)
{
  // translation
  auto translationAnim = passingTranslationAnim();
  addAnimation(translationAnim);
}

// cstr for self rotating prop
PropDwellAnim::PropDwellAnim(JugglingProp *t_prop,
                             QVector3D t_initialPos,
                             QVector3D t_finalPos,
                             int t_launch,
                             float t_initialRotX,
                             float t_finalRotX,
                             float t_initialRotY,
                             float t_finalRotY,
                             QObject *parent)
  : QParallelAnimationGroup(parent),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch),
    m_initialRotX(t_initialRotX),
    m_finalRotX(t_finalRotX),
    m_initialRotY(t_initialRotY),
    m_finalRotY(t_finalRotY)
{
  // translation
  auto translationAnim = selfTranslationAnim();
  addAnimation(translationAnim);

  // rotation X
  auto rotXAnim = selfClubRotXAnim();
  addAnimation(rotXAnim);

  // rotation Y
  auto rotYAnim = clubRotYAnim();
  addAnimation(rotYAnim);
}

// cstr for passing rotating prop
PropDwellAnim::PropDwellAnim(JugglingProp *t_prop,
                             QVector3D t_initialPos,
                             QVector3D t_medPos,
                             QVector3D t_finalPos,
                             int t_launch,
                             float t_initialRotX,
                             float t_finalRotX,
                             float t_initialRotY,
                             float t_finalRotY,
                             QObject *parent)
  : QParallelAnimationGroup(parent),
    m_prop(t_prop),
    m_initialPos(t_initialPos),
    m_medPos(t_medPos),
    m_finalPos(t_finalPos),
    m_launch(t_launch),
    m_initialRotX(t_initialRotX),
    m_finalRotX(t_finalRotX),
    m_initialRotY(t_initialRotY),
    m_finalRotY(t_finalRotY)
{
  // translation
  auto translationAnim = passingTranslationAnim();
  addAnimation(translationAnim);

  // rotation X
  auto rotXAnim = passingClubRotXAnim();
  addAnimation(rotXAnim);

  // rotation Y
  auto rotYAnim = clubRotYAnim();
  addAnimation(rotYAnim);
}

QSequentialAnimationGroup *PropDwellAnim::passingTranslationAnim()
{
  auto translationAnimGroup = new QSequentialAnimationGroup();

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
  // TODO: verify time
  // loop creating our tranlation anim
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_twoQuarterCircular.at(i));
    translationAnim->setEndValue(v_twoQuarterCircular.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationAnimGroup->addAnimation(translationAnim);
  }
  return translationAnimGroup;
}

QSequentialAnimationGroup *PropDwellAnim::selfTranslationAnim()
{
  // anim to return
  auto translationAnimGroup = new QSequentialAnimationGroup();

  if (m_launch == 1) // dwell is a linear move
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration((int)(DWELL_TIME_LAUNCH1 * S_TO_MS));
    translationAnim->setStartValue(m_initialPos);
    translationAnim->setEndValue(m_finalPos);
    translationAnim->setLoopCount(ONE_LOOP);

    translationAnimGroup->addAnimation(translationAnim);
  }
  else
  {
    // determine number of frames
    int frameCount = (int)((DWELL_TIME / DELTA_TIME));

    // we create our curve
    QVector<QVector3D> v_semiCircular = Curves::curveSemiCircular(m_initialPos,
                                                                  m_finalPos,
                                                                  frameCount);

    // loop creating our tranlation anim
    for (int i = 0; i < frameCount; i++)
    {
      auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
      translationAnim->setDuration(qRound(DELTA_TIME * S_TO_MS));
      translationAnim->setStartValue(v_semiCircular.at(i));
      translationAnim->setEndValue(v_semiCircular.at(i + 1));
      translationAnim->setLoopCount(ONE_LOOP);

      translationAnimGroup->addAnimation(translationAnim);
     }
  }
  return translationAnimGroup;
}

QSequentialAnimationGroup *PropDwellAnim::selfClubRotXAnim()
{
  // create a sequential anim group cos we have 2 property animations
  auto animRotX = new QSequentialAnimationGroup();

  // duration
  int duration = (int)(DWELL_TIME * S_TO_MS / 2);

  // mid angle
  float midAngleRotX;
  if (m_launch == 1)
  {
    midAngleRotX = m_initialRotX + ((m_initialRotX - m_finalRotX) / 2);
    duration = (int)(DWELL_TIME_LAUNCH1 * S_TO_MS / 2);
  }
  else
  {
    midAngleRotX = m_finalRotX + CLUB_DWELL_SELF_X_DECAY;
  }

  // TODO: verify time
  // create first anim to low down angle
  auto animRotX1 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
  animRotX1->setDuration(duration);
  animRotX1->setStartValue(m_initialRotX);
  animRotX1->setEndValue(midAngleRotX);
  animRotX1->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX1);

  // create second anim
  auto animRotX2 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
  animRotX2->setDuration(duration);
  animRotX2->setStartValue(midAngleRotX);
  animRotX2->setEndValue(m_finalRotX);
  animRotX2->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX2);

  return animRotX;
}

QSequentialAnimationGroup *PropDwellAnim::passingClubRotXAnim()
{
  // create a sequential anim group cos we have 2 property animations
  auto animRotX = new QSequentialAnimationGroup();

  // duration
  int duration = (int)(DWELL_TIME * S_TO_MS / 2);

  // mid angle
  float midAngleRotX = m_finalRotX + CLUB_DWELL_PASSING_X_DECAY;

  // TODO: verify time
  // create first anim to low down angle
  auto animRotX1 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
//  animRotX1->setDuration((int)(DWELL_TIME * S_TO_MS / 2));
  animRotX1->setDuration(duration);
  animRotX1->setStartValue(m_initialRotX);
  animRotX1->setEndValue(midAngleRotX);
  animRotX1->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX1);

  // create second anim
  auto animRotX2 = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"));
//  animRotX2->setDuration((int)(DWELL_TIME * S_TO_MS / 2));
  animRotX2->setDuration(duration);
  animRotX2->setStartValue(midAngleRotX);
  animRotX2->setEndValue(m_finalRotX);
  animRotX2->setLoopCount(ONE_LOOP);
  // add to sequential anim group
  animRotX->addAnimation(animRotX2);

  return animRotX;
}

QPropertyAnimation *PropDwellAnim::clubRotYAnim()
{
  // duration
  int duration = (int)(DWELL_TIME * S_TO_MS);
  if (m_launch == 1)
  {
    duration = (int)(DWELL_TIME_LAUNCH1 * S_TO_MS);
  }

  // set anim rot (y)
  auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"));
  animRotY->setDuration(duration);
  animRotY->setStartValue(m_initialRotY);
  animRotY->setEndValue(m_finalRotY);
  animRotY->setLoopCount(ONE_LOOP);
  return animRotY;
}

