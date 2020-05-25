#include "my3dwindow.h"

My3DWindow::My3DWindow()
{
  // Root entity
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

  // Camera
  Qt3DRender::QCamera *cameraEntity = camera();

  cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
  cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));

  Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
  light->setColor("white");
  light->setIntensity(1);
  lightEntity->addComponent(light);
  Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
  lightTransform->setTranslation(cameraEntity->position());
  lightEntity->addComponent(lightTransform);

  // For camera controls
  Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camController->setCamera(cameraEntity);

  // Set root object of the scene
  setRootEntity(rootEntity);

  // Torus shape data
  //! [0]
  m_torus = new Qt3DExtras::QTorusMesh();
  m_torus->setRadius(1.0f);
  m_torus->setMinorRadius(0.4f);
  m_torus->setRings(100);
  m_torus->setSlices(20);
  //! [0]

  // TorusMesh Transform
  //! [1]
  Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform();
  torusTransform->setScale(2.0f);
  torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 25.0f));
  torusTransform->setTranslation(QVector3D(5.0f, 4.0f, 0.0f));
  //! [1]

  //! [2]
  torusMaterial = new Qt3DExtras::QPhongMaterial();
  torusMaterial->setDiffuse(QColor(QRgb(0xbeb32b)));
  //! [2]

  // Torus
  //! [3]
  m_torusEntity = new Qt3DCore::QEntity(rootEntity);
  m_torusEntity->addComponent(m_torus);
  m_torusEntity->addComponent(torusMaterial);
  m_torusEntity->addComponent(torusTransform);
  //! [3]

  // Cone shape data
  Qt3DExtras::QConeMesh *cone = new Qt3DExtras::QConeMesh();
  cone->setTopRadius(0.5);
  cone->setBottomRadius(1);
  cone->setLength(3);
  cone->setRings(50);
  cone->setSlices(20);

  // ConeMesh Transform
  Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
  coneTransform->setScale(1.5f);
  coneTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
  coneTransform->setTranslation(QVector3D(0.0f, 4.0f, -1.5));

  coneMaterial = new Qt3DExtras::QPhongMaterial();
  coneMaterial->setDiffuse(QColor(QRgb(0x928327)));

  // Cone
  m_coneEntity = new Qt3DCore::QEntity(rootEntity);
  m_coneEntity->addComponent(cone);
  m_coneEntity->addComponent(coneMaterial);
  m_coneEntity->addComponent(coneTransform);

  // Cylinder shape data
  Qt3DExtras::QCylinderMesh *cylinder = new Qt3DExtras::QCylinderMesh();
  cylinder->setRadius(1);
  cylinder->setLength(3);
  cylinder->setRings(100);
  cylinder->setSlices(20);

  // CylinderMesh Transform
  Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform();
  cylinderTransform->setScale(1.5f);
  cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
  cylinderTransform->setTranslation(QVector3D(-5.0f, 4.0f, -1.5));

  cylinderMaterial = new Qt3DExtras::QPhongMaterial();
  cylinderMaterial->setDiffuse(QColor(QRgb(0x928327)));

  // Cylinder
  m_cylinderEntity = new Qt3DCore::QEntity(rootEntity);
  m_cylinderEntity->addComponent(cylinder);
  m_cylinderEntity->addComponent(cylinderMaterial);
  m_cylinderEntity->addComponent(cylinderTransform);

  // Cuboid shape data
  Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();

  // CuboidMesh Transform
  Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
  cuboidTransform->setScale(4.0f);
  cuboidTransform->setTranslation(QVector3D(5.0f, -4.0f, 0.0f));

  cuboidMaterial = new Qt3DExtras::QPhongMaterial();
  cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

  //Cuboid
  m_cuboidEntity = new Qt3DCore::QEntity(rootEntity);
  m_cuboidEntity->addComponent(cuboid);
  m_cuboidEntity->addComponent(cuboidMaterial);
  m_cuboidEntity->addComponent(cuboidTransform);

  // Plane shape data
  Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
  planeMesh->setWidth(2);
  planeMesh->setHeight(2);

  // Plane mesh transform
  Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
  planeTransform->setScale(1.3f);
  planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
  planeTransform->setTranslation(QVector3D(0.0f, -4.0f, 0.0f));

  planeMaterial = new Qt3DExtras::QPhongMaterial();
  planeMaterial->setDiffuse(QColor(QRgb(0xa69929)));

  // Plane
  m_planeEntity = new Qt3DCore::QEntity(rootEntity);
  m_planeEntity->addComponent(planeMesh);
  m_planeEntity->addComponent(planeMaterial);
  m_planeEntity->addComponent(planeTransform);

  // Sphere shape data
  Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
  sphereMesh->setRings(20);
  sphereMesh->setSlices(20);
  sphereMesh->setRadius(2);

  // Sphere mesh transform
  Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

  sphereTransform->setScale(1.3f);
  sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

  sphereMaterial = new Qt3DExtras::QPhongMaterial();
  sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

  // Sphere
  m_sphereEntity = new Qt3DCore::QEntity(rootEntity);
  m_sphereEntity->addComponent(sphereMesh);
  m_sphereEntity->addComponent(sphereMaterial);
  m_sphereEntity->addComponent(sphereTransform);

  m_torusEntity->setEnabled(enabled);
  m_coneEntity->setEnabled(enabled);
  m_cuboidEntity->setEnabled(enabled);
  m_planeEntity->setEnabled(enabled);
  m_sphereEntity->setEnabled(enabled);

}

void My3DWindow::changeBackground(QColor aColor)
{
  sphereMaterial->setDiffuse(aColor);
  planeMaterial->setDiffuse(aColor);
  cuboidMaterial->setDiffuse(aColor);
  cylinderMaterial->setDiffuse(aColor);
  coneMaterial->setDiffuse(aColor);
  torusMaterial->setDiffuse(aColor);
}
