#include "SceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    IScene* newScene = nullptr;

    if (sceneName == "TITLESCENE") {
        newScene = new TitleScene();
    }
    else if(sceneName == "GAMESCENE") {
        newScene = new GameScene();
    }

    return newScene;
}
