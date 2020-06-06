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
    QLabel *periodLabel;
    QSpinBox *periodSpinBox;
      QSpinBox *firstSiteSpinBox;
      QVector<QSpinBox *> vSpinBox;
    QPushButton *launchPushButton;

};
#endif // MYMAINWINDOW_H
