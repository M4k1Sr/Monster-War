#pragma once

#include "MotionControllerBase.h"

#include "../Transform/Transform.h"

#include "../../../Utility/Utility.h"

// キャラクター移動(空中)制御
class ParaseilMotionController : public MotionControllerBase
{
public:

#pragma region 移動パラメータ

	// 通常移動の最大速度を設定
	void SetMoveSpeed(float value) { moveSpeed = value; }

    // 空中の加速度を設定
    void SetAirAcceleration(float value) { airAcceleration = value; }

    // 高速移動中の方向修正倍率を設定
    void SetMomentumSteering(float value) { momentumSteering = value; }

    // 高速移動と判定する速度を設定
    void SetMomentumThreshold(float value) { momentumThreshold = value; }

    // 空中の水平減速度（速度/フレーム）
    void SetAirDrag(float value) { airDrag = value > 0.0f ? value : 0.0f; }

    // 滑空中の重力
    void SetGlideGravity(float value) { glideGravity = value > 0.0f ? value : 0.0f; }

    // 落下速度の上限
    void SetMaxFallSpeed(float value) { maxFallSpeed = value > 0.0f ? value : 0.0f; }
   
    // 体の向きを変える速さ
    void SetTurnRate(float value) { turnRate = value > 0.0f ? value : 0.0f; }

#pragma endregion


#pragma region 移動制御

    // 入力状態に関係なく、毎フレーム水平速度を減衰させる。
    void Update(float timeScale) override;

    // 移動入力に応じて水平方向の速度を更新
    void ApplyInput(const Vector3& inputDirection) override;

    // 水平方向(XZ)の速度を直接指定する。Y方向の速度は変更しない。
    void SetHorizontalVelocity(const Vector3& horizontal);

    // 上下方向(Y)の速度を直接指定する。XZ方向の速度は変更しない。
    void SetVerticalVelocity(float y);

    // 上下方向(Y)の速度を取得する
    float GetVerticalVelocity(void) const { return physicsBody ? physicsBody->GetVelocity().y : 0.0f; }

    // 指定方向へキャラクターの向きを補間する（水平方向のみ）
    void FaceDirection(const Vector3& direction);

    // 水平方向の速度を、指定の減速度（速度/フレーム）で減らす。空中での弓矢攻撃の時の停止などに使う。
    void Brake(float decelerationPerFrame);

    void SetTransform(Transform* value) { transform = value; }

    // スキル使用時に速度を付与。
    void ApplyRollingImpulse(const Vector3& impulse);

    // CharacterBaseが持つパラセール状態を一度だけ登録する（所有しない）
    void SetParaseiledSource(const bool* value) { paraseiledSource = value; }
    bool IsParaseiled() const { return paraseiledSource && *paraseiledSource; }

    // パラセールを開いた瞬間に呼ぶ
    void OnOpen(void);

#pragma endregion


private:

    const bool* paraseiledSource = nullptr;

    Transform* transform = nullptr; // CharacterBaseが所有

#pragma region 移動パラメータ

    // 通常移動の最大速度
    float moveSpeed = 5.0f;

    // 空中の加速度
    float airAcceleration = 0.12f;

    // 滑空中の重力
    float glideGravity = 0.15f;

    // 落下速度の上限
    float maxFallSpeed = 3.0f;

    // 高速移動中の方向修正倍率
    float momentumSteering = 0.05f;

    // 高速移動と判定する速度
    float momentumThreshold = 10.0f;

    // 旧ActorBaseの減速度を維持する。
    float airDrag = 0.08f;

    // 体の向きを変える速さ
    float turnRate = Deg2Rad(3.0f);

#pragma endregion


    // 現在の向き(角速度の保持のため)
    float yaw = 0.0f;

};
