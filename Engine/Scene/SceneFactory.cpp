#include "SceneFactory.h"
#include "GameScene.h"

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    IScene* newScene = nullptr;

    if (sceneName == "GAMESCENE") {
        newScene = new GameScene();
    }
    else {

    }

    return newScene;
}
