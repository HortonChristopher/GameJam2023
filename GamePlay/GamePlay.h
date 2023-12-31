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
#include "ButaEsa.h"
#include "ButaTeki.h"
#include "HitsujiEsa.h"
#include "HitsujiTeki.h"
#include "UshiEsa.h"
#include "UshiTeki.h"

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

#include "ScoreManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>
#include <map>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

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
		finish,
	};

	enum AnimalSelection
	{
		BUTA = 0,
		HITSUJI = 1,
		USHI = 2
	};
	AnimalSelection animalSelection = BUTA;

	enum ItemSelection
	{
		BUTA_ESA = 0,
		BUTA_TEKI = 1,
		HITSUJI_ESA = 2,
		HITSUJI_TEKI = 3,
		USHI_ESA = 4,
		USHI_TEKI = 5
	};
	ItemSelection itemSelection = BUTA_ESA;

public: // メンバ関数

	// コンストクラタ
	GamePlay();

	// デストラクタ
	~GamePlay();

	bool IsTooClose(const XMFLOAT3& point1, const XMFLOAT3& point2, float minDistance);
	XMFLOAT3 GenerateRandomCoordinate(float innerRadius, float outerRadius, float angleMin, float angleMax, const std::vector<XMFLOAT3>& existingPoints, float minDistance);
	XMFLOAT3 CalculateRotation(const XMFLOAT3& coordinate);
	std::pair<std::vector<XMFLOAT3>, std::vector<XMFLOAT3>> GenerateCoordinatesAndRotations();


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

	void CameraInitialization();
	void SoundInitialization();
	void TextureInitialization();
	void SpriteInitialization();
	void DeviceAndPipelineInitialization();

	void GateInitialization();

	void PlayerFBXInitialization();

	void ObjObjectInitialization();

	void FenceCreation();
	void UpdateFences();
	void DrawFences();

	void ParticleInitialization();

	void InitialAnimalInitialization();

	const int& GetPlayerState() { return PlayerState; }

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;
	ScoreText scoreText;
	ScoreManager* scoreManager = ScoreManager::GetInstance();
	
	Camera* camera = nullptr;

	ParticleManager* Particle = nullptr;
	ParticleManager* CallPart = nullptr;
	ParticleManager* FoodPart = nullptr;

	// Animal and Item lists
	std::list<std::unique_ptr<Tori>> toriList;
	std::list<std::unique_ptr<Buta>> butaList;
	std::list<std::unique_ptr<Hitsuji>> hitsujiList;
	std::list<std::unique_ptr<Ushi>> ushiList;
	std::list<std::unique_ptr<Esa>> esaList;
	std::list<std::unique_ptr<Teki>> tekiList;
	std::list<std::unique_ptr<ButaEsa>> butaEsaList;
	std::list<std::unique_ptr<HitsujiEsa>> hitsujiEsaList;
	std::list<std::unique_ptr<UshiEsa>> ushiEsaList;
	std::list<std::unique_ptr<ButaTeki>> butaTekiList;
	std::list<std::unique_ptr<HitsujiTeki>> hitsujiTekiList;
	std::list<std::unique_ptr<UshiTeki>> ushiTekiList;

	Sprite* gameBG = nullptr;
	Sprite* Reticle = nullptr;
	Sprite* StoragePos = nullptr;

	Sprite* cowIcon = nullptr;
	Sprite* sheepIcon = nullptr;
	Sprite* pigIcon = nullptr;

	Sprite* pauseMenu = nullptr;
	Sprite* pauseMenuS1 = nullptr;
	Sprite* pauseMenuS2 = nullptr;
	Sprite* pauseMenuS3 = nullptr;

	Sprite* cowItemIcon = nullptr;
	Sprite* sheepItemIcon = nullptr;
	Sprite* pigItemIcon = nullptr;

	Sprite* score_gtxt_1 = nullptr;
	Sprite* bonus_gtxt_2 = nullptr;

	Sprite* scoreBase = nullptr;
	Sprite* itemBase = nullptr;

	Sprite* esaIcon = nullptr;
	Sprite* tekiIcon = nullptr;

	//黒背景
	Sprite* Black = nullptr;

	//フィニッシュ
	Sprite* Finish = nullptr;

	GageUI* bonusGage = nullptr;

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

	// タイマーUI
	MeterUI* meterTimer = nullptr;

	// Spawn barn
	ObjObject* barn = nullptr;
	ObjModel* modelBarn = nullptr;

	ObjModel* modelGround = nullptr;
	ObjModel* modelSkydome = nullptr;
	ObjModel* modelFence = nullptr;

	ObjModel* modelBullet = nullptr;

	ObjModel* modelEsa = nullptr;

	ObjModel* modelPig = nullptr;
	ObjModel* modelSheep = nullptr;
	ObjModel* modelHorse = nullptr;

	// Gates
	ObjObject* pigGate = nullptr;
	ObjModel* pigGateModel = nullptr;
	ObjObject* sheepGate = nullptr;
	ObjModel* sheepGateModel = nullptr;
	ObjObject* cowGate = nullptr;
	ObjModel* cowGateModel = nullptr;

	ObjObject* pigSign = nullptr;
	ObjModel* pigSignModel = nullptr;
	ObjObject* sheepSign = nullptr;
	ObjModel* sheepSignModel = nullptr;
	ObjObject* cowSign = nullptr;
	ObjModel* cowSignModel = nullptr;

	ObjObject* siloObject = nullptr;
	ObjModel* siloModel = nullptr;
	std::array<ObjObject*, 20> trees = { {} };
	std::array<ObjObject*, 80> randomTrees = { {} };
	ObjModel* treeModel = nullptr;

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
	FbxObject3d* objPlayerCall = nullptr;

	bool butaEsa = false;
	bool butaTeki = false;
	bool hitsujiEsa = false;
	bool hitsujiTeki = false;
	bool ushiEsa = false;
	bool ushiTeki = false;

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
	//int goalPigs = 0;
	const int goalPigsMax = 20;
	//int goalSheep = 0;
	const int goalSheepMax = 20;
	//int goalHorse = 0;
	const int goalHorseMax = 20;
	//float score = 0.0f;

	bool pigBonusTime = false;
	bool sheepBonusTime = false;
	bool cowBonusTime = false;
	float pigBonusTimeRemaining = 0.0f;
	float pigBonusTimeEntries = 0.0f;
	float sheepBonusTimeRemaining = 0.0f;
	float sheepBonusTimeEntries = 0.0f;
	float cowBonusTimeRemaining = 0.0f;
	float cowBonusTimeEntries = 0.0f;
	const float pigBonusTimeMax = 3600.0f;
	const float sheepBonusTimeMax = 3600.0f;
	const float cowBonusTimeMax = 3600.0f;

	bool bonusTime = false;
	float bonusTimeRemaining = 0.0f;
	float bonusTimeEntries = 0.0f;
	const float bonusTimeMax = 3600.0f;

	bool pause = false;
	int pauseSelection = 0;

	// タイマーを変更したい場合は、この数字を変更してください。
	// 希望の秒数に60をかける。
	float frameTimer = 7200.0f;
	// これは変更しないでください。
	// タイマーUIに使用する。
	const float timerMax = 120.0f;
	int timer = 120;

	//FBXオブジェクト

	//レティクル座標
	XMFLOAT2 ReticlePos = { 0.0f, 0.0f };

	//マウス座標
	POINT mousePosition;

	//プレイヤーの状態の変数
	int PlayerState = 0;

	//FBXのアニメーションタイマー
	int AnimationTimer_T = 0;
	int AnimationTimer_C = 0;

	//アニメーションタイマー切り替え用フラグ
	bool AnimationFlag_T = false;
	bool AnimationFlag_C = false;

	//アニメーションフレーム数
	int ThrowFrame = 132;
	int CallFrame = 191;

	//パーティクル発生用座標
	XMFLOAT3 PigGate_Left = {};
	XMFLOAT3 PigGate_Right = {};

	XMFLOAT3 HitsujiGate_Left = {};
	XMFLOAT3 HitsujiGate_Right = {};

	XMFLOAT3 UshiGate_Left = {};
	XMFLOAT3 UshiGate_Right = {};

	XMFLOAT3 CallPartPos = {};

	XMFLOAT3 PigPartPos = {};
	XMFLOAT3 SheepPartPos = {};
	XMFLOAT3 CowPartPos = {};

	XMFLOAT3 PigPartPos_F = {};
	XMFLOAT3 SheepPartPos_F = {};
	XMFLOAT3 CowPartPos_F = {};

	//黒背景のアルファ値
	float BlackAlpha = 1.0f;

	bool BlackFlag = true;

	bool FinishFlag = false;

	//フィニッシュSE再生タイマー
	int FinishTimer = 0;
	bool FinishSEFlag = false;

	//フィニッシュアルファ
	float FinishAlpha = 0.0f;
};