#include "Result.h"

Result::Result()
{
}

Result::~Result()
{
}

void Result::Initialize()
{
	// �T�E���h������
	sound->Initialize();

	//�����̃��[�h
	sound->LoadWav("SE/Game/Result.wav");

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::result, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	//���w�i
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");

	resultPage = Sprite::Create(result, { 0.0f, 0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	//���w�i������
	Black = Sprite::Create(TextureNumber::black, { 0.0f, 0.0f });
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	//���O�i�̃A���t�@�l�̃��Z�b�g
	BlackAlpha = 1.0f;
	BlackFlag = false;
	SceneFlag = false;

	sound->PlayWav("SE/Game/Result.wav", 0.5f, true);
}

void Result::Finalize()
{
}

void Result::Update()
{
	if (BlackFlag == false)
	{
		BlackAlpha -= 0.04f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

		if (BlackAlpha < 0.0f)
		{
			BlackAlpha = 0.0f;
			BlackFlag = true;
		}
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		SceneFlag = true;
	}

	if (BlackFlag == true && SceneFlag == true)
	{
		if (BlackAlpha < 1.0f)
		{
			BlackAlpha += 0.04f;
			Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
		}
	}

	if (BlackAlpha >= 1.0f)
	{
		sound->StopWav("SE/Game/Result.wav");
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//�J�����X�V
	camera->Update();
}

void Result::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��

	resultPage->Draw();

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