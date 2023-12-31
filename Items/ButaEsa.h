#pragma once
#include "ObjObject.h"

class ButaEsa :
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

private: // 静的メンバ変数
	// 消えるまでの時間
	static const int32_t LifeTime = 240;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static std::unique_ptr<ButaEsa>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale);

public: // メンバ関数
	// 初期化
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale);

	// 毎フレーム処理
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // メンバ変数
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// デスタイマー 
	int32_t deathTimer = LifeTime;

public:
	// デスフラグ
	bool deathFlag = false;
};