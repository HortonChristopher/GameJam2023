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
	sound->LoadWav("SE/Title/Start.wav");
	sound->LoadWav("SE/Title/Move.wav");
	sound->LoadWav("SE/Title/StageSelect.wav");


	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::result_bg, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::triangle_up, L"Resources/Sprite/TitleUI/Triangle_UP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::triangle_down, L"Resources/Sprite/TitleUI/Triangle_DOWN.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage_select, L"Resources/Sprite/TitleUI/StageSelect.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage01, L"Resources/Sprite/TitleUI/STAGE_01.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage02, L"Resources/Sprite/TitleUI/STAGE_02.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_s, L"Resources/Sprite/TitleUI/Key_S.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_w, L"Resources/Sprite/TitleUI/Key_W.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_space, L"Resources/Sprite/TitleUI/Key_SPACE.png")) {
		assert(0);
		return;
	}

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");

	stageSelectBG = Sprite::Create(TextureNumber::result_bg, { 0.0f,0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });
}

void StageSelect::Finalize()
{
}

void StageSelect::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
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
	

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}


