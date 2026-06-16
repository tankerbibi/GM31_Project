#pragma once

#include <list>
#include "vector3.h"
#include "component.h"

class GameObject
{
protected:
	bool m_Destroy = false;

	// 描画順番レイヤー番号 数字が低い順番に描画
	int m_Layer = 1;
	// ソート用Z値
	float m_CameraZ;

	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };

	std::list<Component*> m_Components;

public:
	int GetLayer() { return m_Layer; }

	float GetCameraZ() const { return m_CameraZ; }
	void CalcCameraZ(Vector3 CameraPosition, Vector3 CameraForward)
	{
		Vector3 direction = m_Position - CameraPosition;
		m_CameraZ = Vector3::dot(direction, CameraForward);
	}


	void SetPosition(const Vector3& Position) { m_Position = Position; }
	Vector3 GetPosition() { return m_Position; }

	void SetScale(const Vector3& Scale) { m_Scale = Scale; }
	Vector3 GetScale() { return m_Scale; }

	void SetRotation(const Vector3& Rotation) { m_Rotation = Rotation; }
	Vector3 GetRotation() { return m_Rotation; }

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

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
