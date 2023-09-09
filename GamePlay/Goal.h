#pragma once

#include <DirectXMath.h>
#include <map>
#include <string>

class Goal
{
public:
	std::map<int, DirectX::XMFLOAT3> pigGoalPositions;
	std::map<int, DirectX::XMFLOAT3> sheepGoalPositions;
	std::map<int, DirectX::XMFLOAT3> cowGoalPositions;

	Goal();
};