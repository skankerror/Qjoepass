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
  m_settings = new MySettings() ;

  auto widget = new QWidget;
  setCentralWidget(widget);

  auto topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  m_infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                            "invoke a context menu</i>"));
  m_infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  m_infoLabel->setAlignment(Qt::AlignCenter);

  auto bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  m_my3DWindow = new My3DWindow(m_settings);
  m_container = QWidget::createWindowContainer(m_my3DWindow);
  QSize screenSize = m_my3DWindow->screen()->size();
  m_container->setMinimumSize(QSize(WIDGET3D_MIN_W, WIDGET3D_MIN_H));
  m_container->setMaximumSize(screenSize);

  auto layout = new QVBoxLayout;
  layout->addWidget(m_container);
  widget->setLayout(layout);

  createMenus();
  createToolBar();

  QString message = tr("A context menu is available by right-clicking");
  statusBar()->showMessage(message);

  setWindowTitle(tr("Qjoepass"));
  setMinimumSize(WINDOW_MINIMUM_H, WINDOW_MINIMUM_H);

  resize(WINDOW_W, WINDOW_H);

  m_pref = new Preferences(m_settings);

  connect(m_pref, SIGNAL(colorChanged(QColor)), m_my3DWindow, SLOT(changeBackground(QColor)));
  connect(m_pref, SIGNAL(groundColorChanged(QColor)), m_my3DWindow, SLOT(changeGroundColor(QColor)));

}

void MainWindow::createMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));

  //    QToolBar *fileToolBar = addToolBar(tr("File"));
  const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
  m_newAct = new QAction(newIcon, tr("&New"), this);
  m_newAct->setShortcuts(QKeySequence::New);
  m_newAct->setStatusTip(tr("Create a new file"));
  connect(m_newAct, &QAction::triggered, this, &MainWindow::newFile);
  m_fileMenu->addAction(m_newAct);
  //    fileToolBar->addAction(newAct);

  const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
  m_openAct = new QAction(openIcon, tr("&Open..."), this);
  m_openAct->setShortcuts(QKeySequence::Open);
  m_openAct->setStatusTip(tr("Open an existing file"));
  connect(m_openAct, &QAction::triggered, this, &MainWindow::open);
  m_fileMenu->addAction(m_openAct);
  //    fileToolBar->addAction(openAct);

  const QIcon saveIcon = QIcon::fromTheme("document-open", QIcon(":/images/save.png"));
  m_saveAct = new QAction(saveIcon, tr("&Save..."), this);
  m_saveAct->setShortcuts(QKeySequence::Save);
  m_saveAct->setStatusTip(tr("Save to a file"));
  connect(m_saveAct, &QAction::triggered, this, &MainWindow::save);
  m_fileMenu->addAction(m_saveAct);
  //    fileToolBar->addAction(saveAct);

  const QIcon saveAsIcon = QIcon::fromTheme("document-open", QIcon(":/images/saveAs.png"));
  m_saveAsAct = new QAction(saveAsIcon, tr("&SaveAs..."), this);
  m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
  m_saveAsAct->setStatusTip(tr("Save as a new file"));
  connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
  m_fileMenu->addAction(m_saveAsAct);
  //    fileToolBar->addAction(saveAsAct);

  //    const QIcon exitIcon = QIcon::fromTheme("exit", QIcon(":/images/exit.png"));
  m_exitAct = new QAction(tr("&Quit..."), this);
  m_exitAct->setShortcuts(QKeySequence::Quit);
  m_exitAct->setStatusTip(tr("Quit joePass"));
  connect(m_exitAct, &QAction::triggered, this, &QWidget::close);
  m_fileMenu->addAction(m_exitAct);
  //    fileToolBar->addAction(saveAsAct);


  m_editMenu = menuBar()->addMenu(tr("&Edit"));
  //    QToolBar *editToolBar = addToolBar(tr("Edit"));

  m_prefAct = new QAction( tr("&Preferences..."), this);

  m_l_prefShortcut << QKeySequence("Ctrl+P");
  m_prefAct->setShortcuts(m_l_prefShortcut);
  m_prefAct->setStatusTip(tr("Open preferences dialog"));
  connect(m_prefAct, &QAction::triggered, this, &MainWindow::preferencesDial);
  m_editMenu->addAction(m_prefAct);

  m_helpMenu = menuBar()->addMenu(tr("&Help"));

  const QIcon aboutIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  m_aboutAct = m_helpMenu->addAction(aboutIcon, tr("&About..."), this,  &MainWindow::about);
  m_aboutAct->setStatusTip(tr("Show the QjoePass's About box"));
  connect(m_aboutAct, &QAction::triggered, this, &MainWindow::about);
  m_helpMenu->addAction(m_aboutAct);

  const QIcon aboutQtIcon = QIcon::fromTheme("help-about", QIcon(":/images/about.png"));
  m_aboutQtAct = m_helpMenu->addAction(aboutQtIcon, tr("About &Qt"), qApp, &QApplication::aboutQt);
  m_aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createToolBar()
{
  m_myToolBar = addToolBar("siteswap bar");

  auto myToolBarWidget = new QWidget(this);
  m_toolBarLayout = new QHBoxLayout();

  m_propLabel = new QLabel("type of prop", this);
  m_propTypeComboBox = new QComboBox(this);
  for (int i = 0; i < propNumb; i++)
    m_propTypeComboBox->addItem(getPropToString(i));
  m_launchPropTypeLabel = new QLabel("type of Launch", this);
  m_launchBallTypeComboBox = new QComboBox(this);
    m_launchBallTypeComboBox->addItem("normal", normalBall);
    m_launchBallTypeComboBox->addItem("bounce", bounce);
  m_launchRingTypeComboBox = new QComboBox(this);
    m_launchRingTypeComboBox->addItem("normal", normalRing);
    m_launchRingTypeComboBox->addItem("pancake", panCake);
  m_launchClubTypeComboBox = new QComboBox(this);
    m_launchClubTypeComboBox->addItem("normal", normalClub);
    m_launchClubTypeComboBox->addItem("flat", flat);
    m_launchClubTypeComboBox->addItem("helicopter", helicopter);
  m_propTypeComboBox->setCurrentIndex(ball);

  m_periodLabel = new QLabel("period", this);
  m_periodSpinBox = new QSpinBox(this);
    m_periodSpinBox->setMinimum(1);
    m_periodSpinBox->setValue(1);

  m_firstSiteSpinBox = new QSpinBox(this);


  m_spinBoxContainer = new QWidget(this);
  m_spinBoxLayout = new QHBoxLayout();
    m_firstSiteSpinBox->setMinimum(1);
    m_firstSiteSpinBox->setValue(3);
//  m_spinBoxLayout->addWidget(m_firstSiteSpinBox);
  m_spinBoxContainer->setLayout(m_spinBoxLayout);

  m_launchPushButton = new QPushButton("launch !", this);
    m_launchPushButton->setToolTip("Start animation");
    m_launchPushButton->setToolTipDuration(2000);

  m_toolBarLayout->addWidget(m_propLabel);
  m_toolBarLayout->addWidget(m_propTypeComboBox);
  m_toolBarLayout->addWidget(m_launchPropTypeLabel);
  m_toolBarLayout->addWidget(m_launchBallTypeComboBox);
  m_toolBarLayout->addWidget(m_launchRingTypeComboBox);
    m_launchRingTypeComboBox->hide();
  m_toolBarLayout->addWidget(m_launchClubTypeComboBox);
    m_launchClubTypeComboBox->hide();
  m_toolBarLayout->addWidget(m_periodLabel);
  m_toolBarLayout->addWidget(m_periodSpinBox);
  m_toolBarLayout->addWidget(m_firstSiteSpinBox);
  m_toolBarLayout->addWidget(m_spinBoxContainer);
  m_toolBarLayout->addWidget(m_launchPushButton, 0, Qt::AlignRight);
  myToolBarWidget->setLayout(m_toolBarLayout);
  m_myToolBar->addWidget(myToolBarWidget);

  auto MyToolBarWidget2 = new QWidget(this);
  m_toolBar2Layout = new QHBoxLayout();
  m_cameraLabel = new QLabel("change Camera", this);
  m_cameraComboBox = new QComboBox(this);
  updateCameraComboBox();
  m_toolBar2Layout->addWidget(m_cameraLabel);
  m_toolBar2Layout->addWidget(m_cameraComboBox);

  MyToolBarWidget2->setLayout(m_toolBar2Layout);
  m_myToolBar->addWidget(MyToolBarWidget2);

  // NOTE: tabbing order is bad and that doesn't fix it
  m_myToolBar->setTabOrder(m_firstSiteSpinBox, m_spinBoxContainer);

  connect(m_periodSpinBox, SIGNAL(valueChanged(int)), this, SLOT(periodChanged(int)));
  connect(m_launchPushButton, SIGNAL(clicked()), this, SLOT(launchSiteSwap()));
  connect(m_my3DWindow, SIGNAL(jugglerCountChanged()), this, SLOT(updateCameraComboBox()));
  connect(m_cameraComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cameraIndexChanged(int)));
  connect(m_propTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(propTypeChanged(int)));

}
void MainWindow::preferencesDial()
{
  m_pref->setWindowTitle("Preferences");
  m_pref->show();
}

void MainWindow::launchSiteSwap()
{
  QVector<siteswapEvent *> v_event;
  int f_launch = m_firstSiteSpinBox->value();
  auto event = new struct siteswapEvent; // TODO: le delete !
  event->launch = f_launch;
  // paramétrer les autres arguments ici
  v_event.append(event);
  for (int i = 0; i < m_v_spinBox.size(); i++)
  {
    f_launch = m_v_spinBox.at(i)->value();
    auto newEvent = new struct siteswapEvent; // TODO: le delete
    newEvent->launch = f_launch;
    // pareil
    v_event.append(newEvent);
  }
  propType prop = getPropFromString(m_propTypeComboBox->currentText());
  int launchType = 0; // normalLaunch
  switch(prop)
  {
  case ball: launchType = m_launchBallTypeComboBox->currentIndex(); break;
  case ring: launchType = m_launchRingTypeComboBox->currentIndex(); break;
  case club: launchType = m_launchClubTypeComboBox->currentIndex(); break;
  default: break;
  }

  // TODO: set several jugglers capabilities
  // waiting for better things...
  int jugglerCount = 1;
  // on ne gère pas encore le multiplex
  m_my3DWindow->createSiteSwap(v_event, jugglerCount, prop, launchType, false);
}

void MainWindow::periodChanged(int t_index)
{
  int numSpinBox = m_v_spinBox.size();
  if (numSpinBox)
  {
    for (int j = 0; j < numSpinBox; j++)
    {
      auto spinBox = m_v_spinBox.at(j);
      spinBox->hide();
//      m_toolBarLayout->removeWidget(spinBox);
      m_spinBoxLayout->removeWidget(spinBox);
    }
    m_v_spinBox.clear();
    m_v_spinBox.squeeze();
  }
  if (t_index > 1)
  {
//    m_toolBarLayout->removeWidget(m_launchPushButton);
    for (int k = 1; k < t_index; k++)
    {
      auto newSpinBox = new QSpinBox(this);
      newSpinBox->setMinimum(0);
      m_v_spinBox.append(newSpinBox);
//      m_toolBarLayout->addWidget(newSpinBox);
      m_spinBoxLayout->addWidget(newSpinBox);
    }
//    m_toolBarLayout->addWidget(m_launchPushButton);
  }
  m_myToolBar->setTabOrder(m_firstSiteSpinBox, m_spinBoxContainer);
}

void MainWindow::updateCameraComboBox()
{
  m_cameraComboBox->clear();
  m_cameraComboBox->addItem("Orbit Camera");
  for (int i = 0; i < m_my3DWindow->getJugglerCount(); i++)
    m_cameraComboBox->addItem(QString("Juggler %1 view").arg(i + 1));
}

void MainWindow::cameraIndexChanged(int t_index)
{
  if (t_index == 0)
  {
    m_my3DWindow->setCameraToOrbit();
    return;
  }

  if (t_index < 0)
    return;

  m_my3DWindow->setCameraToFirstPers(t_index - 1);
}

void MainWindow::propTypeChanged(int t_index)
{
  m_launchBallTypeComboBox->hide();
  m_launchRingTypeComboBox->hide();
  m_launchClubTypeComboBox->hide();
  switch(t_index)
  {
  case ball: m_launchBallTypeComboBox->show(); break;
  case ring: m_launchRingTypeComboBox->show(); break;
  case club: m_launchClubTypeComboBox->show(); break;
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
  if (m_curFile.isEmpty())
  {
    return saveAs();
  }
  else
  {
    return saveFile(m_curFile);
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

void MainWindow::loadFile(const QString &t_fileName)
{
  QFile file(t_fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Application"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(QDir::toNativeSeparators(t_fileName), file.errorString()));
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

  setCurrentFile(t_fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

QString MainWindow::getPropToString(const int t_prop)
{
  switch (t_prop)
  {
  case ball: return QString("Ball"); break;
  case ring: return QString("Ring"); break;
  case club: return QString("Club"); break;
  default: return QString(""); break;
  }
}

propType MainWindow::getPropFromString(const QString &t_value)
{
  if (t_value == "Ball") return ball;
  else if (t_value == "Ring") return ring;
  else if (t_value == "Club") return club;
  else return ball;
}

bool MainWindow::saveFile(const QString &t_fileName)
{
  QString errorMessage;

  QGuiApplication::setOverrideCursor(Qt::WaitCursor);
  QSaveFile file(t_fileName);
  if (file.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream out(&file);
    //        out << textEdit->toPlainText();
    if (!file.commit()) {
      errorMessage = tr("Cannot write file %1:\n%2.")
          .arg(QDir::toNativeSeparators(t_fileName), file.errorString());
    }
  } else {
    errorMessage = tr("Cannot open file %1 for writing:\n%2.")
        .arg(QDir::toNativeSeparators(t_fileName), file.errorString());
  }
  QGuiApplication::restoreOverrideCursor();

  if (!errorMessage.isEmpty()) {
    QMessageBox::warning(this, tr("Application"), errorMessage);
    return false;
  }

  setCurrentFile(t_fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void MainWindow::setCurrentFile(const QString &t_fileName)
{
  m_curFile = t_fileName;
  //    textEdit->document()->setModified(false);
  setWindowModified(false);

  QString shownName = m_curFile;
  if (m_curFile.isEmpty())
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

