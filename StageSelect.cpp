#include "StageSelect.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::���ȗ�
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
	// �T�E���h������
	sound->Initialize();

	//�����̃��[�h
	sound->LoadWav("SE/Game/Title.wav");
	sound->LoadWav("SE/Game/MenuMove.wav");

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::explanation1, L"Resources/Sprite/ResultUI/result_bg_lose.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::explanation2, L"Resources/Sprite/ResultUI/result_bg_win.png")) {
		assert(0);
		return;
	}

	//���[�f�B���O�p
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}

	Black = Sprite::Create(TextureNumber::black, { 0.0f, 0.0f });
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	//���O�i�̃A���t�@�l�̃��Z�b�g
	BlackAlpha = 1.0f;


	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
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
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	else if (changeTimerBool)
	{
		changeTimer += 1.0f;
		BlackAlpha += 0.04f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
	}

	//�J�����X�V
	camera->Update();
}

void StageSelect::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��

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

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	ObjObject::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	

	// 3D�I�u�W�F�N�g�`��㏈��
	ObjObject::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �O�i�X�v���C�g�`��
	Black->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}


