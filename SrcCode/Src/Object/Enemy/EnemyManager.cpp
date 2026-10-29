#include "EnemyManager.h"

#include "EnemyBase.h"

#include "GobKing/GobKing.h"

EnemyManager::EnemyManager() :
	ActorBase("Data/Parameter/Enemy/")
{
	SetTimeGroup(TimeScale::GROUP::Enemy);
}

void EnemyManager::Load(void)
{
	// ゴブリンキング生成
	for (int i = 1; ; i++) {
		// その回のパラメーター名
		std::string parameterName = "GobPos" + std::to_string(i);

		// そのパラメータが存在するかどうか
		if (!IsParameterExist("InitPos", parameterName)) { break; }

		// その回のパラメーター名
		std::string parameterHp = "GobHp";
		// そのパラメータが存在するかどうか
		if (!IsParameterExist("Status", parameterHp)) { break; }

		// その回のパラメーター名
		std::string parameterPower = "GobPower";
		// そのパラメータが存在するかどうか
		if (!IsParameterExist("Status", parameterPower)) { break; }

		// 存在するならば、その座標にその敵を生成
		AddChildActor(new GobKing(GetParameterToVector3("InitPos", parameterName), GetParameterToInt("Status", parameterHp)));
	}



}
void EnemyManager::SetPlayerPos(const Vector3* pos)
{
	for (ActorBase* child : GetChildActors()) {

		if (GobKing* gobKing = dynamic_cast<GobKing*>(child)) {

			gobKing->SetPlayerPos(pos);

			continue;
		}

		// 随時追加
		// ダイアー・タウロス
		//if (DireTaurus* direTaurus = dynamic_cast<DireTaurus*>(child)) {

		//	direTaurus->SetPlayerPos(pos);

		//	continue;
		//}


		// クリムゾン・アイズ
		//if (CrimsonEyes* crimsonEyes = dynamic_cast<CrimsonEyes*>(child)) {

		//	crimsonEyes->SetPlayerPos(pos);

		//	continue;
		//}
	}
}

const Vector3* EnemyManager::GetNearestEnemyPos(const Vector3& from, float range)
{
	const Vector3* nearest = nullptr;
	float nearestSq = range * range;

	for (ActorBase* child : GetChildActors()) {
		// 死亡済みの敵は除外
		const Vector3& p = child->GetTrans().pos;
		const float distSq = (p - from).LengthSq();

		if (distSq < nearestSq) { nearestSq = distSq; nearest = &p; }
	}
	return nearest;

}
