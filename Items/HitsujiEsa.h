#pragma once
#include "ObjObject.h"

class HitsujiEsa :
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

private: // �ÓI�����o�ϐ�
	// ������܂ł̎���
	static const int32_t LifeTime = 240;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<HitsujiEsa>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale);

public: // �����o�֐�
	// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale);

	// ���t���[������
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// �f�X�^�C�}�[ 
	int32_t deathTimer = LifeTime;

public:
	// �f�X�t���O
	bool deathFlag = false;
};