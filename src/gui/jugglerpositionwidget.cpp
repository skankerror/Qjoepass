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
  jugglerPixmap.load(JUGGLER_PIXMAP);
  backgroundPixmap.load(GROUND_PIXMAP);
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

void JugglerPositionWidget::jugglerCountChanged(QVector<Juggler *> t_v_juggler)
{
  m_v_juggler = t_v_juggler;
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
                     backgroundPixmap);

  // paint jugglers
  for (int i = 0; i < m_v_juggler.size(); i++)
  {
    auto juggler = m_v_juggler.at(i);

    // set position in widget
    int x = ((juggler->getPosition().x() + GROUND_SIZE / 2)
             * (JUGGLER_POSITION_WIDGET_SIZE / GROUND_SIZE)
             - JUGGLER_PIXMAP_SIZE/2);

    int y = ((juggler->getPosition().z() + GROUND_SIZE / 2)
             * (JUGGLER_POSITION_WIDGET_SIZE / GROUND_SIZE)
             - JUGGLER_PIXMAP_SIZE/2);

    float angle = juggler->getRotY();


    painter.drawPixmap(x,
                       y,
                       JUGGLER_PIXMAP_SIZE,
                       JUGGLER_PIXMAP_SIZE,
                       jugglerPixmap);

  }
  painter.restore();
}
