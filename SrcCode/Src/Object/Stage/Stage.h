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

private:

	// XV
	void SubUpdate(void)override;
};

