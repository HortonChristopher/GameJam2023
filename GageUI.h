#pragma once

#include "Sprite.h"
#include "Lerp.h"

class GageUI
{
private: // エイリアス
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
		game_boss_gauge,

		cow_icon,
		sheep_icon,
		pig_icon,

		game_gtxt_1,
		score_base,
		timer_base,
		item_base,
		bonus_base
	};

public: // 静的メンバ関数
// インスタンス生成
	static GageUI* Create(XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color);

	bool Initialize(XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color);

	void Update(float nowVal, float maxVal, XMFLOAT2 position, XMFLOAT4 start_color, XMFLOAT4 end_color);

	void Draw();

private:
	Sprite* gageBase = nullptr;
	Sprite* gageMain = nullptr;
	
	XMFLOAT2 nowSize = { 384.0f, 25.0f };
	XMFLOAT2 maxSize = {};

	float ratio = {};

	XMFLOAT4 gageColor = {};
};