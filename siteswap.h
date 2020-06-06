#ifndef SITESWAP_H
#define SITESWAP_H

#include <QObject>

class SiteSwap : public QObject
{
  Q_OBJECT
public:
  explicit SiteSwap(QVector<int> &aVInt,
                    bool aSynchron = false,
                    QObject *parent = nullptr);

  bool isValid() const;
  int getNumProp() const;
  int getPeriod() const {return period;};
  int at(int i) const {return v_event.at(i);};

signals:

private:
  QVector<int> v_event; // without multiplex
  QVector<QVector<int>> v_v_Event; // with multiplex
  int period;
  bool valid = false;
  bool synchron = false;
};

#endif // SITESWAP_H
