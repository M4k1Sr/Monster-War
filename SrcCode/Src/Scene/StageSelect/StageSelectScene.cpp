#include "StageSelectScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../SceneManager.h"

StageSelectScene::StageSelectScene() : SceneBase()
{
}

void StageSelectScene::SubPostLoad(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("StageSelect");
}

void StageSelectScene::SubPostUpdate(void)
{
	// ゲーム終了処理
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		App::GetIns().GameEnd();
	}

	// 決定
	if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);
	}
}

void StageSelectScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ステージセレクト", 0xffffff, Font::GetIns().GetFont(FontKinds::Marumiya40));
}