/*
 *  constants.h
 *  joe2
 *
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---today>  Wolfgang Westerboer
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------*/
// Normalvectors
#define aY Vector3D::yNormal
#define aX Vector3D::xNormal
#define aZ Vector3D::zNormal
#define NullVector Vector3D(0,0,0)


// some parameters for displaying unicycles
#define kurbelLength 15.f
#define wheelSize 25.f
#define pedalSize 10.f
#define axisLength 20.f
#define SchulterBreite          50.f                    // Abstand der Schultern