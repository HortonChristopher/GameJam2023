#pragma once
#include "ObjObject.h"

class Tori :
	public ObjObject
{
protected:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<Tori>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale);

public: // �����o�֐�
	// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale);

	// ���t���[������
	void Update();

	void SetNewMovementPosition();
	void Move();

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };
	XMFLOAT3 target = { 0,0,0 };
	float speed = 0.2f;
	float timer = 0.0f;

	bool moving = false;
public:
	// �f�X�t���O
	bool goalFlag = false;
};