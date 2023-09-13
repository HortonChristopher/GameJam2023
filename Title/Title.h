#pragma once

#include "SafeDelete.h"

#include "Buta.h"
#include "Hitsuji.h"
#include "Ushi.h"
#include <array>

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
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

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "ObjectParticle.h"
#include "Sound.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Test.h"

class Title : public BaseScene
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
		title_bg,
		titlefont,
		pressspace,
		black
	};

public:
	// コンストクラタ
	Title();

	// デストラクタ
	~Title();

	bool IsTooClose(const XMFLOAT3& point1, const XMFLOAT3& point2, float minDistance);
	XMFLOAT3 GenerateRandomCoordinate(float innerRadius, float outerRadius, float angleMin, float angleMax, const std::vector<XMFLOAT3>& existingPoints, float minDistance);
	XMFLOAT3 CalculateRotation(const XMFLOAT3& coordinate);
	std::pair<std::vector<XMFLOAT3>, std::vector<XMFLOAT3>> GenerateCoordinatesAndRotations();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// For Fences
	void FenceCreation();
	void UpdateFences();
	void DrawFences();

private: // メンバ変数
	//DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//入力
	Input* input = Input::GetInstance();

	//音声
	Sound* sound = Sound::GetInstance();

	//カメラ
	Camera* camera = nullptr;

	//スプライト
	Sprite* titleBG = nullptr;

	ObjObject* ground = nullptr;
	ObjObject* skydome = nullptr;

	// Animal and Item lists
	std::list<std::unique_ptr<Buta>> butaList;
	std::list<std::unique_ptr<Hitsuji>> hitsujiList;
	std::list<std::unique_ptr<Ushi>> ushiList;

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
};