#include "Hitsuji.h"

std::unique_ptr<Hitsuji> Hitsuji::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, bool initialB)
{
	// 3Dオブジェクトのインスタンスを生成
	Hitsuji* instance = new Hitsuji();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale, initialB)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<Hitsuji>(instance);
}

bool Hitsuji::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, bool initialB)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;
	this->initial = initialB;

	goal = new Goal();

	return true;
}

void Hitsuji::Update()
{
	ObjObject::Update();

	if (!goalFlag)
	{
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

				if (timer > 240.0f)
				{
					timer = 0.0f;
					tekiReaction = Teki_Standing;
				}

				timer += 1.0f;
				break;
			case Teki_Standing:
				if (timer >= standingTimer)
				{
					tekiSet = false;
					timer = timerReset;
					tekiReaction = Teki_None;
				}

				timer += timerOneFrame;
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
				if (timer >= standingTimer)
				{
					esaSet = false;
					timer = timerReset;
					esaReaction = Esa_None;
				}

				timer += timerOneFrame;
				break;
			default:
				break;
			}
		}
		else
		{
			if (timer >= randomCooldown && !moving)
			{
				if (timer >= randomCooldown)
				{
					SetNewMovementPosition();

					moving = true;
				}

				timer = timerReset;
			}

			if (moving)
			{
				Move(true); // Moving towards target
			}

			timer += timerOneFrame;

			if (timer >= wanderMaxTimer && moving)
			{
				moving = false;

				timer = timerReset;

				if (initial)
				{
					initial = false;
				}

				randomCooldown = (float)(rand() % randomCooldownTimesTwo + randomCooldownTime);
			}
		}
	}
	else
	{
		if (!goalInternalSet)
		{
			for (int i = 0; i < 20; i++)
			{
				if (goalNumber == i)
				{
					target = goal->sheepGoalPositions[i];

					goalMagnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

					velocity.x = (target.x - position.x) / goalMagnitude;
					velocity.y = (target.y - position.y) / goalMagnitude;
					velocity.z = (target.z - position.z) / goalMagnitude;
				}
			}

			goalInternalSet = true;
		}
		else
		{
			if (!ItemIntersection(2.0f, target, 2.0f))
			{
				RotationVectorSet(target, position);

				position.x += velocity.x * speed;
				position.y += velocity.y * speed;
				position.z += velocity.z * speed;

				SetRotation({ rotation.x, -degrees - yRotationOffset, rotation.z });
			}
			else
			{
				position = target;
				rotation = { 0.0f, 180.0f, 0.0f };
				SetRotation(rotation);
			}
		}

		SetPosition(position);
	}
}

void Hitsuji::SetNewMovementPosition()
{
	if (initial)
	{
		target = { 0.0f, 0.0f, 155.0f };
	}
	else
	{
		target = { (float)(rand() % mapBoundariesTimesTwo - mapBoundaries), zeroPointFiveFloat, (float)(rand() % mapBoundariesTimesTwo - mapBoundaries) };
	}

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	RotationVectorSet(target, position);

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;
}

void Hitsuji::Move(bool forwardBackwards)
{
	if (forwardBackwards)
	{
		if (tekiReaction == Teki_Moving)
		{
			float randomVarience = ((float)(rand() % tekiRandomVarience)) / divideByTen;

			position.x += velocity.x * speed * (tekiFleeMultiplier + randomVarience);
			position.y += velocity.y * speed * (tekiFleeMultiplier + randomVarience);
			position.z += velocity.z * speed * (tekiFleeMultiplier + randomVarience);
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

	if (!initial)
	{
		checkBoundaries();
	}

	SetRotation({ rotation.x, -degrees - yRotationOffset, rotation.z });
	SetPosition(position);
}

void Hitsuji::UpdateEntitiesInRange(XMFLOAT3 tekiPosition, XMFLOAT3 esaPosition, XMFLOAT3 playerPosition)
{
	if (!goalFlag)
	{
		if (tekiPosition.x != defaultUpdateEntitiesPosition || tekiPosition.y != defaultUpdateEntitiesPosition || tekiPosition.z != defaultUpdateEntitiesPosition) {
			TekiInRange(tekiPosition, playerPosition);
		}

		if (tekiReaction == Teki_None)
		{
			if (esaPosition.x != defaultUpdateEntitiesPosition || esaPosition.y != defaultUpdateEntitiesPosition || esaPosition.z != defaultUpdateEntitiesPosition) {
				EsaInRange(esaPosition);
			}
		}
	}
}

bool Hitsuji::ItemIntersection(float radius1, const XMFLOAT3& center2, float radius2)
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

void Hitsuji::EsaInRange(XMFLOAT3 esaPosition)
{
	if (ItemIntersection(toriRadius, esaPosition, esaMaxDistance) && !ItemIntersection(toriRadius, esaPosition, esaMinDistance))
	{
		if (!esaSet)
		{
			float magnitude = (float)sqrt((esaPosition.x - position.x) * (esaPosition.x - position.x) + (esaPosition.y - position.y) * (esaPosition.y - position.y) + (esaPosition.z - position.z) * (esaPosition.z - position.z));

			velocity.x = (esaPosition.x - position.x) / magnitude;
			velocity.y = (esaPosition.y - position.y) / magnitude;
			velocity.z = (esaPosition.z - position.z) / magnitude;

			RotationVectorSet(esaPosition, position);

			esaSet = true;
			timer = timerReset;
			moving = false;
			esaReaction = Esa_Moving;
		}
	}
	else if (ItemIntersection(toriRadius, esaPosition, esaMinDistance))
	{
		esaReaction = Esa_Standing;
	}
	else
	{
		esaSet = false;
		esaReaction = Esa_None;
	}
}

void Hitsuji::TekiInRange(XMFLOAT3 tekiPosition, XMFLOAT3 playerPosition)
{
	if (ItemIntersection(toriRadius, tekiPosition, tekiDistance)) // To do: Move this center to in front of the item. TL;DR just place the center half the radius in the forward direction the player is facing at the time of placement.
	{
		if (!tekiSet)
		{
			float magnitude = (float)sqrt((tekiPosition.x - playerPosition.x) * (tekiPosition.x - playerPosition.x) + (tekiPosition.y - playerPosition.y) * (tekiPosition.y - playerPosition.y) + (tekiPosition.z - playerPosition.z) * (tekiPosition.z - playerPosition.z));

			velocity.x = (tekiPosition.x - playerPosition.x) / magnitude;
			velocity.y = (tekiPosition.y - playerPosition.y) / magnitude;
			velocity.z = (tekiPosition.z - playerPosition.z) / magnitude;

			RotationVectorSet(tekiPosition, playerPosition);

			tekiSet = true;
			timer = timerReset;
			moving = false;
			tekiReaction = Teki_Moving;
		}
	}
	else
	{
		tekiSet = false;
		tekiReaction = Teki_None;
	}
}

void Hitsuji::RotationVectorSet(XMFLOAT3 target, XMFLOAT3 origin)
{
	x = (target.x - origin.x);
	z = (target.z - origin.z);
	float radians = atan2(z, x);
	degrees = DirectX::XMConvertToDegrees(radians);
}

float Hitsuji::SquaredDistance(const XMFLOAT3& position1, const XMFLOAT3& position2)
{
	float dx = position2.x - position1.x;
	float dy = position2.y - position1.y;
	float dz = position2.z - position1.z;
	return dx * dx + dy * dy + dz * dz;
}

void Hitsuji::checkBoundaries()
{
	if (position.x >= 149.0f)
	{
		position.x = 149.0f;
	}
	else if (position.x <= -149.0f)
	{
		position.x = -149.0f;
	}
	if (position.z >= 149.0f)
	{
		position.z = 149.0f;
	}
	else if (position.z <= -149.0f)
	{
		if (position.x < 30.0f && position.x > -30.0f)
		{
			goalFlag = true;
		}
		else
		{
			position.z = -149.0f;
		}
	}
}