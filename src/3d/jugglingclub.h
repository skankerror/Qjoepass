#ifndef JUGGLINGCLUB_H
#define JUGGLINGCLUB_H

#include "jugglingprop.h"

class JugglingClub : public JugglingProp
{

  Q_OBJECT

  Q_PROPERTY(float m_rotX
             READ getRotX
             WRITE setRotX
             NOTIFY rotXChanged)

  Q_PROPERTY(float m_rotY
             READ getRotY
             WRITE setRotY
             NOTIFY rotYChanged)

  Q_PROPERTY(float m_rotZ
             READ getRotZ
             WRITE setRotZ
             NOTIFY rotZChanged)

public:

  JugglingClub(QEntity *t_rootEntity,
               QMesh *t_clubMesh,
               QColor &t_color,
               launchTypeClub t_launchType = normalClub);

  float getRotX() const { return m_rotX; };
  float getRotY() const { return m_rotY; };
  float getRotZ() const { return m_rotZ; };
  launchTypeClub getLaunchType() const { return m_launchType; };

private:

//  void updateTranslation();
  void updateRotX();
  void updateRotY();
  void updateRotZ();

public slots:

//  void setPosition(QVector3D t_position);
  void setRotX(float t_rotX);
  void setRotY(float t_rotY);
  void setRotZ(float t_rotZ);
  void setLaunchType(launchTypeClub t_launchType) { m_launchType = t_launchType; };

signals:

//  void positionChanged(QVector3D t_position);
  void rotXChanged(float t_rotX);
  void rotYChanged(float t_rotY);
  void rotZChanged(float t_rotZ);

private:

//  QMetalRoughMaterial *m_clubMetalRoughMaterial;
//  Qt3DCore::QTransform *m_pirouetteTransform;
//  QColor m_color;
//  QVector3D m_position;
//  bool m_enabled = true;
  launchTypeClub m_launchType;

  float m_rotX = 0;
  float m_rotY = 0;
  float m_rotZ = 0;


};

#endif // JUGGLINGCLUB_H
