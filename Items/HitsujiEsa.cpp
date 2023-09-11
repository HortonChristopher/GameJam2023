#include "HitsujiEsa.h"

std::unique_ptr<HitsujiEsa> HitsujiEsa::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	HitsujiEsa* instance = new HitsujiEsa();
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

	return std::unique_ptr<HitsujiEsa>(instance);
}

bool HitsujiEsa::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void HitsujiEsa::Update()
{
	ObjObject::Update();

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}