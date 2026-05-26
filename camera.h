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

	Vector3 GetForward() override
	{
		Vector3 forward = m_Target - m_Position;
		forward.normalize();
		return forward;
	}
	Vector3 GetRight() override
	{
		Vector3 forward = m_Target - m_Position;
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = Vector3::cross(up, forward);
		right.normalize();

		return right;
	}

};