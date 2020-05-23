#ifndef PREFERENCES_H
#define PREFERENCES_H



#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

//! [0]
class worldTab : public QWidget
{
    Q_OBJECT

public:
    explicit worldTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
private slots:
    void backgroundColor();
private:
    QPushButton *colorButton;
};
//! [0]


//! [1]
class causalTab : public QWidget
{
    Q_OBJECT

public:
    explicit causalTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [1]


//! [2]
class soundTab : public QWidget
{
    Q_OBJECT

public:
    explicit soundTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
//! [2]


//! [3]
class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(const QString &fileName, QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};
//! [3]

#endif // PREFERENCES_H
