#include "sky.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "inputManager.h"
#include "camera.h"
#include "manager.h"

using namespace DirectX;

void Sky::Init()
{
    m_Position = { -5.0f, 0.0f, 0.0f };

    AddComponent<ModelRenderer>(this)->Load("asset\\model\\sky.obj");
    // --------------------------------------------------------
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
    // --------------------------------------------------------

    m_Scale = {100.0f, 100.0f, 100.0f };
}

void Sky::Uninit()
{
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
}

void Sky::Update()
{
    // 物理の判定は固定フレームレートがおすすめ。描画は可変フレームレートでもよいけど。
    float dt = 1.0f / 60.0f;

    Camera* camera = Manager::GetGameObject<Camera>();

    m_Position = { camera->GetPosition().x, camera->GetPosition().y - 40.0f, camera->GetPosition().z };
}

void Sky::Draw()
{
    // --------------------------------------------------------
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
    // --------------------------------------------------------
    // マトリクス設定
    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;

    Renderer::SetWorldMatrix(world);

    GameObject::Draw();
}
