﻿/*
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
#define CAM_INITIAL_POSITION QVector3D(0,15,15)
#define CAM_UP_VECTOR QVector3D(0,1,0)
#define CAM_ORBIT_VIEW_CENTER QVector3D(0,4,0)
// SkyBox
#define SKYBOX_BASE_NAME "qrc:/skybox/res/images/skybox/night"
#define SKYBOX_EXTENSION ".jpg"
#define SKYBOX_SCALE 10.0F
// Juggler
#define JUGGLER_SCALE /*0.8f*/ 1.0f
//#define JUGGLER_ROT_X 0
//#define JUGGLER_ROT_Z 0
#define JUGGLER_TRANSLATION_Y 0
#define HAND_OFFSET_X  1
#define HAND_OFFSET_Y  4.8
#define HAND_OFFSET_Z  1.5
#define HAND_OFFSET_EXT 1
#define HAND_OFFSET_EXT_PLUS 1.5f // helico pancakes
#define HAND_OFFSET_INT 0.5
#define HEAD_POS_Y 7
#define JUGGLER_METALNESS 0.8
#define JUGGLER_ROUGHNESS 0.5
  // Head
  #define HEAD_RADIUS 0.5f
  #define HEAD_RINGS 32
  #define HEAD_SLICES 32
  #define HEAD_TRANSLATE QVector3D(0.0f,7.0f,0.0f)
  // Shoulders
  #define SHOULDERS_ROTATION QVector3D(0.0f,0.0f,90.0f)
  #define SHOULDERS_TRANSLATION QVector3D(0.0f,6.3f,0.0f)
  #define SHOULDERS_LENGHT 2.0f
  #define LEFT_SHOULDER_X 1.0f
  #define RIGHT_SHOULDER_X -1.0f
  #define SHOULDER_Y 6.3f
  #define SHOULDER_Z 0.0f
  #define LEFT_SHOULDER_TRANSLATION QVector3D(LEFT_SHOULDER_X,SHOULDER_Y,SHOULDER_Z)
  #define RIGHT_SHOULDER_TRANSLATION QVector3D(RIGHT_SHOULDER_X,SHOULDER_Y,SHOULDER_Z)
  // Arms
  #define ARM_ROTATION QVector3D(0.0f,0.0f,0.0f)
  #define LEFT_ARM_TRANSLATION QVector3D(1.0f,5.55f,0.0f)
  #define RIGHT_ARM_TRANSLATION QVector3D(-1.0f,5.55f,0.0f)
  #define ARM_LENGHT 1.5f
  // Elbows
  #define LEFT_ELBOW_TRANSLATION QVector3D(1.0f,4.8f,0.0f)
  #define RIGHT_ELBOW_TRANSLATION QVector3D(-1.0f,4.8f,0.0f)
  // Forearms
  #define FOREARM_ROTATION QVector3D(-90.0f,0.0f,0.0f)
  #define LEFT_FOREARM_TRANSLATION QVector3D(1.0f,4.8f,0.75f)
  #define RIGHT_FOREARM_TRANSLATION QVector3D(-1.0f,4.8f,0.75f)
  #define FOREARM_LENGHT 1.5f
  // Trunk
  #define TRUNK_ROTATION QVector3D(0.0f,0.0f,0.0f)
  #define TRUNK_TRANLATION QVector3D(0.0f,5.3f,0.0f)
  #define TRUNK_LENGHT 2.5f
  // Thighs
  #define LEFT_THIGH_ROTATION QVector3D(0.0f,0.0f,13.5f)
  #define LEFT_THIGH_TRANSLATION QVector3D(0.25f,3.05f,0.0f)
  #define RIGHT_THIGH_ROTATION QVector3D(0.0f,0.0f,-13.5f)
  #define RIGHT_THIGH_TRANLATION QVector3D(-0.25f,3.05f,0.0f)
  #define THIGH_LENGHT 2.2f
  // Knees
  #define LEFT_KNEE_TRANLATION QVector3D(0.5f,2.0f,0.0f)
  #define RIGHT_KNEE_TRANLATION QVector3D(-0.5f,2.0f,0.0f)
  // Legs
  #define LEFT_LEG_ROTATION QVector3D(0.0f,0.0f,13.5f)
  #define LEFT_LEG_TRANSLATION QVector3D(0.75f,1.0f,0.0f)
  #define RIGHT_LEG_ROTATION QVector3D(0.0f,0.0f,-13.5f)
  #define RIGHT_LEG_TRANSLATION QVector3D(-0.75f,1.0f,0.0f)
  #define LEG_LENGHT 2.0f
  // Members
  #define MEMBERS_RADIUS 0.1f
  #define MEMBERS_RINGS 8
  #define MEMBERS_SLICES 32
  // Articulations
  #define ARTICULATION_RADIUS 0.1f
  #define ARTICULATION_RINGS 8
  #define ARTICULATION_SLICES 32
// Ground
#define GROUND_WIDTH 20
#define GROUND_HEIGHT 20
#define GROUND_POSY /*-4*/ 0
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
#define LIGHT1_POS QVector3D(-20,15,20)
#define LIGHT2_POS QVector3D(20,15,20)
#define LIGHT3_POS QVector3D(0,15,-20)
#define IRRADIANCE "qrc:/cubemaps/res/cubemaps/default_irradiance.dds"
#define SPECULAR "qrc:/cubemaps/res/cubemaps/default_specular.dds"

/******************************** Animation *****************************************/

#define SCALE_FACTOR 4.03f // for gravity... size of juggler / normal man size (7.25/1.8)
#define DELTA_TIME 0.015f // in second
#define HAND_PERIOD 0.7f // in second
#define DWELL_RATIO 0.6f // seems to be dwell average of a good juggler
#define LAUNCH1_TIME 0.05f // in second. launch siteswap 1. Must be < HAND_PERIOD/2
#define GRAVITY QVector3D(0,-9.8f*SCALE_FACTOR,0)
#define DWELL_TIME DWELL_RATIO*HAND_PERIOD
#define EMPTY_TIME HAND_PERIOD-DWELL_TIME
#define S_TO_MS 1000.0f
const float DWELL_TIME_LAUNCH1 = (HAND_PERIOD / 2.0f) - LAUNCH1_TIME;
// for QAbstractAnimation::setLoopCount(int)
#define INFINITE_LOOP -1
#define ONE_LOOP 1

/*********************************** Enums *******************************************/

enum hand{leftHand, rightHand};
enum jugglingProp{ball, ring, club, propNumb};
enum launchTypeBall{normalBall, bounce};
enum launchTypeRing{normalRing, panCake};
enum launchTypeClub{normalClub, flat, helicopter};

#endif // QJOEPASS_H
