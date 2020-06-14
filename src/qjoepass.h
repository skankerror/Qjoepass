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

/**************************** App ***************************************/

#define APP_ICON "://res/images/icone.png"
#define WINDOW_MINIMUM_W 160
#define WINDOW_MINIMUM_H 160
#define WINDOW_W 1024
#define WINDOW_H 768
#define WIDGET3D_MIN_W 800
#define WIDGET3D_MIN_H 600

/**************************** 3D definitions ****************************/

// camera
#define CAM_FOV 65.0f
#define CAM_RATIO 16.0f/9.0f
#define CAM_NEARPLANE 0.1f
#define CAM_FARPLANE 1000.0f
#define CAM_INITIAL_POSITION QVector3D(0,10,15)
#define CAM_UP_VECTOR QVector3D(0,1,0)
#define CAM_ORBIT_VIEW_CENTER QVector3D(0,0,0)
// SkyBox
#define SKYBOX_BASE_NAME "qrc:/skybox/res/images/skybox/night"
#define SKYBOX_EXTENSION ".jpg"
#define SKYBOX_SCALE 10.0F
// Juggler
#define JUGGLER_SCALE 0.8f
#define JUGGLER_ROT_X 0
#define JUGGLER_ROT_Z 0
#define JUGGLER_TRANSLATION_Y -4
#define HAND_OFFSET_X 0.65
#define HAND_OFFSET_Y 3.75
#define HAND_OFFSET_Z 1.3
#define HAND_OFFSET_EXT 0.65
#define HAND_OFFSET_EXT_PLUS 1.5f // helico pancakes
#define HAND_OFFSET_INT 0.3
#define HEAD_POS_Y 1.8f
#define JUGGLER_METALNESS 0.8
#define JUGGLER_ROUGHNESS 0.5
// Ground
#define GROUND_WIDTH 20
#define GROUND_HEIGHT 20
#define GROUND_POSY -4
#define GROUND_AMBIENT_OCCLUSION "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_ambient_occlusion.png"
#define GROUND_ROUGHNESS "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_roughness.png"
#define GROUND_BASE_COLOR "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_basecolor.png"
#define GROUND_NORMAL "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_normal.png"
#define GROUND_METALNESS "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_metallic.png"
// Prop
#define PROP_METALNESS 0.2
#define PROP_ROUGHNESS 0.5
// Ball
#define BALL_RINGS 20
#define BALL_SLICES 20
#define BALL_RADIUS 2
#define BALL_SCALE 0.1f
// Ring
#define RING_RADIUS 1.0f
#define RING_MINOR_RADIUS 0.114f
#define RING_RING_NUMBER 100
#define RING_SLICE_NUMBER 20
#define RING_SCALE_X 0.6
#define RING_SCALE_Y 0.6
#define RING_SCALE_Z 0.1
#define RING_BASIC_ROTY 90
#define RING_PANCAKE_ROTX 100
// Club
#define CLUB_SCALE 0.04
#define CLUB_MESH_SRC "qrc:/mesh/res/mesh/J_club3.obj"
#define CLUB_BASIC_ROTX 110
#define CLUB_HELICOPTER_ROTX 90
// Light
#define LIGHT_COLOR 0xFFFFFF
#define LIGHT_INTENSITY 1
#define IRRADIANCE "qrc:/cubemaps/res/cubemaps/default_irradiance.dds"
#define SPECULAR "qrc:/cubemaps/res/cubemaps/default_specular.dds"

/******************************** Animation *****************************************/

#define SCALE_FACTOR 3.28f // for gravity... size of juggler / normal man size (5.9/1.8)
#define DELTA_TIME 0.015f // in second
#define HAND_PERIOD 0.7f // in second
#define DWELL_RATIO 0.68f
#define LAUNCH1_TIME 0.05f // in second. Must be < HAND_PERIOD/2
#define GRAVITY QVector3D(0,-9.8f*SCALE_FACTOR,0)
#define DWELL_TIME DWELL_RATIO*HAND_PERIOD
#define EMPTY_TIME HAND_PERIOD-DWELL_TIME
#define S_TO_MS 1000.0f
const float DWELL_TIME_LAUNCH1 = (HAND_PERIOD / 2.0f) - LAUNCH1_TIME;

/*********************************** Enums *******************************************/

enum hand{leftHand, rightHand};
enum jugglingProp{ball, ring, club, propNumb};
enum launchTypeBall{normalBall, bounce};
enum launchTypeRing{normalRing, panCake};
enum launchTypeClub{normalClub, flat, helicopter};

#endif // QJOEPASS_H
