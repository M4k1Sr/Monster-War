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
	SetDynamicFlg(false);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(50);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Stage/Stage");

	// モデルのスケール設定
	trans.scale.x = 1.5f;
	trans.scale.z = 1.5f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -688.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

#pragma endregion

#pragma region コライダーの生成

	AddCollider(new BoxCollider(COLLIDER_TAG::Stage,Vector3(20000, 0, 20000), Vector3::Yonly(-648.0f)));

#pragma endregion

}

void Stage::SubUpdate(void)
{
}