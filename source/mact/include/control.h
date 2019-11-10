//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment

This file is part of Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Original Source: 1996 - Todd Replogle
Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms
Modifications for JonoF's port by Jonathon Fowler (jf@jonof.id.au)
*/
//-------------------------------------------------------------------------

//***************************************************************************
//
// Public header for CONTROL.C.
//
//***************************************************************************

#pragma once

#ifndef control_public_h_
#define control_public_h_

#include "tarray.h"
#include "inputstate.h"

//***************************************************************************
//
// DEFINES
//
//***************************************************************************

//***************************************************************************
//
// TYPEDEFS
//
//***************************************************************************
typedef enum
   {
   axis_up,
   axis_down,
   axis_left,
   axis_right
   } axisdirection;

typedef enum
   {
   analog_turning=0,
   analog_strafing=1,
   analog_lookingupanddown=2,
   analog_elevation=3,
   analog_rolling=4,
   analog_moving=5,
   analog_maxtype
   } analogcontrol;

typedef enum
   {
   controltype_keyboard,
   controltype_keyboardandmouse,
   controltype_keyboardandjoystick
   } controltype;

typedef enum
   {
   controldevice_keyboard,
   controldevice_mouse,
   controldevice_joystick
   } controldevice;

enum GameControllerButton : int
{
    GAMECONTROLLER_BUTTON_INVALID = -1,
    GAMECONTROLLER_BUTTON_A,
    GAMECONTROLLER_BUTTON_B,
    GAMECONTROLLER_BUTTON_X,
    GAMECONTROLLER_BUTTON_Y,
    GAMECONTROLLER_BUTTON_BACK,
    GAMECONTROLLER_BUTTON_GUIDE,
    GAMECONTROLLER_BUTTON_START,
    GAMECONTROLLER_BUTTON_LEFTSTICK,
    GAMECONTROLLER_BUTTON_RIGHTSTICK,
    GAMECONTROLLER_BUTTON_LEFTSHOULDER,
    GAMECONTROLLER_BUTTON_RIGHTSHOULDER,
    GAMECONTROLLER_BUTTON_DPAD_UP,
    GAMECONTROLLER_BUTTON_DPAD_DOWN,
    GAMECONTROLLER_BUTTON_DPAD_LEFT,
    GAMECONTROLLER_BUTTON_DPAD_RIGHT,
    GAMECONTROLLER_BUTTON_MAX
};

enum GameControllerAxis : int
{
    GAMECONTROLLER_AXIS_INVALID = -1,
    GAMECONTROLLER_AXIS_LEFTX,
    GAMECONTROLLER_AXIS_LEFTY,
    GAMECONTROLLER_AXIS_RIGHTX,
    GAMECONTROLLER_AXIS_RIGHTY,
    GAMECONTROLLER_AXIS_TRIGGERLEFT,
    GAMECONTROLLER_AXIS_TRIGGERRIGHT,
    GAMECONTROLLER_AXIS_MAX
};

enum class LastSeenInput : unsigned char
{
    Keyboard,
    Joystick,
};

//***************************************************************************
//
// GLOBALS
//
//***************************************************************************

extern bool CONTROL_Started;
extern bool CONTROL_MousePresent;
extern bool CONTROL_JoyPresent;
extern bool CONTROL_MouseEnabled;
extern bool CONTROL_JoystickEnabled;

extern LastSeenInput CONTROL_LastSeenInput;


//***************************************************************************
//
// PROTOTYPES
//
//***************************************************************************
void CONTROL_ClearAssignments( void );
void CONTROL_GetInput( ControlInfo *info );

bool CONTROL_Startup(controltype which, int32_t ( *TimeFunction )( void ), int32_t ticspersecond);
void CONTROL_Shutdown( void );

void CONTROL_MapAnalogAxis(int whichaxis, int whichanalog, controldevice device);
void CONTROL_MapDigitalAxis(int32_t whichaxis, int32_t whichfunction, int32_t direction, controldevice device);
void CONTROL_SetAnalogAxisScale(int32_t whichaxis, int32_t axisscale, controldevice device);
void CONTROL_SetAnalogAxisInvert(int32_t whichaxis, int32_t invert, controldevice device);

void CONTROL_ScanForControllers(void);

int32_t CONTROL_GetGameControllerDigitalAxisPos(int32_t axis);
int32_t CONTROL_GetGameControllerDigitalAxisNeg(int32_t axis);
void CONTROL_ClearGameControllerDigitalAxisPos(int32_t axis);
void CONTROL_ClearGameControllerDigitalAxisNeg(int32_t axis);


////////// KEY/MOUSE BIND STUFF //////////


////////////////////

#endif
