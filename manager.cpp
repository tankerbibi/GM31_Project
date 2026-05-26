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

// staticメンバ変数は、.cpp側で宣言する必要がある。
std::list<GameObject*> Manager::m_GameObjects;

void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	InputManager::Init(GetWindow());

	AddGameObject<Camera>();
	
	AddGameObject<Field>();
	AddGameObject<Player>();
	// AddGameObject<Enemy>()->SetPosition({-2.0f, 0.0f, 1.0f});

	AddGameObject<Polygon2D>();
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
