#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Lerp.h"
#include "ScoreManager.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <sstream>
#include <iomanip>
#include <DirectXMath.h>

class Result : public BaseScene
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

		pause_menu,
		pause_menu_s1,
		pause_menu_s2,
		pause_menu_s3,

		game_gtxt_1,
		score_base,
		timer_base,
		item_base,
		bonus_base,
		esa_icon,
		teki_icon,
		game_gtxt_2,
		result
	};

public:
	// コンストラクタ
	Result();

	// デストラクタ
	~Result();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	ScoreManager* scoreManager = ScoreManager::GetInstance();
	ScoreText scoreText;

	Camera* camera = nullptr;

	Sprite* stageSelectBG = nullptr;

	Sprite* resultPage = nullptr;

	Sprite* cowItemIcon = nullptr;
	Sprite* sheepItemIcon = nullptr;
	Sprite* pigItemIcon = nullptr;
};