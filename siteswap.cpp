#include "siteswap.h"

SiteSwap::SiteSwap(QVector<int> &aVInt,
                   bool aSynchron,
                   QObject *parent)
  : QObject(parent),
    v_event(aVInt),
    synchron(aSynchron)
{
  period = v_event.size();
  valid = isValid();
}

bool SiteSwap::isValid() const
{
  // create a vector to test our values
  QVector<int> vTest;
  // let's be optimistic !
  bool ret = true;

  for (int i = 0; i < vTest.size(); i++)
  {
    // we modulo each launch
    int modLaunch = v_event.at(i) % period;
    // and test to find collision
    if (vTest.indexOf(modLaunch) != -1) // collision
    {
      ret = false; // bad news
      break;
    }
    // this seems good, we're adding to our vector for next tests
    vTest.append(modLaunch);
  }
  return ret;
}

int SiteSwap::getNumProp() const
{
  if(!valid)
    return 0;

  int totalLaunch = 0;
  for (int i = 0; i < v_event.size(); i++)
    totalLaunch += v_event.at(i);
  return totalLaunch / period;
}
