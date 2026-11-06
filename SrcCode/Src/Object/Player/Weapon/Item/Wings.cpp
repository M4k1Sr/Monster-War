#include "Wings.h"

#include "../../../Common/Collider/CapsuleCollider.h"

#include "../../../Common/Shader/DefaultShader.h"
#include "../../../Common/Shader/RimLightShader.h"
#include "../../../Common/Shader/WaterShader.h"

#include "../../../../Utility/Utility.h"

Wings::Wings(
	const Transform& playerTrans
) :
	playerTrans(playerTrans)
{
}

void Wings::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(false);

	// 押し出しだしによる重みを設定
	SetPushWeight(0);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Wings");

	// 初期ローカル座標・角度
	wingsLocalPos = Vector3();
	wingsLocalRot = Vector3(Deg2Rad(0.0f), Deg2Rad(180.0f), Deg2Rad(0.0f));

	// モデルのスケール設定
	trans.scale = 0.2f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, 0.0f, -50.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(0.0f)));

	trans.pos = MV1GetFramePosition(playerTrans.model, 4);

#pragma endregion

#pragma region コライダーの生成(maybe不要)

	//AddCollider(
	//	new CapsuleCollider(
	//		COLLIDER_TAG::PlayerWeaponItem,
	//		Vector3::Yonly(30.0f) * trans.scale,
	//		Vector3::Yonly(-55.0f) * trans.scale,
	//		10.0f * trans.scale.MaxElementF()
	//	)
	//);

#pragma endregion

}

void Wings::SubUpdate(void)
{
	// プレイヤーモデルの手フレームのワールド行列（アニメ反映済み）
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(playerTrans.model, 4);

	// 剣のローカル変換（手に対するオフセット）
	MATRIX scaleMat = MGetScale(VGet(trans.scale.x, trans.scale.y, trans.scale.z));
	MATRIX rotMat = MGetRotX(wingsLocalRot.x);     // 必要に応じてXYZ合成
	rotMat = MMult(rotMat, MGetRotY(wingsLocalRot.y));
	rotMat = MMult(rotMat, MGetRotZ(wingsLocalRot.z));
	MATRIX transMat = MGetTranslate(VGet(wingsLocalPos.x, wingsLocalPos.y, wingsLocalPos.z));

	MATRIX localMat = MMult(MMult(scaleMat, rotMat), transMat);

	// 子(剣) × 親(手) の順で合成
	MATRIX mat = MMult(localMat, handMat);

	// 行列をTransformに渡す（Draw → Attach で適用される）
	trans.useMatrix = true;
	trans.matrix = mat;

	// 当たり判定用に位置を保存
	trans.pos = VTransform(VGet(0, 0, 0), mat);
}

void Wings::SubDraw(void)
{
	DrawFormatString(10, 100, 0xffffff, "Pos(%.1f, %.1f, %.1f)", wingsLocalPos.x, wingsLocalPos.y, wingsLocalPos.z);
	DrawFormatString(10, 300, 0xffffff, "Rot(%.1f, %.1f, %.1f)", Rad2Deg(wingsLocalRot.x), Rad2Deg(wingsLocalRot.y), Rad2Deg(wingsLocalRot.z));
}
