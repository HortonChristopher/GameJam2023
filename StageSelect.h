#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include <sstream>
#include <iomanip>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Lerp.h"
#include "Boss.h"
#include "BossTurret.h"
#include "BossCore.h"
#include "BossMainCore.h"
#include "BossParts.h"
#include "BossShield.h"

class Boss;
class BossCore;
class BossMainCore;
class BossParts;
class BossTurret;
class BossShield;

class StageSelect : public BaseScene
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
		result_bg,
		black,
		triangle_up,
		triangle_down,
		stage_select,
		stage01,
		stage02,
		key_w,
		key_s,
		key_space,

		explanation1,
		explanation2,
	};

public:
	// コンストラクタ
	StageSelect();

	// デストラクタ
	~StageSelect();

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

	Camera* camera = nullptr;

	Sprite* stageSelectBG = nullptr;
	Sprite* Black = nullptr;

	Sprite* setsumei1 = nullptr;
	Sprite* setsumei2 = nullptr;

	int explanationPage = 0;
	
	bool changeTimerBool = false;
	const float changeTimerMax = 30.0f;
	float changeTimer = 0.0f;

	//黒背景のアルファ値
	float BlackAlpha = 1.0f;
};

