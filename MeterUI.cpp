#include "MeterUI.h"

MeterUI* MeterUI::Create(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	// �C���X�^���X�𐶐�
	MeterUI* instance = new MeterUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, rotation, color)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool MeterUI::Initialize(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	meterNeedle = Sprite::Create(TextureNumber::speed, { position.x - 48.0f , position.y + 48.0f });
	meterBase = Sprite::Create(TextureNumber::timer_base, position);

	meterNeedle->SetRotation(rotation);
	meterNeedle->SetColor(color);
	meterNeedle->SetAnchorPoint({ 0.5f, 0.5f });

	meterBase->SetAnchorPoint({ 1.0f, 0.0f });

	return true;
}

void MeterUI::Update(float nowVal, float maxVal, XMFLOAT2 position)
{
	// ���[�^�[�ɓ����l�̊������v�Z
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
