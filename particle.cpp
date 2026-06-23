#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

using namespace DirectX;

void Particle::Init()
{
    m_Layer = 2;

    VERTEX_3D vertex[4];

    vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, -1.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");

    TexMetadata metadata;
    ScratchImage image;
    HRESULT hr = LoadFromWICFile(L"asset\\texture\\particle.png", WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);

    assert(m_Texture);

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        m_Particle[i].Enable = false;
    }
}

void Particle::Uninit()
{
    SAFE_RELEASE(m_Texture);
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
    SAFE_RELEASE(m_VertexBuffer);
}




void Particle::Update()
{
    float dt = 1.0f / 60.0f;

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        if (m_Particle[i].Enable == true) continue;
        const int particleLifeMax = 60;
        m_Particle[i].Enable = true;
        m_Particle[i].Life = 60;
        m_Particle[i].Velocity.x = ((float)rand() / RAND_MAX - 0.5f ) * 10.0f;
        m_Particle[i].Velocity.y = ((float)rand() / RAND_MAX) * 10.0f ;
        m_Particle[i].Velocity.z = ((float)rand() / RAND_MAX - 0.5f) * 10.0f;
        m_Particle[i].Position = m_Position;
        break;
    }
    Vector3 gravity{ 0.0f, -9.8f, 0.0f };
    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        if (m_Particle[i].Enable == false) continue;

        m_Particle[i].Life--;
        if (m_Particle[i].Life < 0)
        {
            m_Particle[i].Enable = false;
            continue;
        }

        m_Particle[i].Velocity += gravity * dt;
        m_Particle[i].Velocity.y += 2.8f * dt;
        m_Particle[i].Position += m_Particle[i].Velocity * dt;
    }

    
}

void Particle::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

    // ビルボード用マトリクス
    Camera* camera = Manager::GetGameObject<Camera>();
    XMMATRIX view = camera->GetViewMatrix();
    XMMATRIX invView = XMMatrixInverse(NULL, view);  // 逆行列 Inverseは処理負荷が高いらしい。
    invView.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);  // 平行移動成分をリセット
    /*invView.r[3].m128_f32[0] = 0.0f;
    invView.r[3].m128_f32[1] = 0.0f;
    invView.r[3].m128_f32[2] = 0.0f;*/

    // マテリアル設定
    MATERIAL material{};
    material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    //頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // ブリミティブトボロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    Renderer::SetDepthEnable(false);

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        if (m_Particle[i].Enable == true)
        {
            // マトリクス設定
            XMMATRIX world, scale, rot, trans;
            scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
            // rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
            trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
            world = scale * invView * trans;
            Renderer::SetWorldMatrix(world);

            //ボリゴン描画
            Renderer::GetDeviceContext()->Draw(4, 0);
        }
    }

    Renderer::SetDepthEnable(true);

}