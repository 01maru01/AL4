#include "SceneManager.h"
#include "Easing.h"

const int SceneManager::SCENE_CHANGE_TIME = 60;

SceneManager::~SceneManager()
{
	scene->Finalize();
	delete scene;
	imguiMan->Finalize();
	delete imguiMan;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* instance = new SceneManager;
	return instance;
}

void SceneManager::DeleteInstance()
{
	delete SceneManager::GetInstance();
}

void SceneManager::Initialize()
{
	scene = sceneFactry->CreateScene("GAMESCENE");
	scene->Initialize();
	endLoading = true;

	loadTex = dx->LoadTextureGraph(L"Resources/loading.png");
	loadSprite = std::make_unique<Sprite>(loadTex);
	loadSprite->SetPosition(Vector2D{ Window::window_width - 96,Window::window_height - 98 });
	loadSprite->SetAnchorPoint(Vector2D{ 0.5,0.5 });
	loadSprite->SetSize(Vector2D{ 64,64 });
	loadSprite->TransferVertex();

	imguiMan = new ImGuiManager();
	imguiMan->Initialize();
}

void SceneManager::Update()
{
	if (endLoading) {
		if (sceneInitialized) {

			//	フェードイン
			if (sceneChangeTimer > 0) {
				sceneChangeTimer--;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				screen.SetColor(screenColor);

				scene->MatUpdate();
			}
			else {
				//	同期処理
				scene->Update();
			}
		}
		else {
			//	nextSceneがセットされたら
			//	フェードアウト
			if (sceneChangeTimer < SCENE_CHANGE_TIME) {
				sceneChangeTimer++;

				float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)SCENE_CHANGE_TIME);
				screenColor.x = color;
				screenColor.y = color;
				screenColor.z = color;

				//	色設定
				screen.SetColor(screenColor);
			}

			if (sceneChangeTimer >= SCENE_CHANGE_TIME) {
				//	フェードアウト済みロード画面へ
				endLoading = false;
				//	非同期
				sceneInitInfo = std::async(std::launch::async, [this] {return SceneChange(); });
				sceneChangeTimer = SCENE_CHANGE_TIME;
			}
		}
	}
	else {
		std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready) {
			//	ロード終わりフラグ
			endLoading = true;
		}
		//	ロード画面
		float rot = loadSprite->GetRotation();
		loadSprite->SetRotation(rot + 0.1f);
		loadSprite->MatUpdate();
	}

	imguiMan->Begin();
	imguiMan->End();
}

void SceneManager::Draw()
{
#pragma region DrawScreen
	dx->PrevDrawScreen();

	if (endLoading) {
		scene->Draw();
	}

	dx->PostDrawScreen();
#pragma endregion

#pragma region MultiPath
	dx->PrevDraw();

	screen.Draw();
	if (!endLoading) {
		//	ロード画面
		loadSprite->Draw();
	}

	imguiMan->Draw();
	dx->PostDraw();
#pragma endregion
}

void SceneManager::SceneChange()
{
	if (nextScene) {
		if (scene) {
			scene->Finalize();
			delete scene;
		}

		scene = nextScene;
		scene->Initialize();
		sceneInitialized = true;
		nextScene = nullptr;
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene = sceneFactry->CreateScene(sceneName);
	if (nextScene) sceneInitialized = false;
}
