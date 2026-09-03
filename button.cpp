#include "button.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "inputManager.h"


using namespace DirectX;

void Button::Init()
{
    m_Layer = 1;

    m_Position = { -5.0f, 0.0f, 0.0f };

    AddComponent<ModelRenderer>(this)->Load("asset\\model\\Box.obj");
    // --------------------------------------------------------
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
    // --------------------------------------------------------
}

void Button::Uninit()
{
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
}

void Button::Update()
{
    // 物理の判定は固定フレームレートがおすすめ。描画は可変フレームレートでもよいけど。
    float dt = 1.0f / 60.0f;

    // m_Velocity += Vector3(5.0f, 0.0f, 0.0f) * dt;
    // m_Position += m_Velocity * dt;

    if (m_PushedFrame > 30)
    {
        m_Scale.x += (1.5f - m_Scale.x) * 0.1f;
        m_Scale.y += (0.3f - m_Scale.y) * 0.1f;
        m_Scale.z += (1.5f - m_Scale.z) * 0.1f;
    }
    else
    {
        m_Scale.x += (1.0f - m_Scale.x) * 0.1f;
        m_Scale.y += (1.0f - m_Scale.y) * 0.1f;
        m_Scale.z += (1.0f - m_Scale.z) * 0.1f;
    }
    
    /*if (Input::GetKeyPress('D'))
        m_Position.x += 1.0f * dt;*/
    m_Position.x += InputManager::IsPressed(InputManager::GameAction::MoveX) * dt;

    if (m_Scale.y < 0.8f)
    {
        m_PushedFrame++;
    }
}

void Button::Draw()
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

void Button::PushButton()
{
    if (m_PushedFrame <= 30)
    {
        m_Scale = { 1.5f, 0.5f, 1.5f };
    }
}
