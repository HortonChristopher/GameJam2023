#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include <sstream>
#include <iomanip>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Lerp.h"
#include "Boss.h"
#include "BossTurret.h"
#include "BossCore.h"
#include "BossMainCore.h"
#include "BossParts.h"
#include "BossShield.h"

class Boss;
class BossCore;
class BossMainCore;
class BossParts;
class BossTurret;
class BossShield;

class StageSelect : public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
// �X�v���C�g�̃e�N�X�`���ԍ�
	static enum TextureNumber
	{
		debug_txt,
		result_bg,
		black,
		triangle_up,
		triangle_down,
		stage_select,
		stage01,
		stage02,
		key_w,
		key_s,
		key_space,
	};

public:
	// �R���X�g���N�^
	StageSelect();

	// �f�X�g���N�^
	~StageSelect();

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;


private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();

	Camera* camera = nullptr;

	Sprite* stageSelectBG = nullptr;
};

