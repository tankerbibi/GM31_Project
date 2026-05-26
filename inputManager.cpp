#include "InputManager.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

// =============================
// 内部状態
// =============================
namespace
{
    HWND g_hWnd = nullptr;

    // キーボード
    bool g_KeyCurrent[256] = {};
    bool g_KeyPrev[256] = {};

    // ゲームパッド
    XINPUT_STATE g_PadState{};
    XINPUT_STATE g_PadPrev{};

    // DeadZone
    const float DEAD_ZONE = 8000.0f;
}

// =============================
// 初期化
// =============================
void InputManager::Init(HWND hwnd)
{
    g_hWnd = hwnd;
}

// =============================
// 更新
// =============================
void InputManager::Update()
{
    // --- キーボード ---
    memcpy(g_KeyPrev, g_KeyCurrent, sizeof(g_KeyCurrent));

    for (int i = 0; i < 256; ++i)
    {
        g_KeyCurrent[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }

    // --- ゲームパッド ---
    g_PadPrev = g_PadState;
    ZeroMemory(&g_PadState, sizeof(XINPUT_STATE));
    XInputGetState(0, &g_PadState);
}
float InputManager::GetAxis(GameAction action)
{
    float value = 0.0f;

    switch (action)
    {
    case GameAction::MoveX:
    {
        // キーボード
        if (g_KeyCurrent['A']) value -= 1.0f;
        if (g_KeyCurrent['D']) value += 1.0f;

        // ゲームパッド（左スティック）
        float lx = (float)g_PadState.Gamepad.sThumbLX;

        if (fabs(lx) > DEAD_ZONE)
        {
            value += lx / 32767.0f;
        }
        break;
    }

    case GameAction::MoveY:
    {
        if (g_KeyCurrent['W']) value += 1.0f;
        if (g_KeyCurrent['S']) value -= 1.0f;

        float ly = (float)g_PadState.Gamepad.sThumbLY;

        if (fabs(ly) > DEAD_ZONE)
        {
            value += ly / 32767.0f;
        }
        break;
    }
    }

    // clamp
    if (value > 1.0f) value = 1.0f;
    if (value < -1.0f) value = -1.0f;

    return value;
}

bool InputManager::IsPressed(GameAction action)
{
    switch (action)
    {
    case GameAction::Jump:
        return g_KeyCurrent[VK_SPACE] ||
            (g_PadState.Gamepad.wButtons & XINPUT_GAMEPAD_A);

    case GameAction::Attack:
        return g_KeyCurrent[VK_LBUTTON] ||
            (g_PadState.Gamepad.wButtons & XINPUT_GAMEPAD_X);
    }

    return false;
}

bool InputManager::IsTrigger(GameAction action)
{
    switch (action)
    {
    case GameAction::Jump:
    {
        bool key = g_KeyCurrent[VK_SPACE] && !g_KeyPrev[VK_SPACE];
        bool pad = (g_PadState.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
            !(g_PadPrev.Gamepad.wButtons & XINPUT_GAMEPAD_A);
        return key || pad;
    }

    case GameAction::Attack:
    {
        bool key = g_KeyCurrent[VK_LBUTTON] && !g_KeyPrev[VK_LBUTTON];
        bool pad = (g_PadState.Gamepad.wButtons & XINPUT_GAMEPAD_X) &&
            !(g_PadPrev.Gamepad.wButtons & XINPUT_GAMEPAD_X);
        return key || pad;
    }
    }

    return false;
}

