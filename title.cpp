#include "title.h"
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
#include "game.h"

void Title::Init()
{
	InputManager::Init(GetWindow());

//	Manager::AddGameObject<Camera>();

	Manager::AddGameObject<Polygon2D>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, L"asset\\texture\\Windows.png");
}

void Title::Uninit()
{}

void Title::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::ChangeScene<Game>();
	}
}

void Title::Draw()
{}
