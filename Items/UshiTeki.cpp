#include "UshiTeki.h"

std::unique_ptr<UshiTeki> UshiTeki::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	UshiTeki* instance = new UshiTeki();
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

	return std::unique_ptr<UshiTeki>(instance);
}

bool UshiTeki::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void UshiTeki::Update()
{
	ObjObject::Update();

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}