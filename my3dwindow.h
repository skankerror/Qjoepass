#ifndef MY3DWINDOW_H
#define MY3DWINDOW_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>

class My3DWindow: public Qt3DExtras::Qt3DWindow
{
  Q_OBJECT

public:
  My3DWindow();

public slots:
  void changeBackground(QColor aColor);

private:
  Qt3DExtras::QTorusMesh *m_torus;
  Qt3DCore::QEntity *m_coneEntity;
  Qt3DCore::QEntity *m_cylinderEntity;
  Qt3DCore::QEntity *m_torusEntity;
  Qt3DCore::QEntity *m_cuboidEntity;
  Qt3DCore::QEntity *m_planeEntity;
  Qt3DCore::QEntity *m_sphereEntity;

  Qt3DExtras::QPhongMaterial *sphereMaterial;
  Qt3DExtras::QPhongMaterial *planeMaterial;
  Qt3DExtras::QPhongMaterial *cuboidMaterial;
  Qt3DExtras::QPhongMaterial *cylinderMaterial;
  Qt3DExtras::QPhongMaterial *coneMaterial;
  Qt3DExtras::QPhongMaterial *torusMaterial;

  bool enabled = true;
};

#endif // MY3DWINDOW_H
