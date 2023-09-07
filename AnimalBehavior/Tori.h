#pragma once
#include "ObjObject.h"

class Tori :
	public ObjObject
{
protected:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
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

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static std::unique_ptr<Tori>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale);

public: // メンバ関数
	// 初期化
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale);

	// 毎フレーム処理
	void Update();

	void SetNewMovementPosition();
	void Move(bool forwardBackwards); // true = forwards

	void UpdateEntitiesInRange(XMFLOAT3 tekiPosition, XMFLOAT3 esaPosition, XMFLOAT3 playerPosition);
	bool ItemIntersection(float radius1, const XMFLOAT3& center2, float radius2);
	void EsaInRange(XMFLOAT3 esaPosition);
	void TekiInRange(XMFLOAT3 tekiPosition, XMFLOAT3 playerPosition);

	float SquaredDistance(const XMFLOAT3& position1, const XMFLOAT3& position2);

private: // メンバ変数
	XMFLOAT3 velocity = { 0,0,0 };
	XMFLOAT3 target = { 0,0,0 };
	float speed = 0.2f;
	float timer = 0.0f;

	bool esaSet = false;
	bool tekiSet = false;

	float randomCooldown = 0.0f;

	bool moving = false;

	float x = 0.0f;
	float z = 0.0f;
	float degrees = 0.0f;
	float yRotationOffset = 90.0f;
public:
	// デスフラグ
	bool goalFlag = false;
};