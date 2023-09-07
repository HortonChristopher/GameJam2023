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

	if (tekiReaction != Teki_None)
	{
		switch (tekiReaction)
		{
		case Teki_None:
			break;
		case Teki_Set:
			break;
		case Teki_Moving:
			Move(true); // Moving AWAY from teki in direction Player is facing
			break;
		case Teki_Standing:
			if (timer >= 300.0f)
			{
				tekiSet = false;
				timer = 0.0f;
				tekiReaction = Teki_None;
			}

			timer += 1.0f;
			break;
		default:
			break;
		}
	}
	else if (esaReaction != Esa_None)
	{
		switch (esaReaction)
		{
		case Esa_None:
			break;
		case Esa_Set:
			break;
		case Esa_Moving:
			Move(true); // Moving towards target
			break;
		case Esa_Standing:
			if (timer >= 300.0f)
			{
				esaSet = false;
				timer = 0.0f;
				esaReaction = Esa_None;
			}

			timer += 1.0f;
			break;
		default:
			break;
		}
	}
	else
	{
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
			Move(true); // Moving towards target
		}

		timer += 1.0f;

		if (timer >= 600.0f && moving)
		{
			moving = false;

			timer = 0.0f;

			randomCooldown = (float)(rand() % 60 + 30);
		}
	}
}

void Tori::SetNewMovementPosition()
{
	target = { (float)(rand() % 300 - 150), 0.0f, (float)(rand() % 300 - 150) };

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	x = (target.x - position.x);
	z = (target.z - position.z);
	float radians = atan2(z, x);
	degrees = DirectX::XMConvertToDegrees(radians);

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;
}

void Tori::Move(bool forwardBackwards)
{
	if (forwardBackwards)
	{
		if (tekiReaction == Teki_Moving)
		{
			float randomVarience = (float)(rand() % 21);

			position.x += velocity.x * speed * (2.0f + randomVarience / 10.0f);
			position.y += velocity.y * speed * (2.0f + randomVarience / 10.0f);
			position.z += velocity.z * speed * (2.0f + randomVarience / 10.0f);
		}
		else
		{
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
			position.z += velocity.z * speed;
		}
	}
	else
	{
		position.x -= velocity.x * speed;
		position.y -= velocity.y * speed;
		position.z -= velocity.z * speed;
	}

	SetRotation({ rotation.x, -degrees - yRotationOffset, rotation.z });
	SetPosition(position);
}

void Tori::UpdateEntitiesInRange(XMFLOAT3 tekiPosition, XMFLOAT3 esaPosition, XMFLOAT3 playerPosition)
{
	if (tekiPosition.x != 2500 || tekiPosition.y != 2500 || tekiPosition.z != 2500) {
		TekiInRange(tekiPosition, playerPosition);
	}

	if (tekiReaction == Teki_None)
	{
		if (esaPosition.x != 2500 || esaPosition.y != 2500 || esaPosition.z != 2500) {
			EsaInRange(esaPosition);
		}
	}
}

bool Tori::ItemIntersection(float radius1, const XMFLOAT3& center2, float radius2)
{
	// Calculate the squared distance between the centers of the spheres along the X and Z axes
	float dx = center2.x - position.x;
	float dz = center2.z - position.z;
	float squaredDistance = dx * dx + dz * dz;

	// Calculate the squared sum of the radii
	float squaredSumRadii = radius1 + radius2;
	squaredSumRadii *= squaredSumRadii;

	// Check if the spheres intersect along the X and Z axes
	return squaredDistance <= squaredSumRadii;
}

void Tori::EsaInRange(XMFLOAT3 esaPosition)
{
	if (ItemIntersection(5.0f, esaPosition, 60.0f) && !ItemIntersection(5.0f, esaPosition, 5.0f))
	{
		if (!esaSet)
		{
			float magnitude = (float)sqrt((esaPosition.x - position.x) * (esaPosition.x - position.x) + (esaPosition.y - position.y) * (esaPosition.y - position.y) + (esaPosition.z - position.z) * (esaPosition.z - position.z));

			velocity.x = (esaPosition.x - position.x) / magnitude;
			velocity.y = (esaPosition.y - position.y) / magnitude;
			velocity.z = (esaPosition.z - position.z) / magnitude;

			x = (esaPosition.x - position.x);
			z = (esaPosition.z - position.z);
			float radians = atan2(z, x);
			degrees = DirectX::XMConvertToDegrees(radians);
			esaSet = true;
			timer = 0.0f;
			moving = false;
			esaReaction = Esa_Moving;
		}
	}
	else if (ItemIntersection(5.0f, esaPosition, 5.0f))
	{
		esaReaction = Esa_Standing;
	}
	else
	{
		esaSet = false;
		esaReaction = Esa_None;
	}
}

void Tori::TekiInRange(XMFLOAT3 tekiPosition, XMFLOAT3 playerPosition)
{
	if (ItemIntersection(5.0f, tekiPosition, 100.0f))
	{
		if (!tekiSet)
		{
			float magnitude = (float)sqrt((tekiPosition.x - playerPosition.x) * (tekiPosition.x - playerPosition.x) + (tekiPosition.y - playerPosition.y) * (tekiPosition.y - playerPosition.y) + (tekiPosition.z - playerPosition.z) * (tekiPosition.z - playerPosition.z));

			velocity.x = (tekiPosition.x - playerPosition.x) / magnitude;
			velocity.y = (tekiPosition.y - playerPosition.y) / magnitude;
			velocity.z = (tekiPosition.z - playerPosition.z) / magnitude;

			x = (tekiPosition.x - playerPosition.x);
			z = (tekiPosition.z - playerPosition.z);
			float radians = atan2(z, x);
			degrees = DirectX::XMConvertToDegrees(radians);

			tekiSet = true;
			timer = 0.0f;
			moving = false;
			tekiReaction = Teki_Moving;
		}
	}
	else if (ItemIntersection(5.0f, tekiPosition, 5.0f))
	{
		tekiReaction = Teki_Standing;
	}
	else
	{
		tekiSet = false;
		tekiReaction = Teki_None;
	}
}

float Tori::SquaredDistance(const XMFLOAT3& position1, const XMFLOAT3& position2) {
	float dx = position2.x - position1.x;
	float dy = position2.y - position1.y;
	float dz = position2.z - position1.z;
	return dx * dx + dy * dy + dz * dz;
}