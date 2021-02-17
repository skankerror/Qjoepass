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

#include <QDebug>

/**************************** App ***************************************/

#define APP_ICON "://res/images/icone.png"
#define WINDOW_MINIMUM_W 160
#define WINDOW_MINIMUM_H 160
#define WINDOW_W 1024
#define WINDOW_H 768
#define WIDGET3D_MIN_W 800
#define WIDGET3D_MIN_H 600

/**************************** 3D definitions ****************************/

#define NULL_FLOAT 0.0f
#define NULL_VECTOR QVector3D(NULL_FLOAT,NULL_FLOAT,NULL_FLOAT)

  /***************************** World *******************************/

  // camera
  #define CAM_FOV 65.0f
  #define CAM_RATIO 16.0f/9.0f
  #define CAM_NEARPLANE /*0.1f*/ /*1.0f*/ 0.01f
  #define CAM_FARPLANE 1000.0f
  #define CAM_INITIAL_POSITION QVector3D(NULL_FLOAT,15.0f,15.0f)
  #define CAM_UP_VECTOR QVector3D(NULL_FLOAT,1.0f,NULL_FLOAT)
  #define CAM_ORBIT_VIEW_CENTER QVector3D(NULL_FLOAT,6.0f,NULL_FLOAT)
  // SkyBox
  #define SKYBOX_BASE_NAME "qrc:/skybox/res/images/skybox/night"
  #define SKYBOX_EXTENSION ".jpg"
  #define SKYBOX_SCALE 10.0F
  // Ground
  #define GROUND_WIDTH 30
  #define GROUND_HEIGHT 30
  #define GROUND_POSY NULL_FLOAT
  #define GROUND_AMBIENT_OCCLUSION "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_ambient_occlusion.png"
  #define GROUND_ROUGHNESS "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_roughness.png"
  #define GROUND_BASE_COLOR "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_basecolor.png"
  #define GROUND_NORMAL "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_normal.png"
  #define GROUND_METALNESS "qrc:/woodfloor/res/images/WoodFloor/wooden_planks_01_metallic.png"
  // Light
  #define LIGHT_COLOR 0xFFFFFF
  #define LIGHT_INTENSITY 1
  #define LIGHT1_POS QVector3D(-20,15,20)
  #define LIGHT2_POS QVector3D(20,15,20)
  #define LIGHT3_POS QVector3D(NULL_FLOAT,15,-20)
  #define IRRADIANCE "qrc:/cubemaps/res/cubemaps/default_irradiance.dds"
  #define SPECULAR "qrc:/cubemaps/res/cubemaps/default_specular.dds"

  /************************************* Juggler **************************/

  // Juggler Global
  #define JUGGLER_SCALE 1.0f
  #define JUGGLER_TRANSLATION_Y NULL_FLOAT
  #define HAND_OFFSET_X  1
  #define HAND_OFFSET_Y  4.8
  #define HAND_OFFSET_Z  1.5
  #define HAND_OFFSET_EXT 0.8
  #define HAND_OFFSET_EXT_PLUS 1.5f // helico pancakes
  #define HAND_OFFSET_INT 0.5
  #define JUGGLER_METALNESS 0.8
  #define JUGGLER_ROUGHNESS 0.5
  #define HAND_BACKEST_PASSING_Y 4.0f
  #define HAND_BACKEST_PASSING_Z 0.5f
    // Head
    #define HEAD_POS_Y 7.0f
    #define HEAD_RADIUS 0.5f
    #define HEAD_RINGS 32
    #define HEAD_SLICES 32
    #define HEAD_TRANSLATE QVector3D(NULL_FLOAT,HEAD_POS_Y,NULL_FLOAT)
    #define LOOK_AT_VECTOR QVector3D(NULL_FLOAT,NULL_FLOAT,5.0f)
    // Clavicles
    #define CLAVICLES_ROTATION QVector3D(NULL_FLOAT,NULL_FLOAT,90.0f)
    #define CLAVICLES_LENGHT 2.0f
    #define CLAVICLES_Y 6.3f
    #define CLAVICLES_Z NULL_FLOAT
    #define CLAVICLES_TRANSLATION QVector3D(NULL_FLOAT,CLAVICLES_Y,NULL_FLOAT)
    #define LEFT_SHOULDER_X 1.0f
    #define RIGHT_SHOULDER_X -1.0f
    #define SHOULDER_Y CLAVICLES_Y
    #define SHOULDER_Z CLAVICLES_Z
    // Trunk
    #define TRUNK_ROTATION NULL_VECTOR
    #define TRUNK_TRANLATION QVector3D(NULL_FLOAT,5.3f,NULL_FLOAT)
    #define TRUNK_LENGHT 2.5f
    // Thighs
    #define LEFT_THIGH_ROTATION QVector3D(NULL_FLOAT,NULL_FLOAT,13.5f)
    #define LEFT_THIGH_TRANSLATION QVector3D(0.25f,3.05f,NULL_FLOAT)
    #define RIGHT_THIGH_ROTATION QVector3D(0.0f,0.0f,-13.5f)
    #define RIGHT_THIGH_TRANLATION QVector3D(-0.25f,3.05f,NULL_FLOAT)
    #define THIGH_LENGHT 2.2f
    // Knees
    #define LEFT_KNEE_TRANLATION QVector3D(0.5f,2.0f,NULL_FLOAT)
    #define RIGHT_KNEE_TRANLATION QVector3D(-0.5f,2.0f,NULL_FLOAT)
    // Legs
    #define LEFT_LEG_ROTATION QVector3D(NULL_FLOAT,NULL_FLOAT,13.5f)
    #define LEFT_LEG_TRANSLATION QVector3D(0.75f,1.0f,NULL_FLOAT)
    #define RIGHT_LEG_ROTATION QVector3D(NULL_FLOAT,NULL_FLOAT,-13.5f)
    #define RIGHT_LEG_TRANSLATION QVector3D(-0.75f,1.0f,NULL_FLOAT)
    #define LEG_LENGHT 2.0f
    // Members
    #define MEMBERS_RADIUS 0.1f
    #define MEMBERS_RINGS 8
    #define MEMBERS_SLICES 32
    // Articulations
    #define ARTICULATION_RADIUS 0.1f
    #define ARTICULATION_RINGS 8
    #define ARTICULATION_SLICES 32
      /****************************** Arms *****************************/
      #define GLOBAL_LEFT_ARM_TRANSLATION QVector3D(-6.3,-1,0)
      #define GLOBAL_RIGHT_ARM_TRANSLATION QVector3D(-6.3f,1,0)
      // Shoulder
      #define SHOULDER_TRANSLATION NULL_VECTOR
      // Arm
      #define ARM_ROTATION NULL_VECTOR
      #define ARM_LENGHT 1.5f
      #define ARM_TRANSLATION QVector3D(NULL_FLOAT,-ARM_LENGHT/2.0f,NULL_FLOAT)
      // Elbows
      #define ELBOW_TRANSLATION QVector3D(NULL_FLOAT,-ARM_LENGHT/2.0f,NULL_FLOAT)
      // Forearms
      #define FOREARM_ROTATION NULL_VECTOR
      #define FOREARM_LENGHT 1.5f
      #define FOREARM_TRANSLATION QVector3D(NULL_FLOAT,-ARM_LENGHT/2.0f,NULL_FLOAT)

  /********************************** Prop ********************************/

  // Prop Global
  #define PROP_METALNESS 0.2
  #define PROP_ROUGHNESS 0.5
  #define PROP_JUGGLING_DECAY_ROT_Y 15
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
    #define CLUB_DWELL_SELF_X_DECAY 30
    #define CLUB_DWELL_PASSING_X_DECAY 60

/******************************** Animation *****************************************/

#define SCALE_FACTOR 4.03f // for gravity... size of juggler / normal man size (7.25/1.8)
#define DELTA_TIME 0.015f // in second
#define HAND_PERIOD 0.7f // in second
#define DWELL_RATIO 0.6f // seems to be dwell average of a good juggler
#define LAUNCH1_TIME 0.1f // in second. launch siteswap 1. Must be < HAND_PERIOD/2
#define GRAVITY QVector3D(NULL_FLOAT,-9.8f*SCALE_FACTOR,NULL_FLOAT)
#define DWELL_TIME DWELL_RATIO*HAND_PERIOD// must be currently 0.42
#define EMPTY_TIME HAND_PERIOD-DWELL_TIME // must be currently 0.23
#define S_TO_MS 1000.0f
const float DWELL_TIME_LAUNCH1 = (HAND_PERIOD / 2.0f) - LAUNCH1_TIME; // must be currently 250
#define DEFAULT_JUG_POS_CIRC_RADIUS 8.0f
// for QAbstractAnimation::setLoopCount(int)
#define INFINITE_LOOP -1
#define ONE_LOOP 1
// for anim 0 launch
#define NO_PROP -1

/*********************************** Enums *******************************************/

enum hand{rightHand, leftHand};
enum propType{ball, ring, club, propNumb};
enum launchTypeBall{normalBall, bounce};
enum launchTypeRing{normalRing, panCake};
enum launchTypeClub{normalClub, flat, helicopter};

#endif // QJOEPASS_H
