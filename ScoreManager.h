#pragma once
class ScoreManager
{
public: // �ÓI�����o�֐�
	static ScoreManager* GetInstance();
public: // �����o�֐�]
	const float& GetScore() { return score; }

public: // �����o�ϐ�
	float score = 0.0f;

	int goalPigs = 0;

	int goalSheep = 0;

	int goalHorse = 0;
};