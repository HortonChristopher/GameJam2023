#include "ReflectBullet.h"

std::unique_ptr<ReflectBullet> ReflectBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed)
{
	// 3Dオブジェクトのインスタンスを生成
	ReflectBullet* instance = new ReflectBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale, target, speed)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<ReflectBullet>(instance);
}

bool ReflectBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	this->position = position;
	this->scale = scale;
	this->target = target;
	this->speed = speed;

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;

	return true;
}

void ReflectBullet::Update(const XMFLOAT3 reflect_position)
{
	prevPosition = position;

	ObjObject::Update();

	if (isReflect)
	{
		color = baseColor;
		position = Lerp::LerpFloat3(position, reflect_position, 0.1f);
	}
	else if(isReflect == false)
	{
		HitChangeColor();

		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		position.z += velocity.z * speed;

		// HPが0になったら撃破
		if (life <= 0.0f)
		{
			isReflect = true;
		}

		if (--deathTimer <= 0)
		{
			deathFlag = true;
		}
	}
}

void ReflectBullet::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}

void ReflectBullet::RallyReset(const XMFLOAT3 position, const XMFLOAT3 target, const float speed, const float life)
{
	this->position = position;
	this->target = target;
	this->speed = speed;
	this->life = life;

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;

	deathTimer = LifeTime;
}
