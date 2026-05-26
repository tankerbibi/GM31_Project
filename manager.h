#pragma once
#include <list>
#include "gameObject.h"

class Manager
{
private:
	static std::list<GameObject*> m_GameObjects;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	// テンプレートは、わざわざそれ専用の関数を作っている。多用しすぎると、Exeファイルがでっかくなる。
	template <typename T>
	static T* AddGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}

	template <typename T>
	static T* GetGameObject()
	{
		for (GameObject* gameObject : m_GameObjects)
		{
			T* casted = dynamic_cast<T*>(gameObject);  // RTTI（実行時型情報）を利用して、GameObjectをT型にキャスト
			if (casted != nullptr)
			{
				return casted;
			}
		}
		return nullptr;
	}
};
