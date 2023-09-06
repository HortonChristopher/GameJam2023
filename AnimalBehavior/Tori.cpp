#include "Tori.h"

std::unique_ptr<Tori> Tori::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	Tori* instance = new Tori();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<Tori>(instance);
}

bool Tori::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void Tori::Update()
{
	ObjObject::Update();

	if (!goalFlag && timer >= randomCooldown && !moving)
	{
		if (timer >= randomCooldown)
		{
			SetNewMovementPosition();

			moving = true;
		}

		timer = 0.0f;
	}

	if (moving)
	{
		Move();
	}

	timer += 1.0f;

	if (timer >= 600.0f && moving)
	{
		moving = false;

		timer = 0.0f;

		randomCooldown = (float)(rand() % 60 + 30);
	}
}

void Tori::SetNewMovementPosition()
{
	target = { (float)(rand() % 300 - 150), 0.0f, (float)(rand() % 300 - 150) };

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;
}

void Tori::Move()
{
	position.x += velocity.x * speed;
	position.y += velocity.y * speed;
	position.z += velocity.z * speed;

	SetPosition(position);
}