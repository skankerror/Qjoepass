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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "settings.h"
#include "preferences.h"
#include "3dwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

  Q_OBJECT

public:

  MainWindow();
  void loadFile(const QString &t_fileName);

  static QString getPropToString(const int t_prop);
  static jugglingProp getPropFromString(const QString &t_value);

private:

  void createMenus();
  void createStatusBar();
  void createToolBar();
  bool maybeSave();
  bool saveFile(const QString &t_fileName);
  void setCurrentFile(const QString &t_fileName);
//  QString strippedName(const QString &t_fullFileName);

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
  void periodChanged(int t_index);
  void updateCameraComboBox();
  void cameraIndexChanged(int t_index);
  void propTypeChanged(int t_index);

private:

  QString m_curFile;

  QMenu *m_fileMenu;
  QMenu *m_editMenu;
  QMenu *m_helpMenu;
  QAction *m_newAct;
  QAction *m_openAct;
  QAction *m_saveAct;
  QAction *m_saveAsAct;
  QAction *m_exitAct;
  QAction *m_prefAct;
  QAction *m_aboutAct;
  QAction *m_aboutQtAct;
  QLabel *m_infoLabel;

  MySettings *m_settings;

  Preferences *m_pref;

  QList<QKeySequence> m_l_prefShortcut;

  QWidget *m_container;
  My3DWindow *m_my3DWindow;

  QToolBar *m_myToolBar;
    QHBoxLayout *m_toolBarLayout;

    QLabel *m_propLabel;
      QComboBox *m_propTypeComboBox;

    QLabel *m_launchPropTypeLabel;
      QComboBox *m_launchBallTypeComboBox;
      QComboBox *m_launchRingTypeComboBox;
      QComboBox *m_launchClubTypeComboBox;

    QLabel *m_periodLabel;
      QSpinBox *m_periodSpinBox;
        QWidget *m_spinBoxContainer; // Try to fix tabbing order
        QHBoxLayout *m_spinBoxLayout; // id
        // trouver un vrai moyen de séparer les spinbox du reste
        QSpinBox *m_firstSiteSpinBox;
        QVector<QSpinBox *> m_v_spinBox;
    QPushButton *m_launchPushButton;

    QHBoxLayout *m_toolBar2Layout;
    QLabel *m_cameraLabel;
    QComboBox *m_cameraComboBox;

};
#endif // MAINWINDOW_H
