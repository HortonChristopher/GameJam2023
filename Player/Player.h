#pragma once

#include "DirectXCommon.h"
#include "ObjObject.h"
#include "GamePlay.h"
#include "Input.h"
#include "Camera.h"
#include "DebugText.h"
#include "Vector.h"
#include "Easing.h"
#include "Lerp.h"

#include <cmath>

class Player :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数

	enum State
	{
		Stop,	//接近
		Walk,	//歩き
		Run,	//走り
		Throw,	//投げる
		Call	//呼ぶ
	};

	// 3Dオブジェクト生成
	static Player* Create(ObjModel* model = nullptr);

private:
	static ID3D12Device* device;

	//static Camera* camera;

public: // メンバ関数
	// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	void DebugTextUpdate();

	void DebugTextDraw();

	void Move();

	void Rolling();

	void Boost();

	void BoostTest();

	bool CheckCollisionWithBoss(XMFLOAT3 bossPos, float collisionRadius);

	void MoveTowards(float& current, float target, float speed, float elapsedTime);

	//ゲッター
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetVel() { return Vel; }

	const bool& GetBoostFlag() { return BoostFlag; }

	const XMFLOAT4& GetBoostPartColor() { return BoostPartColor; }

	const float& GetBoostPowNow() { return BoostPowNow; }

	const float& GetBoostPowMax() { return BoostPowMax; }

	const float& GetTotalSpeed() { return totalSpeed; }

	const float& GetNormalShootSpeedMax() { return normalShootSpeedMax; }

	const float& GetChargeShootSpeedMax() { return chargeShootSpeedMax; }

	const float& GetShootSpeedMin() { return shootSpeedMin; }

	const float& GetTotalSpeedMax() { return baseSpeed + boostSpeedMax; }

	/*const int& GetStateInfo() { return StateNum; }*/

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DebugText debugText;

	ObjModel* modelPlayer = nullptr;

	float rotationSpeed = 30.0f;
	float speed = 1.0f;

	XMFLOAT3 playerSpeed = {};

	XMFLOAT3 rollRotation = {};
	XMFLOAT3 axis = { 0.0f, 0.0f, 0.0f };
	float xAngle = 90.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float hypotenuse = 0.0f;
	float radians = 0.0f;
	float radians2 = 0.0f;
	float degrees = 90.0f;
	float degrees2 = 0.0f;
	float yChange = 0.0f;
	float collisionSphereRadius = 3.0f;

	const float rotateSpeed = 22.5f; // Currently using frame time, 1350.0f if using delta time

	//プレイヤースピード
	float totalSpeed = 2.0f;

	XMFLOAT3 Vel = {};
	float yVel = 0.0f;

	//重力加速度
	float Gravity = 9.8f;

	// プ―スト体力の最大値
	float BoostPowMax = 100.0f;

	//ブースト体力
	float BoostPowNow = BoostPowMax;

	//ブーストフラグ
	bool BoostFlag = false;

	//ブーストパーティクルカラー
	XMFLOAT4 BoostPartColor = {};

	// ブースト時の速度
	float boostSpeedMax = 1.0f;
	float boostSpeedMin = 0.0f;
	float boostSpeed = boostSpeedMin;
	// ブースト速度の線分補間レート
	float boostTimeRate = 0.0f;

	// 通常時の速度
	float baseSpeed = 2.0f;

	// 弾発射時の速度
	float normalShootSpeedMax = 1.0f;
	float chargeShootSpeedMax = 2.0f;
	float shootSpeedMin = 0.0f;

	const float mapBoundaries = 149.0f;

	////プレイヤーの状態
	//State phase_ = State::Stop;

	//int StateNum = 0;

public:
	Vector3 direction = { 0, 0, 1 };
	Vector3 moveDirection = {};

	XMFLOAT3 prevPos = { 0.0f, 0.0f, 0.0f };

	// 弾発射時の速度
	float shootSpeed = 0.0f;
	float shootSpeedTimeRate = 0.0f;

	float HPMAX = 15.0f;
	float HP = HPMAX;

	bool boostCancel = false;

	//移動キャンセル変数
	bool MoveCancel_T = false;
	int MoveCancelTimer_T = 0;

	bool MoveCancel_C = false;
	int MoveCancelTimer_C = 0;

};