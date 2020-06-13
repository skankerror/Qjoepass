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

#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "settings.h"
#include "preferences.h"
#include "my3dwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MyMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MyMainWindow();
  void loadFile(const QString &fileName);

  static QString getPropToString(const int prop);
  static jugglingProp getPropFromString(const QString &value);

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void about();
  void documentWasModified();
  void preferencesDial();
  void launchSiteSwap();
  void periodChanged(int i);
  void updateCameraComboBox();
  void cameraIndexChanged(int index);
  void propTypeChanged(int index);

private:
  void createMenus();
  void createStatusBar();
  void createToolBar();
  bool maybeSave();
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullFileName);

  QString curFile;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *helpMenu;
  QAction *newAct;
  QAction *openAct;
  QAction *saveAct;
  QAction *saveAsAct;
  QAction *exitAct;
  QAction *prefAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QLabel *infoLabel;

  MySettings *settings;

  Preferences *pref;

  QList<QKeySequence> prefShortcut;

  QWidget *container;
  My3DWindow *my3DWindow;

  QToolBar *myToolBar;
    QHBoxLayout *toolBarLayout;
    QLabel *propLabel;
      QComboBox *propTypeComboBox;
    QLabel *launchPropTypeLabel;
      QComboBox *launchBallTypeComboBox;
      QComboBox *launchRingTypeComboBox;
      QComboBox *launchClubTypeComboBox;
    QLabel *periodLabel;
      QSpinBox *periodSpinBox;
        QSpinBox *firstSiteSpinBox;
        QVector<QSpinBox *> vSpinBox;
    QPushButton *launchPushButton;

    QHBoxLayout *toolBar2Layout;
    QLabel *cameraLabel;
    QComboBox *cameraComboBox;

};
#endif // MYMAINWINDOW_H
