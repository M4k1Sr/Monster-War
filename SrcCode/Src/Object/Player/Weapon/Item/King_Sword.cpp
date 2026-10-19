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

	// 初期ローカル座標・角度
	swordLocalPos = Vector3(9.5f, 8.5f, 10.5f);
	swordLocalRot = Vector3(Deg2Rad(31.0f), Deg2Rad(176.0f), Deg2Rad(130.0f));

	// モデルのスケール設定
	trans.scale = 1.0f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(0.0f)));

	trans.pos = MV1GetFramePosition(trans.model, 23);

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

	const float MOVE = 0.5f;
	const float ROT = Deg2Rad(1.0f);

	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		// Shift押しながら → 回転
		if (CheckHitKey(KEY_INPUT_I)) swordLocalRot.x += ROT;
		if (CheckHitKey(KEY_INPUT_K)) swordLocalRot.x -= ROT;
		if (CheckHitKey(KEY_INPUT_J)) swordLocalRot.y += ROT;
		if (CheckHitKey(KEY_INPUT_L)) swordLocalRot.y -= ROT;
		if (CheckHitKey(KEY_INPUT_U)) swordLocalRot.z += ROT;
		if (CheckHitKey(KEY_INPUT_O)) swordLocalRot.z -= ROT;
	}
	else {
		// 何も押さない → 位置
		if (CheckHitKey(KEY_INPUT_I)) swordLocalPos.x += MOVE;
		if (CheckHitKey(KEY_INPUT_K)) swordLocalPos.x -= MOVE;
		if (CheckHitKey(KEY_INPUT_J)) swordLocalPos.y += MOVE;
		if (CheckHitKey(KEY_INPUT_L)) swordLocalPos.y -= MOVE;
		if (CheckHitKey(KEY_INPUT_U)) swordLocalPos.z += MOVE;
		if (CheckHitKey(KEY_INPUT_O)) swordLocalPos.z -= MOVE;
	}

	// プレイヤーモデルの手フレームのワールド行列（アニメ反映済み）
	MATRIX handMat = MV1GetFrameLocalWorldMatrix(playerTrans.model, 23);

	// 剣のローカル変換（手に対するオフセット）
	MATRIX scaleMat = MGetScale(VGet(trans.scale.x, trans.scale.y, trans.scale.z));
	MATRIX rotMat = MGetRotX(swordLocalRot.x);     // 必要に応じてXYZ合成
	rotMat = MMult(rotMat, MGetRotY(swordLocalRot.y));
	rotMat = MMult(rotMat, MGetRotZ(swordLocalRot.z));
	MATRIX transMat = MGetTranslate(VGet(swordLocalPos.x, swordLocalPos.y, swordLocalPos.z));

	MATRIX localMat = MMult(MMult(scaleMat, rotMat), transMat);

	// 子(剣) × 親(手) の順で合成
	MATRIX mat = MMult(localMat, handMat);

	// 行列をTransformに渡す（Draw → Attach で適用される）
	trans.useMatrix = true;
	trans.matrix = mat;

	// 当たり判定用に位置を保存
	trans.pos = VTransform(VGet(0, 0, 0), mat);
}

void King_Sword::SubDraw(void)
{
	DrawFormatString(10, 100, 0xffffff, "Pos(%.1f, %.1f, %.1f)", swordLocalPos.x, swordLocalPos.y, swordLocalPos.z);
	DrawFormatString(10, 300, 0xffffff, "Rot(%.1f, %.1f, %.1f)", Rad2Deg(swordLocalRot.x), Rad2Deg(swordLocalRot.y), Rad2Deg(swordLocalRot.z));
}
