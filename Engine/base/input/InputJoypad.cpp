#include "InputJoypad.h"

void InputJoypad::SetDeadZone(short& sThumb, const short& deaadzone)
{
    if ((sThumb < deaadzone) && sThumb > -deaadzone) {
        sThumb = 0;
    }
}

InputJoypad::~InputJoypad()
{
    //XInputEnable(false);
}

InputJoypad* InputJoypad::GetInstance()
{
    static InputJoypad* instance = new InputJoypad;
    return instance;
}

void InputJoypad::DeleteInstance()
{
    delete InputJoypad::GetInstance();
}

void InputJoypad::Update()
{
    prevState = state;

    if (ERROR_SUCCESS == XInputGetState(0, &state)) {
        active = true;
    }
    else {
        active = false;
    }

    SetDeadZone(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

bool InputJoypad::GetButton(WORD button)
{
    return (state.Gamepad.wButtons & button) && active;
}

bool InputJoypad::GetTriggerButton(WORD button)
{
    return (state.Gamepad.wButtons & button) && !(prevState.Gamepad.wButtons & button) && active;
}

bool InputJoypad::GetLTrigger()
{
    return state.Gamepad.bLeftTrigger != 0 && active;
}

bool InputJoypad::GetRTrigger()
{
    return state.Gamepad.bRightTrigger != 0 && active;
}
