#include "SceneFactory.h"

#include "GamePlay.h"
#include "Title.h"
#include "StageSelect.h"
#include "Result.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//　次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new Title();
	}

	else if (sceneName == "STAGESELECT")
	{
		newScene = new StageSelect();
	}

	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlay();
	}

	else if (sceneName == "RESULT")
	{
		newScene = new Result();
	}

	return newScene;
}
