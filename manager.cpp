#include "main.h"
#include "manager.h"
#include "input.h"
#include "inputManager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "field.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "tree.h"
#include "explosion.h"
#include "sky.h"

// staticメンバ変数は、.cpp側で宣言する必要がある。
std::list<GameObject*> Manager::m_GameObjects;

void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	InputManager::Init(GetWindow());

	AddGameObject<Camera>();
	
	AddGameObject<Sky>();
	AddGameObject<Field>();
	AddGameObject<Player>();
	AddGameObject<Enemy>()->SetPosition({-2.0f, 0.0f, 1.0f});
	AddGameObject<Enemy>()->SetPosition({-0.0f, 0.0f, 1.0f});
	AddGameObject<Enemy>()->SetPosition({2.0f, 0.0f, 1.0f});

	AddGameObject<Tree>()->SetPosition({-5.0f, 0.0f, 5.0f });

	AddGameObject<Explosion>();


	// AddGameObject<Polygon2D>();
}


void Manager::Uninit()
{
	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->Uninit();
		delete gameObject;
	}

	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();
	// タスクシステム 範囲for文
	for (GameObject* gameObject : m_GameObjects)
	{
		// ポリモーフィズム、多態性
		gameObject->Update();
	}
	// ループする途中で削除してはいけない！ これはラムダ式。
	// [](GameObject* object){ return object->Destroy(); } ←ラムダ式。
	m_GameObjects.remove_if([](GameObject* object)
		{
			return object->Destroy();
		});
}

void Manager::Draw()
{
	Renderer::Begin();

	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->Draw();
	}

	Renderer::End();
}

// 正しい前後関係の描画だけ。