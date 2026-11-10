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
	SetPushFlg(false);

	// 押し出しだしによる重みを設定
	SetPushWeight(0);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Weapon/King_Sword");

	// 初期ローカル座標・角度
	swordLocalPos = Vector3(0.0f,-11.0f,0.0f);
	swordLocalRot = Vector3(Deg2Rad(0.0f), Deg2Rad(-90.0f), Deg2Rad(0.0f));

	// モデルのスケール設定
	trans.scale = 0.7f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(0.0f)));

	trans.pos = MV1GetFramePosition(trans.model, 21);

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
#ifdef _DEBUG
	//// 握り位置の調整用（調整が終わったら無効化してOK）
	//const float MOVE = 0.5f;
	//const float ROT = Deg2Rad(1.0f);

	//if (CheckHitKey(KEY_INPUT_LSHIFT)) {
	//	// Shift押しながら → 回転
	//	if (CheckHitKey(KEY_INPUT_I)) swordLocalRot.x += ROT;
	//	if (CheckHitKey(KEY_INPUT_K)) swordLocalRot.x -= ROT;
	//	if (CheckHitKey(KEY_INPUT_J)) swordLocalRot.y += ROT;
	//	if (CheckHitKey(KEY_INPUT_L)) swordLocalRot.y -= ROT;
	//	if (CheckHitKey(KEY_INPUT_U)) swordLocalRot.z += ROT;
	//	if (CheckHitKey(KEY_INPUT_O)) swordLocalRot.z -= ROT;
	//}
	//else {
	//	// 何も押さない → 位置
	//	if (CheckHitKey(KEY_INPUT_I)) swordLocalPos.x += MOVE;
	//	if (CheckHitKey(KEY_INPUT_K)) swordLocalPos.x -= MOVE;
	//	if (CheckHitKey(KEY_INPUT_J)) swordLocalPos.y += MOVE;
	//	if (CheckHitKey(KEY_INPUT_L)) swordLocalPos.y -= MOVE;
	//	if (CheckHitKey(KEY_INPUT_U)) swordLocalPos.z += MOVE;
	//	if (CheckHitKey(KEY_INPUT_O)) swordLocalPos.z -= MOVE;
	//}
#endif

	// 最新の位置、向きなどをセット
	MV1SetMatrix(playerTrans.model, playerTrans.GetWorldMatrix());

	// 手のワールド行列を取得
	const MATRIX handMat = MV1GetFrameLocalWorldMatrix(playerTrans.model, 21);

	// パラセールのローカル行列
	MATRIX localMat = MGetScale(trans.scale.ToVECTOR());
	localMat = MMult(localMat, MatrixAllMultXYZ({ swordLocalRot }));
	localMat = MMult(localMat, MGetTranslate(swordLocalPos.ToVECTOR()));

	// 手とパラセールの行列を合成
	const MATRIX mat = MMult(localMat, handMat);

	// 描画用
	trans.useMatrix = true;
	trans.matrix = mat;
	trans.pos = Vector3(mat.m[3][0], mat.m[3][1], mat.m[3][2]);

	// 向き
	MATRIX rotOnly = MGetIdent();
	for (int i = 0; i < 3; i++) {
		const Vector3 row = Vector3(mat.m[i][0], mat.m[i][1], mat.m[i][2]).Normalized();
		rotOnly.m[i][0] = row.x;
		rotOnly.m[i][1] = row.y;
		rotOnly.m[i][2] = row.z;
	}
	trans.rotation = Quaternion::FromMatrix(rotOnly);	trans.pos = VTransform(VGet(0, 0, 0), mat);
}

void King_Sword::SubDraw(void)
{
	//DrawFormatString(300, 330, 0xffffff, "Pos(%.1f, %.1f, %.1f)", swordLocalPos.x, swordLocalPos.y, swordLocalPos.z);
	//DrawFormatString(300, 300, 0xffffff, "Rot(%.1f, %.1f, %.1f)", Rad2Deg(swordLocalRot.x), Rad2Deg(swordLocalRot.y), Rad2Deg(swordLocalRot.z));
}
