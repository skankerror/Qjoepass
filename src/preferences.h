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

#ifndef PREFERENCES_H
#define PREFERENCES_H


#include <QDialog>
#include "settings.h"
#include <QColor>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

class Preferences: public QDialog
{
  Q_OBJECT
public:
  explicit Preferences(MySettings *aSettings,QWidget *parent = nullptr);

private slots:
  void backgroundColor();
  void groundColor();

signals:
  void colorChanged(QColor);
  void groundColorChanged(QColor);

private:
  QTabWidget *tabWidget;

  QWidget *worldTab;

  QDialogButtonBox *buttonBox;
  MySettings *settings;

  QWidget *casualTab;
  QWidget *soundTab;
};

#endif // PREFERENCES_H
