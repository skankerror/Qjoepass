#include "propanim.h"

PropAnim::PropAnim(QVector<Juggler *> t_v_juggler,
                   int t_propId,
                   jugglingProp t_jugglingProp,
                   int t_launchType,
                   QObject *parent):
  QSequentialAnimationGroup(parent),
  m_propId(t_propId),
  m_v_juggler(t_v_juggler),
  m_jugglingProp(t_jugglingProp)
{
  switch (m_jugglingProp) // TODO: check out of range
  {
  default:
  case  ball:
    m_launchTypeBall = launchTypeBall(t_launchType);
    break;
  case ring: m_launchTypeRing = launchTypeRing(t_launchType); break;
  case club: m_launchTypeClub = launchTypeClub(t_launchType); break;
  }
}

QSequentialAnimationGroup *PropAnim::parabolicAnim(int t_jugglerIdLaunch,
                                                   hand t_handLaunch,
                                                   int t_jugglerIdRecieve,
                                                   hand t_handRecieve,
                                                   int t_launch)
{
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerRecieve = m_v_juggler.at(t_jugglerIdRecieve); // TODO: check out of range
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_jugglingProp == ring  && m_launchTypeRing == panCake) ||
      (m_jugglingProp == club && m_launchTypeClub == helicopter);


}

QSequentialAnimationGroup *PropAnim::dwellAnim(int t_jugglerId,
                                               int t_nextLaunch,
                                               hand t_hand)
{

}

