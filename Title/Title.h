#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "ObjectParticle.h"
#include "Sound.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Test.h"

class Title : public BaseScene
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
		title_bg,
		titlefont,
		pressspace,
		black
	};

public:
	// �R���X�g�N���^
	Title();

	// �f�X�g���N�^
	~Title();

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;


private: // �����o�ϐ�
	//DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//����
	Input* input = Input::GetInstance();

	//����
	Sound* sound = Sound::GetInstance();

	//�J����
	Camera* camera = nullptr;

	//�X�v���C�g
	Sprite* titleBG = nullptr;
};