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

#ifndef QJOEPASS_H
#define QJOEPASS_H

/**************************** 3D definitions ****************************/

// QMaterial shader names
#define DIFFUSE_COLOR "kd"
#define AMBIANT_COLOR "ka"
#define SPECULAR_COLOR "ks"
#define SHININESS "shininess"
// camera
#define CAM_FOV 65.0f
#define CAM_RATIO 16.0f/9.0f
#define CAM_NEARPLANE 0.1f
#define CAM_FARPLANE 1000.0f
#define CAM_INITIAL_POSITION QVector3D(0,10,15)
#define CAM_UP_VECTOR QVector3D(0,1,0)
#define CAM_ORBIT_VIEW_CENTER QVector3D(0,0,0)
// SkyBox
#define SKYBOX_BASE_NAME "qrc:/skybox/res/images/skybox/skybox"
#define SKYBOX_EXTENSION ".jpg"
#define SKYBOX_SCALE 10.0F
// Juggler
#define JUGGLER_SCALE 0.8f
#define JUGGLER_ROT_X 0
#define JUGGLER_ROT_Z 0
#define JUGGLER_TRANSLATION_Y -4
#define JUGGLER_SHININESS 50
#define HAND_OFFSET_X 0.65
#define HAND_OFFSET_Y 3.75
#define HAND_OFFSET_Z 1.3
#define HAND_OFFSET_EXT 0.65
#define HAND_OFFSET_INT 0.3
#define HEAD_POS_Y 5
// Ground
#define GROUND_WIDTH 20
#define GROUND_HEIGHT 20
#define GROUND_POSY -4
#define GROUND_SHININESS 10
#define AMBIENT_OCCLUSION "qrc:/woodfloor/res/images/WoodFloor/WoodFloor_AmbientOcclusion.jpg"
//#define ROUGHNESS "qrc:/woodfloor/res/images/WoodFloor/WoodFloor_Roughness.jpg"
#define ROUGHNESS 0.8
//#define BASE_COLOR "qrc:/woodfloor/res/images/WoodFloor/WoodFloor_Color.jpg"
#define NORMAL "qrc:/woodfloor/res/images/WoodFloor/WoodFloor_Normal.jpg"
//#define DISPLACEMENT "qrc:/woodfloor/res/images/WoodFloor/WoodFloor_Displacement.jpg"
#define METALNESS 0.2
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
#define CLUB_MESH_SRC "qrc:/mesh/res/mesh/J_club2.obj"
#define CLUB_SHININESS 50
// Light
#define LIGHT_COLOR 0xFFFFFF
#define LIGHT_INTENSITY 2
#define IRRADIANCE "qrc:/cubemaps/res/cubemaps/default_irradiance.dds"
#define SPECULAR "qrc:/cubemaps/res/cubemaps/default_specular.dds"
// anim
#define GRAVITY QVector3D(0, -9.8, 0)
#define DELTA_TIME 0.0166667f // in second
#define TEMPO 0.5f // time between 2 launchs in sec
//#define DWELL_TIME 0.16666667f // in second
#define DWELL_TIME 0.2f // in second
#define DELAY_LAUNCH 300

enum hand{leftHand, rightHand};
enum jugglingProp{ball, ring, club, propNumb};

#endif // QJOEPASS_H
