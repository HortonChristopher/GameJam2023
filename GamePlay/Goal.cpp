#include "Goal.h"

Goal::Goal()
{
	// West (Pig) goal positions
	pigGoalPositions[0] = DirectX::XMFLOAT3(-170.0f, 0.5f, -50.0f);
	pigGoalPositions[1] = DirectX::XMFLOAT3(-170.0f, 0.5f, -40.0f);
	pigGoalPositions[2] = DirectX::XMFLOAT3(-170.0f, 0.5f, -30.0f);
	pigGoalPositions[3] = DirectX::XMFLOAT3(-170.0f, 0.5f, -20.0f);
	pigGoalPositions[4] = DirectX::XMFLOAT3(-170.0f, 0.5f, -10.0f);
	pigGoalPositions[5] = DirectX::XMFLOAT3(-170.0f, 0.5f, 10.0f);
	pigGoalPositions[6] = DirectX::XMFLOAT3(-170.0f, 0.5f, 20.0f);
	pigGoalPositions[7] = DirectX::XMFLOAT3(-170.0f, 0.5f, 30.0f);
	pigGoalPositions[8] = DirectX::XMFLOAT3(-170.0f, 0.5f, 40.0f);
	pigGoalPositions[9] = DirectX::XMFLOAT3(-170.0f, 0.5f, 50.0f);
	pigGoalPositions[10] = DirectX::XMFLOAT3(-190.0f, 0.5f, -50.0f);
	pigGoalPositions[11] = DirectX::XMFLOAT3(-190.0f, 0.5f, -40.0f);
	pigGoalPositions[12] = DirectX::XMFLOAT3(-190.0f, 0.5f, -30.0f);
	pigGoalPositions[13] = DirectX::XMFLOAT3(-190.0f, 0.5f, -20.0f);
	pigGoalPositions[14] = DirectX::XMFLOAT3(-190.0f, 0.5f, -10.0f);
	pigGoalPositions[15] = DirectX::XMFLOAT3(-190.0f, 0.5f, 10.0f);
	pigGoalPositions[16] = DirectX::XMFLOAT3(-190.0f, 0.5f, 20.0f);
	pigGoalPositions[17] = DirectX::XMFLOAT3(-190.0f, 0.5f, 30.0f);
	pigGoalPositions[18] = DirectX::XMFLOAT3(-190.0f, 0.5f, 40.0f);
	pigGoalPositions[19] = DirectX::XMFLOAT3(-190.0f, 0.5f, 50.0f);

	// South (Sheep) goal positions
	sheepGoalPositions[0] = DirectX::XMFLOAT3(-50.0f, 0.5f, -170.0f);
	sheepGoalPositions[1] = DirectX::XMFLOAT3(-40.0f, 0.5f, -170.0f);
	sheepGoalPositions[2] = DirectX::XMFLOAT3(-30.0f, 0.5f, -170.0f);
	sheepGoalPositions[3] = DirectX::XMFLOAT3(-20.0f, 0.5f, -170.0f);
	sheepGoalPositions[4] = DirectX::XMFLOAT3(-10.0f, 0.5f, -170.0f);
	sheepGoalPositions[5] = DirectX::XMFLOAT3(10.0f, 0.5f, -170.0f);
	sheepGoalPositions[6] = DirectX::XMFLOAT3(20.0f, 0.5f, -170.0f);
	sheepGoalPositions[7] = DirectX::XMFLOAT3(30.0f, 0.5f, -170.0f);
	sheepGoalPositions[8] = DirectX::XMFLOAT3(40.0f, 0.5f, -170.0f);
	sheepGoalPositions[9] = DirectX::XMFLOAT3(50.0f, 0.5f, -170.0f);
	sheepGoalPositions[10] = DirectX::XMFLOAT3(-50.0f, 0.5f, -190.0f);
	sheepGoalPositions[11] = DirectX::XMFLOAT3(-40.0f, 0.5f, -190.0f);
	sheepGoalPositions[12] = DirectX::XMFLOAT3(-30.0f, 0.5f, -190.0f);
	sheepGoalPositions[13] = DirectX::XMFLOAT3(-20.0f, 0.5f, -190.0f);
	sheepGoalPositions[14] = DirectX::XMFLOAT3(-10.0f, 0.5f, -190.0f);
	sheepGoalPositions[15] = DirectX::XMFLOAT3(10.0f, 0.5f, -190.0f);
	sheepGoalPositions[16] = DirectX::XMFLOAT3(20.0f, 0.5f, -190.0f);
	sheepGoalPositions[17] = DirectX::XMFLOAT3(30.0f, 0.5f, -190.0f);
	sheepGoalPositions[18] = DirectX::XMFLOAT3(40.0f, 0.5f, -190.0f);
	sheepGoalPositions[19] = DirectX::XMFLOAT3(50.0f, 0.5f, -190.0f);

	// East (Cow) goal positions
	cowGoalPositions[0] = DirectX::XMFLOAT3(170.0f, 0.5f, -50.0f);
	cowGoalPositions[1] = DirectX::XMFLOAT3(170.0f, 0.5f, -40.0f);
	cowGoalPositions[2] = DirectX::XMFLOAT3(170.0f, 0.5f, -30.0f);
	cowGoalPositions[3] = DirectX::XMFLOAT3(170.0f, 0.5f, -20.0f);
	cowGoalPositions[4] = DirectX::XMFLOAT3(170.0f, 0.5f, -10.0f);
	cowGoalPositions[5] = DirectX::XMFLOAT3(170.0f, 0.5f, 10.0f);
	cowGoalPositions[6] = DirectX::XMFLOAT3(170.0f, 0.5f, 20.0f);
	cowGoalPositions[7] = DirectX::XMFLOAT3(170.0f, 0.5f, 30.0f);
	cowGoalPositions[8] = DirectX::XMFLOAT3(170.0f, 0.5f, 40.0f);
	cowGoalPositions[9] = DirectX::XMFLOAT3(170.0f, 0.5f, 50.0f);
	cowGoalPositions[10] = DirectX::XMFLOAT3(190.0f, 0.5f, -50.0f);
	cowGoalPositions[11] = DirectX::XMFLOAT3(190.0f, 0.5f, -40.0f);
	cowGoalPositions[12] = DirectX::XMFLOAT3(190.0f, 0.5f, -30.0f);
	cowGoalPositions[13] = DirectX::XMFLOAT3(190.0f, 0.5f, -20.0f);
	cowGoalPositions[14] = DirectX::XMFLOAT3(190.0f, 0.5f, -10.0f);
	cowGoalPositions[15] = DirectX::XMFLOAT3(190.0f, 0.5f, 10.0f);
	cowGoalPositions[16] = DirectX::XMFLOAT3(190.0f, 0.5f, 20.0f);
	cowGoalPositions[17] = DirectX::XMFLOAT3(190.0f, 0.5f, 30.0f);
	cowGoalPositions[18] = DirectX::XMFLOAT3(190.0f, 0.5f, 40.0f);
	cowGoalPositions[19] = DirectX::XMFLOAT3(190.0f, 0.5f, 50.0f);
}