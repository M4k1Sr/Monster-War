#include "Paraseil.h"

#include "../../../Common/Collider/CapsuleCollider.h"

#include "../../../../Utility/Utility.h"

Paraseil::Paraseil(const Transform& playerTrans) 
	:
	playerTrans(playerTrans)
{
}

void Paraseil::Load(void)
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

	// 描画しない
	SetIsDraw(false);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Paraseil");

	// 初期ローカル座標・角度
	paraseilLocalPos = Vector3(5.0f,7.5f,-14.5f);
	paraseilLocalRot = Vector3(Deg2Rad(-13.0f), Deg2Rad(37.0f), Deg2Rad(5.0f));

	// モデルのスケール設定
	trans.scale = 1.0f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, 0.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(0.0f)));

	trans.pos = MV1GetFramePosition(playerTrans.model, 21);

#pragma endregion

#pragma region コライダーの生成(maybe不要)
#pragma endregion

}

void Paraseil::SubUpdate(void)
{
	// 剣と一緒
	// 最新の位置、向きなどをセット
	MV1SetMatrix(playerTrans.model, playerTrans.GetWorldMatrix());

	// 手のワールド行列を取得
	const MATRIX handMat = MV1GetFrameLocalWorldMatrix(playerTrans.model, 21);

	// パラセールのローカル行列
	MATRIX localMat = MGetScale(trans.scale.ToVECTOR());
	localMat = MMult(localMat, MatrixAllMultXYZ({ paraseilLocalRot }));
	localMat = MMult(localMat, MGetTranslate(paraseilLocalPos.ToVECTOR()));

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
	trans.rotation = Quaternion::FromMatrix(rotOnly);
}

void Paraseil::SubDraw(void)
{
	DrawFormatString(10, 100, 0xffffff, "Pos(%.1f, %.1f, %.1f)", paraseilLocalPos.x, paraseilLocalPos.y, paraseilLocalPos.z);
	DrawFormatString(10, 300, 0xffffff, "Rot(%.1f, %.1f, %.1f)", Rad2Deg(paraseilLocalRot.x), Rad2Deg(paraseilLocalRot.y), Rad2Deg(paraseilLocalRot.z));
}
