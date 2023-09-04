#pragma once
#include "ObjObject.h"
#include "ParticleManager.h"
#include "Lerp.h"
#include "BossCore.h"

class BossMainCore :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static BossMainCore* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数

public: // メンバ関数
// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// 座標移動
	void ChangePosition();

	// ヒット時のカラー変更
	void HitChangeColor();

	// 生存時のカラー変更
	void AliveChangeColor();

	//タイマーリセット
	void TimerReset(int Timer, int ResetValue);

	// 値をリセット
	void VarReset();

	//ゲッター
	const int& GetLife() { return life; }
	const int& GetDestroyPartTime() { return DestroyPartTime; }
	const bool& GetAliveFlag() { return isAlive; }

	//カラーゲッター
	const XMFLOAT4& GetColorRed() { return Red; }
	const XMFLOAT4& GetColorYellow() { return Yellow; }
	const XMFLOAT4& GetColorOrange() { return Orange; }

private: // メンバ変数
	ObjModel* modelBossPartsSphere = nullptr;

	// コア解放時の座標
	XMFLOAT3 OnPosition = { 0.0f, 0.9f, 0.0f };

	// コア閉鎖時の座標
	XMFLOAT3 OffPosition = { 0.0f, 0.0f, 0.0f };

	// 基本のカラー
	XMFLOAT4 baseColor = { 0.9f, 0.2f, 0.2f, 1.0f };
	// ヒット時のカラー
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 破壊後のカラー
	XMFLOAT4 breakColor = { 0.4f, 0.2f, 0.2f, 1.0f };
	// 生存時のカラー
	XMFLOAT4 aliveColor = {};

	//ボスパーツ破壊パーティクルのカラー
	XMFLOAT4 Red = { 1.0f, 0.1f, 0.1f, 1.0f };
	XMFLOAT4 Yellow = { 0.905f, 0.870f, 0.396f, 1.0f };
	XMFLOAT4 Orange = { 0.964f, 0.470f, 0.117, 1.0f };

	//破壊パーティクル用タイマー
	int DestroyPartTime = 20;
	//デフォルト値
	int DestroyDefault = 20;

	// 座標移動タイムレート
	float positionTimeRate = 0.0f;

public:
	// 生存フラグ
	bool isAlive = true;

	// メインコアの撃破
	bool isBreak = false;


	// 最大の体力
	float lifeMax = 40.0f;
	// 現在の体力
	float life = lifeMax;

	float lifeRatio = {};

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;

	// カラー変更タイムレート
	float colorTimeRate2 = 1.0f;

	// メインコアの出現時間
	float onTimer = 400.0f;
};

