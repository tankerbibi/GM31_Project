#pragma once
#include "main.h"
#include "vector3.h"

class Field
{
private:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};