#include "main.h"
#include "manager.h"
#include "input.h"
#include "audio.h"
#include "inputManager.h"
#include "renderer.h"
#include "camera.h"
#include "game.h"
#include "title.h"

// staticメンバ変数は、.cpp側で宣言する必要がある。
std::list<GameObject*> Manager::m_GameObjects;
Scene* Manager::m_Scene = nullptr ;
Scene* Manager::m_NextScene = nullptr;
float Manager::m_ChangeTime = 0.0f;

void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();

	ChangeScene<Title>();
}


void Manager::Uninit()
{
	if (m_Scene != nullptr)
	{
		m_Scene->Uninit();
		delete m_Scene;
	}
	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->Uninit();
		delete gameObject;
	}
	m_GameObjects.clear();
	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
	
}

void Manager::Update()
{
	const float dt = 1.0f / 60;
	Input::Update();

	if (m_Scene != nullptr)
	{
		m_Scene->Update();
	}
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

	if (m_NextScene != nullptr)
	{
		m_ChangeTime -= dt;
		if (m_ChangeTime < 0.0f)
		{
			if (m_Scene != nullptr)
			{
				m_Scene->Uninit();
				delete m_Scene;
			}

			for (GameObject* gameObject : m_GameObjects)
			{
				gameObject->Uninit();
				delete gameObject;
			}
			m_GameObjects.clear();

			m_Scene = m_NextScene;
			m_Scene->Init();
			m_NextScene = nullptr;
		}
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	Camera* camera = GetGameObject<Camera>();
	if (camera)
	{
		Vector3 forward = camera->GetForward();
		Vector3 position = camera->GetPosition();
		// すべてにZを計算しているので少し効率が悪い
		for (GameObject* gameOject : m_GameObjects)
		{
			gameOject->CalcCameraZ(position, forward);
		}

		// Zソート
		m_GameObjects.sort([](GameObject* a, GameObject* b)
		{
				return a->GetCameraZ() > b->GetCameraZ();
		});
	}
	for (int layer = 0; layer < 4; layer++)
	{
		for (GameObject* gameObject : m_GameObjects)
		{
			if(gameObject->GetLayer() == layer)
				gameObject->Draw();
		}
	}
	Renderer::End();
}

// 正しい前後関係の描画だけ。