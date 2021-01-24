#include "jugglingclub.h"


JugglingClub::JugglingClub(QEntity *t_rootEntity,
                           QMesh *t_clubMesh,
                           QColor &t_color,
                           launchTypeClub t_launchType)
  : JugglingProp(t_rootEntity, t_color),
    m_launchType(t_launchType)
{
  m_propTransform->setScale(CLUB_SCALE);
  addComponent(t_clubMesh);
}

void JugglingClub::setRotX(float t_rotX)
{
  if (m_rotX == t_rotX)
    return;

  m_rotX = t_rotX;
  emit rotXChanged(m_rotX);
  updateRotX();
}

void JugglingClub::setRotY(float t_rotY)
{
  if (m_rotY == t_rotY)
    return;

  m_rotY = t_rotY;
  emit rotYChanged(m_rotY);
  updateRotY();
}

void JugglingClub::setRotZ(float t_rotZ)
{
  if (m_rotZ == t_rotZ)
    return;

  m_rotZ = t_rotZ;
  emit rotZChanged(m_rotZ);
  updateRotZ();
}

void JugglingClub::updateRotX()
{
  m_propTransform->setRotationX(m_rotX);
}

void JugglingClub::updateRotY()
{
  m_propTransform->setRotationY(m_rotY);
}

void JugglingClub::updateRotZ()
{
  m_propTransform->setRotationZ(m_rotZ);
}
