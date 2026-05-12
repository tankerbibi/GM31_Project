#pragma once
#include "main.h"
#include "gameObject.h"
#include "vector3.h"

class Camera : public GameObject
{
private:
	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};