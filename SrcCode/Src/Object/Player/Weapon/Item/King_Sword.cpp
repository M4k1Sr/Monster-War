#include "King_Sword.h"

#include "../../../Common/Collider/CapsuleCollider.h"

#include "../../../Common/Shader/DefaultShader.h"
#include "../../../Common/Shader/RimLightShader.h"
#include "../../../Common/Shader/WaterShader.h"

#include "../../../../Utility/Utility.h"

King_Sword::King_Sword(
	const Vector3& COLL_LOCAL_POS,
	const Transform& playerTrans
):
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans)
{
}

void King_Sword::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(0);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Weapon/King_Sword");

	// モデルのスケール設定
	trans.scale = 1.0f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

#pragma endregion

#pragma region コライダーの生成(maybe不要)

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::PlayerWeaponItem,
			Vector3::Yonly(30.0f) * trans.scale,
			Vector3::Yonly(-55.0f) * trans.scale,
			10.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion

}

void King_Sword::SubUpdate(void)
{	
	// プレイヤーの手の座標を取得(仮)
	trans.pos = playerTrans.pos + COLL_LOCAL_POS.TransMat(MGetRotY(playerTrans.rotation.y));

}