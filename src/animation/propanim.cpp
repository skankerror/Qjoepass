#include "propanim.h"

PropAnim::PropAnim(QVector<Juggler *> t_v_juggler,
                   int t_propId,
                   jugglingProp t_jugglingProp,
                   QObject *parent):
  QSequentialAnimationGroup(parent),
  m_propId(t_propId),
  m_v_juggler(t_v_juggler)
{

}

QSequentialAnimationGroup *PropAnim::parabolicAnim(int t_jugglerIdLaunch,
                                                   hand t_handLaunch,
                                                   int t_jugglerIdRecieve,
                                                   hand t_handRecieve,
                                                   int t_launch)
{

}

QSequentialAnimationGroup *PropAnim::dwellAnim(int t_jugglerId,
                                               int t_nextLaunch,
                                               hand t_hand)
{

}

