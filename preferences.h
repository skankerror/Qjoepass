#ifndef PREFERENCES_H
#define PREFERENCES_H



#include <QDialog>
#include "settings.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

//! [0]
class Preferences: public QDialog
{
 public:
    explicit Preferences(MySettings *aSettings,QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
    MySettings *settings;
};

class worldTab : public QWidget
{
    Q_OBJECT

public:
    explicit worldTab(MySettings *aSettings, QWidget *parent = nullptr);
private slots:
    void backgroundColor();
private:
    QPushButton *colorButton;
    MySettings *settings;
};

class causalTab : public QWidget
{
    Q_OBJECT

public:
    explicit causalTab(MySettings *aSettings, QWidget *parent = nullptr);

private:
  MySettings *settings;
};

class soundTab : public QWidget
{
    Q_OBJECT

public:
    explicit soundTab(MySettings *aSettings, QWidget *parent = nullptr);

private:
  MySettings *settings;
};

#endif // PREFERENCES_H
