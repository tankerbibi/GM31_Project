#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "input.h"

void Camera::Init()
{
	m_Position = {0.0f, 5.0f, -10.0f};
	m_Target = {0.0f, 0.0f, 0.0f};
	GameObject::Init();
}

void Camera::Uninit()
{
	GameObject::Uninit();
}

void Camera::Update()
{
	GameObject::Update();
	Player* player = Manager::GetGameObject<Player>();
	Vector3 playerPos = player->GetPosition();

	float dt = 1.0f / 60.0f;

	// キー入力で回転
	if (Input::GetKeyPress(VK_RIGHT))
		m_Rotation.y += 3.0f * dt;
	if (Input::GetKeyPress(VK_LEFT))
		m_Rotation.y -= 3.0f * dt;
	
	// 線形補完
	float t = 0.1f;
	m_Target = m_Target * (1.0f - t) + (playerPos + Vector3(0.0f, 2.0f, 0.0f)) * t;
	//m_Target = playerPos + Vector3(0.0f, 2.0f, 0.0f);

	m_Position = m_Target + Vector3(-sinf(m_Rotation.y) * 10.0f, 5.0f, -cosf(m_Rotation.y) * 10.0f);
}

void Camera::Draw()
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projection);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target),
		XMLoadFloat3(&up));

	Renderer::SetViewMatrix(m_ViewMatrix);

	GameObject::Draw();
}
