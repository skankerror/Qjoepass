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

#ifndef HANDANIM_H
#define HANDANIM_H

#include <QSequentialAnimationGroup>
#include "juggler.h"
#include "jugglingprop.h"
#include "siteswap.h"

class HandAnim : public QSequentialAnimationGroup
{

  Q_OBJECT

public:

  explicit HandAnim(QVector<Juggler *> t_v_juggler,
                    int t_jugglerId,
                    hand t_hand,
                    QVector<JugglingProp *> t_v_prop,
                    bool t_isExtCatchPlus,
                    QObject *parent = nullptr);

  void setAnim(QVector<handAnimEvent *>* t_v_handAnimEvents);

private:

  QParallelAnimationGroup *dwellAnim(int t_propId,
                                     int t_launch,
                                     int t_jugglerReceiveId,
                                     hand t_receiveHand);

  QSequentialAnimationGroup *emptyHandAnim(QVector3D t_initialPos,
                                           QVector3D t_finalPos,
                                           int t_duration);

  QVector3D getExtHandPos(const Juggler *t_juggler,
                          const hand t_side) const;

  QVector3D getIntHandPos(const Juggler *t_juggler,
                          const hand t_side) const;

  QVector3D setMedPosForPassingDwell(QVector3D t_finalPos,
                                     Juggler *t_jugglerReceive,
                                     hand t_receiveHand);

private:

  QVector<Juggler *> m_v_juggler; // To know where to launch for passing
  Juggler *m_juggler; // juggler earning the hand
  hand m_hand; // hand to move
  QVector<JugglingProp *> m_v_prop; // all the props created for the anim

  // bool to know if we need to enlarge juggling
  bool m_isExtPlusCatch;


};

#endif // HANDANIM_H
