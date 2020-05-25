#ifndef PREFERENCES_H
#define PREFERENCES_H



#include <QDialog>
#include "settings.h"
#include <QColor>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

//! [0]
class Preferences: public QDialog
{
  Q_OBJECT
 public:
    explicit Preferences(MySettings *aSettings,QWidget *parent = nullptr);

  private slots:
      void backgroundColor();

signals:
      void colorChanged(QColor);

private:
    QTabWidget *tabWidget;

    QWidget *worldTab;

    QDialogButtonBox *buttonBox;
    MySettings *settings;

    QWidget *casualTab;
    QWidget *soundTab;
};

#endif // PREFERENCES_H
