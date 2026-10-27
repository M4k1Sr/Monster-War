#include "GameScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/TimeScale/TimeScale.h"
#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../../Manager/Camera/FollowRemote/FollowRemoteCamera.h"
#include "../../Manager/Camera/FollowAction/FollowActionCamera.h"
#include "../../Manager/Camera/FollowYaw/FollowYawCamera.h"

#include "../SceneManager.h"

#include "../ActorUseDefine.h"

#include "../Common/PostEffect/CRTPostEffect/CRTPostEffect.h"
#include "../Common/PostEffect/FocusLinesPostEffect/FocusLinesPostEffect.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"
#include "../../Object/Common/DebugObject/MeshDebugObject.h"

#include "../../Object/Player/Player.h"

#include "../../Object/Enemy/Enemymanager.h"

#include "../../Object/Stage/Stage.h"

#include "../../Object/Water.h"

GameScene::GameScene() : SceneBase()
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	//AddActor(new BoxDebugObject(Vector3(20000, 0, 20000), Vector3::Yonly(-500), false));

	//AddActor(new CapsuleDebugObject(Vector3(50, 0, 0), Vector3(-50, 0, 0), 50.0f, Vector3(), true, true, true, 50, true));
	operatorObject = new Player();
	AddActor(operatorObject);

	// 敵の生成
	AddActor(new EnemyManager);
	// プレイヤー座標渡し
	ActorSerch<EnemyManager>(actors)->SetPlayerPos(&operatorObject->GetTrans().pos);

	// プレイヤーに敵管理クラスを渡す（これを追加）
	ActorSerch<Player>(actors)->SetEnemyManager(ActorSerch<EnemyManager>(actors));

	AddActor(new Stage);

	//AddActor(new Water);

	//for (int i = 0; i < 50; i++) {
	//	AddActor(new SphereDebugObject(50.0f, Vector3::Xonly(10.0f * 25 - (10.0f * i))));
	//	AddActor(new SphereDebugObject(50.0f, Vector3::Zonly(10.0f * 25 - (10.0f * i))));
	//}
}

void GameScene::SubPostInit(void)
{
	//AddPostEffect(new FocusLinesPostEffect(1.5f, 18.0f, 100.0f));
}

void GameScene::SubPostUpdate(void)
{
	// ゲーム終了処理
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

	// 決定
	if (CheckHitKey(KEY_INPUT_RSHIFT) == 1) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
	}
}

void GameScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ゲーム", 0xffffff, Font::GetIns().GetFont(FontKinds::Marumiya40));
}

void GameScene::CreateCamera(void)
{
	camera = new FollowActionCamera(&ActorSerch<Player>(actors)->GetTrans().pos);
}