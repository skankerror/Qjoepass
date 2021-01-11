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

Preferences::Preferences(MySettings *t_settings, QWidget *parent)
  : QDialog(parent),
    m_settings(t_settings)
{
  m_tabWidget = new QTabWidget;

  m_worldTab = new QWidget(this);

  QVBoxLayout *mainLayout = new QVBoxLayout;

  auto colorLabel = new QLabel(tr("Background:"));
  auto colorButton = new QPushButton("Change Color...", this);
  connect(colorButton, SIGNAL (released()), this, SLOT (backgroundColor()));
  mainLayout->addWidget(colorLabel);
  mainLayout->addWidget(colorButton);
  //    mainLayout->addWidget(pathLabel);

  auto groundColorLabel = new QLabel(tr("Ground:"));
  auto groundColorButton = new QPushButton("Change Color...", this);
  connect(groundColorButton, SIGNAL (released()), this, SLOT (groundColor()));

  mainLayout->addWidget(groundColorLabel);
  mainLayout->addWidget(groundColorButton);
  mainLayout->addStretch(1);
  m_worldTab->setLayout(mainLayout);

  m_casualTab = new QWidget(this);

  m_soundTab = new QWidget(this);
  auto topLabel = new QLabel(tr("Open with:"));
  auto applicationsListBox = new QListWidget;
  QStringList applications;
  for (int i = 1; i <= 30; ++i)
    applications.append(tr("Application %1").arg(i));
  applicationsListBox->insertItems(0, applications);
  auto layout = new QVBoxLayout;
  layout->addWidget(topLabel);
  layout->addWidget(applicationsListBox);
  m_soundTab->setLayout(layout);

  m_tabWidget->addTab(m_worldTab, "World");
  m_tabWidget->addTab(m_casualTab, "Causal editor");
  m_tabWidget->addTab(m_soundTab, "Sound");

  m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel);

  connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  auto mainLayout2 = new QVBoxLayout;
  mainLayout2->addWidget(m_tabWidget);
  mainLayout2->addWidget(m_buttonBox);
  setLayout(mainLayout2);

  setWindowTitle(tr("Tab Dialog"));
}

void Preferences::backgroundColor()
{
  QColor colorBG = m_settings->value("world/colorbg").value<QColor>();
  QColor color = QColorDialog::getColor(colorBG, this, "Choose background color", QColorDialog::DontUseNativeDialog);
  if( color.isValid() )
  {
    m_settings->setValue("world/colorbg",color);
    emit colorChanged(color);
  }
}

void Preferences::groundColor()
{
  QColor groundColor = m_settings->value("world/groundColor").value<QColor>();
  QColor color = QColorDialog::getColor(groundColor, this, "Choose ground color", QColorDialog::DontUseNativeDialog);
  if( color.isValid() )
  {
    qDebug() << "Ground Color Choosen : " << color.name();
    m_settings->setValue("world/groundColor",color);
    emit groundColorChanged(color);
  }
}
