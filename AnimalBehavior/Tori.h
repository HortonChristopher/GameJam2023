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

private:
	static enum EsaReaction {
		Esa_None = 0,
		Esa_Set = 1,
		Esa_Moving = 2,
		Esa_Standing = 3
	};
	EsaReaction esaReaction = Esa_None;
	
	static enum TekiReaction {
		Teki_None = 0,
		Teki_Set = 1,
		Teki_Moving = 2,
		Teki_Standing = 3
	};
	TekiReaction tekiReaction = Teki_None;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<Tori>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale);

public: // �����o�֐�
	// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale);

	// ���t���[������
	void Update();

	void SetNewMovementPosition();
	void Move(bool forwardBackwards); // true = forwards

	void UpdateEntitiesInRange(XMFLOAT3 tekiPosition, XMFLOAT3 esaPosition, XMFLOAT3 playerPosition);
	bool ItemIntersection(float radius1, const XMFLOAT3& center2, float radius2);
	void EsaInRange(XMFLOAT3 esaPosition);
	void TekiInRange(XMFLOAT3 tekiPosition, XMFLOAT3 playerPosition);

	void RotationVectorSet(XMFLOAT3 target, XMFLOAT3 origin);

	float SquaredDistance(const XMFLOAT3& position1, const XMFLOAT3& position2);

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };
	XMFLOAT3 target = { 0,0,0 };
	float speed = 0.2f;
	float timer = 0.0f;

	const float zeroFloat = 0.0f;
	const float zeroPointFiveFloat = 0.5f;

	bool esaSet = false;
	bool tekiSet = false;

	float randomCooldown = 0.0f;

	bool moving = false;

	const float defaultUpdateEntitiesPosition = 2500.0f;

	const int mapBoundaries = 150;
	const int mapBoundariesTimesTwo = 300;

	const float timerOneFrame = 1.0f;

	const float standingTimer = 300.0f;
	const float wanderMaxTimer = 600.0f;

	const float timerReset = 0.0f;
	const int randomCooldownTime = 30;
	const int randomCooldownTimesTwo = 60;

	const float toriRadius = 5.0f;
	const float tekiDistance = 100.0f;
	const float esaMaxDistance = 60.0f;
	const float esaMinDistance = 5.0f;

	const float tekiFleeMultiplier = 2.0f;
	const int tekiRandomVarience = 21;
	const float divideByTen = 10.0f;

	float x = 0.0f;
	float z = 0.0f;
	float degrees = 0.0f;
	float yRotationOffset = 90.0f;
public:
	// �f�X�t���O
	bool goalFlag = false;
};