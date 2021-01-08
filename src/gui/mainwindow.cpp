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

#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow()
{
  QIcon icon = QIcon(APP_ICON);
  setWindowIcon(icon);

  QStringList cmdline_args = QCoreApplication::arguments();
  //    QFile file(cmdline_args[0]);
  settings = new MySettings() ;

  auto widget = new QWidget;
  setCentralWidget(widget);

  auto topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                            "invoke a context menu</i>"));
  infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  infoLabel->setAlignment(Qt::AlignCenter);

  auto bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  my3DWindow = new My3DWindow(settings);
  container = QWidget::createWindowContainer(my3DWindow);
  QSize screenSize = my3DWindow->screen()->size();
  container->setMinimumSize(QSize(WIDGET3D_MIN_W, WIDGET3D_MIN_H));
  container->setMaximumSize(screenSize);

  auto layout = new QVBoxLayout;
  //  layout->setContentsMargins(1, 1, 1, 1);
  //  layout->addWidget(topFiller);
  //  layout->addWidget(infoLabel);
  layout->addWidget(container);
  //  layout->addWidget(bottomFiller);
  widget->setLayout(layout);

  createMenus();
  createToolBar();

  QString message = tr("A context menu is available by right-clicking");
  statusBar()->showMessage(message);

  setWindowTitle(tr("joePass"));
  setMinimumSize(WINDOW_MINIMUM_H, WINDOW_MINIMUM_H);

  resize(WINDOW_W, WINDOW_H);

  pref = new Preferences(settings);

  connect(pref, SIGNAL(colorChanged(QColor)), my3DWindow, SLOT(changeBackground(QColor)));
  connect(pref, SIGNAL(groundColorChanged(QColor)), my3DWindow, SLOT(changeGroundColor(QColor)));

}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));

  //    QToolBar *fileToolBar = addToolBar(tr("File"));
  const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
  newAct = new QAction(newIcon, tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
  fileMenu->addAction(newAct);
  //    fileToolBar->addAction(newAct);

  const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
  openAct = new QAction(openIcon, tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, &QAction::triggered, this, &MainWindow::open);
  fileMenu->addAction(openAct);
  //    fileToolBar->addAction(openAct);

  const QIcon saveIcon = QIcon::fromTheme("document-open", QIcon(":/images/save.png"));
  saveAct = new QAction(saveIcon, tr("&Save..."), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save to a file"));
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);
  fileMenu->addAction(saveAct);
  //    fileToolBar->addAction(saveAct);

  const QIcon saveAsIcon = QIcon::fromTheme("document-open", QIcon(":/images/saveAs.png"));
  saveAsAct = new QAction(saveAsIcon, tr("&SaveAs..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save as a new file"));
  connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
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
  connect(prefAct, &QAction::triggered, this, &MainWindow::preferencesDial);
  editMenu->addAction(prefAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));

  const QIcon aboutIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  aboutAct = helpMenu->addAction(aboutIcon, tr("&About..."), this,  &MainWindow::about);
  aboutAct->setStatusTip(tr("Show the QjoePass's About box"));
  connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
  helpMenu->addAction(aboutAct);

  const QIcon aboutQtIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  aboutQtAct = helpMenu->addAction(aboutIcon, tr("About &Qt"), qApp, &QApplication::aboutQt);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createToolBar()
{
  myToolBar = addToolBar("siteswap bar");

  auto myToolBarWidget = new QWidget(this);
  toolBarLayout = new QHBoxLayout();
  propLabel = new QLabel("type of prop", this);
  propTypeComboBox = new QComboBox(this);
  for (int i = 0; i < propNumb; i++)
    propTypeComboBox->addItem(getPropToString(i));
  launchPropTypeLabel = new QLabel("type of Launch", this);
  launchBallTypeComboBox = new QComboBox(this);
    launchBallTypeComboBox->addItem("normal", normalBall);
    launchBallTypeComboBox->addItem("bounce", bounce);
  launchRingTypeComboBox = new QComboBox(this);
    launchRingTypeComboBox->addItem("normal", normalRing);
    launchRingTypeComboBox->addItem("pancake", panCake);
  launchClubTypeComboBox = new QComboBox(this);
    launchClubTypeComboBox->addItem("normal", normalClub);
    launchClubTypeComboBox->addItem("flat", flat);
    launchClubTypeComboBox->addItem("helicopter", helicopter);
  propTypeComboBox->setCurrentIndex(ball);
  periodLabel = new QLabel("period", this);
  periodSpinBox = new QSpinBox(this);
    periodSpinBox->setMinimum(1);
    periodSpinBox->setValue(1);
  firstSiteSpinBox = new QSpinBox(this);
    firstSiteSpinBox->setMinimum(1);
    firstSiteSpinBox->setValue(3);
  launchPushButton = new QPushButton("launch !", this);
    launchPushButton->setToolTip("Start animation");
    launchPushButton->setToolTipDuration(2000);
  toolBarLayout->addWidget(propLabel);
  toolBarLayout->addWidget(propTypeComboBox);
  toolBarLayout->addWidget(launchPropTypeLabel);
  toolBarLayout->addWidget(launchBallTypeComboBox);
  toolBarLayout->addWidget(launchRingTypeComboBox);
    launchRingTypeComboBox->hide();
  toolBarLayout->addWidget(launchClubTypeComboBox);
    launchClubTypeComboBox->hide();
  toolBarLayout->addWidget(periodLabel);
  toolBarLayout->addWidget(periodSpinBox);
  toolBarLayout->addWidget(firstSiteSpinBox);
  toolBarLayout->addWidget(launchPushButton, 0, Qt::AlignRight);
  myToolBarWidget->setLayout(toolBarLayout);
  myToolBar->addWidget(myToolBarWidget);

  auto MyToolBarWidget2 = new QWidget(this);
  toolBar2Layout = new QHBoxLayout();
  cameraLabel = new QLabel("change Camera", this);
  cameraComboBox = new QComboBox(this);
  updateCameraComboBox();
  toolBar2Layout->addWidget(cameraLabel);
  toolBar2Layout->addWidget(cameraComboBox);

  MyToolBarWidget2->setLayout(toolBar2Layout);
  myToolBar->addWidget(MyToolBarWidget2);

  connect(periodSpinBox, SIGNAL(valueChanged(int)), this, SLOT(periodChanged(int)));
  connect(launchPushButton, SIGNAL(clicked()), this, SLOT(launchSiteSwap()));
  connect(my3DWindow, SIGNAL(jugglerCountChanged()), this, SLOT(updateCameraComboBox()));
  connect(cameraComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cameraIndexChanged(int)));
  connect(propTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(propTypeChanged(int)));

}
void MainWindow::preferencesDial()
{
  pref->setWindowTitle("Preferences");
  pref->show();
}

void MainWindow::launchSiteSwap()
{
  QVector<SiteswapEvent*> vecEvent;
  int launch = firstSiteSpinBox->value();
  auto event = new SiteswapEvent(launch);
  // paramétrer les autres arguments ici
  vecEvent.append(event);
  for (int i = 0; i < vSpinBox.size(); i++)
  {
    launch = vSpinBox.at(i)->value();
    auto newEvent = new SiteswapEvent(launch);
    // pareil
    vecEvent.append(newEvent);
  }
  jugglingProp prop = getPropFromString(propTypeComboBox->currentText());
  int launchType = 0; // normalLaunch
  switch(prop)
  {
  case ball: launchType = launchBallTypeComboBox->currentIndex(); break;
  case ring: launchType = launchRingTypeComboBox->currentIndex(); break;
  case club: launchType = launchClubTypeComboBox->currentIndex(); break;
  default: break;
  }

  // en attendant mieux
  int jugglerCount = 1;
  my3DWindow->createSiteSwap(vecEvent, jugglerCount, prop, launchType, false);
}

void MainWindow::periodChanged(int i)
{
  int numSpinBox = vSpinBox.size();
  if (numSpinBox)
  {
    for (int j = 0; j < numSpinBox; j++)
    {
      auto spinBox = vSpinBox.at(j);
      spinBox->hide();
      toolBarLayout->removeWidget(spinBox);
    }
    vSpinBox.clear();
    vSpinBox.squeeze();
  }
  if (i > 1)
  {
    toolBarLayout->removeWidget(launchPushButton);
    for (int k = 1; k < i; k++)
    {
      auto newSpinBox = new QSpinBox(this);
      newSpinBox->setMinimum(0);
      vSpinBox.append(newSpinBox);
      toolBarLayout->addWidget(newSpinBox);
    }
    toolBarLayout->addWidget(launchPushButton);
  }
}

void MainWindow::updateCameraComboBox()
{
  cameraComboBox->clear();
  cameraComboBox->addItem("Orbit Camera");
  for (int i = 0; i < my3DWindow->getJugglerCount(); i++)
    cameraComboBox->addItem(QString("Juggler %1 view").arg(i + 1));
}

void MainWindow::cameraIndexChanged(int index)
{
  if (index == 0)
  {
    my3DWindow->setCameraToOrbit();
    return;
  }

  if (index < 0)
    return;

  my3DWindow->setCameraToFirstPers(index - 1);
}

void MainWindow::propTypeChanged(int index)
{
  launchBallTypeComboBox->hide();
  launchRingTypeComboBox->hide();
  launchClubTypeComboBox->hide();
  switch(index)
  {
  case ball: launchBallTypeComboBox->show(); break;
  case ring: launchRingTypeComboBox->show(); break;
  case club: launchClubTypeComboBox->show(); break;
  default: break;
  }
}

void MainWindow::newFile()
{
  if (maybeSave()) {
    setCurrentFile(QString());
  }
}

void MainWindow::open()
{
  if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
      loadFile(fileName);
  }
}

bool MainWindow::save()
{
  if (curFile.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(curFile);
  }
}

bool MainWindow::saveAs()
{
  QFileDialog dialog(this);
  dialog.setWindowModality(Qt::WindowModal);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  if (dialog.exec() != QDialog::Accepted)
    return false;
  return saveFile(dialog.selectedFiles().first());
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Application"),
                     tr("The <b>Application</b> example demonstrates how to "
                        "write modern GUI applications using Qt, with a menu bar, "
                        "toolbars, and a status bar."));
}

void MainWindow::loadFile(const QString &fileName)
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

QString MainWindow::getPropToString(const int prop)
{
  switch (prop)
  {
  case ball: return QString("Ball"); break;
  case ring: return QString("Ring"); break;
  case club: return QString("Club"); break;
  default: return QString(""); break;
  }
}

jugglingProp MainWindow::getPropFromString(const QString &value)
{
  if (value == "Ball") return ball;
  else if (value == "Ring") return ring;
  else if (value == "Club") return club;
  else return ball;
}

bool MainWindow::saveFile(const QString &fileName)
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

void MainWindow::setCurrentFile(const QString &fileName)
{
  curFile = fileName;
  //    textEdit->document()->setModified(false);
  setWindowModified(false);

  QString shownName = curFile;
  if (curFile.isEmpty())
    shownName = "untitled.txt";
  setWindowFilePath(shownName);
}

void MainWindow::documentWasModified()
{
  //    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave()) {
    //        writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

bool MainWindow::maybeSave()
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

