#include "juggler.h"
#include <QDebug>

Juggler::Juggler(QEntity *aRootEntity,
                 QSkeletonLoader *aSkeleton,
                 QMesh *aSkeletonMesh,
                 QEffect *aEffect,
                 float &aRoty,
                 QVector2D &aPosition,
                 QColor &aColor)
  :/*skeleton(aSkeleton),*/
    skeletonMaterial(new QMaterial()),
    diffuseColorParameter(new QParameter()),
    shininessParameter(new QParameter()),
    skeletonTransform(new Qt3DCore::QTransform()),
    skeletonArmature(new QArmature()),
    color(aColor)
{
  QSkeleton *skelet = new QSkeleton();

  auto matRJ = QMatrix4x4();
  matRJ.setToIdentity();
//  ibMatrix *= 5;
  rootJoint = new QJoint();
  rootJoint->setInverseBindMatrix(matRJ);
  rootJoint->setScale(QVector3D(0.4, 0.4, 0.4));
  rootJoint->setRotation(QQuaternion(0, 0, -7, -7));
  rootJoint->setTranslation(QVector3D(0, 3, 0));

  QJoint *epauleL = new QJoint();// bone0 joint5
  auto matEpauleL = QMatrix4x4(-1, -2.5, 0, 7.8,
                               2.5, 0, 0, 0,
                               0, 0, 2.5, 0,
                               0, 0, 0, 1);
  epauleL->setInverseBindMatrix(matEpauleL);
//  epauleL->setRotation(QQuaternion(1, 0, 0, 0));
  epauleL->setTranslation(QVector3D(1, 0, 0));
  rootJoint->addChildJoint(epauleL);

  QJoint *coudeL = new QJoint();//bone4 joint4
  auto matCoudeL = QMatrix4x4(-2.5, 0, 0, 1,
                              0, -2.5, 0, 7.8,
                               0, 0, 2.5, 0,
                              0, 0, 0, 1);
  coudeL->setInverseBindMatrix(matCoudeL);
  coudeL->setRotation(QQuaternion(-5.4, 4, -0.7, -0.7));
  coudeL->setTranslation(QVector3D(0, -1, 0));
  epauleL->addChildJoint(coudeL);

  QJoint *poigneeL = new QJoint();//bone5 joint3
  auto matPoigneeL = QMatrix4x4(-2.5, 0, 0, 1,
                                0, 0, 2.5, 0,
                                0, 2.5, 0, -5.6,
                                0, 0, 0, 1);
  poigneeL->setInverseBindMatrix(matPoigneeL);
  poigneeL->setRotation(QQuaternion(0.7, -4, -5.3, 0.7));
  poigneeL->setTranslation(QVector3D(0, 0, 1));
  coudeL->addChildJoint(poigneeL);

  skelet->setRootJoint(rootJoint);

  //skeleton Transform
  skeletonTransform->setScale(JUGGLER_SCALE);
  eulerAngles = QVector3D(JUGGLER_ROT_X, aRoty, JUGGLER_ROT_Z);
  skeletonTransform->setRotation(QQuaternion::fromEulerAngles(eulerAngles));
  m_position = QVector3D(aPosition.x(), JUGGLER_TRANSLATION_Y, aPosition.y());
  skeletonTransform->setTranslation(m_position);
  //skeletonArmature
  skeletonArmature->setSkeleton(skelet);
  //skeletonMaterial
  diffuseColorParameter->setName(QLatin1String(DIFFUSE_COLOR));
  shininessParameter->setName(QLatin1String(SHININESS));
  skeletonMaterial->addParameter(diffuseColorParameter);
  diffuseColorParameter->setValue(QVariant::fromValue(color));
  skeletonMaterial->addParameter(shininessParameter);
  shininessParameter->setValue(QVariant::fromValue(JUGGLER_SHININESS));
  skeletonMaterial->setEffect(aEffect);

  qDebug() << rootJoint;
  qDebug() << skelet->rootJoint();
  qDebug() << skelet->jointCount();
  qDebug() << skelet->rootJoint()->childJoints();

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
  addComponent(aSkeletonMesh);
  addComponent(skeletonArmature);
  addComponent(skeletonMaterial);
  setEnabled(enabled);

  qDebug() << skelet->jointCount();

  QSkeletonMapping *skeletonMapping = new QSkeletonMapping(this);
  skeletonMapping->setSkeleton(skelet);
  auto channelMapper = new QChannelMapper(this);
  channelMapper->addMapping(skeletonMapping);
  qDebug() << channelMapper->mappings();

  auto animator = new QClipAnimator(this);
  auto clip = new QAnimationClipLoader(QUrl("file://home/ray/Data/dev/qt3d-5.14.2/tests/manual/skinned-mesh/jump.json"));
//  auto clipData = QAnimationClipData();
//  auto channel = QChannel("channelTest");
//  auto channelComponent = QChannelComponent("channelComponentTest");
//  channelComponent.appendKeyFrame(QKeyFrame({0.0f, 0.0f}, {-1.0f, 0.0f}, {1.0f, 0.0f}));
//  channelComponent.appendKeyFrame(QKeyFrame({2.45f, 5.0f}, {1.45f, 5.0f}, {3.45f, 5.0f}));
//  channel.appendChannelComponent(channelComponent);
//  clipData.appendChannel(channel);
//  clip->setClipData(clipData);

  auto clock = new QClock();
  clock->setPlaybackRate(1);
  animator->setClock(clock);
  animator->setClip(clip);
  animator->setChannelMapper(channelMapper);
  animator->setLoopCount(-1);
  animator->setRunning(true);
  qDebug() << "skeletMap jointCount" << skeletonMapping->skeleton()->jointCount();
}

void Juggler::setPosition(QVector3D aPosition)
{
  if (m_position == aPosition)
    return;

  m_position = aPosition;
  emit positionChanged(m_position);
  updateTransform();

}

void Juggler::updateTransform()
{
  skeletonTransform->setTranslation(m_position);
}
