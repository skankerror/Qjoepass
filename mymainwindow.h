#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

#include "settings.h"
#include "animationview.h"
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

private:
  void createMenus();
  void createStatusBar();
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

  animationView *anim;
  Preferences *pref;

  QList<QKeySequence> prefShortcut;

  QWidget *container;
  My3DWindow *my3DWindow;

};
#endif // MYMAINWINDOW_H
