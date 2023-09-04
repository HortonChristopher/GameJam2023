#include "SceneFactory.h"

#include "GamePlay.h"
#include "Title.h"
#include "StageSelect.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//Å@éüÇÃÉVÅ[ÉìÇê∂ê¨
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

	return newScene;
}
