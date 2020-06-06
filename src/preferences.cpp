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
#include "preferences.h"

Preferences::Preferences(MySettings *aSettings, QWidget *parent)
  : QDialog(parent),
    settings(aSettings)
{

  tabWidget = new QTabWidget;

  worldTab = new QWidget(this);

  QVBoxLayout *mainLayout = new QVBoxLayout;

  QLabel *colorLabel = new QLabel(tr("Background:"));
  QPushButton *colorButton = new QPushButton("Change Color...", this);
  connect(colorButton, SIGNAL (released()), this, SLOT (backgroundColor()));
  mainLayout->addWidget(colorLabel);
  mainLayout->addWidget(colorButton);
  //    mainLayout->addWidget(pathLabel);

  QLabel *groundColorLabel = new QLabel(tr("Ground:"));
  QPushButton *groundColorButton = new QPushButton("Change Color...", this);
  connect(groundColorButton, SIGNAL (released()), this, SLOT (groundColor()));

  mainLayout->addWidget(groundColorLabel);
  mainLayout->addWidget(groundColorButton);
  mainLayout->addStretch(1);
  worldTab->setLayout(mainLayout);

  casualTab = new QWidget(this);

  soundTab = new QWidget(this);
  QLabel *topLabel = new QLabel(tr("Open with:"));
  QListWidget *applicationsListBox = new QListWidget;
  QStringList applications;
  for (int i = 1; i <= 30; ++i)
    applications.append(tr("Application %1").arg(i));
  applicationsListBox->insertItems(0, applications);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(topLabel);
  layout->addWidget(applicationsListBox);
  soundTab->setLayout(layout);

  tabWidget->addTab(worldTab, "World");
  tabWidget->addTab(casualTab, "Causal editor");
  tabWidget->addTab(soundTab, "Sound");

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *mainLayout2 = new QVBoxLayout;
  mainLayout2->addWidget(tabWidget);
  mainLayout2->addWidget(buttonBox);
  setLayout(mainLayout2);

  setWindowTitle(tr("Tab Dialog"));

}

void Preferences::backgroundColor()
{
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  QColor color = QColorDialog::getColor(colorBG, this, "Choose background color", QColorDialog::DontUseNativeDialog);
  if( color.isValid() )
  {
    settings->setValue("world/colorbg",color);
    emit colorChanged(color);
  }
}

void Preferences::groundColor()
{
  QColor groundColor = settings->value("world/groundColor").value<QColor>();
  QColor color = QColorDialog::getColor(groundColor, this, "Choose ground color", QColorDialog::DontUseNativeDialog);
  if( color.isValid() )
  {
    qDebug() << "Ground Color Choosen : " << color.name();
    settings->setValue("world/groundColor",color);
    emit groundColorChanged(color);
  }
}
