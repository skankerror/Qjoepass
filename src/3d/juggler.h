/*
 * (c) 2020 Pat Co / M. C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JUGGLER_H
#define JUGGLER_H

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include "qjoepass.h"

using namespace Qt3DCore;
using namespace Qt3DExtras;
using namespace Qt3DRender;

class Juggler: public QEntity
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position
             READ position
             WRITE setPosition
             NOTIFY positionChanged)
  Q_PROPERTY(QVector2D leftHandPosition
             READ getLeftHandPosition
             WRITE setLeftHandPosition)
  Q_PROPERTY(QVector2D rightHandPosition
             READ getRightHandPosition
             WRITE setRightHandPosition)
  Q_PROPERTY(QVector3D handPosition
             READ handPosition
             WRITE setHandPosition)
public:

  explicit Juggler(QEntity *aRootEntity,
                   float &aRoty,
                   QVector2D &aPosition,
                   QColor &aColor);

  QVector3D position() const {return m_position;}
  Qt3DCore::QTransform * getSkeletonTransform() const {return skeletonTransform;};
  void setPosition(QVector3D aPosition);
  void setSkeletonTransform(Qt3DCore::QTransform *aTransform) {skeletonTransform = aTransform;};

  QVector2D getLeftHandPosition() {return leftHandPosition;};
  void setLeftHandPosition(QVector2D rot);
  QVector2D getRightHandPosition() {return rightHandPosition;};
  void setRightHandPosition(QVector2D rot);
  QVector3D handPosition() {return myHandPosition;};
  void setHandPosition(QVector3D rot);

  // getters for hands positions ext to catch, int to launch, med for siteswap 2
  QVector3D getPositionLHextPlus() const {return posLHextPlus;}; // helico, pancakes
  QVector3D getPositionLHext() const {return posLHext;};
  QVector3D getPositionLHint() const {return posLHint;};
  QVector3D getPositionLHmed() const {return posLHmed;};
  QVector3D getPositionRHextPlus() const {return posRHextPlus;}; // helico, pancakes
  QVector3D getPositionRHext() const {return posRHext;};
  QVector3D getPositionRHint() const {return posRHint;};
  QVector3D getPositionRHmed() const {return posRHmed;};
  // getters for camera position
  QVector3D getPositionHead() const {return posHead;};
  QVector3D getLookAt() const {return headLookAt;};

  float getRotY() const {return eulerAngles.y();};

private:

  void createHead();
  void createShoulders();
  void createArms();
  void createElbows();
  void createForearms();
  void createTrunk();
  void createThighs();
  void createKnees();
  void createLegs();

  void updateTransform();

  QMatrix4x4 getRotMatrix();
  // setters for hands pos
  void setPositionLHextPlus(); // for helico, pancakes
  void setPositionLHext();
  void setPositionLHint();
  void setPositionLHmed();
  void setPositionRHextPlus(); // for helico, pancakes
  void setPositionRHext();
  void setPositionRHint();
  void setPositionRHmed();
  void setPosHead();

  void makeMember(QCylinderMesh *aMember,
                  Qt3DCore::QTransform *aMemberTransform,
                  QEntity *aMemberEntity,
                  QVector3D aRot,
                  QVector3D aTrans,
                  float globalRotation,
                  float length);

  void makeArticulation(QSphereMesh *aSphere,
                        Qt3DCore::QTransform *aSphereTransform,
                        QEntity *aSphereEntity,
                        QVector3D aTrans,
                        float globalRotation);


private slots:

  void setPositionHands();

signals:

  void skeletonTransformChanged();
  void positionChanged();

private:

  QMetalRoughMaterial *jugglerMetalRoughMaterial;

  Qt3DCore::QTransform *skeletonTransform;

  QVector3D eulerAngles;
  QVector3D m_position;
  QColor color;
  float rotY;

  bool enabled = true;

  // handPos
  QVector3D posLHextPlus; // helico pancake
  QVector3D posLHext;
  QVector3D posLHint;
  QVector3D posLHmed;
  QVector3D posRHextPlus; // helico pancake
  QVector3D posRHext;
  QVector3D posRHint;
  QVector3D posRHmed;
  // headPos
  QVector3D posHead;
  QVector3D headLookAt;


  QVector2D leftHandPosition;
  QVector2D rightHandPosition;
  QVector3D myHandPosition;


  QSkeleton *mySkeleton;
  QArmature *myArmature;
  QJoint *rootJoint;

  QEntity *aHeadEntity;
  QSphereMesh *Head;
  Qt3DCore::QTransform *headTransform;
  QJoint *headJoint;

  QEntity *aShoulderEntity;
  QCylinderMesh *Shoulders;
  Qt3DCore::QTransform *shouldersTransform;

  QEntity *aRightShoulderEntity;
  QSphereMesh *RightShoulder;
  Qt3DCore::QTransform *rightShoulderTransform;

  QEntity *aLeftShoulderEntity;
  QSphereMesh *LeftShoulder;
  Qt3DCore::QTransform *leftShoulderTransform;

  QEntity *aLeftArmEntity;
  QCylinderMesh *LeftArm;
  Qt3DCore::QTransform *leftArmTransform;
  QMatrix4x4 leftArmMatrix;

  QEntity *aRightArmEntity;
  QCylinderMesh *RightArm;
  Qt3DCore::QTransform *rightArmTransform;
  QMatrix4x4 rightArmMatrix;

  QEntity *aRightElbowEntity;
  QSphereMesh *RightElbow;
  Qt3DCore::QTransform *rightElbowTransform;

  QEntity *aLeftElbowEntity;
  QSphereMesh *LeftElbow;
  Qt3DCore::QTransform *leftElbowTransform;

  QEntity *aLeftForearmEntity;
  QCylinderMesh *LeftForearm;
  Qt3DCore::QTransform *leftForearmTransform;
  QMatrix4x4 leftForearmMatrix;

  QEntity *aRightForearmEntity;
  QCylinderMesh *RightForearm;
  Qt3DCore::QTransform *rightForearmTransform;
  QMatrix4x4 rightForearmMatrix;

  QEntity *aTrunkEntity;
  QCylinderMesh *Trunk;
  Qt3DCore::QTransform *trunkTransform;

  QEntity *aLeftThighEntity;
  QCylinderMesh *LeftThigh;
  Qt3DCore::QTransform *leftThighTransform;

  QEntity *aRightThighEntity;
  QCylinderMesh *RightThigh;
  Qt3DCore::QTransform *rightThighTransform;

  QEntity *aRightKneeEntity;
  QSphereMesh *RightKnee;
  Qt3DCore::QTransform *rightKneeTransform;

  QEntity *aLeftKneeEntity;
  QSphereMesh *LeftKnee;
  Qt3DCore::QTransform *leftKneeTransform;

  QEntity *aRightLegEntity;
  QCylinderMesh *RightLeg;
  Qt3DCore::QTransform *rightLegTransform;

  QEntity *aLeftLegEntity;
  QCylinderMesh *LeftLeg;
  Qt3DCore::QTransform *leftLegTransform;

};

#endif // JUGGLER_H
