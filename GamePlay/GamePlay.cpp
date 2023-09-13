#include "GamePlay.h"
#include "FbxObject.h"

using namespace DirectX;
extern HWND hwnd;
extern bool run;

extern bool victory;

GamePlay::GamePlay()
{
}

GamePlay::~GamePlay()
{
}

bool GamePlay::IsTooClose(const XMFLOAT3& point1, const XMFLOAT3& point2, float minDistance) {
	float dx = point1.x - point2.x;
	float dz = point1.z - point2.z;
	return dx * dx + dz * dz < minDistance * minDistance;
}

XMFLOAT3 GamePlay::GenerateRandomCoordinate(float innerRadius, float outerRadius, float angleMin, float angleMax, const std::vector<XMFLOAT3>&existingPoints, float minDistance) {
	XMFLOAT3 coordinate;
	bool valid;
	do {
		valid = true;

		// Generate a random angle between angleMin and angleMax
		float angle = angleMin + (float(rand()) / RAND_MAX) * (angleMax - angleMin);

		// Generate a random radius between innerRadius and outerRadius
		float radius = innerRadius + (float(rand()) / RAND_MAX) * (outerRadius - innerRadius);

		// Convert polar coordinates (angle & radius) to Cartesian coordinates
		coordinate.x = radius * cos(angle);
		coordinate.y = 0.0f;
		coordinate.z = radius * sin(angle);

		for (const XMFLOAT3& existingPoint : existingPoints) {
			if (IsTooClose(coordinate, existingPoint, minDistance)) {
				valid = false;
				break;
			}
		}
	} while (!valid);

	return coordinate;
}

XMFLOAT3 GamePlay::CalculateRotation(const XMFLOAT3& coordinate) {
	float yaw = atan2(-coordinate.z, -coordinate.x);  // Note the '-' to make it face towards (0, 0, 0)
	return XMFLOAT3(0.0f, yaw, 0.0f);
}

std::pair<std::vector<XMFLOAT3>, std::vector<XMFLOAT3>> GamePlay::GenerateCoordinatesAndRotations() {
	// Seed the random number generator
	std::srand(std::time(0));

	std::vector<XMFLOAT3> allCoordinates;
	std::vector<XMFLOAT3> allRotations;

	// Generate 20 coordinates for each of the 4 quadrants
	for (int quadrant = 0; quadrant < 4; ++quadrant) {
		float angleMin = float(quadrant) * 0.5f * 3.14159265358979323846f;
		float angleMax = float(quadrant + 1) * 0.5f * 3.14159265358979323846f;

		float rotationValue = 0.0f;

		// Manually set the rotation based on the quadrant
		switch (quadrant) {
		case 0: rotationValue = 0.0f; break; // North-East
		case 1: rotationValue = 4.71238898038f; break; // South-East (90 degrees in radians)
		case 2: rotationValue = 3.14159265359f; break; // South-West (180 degrees in radians)
		case 3: rotationValue = 1.57079632679f; break; // North-West (270 degrees in radians)
		}

		for (int i = 0; i < 20; ++i) {
			XMFLOAT3 coordinate = GenerateRandomCoordinate(300.0f, 450.0f, angleMin, angleMax, allCoordinates, 10.0f);
			allCoordinates.push_back(coordinate);

			// Set rotation manually
			XMFLOAT3 rotation = XMFLOAT3(0.0f, rotationValue, 0.0f);

			allRotations.push_back(rotation);
		}
	}

	return { allCoordinates, allRotations };
}

void GamePlay::Initialize()
{
	// Seed the random number generator
	std::srand(std::time(0));

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);
	Player::SetCamera(camera);
	Boss::SetCamera(camera);

	//FBXカメラセット
	FbxObject3d::SetCamera(camera);

	// 座標のセット
	camera->title = false;
	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetEye({ 0, 2, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(96.0f);

	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// サウンド初期化
	sound->Initialize();

	sound->LoadWav("SE/Game/Call.wav");
	sound->LoadWav("SE/Game/GameFinish.wav");
	sound->LoadWav("SE/Game/Goal.wav");
	sound->LoadWav("SE/Game/Throw.wav");
	sound->LoadWav("SE/Game/Decide.wav");
	sound->LoadWav("SE/Game/Menu.wav");
	sound->LoadWav("SE/Game/MenuMove.wav");
	sound->LoadWav("SE/Game/GameBGM.wav");


	if (!Sprite::LoadTexture(TextureNumber::game_bg, L"Resources/Sprite/GameUI/game_bg.png")) {
		assert(0);
		return;
	}

	//レティクルテクスチャ
	if (!Sprite::LoadTexture(TextureNumber::reticle, L"Resources/Re.png")) {
		assert(0);
		return;
	}

	// ゲージUI
	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_1, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_2, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_gauge, L"Resources/Sprite/GameUI/BossHpUI/game_boss_gauge.png")) {
		assert(0);
		return;
	}

	//黒背景
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	//黒背景
	if (!Sprite::LoadTexture(TextureNumber::rule, L"Resources/Sprite/TitleUI/Rule.png")) {
		assert(0);
		return;
	}

	// スピード
	if (!Sprite::LoadTexture(TextureNumber::speed, L"Resources/Sprite/GameUI/timerNeedle.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::meter, L"Resources/Sprite/GameUI/meter.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::process, L"Resources/Sprite/GameUI/process.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::breakshield, L"Resources/Sprite/GameUI/breakshield.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::breakmaincore, L"Resources/Sprite/GameUI/breakmaincore.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::breakcore, L"Resources/Sprite/GameUI/breakcore.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::cow_icon, L"Resources/Sprite/GameUI/CowIcon.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::sheep_icon, L"Resources/Sprite/GameUI/SheepIcon.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pig_icon, L"Resources/Sprite/GameUI/PigIcon.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_1, L"Resources/Sprite/GameUI/game_gtxt_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::score_base, L"Resources/Sprite/GameUI/scoreBase.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::timer_base, L"Resources/Sprite/GameUI/timerBase.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::item_base, L"Resources/Sprite/GameUI/itemBase.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::bonus_base, L"Resources/Sprite/GameUI/bonusBase.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::esa_icon, L"Resources/Sprite/GameUI/Esa.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::teki_icon, L"Resources/Sprite/GameUI/Teki.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pause_menu, L"Resources/Sprite/GameUI/PauseMenu.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pause_menu_s1, L"Resources/Sprite/GameUI/PauseMenuS1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pause_menu_s2, L"Resources/Sprite/GameUI/PauseMenuS2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pause_menu_s3, L"Resources/Sprite/GameUI/PauseMenuS3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_2, L"Resources/Sprite/GameUI/game_gtxt_2.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);
	// スコアテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(score_txt, L"Resources/Sprite/Common/common_dtxt_2.png")) {
		assert(0);
		return;
	}
	// スコアテキスト初期化
	scoreText.Initialize(score_txt);

	// スプライト
	gameBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	StoragePos = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	cowIcon = Sprite::Create(TextureNumber::cow_icon, { 50.0f,50.0f });
	cowIcon->SetAnchorPoint({ 0.5f, 0.5f });

	sheepIcon = Sprite::Create(TextureNumber::sheep_icon, { 175.0f,50.0f });
	sheepIcon->SetAnchorPoint({ 0.5f, 0.5f });

	pigIcon = Sprite::Create(TextureNumber::pig_icon, { 300.0f,50.0f });
	pigIcon->SetAnchorPoint({ 0.5f, 0.5f });

	cowItemIcon = Sprite::Create(TextureNumber::cow_icon, { 1139.0f,653.0f });
	cowItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	cowItemIcon->SetSize({ 96.0f, 96.0f });

	sheepItemIcon = Sprite::Create(TextureNumber::sheep_icon, { 1139.0f,653.0f });
	sheepItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	sheepItemIcon->SetSize({ 96.0f, 96.0f });

	pigItemIcon = Sprite::Create(TextureNumber::pig_icon, { 1139.0f,653.0f });
	pigItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	pigItemIcon->SetSize({ 96.0f, 96.0f });

	score_gtxt_1 = Sprite::Create(TextureNumber::game_gtxt_1, { 30.0f,91.0f });
	score_gtxt_1->SetColor({ 1.0f, 0.6f, 0.1f, 1.0f });

	bonus_gtxt_2 = Sprite::Create(TextureNumber::game_gtxt_2, { 30.0f,641.0f });
	bonus_gtxt_2->SetColor({ 1.0f, 0.6f, 0.1f, 1.0f });

	scoreBase = Sprite::Create(TextureNumber::score_base, { 5.0f,5.0f });

	itemBase = Sprite::Create(TextureNumber::item_base, { 1275.0f,715.0f });
	itemBase->SetAnchorPoint({ 1, 1 });

	esaIcon = Sprite::Create(TextureNumber::esa_icon, { 1051.0f,653.0f });
	esaIcon->SetAnchorPoint({ 0.5f, 0.5f });

	tekiIcon = Sprite::Create(TextureNumber::teki_icon, { 1227.0f,653.0f });
	tekiIcon->SetAnchorPoint({ 0.5f, 0.5f });

	bonusGage = GageUI::Create({ 5.0f,715.0f }, { 384.0f, 25.0f }, { 1.0f, 0.6f, 0.1f, 1.0f });

	pauseMenu = Sprite::Create(TextureNumber::pause_menu, { 0.0f, 0.0f });
	pauseMenuS1 = Sprite::Create(TextureNumber::pause_menu_s1, { 0.0f, 0.0f });
	pauseMenuS2 = Sprite::Create(TextureNumber::pause_menu_s2, { 0.0f, 0.0f });
	pauseMenuS3 = Sprite::Create(TextureNumber::pause_menu_s3, { 0.0f, 0.0f });

	// タイマーUI
	meterTimer = MeterUI::Create({ 1275.0f,5.0f }, 360.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	pigGate = ObjObject::Create();
	pigGateModel = ObjModel::CreateFromOBJ("butagate");
	pigGate->SetModel(pigGateModel);
	pigGate->SetPosition({ -155.0f, 0.0f, 0.0f });
	pigGate->SetRotation({ 0.0f, 270.0f, 0.0f });
	pigGate->SetScale({ 3.7f, 3.7f, 3.7f });

	sheepGate = ObjObject::Create();
	sheepGateModel = ObjModel::CreateFromOBJ("hutsuzigate");
	sheepGate->SetModel(sheepGateModel);
	sheepGate->SetPosition({ 0.0f, 0.0f, -155.0f });
	sheepGate->SetRotation({ 0.0f, 180.0f, 0.0f });
	sheepGate->SetScale({ 3.7f, 3.7f, 3.7f });

	cowGate = ObjObject::Create();
	cowGateModel = ObjModel::CreateFromOBJ("ushigate");
	cowGate->SetModel(cowGateModel);
	cowGate->SetPosition({ 155.0f, 0.0f, 0.0f });
	cowGate->SetRotation({ 0.0f, 90.0f, 0.0f });
	cowGate->SetScale({ 3.7f, 3.7f, 3.7f });

	pigSign = ObjObject::Create();
	pigSignModel = ObjModel::CreateFromOBJ("butakanban");
	pigSign->SetModel(pigSignModel);
	pigSign->SetPosition({ -152.5f, 0.0f, -40.0f });
	pigSign->SetRotation({ 0.0f, 270.0f, 0.0f });

	sheepSign = ObjObject::Create();
	sheepSignModel = ObjModel::CreateFromOBJ("hitsuzikanban");
	sheepSign->SetModel(sheepSignModel);
	sheepSign->SetPosition({ 40.0f, 0.0f, -152.5f });
	sheepSign->SetRotation({ 0.0f, 180.0f, 0.0f });

	cowSign = ObjObject::Create();
	cowSignModel = ObjModel::CreateFromOBJ("ushikanban");
	cowSign->SetModel(cowSignModel);
	cowSign->SetPosition({ 152.5f, 0.0f, 40.0f });
	cowSign->SetRotation({ 0.0f, 90.0f, 0.0f });

	// プレイヤー
	player = Player::Create();

	modelEsa = ObjModel::CreateFromOBJ("esa");

	//プレイヤーFBXモデル
	modelPlayerStop = FbxLoader::GetInstance()->LoadModelFromFile("Stop");
	modelPlayerWalking = FbxLoader::GetInstance()->LoadModelFromFile("Walking");
	modelPlayerRun = FbxLoader::GetInstance()->LoadModelFromFile("Running");
	modelPlayerThrow = FbxLoader::GetInstance()->LoadModelFromFile("Throw");
	modelPlayerCall = FbxLoader::GetInstance()->LoadModelFromFile("Call");

	//プレイヤーFBXオブジェクト
	//待機
	objPlayerStop = new FbxObject3d;
	objPlayerStop->Initialize();
	objPlayerStop->SetModel(modelPlayerStop);

	objPlayerStop->SetPosition({ 0.0f, 0.0f, 0.0f });
	objPlayerStop->SetRotation({ 0.0f, 0.0f, 0.0f });
	objPlayerStop->SetScale({ 0.5f, 0.5f, 0.5f });

	//歩き
	objPlayerWalking = new FbxObject3d;
	objPlayerWalking->Initialize();
	objPlayerWalking->SetModel(modelPlayerWalking);

	objPlayerWalking->SetPosition({ 0.0f, 0.0f, 0.0f });
	objPlayerWalking->SetRotation({ 0.0f, 0.0f, 0.0f });
	objPlayerWalking->SetScale({ 0.5f, 0.5f, 0.5f });

	//走り
	objPlayerRun = new FbxObject3d;
	objPlayerRun->Initialize();
	objPlayerRun->SetModel(modelPlayerRun);

	objPlayerRun->SetPosition({ 0.0f, 0.0f, 0.0f });
	objPlayerRun->SetRotation({ 0.0f, 0.0f, 0.0f });
	objPlayerRun->SetScale({ 0.5f, 0.5f, 0.5f });

	//エサ投げ
	objPlayerThrow = new FbxObject3d;
	objPlayerThrow->Initialize();
	objPlayerThrow->SetModel(modelPlayerThrow);

	objPlayerThrow->SetPosition({ 0.0f, 0.0f, 0.0f });
	objPlayerThrow->SetRotation({ 0.0f, 0.0f, 0.0f });
	objPlayerThrow->SetScale({ 0.5f, 0.5f, 0.5f });

	//追い出し
	objPlayerCall = new FbxObject3d;
	objPlayerCall->Initialize();
	objPlayerCall->SetModel(modelPlayerCall);

	objPlayerCall->SetPosition({ 0.0f, 0.0f, 0.0f });
	objPlayerCall->SetRotation({ 0.0f, 0.0f, 0.0f });
	objPlayerCall->SetScale({ 0.5f, 0.5f, 0.5f });

	// 3D OBJ
	ground = ObjObject::Create();
	modelGround = ObjModel::CreateFromOBJ("grand");
	ground->SetModel(modelGround);

	// Silo
	siloObject = ObjObject::Create();
	siloModel = ObjModel::CreateFromOBJ("RanchSilo");
	siloObject->SetModel(siloModel);
	siloObject->SetPosition({ 60.0f, 0.0f, 225.0f });
	siloObject->SetRotation({ 0.0f, 0.0f, 0.0f });
	siloObject->SetScale({ 6.0f, 6.0f, 6.0f });

	// Tree
	treeModel = ObjModel::CreateFromOBJ("RanchTree");
	for (int i = 0; i < 20; i++)
	{
		trees[i] = ObjObject::Create();
		trees[i]->SetModel(treeModel);
		switch (i)
		{
		case 0:
			trees[i]->SetPosition({ -130.0f, 0.0f, 180.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 1:
			trees[i]->SetPosition({ -180.0f, 0.0f, 130.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 2:
			trees[i]->SetPosition({ -190.0f, 0.0f, 190.0f });
			trees[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			break;
		case 3:
			trees[i]->SetPosition({ -130.0f, 0.0f, -180.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 4:
			trees[i]->SetPosition({ -180.0f, 0.0f, -130.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 5:
			trees[i]->SetPosition({ -190.0f, 0.0f, -190.0f });
			trees[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			break;
		case 6:
			trees[i]->SetPosition({ 130.0f, 0.0f, -180.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 7:
			trees[i]->SetPosition({ 180.0f, 0.0f, -130.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 8:
			trees[i]->SetPosition({ 190.0f, 0.0f, -190.0f });
			trees[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			break;
		case 9:
			trees[i]->SetPosition({ 130.0f, 0.0f, 180.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 10:
			trees[i]->SetPosition({ 180.0f, 0.0f, 130.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 11:
			trees[i]->SetPosition({ 190.0f, 0.0f, 190.0f });
			trees[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			break;
		case 12:
			trees[i]->SetPosition({ -100.0f, 0.0f, 200.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 13:
			trees[i]->SetPosition({ -200.0f, 0.0f, 100.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 14:
			trees[i]->SetPosition({ -100.0f, 0.0f, -200.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 15:
			trees[i]->SetPosition({ -200.0f, 0.0f, -100.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 16:
			trees[i]->SetPosition({ 100.0f, 0.0f, -200.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 17:
			trees[i]->SetPosition({ 200.0f, 0.0f, -100.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 18:
			trees[i]->SetPosition({ 100.0f, 0.0f, 200.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 19:
			trees[i]->SetPosition({ 200.0f, 0.0f, 100.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		}
		trees[i]->SetScale({ 2.5f, 2.5f, 2.5f });
	}

	// Generate coordinates and rotations
	std::pair<std::vector<XMFLOAT3>, std::vector<XMFLOAT3>> data = GenerateCoordinatesAndRotations();

	// Extract the vectors from the pair
	std::vector<XMFLOAT3>& coordinates = data.first;
	std::vector<XMFLOAT3>& rotations = data.second;

	// Assume object is some array or vector of pointers to your objects
	for (size_t i = 0; i < coordinates.size(); ++i) {
		randomTrees[i] = ObjObject::Create();
		randomTrees[i]->SetModel(treeModel);
		randomTrees[i]->SetPosition(coordinates[i]);

		float radiansToDegrees = 180.0f / 3.14159265358979323846f;
		XMFLOAT3 convertedRotation = { 0.0f, rotations[i].y * radiansToDegrees, 0.0f };

		randomTrees[i]->SetRotation(convertedRotation);
		randomTrees[i]->SetScale({ 2.5f, 2.5f, 2.5f });
	}

	// Spawn barn
	barn = ObjObject::Create();
	modelBarn = ObjModel::CreateFromOBJ("RanchHut");
	barn->SetModel(modelBarn);
	barn->SetPosition({ 0.0f, 0.0f, 255.0f });
	barn->SetRotation({ 0.0f, 0.0f, 0.0f });
	barn->SetScale({ 6.0f, 6.0f, 6.0f });

	// Fences
	FenceCreation();

	skydome = ObjObject::Create();
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	skydome->SetModel(modelSkydome);
	skydome->SetScale({ 8.0f, 8.0f, 8.0f });

	ground->SetPosition({ 0.0f, -0.5f, 0.0f });
	ground->SetRotation({ 0.0f, 0.0f, 0.0f });
	ground->SetScale({ 80.0f, 1.0f, 80.0f });

	// プレイヤー
	player->SetPosition({ 0.0f, 0.0f, 0.0f });
	player->SetRotation({ 0.0f, 0.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	modelPig = ObjModel::CreateFromOBJ("buta");
	modelSheep = ObjModel::CreateFromOBJ("hitsuzi");
	modelHorse = ObjModel::CreateFromOBJ("ushi");

	for (int i = 0; i < 2; i++)
	{
		std::unique_ptr<Buta> newButa = Buta::Create(modelPig, { -130.0f + (260.0f * i), player->GetPosition().y, 130.0f }, { 1.0f, 1.0f, 1.0f }, false);
		butaList.push_back(std::move(newButa));
		std::unique_ptr<Hitsuji> newHitsuji = Hitsuji::Create(modelSheep, { -130.0f + (260.0f * i), player->GetPosition().y, 0.0f }, { 1.0f, 1.0f, 1.0f }, false);
		hitsujiList.push_back(std::move(newHitsuji));
		std::unique_ptr<Ushi> newUshi = Ushi::Create(modelHorse, { -130.0f + (260.0f * i), player->GetPosition().y, -130.0f }, { 1.0f, 1.0f, 1.0f }, false);
		ushiList.push_back(std::move(newUshi));
	}

	// パーティクル
	Particle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	CallPart = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/water.png");
	FoodPart = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect7.png");

	//パーティクル発生用座標初期化
	PigGate_Left = { pigGate->GetPosition().x, pigGate->GetPosition().y + 40, pigGate->GetPosition().z - 35 };
	PigGate_Right = { pigGate->GetPosition().x, pigGate->GetPosition().y + 40, pigGate->GetPosition().z + 35 };

	HitsujiGate_Left = { sheepGate->GetPosition().x + 35, sheepGate->GetPosition().y + 40, sheepGate->GetPosition().z };
	HitsujiGate_Right = { sheepGate->GetPosition().x - 35, sheepGate->GetPosition().y + 40, sheepGate->GetPosition().z };

	UshiGate_Left = { cowGate->GetPosition().x, cowGate->GetPosition().y + 40, cowGate->GetPosition().z + 35 };
	UshiGate_Right = { cowGate->GetPosition().x, cowGate->GetPosition().y + 40, cowGate->GetPosition().z - 35 };

	scoreManager->score = 0;
	scoreManager->goalHorse = 0;
	scoreManager->goalPigs = 0;
	scoreManager->goalSheep = 0;

	ShowCursor(false);

	sound->PlayWav("SE/Game/GameBGM.wav", 0.07f, true);
}

void GamePlay::Finalize()
{
}

void GamePlay::Update()
{

	if (pause)
	{
		if (input->TriggerKey(DIK_ESCAPE))
		{
			pause = false;
		}

		if (input->TriggerKey(DIK_S))
		{
			switch (pauseSelection)
			{
			case 0:
				sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
				pauseSelection = 1;
				break;
			case 1:
				sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
				pauseSelection = 2;
				break;
			case 2:
				break;
			default:
				break;
			}
		}
		else if (input->TriggerKey(DIK_W))
		{
			switch (pauseSelection)
			{
			case 0:
				break;
			case 1:
				sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
				pauseSelection = 0;
				break;
			case 2:
				sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
				pauseSelection = 1;
				break;
			default:
				break;
			}
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			switch (pauseSelection)
			{
			case 0:
				sound->PlayWav("SE/Game/Decide.wav", 0.7f);
				pause = false;
				break;
			case 1:

				sound->StopWav("SE/Game/GameBGM.wav");
				//シーン切り替え
				SceneManager::GetInstance()->ChangeScene("TITLE");
				break;
			case 2:
				run = false;
				break;
			}
		}
	}
	else
	{
		if (input->TriggerKey(DIK_ESCAPE))
		{
			sound->PlayWav("SE/Game/Menu.wav", 0.7f);
			pause = true;
		}

		// Get mouse input
		Input::MouseMove mouseMove = input->GetMouseMove();

		//RECT構造体へのポインタ
		RECT rect;

		//ウィンドウの外側のサイズを取得
		GetClientRect(hwnd, &rect);

		POINT ul;
		ul.x = rect.left;
		ul.y = rect.top;

		POINT lr;
		lr.x = rect.right;
		lr.y = rect.bottom;

		MapWindowPoints(hwnd, nullptr, &ul, 1);
		MapWindowPoints(hwnd, nullptr, &lr, 1);

		rect.left = ul.x;
		rect.top = ul.y;
		rect.right = lr.x;
		rect.bottom = lr.y;

		// This locks the cursor to not leave the game screen, with the side effect of locking it to that size so long
		// as the game is running.
		ClipCursor(&rect);
	//追い出しパーティクルの座標更新
	CallPartPos = { player->GetPosition().x, player->GetPosition().y + 10 , player->GetPosition().z };


		//移動時は走りモーションにする
		if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D))
		{
			PlayerState = 2;
		}

	else
	{
		PlayerState = 0;
	}


		if (pigRespawn >= pigRespawnMax && pigNumber < pigNumberMax)
		{
			std::unique_ptr<Buta> newButa = Buta::Create(modelPig, { 0.0f, player->GetPosition().y - 0.5f, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
			butaList.push_back(std::move(newButa));

			pigRespawn = 0.0f;
			pigNumber++;
		}
		else
		{
			pigRespawn += 1.0f;
		}

		if (sheepRespawn >= sheepRespawnMax && sheepNumber < sheepNumberMax)
		{
			std::unique_ptr<Hitsuji> newHitsuji = Hitsuji::Create(modelSheep, { 0.0f, player->GetPosition().y - 0.5f, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
			hitsujiList.push_back(std::move(newHitsuji));

			sheepRespawn = 0.0f;
			sheepNumber++;
		}
		else
		{
			sheepRespawn += 1.0f;
		}

		if (horseRespawn >= horseRespawnMax && horseNumber < horseNumberMax)
		{
			std::unique_ptr<Ushi> newHorse = Ushi::Create(modelHorse, { 0.0f, player->GetPosition().y - 0.5f, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
			ushiList.push_back(std::move(newHorse));

			horseRespawn = 0.0f;
			horseNumber++;
		}
		else
		{
			horseRespawn += 1.0f;
		}

		if (mouseMove.lZ != 0)
		{
			switch (animalSelection)
			{
			case BUTA:
				if (mouseMove.lZ < 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = HITSUJI;
				}
				else if (mouseMove.lZ > 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = USHI;
				}
				break;
			case HITSUJI:
				if (mouseMove.lZ < 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = USHI;
				}
				else if (mouseMove.lZ > 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = BUTA;
				}
				break;
			case USHI:
				if (mouseMove.lZ < 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = BUTA;
				}
				else if (mouseMove.lZ > 0)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					animalSelection = HITSUJI;
				}
				break;
			default:
				break;
			}
		}

		if (input->TriggerMouseLeft() || input->TriggerMouseRight())
		{
			if (input->TriggerMouseLeft() == true)
			{
				if (AnimationFlag_T == false)
				{
					sound->PlayWav("SE/Game/Throw.wav", 0.07f);
				}
				
				AnimationFlag_T = true;
			}

			if (input->TriggerMouseRight() == true)
			{
				if (AnimationFlag_C == false)
				{
					sound->PlayWav("SE/Game/Call.wav", 0.07f);
				}
				AnimationFlag_C = true;
			}
			
			float yawInRadians = XMConvertToRadians(player->GetRotation().y);
			float x = player->GetPosition().x + sin(yawInRadians) * 8.0f;
			float z = player->GetPosition().z + cos(yawInRadians) * 8.0f;

			if (input->TriggerMouseLeft())
			{
				switch (animalSelection)
				{
				case BUTA:
				{
					if (!butaEsa)
					{
						std::unique_ptr<ButaEsa> newButaEsa = ButaEsa::Create(
							modelEsa,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						butaEsaList.push_back(std::move(newButaEsa));

						butaEsa = true;
					}
					break;
				}
				case HITSUJI:
				{
					if (!hitsujiEsa)
					{
						std::unique_ptr<HitsujiEsa> newHitsujiEsa = HitsujiEsa::Create(
							modelEsa,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						hitsujiEsaList.push_back(std::move(newHitsujiEsa));

						hitsujiEsa = true;
					}

					break;
				}
				case USHI:
				{
					if (!ushiEsa)
					{
						std::unique_ptr<UshiEsa> newUshiEsa = UshiEsa::Create(
							modelEsa,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						ushiEsaList.push_back(std::move(newUshiEsa));

						ushiEsa = true;
					}
					break;
				}
				default:
					break;
				}
			}

			if (input->TriggerMouseRight())
			{
				switch (animalSelection)
				{
				case BUTA:
				{
					if (!butaTeki)
					{
						std::unique_ptr<ButaTeki> newButaTeki = ButaTeki::Create(
							modelBullet,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						butaTekiList.push_back(std::move(newButaTeki));

						butaTeki = true;
					}
					break;
				}
				case HITSUJI:
				{
					if (!hitsujiTeki)
					{
						std::unique_ptr<HitsujiTeki> newHitsujiTeki = HitsujiTeki::Create(
							modelBullet,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						hitsujiTekiList.push_back(std::move(newHitsujiTeki));

						hitsujiTeki = true;
					}
					break;
				}
				case USHI:
				{
					if (!ushiTeki)
					{
						std::unique_ptr<UshiTeki> newUshiTeki = UshiTeki::Create(
							modelBullet,
							{ x, 0.0f, z },
							{ 0.5f, 0.5f, 0.5f }
						);

						ushiTekiList.push_back(std::move(newUshiTeki));

						ushiTeki = true;
					}
					break;
				}
				default:
					break;
				}
			}
		}

		//アニメーションフラグがtrueならタイマーを加算
		//エサ投げ
		if (AnimationFlag_T == true)
		{
			PlayerState = 3;
			AnimationTimer_T++;
		}

		if (AnimationTimer_T >= ThrowFrame)
		{
			AnimationFlag_T = false;
			AnimationTimer_T = 0;
			PlayerState = 0;
		}

		//追い出し
		if (AnimationFlag_C == true)
		{
			PlayerState = 4;
			AnimationTimer_C++;
		}

		if (AnimationTimer_C >= CallFrame)
		{
			AnimationFlag_C = false;
			AnimationTimer_C = 0;
			PlayerState = 0;
		}

		if (timer <= 0.0f)
		{
			timer = 0.0f;
			sound->StopWav("SE/Game/GameBGM.wav");
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("RESULT");
		}

		for (std::unique_ptr<Buta>& buta : butaList)
		{
			if (!buta->goalFlag)
			{
				if (!buta->initial)
				{
					XMFLOAT3 butaPosition = buta->GetPosition(); // Get the position of the current Tori

					XMFLOAT3 closestTekiPosition = { 2500.0f, 2500.0f, 2500.0f };
					float closestTekiDistance = 1e30;

					// Find the closest Teki
					if (!butaTekiList.empty())
					{
						for (std::unique_ptr<ButaTeki>& butaTeki : butaTekiList) {
							float distance = buta->SquaredDistance(butaPosition, butaTeki->GetPosition());
							if (distance < closestTekiDistance) {
								closestTekiDistance = distance;
								closestTekiPosition = butaTeki->GetPosition();
							}
						}
					}

					XMFLOAT3 closestEsaPosition = { 2500, 2500, 2500 };
					float closestEsaDistance = 1e30;

					// Find the closest Esa
					if (!butaEsaList.empty())
					{
						for (std::unique_ptr<ButaEsa>& butaEsa : butaEsaList) {
							float distance = buta->SquaredDistance(butaPosition, butaEsa->GetPosition());
							if (distance < closestEsaDistance) {
								closestEsaDistance = distance;
								closestEsaPosition = butaEsa->GetPosition();
							}
						}
					}

					// Now closestTekiPosition and closestEsaPosition hold the positions of the closest Teki and Esa respectively
					buta->UpdateEntitiesInRange(closestTekiPosition, closestEsaPosition, player->GetPosition());
				}
			}
			else
			{
				if (!buta->goalSet)
				{
					if (scoreManager->goalPigs < goalPigsMax)
					{
						buta->goalNumber = scoreManager->goalPigs;
						scoreManager->goalPigs++;
						buta->goalSet = true;
					}
					else
					{
						buta->goalSet = true;
						scoreManager->goalPigs++;
						buta->deathFlag = true;
					}

					/*if (!pigBonusTime)
					{
						pigBonusTime = true;
						pigBonusTimeRemaining = pigBonusTimeMax;
						score += 10.0f;
					}
					else if (pigBonusTime)
					{
						pigBonusTimeEntries += 1.0f;
						score += ((10.0f * pigBonusTimeEntries) + 10.0f);
					}*/

					if (!bonusTime)
					{
						bonusTime = true;
						bonusTimeRemaining = bonusTimeMax;
						scoreManager->score += 10.0f;
					}
					else if (bonusTime)
					{
						bonusTimeEntries += 1.0f;
						scoreManager->score += ((10.0f * bonusTimeEntries) + 10.0f);
					}

					frameTimer += 60.0f;
					pigNumber--;
				}
			}
		}

		for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
		{
			if (!hitsuji->goalFlag)
			{
				if (!hitsuji->initial)
				{
					XMFLOAT3 hitsujiPosition = hitsuji->GetPosition(); // Get the position of the current Tori

					XMFLOAT3 closestTekiPosition = { 2500.0f, 2500.0f, 2500.0f };
					float closestTekiDistance = 1e30;

					// Find the closest Teki
					if (!hitsujiTekiList.empty())
					{
						for (std::unique_ptr<HitsujiTeki>& hitsujiTeki : hitsujiTekiList) {
							float distance = hitsuji->SquaredDistance(hitsujiPosition, hitsujiTeki->GetPosition());
							if (distance < closestTekiDistance) {
								closestTekiDistance = distance;
								closestTekiPosition = hitsujiTeki->GetPosition();
							}
						}
					}

					XMFLOAT3 closestEsaPosition = { 2500, 2500, 2500 };
					float closestEsaDistance = 1e30;

					// Find the closest Esa
					if (!hitsujiEsaList.empty())
					{
						for (std::unique_ptr<HitsujiEsa>& hitsujiEsa : hitsujiEsaList) {
							float distance = hitsuji->SquaredDistance(hitsujiPosition, hitsujiEsa->GetPosition());
							if (distance < closestEsaDistance) {
								closestEsaDistance = distance;
								closestEsaPosition = hitsujiEsa->GetPosition();
							}
						}
					}

					// Now closestTekiPosition and closestEsaPosition hold the positions of the closest Teki and Esa respectively
					hitsuji->UpdateEntitiesInRange(closestTekiPosition, closestEsaPosition, player->GetPosition());
				}
			}
			else
			{
				if (!hitsuji->goalSet)
				{
					if (scoreManager->goalSheep < goalSheepMax)
					{
						hitsuji->goalNumber = scoreManager->goalSheep;
						scoreManager->goalSheep++;
						hitsuji->goalSet = true;
					}
					else
					{
						hitsuji->goalSet = true;
						scoreManager->goalSheep++;
						hitsuji->deathFlag = true;
					}

					/*if (!sheepBonusTime)
					{
						sheepBonusTime = true;
						sheepBonusTimeRemaining = sheepBonusTimeMax;
						score += 10.0f;
					}
					else if (sheepBonusTime)
					{
						sheepBonusTimeEntries += 1.0f;
						score += ((10.0f * sheepBonusTimeEntries) + 10.0f);
					}*/

					if (!bonusTime)
					{
						bonusTime = true;
						bonusTimeRemaining = bonusTimeMax;
						scoreManager->score += 10.0f;
					}
					else if (bonusTime)
					{
						bonusTimeEntries += 1.0f;
						scoreManager->score += ((10.0f * bonusTimeEntries) + 10.0f);
					}

					frameTimer += 60.0f;
					sheepNumber--;
				}
			}
		}

		for (std::unique_ptr<Ushi>& ushi : ushiList)
		{
			if (!ushi->goalFlag)
			{
				if (!ushi->initial)
				{
					XMFLOAT3 ushiPosition = ushi->GetPosition(); // Get the position of the current Tori

					XMFLOAT3 closestTekiPosition = { 2500.0f, 2500.0f, 2500.0f };
					float closestTekiDistance = 1e30;

					// Find the closest Teki
					if (!ushiTekiList.empty())
					{
						for (std::unique_ptr<UshiTeki>& ushiTeki : ushiTekiList) {
							float distance = ushi->SquaredDistance(ushiPosition, ushiTeki->GetPosition());
							if (distance < closestTekiDistance) {
								closestTekiDistance = distance;
								closestTekiPosition = ushiTeki->GetPosition();
							}
						}
					}

					XMFLOAT3 closestEsaPosition = { 2500, 2500, 2500 };
					float closestEsaDistance = 1e30;

					// Find the closest Esa
					if (!ushiEsaList.empty())
					{
						for (std::unique_ptr<UshiEsa>& ushiEsa : ushiEsaList) {
							float distance = ushi->SquaredDistance(ushiPosition, ushiEsa->GetPosition());
							if (distance < closestEsaDistance) {
								closestEsaDistance = distance;
								closestEsaPosition = ushiEsa->GetPosition();
							}
						}
					}

					// Now closestTekiPosition and closestEsaPosition hold the positions of the closest Teki and Esa respectively
					ushi->UpdateEntitiesInRange(closestTekiPosition, closestEsaPosition, player->GetPosition());
				}
			}
			else
			{
				if (!ushi->goalSet)
				{
					if (scoreManager->goalHorse < goalHorseMax)
					{
						ushi->goalNumber = scoreManager->goalHorse;
						scoreManager->goalHorse++;
						ushi->goalSet = true;
					}
					else
					{
						ushi->goalSet = true;
						scoreManager->goalHorse++;
						ushi->deathFlag = true;
					}

					/*if (!cowBonusTime)
					{
						cowBonusTime = true;
						cowBonusTimeRemaining = cowBonusTimeMax;
						score += 10.0f;
					}
					else if (cowBonusTime)
					{
						cowBonusTimeEntries += 1.0f;
						score += ((10.0f * cowBonusTimeEntries) + 10.0f);
					}*/

					if (!bonusTime)
					{
						bonusTime = true;
						bonusTimeRemaining = bonusTimeMax;
						scoreManager->score += 10.0f;
					}
					else if (bonusTime)
					{
						bonusTimeEntries += 1.0f;
						scoreManager->score += ((10.0f * bonusTimeEntries) + 10.0f);
					}

					frameTimer += 60.0f;
					horseNumber--;
				}
			}
		}

		if (butaEsaList.empty())
		{
			butaEsa = false;
		}
		if (butaTekiList.empty())
		{
			butaTeki = false;
		}
		if (hitsujiEsaList.empty())
		{
			hitsujiEsa = false;
		}
		if (hitsujiTekiList.empty())
		{
			hitsujiTeki = false;
		}
		if (ushiEsaList.empty())
		{
			ushiEsa = false;
		}
		if (ushiTekiList.empty())
		{
			ushiTeki = false;
		}
	if (input->GetInstance()->TriggerKey(DIK_L))
	{
		CallPart->ExpelParticle(3, CallPartPos, CallPartPos,
			3.0f, 15.0f, 3.0f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	}

	camera->SetTarget({player->GetPosition().x, player->GetPosition().y + 0.1f, player->GetPosition().z});
	ground->Update();
	skydome->SetPosition(player->GetPosition());
	skydome->Update();
	camera->Update();
	camera->SetEye({ camera->GetEye().x, camera->GetEye().y + 10.0f, camera->GetEye().z });
	player->Update();

	// Spawn Barn
	barn->Update();
	siloObject->Update();

	for (int i = 0; i < 20; i++)
	{
		trees[i]->Update();
	}

	for (int i = 0; i < 80; i++)
	{
		randomTrees[i]->Update();
	}

	// Fences
	UpdateFences();

	pigSign->Update();
	sheepSign->Update();
	cowSign->Update();

	pigGate->Update();
	sheepGate->Update();
	cowGate->Update();
	Particle->Update();
	CallPart->Update();
	FoodPart->Update();

	for (std::unique_ptr<Buta>& buta : butaList)
	{
		PigPartPos = { buta->GetPosition().x, buta->GetPosition().y + 5, buta->GetPosition().z };
		PigPartPos_F = { buta->GetPosition().x, buta->GetPosition().y + 5, buta->GetPosition().z };

		if (buta->GetgoalFlag() == true && buta->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, PigGate_Left, PigGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, PigGate_Right, PigGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
		}

		if (buta->GetRunFlag() == true)
		{
			if (buta->GetRunTimer() == 10 || buta->GetRunTimer() == 30 || buta->GetRunTimer() == 50 || buta->GetRunTimer() == 70)
			{
				CallPart->ExpelParticle(3, PigPartPos, PigPartPos,
					3.0f, 15.0f, 3.0f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
			}
		}

		if (buta->GetFoodFlag() == true)
		{
			if (buta->GetFoodTimer() == 10 || buta->GetFoodTimer() == 30 || buta->GetFoodTimer() == 50 || buta->GetFoodTimer() == 70)
			{
				FoodPart->ExpelParticle(3, PigPartPos_F, PigPartPos_F,
					3.0f, 15.0f, 3.0f, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
			}
		}

		if (buta->GetgoalFlag() == true)
		{
			if (buta->GetGoalSEFlag() == false)
			{
				sound->PlayWav("SE/Game/Goal.wav", 0.07f);
				buta->SetGoalSEFlag(true);
			}
		}

		buta->Update();
	}

	for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
	{
		SheepPartPos = { hitsuji->GetPosition().x, hitsuji->GetPosition().y + 5, hitsuji->GetPosition().z };
		SheepPartPos_F = { hitsuji->GetPosition().x,  hitsuji->GetPosition().y + 5,  hitsuji->GetPosition().z };

		if (hitsuji->GetgoalFlag() == true && hitsuji->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, HitsujiGate_Left, HitsujiGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, HitsujiGate_Right, HitsujiGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
		}

		if (hitsuji->GetRunFlag() == true)
		{
			if (hitsuji->GetRunTimer() == 10 || hitsuji->GetRunTimer() == 30 || hitsuji->GetRunTimer() == 50 || hitsuji->GetRunTimer() == 70)
			{
				CallPart->ExpelParticle(3, SheepPartPos, SheepPartPos,
					3.0f, 15.0f, 3.0f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
			}
		}

		if (hitsuji->GetFoodFlag() == true)
		{
			if (hitsuji->GetFoodTimer() == 10 || hitsuji->GetFoodTimer() == 30 || hitsuji->GetFoodTimer() == 50 || hitsuji->GetFoodTimer() == 70)
			{
				FoodPart->ExpelParticle(3, SheepPartPos_F, SheepPartPos_F,
					3.0f, 15.0f, 3.0f, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
			}
		}

		if (hitsuji->GetgoalFlag() == true)
		{
			if (hitsuji->GetGoalSEFlag() == false)
			{
				sound->PlayWav("SE/Game/Goal.wav", 0.07f);
				hitsuji->SetGoalSEFlag(true);
			}
		}

		hitsuji->Update();
	}

	for (std::unique_ptr<Ushi>& ushi : ushiList)
	{
		CowPartPos = { ushi->GetPosition().x, ushi->GetPosition().y + 5, ushi->GetPosition().z };
		CowPartPos_F = { ushi->GetPosition().x,  ushi->GetPosition().y + 5, ushi->GetPosition().z };

		if (ushi->GetgoalFlag() == true && ushi->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, UshiGate_Left, UshiGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, UshiGate_Right, UshiGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
		}

		if (ushi->GetRunFlag() == true)
		{
			if (ushi->GetRunTimer() == 10 || ushi->GetRunTimer() == 30 || ushi->GetRunTimer() == 50 || ushi->GetRunTimer() == 70)
			{
				CallPart->ExpelParticle(3, CowPartPos, CowPartPos,
					3.0f, 15.0f, 3.0f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
			}
		}

		if (ushi->GetFoodFlag() == true)
		{
			if (ushi->GetFoodTimer() == 10 || ushi->GetFoodTimer() == 30 || ushi->GetFoodTimer() == 50 || ushi->GetFoodTimer() == 70)
			{
				FoodPart->ExpelParticle(3, CowPartPos_F, CowPartPos_F,
					3.0f, 15.0f, 3.0f, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });
			}
		}

		if (ushi->GetgoalFlag() == true)
		{
			if (ushi->GetGoalSEFlag() == false)
			{
				sound->PlayWav("SE/Game/Goal.wav", 0.07f);
				ushi->SetGoalSEFlag(true);
			}
		}
		ushi->Update();
	}

		for (std::unique_ptr<Esa>& esa : esaList)
		{
			esa->Update();
		}

		for (std::unique_ptr<Teki>& teki : tekiList)
		{
			teki->Update();
		}

		for (std::unique_ptr<ButaEsa>& butaEsa : butaEsaList)
		{
			butaEsa->Update();
		}

		for (std::unique_ptr<ButaTeki>& butaTeki : butaTekiList)
		{
			butaTeki->Update();
		}

		for (std::unique_ptr<HitsujiEsa>& hitsujiEsa : hitsujiEsaList)
		{
			hitsujiEsa->Update();
		}

		for (std::unique_ptr<HitsujiTeki>& hitsujiTeki : hitsujiTekiList)
		{
			hitsujiTeki->Update();
		}

		for (std::unique_ptr<UshiEsa>& ushiEsa : ushiEsaList)
		{
			ushiEsa->Update();
		}

		for (std::unique_ptr<UshiTeki>& ushiTeki : ushiTekiList)
		{
			ushiTeki->Update();
		}

		esaList.remove_if([](std::unique_ptr<Esa>& esa)
			{
				return esa->GetDeathFlag();
			}
		);

		tekiList.remove_if([](std::unique_ptr<Teki>& teki)
			{
				return teki->GetDeathFlag();
			}
		);

		butaEsaList.remove_if([](std::unique_ptr<ButaEsa>& butaEsa)
			{
				return butaEsa->GetDeathFlag();
			}
		);

		butaTekiList.remove_if([](std::unique_ptr<ButaTeki>& butaTeki)
			{
				return butaTeki->GetDeathFlag();
			}
		);

		hitsujiEsaList.remove_if([](std::unique_ptr<HitsujiEsa>& hitsujiEsa)
			{
				return hitsujiEsa->GetDeathFlag();
			}
		);

		hitsujiTekiList.remove_if([](std::unique_ptr<HitsujiTeki>& hitsujiTeki)
			{
				return hitsujiTeki->GetDeathFlag();
			}
		);

		ushiEsaList.remove_if([](std::unique_ptr<UshiEsa>& ushiEsa)
			{
				return ushiEsa->GetDeathFlag();
			}
		);

		ushiTekiList.remove_if([](std::unique_ptr<UshiTeki>& ushiTeki)
			{
				return ushiTeki->GetDeathFlag();
			}
		);

		//FBX各種の座標、ローテート更新
		//待機

		if (PlayerState == 0)
		{
			objPlayerStop->SetPosition({ player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z });
			objPlayerStop->SetRotation(player->GetRotation());
			objPlayerStop->Update();
		}

		if (PlayerState == 1)
		{
			//歩き
			objPlayerWalking->SetPosition({ player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z });
			objPlayerWalking->SetRotation(player->GetRotation());
			objPlayerWalking->Update();
		}

		if (PlayerState == 2)
		{
			//走り
			objPlayerRun->SetPosition({ player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z });
			objPlayerRun->SetRotation(player->GetRotation());
			objPlayerRun->Update();

		}

		if (PlayerState == 3)
		{
			//エサ投げ
			objPlayerThrow->SetPosition({ player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z });
			objPlayerThrow->SetRotation(player->GetRotation());
			objPlayerThrow->Update();
		}

		if (PlayerState == 4)
		{
			//
			objPlayerCall->SetPosition({ player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z });
			objPlayerCall->SetRotation(player->GetRotation());
			objPlayerCall->Update();
		}

		frameTimer -= 1.0f;
		timer = (frameTimer / 60.0f);

		meterTimer->Update(timer, timerMax, { 1240, 45 });

	bonusGage->Update(bonusTimeRemaining, bonusTimeMax, { 5.0f,715.0f }, { 0.8f, 0.6f, 0.1f, 1.0f }, { 0.8f, 0.6f, 0.1f, 1.0f });

	butaList.remove_if([](std::unique_ptr<Buta>& buta)
		{
			return buta->GetDeathFlag();
		}
	);

	hitsujiList.remove_if([](std::unique_ptr<Hitsuji>& hitsuji)
		{
			return hitsuji->GetDeathFlag();
		}
	);

	ushiList.remove_if([](std::unique_ptr<Ushi>& ushi)
		{
			return ushi->GetDeathFlag();
		}
	);

	/*if (pigBonusTimeRemaining <= 0.0f)
	{
		pigBonusTime = false;
		pigBonusTimeRemaining = 0.0f;
	}
	else
	{
		pigBonusTimeRemaining -= 1.0f;
	}

		if (sheepBonusTimeRemaining <= 0.0f)
		{
			sheepBonusTime = false;
			sheepBonusTimeRemaining = 0.0f;
		}
		else
		{
			sheepBonusTimeRemaining -= 1.0f;
		}

		if (cowBonusTimeRemaining <= 0.0f)
		{
			cowBonusTime = false;
			cowBonusTimeRemaining = 0.0f;
		}
		else
		{
			cowBonusTimeRemaining -= 1.0f;
		}*/

		if (bonusTimeRemaining <= 0.0f)
		{
			bonusTime = false;
			bonusTimeRemaining = 0.0f;
			bonusTimeEntries = 0.0f;
		}
		else
		{
			bonusTimeRemaining -= 1.0f;
		}

		//Debug Start
		//char msgbuf[256];
		//char msgbuf2[256];
		//char msgbuf3[256];
		//sprintf_s(msgbuf, 256, "Bonus Time: %f\n", bonusTimeRemaining);
		//sprintf_s(msgbuf2, 256, "SheepBT: %f\n", sheepBonusTimeRemaining);
		//sprintf_s(msgbuf3, 256, "CowBT: %f\n", cowBonusTimeRemaining);
		//OutputDebugStringA(msgbuf);
		//OutputDebugStringA(msgbuf2);
		//OutputDebugStringA(msgbuf3);
		//Debug End

		//DrawDebugText();
	}

	// スコアの描画
	std::ostringstream Score;
	Score << std::fixed << std::setprecision(0) << scoreManager->score;
	scoreText.Print(Score.str(), { 155, 117 }, { 1.0f, 0.6f, 0.1f, 1.0f }, 1.0f);

	std::ostringstream CowGoalCount;
	CowGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalHorse;
	scoreText.Print(CowGoalCount.str(), { 115.0f,68.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 1.0f);

	std::ostringstream SheepGoalCount;
	SheepGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalSheep;
	scoreText.Print(SheepGoalCount.str(), { 240.0f,68.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 1.0f);

	std::ostringstream PigGoalCount;
	PigGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalPigs;
	scoreText.Print(PigGoalCount.str(), { 365, 68 }, { 1.0f, 0.6f, 0.1f, 1.0f }, 1.0f);

	// ゲームタイマー
	std::ostringstream GameTimer;
	GameTimer << std::fixed << std::setprecision(0) << timer;
	scoreText.Print(GameTimer.str(), { 1119.0f, 63.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 1.0f);
}

void GamePlay::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	gameBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	ground->Draw();
	skydome->Draw();
	//player->Draw();

	// Spawn Barn
	barn->Draw();
	siloObject->Draw();

	for (int i = 0; i < 20; i++)
	{
		trees[i]->Draw();
	}

	for (int i = 0; i < 80; i++)
	{
		randomTrees[i]->Draw();
	}

	// Fences
	DrawFences();

	pigGate->Draw();
	sheepGate->Draw();
	cowGate->Draw();

	pigSign->Draw();
	sheepSign->Draw();
	cowSign->Draw();

	//プレイヤーの描画
	if (PlayerState == 0)
	{
		objPlayerStop->Draw(cmdList);
	}

	if (PlayerState == 1)
	{
		objPlayerWalking->Draw(cmdList);
	}

	if (PlayerState == 2)
	{
		objPlayerRun->Draw(cmdList);
	}

	if (PlayerState == 3)
	{
		objPlayerThrow->Draw(cmdList);
	}

	if (PlayerState == 4)
	{
		objPlayerCall->Draw(cmdList);
	}

	for (std::unique_ptr<Buta>& buta : butaList)
	{
		buta->Draw();
	}

	for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
	{
		hitsuji->Draw();
	}

	for (std::unique_ptr<Ushi>& ushi : ushiList)
	{
		ushi->Draw();
	}

	for (std::unique_ptr<Esa>& esa : esaList)
	{
		esa->Draw();
	}

	for (std::unique_ptr<Teki>& teki : tekiList)
	{
		teki->Draw();
	}

	for (std::unique_ptr<ButaEsa>& butaEsa : butaEsaList)
	{
		butaEsa->Draw();
	}

	/*for (std::unique_ptr<ButaTeki>& butaTeki : butaTekiList)
	{
		butaTeki->Draw();
	}*/

	for (std::unique_ptr<HitsujiEsa>& hitsujiEsa : hitsujiEsaList)
	{
		hitsujiEsa->Draw();
	}

	/*for (std::unique_ptr<HitsujiTeki>& hitsujiTeki : hitsujiTekiList)
	{
		hitsujiTeki->Draw();
	}*/

	for (std::unique_ptr<UshiEsa>& ushiEsa : ushiEsaList)
	{
		ushiEsa->Draw();
	}

	/*for (std::unique_ptr<UshiTeki>& ushiTeki : ushiTekiList)
	{
		ushiTeki->Draw();
	}*/

	// パーティクルの描画
	Particle->Draw(cmdList);
	CallPart->Draw(cmdList);
	FoodPart->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画
	scoreBase->Draw();
	itemBase->Draw();
	bonusGage->Draw();

	cowIcon->Draw();
	sheepIcon->Draw();
	pigIcon->Draw();

	if (animalSelection == USHI)
	{
		cowItemIcon->Draw();
	}
	else if (animalSelection == HITSUJI)
	{
		sheepItemIcon->Draw();
	}
	else if (animalSelection == BUTA)
	{
		pigItemIcon->Draw();
	}

	esaIcon->Draw();
	tekiIcon->Draw();

	meterTimer->Draw();

	score_gtxt_1->Draw();
	bonus_gtxt_2->Draw();

	player->DebugTextDraw();

	debugText.DrawAll(cmdList);
	scoreText.DrawAll(cmdList);

	if (pause)
	{
		pauseMenu->Draw();

		switch (pauseSelection)
		{
		case 0:
			pauseMenuS1->Draw();
			break;
		case 1:
			pauseMenuS2->Draw();
			break;
		case 2:
			pauseMenuS3->Draw();
			break;
		default:
			break;
		}
	}

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

// マウスの処理
void GamePlay::GetMouse()
{
	//マウスの(スクリーン)座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInst()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入
	ReticlePos.x = (float)mousePosition.x;
	ReticlePos.y = (float)mousePosition.y;
}

// デバックテキスト
void GamePlay::DrawDebugText()
{
	//アニメーションタイマー
	std::ostringstream AnimeTimer;
	AnimeTimer << "AnimationTimer("
		<< std::fixed << std::setprecision(5)
		<< AnimationTimer_T << ")";
	debugText.Print(AnimeTimer.str(), 0, 500, 1.0f);

	// 牛のゴール
	std::ostringstream UshiGoalCount;
	UshiGoalCount << "UshiGoalCount("
		<< std::fixed << std::setprecision(5)
		<< scoreManager->goalHorse << ")";
	debugText.Print(UshiGoalCount.str(), 0, 540, 1.0f);

	// 羊のゴール
	std::ostringstream HitsuziGoalCount;
	HitsuziGoalCount << "HitsuziGoalCount("
		<< std::fixed << std::setprecision(5)
		<< scoreManager->goalSheep << ")";
	debugText.Print(HitsuziGoalCount.str(), 200, 540, 1.0f);

	// 豚のゴール
	std::ostringstream ButaGoalCount;
	ButaGoalCount << "ButaGoalCount("
		<< std::fixed << std::setprecision(5)
		<< scoreManager->goalPigs << ")";
	debugText.Print(ButaGoalCount.str(), 400, 540, 1.0f);

	// スコア
	std::ostringstream GameScore;
	GameScore << "GameScore("
		<< std::fixed << std::setprecision(5)
		<< scoreManager->score << ")";
	debugText.Print(GameScore.str(), 0, 560, 1.0f);
}

void GamePlay::CameraInitialization()
{
	
}

void GamePlay::SoundInitialization()
{
	
}

void GamePlay::TextureInitialization()
{
	
}

void GamePlay::SpriteInitialization()
{
	
}

void GamePlay::DeviceAndPipelineInitialization()
{
	
}

void GamePlay::GateInitialization()
{
	
}

void GamePlay::PlayerFBXInitialization()
{
	
}

void GamePlay::ObjObjectInitialization()
{
	
}

void GamePlay::FenceCreation()
{
	modelFence = ObjModel::CreateFromOBJ("fence");

	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			// West side North
			westSideNorthFence[i] = ObjObject::Create();
			westSideNorthFence[i]->SetModel(modelFence);
			westSideNorthFence[i]->SetPosition({ -155.0f, -1.0f, 145.0f - (20.0f * i) });
			westSideNorthFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			westSideNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// West side South
			westSideSouthFence[i] = ObjObject::Create();
			westSideSouthFence[i]->SetModel(modelFence);
			westSideSouthFence[i]->SetPosition({ -155.0f, -1.0f, -145.0f + (20.0f * i) });
			westSideSouthFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			westSideSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South side West
			southSideWestFence[i] = ObjObject::Create();
			southSideWestFence[i]->SetModel(modelFence);
			southSideWestFence[i]->SetPosition({ -145.0f + (20.0f * i), -1.0f, -155.0f });
			southSideWestFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			southSideWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South side East
			southSideEastFence[i] = ObjObject::Create();
			southSideEastFence[i]->SetModel(modelFence);
			southSideEastFence[i]->SetPosition({ 145.0f - (20.0f * i), -1.0f, -155.0f });
			southSideEastFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			southSideEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East side North
			eastSideNorthFence[i] = ObjObject::Create();
			eastSideNorthFence[i]->SetModel(modelFence);
			eastSideNorthFence[i]->SetPosition({ 155.0f, -1.0f, 145.0f - (20.0f * i) });
			eastSideNorthFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			eastSideNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East side South
			eastSideSouthFence[i] = ObjObject::Create();
			eastSideSouthFence[i]->SetModel(modelFence);
			eastSideSouthFence[i]->SetPosition({ 155.0f, -1.0f, -145.0f + (20.0f * i) });
			eastSideSouthFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			eastSideSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North side West
			northSideWestFence[i] = ObjObject::Create();
			northSideWestFence[i]->SetModel(modelFence);
			northSideWestFence[i]->SetPosition({ -145.0f + (20.0f * i), -1.0f, 155.0f });
			northSideWestFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			northSideWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North side East
			northSideEastFence[i] = ObjObject::Create();
			northSideEastFence[i]->SetModel(modelFence);
			northSideEastFence[i]->SetPosition({ 145.0f - (20.0f * i), -1.0f, 155.0f });
			northSideEastFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			northSideEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}

		if (i < 3)
		{
			// West goal
			// South
			westGoalSouthFence[i] = ObjObject::Create();
			westGoalSouthFence[i]->SetModel(modelFence);
			westGoalSouthFence[i]->SetPosition({ -166.0f - (20.0f * i), -1.0f, -70.0f });
			westGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			westGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North
			westGoalNorthFence[i] = ObjObject::Create();
			westGoalNorthFence[i]->SetModel(modelFence);
			westGoalNorthFence[i]->SetPosition({ -166.0f - (20.0f * i), -1.0f, 70.0f });
			westGoalNorthFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			westGoalNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South Goal
			// West
			southGoalWestFence[i] = ObjObject::Create();
			southGoalWestFence[i]->SetModel(modelFence);
			southGoalWestFence[i]->SetPosition({ -70.0f, -1.0f, -166.0f - (20.0f * i) });
			southGoalWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			southGoalWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East
			southGoalEastFence[i] = ObjObject::Create();
			southGoalEastFence[i]->SetModel(modelFence);
			southGoalEastFence[i]->SetPosition({ 70.0f, -1.0f, -166.0f - (20.0f * i) });
			southGoalEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			southGoalEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East Goal
			// South
			eastGoalSouthFence[i] = ObjObject::Create();
			eastGoalSouthFence[i]->SetModel(modelFence);
			eastGoalSouthFence[i]->SetPosition({ 166.0f + (20.0f * i), -1.0f, -70.0f });
			eastGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			eastGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North
			eastGoalNorthFence[i] = ObjObject::Create();
			eastGoalNorthFence[i]->SetModel(modelFence);
			eastGoalNorthFence[i]->SetPosition({ 166.0f + (20.0f * i), -1.0f, 70.0f });
			eastGoalNorthFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			eastGoalNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}
		if (i < 2)
		{
			// Barn
			// West
			barnWestFence[i] = ObjObject::Create();
			barnWestFence[i]->SetModel(modelFence);
			barnWestFence[i]->SetPosition({ -35.0f, -1.0f, 165.0f + (20.0f * i) });
			barnWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			barnWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East
			barnEastFence[i] = ObjObject::Create();
			barnEastFence[i]->SetModel(modelFence);
			barnEastFence[i]->SetPosition({ 35.0f, -1.0f, 165.0f + (20.0f * i) });
			barnEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			barnEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}

		// West goal West
		westGoalWestFence[i] = ObjObject::Create();
		westGoalWestFence[i]->SetModel(modelFence);
		westGoalWestFence[i]->SetPosition({ -216.0f, -1.0f, -60.0f + (20.0f * i) });
		westGoalWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
		westGoalWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

		// South goal South
		southGoalSouthFence[i] = ObjObject::Create();
		southGoalSouthFence[i]->SetModel(modelFence);
		southGoalSouthFence[i]->SetPosition({ -60.0f + (20.0f * i), -1.0f, -216.0f });
		southGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
		southGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

		// East goal East
		eastGoalEastFence[i] = ObjObject::Create();
		eastGoalEastFence[i]->SetModel(modelFence);
		eastGoalEastFence[i]->SetPosition({ 216.0f, -1.0f, -60.0f + (20.0f * i) });
		eastGoalEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
		eastGoalEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
	}
}

void GamePlay::UpdateFences()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			westSideNorthFence[i]->Update();
			westSideSouthFence[i]->Update();
			southSideWestFence[i]->Update();
			southSideEastFence[i]->Update();
			eastSideNorthFence[i]->Update();
			eastSideSouthFence[i]->Update();
			northSideWestFence[i]->Update();
			northSideEastFence[i]->Update();
		}
		if (i < 3)
		{
			westGoalNorthFence[i]->Update();
			westGoalSouthFence[i]->Update();
			southGoalEastFence[i]->Update();
			southGoalWestFence[i]->Update();
			eastGoalNorthFence[i]->Update();
			eastGoalSouthFence[i]->Update();
		}
		if (i < 2)
		{
			barnWestFence[i]->Update();
			barnEastFence[i]->Update();
		}
		westGoalWestFence[i]->Update();
		southGoalSouthFence[i]->Update();
		eastGoalEastFence[i]->Update();
	}
}

void GamePlay::DrawFences()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			westSideNorthFence[i]->Draw();
			westSideSouthFence[i]->Draw();
			southSideWestFence[i]->Draw();
			southSideEastFence[i]->Draw();
			eastSideNorthFence[i]->Draw();
			eastSideSouthFence[i]->Draw();
			northSideEastFence[i]->Draw();
			northSideWestFence[i]->Draw();
		}
		if (i < 3)
		{
			westGoalNorthFence[i]->Draw();
			westGoalSouthFence[i]->Draw();
			southGoalWestFence[i]->Draw();
			southGoalEastFence[i]->Draw();
			eastGoalSouthFence[i]->Draw();
			eastGoalNorthFence[i]->Draw();
		}
		if (i < 2)
		{
			barnWestFence[i]->Draw();
			barnEastFence[i]->Draw();
		}
		westGoalWestFence[i]->Draw();
		southGoalSouthFence[i]->Draw();
		eastGoalEastFence[i]->Draw();
	}
}

void GamePlay::ParticleInitialization()
{
	
}

void GamePlay::InitialAnimalInitialization()
{
	
}