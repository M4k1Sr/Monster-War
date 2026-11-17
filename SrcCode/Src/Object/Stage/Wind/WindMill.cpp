#include "WindMill.h"

#include "../../Utility/Utility.h"

#include "../Common/Collider/BoxCollider.h"

#include "../Common/PhysicsBody/PhysicsBody.h"
#include "../Common/MotionController/GroundMotionController.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

WindMill::WindMill(const WindMillParam& param)
{
	originModel = -1;
	trans.pos = param.pos;
	modelCount = 0;
}

void WindMill::Load(void)
{         
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetColliderMobility(COLLIDER_MOBILITY::Static);

	//// 物理挙動の有効化
	//EnablePhysics();

	//// 重力を有効にする
	//GetPhysicsBody()->SetGravityEnabled(true);

	//// キャラクター用の物理挙動に設定する
	//GetPhysicsBody()->SetMode(PhysicsBody::MotionMode::Kinematic);

	// 当たり判定による押し出しを有効にする
	SetContactResponseEnabled(true);

	//// 物理衝突時の重さを設定
	//GetPhysicsBody()->SetMass(1.0f);

	//// 着地時に跳ね返らないようにする
	//GetPhysicsBody()->SetRestitution(0.0f);

#pragma endregion


#pragma region モデル設定
	
	if (originModel == -1) {
		// もととなるモデルの読み込み
		trans.LoadModel("Stage/WindMill");
		originModel = trans.model;
	}

	// モデル台数
	modelCount++;

	// モデルの読み込み
	trans.Duplicate(originModel);
	
	// モデルのスケール設定
	trans.scale.x = 0.1f;
	trans.scale.y = 0.5f;
	trans.scale.z = 0.1f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -100.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(0.0f)));

#pragma endregion

#pragma region コライダーの生成

	AddCollider(new BoxCollider(
		COLLIDER_TAG::WindMill, 
		Vector3(450.0f,50.0f,450.0f))
	);

#pragma endregion

}

void WindMill::SubUpdate(void)
{
	spinAngle += 0.15f * GetTimeScale();
	if (spinAngle > DX_TWO_PI_F) { spinAngle -= DX_TWO_PI_F;}

	const Quaternion spin = Quaternion::FromEulerXZY(Vector3::Yonly(spinAngle));
	trans.SetLocalRotation(spin * baseQuaternion);
	trans.Attach(); 
}
void WindMill::SubRelease(void)
{
	modelCount--;                       // 使う人が1人減った
	if (modelCount <= 0 && originModel != -1) {
		MV1DeleteModel(originModel);  // 誰も使ってないから消す
		originModel = -1;             // 次のシーンでまた読み込めるように戻す
		modelCount = 0;
	}
}
