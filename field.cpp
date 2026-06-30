#include "field.h"
#include "renderer.h"
#include "audio.h"

using namespace DirectX;

void Field::Init()
{
    m_Layer = 1;

    VERTEX_3D vertex[4];

    vertex[0].Position = XMFLOAT3(-30.0f, 0.0f, 30.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(30.0f, 0.0f, 30.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(10.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(-30.0f, 0.0f, -30.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 10.0f);

    vertex[3].Position = XMFLOAT3(30.0f, 0.0f, -30.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(10.0f, 10.0f);



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
    HRESULT hr = LoadFromWICFile(L"asset\\texture\\Block_Fragile.png", WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);
    // BGM再生
    Audio* bgm = AddComponent<Audio>(this);
    bgm->Load("asset\\audio\\bgm.wav");
    bgm->Play(true);
    GameObject::Init();
}

void Field::Uninit()
{
    SAFE_RELEASE(m_Texture);
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
    SAFE_RELEASE(m_VertexBuffer);

    GameObject::Uninit();
}

void Field::Update()
{
    // 今は特に何もしない
    GameObject::Update();
}

void Field::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

    // マトリクス設定
    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;

    Renderer::SetWorldMatrix(world);

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
    //ボリゴン描画
    Renderer::GetDeviceContext()->Draw(4, 0);

    GameObject::Draw();
}

// パーティクルを表面にくっつけることもできるらしい。 ベクターフィールド、うねうねパーティクル作れる。ブルームっていうポストエフェクトかけると花火がきれいになる。