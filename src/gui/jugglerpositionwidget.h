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

#ifndef JUGGLERPOSITIONWIDGET_H
#define JUGGLERPOSITIONWIDGET_H

#include <QWidget>
#include "juggler.h"

class JugglerPositionWidget : public QWidget
{

  Q_OBJECT

public:

  explicit JugglerPositionWidget(QWidget *parent = nullptr);

//  QPaintEngine *paintEngine() const override;
  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

protected:

  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

signals:

public slots:

  void jugglerCountChanged(QVector<Juggler *> t_v_juggler);

private:

  QPixmap jugglerPixmap;
  QPixmap backgroundPixmap;

  QVector<Juggler *> m_v_juggler;
};

#endif // JUGGLERPOSITIONWIDGET_H
