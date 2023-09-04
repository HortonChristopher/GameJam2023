#include "DeltaGageUI.h"

DeltaGageUI* DeltaGageUI::Create(XMFLOAT2 position, XMFLOAT2 size)
{
	// インスタンスを生成
	DeltaGageUI* instance = new DeltaGageUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, size)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool DeltaGageUI::Initialize(XMFLOAT2 position, XMFLOAT2 size)
{
	// UIのベース
	gageBase = Sprite::Create(TextureNumber::game_boss_frame_1, { position.x + 10.0f, position.y });
	gageBase->SetAnchorPoint({ 1.0f, 0.5f });

	// 差分ゲージ
	gageDelta = Sprite::Create(TextureNumber::game_boss_gauge, position);
	gageDelta->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	gageDelta->SetSize(size);
	gageDelta->SetAnchorPoint({ 1.0f, 0.5f });

	// メインゲージ
	gageMain = Sprite::Create(TextureNumber::game_boss_gauge, position);
	gageMain->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	gageMain->SetSize(size);
	gageMain->SetAnchorPoint({ 1.0f, 0.5f });

	// UIのフレーム
	gageCover = Sprite::Create(TextureNumber::game_boss_frame_2, { position.x + 10.0f, position.y });
	gageCover->SetAnchorPoint({ 1.0f, 0.5f });

	maxSize = size;

	return true;
}

void DeltaGageUI::Update(float nowVal, float maxVal, XMFLOAT2 position)
{
	// ゲージに入れる値の割合を計算
	ratio = nowVal / maxVal;
	nowSize.x = ratio * maxSize.x;

	// 差分ゲージの計算
	delatSize = Lerp::LerpFloat2(gageDelta->GetSize(), nowSize, 0.1f);

	// 座標をセット
	gageBase->SetPosition({ position.x + 10.0f, position.y });
	gageDelta->SetPosition(position);
	gageMain->SetPosition(position);
	gageCover->SetPosition({ position.x + 10.0f, position.y });

	// サイズをセット
	gageMain->SetSize(nowSize);
	gageDelta->SetSize(delatSize);
}

void DeltaGageUI::Draw()
{
	gageBase->Draw();
	gageDelta->Draw();
	gageMain->Draw();
	gageCover->Draw();
}
