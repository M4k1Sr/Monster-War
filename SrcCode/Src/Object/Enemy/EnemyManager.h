#pragma once

#include "../Common/ActorBase/ActorBase.h"

class EnemyBase;

class EnemyManager : public ActorBase
{
public:

	EnemyManager();

	~EnemyManager()override = default;

	void Load(void)override;

	void SetPlayerPos(const Vector3* pos);

	const Vector3* GetNearestEnemyPos(const Vector3& from, float range);

private:
};

