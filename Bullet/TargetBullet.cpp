#include "TargetBullet.h"

extern HWND hwnd;

using namespace DirectX;

std::unique_ptr<TargetBullet> TargetBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed, XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up, XMFLOAT3 near_point)
{
	// 3Dオブジェクトのインスタンスを生成
	TargetBullet* instance = new TargetBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale, target, speed, eye, aimTarget, up, near_point)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<TargetBullet>(instance);
}

bool TargetBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed,
	XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up, XMFLOAT3 near_point)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;
	this->target = target;
	this->speed = speed;
	this->eyePosition = eye;
	this->targetPosition = aimTarget;
	this->upVector = up;

	prevPosition = this->position;

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;

	// Get the position of the mouse cursor in screen space
	RECT rect;

	//ウィンドウの外側のサイズを取得
	GetClientRect(hwnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	GetCursorPos(&cursorPos);
	ScreenToClient(hwnd, &cursorPos);

	if (cursorPos.x <= 316.0f)
	{
		cursorPos.x = 316.0f;
	}
	if (cursorPos.x >= 1220.0f)
	{
		cursorPos.x = 1220.0f;
	}

	if (cursorPos.y <= 200.0f)
	{
		cursorPos.y = 200.0f;
	}
	if (cursorPos.y >= 640.0f)
	{
		cursorPos.y = 640.0f;
	}

	// Calculate the ray direction in world space
	XMFLOAT3 rayDirection;
	XMVECTOR nearPoint = DirectX::XMVectorSet(cursorPos.x, cursorPos.y, 0.0f, 1.0f);
	XMVECTOR farPoint = DirectX::XMVectorSet(cursorPos.x, cursorPos.y, 1.0f, 1.0f);

	// Create the view matrix based on the camera's position and orientation
	XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eyePosition), XMLoadFloat3(&targetPosition), XMLoadFloat3(&upVector));

	// Combine the view and projection matrices into a single transformation matrix
	XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)width / (float)height, 0.01f, 100.0f);
	XMMATRIX transformationMatrix = DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);

	// Unproject the cursor position from screen space to world space
	nearPoint = DirectX::XMVector3Unproject(nearPoint, 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f, projectionMatrix, viewMatrix, DirectX::XMMatrixIdentity());
	farPoint = DirectX::XMVector3Unproject(farPoint, 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f, projectionMatrix, viewMatrix, DirectX::XMMatrixIdentity());

	permFarPoint = farPoint;
	permNearPoint = nearPoint;

	// Calculate the ray direction in world space based on the camera's position and orientation
	rayDirection = XMFLOAT3(DirectX::XMVectorGetX(farPoint) - DirectX::XMVectorGetX(nearPoint), DirectX::XMVectorGetY(farPoint) - DirectX::XMVectorGetY(nearPoint), DirectX::XMVectorGetZ(farPoint) - DirectX::XMVectorGetZ(nearPoint));
	rayDirection.y += 3.0f; // Offset for camera being ABOVE the player, not lined up on the Y-axis
	XMVECTOR RayDirection = DirectX::XMVector3Normalize(XMLoadFloat3(&rayDirection));

	float scaleFactorX = 1.0f;
	float scaleFactorY = 1.0f;
	float scaleFactorZ = 1.0f;

	// Scale the Y component of the bullet direction vector
	RayDirection *= DirectX::XMVectorSet(scaleFactorX, scaleFactorY, scaleFactorZ, 1.0f);

	//RayDirection = DirectX::XMVector3Normalize(RayDirection);

	// Set the bullet direction to the ray direction
	bulletDirection = XMFLOAT3(RayDirection.m128_f32[0], RayDirection.m128_f32[1], RayDirection.m128_f32[2]);

	float x = (target.x - position.x);
	float z = (target.z - position.z);
	float hypotenuse = sqrt(pow(x, 2) + pow(z, 2));
	float radians = atan2(z, x);
	degrees = XMConvertToDegrees(radians);

	return true;
}

void TargetBullet::Update(XMFLOAT3 prevPos, XMFLOAT3 currentPos, XMMATRIX viewMat, XMFLOAT3 playerRot)
{
	prevPosition = position;
	futurePosition = position + (position - prevPos);

	ObjObject::Update();

	// Update the bullet position based on the bullet direction
	position.x += bulletDirection.x * speed;
	position.y += bulletDirection.y * speed;
	position.z += bulletDirection.z * speed;

	// Compute the player's position relative to the origin
	XMVECTOR playerPositionRelative = XMVectorSubtract(XMLoadFloat3(&currentPos), XMLoadFloat3(&targetPosition));

	// Compute the player's velocity relative to the origin
	XMVECTOR playerVelocityRelative = XMVectorSubtract(XMLoadFloat3(&(currentPos - prevPos)), XMLoadFloat3(&nullVel));

	// Compute the player's angular velocity around the origin
	float playerAngularVelocity = XMVectorGetX(XMVector3Cross(playerPositionRelative, playerVelocityRelative));

	bool isCursorOnRightHalf = cursorPos.x > width / 2;

	XMFLOAT3 nowPos = (currentPos - prevPos);

	if (!diffSet)
	{
		diff = ((-degrees + 90.0f) - playerRot.y);

		if (diff > 180.0f)
		{
			diff -= 360.0f;
		}
		else if (diff < -180.0f)
		{
			diff += 360.0f;
		}

		diffSet = true;
	}

	//Debug Start
	/*char msgbuf[256];
	sprintf_s(msgbuf, 256, "X: %d\n", testing);
	OutputDebugStringA(msgbuf);*/
	//Debug End

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}