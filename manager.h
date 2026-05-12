#pragma once
#include <list>

class GameObject;

class Manager
{
private:
	static std::list<GameObject*> m_GameObjects;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

};