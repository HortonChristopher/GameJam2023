#include "GamePlay.h"

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

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	// スプライト
	gameBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	StoragePos = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	// プレイヤー
	player = Player::Create();

	// 3D OBJ
	ground = ObjObject::Create();
	modelGround = ObjModel::CreateFromOBJ("untitled");
	ground->SetModel(modelGround);

	skydome = ObjObject::Create();
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	skydome->SetModel(modelSkydome);

	ground->SetPosition({ 0.0f, -0.5f, 0.0f });
	ground->SetRotation({ 0.0f, 0.0f, 0.0f });
	ground->SetScale({ 40.0f, 1.0f, 40.0f });

	// プレイヤー
	player->SetPosition({ 0.0f, 0.0f, 0.0f });
	player->SetRotation({ 0.0f, 0.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	// パーティクル
	circleParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	// 座標のセット
	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0, 2, -10 });
	camera->SetUp({ 0, 1, 0 });

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	for (int i = 0; i < 10; i++)
	{
		std::unique_ptr<Tori> newTori = Tori::Create(modelBullet, { 0.0f, player->GetPosition().y, 0.0f }, { 2.0f, 2.0f, 2.0f });
		toriList.push_back(std::move(newTori));
	}

	ShowCursor(false);
}

void GamePlay::Finalize()
{
}

void GamePlay::Update()
{
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

	if (input->PushMouseLeft())
	{
		std::unique_ptr<Esa> newEsa = Esa::Create(modelBullet, player->GetPosition(), {0.5f, 0.5f, 0.5f});
		esaList.push_back(std::move(newEsa));
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	camera->SetTarget(player->GetPosition());
	ground->Update();
	skydome->Update();
	camera->Update();
	camera->SetEye({ camera->GetEye().x, camera->GetEye().y + 10.0f, camera->GetEye().z });
	player->Update();

	for (std::unique_ptr<Tori>& tori : toriList)
	{
		tori->Update();
	}

	for (std::unique_ptr<Esa>& esa : esaList)
	{
		esa->Update();
	}

	esaList.remove_if([](std::unique_ptr<Esa>& esa)
		{
			return esa->GetDeathFlag();
		}
	);
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
	player->Draw();

	for (std::unique_ptr<Tori>& tori : toriList)
	{
		tori->Draw();
	}

	for (std::unique_ptr<Esa>& esa : esaList)
	{
		esa->Draw();
	}

	// パーティクルの描画
	circleParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	
	// 前景スプライト描画
	player->DebugTextDraw();

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
	//マウスの座標
	std::ostringstream MousePosition;
	MousePosition << "MousePosition("
		<< std::fixed << std::setprecision(5)
		<< mousePosition.x << ","
		<< mousePosition.y << ")";
	debugText.Print(MousePosition.str(), 0, 0, 1.0f);

	//レティクルの座標
	std::ostringstream ReticlePosition;
	ReticlePosition << "ReticlePosition("
		<< std::fixed << std::setprecision(5)
		<< ReticlePos.x << ","
		<< ReticlePos.y << ")";
	debugText.Print(ReticlePosition.str(), 0, 60, 1.0f);
}