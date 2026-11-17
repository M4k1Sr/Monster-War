#pragma once

// 挙動タイプ
enum class COLLIDER_MOBILITY { Static, Movable };

// 形状列挙型定義
enum class COLLIDER_SHAPE
{
	None = -1,

	Line,
	Sphere,
	Capsule,
	Box,
	Mesh,
	XzCircle,
};

// コライダータグ列挙型定義
enum class COLLIDER_TAG
{
	None = -1,

	Player,
	PlayerAttackSlash,
	PlayerRollingSlash,
	PlayerRushSlash,
	PlayerKickDownAttack,
	Playerdodge,

	PlayerWeaponItem,

	Enemy,
	GobKingSwing,

	Stage,
	WindMill,
	IceStage,
	Water,

	Goal,

	CameraPoint,
	CameraLine,

	DebugObject,
};