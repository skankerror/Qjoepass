#include "propanim.h"

PropAnim::PropAnim(QVector<Juggler *> t_v_juggler,
                   JugglingProp *t_prop,
                   int t_propId,
                   propType t_propType,
                   int t_launchType,
                   QObject *parent)

  : QSequentialAnimationGroup(parent),
    m_propId(t_propId),
    m_v_juggler(t_v_juggler),
    m_propType(t_propType),
    m_prop(t_prop)
{
  switch (m_propType) // TODO: check out of range
  {
  default:
  case  ball: m_launchTypeBall = launchTypeBall(t_launchType); break;
  case ring: m_launchTypeRing = launchTypeRing(t_launchType); break;
  case club: m_launchTypeClub = launchTypeClub(t_launchType); break;
  }

  // là on met tout le merdier du 1er lancement ? donné par siteswap ?
  // 1er lancer, bien orienter les objets, les placer dans les bonnes mains
  // créer les 1ers dwells avec plusieurs objets dans les mains
  // ou on fait tout ça dans animation ?

  // pour les mains, principe de la timeline, chaque mouvement de main doit avoir
  // un temps de début et un temps de fin, voir dans handanim. Connecter les objets ?

  /************************testing zone ****************************/
  auto testAnim = parabolicAnim(1,
                                hand(rightHand),
                                0,
                                hand(leftHand),
                                5);
  addAnimation(testAnim);
  setLoopCount(INFINITE_LOOP);
  start();
}

QParallelAnimationGroup *PropAnim::parabolicAnim(int t_jugglerIdLaunch,
                                                   hand t_handLaunch,
                                                   int t_jugglerIdRecieve,
                                                   hand t_handRecieve,
                                                   int t_launch)
{
  // create parallel animation group to translate and rotation if needed
  auto parallelAnimGroup = new QParallelAnimationGroup(this);

  // get our concerned jugglers
  auto jugglerLaunch = m_v_juggler.at(t_jugglerIdLaunch); // TODO: check out of range
  auto jugglerRecieve = m_v_juggler.at(t_jugglerIdRecieve); // TODO: check out of range

  // declare our 2 positions
  QVector3D posProp; // pos where it starts
  QVector3D posFinal; // pos where it should finish

  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_propType == ring  && m_launchTypeRing == panCake) ||
      (m_propType == club && m_launchTypeClub == helicopter);

  // set position were parabolic curve starts
  if (t_handLaunch == leftHand)
  {
    t_jugglerIdLaunch == t_jugglerIdRecieve ? // is it a passing launch ?
          posProp = jugglerLaunch->getPositionLHint():
        posProp = jugglerLaunch->getPositionLHmed(); // NOTE: don't know we must set another launch place
  }
  else
  {
    t_jugglerIdLaunch == t_jugglerIdRecieve ? // is it a passing launch ?
          posProp = jugglerLaunch->getPositionRHint():
        posProp = jugglerLaunch->getPositionRHmed(); // NOTE: don't know we must set another launch place
  }

  // set position where it ends
  // TODO: il faut bouger l'endroit du départ pour aller dans la direction du recieve
  // l'obtenir de juggler ?
  if (t_handRecieve == leftHand)
  {
    isExtPlusCatch ? // helico, pancakes ?
          posFinal = jugglerRecieve->getPositionLHextPlus():
        posFinal = jugglerRecieve->getPositionLHext();
  }
  else
  {
    isExtPlusCatch ?
          posFinal = jugglerRecieve->getPositionRHextPlus():
        posFinal = jugglerRecieve->getPositionRHext();
  }

  // Shannon theorem
  float arcTime;
  if (t_launch == 1) // For launch 1 Shannon doesn't work
  {
    arcTime = LAUNCH1_TIME;
  }
  else
    arcTime = ((HAND_PERIOD) / 2) * (t_launch - (2 * DWELL_RATIO));

  // we calculate velocity launch
  auto velBall = ((posFinal - posProp) - 0.5 *
                       (GRAVITY * qPow(arcTime, 2))) / arcTime;

  int frameCount = (int)((arcTime / (DELTA_TIME)));

  // We create our curve
  /*QVector<QVector3D>*/ auto v_parabolic = Curves::curveParabolic(velBall, posProp, frameCount);

  // create sequential for translation
  auto translationAnimGroup = new QSequentialAnimationGroup(this);

  // this loop creates all animations for translation
  for (int i = 0; i < frameCount; i++)
  {
    auto translationAnim = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_position"));
    translationAnim->setDuration((int)(DELTA_TIME * S_TO_MS));
    translationAnim->setStartValue(v_parabolic.at(i));
    translationAnim->setEndValue(v_parabolic.at(i + 1));
    translationAnim->setLoopCount(ONE_LOOP);
    translationAnimGroup->addAnimation(translationAnim);
  }
  parallelAnimGroup->addAnimation(translationAnimGroup);

  // NOTE: see rebounds stuff when it will be implemented
  // if prop is a ball simply return translation
  if (m_propType == ball)
    return parallelAnimGroup;

  // else we must rotate prop
  else
  {
    // set numbers of rotations on (x)
    int rotCount = (int)(t_launch / 2);

    // now set rotations
    float initialRotX = m_prop->getRotX(); // NOTE: not sure of that
    // maybe we have to store the rotX in an argument...

    float addRotX = 0;
    // if passing launch add some rot
    if (t_jugglerIdLaunch != t_jugglerIdRecieve)
      addRotX = ((initialRotX - 90.0f) * 2.0f);

    // adjust values if necessary
    // TODO: faire des if, plus simple
    switch (m_propType)
    {
    default: break;
    case ring:
      switch (m_launchTypeRing)
      {
      default: break;
      case normalRing:
        rotCount = 0;
        addRotX = 0;
        break;
      case panCake:
        break;
      }
      break;

    case club:
      switch (m_launchTypeClub)
      {
      default: break;
      case normalClub:
        break;

      case flat:
        rotCount = 0;
        // if passing launch, change rotx
        if (t_jugglerIdLaunch != t_jugglerIdRecieve)
          addRotX = ((-180 - ((initialRotX - 90.0f) * 2.0f)));
        break;

      case helicopter:
        rotCount = 0;
        addRotX = 0;
        break;
      }

      break;
    }

    // rotation X
    auto animRotX = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"), this);
    animRotX->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
    animRotX->setStartValue((360 * rotCount) + initialRotX);
//    animRotX->setStartValue(720);
    animRotX->setEndValue(initialRotX + addRotX);
//    animRotX->setEndValue(0);
    animRotX->setLoopCount(ONE_LOOP);
    parallelAnimGroup->addAnimation(animRotX);

    // finally return anim
    return parallelAnimGroup;
  }

}

QSequentialAnimationGroup *PropAnim::dwellAnim(int t_jugglerId,
                                               int t_nextLaunch,
                                               hand t_hand)
{

}

