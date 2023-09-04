#include "BossTurret.h"

using namespace DirectX;

BossTurret* BossTurret::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossTurret* instance = new BossTurret();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool BossTurret::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// モデルのセット
	modelBossTurret = ObjModel::CreateFromOBJ("Turret");
	SetModel(modelBossTurret);

	// 生存フラグの初期化
	isAlive = true;

	// 体力の初期化
	life = lifeMax;

	// カラー変更タイムレートの初期化
	colorTimeRate = 1.0f;

	return true;
}

void BossTurret::Update(XMFLOAT3 target_position)
{
	ObjObject::Update();

	// ヒット時のカラー変更

	HitChangeColor();

	/*if (input->PushKey(DIK_UP) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x -= 1.0f;
	}
	if (input->PushKey(DIK_DOWN) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x += 1.0f;
	}*/

	// HPが0になったら撃破
	if (life <= 0.0f)
	{
		isAlive = false;
	}

	// 撃破時にタレットを下に向ける
	if (isAlive == false)
	{
		rotationTimeRate += 0.001;
		if (rotationTimeRate > 1.0f)
		{
			rotationTimeRate = 1.0f;
		}
		rotation = Lerp::LerpFloat3(GetRotation(), breakRotation, rotationTimeRate);
	}
	else
	{
		float axisX = (target_position.x - position.x);
		float axisY = (target_position.y - position.y);
		float axisZ = (target_position.z - position.z);
		float hypotenuse = sqrt(pow(axisX, 2) + pow(axisY, 2) + pow(axisZ, 2));
		float radians = atan2(axisZ, axisX);
		float degrees = XMConvertToDegrees(radians);
		float radians2 = asin(axisY / hypotenuse);
		float degrees2 = XMConvertToDegrees(radians2);
		SetRotation({ -degrees2, rotation.y, rotation.z });
		externRotationY = -degrees + 90.0f;
	}

	// X軸を制限
	rotation.x = max(rotation.x, -limitRot);
	rotation.x = min(rotation.x, +limitRot);
	SetRotation({ rotation.x, rotation.y, rotation.z });
}

// ヒット時のカラー変更
void BossTurret::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}
