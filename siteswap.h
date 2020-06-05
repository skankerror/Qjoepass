#ifndef SITESWAP_H
#define SITESWAP_H

#include <QObject>

class SiteSwap : public QObject
{
  Q_OBJECT
public:
  explicit SiteSwap(QObject *parent = nullptr);

signals:

private:
  QVector<QVector<int>> v_v_Event;
  bool isSynchron = false;
};

#endif // SITESWAP_H
