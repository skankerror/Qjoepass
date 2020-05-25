
#include <QtWidgets>
#include "preferences.h"

Preferences::Preferences(MySettings *aSettings, QWidget *parent)
  : QDialog(parent),
    settings(aSettings)
{

  tabWidget = new QTabWidget;

  worldTab = new QWidget(this);

  QLabel *colorLabel = new QLabel(tr("Background:"));
  QPushButton *colorButton = new QPushButton("Change Color...", this);
  connect(colorButton, SIGNAL (released()), this, SLOT (backgroundColor()));
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(colorLabel);
  mainLayout->addWidget(colorButton);
  //    mainLayout->addWidget(pathLabel);

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
  //    MySettings settings;
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  QColor color = QColorDialog::getColor(colorBG, this, "Choose background color", QColorDialog::DontUseNativeDialog);
  if( color.isValid() )
  {
    qDebug() << "Color Choosen : " << color.name();
    emit colorChanged(color);
  }
}
