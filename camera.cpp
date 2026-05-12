#include "camera.h"
#include "renderer.h"

void Camera::Init()
{
	m_Position = {0.0f, 3.0f, -5.0f};
	m_Target = {0.0f, 0.0f, 0.0f};
}

void Camera::Uninit()
{}

void Camera::Update()
{}

void Camera::Draw()
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projection);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target),
		XMLoadFloat3(&up));

	Renderer::SetViewMatrix(view);
}
