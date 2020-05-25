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
    explicit Preferences(MySettings *settings,QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};

class worldTab : public QWidget
{
    Q_OBJECT

public:
    explicit worldTab(MySettings *settings, QWidget *parent = nullptr);
private slots:
    void backgroundColor();
private:
    QPushButton *colorButton;
};

class causalTab : public QWidget
{
    Q_OBJECT

public:
    explicit causalTab(MySettings *settings, QWidget *parent = nullptr);
};

class soundTab : public QWidget
{
    Q_OBJECT

public:
    explicit soundTab(MySettings *settings, QWidget *parent = nullptr);
};

#endif // PREFERENCES_H
