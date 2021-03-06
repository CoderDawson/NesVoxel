#include "stdafx.h"
#include "VxlInput.h"
#include "NesEmulator.h"
#include "libretro.h"

ControllerState::ControllerState()
{
}

ControllerState::ControllerState(XINPUT_GAMEPAD gamepad)
{
	// Read buttons
	buttonStates[ba] = ((gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
	buttonStates[bb] = ((gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
	buttonStates[bx] = ((gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
	buttonStates[by] = ((gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
	buttonStates[bstart] = ((gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
	buttonStates[bselect] = ((gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
	buttonStates[bdLeft] = ((gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	buttonStates[bdRight] = ((gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
	buttonStates[bdUp] = ((gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
	buttonStates[bdDown] = ((gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
	buttonStates[blb] = ((gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
	buttonStates[brb] = ((gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
	// Read analog sticks
	analogStickStates[lStick] = { gamepad.sThumbLX / 32767.0f, gamepad.sThumbLY / 32767.0f };
	analogStickStates[rStick] = { gamepad.sThumbRX / 32767.0f, gamepad.sThumbRY / 32767.0f };
	// Read triggers
	triggerStates[lTrigger] = gamepad.bLeftTrigger / 255.0f;
	triggerStates[rTrigger] = gamepad.bRightTrigger / 255.0f;
	// Dead-zone sticks and triggers
	if (analogStickStates[lStick].x < 0.15f && analogStickStates[lStick].x > -0.15f)
		analogStickStates[lStick].x = 0;
	if (analogStickStates[lStick].y < 0.15f && analogStickStates[lStick].y > -0.15f)
		analogStickStates[lStick].y = 0;
	if (analogStickStates[rStick].x < 0.15f && analogStickStates[rStick].x > -0.15f)
		analogStickStates[rStick].x = 0;
	if (analogStickStates[rStick].y < 0.15f && analogStickStates[rStick].y > -0.15f)
		analogStickStates[rStick].y = 0;
}

InputState::InputState()
{
}

void InputState::refreshInput()
{
	XINPUT_STATE state;
	// See if either controller is connected and grab input state if so
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		connected[0] = true;
		gamePads[0] = ControllerState(state.Gamepad);
	}
	else
	{
		connected[0] = false;
	}
	if (XInputGetState(1, &state) == ERROR_SUCCESS)
	{
		connected[1] = true;
		gamePads[1] = ControllerState(state.Gamepad);
	}
	else
	{
		connected[1] = false;
	}
	// Send to emulator
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_A] = gamePads[0].buttonStates[ba];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_B] = gamePads[0].buttonStates[bx];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_SELECT] = gamePads[0].buttonStates[bselect];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_START] = gamePads[0].buttonStates[bstart];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_UP] = gamePads[0].buttonStates[bdUp];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_DOWN] = gamePads[0].buttonStates[bdDown];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_LEFT] = gamePads[0].buttonStates[bdLeft];
	NesEmulator::inputs[0][RETRO_DEVICE_ID_JOYPAD_RIGHT] = gamePads[0].buttonStates[bdRight];
}
