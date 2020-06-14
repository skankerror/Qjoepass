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

#include "siteswap.h"
#include <QDebug>

SiteSwap::SiteSwap(QVector<int> &aVInt,
                   jugglingProp aProp,
                   bool aSynchron,
                   QObject *parent)
  : QObject(parent),
    v_event(aVInt),
    synchron(aSynchron),
    prop(aProp)
{
  period = v_event.size();
  valid = isValid();
  propCount = getNumProp();
  if (valid)
  {
    setState();
  }
}

bool SiteSwap::isValid() const
{
  // create a vector to test our values
  QVector<int> vTest;
  // let's be optimistic !
  bool ret = true;

  for (int i = 0; i < period; i++)
  {
    // we modulo each launch in order to find where this launch will be in next loop
    int modLaunch = (v_event.at(i) + i) % period;
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

void SiteSwap::setPropType(jugglingProp aProp)
{
  prop = aProp;
}

void SiteSwap::setState()
{
  if (!period) // this shoud not happen
    return;

  // simple case
  if (period == 1)
  {
    state.resize(propCount);
    for (int i = 0; i < propCount; i++)
    {
      state.setBit(i, true);
    }
    return;
  }

  // a bit more complex
  int propFoundCount = 0; // count of true sites found
  int index = 0;
  QVector<int> indexAlreadySet; // store false forward sites

  while (propFoundCount < propCount) // while we haven't found all true sites
  {
    // verify if site has not already been set
    if (indexAlreadySet.indexOf(index) == -1)
    {
      if (index + 1 > state.size())
        state.resize(index + 1); // we must resize
      int launch = v_event.at(index % period);
      if (launch) // if launch is not 0
      {
        state.setBit(index, true); // it's a true site
        propFoundCount++; // we found one more
        if ((index + launch + 1) > state.size())
          state.resize(index + launch + 1); // we must resize
        state.setBit(index + launch, false); // it's surely a false
        indexAlreadySet.append(index + launch); // we declare it as already set
      }
    }
    index++;
  }
  // now we truncate finishing 0s
  bool test = state.testBit(state.size() - 1);
  while (test == false)
  {
    state.resize(state.size() - 1); // erase last bit
    test = state.testBit(state.size() - 1);
  }
}
