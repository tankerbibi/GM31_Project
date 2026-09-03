#include "player.h"
#include "input.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "inputManager.h"
#include "manager.h"
#include "camera.h"
#include "bullet.h"
#include "tree.h"
#include "box.h"
#include "button.h"
#include "enemy.h"
#include "result.h"
#include "audio.h"

//m_Position.x += InputManager::IsPressed(InputManager::GameAction::MoveX) * dt;
using namespace DirectX;
// 3Dの世界で親子関係を作るためには、マトリクスをかける。
void Player::Init()
{
    m_Layer = 1;

    m_Position = { -5.0f, 0.0f, 0.0f };
    m_Scale = { 0.01f, 0.01f, 0.01f };

    m_AnimationModel = AddComponent < AnimationModel>(this);
	m_AnimationModel->Load("asset\\model\\Akai.fbx");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

    m_AnimationName = "Idle";
    m_NextAnimationName = "Idle";

    AddComponent<ModelRenderer>(this)->Load("asset\\model\\player.obj");
    // --------------------------------------------------------
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
    // --------------------------------------------------------

    m_JumpSE = AddComponent<Audio>(this);
    m_JumpSE->Load("asset\\audio\\wan.wav");
    GameObject::Init();

}

void Player::Uninit()
{
    SAFE_RELEASE(m_PixelShader);
    SAFE_RELEASE(m_VertexShader);
    SAFE_RELEASE(m_VertexLayout);
    GameObject::Uninit();
}

void Player::Update()
{
    Vector3 oldPosition = m_Position;

    // 物理の判定は固定フレームレートがおすすめ。描画は可変フレームレートでもよいけど。
    float dt = 1.0f / 60.0f;

    Camera* camera = Manager::GetGameObject<Camera>();
    Vector3 forward = camera->GetForward();
    Vector3 right = camera->GetRight();

    forward.y = 0.0f;
    forward.normalize();

    right.y = 0.0f;
    right.normalize();

    bool move = false;
    if (!m_Ground)
    {
        if (Input::GetKeyPress('D'))
        {
            move = true;
            m_Velocity += right * 40.0f * dt;
        }
        if (Input::GetKeyPress('A'))
        {
            move = true;
            m_Velocity -= right * 40.0f * dt;
        }
        if (Input::GetKeyPress('W'))
        {
            m_Velocity += forward * 40.0f * dt;
            move = true;
        }
        if (Input::GetKeyPress('S'))
        {
            m_Velocity -= forward * 40.0f * dt;
            move = true;
        }
    }
    
    if (move)
    {
		SetAnimation("Run");
        // 移動方向に回転
        m_Rotation.y = atan2f(m_Velocity.x, m_Velocity.z);
	}
    else
    {
        SetAnimation("Idle");
    }

    // ジャンプ
    if (Input::GetKeyTrigger('K'))
    {
        m_Velocity.y += 20.0f;  // 撃力（瞬間的な力）
        // スケールアニメーション
        //m_Scale.y = 2.0f;
        //m_Scale.x = 0.5f;
        //m_Scale.z = 0.5f;

        m_JumpSE->Play();
    }
    //m_Scale.x += (1.0f - m_Scale.x) * 0.1f;
    //m_Scale.y += (1.0f - m_Scale.y) * 0.1f;
    //m_Scale.z += (1.0f - m_Scale.z) * 0.1f;

    // 重力
    m_Velocity.y += -15.0f * dt;

    // 抵抗力
    m_Velocity.x += -m_Velocity.x * 5.0f * dt;
    m_Velocity.z += -m_Velocity.z * 5.0f * dt;

    // 位置更新
    m_Position += m_Velocity * dt;
    bool oldGround = m_Ground;
    m_Ground = false;

    // 地面に衝突
    if (m_Position.y < 0.0f)
    {
        m_Position.y = 0.0f;
        m_Velocity.y = 0.0f;
        m_Ground = true;
    }
    // 木との衝突判定
    auto trees = Manager::GetGameObjects<Tree>();
    for (auto tree : trees)
    {
        Vector3 treePosition = tree->GetPosition();
        Vector3 playerPosition = m_Position;
        
        treePosition.y = 0.0f;
        playerPosition.y = 0.0f;
        Vector3 direction = playerPosition - treePosition;
        float length = direction.length();
        const float collisionRadius = 1.0f;
        if (length < collisionRadius)
        {
            direction /= length;
            const float overlap = 1.0f - length;
            direction *= overlap;

            m_Position += direction;
        }
    }
    // ボックスとの衝突判定
    auto boxes = Manager::GetGameObjects<Box>();
    for (auto box : boxes)
    {
        Vector3 boxPosition = box->GetPosition();
        Vector3 boxScale = box->GetScale();

        if (boxPosition.x - boxScale.x < m_Position.x && m_Position.x < boxPosition.x + boxScale.x && boxPosition.z - boxScale.z < m_Position.z && m_Position.z < boxPosition.z + boxScale.z)
        {
            if (boxPosition.y + boxScale.y < m_Position.y && m_Position.y < boxPosition.y + boxScale.y * 2.0f)
            {// 上面に衝突
                m_Position.y = boxPosition.y + boxScale.y * 2.0f;
                m_Velocity.y = 0.0f;
                m_Ground = true;
            }
            else if (boxPosition.y - boxScale.y < m_Position.y && m_Position.y < boxPosition.y + boxScale.y)
            {// 側面に衝突
                m_Position.x = oldPosition.x;
                m_Position.z = oldPosition.z;
                m_Velocity.x = 0.0f;
                m_Velocity.z = 0.0f;
            }
        }
    }

    // ボタンとの衝突判定
    auto buttons = Manager::GetGameObjects<Button>();
    for (auto button : buttons)
    {
        Vector3 buttonPosition = button->GetPosition();
        Vector3 buttonScale = button->GetScale();

        if (buttonPosition.x - buttonScale.x < m_Position.x && m_Position.x < buttonPosition.x + buttonScale.x && buttonPosition.z - buttonScale.z < m_Position.z && m_Position.z < buttonPosition.z + buttonScale.z)
        {
            if (buttonPosition.y + buttonScale.y < m_Position.y && m_Position.y < buttonPosition.y + buttonScale.y * 2.0f)
            {// 上面に衝突
                m_Position.y = buttonPosition.y + buttonScale.y * 2.0f;
                m_Velocity.y = 0.0f;
                m_Ground = true;
                button->PushButton();
            }
            else if (buttonPosition.y - buttonScale.y < m_Position.y && m_Position.y < buttonPosition.y + buttonScale.y)
            {// 側面に衝突
                m_Position.x = oldPosition.x;
                m_Position.z = oldPosition.z;
                m_Velocity.x = 0.0f;
                m_Velocity.z = 0.0f;
            }
        }
    }


    if (!oldGround && m_Ground)
    {
        //m_Scale.y = 0.5f;
        //m_Scale.x = 1.5f;
        //m_Scale.z = 1.5f;
    }

    // 弾発射
    if (Input::GetKeyTrigger('J'))
    {
        Bullet* bullet = Manager::AddGameObject<Bullet>();
        bullet->SetPosition(m_Position);
        bullet->SetVelocity(GetForward() * 30.0f);
    }

    if (m_Ground)
    {
        m_MoveAnimation += m_Velocity.length() * dt;
        if (m_Velocity.length() > 0.01f)
        {
            //m_Scale.y += sinf(m_MoveAnimation * 3.0f) * 0.03f;
        }
    }
    m_AnimationFrame++;
    m_NextAnimationFrame++;

    m_Blend += 0.1f;
    if (m_Blend > 1.0f)
    {
        m_Blend = 1.0f;
    }

    GameObject::Update();
}

void Player::Draw()
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

	m_AnimationModel->Update(m_AnimationName.c_str(), m_AnimationFrame, m_NextAnimationName.c_str(), m_NextAnimationFrame, m_Blend);

    GameObject::Draw();
}

void Player::SetAnimation(const char* AnimationName)
{
    if (m_NextAnimationName != AnimationName)
    {
		m_AnimationName = m_NextAnimationName;
        m_AnimationFrame = m_NextAnimationFrame;

        m_NextAnimationName = AnimationName;
        m_NextAnimationFrame = 0;

        m_Blend = 0.0f;
    }
}
