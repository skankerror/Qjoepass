
#include <QtWidgets>
#include "preferences.h"

Preferences::Preferences(MySettings *aSettings, QWidget *parent)
    : QDialog(parent),
      settings(aSettings)
{
//    QFileInfo fileInfo(fileName);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new worldTab(aSettings), tr("World"));
    tabWidget->addTab(new causalTab(aSettings), tr("Causal Editor"));
    tabWidget->addTab(new soundTab(aSettings), tr("Sound"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
//! [4]

//! [5]
    setWindowTitle(tr("Tab Dialog"));
}
//! [5]

//! [6]
worldTab::worldTab(MySettings *aSettings, QWidget *parent)
    : QWidget(parent),
      settings(aSettings)
{
    QLabel *colorLabel = new QLabel(tr("Background:"));
    QPushButton *colorButton = new QPushButton("Change Color...", this);
    connect(colorButton, SIGNAL (released()), this, SLOT (backgroundColor()));


//    QLabel *pathLabel = new QLabel(tr("Path:"));
//    pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(colorLabel);
    mainLayout->addWidget(colorButton);
//    mainLayout->addWidget(pathLabel);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
void worldTab::backgroundColor()
{
//    MySettings settings;
    QColor colorBG = settings->value("world/colorbg").value<QColor>();
    QColor color = QColorDialog::getColor(colorBG, this );
    if( color.isValid() )
    {
      qDebug() << "Color Choosen : " << color.name();
    }
}
causalTab::causalTab(MySettings *aSettings, QWidget *parent)
    : QWidget(parent),
      settings(aSettings)
{

}
//! [7]

//! [8]
soundTab::soundTab(MySettings *aSettings, QWidget *parent)
    : QWidget(parent),
      settings(aSettings)
{
    QLabel *topLabel = new QLabel(tr("Open with:"));

    QListWidget *applicationsListBox = new QListWidget;
    QStringList applications;

    for (int i = 1; i <= 30; ++i)
        applications.append(tr("Application %1").arg(i));
    applicationsListBox->insertItems(0, applications);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(applicationsListBox);
    setLayout(layout);
}
