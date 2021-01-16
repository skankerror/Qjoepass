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

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>



int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(application);

  QApplication app(argc, argv);
  // needed for QSettings
  QCoreApplication::setOrganizationName("Anonymous_jugglers");
  QCoreApplication::setApplicationName("Qjoepass");
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);

  // uncomment when we will be at this point of development
//  QCommandLineParser parser;
//  parser.setApplicationDescription(QCoreApplication::applicationName());
//  parser.addHelpOption();
//  parser.addVersionOption();
//  parser.addPositionalArgument("file", "The file to open.");
//  parser.process(app);

  // qss
  QFile file(":/qss/res/qss/MyStyleSheet.qss");
      file.open(QFile::ReadOnly);
      QString styleSheet = QString::fromLatin1(file.readAll());
      app.setStyleSheet(styleSheet);

  MainWindow mainWin;
//  if (!parser.positionalArguments().isEmpty())
//      mainWin.loadFile(parser.positionalArguments().first());
  mainWin.show();
  return app.exec();
}
