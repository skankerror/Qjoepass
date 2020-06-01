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
  rootJoint = new QJoint();
  rootJoint->setToIdentity();
  rootJoint->setScale(QVector3D(3, 3, 3));
  rootJoint->setInverseBindMatrix(QMatrix4x4());
  rootJoint->setRotationX(42);
  rootJoint->setToIdentity();
  QJoint *childJoint1 = new QJoint();
  childJoint1->setToIdentity();
  childJoint1->setTranslation(QVector3D(3, 3, 3));
  QJoint *childJoint2 = new QJoint();
  childJoint2->setToIdentity();
  childJoint2->setTranslation(QVector3D(0, -4, 0));
  rootJoint->addChildJoint(childJoint1);
  rootJoint->addChildJoint(childJoint2);
  skelet->setRootJoint(rootJoint);

  //  QSkeletonMapping *skeletMapping = new QSkeletonMapping();
  //  skeletMapping->setSkeleton(skeleton);
  //  qDebug() << skelet->jointCount();
  //  qDebug() << skelet->jointCount();

    skeleton = new QSkeletonLoader(this);
    skeleton->setCreateJointsEnabled(true);
  ////  skeleton->setEnabled(true);
  ////  skeleton->setRootJoint(rootJoint);
    skeleton->setSource(QUrl(SKELETON_SRC));



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

  rootJoint = skelet->rootJoint();
  //  rootJoint->setRotationX(10);
  //  QJoint *testJoint = new QJoint();
  //  rootJoint->addChildJoint(testJoint);
  //  QJoint *testJoint = rootJoint->childJoints().at(0);
  qDebug() << rootJoint;
  qDebug() << skelet->rootJoint();
  qDebug() << skelet->jointCount();
  qDebug() << skelet->rootJoint()->childJoints();

  qDebug() << skeleton->rootJoint();
  qDebug() << skeleton->jointCount();
//  qDebug() << skeleton->rootJoint()->childJoints();


//  qDebug() << skeletonArmature->skeleton()->jointCount();

  QEntity::setParent(aRootEntity);
  addComponent(skeletonTransform);
  addComponent(aSkeletonMesh);
  addComponent(skeletonArmature);
  addComponent(skeletonMaterial);
  setEnabled(enabled);


  //  QSkeletonMapping *skeletonMapping = new QSkeletonMapping();
  //  skeletonMapping->setSkeleton(skelet);
  //    QChannelMapper *m_channelMapper = new QChannelMapper;
  //    m_channelMapper->addMapping(skeletonMapping);

  rootJoint->setRotationZ(57);
  childJoint1->setScale(QVector3D(10, 1, 20));

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
