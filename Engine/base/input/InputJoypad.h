#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "MyMath.h"

#pragma comment (lib, "xinput.lib")

class InputJoypad
{
private:
    XINPUT_STATE state;
    XINPUT_STATE prevState;

    bool active = false;
private:
    void SetDeadZone(short& sThumb, const short& deaadzone);

    InputJoypad() {};
    ~InputJoypad();
public:
    static InputJoypad* GetInstance();
    static void DeleteInstance();
    InputJoypad(const InputJoypad& obj) = delete;
    InputJoypad& operator=(const InputJoypad& obj) = delete;

    void Update();
    bool GetButton(WORD button);
    bool GetTriggerButton(WORD button);
    bool GetLTrigger();
    bool GetRTrigger();
    const bool ActivePad() { return active; }
    const Vector2D& GetThumbR() { return Vector2D(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY); }
    const Vector2D& GetThumbL() { return Vector2D(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY); }
    bool GetTriggerThumbLX() { return ((state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (prevState.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) || ((state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && (prevState.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)); }
    bool GetTriggerThumbLY() { return ((state.Gamepad.sThumbLY > 0) && (prevState.Gamepad.sThumbLY <= 0)) || ((state.Gamepad.sThumbLY <= 0) && (prevState.Gamepad.sThumbLY > 0)); }
};
