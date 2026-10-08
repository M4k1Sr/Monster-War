#pragma once

#include <map>

#include "../Common/ActorBase/ActorBase.h"

class Stage
	: public ActorBase
{
public:
	Stage();
	~Stage()override = default;

	// “Ç‚İ‚İˆ—
	void Load(void)override;

	// “–‚½‚è”»’è‚Ì’Ê’m
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	// XV
	void SubUpdate(void)override;

};

