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

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �X�R�A�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(score_txt, L"Resources/Sprite/Common/common_dtxt_2.png")) {
		assert(0);
		return;
	}
	// �X�R�A�e�L�X�g������
	scoreText.Initialize(score_txt);

	resultPage = Sprite::Create(result, { 0.0f, 0.0f });

	cowItemIcon = Sprite::Create(TextureNumber::cow_icon, { 225.0f,365.0f });
	cowItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	cowItemIcon->SetSize({ 96.0f, 96.0f });

	sheepItemIcon = Sprite::Create(TextureNumber::sheep_icon, { 544.0f,365.0f });
	sheepItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	sheepItemIcon->SetSize({ 96.0f, 96.0f });

	pigItemIcon = Sprite::Create(TextureNumber::pig_icon, { 863.0f, 365.0f });
	pigItemIcon->SetAnchorPoint({ 0.5f, 0.5f });
	pigItemIcon->SetSize({ 96.0f, 96.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	sound->PlayWav("SE/Game/Result.wav", 0.1f, true);
}

void Result::Finalize()
{
}

void Result::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		sound->StopWav("SE/Game/Result.wav");
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//�J�����X�V
	camera->Update();

	// �X�R�A�̕`��
	std::ostringstream Score;
	Score << std::fixed << std::setprecision(0) << scoreManager->score;
	scoreText.Print(Score.str(), { 710, 570 }, { 1.0f, 0.6f, 0.1f, 1.0f }, 3.0f);

	std::ostringstream CowGoalCount;
	CowGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalHorse;
	scoreText.Print(CowGoalCount.str(), { 395.0f,400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);

	std::ostringstream SheepGoalCount;
	SheepGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalSheep;
	scoreText.Print(SheepGoalCount.str(), { 714.0f,400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);

	std::ostringstream PigGoalCount;
	PigGoalCount << std::fixed << std::setprecision(0) << scoreManager->goalPigs;
	scoreText.Print(PigGoalCount.str(), { 1033.0f, 400.0f }, { 1.0f, 0.6f, 0.1f, 1.0f }, 2.0f);
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
	scoreText.DrawAll(cmdList);
	cowItemIcon->Draw();
	sheepItemIcon->Draw();
	pigItemIcon->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}