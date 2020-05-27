#include "my3dwindow.h"

My3DWindow::My3DWindow(MySettings *aSettings)
  :settings(aSettings)
{
  //background
  QColor colorBG = settings->value("world/colorbg").value<QColor>();
  defaultFrameGraph()->setClearColor(colorBG);

  // Root entity
  rootEntity = new Qt3DCore::QEntity();

  // Camera
  m_camera = camera();
  m_camera->lens()->setPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  m_camera->setPosition(QVector3D(0, 0, 20.0f));
  m_camera->setUpVector(QVector3D(0, 1, 0));
  m_camera->setViewCenter(QVector3D(0, 0, 0));
  // For camera controls
  camFPController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camOController = new Qt3DExtras::QOrbitCameraController(rootEntity);
  camOController->setCamera(m_camera);

  //light
  lightEntity = new Qt3DCore::QEntity(rootEntity);
  light = new Qt3DRender::QPointLight(lightEntity);
  light->setColor(QColor(QRgb(0xFFEFE1)));
  light->setIntensity(2);
  lightEntity->addComponent(light);
  lightTransform = new Qt3DCore::QTransform(lightEntity);
  lightTransform->setTranslation(QVector3D(-20, 10, 20));
  lightEntity->addComponent(lightTransform);

  //light2
  lightEntity2 = new Qt3DCore::QEntity(rootEntity);
  light2 = new Qt3DRender::QPointLight(lightEntity2);
  light2->setColor(QColor(QRgb(0xD0E1FF)));
  light2->setIntensity(1);
  lightEntity2->addComponent(light2);
  lightTransform2 = new Qt3DCore::QTransform(lightEntity2);
  lightTransform2->setTranslation(QVector3D(20, 10, 20));
  lightEntity2->addComponent(lightTransform2);

  //light3
  lightEntity3 = new Qt3DCore::QEntity(rootEntity);
  light3 = new Qt3DRender::QPointLight(lightEntity3);
  light3->setColor(QColor(QRgb(0xFFFFFF)));
  light3->setIntensity(3);
  lightEntity3->addComponent(light3);
  lightTransform3 = new Qt3DCore::QTransform(lightEntity3);
  lightTransform3->setTranslation(QVector3D(0, 10, -20));
  lightEntity3->addComponent(lightTransform3);

  // Set root object of the scene
  setRootEntity(rootEntity);

  // Plane shape data
  planeMesh = new Qt3DExtras::QPlaneMesh();
  planeMesh->setWidth(2);
  planeMesh->setHeight(2);
  // Plane mesh transform
  planeTransform = new Qt3DCore::QTransform();
  planeTransform->setScale(10.0f);
  planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
  planeTransform->setTranslation(QVector3D(0.0f, -4.0f, 0.0f));
  // Plane Material
  planeMaterial = new Qt3DExtras::QPhongMaterial();
  planeMaterial->setDiffuse(QColor(QRgb(0x301510)));
  // Plane
  planeEntity = new Qt3DCore::QEntity(rootEntity);
  planeEntity->addComponent(planeMesh);
  planeEntity->addComponent(planeMaterial);
  planeEntity->addComponent(planeTransform);
  planeEntity->setEnabled(true);

  //skeleton Transform
  skeletonTransform = new Qt3DCore::QTransform();
  skeletonTransform->setScale(3.0f);
  skeletonTransform->setRotation(QQuaternion::fromEulerAngles(QVector3D(0, 90, 0)));
  skeletonTransform->setTranslation(QVector3D(-7, -4, 0));
  //skeletonMesh
  skeletonMesh = new Qt3DRender::QMesh();
//  skeletonMesh->setSource(QUrl::fromLocalFile(SKELETON_MODEL));
  skeletonMesh->setSource(QUrl("qrc:/models/juggler.qgltf"));
  //skeleton
  skeleton = new Qt3DCore::QSkeletonLoader();
  skeleton->setSource(QUrl::fromLocalFile(SKELETON_MODEL));
  //skeletonArmature
  skeletonArmature = new Qt3DCore::QArmature();
  skeletonArmature->setSkeleton(skeleton);
  //skeletonMaterial
  skeletonMaterial = new Qt3DExtras::QPhongMaterial();
  skeletonMaterial->setDiffuse(QColor(QRgb(0x204C9B)));
  //skeletonEntity
  skeletonEntity = new Qt3DCore::QEntity(rootEntity);
  skeletonEntity->addComponent(skeletonTransform);
  skeletonEntity->addComponent(skeletonMesh);
  skeletonEntity->addComponent(skeletonArmature);
  skeletonEntity->addComponent(skeletonMaterial);
  skeletonEntity->setEnabled(true);

  //skeleton Transform
  skeletonTransform2 = new Qt3DCore::QTransform();
  skeletonTransform2->setScale(3.0f);
  skeletonTransform2->setRotation(QQuaternion::fromEulerAngles(QVector3D(0, -90, 0)));
  skeletonTransform2->setTranslation(QVector3D(7, -4, 0));
  //skeletonMesh
  skeletonMesh2 = new Qt3DRender::QMesh();
  skeletonMesh2->setSource(QUrl("qrc:/models/juggler.qgltf"));
  //skeleton
  skeleton2 = new Qt3DCore::QSkeletonLoader();
  skeleton2->setSource(QUrl::fromLocalFile(SKELETON_MODEL));
  //skeletonArmature
  skeletonArmature2 = new Qt3DCore::QArmature();
  skeletonArmature2->setSkeleton(skeleton2);
  //skeletonMaterial
  skeletonMaterial2 = new Qt3DExtras::QPhongMaterial();
  skeletonMaterial2->setDiffuse(QColor(QRgb(0x10561B)));
  //skeletonEntity
  skeletonEntity2 = new Qt3DCore::QEntity(rootEntity);
  skeletonEntity2->addComponent(skeletonTransform2);
  skeletonEntity2->addComponent(skeletonMesh2);
  skeletonEntity2->addComponent(skeletonArmature2);
  skeletonEntity2->addComponent(skeletonMaterial2);
  skeletonEntity2->setEnabled(true);


}

void My3DWindow::changeBackground(QColor aColor)
{
  defaultFrameGraph()->setClearColor(aColor);
}
