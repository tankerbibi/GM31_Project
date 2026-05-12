#pragma once
#include "main.h"
#include "vector3.h"

class InputManager
{
private:
	enum class GameAction
	{
		MoveX,
		MoveY,
		Jump,
		Attack,
	};

private:
	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };

public:
	static void Init(HWND hwnd);

	static void Update();
	
	static float GetAxis(GameAction action);
	static bool IsPressed(GameAction action);
	static bool IsTrigger(GameAction action);
};