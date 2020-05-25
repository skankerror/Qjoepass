#include <QtWidgets>
#include "mymainwindow.h"
//#include "settings.h"

MyMainWindow::MyMainWindow()
{
  QStringList cmdline_args = QCoreApplication::arguments();
//    QFile file(cmdline_args[0]);
  settings = new MySettings() ;

  QWidget *widget = new QWidget;
  setCentralWidget(widget);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
  infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  infoLabel->setAlignment(Qt::AlignCenter);

  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//  animationView *anim = new animationView();
  my3DWindow = new My3DWindow();
  container = QWidget::createWindowContainer(my3DWindow);
  QSize screenSize = my3DWindow->screen()->size();
  container->setMinimumSize(QSize(200, 100));
  container->setMaximumSize(screenSize);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(1, 1, 1, 1);
  layout->addWidget(topFiller);
  layout->addWidget(infoLabel);
  layout->addWidget(container);
  layout->addWidget(bottomFiller);
  widget->setLayout(layout);

  createMenus();

  QString message = tr("A context menu is available by right-clicking");
  statusBar()->showMessage(message);

  setWindowTitle(tr("joePass"));
  setMinimumSize(160, 160);

  resize(480, 320);

  pref = new Preferences(settings);

  connect(pref, SIGNAL(colorChanged(QColor)), my3DWindow, SLOT(changeBackground(QColor)));
}

void MyMainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));

//    QToolBar *fileToolBar = addToolBar(tr("File"));
  const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
  newAct = new QAction(newIcon, tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, &QAction::triggered, this, &MyMainWindow::newFile);
  fileMenu->addAction(newAct);
//    fileToolBar->addAction(newAct);

  const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
  openAct = new QAction(openIcon, tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, &QAction::triggered, this, &MyMainWindow::open);
  fileMenu->addAction(openAct);
//    fileToolBar->addAction(openAct);

  const QIcon saveIcon = QIcon::fromTheme("document-open", QIcon(":/images/save.png"));
  saveAct = new QAction(saveIcon, tr("&Save..."), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save to a file"));
  connect(saveAct, &QAction::triggered, this, &MyMainWindow::save);
  fileMenu->addAction(saveAct);
//    fileToolBar->addAction(saveAct);


  const QIcon saveAsIcon = QIcon::fromTheme("document-open", QIcon(":/images/saveAs.png"));
  saveAsAct = new QAction(saveAsIcon, tr("&SaveAs..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save as a new file"));
  connect(saveAsAct, &QAction::triggered, this, &MyMainWindow::saveAs);
  fileMenu->addAction(saveAsAct);
//    fileToolBar->addAction(saveAsAct);

//    const QIcon exitIcon = QIcon::fromTheme("exit", QIcon(":/images/exit.png"));
  exitAct = new QAction(tr("&Quit..."), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Quit joePass"));
  connect(exitAct, &QAction::triggered, this, &QWidget::close);
  fileMenu->addAction(exitAct);
   //    fileToolBar->addAction(saveAsAct);


  editMenu = menuBar()->addMenu(tr("&Edit"));
    //    QToolBar *editToolBar = addToolBar(tr("Edit"));

  prefAct = new QAction( tr("&Preferences..."), this);

  prefShortcut << QKeySequence("Ctrl+P");
  prefAct->setShortcuts(prefShortcut);
  prefAct->setStatusTip(tr("Open preferences dialog"));
  connect(prefAct, &QAction::triggered, this, &MyMainWindow::preferencesDial);
  editMenu->addAction(prefAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));

  const QIcon aboutIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  aboutAct = helpMenu->addAction(aboutIcon, tr("&About..."), this,  &MyMainWindow::about);
  aboutAct->setStatusTip(tr("Show the QjoePass's About box"));
  connect(aboutAct, &QAction::triggered, this, &MyMainWindow::about);
  helpMenu->addAction(aboutAct);

  const QIcon aboutQtIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  aboutQtAct = helpMenu->addAction(aboutIcon, tr("About &Qt"), qApp, &QApplication::aboutQt);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}
void MyMainWindow::preferencesDial()
{
//  pref = new Preferences(settings);
  pref->setWindowTitle("Preferences");
  pref->show();
}

void MyMainWindow::newFile()
{
  if (maybeSave()) {
        setCurrentFile(QString());
    }
}

void MyMainWindow::open()
{
  if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MyMainWindow::save()
{
  if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MyMainWindow::saveAs()
{
  QFileDialog dialog(this);
  dialog.setWindowModality(Qt::WindowModal);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  if (dialog.exec() != QDialog::Accepted)
    return false;
  return saveFile(dialog.selectedFiles().first());
}

void MyMainWindow::about()
{
  QMessageBox::about(this, tr("About Application"),
    tr("The <b>Application</b> example demonstrates how to "
      "write modern GUI applications using Qt, with a menu bar, "
      "toolbars, and a status bar."));
}

void MyMainWindow::loadFile(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
    return;
  }

  QTextStream in(&file);
#ifndef QT_NO_CURSOR
  QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
//    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
  QGuiApplication::restoreOverrideCursor();
#endif

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MyMainWindow::saveFile(const QString &fileName)
{
  QString errorMessage;

  QGuiApplication::setOverrideCursor(Qt::WaitCursor);
  QSaveFile file(fileName);
  if (file.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream out(&file);
//        out << textEdit->toPlainText();
    if (!file.commit()) {
      errorMessage = tr("Cannot write file %1:\n%2.")
        .arg(QDir::toNativeSeparators(fileName), file.errorString());
     }
  } else {
    errorMessage = tr("Cannot open file %1 for writing:\n%2.")
      .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
  QGuiApplication::restoreOverrideCursor();

  if (!errorMessage.isEmpty()) {
    QMessageBox::warning(this, tr("Application"), errorMessage);
    return false;
  }

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void MyMainWindow::setCurrentFile(const QString &fileName)
{
  curFile = fileName;
//    textEdit->document()->setModified(false);
  setWindowModified(false);

  QString shownName = curFile;
  if (curFile.isEmpty())
      shownName = "untitled.txt";
  setWindowFilePath(shownName);
}

void MyMainWindow::documentWasModified()
{
//    setWindowModified(textEdit->document()->isModified());
}

void MyMainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
//        writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

bool MyMainWindow::maybeSave()
{
//    if (!textEdit->document()->isModified())
//        return true;
  const QMessageBox::StandardButton ret
    = QMessageBox::warning(this, tr("Application"),
              tr("The document has been modified.\n"
                 "Do you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  switch (ret) {
  case QMessageBox::Save:
    return save();
  case QMessageBox::Cancel:
    return false;
  default:
    break;
  }
  return true;
}

