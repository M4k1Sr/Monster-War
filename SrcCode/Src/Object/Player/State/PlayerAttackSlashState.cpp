#include <algorithm>

#include "PlayerAttackSlashState.h"

#include "../../Common/Transform/Transform.h"

#include "../../../Manager/Input/InputManager.h"

#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/MotionController/GroundMotionController.h"

#include "../Weapon/CollOperator/PlayerAttackSlashCollOperator.h"

PlayerAttackSlashState::PlayerAttackSlashState(
	float COLL_START_TIME,
	float COLL_END_TIME,
	float COMBO_ACCEPT_TIME,

	const Vector3& playerPos,

	const Vector3* const& lockOnTarget,

	GroundMotionController& motion,

	const bool& isRush,

	PlayerAttackSlashCollOperator& collOperator,

	std::function<void(void)> playAnimeAttackSlash_Down,
	std::function<void(void)> playAnimeAttackSlash_Up,
	std::function<void(void)> playAnimeAttackSlash_End,

	std::function<float(void)> getAnimeRatio,
	std::function<void(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	COMBO_ACCEPT_TIME(COMBO_ACCEPT_TIME),

	playerPos(playerPos),

	lockOnTarget(lockOnTarget),

	motion(motion),

	isRush(isRush),

	collOperator(collOperator),

	playAnimeAttackSlash_Down(playAnimeAttackSlash_Down),
	playAnimeAttackSlash_Up(playAnimeAttackSlash_Up),
	playAnimeAttackSlash_End(playAnimeAttackSlash_End),

	getAnimeRatio(getAnimeRatio),
	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(),
	comboStep(),

	RushComboCnt(0),

	comboKeyInputFrame(false),
	comboKeyInput(false)
{
}

void PlayerAttackSlashState::OwnStateConditionUpdate(void)
{
	// 自分の状態に遷移する条件関数(斬撃)
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
		OwnChangeState();
	}
}	

void PlayerAttackSlashState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator.Off();

	// コンボステップも同時に「斬り下ろし」へ
	comboStep = COMBO_STEP::Down;

	// 攻撃アニメーション再生
	playAnimeAttackSlash_Down();

	// コンボキー遷移
	comboKeyInputFrame = true;
	// コンボキー受付をリセット
	comboKeyInput = false;
}

void PlayerAttackSlashState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// コンボ入力の受付
	if (comboKeyInputFrame) {
		// このフレームのみスキップ
		comboKeyInputFrame = false;
	}
	else if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
		comboKeyInput = true;
	}

	// ステップ別更新
	switch (step) {

	case PlayerAttackSlashState::STEP::Startup: {
		// 前隙

		// 移動補正
		attackMoveOffset();

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.On();
		}

		break;
	}

	case PlayerAttackSlashState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		// 攻撃判定発生終了
		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.Off();
		}

		break;
	}

	case PlayerAttackSlashState::STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			changeStateIdle();
			return;
		}

		if (!comboKeyInput) { break; }					// コンボキーが押されていない場合はコンボ受付しない
		if (comboStep == COMBO_STEP::End) { break; }	// 最終段は繋がない
		if (animeRatio <= COMBO_ACCEPT_TIME) { break; }	// コンボ受付時間に達していない場合は受付ない

		// ラッシュの場合
		if (isRush) {
			StartRushComboNext();
		}
		else {
			StartComboNext();
		}

		break;
	}

	}
}

void PlayerAttackSlashState::Exit(void)
{
	// 当たり判定を消去
	collOperator.Off();
}

void PlayerAttackSlashState::StartComboNext(void)
{
	comboKeyInput = false;
	step = STEP::Startup;
	collOperator.Off();

	// コンボステップ
	switch (comboStep) {
	case COMBO_STEP::Down:
		comboStep = COMBO_STEP::Up;
		playAnimeAttackSlash_Up();
		break;
	case COMBO_STEP::Up:
		step = STEP::Startup;		// ステップを最初から
		comboStep = COMBO_STEP::End;// コンボステップを最終コンボに
		playAnimeAttackSlash_End();
		break;
	case COMBO_STEP::End:
		step = STEP::Startup;		// ステップを最初から
		comboStep = COMBO_STEP::Down;
		playAnimeAttackSlash_Down();	
		break;
	}
}

void PlayerAttackSlashState::StartRushComboNext(void)
{
	comboKeyInput = false;
	step = STEP::Startup;
	collOperator.Off();

	// コンボステップ
	switch (comboStep) {
	case COMBO_STEP::Down:
		comboStep = COMBO_STEP::Up;
		RushComboCnt++;
		playAnimeAttackSlash_Up();
		break;
	case COMBO_STEP::Up:
		step = STEP::Startup;		// ステップを最初から
		if (RushComboCnt > 2) {
			comboStep = COMBO_STEP::End;// コンボステップを最終コンボに
			playAnimeAttackSlash_End();
			break;
		}
		else {
			comboStep = COMBO_STEP::Down;// 斬りつけコンボへ
			playAnimeAttackSlash_Down();
			break;
		}
	case COMBO_STEP::End:
			step = STEP::Startup;	// ステップを最初から
			comboStep = COMBO_STEP::Down;
			playAnimeAttackSlash_Down();
			RushComboCnt = 0;
			break;
	}

}

void PlayerAttackSlashState::attackMoveOffset(void)
{
	// 移動方向
	Vector3 dir = Vector3();
	float dist = 0.0f;

	if (lockOnTarget != nullptr) {
		dir = *lockOnTarget - playerPos;	// プレイヤー→敵 のベクトル
		dir.y = 0.0f;						// 水平方向だけ使う
		dist = dir.Length();				// 正規化
		dir.Normalize();					// 長さ1にして「方向」にする
	}

	// 止まる距離 = 敵のコライダー半径 + 攻撃が届く距離
	const float stopDist = 150.0f + 50.0f;

	if (/*lockOnTarget != nullptr && */dist > stopDist) {

		if (isRush) {
			// ラッシュ中は敵へ向かって素早く踏み込む
			motion.FaceDirection(dir);
			motion.SetHorizontalVelocity(dir * RUSH_MOVE_SPEED);
		}
		else {
			// 通常攻撃はわずかに踏み込む(向き補正もApplyInput内でかかる)
			motion.SetMoveSpeed(NORMAL_MOVE_SPEED);
			motion.ApplyInput(dir);
		}
	}
	else {
		// 踏み込まない・踏み込み終わったら減速する
		motion.SetMoveSpeed(0.0f);
		motion.Brake(isRush ? RUSH_BRAKE : NORMAL_BRAKE);
	}
}
