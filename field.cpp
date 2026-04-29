#include "field.h"
#include "renderer.h"

using namespace DirectX;

struct VERTEX
{
    XMFLOAT3 Position;
    XMFLOAT2 TexCoord;
};

void Field::Init()
{
    // 初期値
    m_Position = Vector3(0.0f, 0.0f, 0.0f);
    m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
    m_Scale = Vector3(1.0f, 1.0f, 1.0f);

    // ================================
    // 頂点バッファ作成
    // ================================
    VERTEX vertex[] =
    {
        { XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
    assert(SUCCEEDED(hr));

    // ================================
    // シェーダ作成
    // ================================
    Renderer::CreateVertexShader(
        &m_VertexShader,
        &m_VertexLayout,
        "shader/fieldVS.cso"
    );

    Renderer::CreatePixelShader(
        &m_PixelShader,
        "shader/fieldPS.cso"
    );

    // ================================
    // テクスチャ読み込み
    // ================================
    TexMetadata metadata;
    ScratchImage image;

    hr = LoadFromWICFile(
        L"asset\\texture\\Block_Fragile.jpg",
        WIC_FLAGS_NONE,
        &metadata,
        image
    );
    assert(SUCCEEDED(hr));

    hr = CreateShaderResourceView(
        Renderer::GetDevice(),
        image.GetImages(),
        image.GetImageCount(),
        metadata,
        &m_Texture
    );
    assert(SUCCEEDED(hr));
}

void Field::Uninit()
{
    SAFE_RELEASE(m_Texture);
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
    SAFE_RELEASE(m_VertexBuffer);
}

void Field::Update()
{
    // 今は特に何もしない
}

void Field::Draw()
{
    ID3D11DeviceContext* dc = Renderer::GetDeviceContext();

    // 頂点バッファ設定
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    dc->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    dc->IASetInputLayout(m_VertexLayout);

    // シェーダ設定
    dc->VSSetShader(m_VertexShader, nullptr, 0);
    dc->PSSetShader(m_PixelShader, nullptr, 0);

    // テクスチャ設定
    dc->PSSetShaderResources(0, 1, &m_Texture);

    // 描画
    dc->Draw(4, 0);
}