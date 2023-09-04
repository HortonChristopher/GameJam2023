#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class BossTurret :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static BossTurret* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数

public: // メンバ関数
// 初期化
	bool Initialize();

	// 毎フレーム処理
	void Update(XMFLOAT3 target_position);

	// ヒット時のカラー変更
	void HitChangeColor();

	const float& GetExternRotationY() { return externRotationY; }

private: // メンバ変数
	Input* input = Input::GetInstance();

	ObjModel* modelBossTurret = nullptr;

	float limitRot = 30.0f;

	// 最大の体力
	float lifeMax = 10.0f;

	// 基本のカラー
	XMFLOAT4 baseColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	// ヒット時のカラー
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 撃破時の角度
	XMFLOAT3 breakRotation = { 30.0f, 0.0f, 0.0f };

	float externRotationY = 0.0f;

public:
	// 生存フラグ
	bool isAlive = true;

	// 現在の体力
	float life = lifeMax;

	bool isShot = false;

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;

	// 角度変更タイムレート
	float rotationTimeRate = 0.0f;

	// 弾の発射間隔
	static const int32_t ShotInterval = 60;

	// 発射タイマー
	int32_t shotTimer = 240;
};

