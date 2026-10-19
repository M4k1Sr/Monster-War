#include "EnemyManager.h"

#include "EnemyBase.h"

#include "GobKing/GobKing.h"
//#include "Cactus/Cactus.h"
//#include "Thorn/Thorn.h"
//#include "Boss/Boss.h"

EnemyManager::EnemyManager() :
	ActorBase("Data/Parameter/Enemy/")
{
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

	//// サボテン生成
	//for (int i = 1; ; i++) {
	//	// その回のパラメーター名
	//	std::string parameterName = "CactusPos" + std::to_string(i);

	//	// そのパラメータが存在するかどうか
	//	if (!IsParameterExist("InitPos", parameterName)) { break; }

	//	// 存在するならば、その座標にその敵を生成
	//	AddChildActor(new Cactus(GetParameterToVector3("InitPos", parameterName)));
	//}

	//// とげ生成
	//for (int i = 1; ; i++) {
	//	// その回のパラメーター名
	//	std::string parameterName = "ThornPos" + std::to_string(i);

	//	// そのパラメータが存在するかどうか
	//	if (!IsParameterExist("InitPos", parameterName)) { break; }

	//	// 存在するならば、その座標にその敵を生成
	//	AddChildActor(new Thorn(GetParameterToVector3("InitPos", parameterName)));
	//}

	//// ボス生成
	//for (int i = 1; ; i++) {
	//	// その回のパラメーター名
	//	std::string parameterName = "BossPos" + std::to_string(i);

	//	// そのパラメータが存在するかどうか
	//	if (!IsParameterExist("InitPos", parameterName)) { break; }

	//	// その回のパラメーター名
	//	std::string parameterHp = "BossHp";
	//	// そのパラメータが存在するかどうか
	//	if (!IsParameterExist("States", parameterHp)) { break; }

	//	// その回のパラメーター名
	//	std::string parameterPower = "BossPower";

	//	// そのパラメータが存在するかどうか
	//	if (!IsParameterExist("States", parameterPower)) { break; }

	//	// 存在するならば、その座標にその敵を生成
	//	AddChildActor(new Boss(GetParameterToVector3("InitPos", parameterName), GetParameterToInt("States", parameterHp)));
	//}

}
void EnemyManager::SetPlayerPos(const Vector3* pos)
{
	for (ActorBase* child : GetChildActors()) {

		if (GobKing* gobKing = dynamic_cast<GobKing*>(child)) {

			gobKing->SetPlayerPos(pos);

			continue;
		}

		//if (Cactus* cactus = dynamic_cast<Cactus*>(child)) {

		//	cactus->SetPlayerPos(pos);

		//	continue;
		//}
	}
}
