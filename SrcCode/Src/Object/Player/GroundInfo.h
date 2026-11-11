#pragma once

#include "../../Common/Vector3.h"
#include "../../Object/Common/Collider/ColliderTagDefine.h"

struct GroundInfo {
	bool hit = false;
	float distance = 0.0f;		// レイ始点から地面まで
	Vector3 point;
	Vector3 normal = Vector3::Yonly(1.0f);
	COLLIDER_TAG tag = COLLIDER_TAG::None;
};