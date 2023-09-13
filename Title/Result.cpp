#include "Result.h"

Result::Result()
{
}

Result::~Result()
{
}

void Result::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	//音声のロード
	sound->LoadWav("SE/Game/Result.wav");

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::result, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// スコアテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(score_txt, L"Resources/Sprite/Common/common_dtxt_2.png")) {
		assert(0);
		return;
	}
	// スコアテキスト初期化
	scoreText.Initialize(score_txt);

	resultPage = Sprite::Create(result, { 0.0f, 0.0f });

	cowItemIcon = Sprite::Create(TextureNumber::cow_icon, { 225.0f,365.0f });
	cowItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	cowItemIcon->SetSize({ 96.0f, 96.0f });

	sheepItemIcon = Sprite::Create(TextureNumber::sheep_icon, { 544.0f,365.0f });
	sheepItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	sheepItemIcon->SetSize({ 96.0f, 96.0f });

	pigItemIcon = Sprite::Create(TextureNumber::pig_icon, { 863.0f, 365.0f });
	pigItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	pigItemIcon->SetSize({ 96.0f, 96.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	sound->PlayWav("SE/Game/Result.wav", 0.1f, true);
}

void Result::Finalize()
{
}

void Result::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sound->StopWav("SE/Game/Result.wav");
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//カメラ更新
	camera->Update();

	// スコアの描画
	std::ostringstream Score;
	Score << std::fixed << std::setprecision(0) << scoreManager->score;
	scoreText.Print(Score.str(), { 710, 570 }, { 1.0f, 0.6f, 0.1f, 1.0f }, 3.0f);

	std::ostringstream CowGoalCount;
	CowGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalHorse;
	scoreText.Print(CowGoalCount.str(), { 395.0f,400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);

	std::ostringstream SheepGoalCount;
	SheepGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalSheep;
	scoreText.Print(SheepGoalCount.str(), { 714.0f,400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);

	std::ostringstream PigGoalCount;
	PigGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalPigs;
	scoreText.Print(PigGoalCount.str(), { 1033.0f, 400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);
}

void Result::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画

	resultPage->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画


	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画
	scoreText.DrawAll(cmdList);
	cowItemIcon->Draw();
	sheepItemIcon->Draw();
	pigItemIcon->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}