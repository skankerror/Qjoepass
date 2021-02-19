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

#include "jugglerpositionwidget.h"
#include "qjoepass.h"
#include <QPainter>

JugglerPositionWidget::JugglerPositionWidget(QWidget *parent)
  : QWidget(parent)
{
  m_backgroundPixmap.load(GROUND_PIXMAP);
}


void JugglerPositionWidget::mousePressEvent(QMouseEvent *event)
{
}

void JugglerPositionWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void JugglerPositionWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void JugglerPositionWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void JugglerPositionWidget::jugglerCountChanged(QVector<QVector3D> t_v_jugPosAndAngles)
{
  m_v_jugPosAndAngle = t_v_jugPosAndAngles;
  update();
}

QSize JugglerPositionWidget::sizeHint() const
{
  return QSize(JUGGLER_POSITION_WIDGET_SIZE, JUGGLER_POSITION_WIDGET_SIZE);
}

QSize JugglerPositionWidget::minimumSizeHint() const
{
  return QSize(JUGGLER_POSITION_WIDGET_SIZE, JUGGLER_POSITION_WIDGET_SIZE);
}

void JugglerPositionWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)

  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.save();

  // paint the ground
  painter.drawPixmap(0,
                     0,
                     JUGGLER_POSITION_WIDGET_SIZE,
                     JUGGLER_POSITION_WIDGET_SIZE,
                     m_backgroundPixmap);

  // paint jugglers
  for (int i = 0; i < m_v_jugPosAndAngle.size(); i++)
  {
    auto jugPosAndAngle = m_v_jugPosAndAngle.at(i);

    // set position in widget
    int x = ((jugPosAndAngle.x() + GROUND_SIZE / 2)
             * (JUGGLER_POSITION_WIDGET_SIZE / GROUND_SIZE)
             - JUGGLER_PIXMAP_SIZE/2);

    int y = ((jugPosAndAngle.y() + GROUND_SIZE / 2)
             * (JUGGLER_POSITION_WIDGET_SIZE / GROUND_SIZE)
             - JUGGLER_PIXMAP_SIZE/2);

    float angle = jugPosAndAngle.z();
    int pixmapSize;
//    (angle == 0 || angle == 180) ?
//          pixmapSize = JUGGLER_PIXMAP_SIZE / 2:
        pixmapSize = JUGGLER_PIXMAP_SIZE;

    qDebug() << " angle envoyé au widget : " << angle;

    QPixmap jugglerPixmap;
    jugglerPixmap.load(JUGGLER_PIXMAP);
    auto jugglerPixmapTransform = QTransform();
    jugglerPixmapTransform.rotate(-angle);
    auto pixmap = jugglerPixmap.transformed(jugglerPixmapTransform);
    painter.drawPixmap(x,
                       y,
                       pixmapSize,
                       pixmapSize,
                       pixmap);
  }
  painter.restore();
}
