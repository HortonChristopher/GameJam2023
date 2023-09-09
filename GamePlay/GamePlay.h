﻿#pragma once

#include "BaseScene.h"

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Sound.h"
#include "FbxObject.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject.h"
#include "WinApp.h"

#include "Tori.h"
#include "Buta.h"
#include "Hitsuji.h"
#include "Ushi.h"
#include "Esa.h"
#include "Teki.h"

#include "Player.h"
#include "Bullet.h"
#include "Boss.h"
#include "BossTurret.h"
#include "BossCore.h"
#include "BossMainCore.h"
#include "BossParts.h"
#include "TargetBullet.h"
#include "ObjectParticle.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"

#include "SceneManager.h"

#include "GageUI.h"
#include "DeltaGageUI.h"
#include "collision.h"

#include "MeterUI.h"
#include "ProcessUI.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>

#include <limits>

class Player;
class Bullet;
class PlayerSpecialBullet;
class Boss;
class BossBullet;
class TargetBullet;
class StageObject;
class BossCore;
class BossMainCore;
class BossParts;
class BossTurret;
class GageUI;
class DeltaGageUI;
class MeterUI;
class ProcessUI;

// ゲームシーン
class GamePlay : public BaseScene
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

	// プレイヤーの球
	static enum BulletType
	{
		Normal,
		Charge
	};

public: // メンバ関数

	// コンストクラタ
	GamePlay();

	// デストラクタ
	~GamePlay();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	//マウス情報取得
	void GetMouse();

	//デバッグテキスト用関数
	void DrawDebugText();

	void FenceCreation();
	void UpdateFences();
	void DrawFences();

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;
	
	Camera* camera = nullptr;

	ParticleManager* circleParticle = nullptr;

	// Animal and Item lists
	std::list<std::unique_ptr<Tori>> toriList;
	std::list<std::unique_ptr<Buta>> butaList;
	std::list<std::unique_ptr<Hitsuji>> hitsujiList;
	std::list<std::unique_ptr<Ushi>> ushiList;
	std::list<std::unique_ptr<Esa>> esaList;
	std::list<std::unique_ptr<Teki>> tekiList;

	Sprite* gameBG = nullptr;
	Sprite* Reticle = nullptr;
	Sprite* StoragePos = nullptr;

	ObjObject* ground = nullptr;
	ObjObject* skydome = nullptr;
	// Fences
	std::array<ObjObject*, 6> westSideNorthFence = { {} };
	std::array<ObjObject*, 6> westSideSouthFence = { {} };
	std::array<ObjObject*, 6> southSideWestFence = { {} };
	std::array<ObjObject*, 6> southSideEastFence = { {} };
	std::array<ObjObject*, 6> eastSideSouthFence = { {} };
	std::array<ObjObject*, 6> eastSideNorthFence = { {} };
	std::array<ObjObject*, 3> westGoalSouthFence = { {} };
	std::array<ObjObject*, 7> westGoalWestFence = { {} };
	std::array<ObjObject*, 3> westGoalNorthFence = { {} };
	std::array<ObjObject*, 3> southGoalWestFence = { {} };
	std::array<ObjObject*, 7> southGoalSouthFence = { {} };
	std::array<ObjObject*, 3> southGoalEastFence = { {} };
	std::array<ObjObject*, 3> eastGoalSouthFence = { {} };
	std::array<ObjObject*, 7> eastGoalEastFence = { {} };
	std::array<ObjObject*, 3> eastGoalNorthFence = { {} };
	std::array<ObjObject*, 6> northSideWestFence = { {} };
	std::array<ObjObject*, 6> northSideEastFence = { {} };
	std::array<ObjObject*, 2> barnWestFence = { {} };
	std::array<ObjObject*, 2> barnEastFence = { {} };

	// Spawn barn
	ObjObject* barn = nullptr;
	ObjModel* modelBarn = nullptr;

	ObjModel* modelGround = nullptr;
	ObjModel* modelSkydome = nullptr;
	ObjModel* modelFence = nullptr;

	ObjModel* modelBullet = nullptr;

	ObjModel* modelPig = nullptr;
	ObjModel* modelSheep = nullptr;
	ObjModel* modelHorse = nullptr;

	Player* player = nullptr;

	//FBXモデル
	FbxModel* modelPlayerStop = nullptr;
	FbxModel* modelPlayerWalking = nullptr;
	FbxModel* modelPlayerRun = nullptr;
	FbxModel* modelPlayerThrow = nullptr;
	FbxModel* modelPlayerCall = nullptr;

	//FBXオブジェクト
	FbxObject3d* objPlayerStop = nullptr;
	FbxObject3d* objPlayerWalking = nullptr;
	FbxObject3d* objPlayerRun = nullptr;
	FbxObject3d* objPlayerThrow = nullptr;
	FbxObject3d* PlayerCall = nullptr;

	// Temporary
	int pigNumber = 2;
	const int pigNumberMax = 10;
	int sheepNumber = 2;
	const int sheepNumberMax = 10;
	int horseNumber = 2;
	const int horseNumberMax = 10;
	float pigRespawn = 0.0f;
	const float pigRespawnMax = 240.0f;
	float sheepRespawn = 0.0f;
	const float sheepRespawnMax = 360.0f;
	float horseRespawn = 0.0f;
	const float horseRespawnMax = 300.0f;
	int goalPigs = 0;
	const int goalPigsMax = 20;
	int goalSheep = 0;
	const int goalSheepMax = 20;
	int goalHorse = 0;
	const int goalHorseMax = 20;
	float score = 0.0f;

	// タイマーを変更したい場合は、この数字を変更してください。
	// 希望の秒数に60をかける。
	float frameTimer = 3600.0f;
	// これは変更しないでください。
	// タイマーUIに使用する。
	int timer = 60;

	//FBXオブジェクト

	//レティクル座標
	XMFLOAT2 ReticlePos = { 0.0f, 0.0f };

	//マウス座標
	POINT mousePosition;
};