#include "Title.h"

extern bool run;

Title::Title()
{
}

Title::~Title()
{
}

bool Title::IsTooClose(const XMFLOAT3& point1, const XMFLOAT3& point2, float minDistance) {
	float dx = point1.x - point2.x;
	float dz = point1.z - point2.z;
	return dx * dx + dz * dz < minDistance * minDistance;
}

DirectX::XMFLOAT3 Title::GenerateRandomCoordinate(float innerRadius, float outerRadius, float angleMin, float angleMax, const std::vector<XMFLOAT3>& existingPoints, float minDistance) {
	XMFLOAT3 coordinate;
	bool valid;
	do {
		valid = true;

		// Generate a random angle between angleMin and angleMax
		float angle = angleMin + (float(rand()) / RAND_MAX) * (angleMax - angleMin);

		// Generate a random radius between innerRadius and outerRadius
		float radius = innerRadius + (float(rand()) / RAND_MAX) * (outerRadius - innerRadius);

		// Convert polar coordinates (angle & radius) to Cartesian coordinates
		coordinate.x = radius * cos(angle);
		coordinate.y = 0.0f;
		coordinate.z = radius * sin(angle);

		for (const XMFLOAT3& existingPoint : existingPoints) {
			if (IsTooClose(coordinate, existingPoint, minDistance)) {
				valid = false;
				break;
			}
		}
	} while (!valid);

	return coordinate;
}

DirectX::XMFLOAT3 Title::CalculateRotation(const XMFLOAT3& coordinate) {
	float yaw = atan2(-coordinate.z, -coordinate.x);  // Note the '-' to make it face towards (0, 0, 0)
	return XMFLOAT3(0.0f, yaw, 0.0f);
}

std::pair<std::vector<DirectX::XMFLOAT3>, std::vector<DirectX::XMFLOAT3>> Title::GenerateCoordinatesAndRotations() {
	// Seed the random number generator
	std::srand(std::time(0));

	std::vector<XMFLOAT3> allCoordinates;
	std::vector<XMFLOAT3> allRotations;

	// Generate 20 coordinates for each of the 4 quadrants
	for (int quadrant = 0; quadrant < 4; ++quadrant) {
		float angleMin = float(quadrant) * 0.5f * 3.14159265358979323846f;
		float angleMax = float(quadrant + 1) * 0.5f * 3.14159265358979323846f;

		float rotationValue = 0.0f;

		// Manually set the rotation based on the quadrant
		switch (quadrant) {
		case 0: rotationValue = 0.0f; break; // North-East
		case 1: rotationValue = 4.71238898038f; break; // South-East (90 degrees in radians)
		case 2: rotationValue = 3.14159265359f; break; // South-West (180 degrees in radians)
		case 3: rotationValue = 1.57079632679f; break; // North-West (270 degrees in radians)
		}

		for (int i = 0; i < 20; ++i) {
			XMFLOAT3 coordinate = GenerateRandomCoordinate(300.0f, 450.0f, angleMin, angleMax, allCoordinates, 10.0f);
			allCoordinates.push_back(coordinate);

			// Set rotation manually
			XMFLOAT3 rotation = XMFLOAT3(0.0f, rotationValue, 0.0f);

			allRotations.push_back(rotation);
		}
	}

	return { allCoordinates, allRotations };
}

void Title::Initialize()
{
	// Seed the random number generator
	std::srand(std::time(0));

	// サウンド初期化
	sound->Initialize();

	//音声のロード
	sound->LoadWav("SE/Title/Crash.wav");
	sound->LoadWav("SE/Title/Push.wav");
	sound->LoadWav("SE/Title/BGM.wav");


	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);
	ObjectParticle::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::title_bg, L"Resources/Sprite/TitleUI/Title.jpg")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::titlefont, L"Resources/Sprite/TitleUI/TitleFont.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pressspace, L"Resources/Sprite/TitleUI/PressSpace.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	titleBG = Sprite::Create(TextureNumber::title_bg, { 0.0f,0.0f });

	pigGate = ObjObject::Create();
	pigGateModel = ObjModel::CreateFromOBJ("butagate");
	pigGate->SetModel(pigGateModel);
	pigGate->SetPosition({ -155.0f, 0.0f, 0.0f });
	pigGate->SetRotation({ 0.0f, 270.0f, 0.0f });
	pigGate->SetScale({ 3.7f, 3.7f, 3.7f });

	sheepGate = ObjObject::Create();
	sheepGateModel = ObjModel::CreateFromOBJ("hutsuzigate");
	sheepGate->SetModel(sheepGateModel);
	sheepGate->SetPosition({ 0.0f, 0.0f, -155.0f });
	sheepGate->SetRotation({ 0.0f, 180.0f, 0.0f });
	sheepGate->SetScale({ 3.7f, 3.7f, 3.7f });

	cowGate = ObjObject::Create();
	cowGateModel = ObjModel::CreateFromOBJ("ushigate");
	cowGate->SetModel(cowGateModel);
	cowGate->SetPosition({ 155.0f, 0.0f, 0.0f });
	cowGate->SetRotation({ 0.0f, 90.0f, 0.0f });
	cowGate->SetScale({ 3.7f, 3.7f, 3.7f });

	pigSign = ObjObject::Create();
	pigSignModel = ObjModel::CreateFromOBJ("butakanban");
	pigSign->SetModel(pigSignModel);
	pigSign->SetPosition({ -152.5f, 0.0f, -40.0f });
	pigSign->SetRotation({ 0.0f, 270.0f, 0.0f });

	sheepSign = ObjObject::Create();
	sheepSignModel = ObjModel::CreateFromOBJ("hitsuzikanban");
	sheepSign->SetModel(sheepSignModel);
	sheepSign->SetPosition({ 40.0f, 0.0f, -152.5f });
	sheepSign->SetRotation({ 0.0f, 180.0f, 0.0f });

	cowSign = ObjObject::Create();
	cowSignModel = ObjModel::CreateFromOBJ("ushikanban");
	cowSign->SetModel(cowSignModel);
	cowSign->SetPosition({ 152.5f, 0.0f, 40.0f });
	cowSign->SetRotation({ 0.0f, 90.0f, 0.0f });

	// 3D OBJ
	ground = ObjObject::Create();
	modelGround = ObjModel::CreateFromOBJ("grand");
	ground->SetModel(modelGround);

	// Silo
	siloObject = ObjObject::Create();
	siloModel = ObjModel::CreateFromOBJ("RanchSilo");
	siloObject->SetModel(siloModel);
	siloObject->SetPosition({ 60.0f, 0.0f, 225.0f });
	siloObject->SetRotation({ 0.0f, 0.0f, 0.0f });
	siloObject->SetScale({ 6.0f, 6.0f, 6.0f });

	// Tree
	treeModel = ObjModel::CreateFromOBJ("RanchTree");
	for (int i = 0; i < 20; i++)
	{
		trees[i] = ObjObject::Create();
		trees[i]->SetModel(treeModel);
		switch (i)
		{
		case 0:
			trees[i]->SetPosition({ -130.0f, 0.0f, 180.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 1:
			trees[i]->SetPosition({ -180.0f, 0.0f, 130.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 2:
			trees[i]->SetPosition({ -190.0f, 0.0f, 190.0f });
			trees[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			break;
		case 3:
			trees[i]->SetPosition({ -130.0f, 0.0f, -180.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 4:
			trees[i]->SetPosition({ -180.0f, 0.0f, -130.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 5:
			trees[i]->SetPosition({ -190.0f, 0.0f, -190.0f });
			trees[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			break;
		case 6:
			trees[i]->SetPosition({ 130.0f, 0.0f, -180.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 7:
			trees[i]->SetPosition({ 180.0f, 0.0f, -130.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 8:
			trees[i]->SetPosition({ 190.0f, 0.0f, -190.0f });
			trees[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			break;
		case 9:
			trees[i]->SetPosition({ 130.0f, 0.0f, 180.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 10:
			trees[i]->SetPosition({ 180.0f, 0.0f, 130.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 11:
			trees[i]->SetPosition({ 190.0f, 0.0f, 190.0f });
			trees[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			break;
		case 12:
			trees[i]->SetPosition({ -100.0f, 0.0f, 200.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 13:
			trees[i]->SetPosition({ -200.0f, 0.0f, 100.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 14:
			trees[i]->SetPosition({ -100.0f, 0.0f, -200.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 15:
			trees[i]->SetPosition({ -200.0f, 0.0f, -100.0f });
			trees[i]->SetRotation({ 0.0f, 225.0f, 0.0f });
			break;
		case 16:
			trees[i]->SetPosition({ 100.0f, 0.0f, -200.0f });
			trees[i]->SetRotation({ 0.0f, 135.0f, 0.0f });
			break;
		case 17:
			trees[i]->SetPosition({ 200.0f, 0.0f, -100.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		case 18:
			trees[i]->SetPosition({ 100.0f, 0.0f, 200.0f });
			trees[i]->SetRotation({ 0.0f, 315.0f, 0.0f });
			break;
		case 19:
			trees[i]->SetPosition({ 200.0f, 0.0f, 100.0f });
			trees[i]->SetRotation({ 0.0f, 45.0f, 0.0f });
			break;
		}
		trees[i]->SetScale({ 2.5f, 2.5f, 2.5f });
	}

	// Generate coordinates and rotations
	std::pair<std::vector<XMFLOAT3>, std::vector<XMFLOAT3>> data = GenerateCoordinatesAndRotations();

	// Extract the vectors from the pair
	std::vector<XMFLOAT3>& coordinates = data.first;
	std::vector<XMFLOAT3>& rotations = data.second;

	// Assume object is some array or vector of pointers to your objects
	for (size_t i = 0; i < coordinates.size(); ++i) {
		randomTrees[i] = ObjObject::Create();
		randomTrees[i]->SetModel(treeModel);
		randomTrees[i]->SetPosition(coordinates[i]);

		float radiansToDegrees = 180.0f / 3.14159265358979323846f;
		XMFLOAT3 convertedRotation = { 0.0f, rotations[i].y * radiansToDegrees, 0.0f };

		randomTrees[i]->SetRotation(convertedRotation);
		randomTrees[i]->SetScale({ 2.5f, 2.5f, 2.5f });
	}

	// Spawn barn
	barn = ObjObject::Create();
	modelBarn = ObjModel::CreateFromOBJ("RanchHut");
	barn->SetModel(modelBarn);
	barn->SetPosition({ 0.0f, 0.0f, 255.0f });
	barn->SetRotation({ 0.0f, 0.0f, 0.0f });
	barn->SetScale({ 6.0f, 6.0f, 6.0f });

	// Fences
	FenceCreation();

	skydome = ObjObject::Create();
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	skydome->SetModel(modelSkydome);
	skydome->SetScale({ 8.0f, 8.0f, 8.0f });

	ground->SetPosition({ 0.0f, -0.5f, 0.0f });
	ground->SetRotation({ 0.0f, 0.0f, 0.0f });
	ground->SetScale({ 80.0f, 1.0f, 80.0f });

	modelPig = ObjModel::CreateFromOBJ("buta");
	modelSheep = ObjModel::CreateFromOBJ("hitsuzi");
	modelHorse = ObjModel::CreateFromOBJ("ushi");

	// 座標のセット
	camera->title = true;
	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetEye({ 0, 2, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(224.0f);

	for (int i = 0; i < 3; i++)
	{
		std::unique_ptr<Buta> newButa = Buta::Create(modelPig, { -130.0f + (130.0f * i), 0.5f, 130.0f }, { 1.0f, 1.0f, 1.0f }, false);
		butaList.push_back(std::move(newButa));
		std::unique_ptr<Hitsuji> newHitsuji = Hitsuji::Create(modelSheep, { -130.0f + (130.0f * i), 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, false);
		hitsujiList.push_back(std::move(newHitsuji));
		std::unique_ptr<Ushi> newUshi = Ushi::Create(modelHorse, { -130.0f + (130.0f * i), 0.5f, -130.0f }, { 1.0f, 1.0f, 1.0f }, false);
		ushiList.push_back(std::move(newUshi));
	}

	ShowCursor(false);
}

void Title::Finalize()
{
	safe_delete(titleBG);
}

void Title::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("STAGESELECT");
	}

	ground->Update();
	skydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	skydome->Update();
	camera->Update();
	camera->SetEye({ camera->GetEye().x, camera->GetEye().y + 10.0f, camera->GetEye().z });

	// Spawn Barn
	barn->Update();
	siloObject->Update();

	for (int i = 0; i < 20; i++)
	{
		trees[i]->Update();
	}

	for (int i = 0; i < 80; i++)
	{
		randomTrees[i]->Update();
	}

	// Fences
	UpdateFences();

	pigSign->Update();
	sheepSign->Update();
	cowSign->Update();

	pigGate->Update();
	sheepGate->Update();
	cowGate->Update();

	for (std::unique_ptr<Buta>& buta : butaList)
	{
		buta->Update();
	}

	for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
	{
		hitsuji->Update();
	}

	for (std::unique_ptr<Ushi>& ushi : ushiList)
	{
		ushi->Update();
	}

	if (input->TriggerKey(DIK_ESCAPE))
	{
		run = false;
	}
}

void Title::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//titleBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);
	ObjectParticle::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	ground->Draw();
	skydome->Draw();
	//player->Draw();

	// Spawn Barn
	barn->Draw();
	siloObject->Draw();

	for (int i = 0; i < 20; i++)
	{
		trees[i]->Draw();
	}

	for (int i = 0; i < 80; i++)
	{
		randomTrees[i]->Draw();
	}

	// Fences
	DrawFences();

	pigGate->Draw();
	sheepGate->Draw();
	cowGate->Draw();

	pigSign->Draw();
	sheepSign->Draw();
	cowSign->Draw();
	
	for (std::unique_ptr<Buta>& buta : butaList)
	{
		buta->Draw();
	}

	for (std::unique_ptr<Hitsuji>& hitsuji : hitsujiList)
	{
		hitsuji->Draw();
	}

	for (std::unique_ptr<Ushi>& ushi : ushiList)
	{
		ushi->Draw();
	}

	// 3Dオブジェクト描画後処理
	ObjectParticle::PostDraw();
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

}

void Title::FenceCreation()
{
	modelFence = ObjModel::CreateFromOBJ("fence");

	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			// West side North
			westSideNorthFence[i] = ObjObject::Create();
			westSideNorthFence[i]->SetModel(modelFence);
			westSideNorthFence[i]->SetPosition({ -155.0f, -1.0f, 145.0f - (20.0f * i) });
			westSideNorthFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			westSideNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// West side South
			westSideSouthFence[i] = ObjObject::Create();
			westSideSouthFence[i]->SetModel(modelFence);
			westSideSouthFence[i]->SetPosition({ -155.0f, -1.0f, -145.0f + (20.0f * i) });
			westSideSouthFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			westSideSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South side West
			southSideWestFence[i] = ObjObject::Create();
			southSideWestFence[i]->SetModel(modelFence);
			southSideWestFence[i]->SetPosition({ -145.0f + (20.0f * i), -1.0f, -155.0f });
			southSideWestFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			southSideWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South side East
			southSideEastFence[i] = ObjObject::Create();
			southSideEastFence[i]->SetModel(modelFence);
			southSideEastFence[i]->SetPosition({ 145.0f - (20.0f * i), -1.0f, -155.0f });
			southSideEastFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			southSideEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East side North
			eastSideNorthFence[i] = ObjObject::Create();
			eastSideNorthFence[i]->SetModel(modelFence);
			eastSideNorthFence[i]->SetPosition({ 155.0f, -1.0f, 145.0f - (20.0f * i) });
			eastSideNorthFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			eastSideNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East side South
			eastSideSouthFence[i] = ObjObject::Create();
			eastSideSouthFence[i]->SetModel(modelFence);
			eastSideSouthFence[i]->SetPosition({ 155.0f, -1.0f, -145.0f + (20.0f * i) });
			eastSideSouthFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			eastSideSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North side West
			northSideWestFence[i] = ObjObject::Create();
			northSideWestFence[i]->SetModel(modelFence);
			northSideWestFence[i]->SetPosition({ -145.0f + (20.0f * i), -1.0f, 155.0f });
			northSideWestFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			northSideWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North side East
			northSideEastFence[i] = ObjObject::Create();
			northSideEastFence[i]->SetModel(modelFence);
			northSideEastFence[i]->SetPosition({ 145.0f - (20.0f * i), -1.0f, 155.0f });
			northSideEastFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			northSideEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}

		if (i < 3)
		{
			// West goal
			// South
			westGoalSouthFence[i] = ObjObject::Create();
			westGoalSouthFence[i]->SetModel(modelFence);
			westGoalSouthFence[i]->SetPosition({ -166.0f - (20.0f * i), -1.0f, -70.0f });
			westGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			westGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North
			westGoalNorthFence[i] = ObjObject::Create();
			westGoalNorthFence[i]->SetModel(modelFence);
			westGoalNorthFence[i]->SetPosition({ -166.0f - (20.0f * i), -1.0f, 70.0f });
			westGoalNorthFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			westGoalNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// South Goal
			// West
			southGoalWestFence[i] = ObjObject::Create();
			southGoalWestFence[i]->SetModel(modelFence);
			southGoalWestFence[i]->SetPosition({ -70.0f, -1.0f, -166.0f - (20.0f * i) });
			southGoalWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			southGoalWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East
			southGoalEastFence[i] = ObjObject::Create();
			southGoalEastFence[i]->SetModel(modelFence);
			southGoalEastFence[i]->SetPosition({ 70.0f, -1.0f, -166.0f - (20.0f * i) });
			southGoalEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			southGoalEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East Goal
			// South
			eastGoalSouthFence[i] = ObjObject::Create();
			eastGoalSouthFence[i]->SetModel(modelFence);
			eastGoalSouthFence[i]->SetPosition({ 166.0f + (20.0f * i), -1.0f, -70.0f });
			eastGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
			eastGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// North
			eastGoalNorthFence[i] = ObjObject::Create();
			eastGoalNorthFence[i]->SetModel(modelFence);
			eastGoalNorthFence[i]->SetPosition({ 166.0f + (20.0f * i), -1.0f, 70.0f });
			eastGoalNorthFence[i]->SetRotation({ 0.0f, 0.0f, 0.0f });
			eastGoalNorthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}
		if (i < 2)
		{
			// Barn
			// West
			barnWestFence[i] = ObjObject::Create();
			barnWestFence[i]->SetModel(modelFence);
			barnWestFence[i]->SetPosition({ -35.0f, -1.0f, 165.0f + (20.0f * i) });
			barnWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
			barnWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

			// East
			barnEastFence[i] = ObjObject::Create();
			barnEastFence[i]->SetModel(modelFence);
			barnEastFence[i]->SetPosition({ 35.0f, -1.0f, 165.0f + (20.0f * i) });
			barnEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
			barnEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
		}

		// West goal West
		westGoalWestFence[i] = ObjObject::Create();
		westGoalWestFence[i]->SetModel(modelFence);
		westGoalWestFence[i]->SetPosition({ -216.0f, -1.0f, -60.0f + (20.0f * i) });
		westGoalWestFence[i]->SetRotation({ 0.0f, 270.0f, 0.0f });
		westGoalWestFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

		// South goal South
		southGoalSouthFence[i] = ObjObject::Create();
		southGoalSouthFence[i]->SetModel(modelFence);
		southGoalSouthFence[i]->SetPosition({ -60.0f + (20.0f * i), -1.0f, -216.0f });
		southGoalSouthFence[i]->SetRotation({ 0.0f, 180.0f, 0.0f });
		southGoalSouthFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });

		// East goal East
		eastGoalEastFence[i] = ObjObject::Create();
		eastGoalEastFence[i]->SetModel(modelFence);
		eastGoalEastFence[i]->SetPosition({ 216.0f, -1.0f, -60.0f + (20.0f * i) });
		eastGoalEastFence[i]->SetRotation({ 0.0f, 90.0f, 0.0f });
		eastGoalEastFence[i]->SetScale({ 1.0f, 1.0f, 1.0f });
	}
}

void Title::UpdateFences()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			westSideNorthFence[i]->Update();
			westSideSouthFence[i]->Update();
			southSideWestFence[i]->Update();
			southSideEastFence[i]->Update();
			eastSideNorthFence[i]->Update();
			eastSideSouthFence[i]->Update();
			northSideWestFence[i]->Update();
			northSideEastFence[i]->Update();
		}
		if (i < 3)
		{
			westGoalNorthFence[i]->Update();
			westGoalSouthFence[i]->Update();
			southGoalEastFence[i]->Update();
			southGoalWestFence[i]->Update();
			eastGoalNorthFence[i]->Update();
			eastGoalSouthFence[i]->Update();
		}
		if (i < 2)
		{
			barnWestFence[i]->Update();
			barnEastFence[i]->Update();
		}
		westGoalWestFence[i]->Update();
		southGoalSouthFence[i]->Update();
		eastGoalEastFence[i]->Update();
	}
}

void Title::DrawFences()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 6)
		{
			westSideNorthFence[i]->Draw();
			westSideSouthFence[i]->Draw();
			southSideWestFence[i]->Draw();
			southSideEastFence[i]->Draw();
			eastSideNorthFence[i]->Draw();
			eastSideSouthFence[i]->Draw();
			northSideEastFence[i]->Draw();
			northSideWestFence[i]->Draw();
		}
		if (i < 3)
		{
			westGoalNorthFence[i]->Draw();
			westGoalSouthFence[i]->Draw();
			southGoalWestFence[i]->Draw();
			southGoalEastFence[i]->Draw();
			eastGoalSouthFence[i]->Draw();
			eastGoalNorthFence[i]->Draw();
		}
		if (i < 2)
		{
			barnWestFence[i]->Draw();
			barnEastFence[i]->Draw();
		}
		westGoalWestFence[i]->Draw();
		southGoalSouthFence[i]->Draw();
		eastGoalEastFence[i]->Draw();
	}
}