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
//  auto testAnim = parabolicAnim(0,
//                                hand(rightHand),
//                                1,
//                                hand(leftHand),
//                                5);

//  auto testAnim2 = parabolicAnim(1,
//                                 hand(leftHand),
//                                 1,
//                                 hand(rightHand),
//                                 3);

//  auto testAnim3 = parabolicAnim(1,
//                                 hand(rightHand),
//                                 0,
//                                 hand(leftHand),
//                                 5);

//  auto testAnim4 = parabolicAnim(0,
//                                 hand(leftHand),
//                                 0,
//                                 hand(rightHand),
//                                 3);

//  addAnimation(testAnim);
//  addAnimation(testAnim2);
//  addAnimation(testAnim3);
//  addAnimation(testAnim4);
//  setLoopCount(INFINITE_LOOP);

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

  // bool to know if it's a passing launch
  bool isPassing = (t_jugglerIdLaunch != t_jugglerIdRecieve);

  // bool to know if we need to enlarge our juggling
  bool isExtPlusCatch = (m_propType == ring  && m_launchTypeRing == panCake) ||
      (m_propType == club && m_launchTypeClub == helicopter);

  // set position were parabolic curve starts
  if (t_handLaunch == leftHand)
  {
    isPassing ? // is it a passing launch ?
                posProp = jugglerLaunch->getPositionLHmed(): // NOTE: don't know we must set another launch place
        posProp = jugglerLaunch->getPositionLHint();
  }
  else
  {
    isPassing ? // is it a passing launch ?
                posProp = jugglerLaunch->getPositionRHmed(): // NOTE: don't know we must set another launch place
        posProp = jugglerLaunch->getPositionRHint();
  }

  // set position where it ends
  // TODO: il faut bouger l'endroit du départ pour aller dans la direction du recieve
  // dans dwellAnim ?
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
  auto v_parabolic = Curves::curveParabolic(velBall, posProp, frameCount);

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
    int rotXCount = (int)(t_launch / 2);

    // now set rotations
    float initialRotX = m_prop->getRotX(); // NOTE: not sure of that
    // maybe we have to store the rotX in an argument...

    float addRotX = 0;
    // if passing launch add some rot
    if (isPassing)
      addRotX = ((initialRotX - 90.0f) * 2.0f);

    // adjust values if necessary
    // no rotX for normal ring
    if (m_propType == ring &&
        m_launchTypeRing == normalRing)
    {
      rotXCount = 0;
      addRotX = 0;
    }

    // change rotX for normal club
    if (m_propType == club &&
        m_launchTypeClub == normalClub)
    {
      // if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = (90.0f - (3 * initialRotX)) / 2.0f; // - initX - (initX - 90)/2
      else
      {
        initialRotX = - m_prop->getRotX(); // NOTE: not sure of that
        addRotX = -addRotX; // maybe this can be correctly set in dwell
      }
    }

    if (m_propType == club &&
        m_launchTypeClub == flat)
    {
      rotXCount = 0; // no rot it's flat
      // but if passing, club must be vertical in recieving hand
      if (isPassing)
        addRotX = - initialRotX;
    }

    if (m_propType == club &&
        m_launchTypeClub == helicopter)
    {
      int rotYCount = rotXCount;
      float initialRotY = m_prop->getRotY();
      float endValue = jugglerRecieve->getRotY();
      t_handRecieve == leftHand ? // NOTE: doesn't seem totally good
        endValue += 90 - (rotYCount * 360):
          endValue -= 90 - (rotYCount * 360);
      auto animRotY = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotY"), this);
      animRotY->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
      animRotY->setStartValue(initialRotY);
      animRotY->setEndValue(endValue);
      animRotY->setLoopCount(ONE_LOOP);
      parallelAnimGroup->addAnimation(animRotY);

      return parallelAnimGroup;
    }

    // rotation X
    auto animRotX = new QPropertyAnimation(m_prop, QByteArrayLiteral("m_rotX"), this);
    animRotX->setDuration(((int)(DELTA_TIME * S_TO_MS)) * frameCount);
    animRotX->setStartValue((360 * rotXCount) + initialRotX);
    animRotX->setEndValue(initialRotX + addRotX);
    animRotX->setLoopCount(ONE_LOOP);
    parallelAnimGroup->addAnimation(animRotX);

    // finally return anim
    return parallelAnimGroup;
  }
}

QSequentialAnimationGroup *PropAnim::dwellAnim(int t_jugglerIdLaunch,
                                               hand t_handLaunch,
                                               int t_jugglerIdRecieve,
                                               hand t_handRecieve,
                                               int t_launch)
{
  // Voir d'où la main part
  // si c'est pas un passing, c'est simple.
  // Mais bouger sur y pour ring et club
  // si c'est un passing reculer la main, jusqu'à z = 0
  // se diriger dans la direction de la main du reciever
}

