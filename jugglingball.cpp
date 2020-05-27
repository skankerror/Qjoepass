#include "jugglingball.h"

JugglingBall::JugglingBall(Qt3DCore::QEntity *aRootEntity, QColor aColor)
  :rootEntity(aRootEntity),
    color(aColor)
{
  // Sphere shape data
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(BALL_RINGS);
    sphereMesh->setSlices(BALL_SLICES);
    sphereMesh->setRadius(BALL_RADIUS);

    // Sphere mesh transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(BALL_SCALE);
//    sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

    sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(color);

    Qt3DCore::QEntity::setParent(rootEntity);
    addComponent(sphereMesh);
    addComponent(sphereMaterial);
    addComponent(sphereTransform);
}
