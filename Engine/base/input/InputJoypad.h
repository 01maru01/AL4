#pragma once
#include <Windows.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")

class InputJoypad
{
private:
    XINPUT_STATE state;
    XINPUT_STATE prevState;
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
    bool GetLTrigger();
    bool GetRTrigger();
};
