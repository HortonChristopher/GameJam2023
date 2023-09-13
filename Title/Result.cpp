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
	sound->LoadWav("SE/Title/Start.wav");
	sound->LoadWav("SE/Title/Move.wav");
	sound->LoadWav("SE/Title/StageSelect.wav");

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

	resultPage = Sprite::Create(result, { 0.0f, 0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });
}

void Result::Finalize()
{
}

void Result::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//カメラ更新
	camera->Update();
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


	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}