#include "player.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "inputManager.h"

//m_Position.x += InputManager::IsPressed(InputManager::GameAction::MoveX) * dt;
using namespace DirectX;

void Player::Init()
{
    m_Position = { -5.0f, 0.0f, 0.0f };

    AddComponent<ModelRenderer>(this)->Load("asset\\model\\player.obj");
    // --------------------------------------------------------
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
    // --------------------------------------------------------
}

void Player::Uninit()
{
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
}

void Player::Update()
{
    // 物理の判定は固定フレームレートがおすすめ。描画は可変フレームレートでもよいけど。
    float dt = 1.0f / 60.0f;


    if (Input::GetKeyPress('D'))
        m_Velocity.x += 40.0f * dt;
    if (Input::GetKeyPress('A'))
        m_Velocity.x -= 40.0f * dt;
    if (Input::GetKeyPress('W'))
        m_Velocity.z += 40.0f * dt;
    if (Input::GetKeyPress('S'))
        m_Velocity.z -= 40.0f * dt;

    if (Input::GetKeyTrigger(VK_SPACE))
        m_Velocity.y += 20.0f;  // 撃力（瞬間的な力）

    m_Velocity.y += -60.0f * dt;

    m_Velocity.x += -m_Velocity.x * 5.0f * dt;
    m_Velocity.z += -m_Velocity.z * 5.0f * dt;

    m_Position += m_Velocity * dt;

    if (m_Position.y < 0.0f)
    {
        m_Position.y = 0.0f;
        m_Velocity.y = 0.0f;
    }

    GameObject::Update();

    
}

void Player::Draw()
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
    // --------------------------------------------------------
    //// マテリアル設定
    //MATERIAL material{};
    //material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    //material.TextureEnable = true;
    //Renderer::SetMaterial(material);

    //Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    ////頂点バッファ設定
    //UINT stride = sizeof(VERTEX_3D);
    //UINT offset = 0;
    //Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //// ブリミティブトボロジ設定
    //Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    ////ボリゴン描画
    //Renderer::GetDeviceContext()->Draw(4, 0);
    GameObject::Draw();
}
