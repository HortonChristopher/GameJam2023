#include "GamePlay.h"
#include "FbxObject.h"

using namespace DirectX;
extern HWND hwnd;

extern bool victory;

GamePlay::GamePlay()
{
}

GamePlay::~GamePlay()
{
}

void GamePlay::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);
	Player::SetCamera(camera);
	Boss::SetCamera(camera);

	//FBXカメラセット
	FbxObject3d::SetCamera(camera);
	
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	sound->LoadWav("SE/Game/game_player_shot.wav");
	sound->LoadWav("SE/Game/game_boss_shot.wav");
	sound->LoadWav("SE/Game/game_player_damage.wav");
	sound->LoadWav("SE/Game/game_boss_damage.wav");
	sound->LoadWav("BGM/Game/game_bgm.wav");

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
	if (!Sprite::LoadTexture(TextureNumber::speed, L"Resources/Sprite/GameUI/Speed.png")) {
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

	cowIcon = Sprite::Create(TextureNumber::cow_icon, { 5.0f,5.0f });
	sheepIcon = Sprite::Create(TextureNumber::sheep_icon, { 105.0f,5.0f });
	pigIcon = Sprite::Create(TextureNumber::pig_icon, { 205.0f,5.0f });

	score_gtxt_1 = Sprite::Create(TextureNumber::game_gtxt_1, { 75.0f,60.0f });
	score_gtxt_1->SetSize({133.0f, 28.0f});

	// タイマーUI
	meterTimer = MeterUI::Create({ 1230, 10 }, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

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

	// プレイヤー
	player = Player::Create();

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
	ground->SetScale({ 40.0f, 1.0f, 40.0f });

	// プレイヤー
	player->SetPosition({ 0.0f, 0.5f, 0.0f });
	player->SetRotation({ 0.0f, 0.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	modelPig = ObjModel::CreateFromOBJ("buta");
	modelSheep = ObjModel::CreateFromOBJ("hitsuzi");
	modelHorse = ObjModel::CreateFromOBJ("ushi");

	// パーティクル
	Particle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	//パーティクル発生用座標初期化
	PigGate_Left = { pigGate->GetPosition().x, pigGate->GetPosition().y + 40, pigGate->GetPosition().z - 35 };
	PigGate_Right = { pigGate->GetPosition().x, pigGate->GetPosition().y + 40, pigGate->GetPosition().z + 35 };

	HitsujiGate_Left = { sheepGate->GetPosition().x + 35, sheepGate->GetPosition().y + 40, sheepGate->GetPosition().z};
	HitsujiGate_Right = { sheepGate->GetPosition().x - 35, sheepGate->GetPosition().y + 40, sheepGate->GetPosition().z};

	UshiGate_Left = { cowGate->GetPosition().x, cowGate->GetPosition().y + 40, cowGate->GetPosition().z + 35 };
	UshiGate_Right = { cowGate->GetPosition().x, cowGate->GetPosition().y + 40, cowGate->GetPosition().z - 35 };

	// 座標のセット
	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0, 2, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(96.0f);

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	for (int i = 0; i < 2; i++)
	{
		std::unique_ptr<Buta> newButa = Buta::Create(modelPig, { -130.0f + (260.0f * i), player->GetPosition().y, 130.0f}, {1.0f, 1.0f, 1.0f}, false);
		butaList.push_back(std::move(newButa));
		std::unique_ptr<Hitsuji> newHitsuji = Hitsuji::Create(modelSheep, { -130.0f + (260.0f * i), player->GetPosition().y, 0.0f }, { 1.0f, 1.0f, 1.0f }, false);
		hitsujiList.push_back(std::move(newHitsuji));
		std::unique_ptr<Ushi> newUshi = Ushi::Create(modelHorse, { -130.0f + (260.0f * i), player->GetPosition().y, -130.0f }, { 1.0f, 1.0f, 1.0f }, false);
		ushiList.push_back(std::move(newUshi));
	}

	ShowCursor(false);
}

void GamePlay::Finalize()
{
}

void GamePlay::Update()
{
	// Get mouse input
	Input::MouseMove mouseMove = input->GetMouseMove();

	//RECT構造体へのポインタ
	RECT rect;

	//ウィンドウの外側のサイズを取得
	GetClientRect(hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//マウスの(スクリーン)座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入
	ReticlePos.x = ((float)(mousePosition.x) / (float)width) * WinApp::window_width;
	ReticlePos.y = ((float)(mousePosition.y) / (float)height) * WinApp::window_height;

	if (ReticlePos.x <= 280.0f)
	{
		ReticlePos.x = 280.0f;
	}
	if (ReticlePos.x >= 1000.0f)
	{
		ReticlePos.x = 1000.0f;
	}

	if (ReticlePos.y <= 180.0f)
	{
		ReticlePos.y = 180.0f;
	}
	if (ReticlePos.y >= 575.0f)
	{
		ReticlePos.y = 575.0f;
	}
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
		std::unique_ptr<Buta> newButa = Buta::Create(modelPig, { 0.0f, player->GetPosition().y, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
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
		std::unique_ptr<Hitsuji> newHitsuji = Hitsuji::Create(modelSheep, { 0.0f, player->GetPosition().y, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
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
		std::unique_ptr<Ushi> newHorse = Ushi::Create(modelHorse, { 0.0f, player->GetPosition().y, 255.0f }, { 1.0f, 1.0f, 1.0f }, true);
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
		switch (itemSelection)
		{
		case BUTA_ESA:
			if (mouseMove.lZ < 0)
			{
				itemSelection = BUTA_TEKI;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = USHI_TEKI;
			}
			break;
		case BUTA_TEKI:
			if (mouseMove.lZ < 0)
			{
				itemSelection = HITSUJI_ESA;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = BUTA_ESA;
			}
			break;
		case HITSUJI_ESA:
			if (mouseMove.lZ < 0)
			{
				itemSelection = HITSUJI_TEKI;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = BUTA_TEKI;
			}
			break;
		case HITSUJI_TEKI:
			if (mouseMove.lZ < 0)
			{
				itemSelection = USHI_ESA;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = HITSUJI_ESA;
			}
			break;
		case USHI_ESA:
			if (mouseMove.lZ < 0)
			{
				itemSelection = USHI_TEKI;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = HITSUJI_TEKI;
			}
			break;
		case USHI_TEKI:
			if (mouseMove.lZ < 0)
			{
				itemSelection = BUTA_ESA;
			}
			else if (mouseMove.lZ > 0)
			{
				itemSelection = USHI_ESA;
			}
			break;
		default:
			break;
		}
	}

	if (input->TriggerMouseLeft())
	{
		AnimationFlag_T = true;
		float yawInRadians = XMConvertToRadians(player->GetRotation().y);
		float x = player->GetPosition().x + sin(yawInRadians) * 8.0f;
		float z = player->GetPosition().z + cos(yawInRadians) * 8.0f;

		switch (itemSelection)
		{
		case BUTA_ESA:
		{
			std::unique_ptr<ButaEsa> newButaEsa = ButaEsa::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			butaEsaList.push_back(std::move(newButaEsa));
			break;
		}
		case BUTA_TEKI:
		{
			std::unique_ptr<ButaTeki> newButaTeki = ButaTeki::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			butaTekiList.push_back(std::move(newButaTeki));
			break;
		}
		case HITSUJI_ESA:
		{
			std::unique_ptr<HitsujiEsa> newHitsujiEsa = HitsujiEsa::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			hitsujiEsaList.push_back(std::move(newHitsujiEsa));
			break;
		}
		case HITSUJI_TEKI:
		{
			std::unique_ptr<HitsujiTeki> newHitsujiTeki = HitsujiTeki::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			hitsujiTekiList.push_back(std::move(newHitsujiTeki));
			break;
		}
		case USHI_ESA:
		{
			std::unique_ptr<UshiEsa> newUshiEsa = UshiEsa::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			ushiEsaList.push_back(std::move(newUshiEsa));
			break;
		}
		case USHI_TEKI:
		{
			std::unique_ptr<UshiTeki> newUshiTeki = UshiTeki::Create(
				modelBullet,
				{ x, 0.5f, z },
				{ 0.5f, 0.5f, 0.5f }
			);

			ushiTekiList.push_back(std::move(newUshiTeki));
			break;
		}
		default:
			break;
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

	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
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
				if (goalPigs < goalPigsMax)
				{
					buta->goalNumber = goalPigs;
					goalPigs++;
					buta->goalSet = true;
				}
				else
				{
					buta->goalSet = true;
					buta->deathFlag = true;
				}

				score += 10.0f;
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
				if (goalSheep < goalSheepMax)
				{
					hitsuji->goalNumber = goalSheep;
					goalSheep++;
					hitsuji->goalSet = true;
				}
				else
				{
					hitsuji->goalSet = true;
					hitsuji->deathFlag = true;
				}

				score += 10.0f;
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
				if (goalHorse < goalHorseMax)
				{
					ushi->goalNumber = goalHorse;
					goalHorse++;
					ushi->goalSet = true;
				}
				else
				{
					ushi->goalSet = true;
					ushi->deathFlag = true;
				}

				score += 10.0f;
				horseNumber--;
			}
		}
	}

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

	//Particle->DefaultParticle(10, 10, player->GetPosition(), 3.0f, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, {0.2f, 0.2f, 0.2f, 1.0f});

	camera->SetTarget(player->GetPosition());
	ground->Update();
	skydome->SetPosition(player->GetPosition());
	skydome->Update();
	camera->Update();
	camera->SetEye({ camera->GetEye().x, camera->GetEye().y + 10.0f, camera->GetEye().z });
	player->Update();
	
	// Spawn Barn
	barn->Update();

	// Fences
	UpdateFences();

	pigGate->Update();
	sheepGate->Update();
	cowGate->Update();
	Particle->Update();

	for (std::unique_ptr<Buta>& buta : butaList)
	{
		if (buta->GetgoalFlag() == true && buta->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, PigGate_Left, PigGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, PigGate_Right, PigGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
		}

		buta->Update();
	}

	for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
	{
		if (hitsuji->GetgoalFlag() == true && hitsuji->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, HitsujiGate_Left, HitsujiGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, HitsujiGate_Right, HitsujiGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
		}

		hitsuji->Update();
	}

	for (std::unique_ptr<Ushi>& ushi : ushiList)
	{
		if (ushi->GetgoalFlag() == true && ushi->GetgoalTimer() <= 30)
		{
			//パーティクル発生 player->GetPosition()
			Particle->LevelUpParticle(10, UshiGate_Left, UshiGate_Left,
				5.0f, 15.0f, 8.0f, 0.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });

			Particle->LevelUpParticle(10, UshiGate_Right, UshiGate_Right,
				5.0f, 15.0f, 8.0f, 1.0f, { 0.921f, 0.039f, 0.886f, 1.0f }, { 0.200f, 0.482f, 0.176f, 1.0f });
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

	meterTimer->Update(timer, 60.0f, { 1240, 45 });

	//Debug Start
	/*char msgbuf[256];
	char msgbuf2[256];
	char msgbuf3[256];
	sprintf_s(msgbuf, 256, "FT: %f\n", frameTimer);
	sprintf_s(msgbuf2, 256, "T: %d\n", timer);
	sprintf_s(msgbuf3, 256, "S: %f\n", score);
	OutputDebugStringA(msgbuf);
	OutputDebugStringA(msgbuf2);
	OutputDebugStringA(msgbuf3)*/
	//Debug End

	//DrawDebugText();

	// スコアの描画
	std::ostringstream Score;
	Score << std::fixed << std::setprecision(0)  << score;
	scoreText.Print(Score.str(), { 250, 90 }, { 0.760f, 0.929f, 1.0f, 1.0f }, 0.8f);

	std::ostringstream CowGoalCount;
	CowGoalCount <<  std::fixed << std::setprecision(0)  << goalHorse;
	scoreText.Print(CowGoalCount.str(), { 80, 45 }, { 0.760f, 0.929f, 1.0f, 1.0f }, 0.8f);

	std::ostringstream SheepGoalCount;
	SheepGoalCount << std::fixed << std::setprecision(0) << goalSheep;
	scoreText.Print(SheepGoalCount.str(), { 180, 45 }, { 0.760f, 0.929f, 1.0f, 1.0f }, 0.8f);

	std::ostringstream PigGoalCount;
	PigGoalCount << std::fixed << std::setprecision(0) << goalPigs;
	scoreText.Print(PigGoalCount.str(), { 280, 45 }, { 0.760f, 0.929f, 1.0f, 1.0f }, 0.8f);

	// ゲームタイマー
	std::ostringstream GameTimer;
	GameTimer << std::fixed << std::setprecision(0)<< timer ;
	scoreText.Print(GameTimer.str(), { 1230, 67 }, { 0.760f, 0.929f, 1.0f, 1.0f }, 0.8f);
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

	// Fences
	DrawFences();

	pigGate->Draw();
	sheepGate->Draw();
	cowGate->Draw();

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

	for (std::unique_ptr<ButaTeki>& butaTeki : butaTekiList)
	{
		butaTeki->Draw();
	}

	for (std::unique_ptr<HitsujiEsa>& hitsujiEsa : hitsujiEsaList)
	{
		hitsujiEsa->Draw();
	}

	for (std::unique_ptr<HitsujiTeki>& hitsujiTeki : hitsujiTekiList)
	{
		hitsujiTeki->Draw();
	}

	for (std::unique_ptr<UshiEsa>& ushiEsa : ushiEsaList)
	{
		ushiEsa->Draw();
	}

	for (std::unique_ptr<UshiTeki>& ushiTeki : ushiTekiList)
	{
		ushiTeki->Draw();
	}

	// パーティクルの描画
	Particle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	
	// 前景スプライト描画
	cowIcon->Draw();
	sheepIcon->Draw();
	pigIcon->Draw();

	meterTimer->Draw();

	score_gtxt_1->Draw();

	player->DebugTextDraw();

	debugText.DrawAll(cmdList);
	scoreText.DrawAll(cmdList);

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
		<< goalHorse << ")";
	debugText.Print(UshiGoalCount.str(), 0, 540, 1.0f);

	// 羊のゴール
	std::ostringstream HitsuziGoalCount;
	HitsuziGoalCount << "HitsuziGoalCount("
		<< std::fixed << std::setprecision(5)
		<< goalSheep << ")";
	debugText.Print(HitsuziGoalCount.str(), 200, 540, 1.0f);

	// 豚のゴール
	std::ostringstream ButaGoalCount;
	ButaGoalCount << "ButaGoalCount("
		<< std::fixed << std::setprecision(5)
		<< goalPigs << ")";
	debugText.Print(ButaGoalCount.str(), 400, 540, 1.0f);

	// スコア
	std::ostringstream GameScore;
	GameScore << "GameScore("
		<< std::fixed << std::setprecision(5)
		<< score << ")";
	debugText.Print(GameScore.str(), 0, 560, 1.0f);
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