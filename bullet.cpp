#include "bullet.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "inputManager.h"
#include "enemy.h"
#include "manager.h"
#include "explosion.h"


using namespace DirectX;

void Bullet::Init()
{
    m_Position = { -5.0f, 0.0f, 0.0f };

    AddComponent<ModelRenderer>(this)->Load("asset\\model\\bullet.obj");
    // --------------------------------------------------------
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
    // --------------------------------------------------------
    GameObject::Init();
}

void Bullet::Uninit()
{
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);

    GameObject::Uninit();
}

void Bullet::Update()
{
    // 物理の判定は固定フレームレートがおすすめ。描画は可変フレームレートでもよいけど。
    float dt = 1.0f / 60.0f;

    m_Position += m_Velocity * dt;

    // 敵との衝突判定
    auto enemies = Manager::GetGameObjects<Enemy>();
    for (auto enemy : enemies)
    {
        Vector3 direction = enemy->GetPosition() - m_Position;
        float length = direction.length();

        if (length < 1.0f)
        {
             enemy->SetDestroy();
             SetDestroy();

             Manager::AddGameObject<Explosion>()->SetPosition(enemy->GetPosition());
             break;
        }
    }


    m_Lifetime -= dt;
    if (m_Lifetime <= 0.0f)
    {  // 自分自身を削除する予約
        SetDestroy();
    }

    GameObject::Update();
}

void Bullet::Draw()
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
