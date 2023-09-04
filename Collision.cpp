#include "Collision.h"

using namespace DirectX;

bool Collision::BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize)
{
	XMVECTOR s1Pos = XMLoadFloat3(&bulletPos);
	XMVECTOR s2Pos = XMLoadFloat3(&bossPos);
	float distance = XMVectorGetX(XMVector3Length(s1Pos - s2Pos));
	float radiusSum = bulletSize + bossSize;

	return distance <= radiusSum;
}

bool Collision::CCDCollisionDetection(XMFLOAT3 prevBulletPos, XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize)
{
	XMVECTOR s1PrevPos = XMLoadFloat3(&prevBulletPos);
	XMVECTOR s1Pos = XMLoadFloat3(&bulletPos);
	XMVECTOR s2Pos = XMLoadFloat3(&bossPos);

	// the vector from the previous position to the current position
	XMVECTOR bulletTravelVector = s1Pos - s1PrevPos;

	// the vector from the boss to the bullet's previous position
	XMVECTOR bossToBulletPrevPosVector = s1PrevPos - s2Pos;

	// the projection of the bossToBulletPrevPosVector onto the bulletTravelVector
	float dotProduct = XMVectorGetX(XMVector3Dot(bossToBulletPrevPosVector, bulletTravelVector));
	float projectionScalar = dotProduct / XMVectorGetX(XMVector3LengthSq(bulletTravelVector));
	XMVECTOR projection = bulletTravelVector * projectionScalar;

	if (dotProduct < 0)
	{
		// the projection is in the opposite direction of the bullet's travel, so the closest point
		// is the bullet's previous position
		float distance = XMVectorGetX(XMVector3Length(s1PrevPos - s2Pos));
		float radiusSum = bulletSize + bossSize;

		return distance <= radiusSum;
	}
	else
	{
		float projectionLength = XMVectorGetX(XMVector3Length(projection));
		float travelLength = XMVectorGetX(XMVector3Length(bulletTravelVector));

		if (projectionLength > travelLength)
		{
			// the projection is longer than the bullet's travel distance, so the closest point
			// is the bullet's current position
			float distance = XMVectorGetX(XMVector3Length(s1Pos - s2Pos));
			float radiusSum = bulletSize + bossSize;

			return distance <= radiusSum;
		}
		else
		{
			// the closest point to the boss along the bullet's path is the end of the projection
			XMVECTOR closestPoint = s1PrevPos + projection;
			float distance = XMVectorGetX(XMVector3Length(closestPoint - s2Pos));
			float radiusSum = bulletSize + bossSize;

			return distance <= radiusSum;
		}
	}
}
