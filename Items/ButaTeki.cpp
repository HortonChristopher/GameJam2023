#include "ButaTeki.h"

std::unique_ptr<ButaTeki> ButaTeki::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ButaTeki* instance = new ButaTeki();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, scale)) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<ButaTeki>(instance);
}

bool ButaTeki::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void ButaTeki::Update()
{
	ObjObject::Update();

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}