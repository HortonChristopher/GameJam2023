#include "StageSelect.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::を省略
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

StageSelect::StageSelect()
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	//音声のロード
	sound->LoadWav("SE/Game/Title.wav");

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::explanation1, L"Resources/Sprite/ResultUI/result_bg_lose.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::explanation2, L"Resources/Sprite/ResultUI/result_bg_win.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");

	setsumei1 = Sprite::Create(explanation1, { 0.0f, 0.0f });
	setsumei2 = Sprite::Create(explanation2, { 0.0f, 0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	sound->PlayWav("SE/Game/Title.wav", 0.1f, true);
}

void StageSelect::Finalize()
{
}

void StageSelect::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		switch (explanationPage)
		{
		case 0:
			explanationPage = 1;
			break;
		case 1:
			sound->StopWav("SE/Game/Title.wav");
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
			break;
		default:
			break;
		}
	}

	//カメラ更新
	camera->Update();
}

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画

	switch (explanationPage)
	{
	case 0:
		setsumei1->Draw();
		break;
	case 1:
		setsumei2->Draw();
		break;
	default:
		break;
	}

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


