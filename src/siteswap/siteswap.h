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

#ifndef SITESWAP_H
#define SITESWAP_H

#include <QObject>
#include <QBitArray>
#include "qjoepass.h"
#include "animevent.h"
#include "siteswapevent.h"

class SiteSwap : public QObject
{
  Q_OBJECT

public:
  explicit SiteSwap(QVector<SiteswapEvent *> &aVInt,
                    int aJugCount,
                    jugglingProp aProp = ball,
                    bool aSynchron = false,
                    QObject *parent = nullptr);

  //  int at(int i) const {return v_event.at(i);};
  int at(int i) const { return v_event.at(i)->getLaunch(); }

  // getters
  bool isValid() const;
  int getNumProp() const;
  int getPeriod() const {return period;};
  jugglingProp getPropType() const {return prop;};
  int getLaunchType() const {return launchType;};
  QBitArray getState() {return state;};
  int getJugglerCount() const { return jugglerCount; };
  // For sending datas to animation
  // ici faire getPropAnim avec tout le trajet de toutes les balles
  // un QVector<QSequentialAnimationGroup*> suffira
  QVector<AnimEvent*> getAnimEvents(int launchPos, hand handLaunch, int jugLaunchId);

  // setters
  void setPropType(jugglingProp aProp);
  void setLaunchType(int aLaunchType) {launchType = aLaunchType;};


private:
  void setState();
  hand changeHand(hand aHand){return (aHand == leftHand) ? rightHand : leftHand;};

signals:

private:
  QVector<SiteswapEvent*> v_event;
  int period;
  bool valid = false;
  bool synchron = false;
  jugglingProp prop;
  int launchType;
  QBitArray state;
  int propCount;
  int jugglerCount;

};

#endif // SITESWAP_H
