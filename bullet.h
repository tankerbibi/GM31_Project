#pragma once
#include "main.h"
#include "vector3.h"
#include "gameObject.h"

class ModelRenderer;

class Bullet : public GameObject
{
private:
	// 速度
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	float m_Lifetime{ 2.0f };

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetVelocity(const Vector3& Velocity) { m_Velocity = Velocity; }
};