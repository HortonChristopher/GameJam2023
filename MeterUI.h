#pragma once

#include "Sprite.h"
#include "Lerp.h"
#include "Easing.h"

class MeterUI
{
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
// スプライトのテクスチャ番号
	static enum TextureNumber
	{
		debug_txt,
		score_txt,
		game_bg,
		reticle,
		black,
		rule,
		speed,
		meter,
		process,
		breakshield,
		breakmaincore,
		breakcore,

		// ボス
		game_boss_frame_1,
		game_boss_frame_2,
		game_boss_gauge
	};

public: // 静的メンバ関数
// インスタンス生成
	static MeterUI* Create(XMFLOAT2 position, float rotation, XMFLOAT4 color);

	bool Initialize(XMFLOAT2 position, float rotation, XMFLOAT4 color);

	void Update(float nowVal, float maxVal, XMFLOAT2 position);

	void Draw();

private:
	Sprite* meterBase = nullptr;
	Sprite* meterNeedle = nullptr;

	float nowRotation = 0.0f;
	float maxRotation = 360.0f;
	float EasRotation = {};

	float ratio = {};

	XMFLOAT4 gageColor = {};
};

