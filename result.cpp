#include "result.h"
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
#include "box.h"
#include "particle.h"
#include "Title.h"

void Result::Init()
{
	InputManager::Init(GetWindow());

	//	Manager::AddGameObject<Camera>();

	Manager::AddGameObject<Polygon2D>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\WindowsLock.png");
}

void Result::Uninit()
{}

void Result::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::ChangeScene<Title>();
	}
}

void Result::Draw()
{}
