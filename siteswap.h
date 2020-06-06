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

class SiteSwap : public QObject
{
  Q_OBJECT
public:
  explicit SiteSwap(QVector<int> &aVInt,
                    bool aSynchron = false,
                    QObject *parent = nullptr);

  bool isValid() const;
  int getNumProp() const;
  int getPeriod() const {return period;};
  int at(int i) const {return v_event.at(i);};

signals:

private:
  QVector<int> v_event; // without multiplex
  QVector<QVector<int>> v_v_Event; // with multiplex
  int period;
  bool valid = false;
  bool synchron = false;
};

#endif // SITESWAP_H
