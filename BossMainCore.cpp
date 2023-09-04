#include "BossMainCore.h"

using namespace DirectX;

BossMainCore* BossMainCore::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossMainCore* instance = new BossMainCore();
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

bool BossMainCore::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// モデルのセット
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);

	// 体力の初期化
	life = lifeMax;

	// カラー変更タイムレートの初期化
	colorTimeRate = 1.0f;

	//破壊タイマーのリセット
	DestroyPartTime = DestroyDefault;

	// 変数のリセット
	VarReset();

	return true;
}

void BossMainCore::Update()
{
	ObjObject::Update();

	ChangePosition();

	if (isAlive)
	{
		// 生存時のカラー変更
		AliveChangeColor();
		// ヒット時のカラー変更
		HitChangeColor();

		// HPが0になったら撃破
		if (life <= 0.0f)
		{
			if (DestroyPartTime > 0)
			{
				DestroyPartTime--;
			}

			isAlive = false;
		}

		onTimer--;
		if (onTimer < 0.0f)
		{
			isBreak = true;
		}
	}

	if (!isAlive)
	{
		color = breakColor;
	}
}

void BossMainCore::ChangePosition()
{
	if (isAlive)
	{
		positionTimeRate += 0.01f;
		if (positionTimeRate > 1.0f)
		{
			positionTimeRate = 1.0f;
		}
	}

	if (!isAlive)
	{
		positionTimeRate -= 0.01f;
		if (positionTimeRate < 0.0f)
		{
			positionTimeRate = 0.0f;
		}
	}

	position = Lerp::LerpFloat3(OffPosition, OnPosition, positionTimeRate);
}

void BossMainCore::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, aliveColor, colorTimeRate);
}

void BossMainCore::AliveChangeColor()
{
	colorTimeRate2 += 0.02;
	if (colorTimeRate2 > 1.0f)
	{
		colorTimeRate2 = 0.0f;
	}
	aliveColor = Lerp::LerpFloat4(baseColor, breakColor, colorTimeRate2);
}

void BossMainCore::TimerReset(int Timer, int ResetValue)
{
	Timer = ResetValue;
}

void BossMainCore::VarReset()
{
	// 生存フラグの初期化
	isAlive = false;

	// メインコアの出現時間
	onTimer = 400.0f;
}
