#include "GageUI.h"

GageUI* GageUI::Create(XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color)
{
	// �C���X�^���X�𐶐�
	GageUI* instance = new GageUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, size, color)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool GageUI::Initialize(XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color)
{
	// UI�̃x�[�X
	gageBase = Sprite::Create(TextureNumber::bonus_base, position);
	gageMain = Sprite::Create(TextureNumber::game_boss_gauge, { position.x + 16.0f , position.y - 16.0f });

	// �����Q�[�W
	// gageMain->SetColor({ 0.6f, 0.6f, 0.1f, 1.0f });
	gageMain->SetColor(color);
	gageMain->SetSize(size);
	gageMain->SetAnchorPoint({ 0.0f, 1.0f });

	// ���C���Q�[�W
	gageBase->SetAnchorPoint({ 0.0f, 1.0f });

	maxSize = size;

	return true;
}

void GageUI::Update(float nowVal, float maxVal, XMFLOAT2 position, XMFLOAT4 start_color, XMFLOAT4 end_color)
{
	// �Q�[�W�ɓ����l�̊������v�Z
	ratio = nowVal / maxVal;
	nowSize.x = ratio * maxSize.x;

	gageColor = Lerp::LerpFloat4(end_color, start_color, ratio);

	// ���W���Z�b�g
	gageBase->SetPosition(position);
	gageMain->SetPosition({ position.x + 16.0f , position.y - 16.0f });

	gageMain->SetColor(gageColor);

	// �T�C�Y���Z�b�g
	gageMain->SetSize(nowSize);
}

void GageUI::Draw()
{
	gageBase->Draw();
	gageMain->Draw();
}
