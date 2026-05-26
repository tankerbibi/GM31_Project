#pragma once

#include <list>
#include "vector3.h"
#include "component.h"

class GameObject
{
protected:
	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };

	std::list<Component*> m_Components;

public:
	void SetPosition(const Vector3& Position) { m_Position = Position; }
	Vector3 GetPosition() { return m_Position; }

	virtual void Init() {}
	virtual void Uninit()
	{
		for (Component* component : m_Components)
		{
			component->Uninit();
			delete component;
		}
	}
	virtual void Update()
	{
		for (Component* component : m_Components)
		{
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (Component* component : m_Components)
		{
			component->Draw();
		}
	}

	template<typename T>
	T* AddComponent(GameObject* Object)
	{
		T* component = new T(Object);
		component->Init();
		m_Components.push_back(component);
		return component;
	}

	virtual Vector3 GetForward()
	{
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		// [2]でZ成分のベクトルを取得することができる。 前方向
		XMStoreFloat3((XMFLOAT3*)&forward, rot.r[2]);
		return forward;
	}

	virtual Vector3 GetRight()
	{
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		// [0]でX成分のベクトルを取得することができる 右方向
		XMStoreFloat3((XMFLOAT3*)&forward, rot.r[0]);
		return forward;
	}

	virtual Vector3 GetUp()
	{
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		// [1]でY成分のベクトルを取得することができる 上方向
		XMStoreFloat3((XMFLOAT3*)&forward, rot.r[1]);
		return forward;
	}
};
