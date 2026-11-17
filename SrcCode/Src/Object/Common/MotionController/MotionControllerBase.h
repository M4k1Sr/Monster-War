#pragma once

#include <functional>

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../PhysicsBody/PhysicsBody.h"

// 移動操作の共通基底。PhysicsBodyの所有権は持たない。
class MotionControllerBase
{
public:
    virtual ~MotionControllerBase() = default;

    // CharacterBase::AddMotionControllerから一度だけ設定する。
    void Init(PhysicsBody* body) { physicsBody = body; }

    // 時間倍率の取得元を設定する（CharacterBase::AddMotionControllerが登録する）。
    // 持ち主のグループ別タイムスケールを反映するために使う。
    void SetTimeScaleSource(const std::function<float(void)>& source) { timeScaleSource = source; }

    // 入力方向を反映する。時間倍率は持ち主のものを内部で取得する。
    virtual void ApplyInput(const Vector3& direction) = 0;

    // 入力がないステートでも毎フレーム呼び出す移動制御。
    virtual void Update(float timeScale) {}

protected:

    // 持ち主の現在の時間倍率を取得する（未登録ならグローバル倍率）
    float CurrentTimeScale(void) const { return timeScaleSource ? timeScaleSource() : TimeScale::Get(); }

    // 物理挙動制御の参照
    PhysicsBody* physicsBody = nullptr;

private:

    // 時間倍率の取得関数
    std::function<float(void)> timeScaleSource;
};
