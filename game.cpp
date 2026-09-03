#include "game.h"
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
#include "result.h"
#include "score.h"
#include "button.h"


void Game::Init()
{
	InputManager::Init(GetWindow());

	Manager::AddGameObject<Camera>();

	Manager::AddGameObject<Sky>();
	Manager::AddGameObject<Field>();
	Box* box = Manager::AddGameObject<Box>();
	box->SetPosition({ -20.0f, 0.0f, -3.0f });
	box->SetScale({ 5.0f, 2.0f, 5.0f });

	Button* button = Manager::AddGameObject<Button>();
	button->SetPosition({ 0.0f, 0.0f, -3.0f });
	button->SetScale({ 1.0f, 1.0f, 1.0f });


	Manager::AddGameObject<Player>();
	Manager::AddGameObject<Enemy>()->SetPosition({ -2.0f, 0.0f, 1.0f });
	Manager::AddGameObject<Enemy>()->SetPosition({ -0.0f, 0.0f, 1.0f });
	Manager::AddGameObject<Enemy>()->SetPosition({ 2.0f, 0.0f, 1.0f });

	Manager::AddGameObject<Tree>()->SetPosition({ -5.0f, 0.0f, 5.0f });

	Manager::AddGameObject<Particle>()->SetPosition({ -2.0f, 1.0, -1.0f });

	Manager::AddGameObject<Explosion>();

	Manager::AddGameObject<Polygon2D>()->Init(0.0f, 0.0f, 200.0f, 200.0f, L"asset\\texture\\Windows.png");
	Manager::AddGameObject<Score>()->SetPosition({ 100.0f, 100.0f, 0.0f });

	// AddGameObject<Polygon2D>();
}

void Game::Uninit()
{

}

void Game::Update()
{
	auto enemies = Manager::GetGameObjects<Enemy>();

	if (enemies.size() == 0)
	{
		Manager::ChangeScene<Result>(2.0f);
	}
}

void Game::Draw()
{
}
