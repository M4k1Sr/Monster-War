#include "Stage.h"

#include "../../Utility/Utility.h"

#include "../Common/Collider/BoxCollider.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

Stage::Stage()
{
}

void Stage::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(50);

	// シェーダー登録
	CreateShader(new DefaultShader());

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Stage/Stage");

	// モデルのスケール設定
	trans.scale = 10;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	// シェーダー登録
	CreateShader(new DefaultShader());

#pragma endregion

#pragma region コライダーの生成

	//AddCollider(
	//	new BoxCollider(
	//		COLLIDER_TAG::Stage,
	//		Vector3(0.0f, 0.0f,0.0f),
	//		500.0f * trans.scale.MaxElementF()
	//	)
	//);

#pragma endregion

}

void Stage::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Stage::SubUpdate(void)
{
	static char type = 0;
	static bool prev = false, now = false;

	prev = now;
	now = CheckHitKey(KEY_INPUT_SPACE) == 1;

	if (!prev && now) {

		if (++type > 1) { type = 0; }

		switch (type) {
		case 0: { CreateShader(new DefaultShader()); break; }
		case 1: { CreateShader(new RimLightShader()); break; }
		}
	}

}
