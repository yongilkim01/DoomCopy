#pragma once

enum class EGunType : uint8
{
	NONE,
	PISTOL,
	SHOTGUN,
};

enum class EGunState : uint8
{
	NONE,
	MOVE,
	FIRE,
	RELOAD,
};

enum class EDropItemType
{
	NONE,
	ARMOR,
	PORTION,
	SHOTGUN,
};

enum class EDropItemState
{
	NONE,
	DROP,
	PICKUP,
};
