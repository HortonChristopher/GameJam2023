#include "UshiEsa.h"

std::unique_ptr<UshiEsa> UshiEsa::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	UshiEsa* instance = new UshiEsa();
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

	return std::unique_ptr<UshiEsa>(instance);
}

bool UshiEsa::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void UshiEsa::Update()
{
	ObjObject::Update();

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}