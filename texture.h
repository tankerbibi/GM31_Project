#pragma once
#include "main.h"

class Texture
{
public:
	int LoadTexture();
	ID3D11ShaderResourceView* GetTexture(int id);

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

