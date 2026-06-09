#pragma once
#include "main.h"
#include "vector3.h"
#include "gameObject.h"

class Explosion : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	float m_Lifetime{ 0.5f };
	int m_Frame{ 0 };
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};