#include "MeterUI.h"

MeterUI* MeterUI::Create(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	// インスタンスを生成
	MeterUI* instance = new MeterUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, rotation, color)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool MeterUI::Initialize(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	//meterBase = 
	meterNeedle = Sprite::Create(TextureNumber::speed, {position.x, position.y + 44.0f});
	meterBase = Sprite::Create(TextureNumber::meter, position);

	meterNeedle->SetRotation(rotation);
	meterNeedle->SetColor(color);
	meterNeedle->SetAnchorPoint({ 0.9f, 0.5 });

	meterBase->SetAnchorPoint({ 0.5f, 0.5f });
	meterBase->SetSize({210.0f, 120.0f});

	return true;
}

void MeterUI::Update(float nowVal, float maxVal, XMFLOAT2 position)
{
	// メーターに入れる値の割合を計算
	ratio = nowVal / maxVal;
	nowRotation = ratio * maxRotation;

	EasRotation = Easing::OutCubicFloat(meterNeedle->GetRotation(), nowRotation, 0.1f);

	meterNeedle->SetRotation(EasRotation);
}

void MeterUI::Draw()
{
	
	meterBase->Draw();
	meterNeedle->Draw();
}
