#ifndef QJOEPASS_H
#define QJOEPASS_H

/**************************** 3D definitions ****************************/

// QMaterial shader names
#define DIFFUSE_COLOR "kd"
#define AMBIANT_COLOR "ka"
#define SPECULAR_COLOR "ks"
#define SHININESS "shininess"
// Juggler
#define JUGGLER_SCALE 1.6f
#define JUGGLER_ROT_X 0
#define JUGGLER_ROT_Z 0
#define JUGGLER_TRANSLATION_Y -4
#define SKELETON_MESH_SRC "qrc:/models/juggler_.qgltf"
//#define SKELETON_MESH_SRC "qrc:/models/juggler01.gltf"
#define SKELETON_SRC "qrc:/models/juggler01.gltf"
//#define SKELETON_MESH_SRC "file:/home/ray/dev/qt_exemples/exampleresources/assets/gltf/2.0/RiggedSimple/RiggedSimple.gltf"
//#define SKELETON_SRC "file:/home/ray/dev/qt_exemples/exampleresources/assets/gltf/2.0/RiggedSimple/RiggedSimple.gltf"
//#define SKELETON_MESH_SRC "file:/home/ray/Data/dev/qt3d-examples-master/resources/assets/gltf/2.0/Robot/robot.gltf"
//#define SKELETON_SRC "file:/home/ray/Data/dev/qt3d-examples-master/resources/assets/gltf/2.0/Robot/robot.gltf"

#define JUGGLER_SHININESS 50
// Ground
#define GROUND_WIDTH 20
#define GROUND_HEIGHT 20
#define GROUND_POSY -4
#define GROUND_SHININESS 10
// Ball
#define BALL_RINGS 20
#define BALL_SLICES 20
#define BALL_RADIUS 2
#define BALL_SCALE 0.1f
#define BALL_SHININESS 50
// Ring
#define RING_RADIUS 1.0f
#define RING_MINOR_RADIUS 0.114f
#define RING_RING_NUMBER 100
#define RING_SLICE_NUMBER 20
#define RING_SCALE_X 0.8
#define RING_SCALE_Y 0.8
#define RING_SCALE_Z 0.1
#define RING_SHININESS 50
// Club
#define CLUB_SCALE 0.05
#define CLUB_MESH_SRC "qrc:/models/J_club2.qgltf"
#define CLUB_SHININESS 50
// Light
#define LIGHT_COLOR 0xFFFFFF
#define LIGHT_INTENSITY 2
// anim
#define DELTA_TIME 0.03333f

#endif // QJOEPASS_H