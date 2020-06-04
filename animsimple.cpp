#include "animsimple.h"
//#include <qmath.h>

AnimSimple::AnimSimple(Juggler *aJuggler,
                       QVector<JugglingBall *> aVBall,
                       QVector<int> aSiteswap)
  :juggler(aJuggler),
    vBall(aVBall),
    siteswap(aSiteswap),
    siteswapAnimation(new QParallelAnimationGroup())
{
  period = siteswap.size();
  int numProp = aVBall.size();
  // Il faudrait vérifier la validité du site swap

  /*************************** test pour période > 1 *************************/

  for (int i = 0; i < numProp; i++) // pour chaque objet
  {
    int launchPos = i % period;
    int launch = siteswap.at(launchPos);
    hand launchHand;
    auto ball = vBall.at(i);
    auto ballAnim = new QSequentialAnimationGroup(); // pour agréger tout le trajet
    auto ballGlobAnim = new QSequentialAnimationGroup();// nécessaire pour la pause au début

    // on rajoute le delay pour chaque balle
    if (i)
      ballGlobAnim->addPause(DELAY_LAUNCH * i);

    // faire la 1ère anim
    if (i % 2 == 0)
      launchHand = rightHand;
    else
      launchHand = leftHand;
    auto launchAnim = launchBall(juggler, ball, launch, launchHand);
    launchAnim->setLoopCount(1);
    ballAnim->addAnimation(launchAnim);

    // la suite
    // on cherche la suite de la balle dans le siteswap
    int newLaunchPos = (launch + launchPos) % period;
    // on se met sur la main qui a reçu
    // si launch est impair on change de main
    hand newLaunchHand;
    if (launch % 2 == 1)
      newLaunchHand = changeHand(launchHand);
    else
      newLaunchHand = launchHand;
    // si on n'arrive pas sur la même position on continue
    while (newLaunchPos != launchPos)
    {
      int newLaunch = siteswap.at(newLaunchPos);
      auto followLaunchAnim = launchBall(juggler, ball, newLaunch, newLaunchHand);
      followLaunchAnim->setLoopCount(1);
      ballAnim->addAnimation(followLaunchAnim);
      // changer de main ?
      if (newLaunch % 2 == 1)
        newLaunchHand = changeHand(newLaunchHand);
      newLaunchPos = (newLaunch + newLaunchPos) % period;
    }

    // on arrive au début mais si on a changé de main faut refaire
    if (newLaunchHand != launchHand)
    {
      auto backLaunchAnim = launchBall(juggler, ball, launch, newLaunchHand);
      backLaunchAnim->setLoopCount(1);
      ballAnim->addAnimation(backLaunchAnim);
      // la suite
      // on cherche la suite de la balle dans le siteswap
      newLaunchPos = (launch + launchPos) % period;
      if (launch % 2 == 1) // suivant le cas on change de main
        newLaunchHand = changeHand(newLaunchHand);
      // si on n'arrive pas sur la même position on continue
      while (newLaunchPos != launchPos)
      {
        int newLaunch = siteswap.at(newLaunchPos);
        auto backFollowLaunchAnim = launchBall(juggler, ball, newLaunch, newLaunchHand);
        backFollowLaunchAnim->setLoopCount(1);
        ballAnim->addAnimation(backFollowLaunchAnim);
        // changer de main ?
        if (newLaunch % 2 == 1)
          newLaunchHand = changeHand(newLaunchHand);
        newLaunchPos = (newLaunch + newLaunchPos) % period;
      }
    }
    // on rajoute le tout à notre global anim qui contient la pause
    ballGlobAnim->addAnimation(ballAnim);
    ballAnim->setLoopCount(-1);
    siteswapAnimation->addAnimation(ballGlobAnim); // on ajoute à notre anim parallele
  }


  /********************** test pour période 1 **********************************/

  //  int launch = siteswap.at(0);

  //  for (int i = 0; i < numProp; i++)
  //  {
  //    auto ball = vBall.at(i);
  //    auto launchAnim = new QSequentialAnimationGroup(); // anim 1 pour pair et impair
  //    auto ballAnim = new QSequentialAnimationGroup(); // regroupe si impair
  //    auto ballGlobAnim = new QSequentialAnimationGroup();// nécessaire pour la pause au début

  //    // on rajoute le delay pour chaque balle
  //    if (i)
  //      ballGlobAnim->addPause(DELAY_LAUNCH * i);

  //    // voir si i est pair ou impair pour savoir si c'est left ou right
  //    if (i % 2 == 0) // rightHand
  //    {
  //      launchAnim = launchBall(juggler, ball, launch, rightHand);
  //      launchAnim->setLoopCount(1);
  //    }
  //    else // leftHand
  //    {
  //      launchAnim = launchBall(juggler, ball, launch, leftHand);
  //      launchAnim->setLoopCount(1);
  //    }
  //    ballAnim->addAnimation(launchAnim);
  //    // si launch est impair il faut faire un nouveau trajet pour boucler au départ
  //    if (launch % 2 !=0)
  //    {
  //      auto launchAnim2 = new QSequentialAnimationGroup();
  //      if (i % 2 == 0) // si on est parti de la droite on repart de la gauche
  //      {
  //        launchAnim2 = launchBall(juggler, ball, launch, leftHand);
  //        launchAnim2->setLoopCount(1);
  //      }
  //      else // et inversement
  //      {
  //        launchAnim2 = launchBall(juggler, ball, launch, rightHand);
  //        launchAnim2->setLoopCount(1);
  //      }
  //      ballAnim->addAnimation(launchAnim2);
  //    }
  //    ballAnim->setLoopCount(-1);
  //    ballGlobAnim->addAnimation(ballAnim); // permet de coller la pause une seule fois au début
  //    siteswapAnimation->addAnimation(ballGlobAnim); // on ajoute à notre anim parallele
  //  }

}

void AnimSimple::startAnimation()
{
  siteswapAnimation->start();
}

QSequentialAnimationGroup *AnimSimple::launchBall(Juggler *aJuggler,
                                                  JugglingBall *aBall,
                                                  int launch,
                                                  hand aHand)
{
  auto animGroup = new QSequentialAnimationGroup();
  QVector3D posBall; // pos where it starts
  QVector3D posFinal; // pos where it should finish
  hand receiveHand; // recieve hand to calculate curve after catch
  // todo handle 0 launch

  // odd launches
  if (aHand == leftHand && launch % 2 != 0)
  {
    posBall = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
  if (aHand == rightHand && launch % 2 != 0)
  {
    posBall = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }

  // even launches
  if (aHand == leftHand && launch % 2 == 0)
  {
    posBall = aJuggler->getPositionLHint();
    posFinal = aJuggler->getPositionLHext();
    receiveHand = leftHand;
  }
  if (aHand == rightHand && launch % 2 == 0)
  {
    posBall = aJuggler->getPositionRHint();
    posFinal = aJuggler->getPositionRHext();
    receiveHand = rightHand;
  }
  //  qDebug() << posBall;

  float timeLaunch = ((float)(launch) * TEMPO) - DWELL_TIME;
  //  qDebug() << timeLaunch;

  // we calculate velocity launch
  QVector3D velBall = ((posFinal - posBall) - 0.5 *
                       (GRAVITY * timeLaunch * timeLaunch)) / timeLaunch;

  // By counting frames we add 1 due to float to integer approx.
  int frameCount = (int)((timeLaunch / (DELTA_TIME)) + 1);
  //  qDebug() << frameCount;

  // loop creates all our animations for launch
  for (int i = 0; i <= frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
    animBall->setStartValue(posBall);
    QVector3D posBall2 = posBall + (DELTA_TIME * velBall);
    animBall->setEndValue(posBall2);
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
    posBall = posBall2;
    //    qDebug() << posBall;
    velBall = velBall + (DELTA_TIME * GRAVITY);
  }


  // now we handle curve in the hand
  // surely it's the right place to begin
  posBall = posFinal;

  // find out rotation center
  QVector3D centerCurve;
  if (receiveHand == leftHand)
  {
    centerCurve = (posBall + aJuggler->getPositionLHint()) / 2;
    //    qDebug() << posBall;
    //    qDebug() << centerCurve;
    //    qDebug() << aJuggler->getPositionLHint();
  }
  else
  {
    centerCurve = (posBall + aJuggler->getPositionRHint()) / 2;
    //    qDebug() << posBall;
    //    qDebug() << centerCurve;
    //    qDebug() << aJuggler->getPositionRHint();
  }

  // determine axis for rotation
  float rotY = juggler->getRotY();
  QVector3D axisCurve = QVector3D(sinf(rotY), 0, cosf(rotY));

  // determine number of frames
  frameCount = (int)((DWELL_TIME / DELTA_TIME) + 1);

  // determine angles for each delta animation
  float deltaAngles = (float)(180 / frameCount);

  // loop creates all our animations for dwell time
  for (int i = 0; i <= frameCount; i++)
  {
    auto animBall = new QPropertyAnimation(aBall, QByteArrayLiteral("position"));
    animBall->setDuration((int)(DELTA_TIME * 600)); // sould be at 1000... wtf
    animBall->setStartValue(posBall);
    // handle rotation stuff
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.translate(centerCurve);
    if (receiveHand == leftHand)
      rot.rotate(-deltaAngles, axisCurve);
    else
      rot.rotate(deltaAngles, axisCurve);
    rot.translate(-centerCurve);
    QVector3D posBall2 = rot * posBall;

    animBall->setEndValue(posBall2);
    animBall->setLoopCount(1);
    animGroup->addAnimation(animBall);
    posBall = posBall2;
    //    qDebug() << posBall;
  }

  return animGroup;
}

hand AnimSimple::changeHand(hand aHand)
{
  if (aHand == leftHand)
    return rightHand;
  else
    return leftHand;
}
