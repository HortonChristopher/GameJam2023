#include "Esa.h"

std::unique_ptr<Esa> Esa::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Esa* instance = new Esa();
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

	return std::unique_ptr<Esa>(instance);
}

bool Esa::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;

	return true;
}

void Esa::Update()
{
	ObjObject::Update();

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}

	//Debug Start
	/*char msgbuf[256];
	sprintf_s(msgbuf, 256, "X: %d\n", deathTimer);
	OutputDebugStringA(msgbuf);*/
	//Debug End
}