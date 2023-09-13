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
	sound->LoadWav("SE/Game/MenuMove.wav");

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

	//ローディング用
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}

	Black = Sprite::Create(TextureNumber::black, { 0.0f, 0.0f });
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	//黒前景のアルファ値のリセット
	BlackAlpha = 1.0f;


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
	if (BlackAlpha > 0.0f && changeTimerBool == false)
	{
		BlackAlpha -= 0.04f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
	}
	else if(BlackAlpha < 0.0f && changeTimerBool == false)
	{
		BlackAlpha = 0.0f;
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		switch (explanationPage)
		{
		case 0:
			sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
			explanationPage = 1;
			break;
		case 1:
			if (input->TriggerKey(DIK_SPACE))
			{
				if (!changeTimerBool)
				{
					sound->PlayWav("SE/Game/MenuMove.wav", 0.7f);
					changeTimerBool = true;
				}
			}
			
			break;
		default:
			break;
		}
	}

	if (changeTimerBool && (changeTimer >= changeTimerMax))
	{
		sound->StopWav("SE/Game/Title.wav");
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	else if (changeTimerBool)
	{
		changeTimer += 1.0f;
		BlackAlpha += 0.04f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
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
	Black->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}


