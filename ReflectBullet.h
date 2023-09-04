#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class ReflectBullet :
	public ObjObject
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static std::unique_ptr<ReflectBullet>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

private: // 静的メンバ変数
	// 消えるまでの時間
	static const int32_t LifeTime = 360;

public: // メンバ関数
	// 初期化
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

	// 毎フレーム処理
	void Update(const XMFLOAT3 reflect_position);

	// ヒット時のカラー変更
	void HitChangeColor();

	const bool GetDeathFlag() { return deathFlag; }

	void RallyReset(const XMFLOAT3 position, const XMFLOAT3 target, const float speed, const float life);

private: // メンバ変数
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// 最大の体力
	float lifeMax = 7.0f;

	// 基本のカラー
	XMFLOAT4 baseColor = { 0.9f, 0.5f, 0.2f, 1.0f };
	// ヒット時のカラー
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// デスタイマー 
	int32_t deathTimer = LifeTime;

public:
	// デスフラグ
	bool deathFlag = false;

	bool isReflect = false;

	int rallyCount = 1;

	// 現在の体力
	float life = lifeMax;

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;

	XMFLOAT3 prevPosition = { 0.0f, 0.0f, 0.0f };
};

