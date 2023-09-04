#pragma once
#include "ObjObject.h"
#include "Lerp.h"
#include "Easing.h"

class BossShield :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static BossShield* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数

public: // メンバ関数
// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// ヒット時のカラー変更
	void HitChangeColor();

	// 生存時のカラー変更
	void AliveChangeColor();

	// 反射時のモーション
	void ReflectMotion();

	void Revival();

private: // メンバ変数
	ObjModel* modelBossPartsShield = nullptr;

	// 最大の体力
	float lifeMax = 10.0f;

	// 基本のカラー
	XMFLOAT4 baseColor = { 0.9f, 0.2f, 0.2f, 1.0f };
	// ヒット時のカラー
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 破壊後のカラー
	XMFLOAT4 breakColor = { 0.4f, 0.2f, 0.2f, 1.0f };
	// 生存時のカラー
	XMFLOAT4 aliveColor = {};

public:
	// 生存フラグ
	bool isAlive = true;

	// 現在の体力
	float life = lifeMax;

	// タイムレート
	float timeRate = 1.0f;

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;

	// カラー変更タイムレート
	float colorTimeRate2 = 1.0f;

	float revivalTimeRate = 1.0f;

	bool isRevival = false;
};

