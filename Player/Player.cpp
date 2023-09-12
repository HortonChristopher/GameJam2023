#include "Player.h"

using namespace DirectX;

ID3D12Device* Player::device = nullptr;
//Camera* Player::camera = nullptr;

Player* Player::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelPlayer = ObjModel::CreateFromOBJ("bullet");
	SetModel(modelPlayer);

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �f�o�b�O�e�L�X�g������
	debugText.Initialize(0);

	SetScale({ 1.0f, 1.0f, 1.0f });

	return true;
}

void Player::Update()
{
	/*switch (phase_)
	{
	case State::Stop:
		default:
		StateNum = State::Stop;

		break;

	case State::Walk:
		StateNum = State::Walk;

		break;

	case State::Run:
		StateNum = State::Run;

		break;

	case State::Throw:
		StateNum = State::Throw;

		break;

	case State::Call:
		StateNum = State::Call;

		break;

	}*/


	ObjObject::Update();

	//�u�[�X�g
	//Boost();
	//BoostTest();

	// �I�u�W�F�N�g�ړ�
	Move();

	// ���[�����O
	//Rolling();

	//DebugTextUpdate();
}

void Player::Draw()
{
	ObjObject::Draw();
}

void Player::DebugTextUpdate()
{
	// �v���C���[�̍��W��\��
	std::ostringstream PlayerPosition;
	PlayerPosition << "PlayerPosition:("
		<< std::fixed << std::setprecision(2)
		<< position.x << "," // x
		<< position.y << "," // y
		<< position.z << ") Local"; // z
	debugText.Print(PlayerPosition.str(), 0, 590, 1.0f);

	// �v���C���[�̊p�x��\��
	std::ostringstream PlayerRotation;
	PlayerRotation << "PlayerRotation:("
		<< std::fixed << std::setprecision(2)
		<< rotation.x << "," // x
		<< rotation.y << "," // y
		<< rotation.z << ")"; // z
	debugText.Print(PlayerRotation.str(), 0, 610, 1.0f);
}

void Player::DebugTextDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
}

// �O�����ړ�����
void Player::Move()
{
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetViewMatrix());
	const Vector3 camDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 camDirectionY = Vector3(camMatWorld.r[1].m128_f32[0], 0, camMatWorld.r[1].m128_f32[2]).Normalize();
	const Vector3 camDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	Input* input = Input::GetInstance();


	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_S) || input->PushKey(DIK_W))
	{
	/*	phase_ = State::Run;*/
		moveDirection = {};
		
		if (MoveCancel == false)
		{
			if (input->PushKey(DIK_A))
			{
				moveDirection += camDirectionX * -1;
			}
			else if (input->PushKey(DIK_D))
			{
				moveDirection += camDirectionX;
			}
			if (input->PushKey(DIK_S))
			{
				moveDirection += camDirectionZ * -1;
			}
			else if (input->PushKey(DIK_W))
			{
				moveDirection += camDirectionZ;
			}


			moveDirection.Normalize();
			direction.Normalize();

			float cosA = direction.Dot(moveDirection);

			if (cosA > 1.0f)
			{
				cosA = 1.0f;
			}
			else if (cosA < -1.0f)
			{
				cosA = -1.0f;
			}

			float rotY = (float)acos(cosA) * 180 / 3.14159365f;
			const Vector3 CrossVec = direction.Cross(moveDirection);

			float rotSpeed = rotateSpeed;

			if (abs(rotY) < 55)
			{
				position.x += moveDirection.x * speed;
				position.y += moveDirection.y * speed;
				position.z += moveDirection.z * speed;

				if (position.x > mapBoundaries)
				{
					position.x = mapBoundaries;
				}
				else if (position.x < -mapBoundaries)
				{
					position.x = -mapBoundaries;
				}
				if (position.z > mapBoundaries)
				{
					position.z = mapBoundaries;
				}
				else if (position.z < -mapBoundaries)
				{
					position.z = -mapBoundaries;
				}
			}

			if (rotSpeed > abs(rotY))
			{
				rotSpeed = rotY;
			}

			if (CrossVec.y < 0)
			{
				rotSpeed *= -1;
			}

			rotation.y += rotSpeed;

			XMMATRIX matRotation = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
			XMVECTOR dir = { direction.x, direction.y, direction.z, 0 };
			dir = XMVector3TransformNormal(dir, matRotation);
			direction = dir;

			SetPosition(position);
			SetRotation(rotation);
		}
	}

	if (input->TriggerMouseLeft())
	{
		MoveCancel = true;
	}

	if (MoveCancel == true)
	{
		MoveCancelTimer++;
	}

	if (MoveCancelTimer >= 132)
	{
		MoveCancelTimer = 0;
		MoveCancel = false;
	}


	/*else
	{
		phase_ = State::Stop;
	}*/
}

// �O�������̎��@�̌X��
void Player::Rolling()
{
	Input* input = Input::GetInstance();

	// ���[��
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		if (input->PushKey(DIK_A) && rollRotation.z <= +40.0f)
		{
			rollRotation.z += 5.0f;
		}

		if (input->PushKey(DIK_D) && rollRotation.z >= -40.0f)
		{
			rollRotation.z -= 5.0f;
		}
	}

	// �X����߂�
	if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.z != 0.0f)
	{
		if (rollRotation.z > 0.0f)
		{
			rollRotation.z -= 5.0f;
		}

		if (rollRotation.z < 0.0f)
		{
			rollRotation.z += 5.0f;
		}
	}

	SetRotation({ rotation.x, rotation.y, rollRotation.z });
}

void Player::Boost()
{
	BoostPartColor = { (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , 1.0f };
	if(Input::GetInstance()->PushMouseRight() == true)
	{
		BoostFlag = true;
	}

	if(Input::GetInstance()->PushMouseRight() == false)
	{
		BoostFlag = false;
	}

	if (BoostFlag == true)
	{
		if (BoostPowNow > 0)
		{
			BoostPowNow--;
			totalSpeed = 3.0f;
		}

		if (BoostPowNow <= 0)
		{
			BoostFlag = false;
		}
	}

	if (BoostFlag == false)
	{
		totalSpeed = 2.0f;
		if (BoostPowNow < BoostPowMax)
		{
			BoostPowNow++;
		}
	}
}

void Player::BoostTest()
{
	BoostPartColor = { (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , 1.0f };
	if (Input::GetInstance()->PushMouseRight() == true && boostCancel == false)
	{
		BoostFlag = true;
	}

	if (Input::GetInstance()->PushMouseRight() == false)
	{
		BoostFlag = false;
	}

	if (BoostFlag == true)
	{
		if (BoostPowNow > 0)
		{
			BoostPowNow--;
			boostTimeRate += 0.1f;
		}

		if (BoostPowNow <= 0)
		{
			boostCancel = true;
		}
	}

	if (BoostFlag == false)
	{
		boostTimeRate -= 0.1f;
		if (BoostPowNow < BoostPowMax)
		{
			BoostPowNow++;
		}
	}

	if (boostCancel)
	{
		BoostFlag = false;
		if (BoostPowNow >= BoostPowMax)
		{
			boostCancel = false;
		}
	}

	boostSpeed = Lerp::LerpFloat(boostSpeedMin, boostSpeedMax, boostTimeRate);

	boostSpeed = max(boostSpeed, boostSpeedMin);
	boostSpeed = min(boostSpeed, boostSpeedMax);

	boostTimeRate = max(boostTimeRate, 0.0f);
	boostTimeRate = min(boostTimeRate, 1.0f);

	totalSpeed = (baseSpeed + boostSpeed) - shootSpeed;
}

bool Player::CheckCollisionWithBoss(XMFLOAT3 bossPos, float collisionRadius)
{
	float dx = bossPos.x - position.x;
	float dy = bossPos.y - position.y;
	float dz = bossPos.z - position.z;

	float distance = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

	return distance <= collisionSphereRadius + collisionRadius;
}

void Player::MoveTowards(float& current, float target, float speed, float elapsedTime)
{
	float delta = target - current;
	float step = speed * elapsedTime;
	if (step > abs(delta))
	{
		current = target;
	}
	else
	{
		current += step * (delta < 0 ? -1 : 1);
	}
}