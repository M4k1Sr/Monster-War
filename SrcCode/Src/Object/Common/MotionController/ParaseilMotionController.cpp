#include "ParaseilMotionController.h"

#include <algorithm>
#include <cmath>
#include "../../../Manager/TimeScale/TimeScale.h"

#pragma region 移動制御

void ParaseilMotionController::Update(float timeScale)
{
    if (!physicsBody || physicsBody->IsFrozen() || timeScale <= 0.0f) return;
    if (physicsBody->GetMode() != PhysicsBody::MotionMode::Character) return;

    // パラセールを開いてないときはなにもしない。
    if (!IsParaseiled()) { return; }

    Vector3 velocity = physicsBody->GetVelocity();

    //  水平方向でゆっくり減速 airDrag＊時間によりマイナスされる。
    const float speed = std::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
    if (speed > 0.0f) {
        const float remaining = speed - airDrag * timeScale;
        const float ratio = remaining > 0.0f ? remaining / speed : 0.0f;

        velocity.x *= ratio;
        velocity.z *= ratio;
    }

    // 垂直方向は重力によりゆっくり下がっていく上限で止める
    velocity.y -= glideGravity * timeScale;
    if (velocity.y < -maxFallSpeed) { velocity.y = -maxFallSpeed; }

    physicsBody->SetVelocity(velocity);
}
void ParaseilMotionController::ApplyInput(const Vector3& inputDirection)
{
    // パラセールを開いてないときはなにもしない。
    if (!IsParaseiled()) { return; }

    if (!physicsBody) return;
    PhysicsBody& body = *physicsBody;
    // 持ち主の時間倍率
    const float timeScale = CurrentTimeScale();
    // 物理停止中・時間停止中は処理しない
    if (body.IsFrozen() || timeScale <= 0.0f) return;

    Vector3 input(inputDirection);

    // キャラの前方向と右方向
    Vector3 forward = transform->GetForward();
    forward.y = 0.0f;
    forward.Normalize();
    Vector3 right = Vector3(forward.z, 0.0f, -forward.x);

    // 入力キャラ基準の前後、左右に分ける
    float inputFwd = input.Dot(forward);
    float inputSide = input.Dot(right);

    // 向きは左右成分だけで旋回する(真後ろ入力は振り向かない→原作がそうです。)
    yaw += inputSide * turnRate * timeScale;
    if (yaw > DX_PI_F) yaw -= DX_TWO_PI_F;
    if (yaw < -DX_PI_F) yaw += DX_TWO_PI_F;
    transform->SetRotation(Quaternion::FromRotationY(yaw));

    // 旋回したのちの前方向で進む
    forward = Vector3(sinf(yaw), 0.0f, cosf(yaw));

    // 速度 前は加速、後ろは減速しながらゆっくり進行
    float targetSpeed = (inputFwd >= 0.0f)
        ? moveSpeed * inputFwd
        : backSpeed * inputFwd;

    Vector3 v = body.GetVelocity();
    Vector3 horizontal(v.x, 0.0f, v.z);

    // 現在の水平移動速度を取得
    const float speed = horizontal.Length();

    // 高速移動中か判定
    const bool highSpeed = speed > momentumThreshold;

    // 高速移動中は方向修正倍率を適用
    const float steering =
        highSpeed ? momentumSteering : 1.0f;

    // パラセール状態の加速度を計算
    const float accel =
        airAcceleration * steering * timeScale;

    // 高速移動中の場合
    if (highSpeed)
    {
        // 入力がある場合のみ弱い加速度を加算
        if (input.Length() > 0.0001f)
        {
            horizontal += input * accel;

            // 曲がるのはOKだが、速さは増やさないようにする
            // 減速はUpdateに任せる
            const float newSpeed = horizontal.Length();
            if (newSpeed > speed) {
                horizontal *= (speed / newSpeed);
            }
        }
    }
    // 通常移動中の場合 
    // キャラの前方向に目標速度進む
    else
    {
        // 入力方向から目標速度を計算
        const Vector3 target = forward * targetSpeed;

        // 現在速度から目標速度への差を計算
        Vector3 delta = target - horizontal;

        // 速度差の大きさを取得
        const float len = delta.Length();

        // 1フレームあたりの加速度に制限
        if (len > accel && len > 0.0001f)
        {
            delta *= (accel / len);
        }

        // 目標速度へ向けて加減速
        horizontal += delta;
    }

    //// 入力方向へキャラクターの向きを回す
    //RotateTowards(input, timeScale);

    // 水平方向の速度を反映
    v.x = horizontal.x;
    v.z = horizontal.z;

    // 更新した速度を設定
    body.SetVelocity(v);

}

void ParaseilMotionController::SetHorizontalVelocity(const Vector3& horizontal)
{
    if (!physicsBody || physicsBody->IsFrozen()) return;

    Vector3 v = physicsBody->GetVelocity();
    v.x = horizontal.x;
    v.z = horizontal.z;
    physicsBody->SetVelocity(v);
}

void ParaseilMotionController::SetVerticalVelocity(float y)
{
    if (!physicsBody || physicsBody->IsFrozen()) return;

    Vector3 v = physicsBody->GetVelocity();
    v.y = y;
    physicsBody->SetVelocity(v);
}

void ParaseilMotionController::FaceDirection(const Vector3& direction)
{
    const float timeScale = CurrentTimeScale();
    if (timeScale <= 0.0f) return;
    RotateTowards(direction, timeScale);
}

void ParaseilMotionController::Brake(float decelerationPerFrame)
{
    if (!physicsBody || physicsBody->IsFrozen()) return;

    const float timeScale = CurrentTimeScale();
    if (timeScale <= 0.0f) return;

    Vector3 v = physicsBody->GetVelocity();
    const float speed = std::sqrt(v.x * v.x + v.z * v.z);
    if (speed <= 0.0f) return;

    const float remaining = speed - decelerationPerFrame * timeScale;
    const float ratio = remaining > 0.0f ? remaining / speed : 0.0f;

    v.x *= ratio;
    v.z *= ratio;
    physicsBody->SetVelocity(v);
}

void ParaseilMotionController::ApplyImpulse(const Vector3& impulse)
{
    if (!physicsBody || physicsBody->IsFrozen()) return;

    // 開いてないときは受けない
    if (!IsParaseiled()) return;

    // 上昇気流に乗ったとき速度を付与する。
    physicsBody->AddVelocity(impulse);

}

void ParaseilMotionController::OnOpen(void)
{
    // 現在の向きからyawを取り出す
    if (transform) {
        const Vector3 fwd = transform->rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
        yaw = atan2f(fwd.x, fwd.z);
    }

    if (!physicsBody)return;
    Vector3 v = physicsBody->GetVelocity();

    // 落下中だけ止める
    if (v.y < 0.0f) v.y = 0.0f;

    physicsBody->SetVelocity(v);
}

void ParaseilMotionController::RotateTowards(const Vector3& direction, float timeScale)
{
    if (!transform) return;

    const Vector3 flat(direction.x, 0.0f, direction.z);
    if (flat.LengthSq() <= 0.0001f) return;

    // 目的方向 targetYaw
    const float targetYaw = atan2f(flat.x, flat.z);

    // 差を -π〜π に折り返す
    float diff = targetYaw - yaw;
    while (diff > DX_PI_F) diff -= DX_TWO_PI_F;
    while (diff < -DX_PI_F) diff += DX_TWO_PI_F;

    // 1フレームで回せる角度の制限
    const float maxStep = turnRate * timeScale;
    if (diff > maxStep) diff = maxStep;
    if (diff < -maxStep) diff = -maxStep;

    yaw += diff;

    // yaw自体も-π∼πに保つ
    if (yaw > DX_PI_F) yaw -= DX_TWO_PI_F;
    if (yaw < -DX_PI_F) yaw += DX_TWO_PI_F;

    transform->SetRotation(Quaternion::FromRotationY(yaw));
}

#pragma endregion
