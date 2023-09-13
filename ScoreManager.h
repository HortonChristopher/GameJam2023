#pragma once
class ScoreManager
{
public: // 静的メンバ関数
	static ScoreManager* GetInstance();
public: // メンバ関数]
	const float& GetScore() { return score; }

public: // メンバ変数
	float score = 0.0f;

	int goalPigs = 0;

	int goalSheep = 0;

	int goalHorse = 0;
};