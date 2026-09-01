#pragma once
#include "main.h"
#include "vector3.h"
#include "gameObject.h"
#include "animationModel.h"

class ModelRenderer;
class Audio;
class Player : public GameObject
{
private:
	// 速度
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };

	// ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	// 接地状態
	bool m_Ground = true;
	float m_MoveAnimation = 0.0f;
	class AnimationModel* m_AnimationModel;
	int m_AnimationFrame = 0;

	Audio* m_JumpSE;

	//ID3D11ShaderResourceView* m_Texture;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};