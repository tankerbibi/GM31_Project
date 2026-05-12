#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "field.h"
#include "camera.h"

// staticメンバ変数は、.cpp側で宣言する必要がある。
std::list<GameObject*> Manager::m_GameObjects;

void Manager::Init()
{
	Renderer::Init();

	GameObject* gameObject;

	gameObject = new Camera();
	gameObject->Init();
	m_GameObjects.push_back(gameObject);


	gameObject = new Field();
	gameObject->Init();
	m_GameObjects.push_back(gameObject);

	gameObject = new Polygon2D();
	gameObject->Init();
	m_GameObjects.push_back(gameObject);
}


void Manager::Uninit()
{
	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->Uninit();
		delete gameObject;
	}

	Renderer::Uninit();
}

void Manager::Update()
{
	// タスクシステム
	for (GameObject* gameObject : m_GameObjects)
	{
		// ポリモーフィズム
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
