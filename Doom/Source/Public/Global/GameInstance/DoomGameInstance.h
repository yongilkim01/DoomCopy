#pragma once
#include <Engine/Classes/Engine/GameInstance.h>
#include "Public/Global/DoomStructs.h"

/**
 *	설명
 */
class UDoomGameInstance : public UGameInstance
{
public:
	/** 생성자, 소멸자 */
	UDoomGameInstance();
	~UDoomGameInstance();

	/** 객체 값 복사 방지 */
	UDoomGameInstance(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance(UDoomGameInstance&& Other) noexcept = delete;
	UDoomGameInstance& operator=(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance& operator=(UDoomGameInstance&& Other) noexcept = delete;

	FDoomStatus DoomGuyStatus;

	int GetDoomGuyHealth() { return DoomGuyHealth; }
	void SetDoomGuyHealth(int Health) { DoomGuyHealth = Health; }
	void AddDoomGuyHealth(int Health) 
	{ 
		DoomGuyHealth += Health; 
		if (DoomGuyHealth < 0) DoomGuyHealth = 0;
		
	}
	int GetDoomGuyBullet() { return DoomGuyBullet; }
	void SetDoomGuyBullet(int Bullet) { DoomGuyBullet = Bullet; }
	void AddDoomGuyBullet(int Bullet) 
	{
		DoomGuyBullet += Bullet;
		if (DoomGuyBullet < 0) DoomGuyBullet = 0;
	}

	int GetDoomGuyShell() { return DoomGuyShell; }
	void SetDoomGuyShell(int Shell) { DoomGuyShell = Shell; }
	void AddDoomGuyShell(int Shell) 
	{
		DoomGuyShell += Shell;
		if (DoomGuyShell < 0) DoomGuyShell = 0;
	}

	int GetDoomGuyRocket() { return DoomGuyRocket; }
	void SetDoomGuyRocket(int Rocket) { DoomGuyRocket = Rocket; }
	void AddDoomGuyRocket(int Rocket) 
	{
		DoomGuyRocket += Rocket;
		if (DoomGuyRocket < 0) DoomGuyRocket = 0;
	}

	int GetDoomGuyCell() { return DoomGuyCell; }
	void SetDoomGuyCell(int Cell) { DoomGuyCell = Cell; }
	void AddDoomGuyCell(int Cell) 
	{
		DoomGuyCell += Cell;
		if (DoomGuyCell < 0) DoomGuyCell = 0;
	}

	int GetDoomGuyArmor() { return DoomGuyArmor; }
	void SetDoomGuyArmor(int Armor) { DoomGuyArmor = Armor; }
	void AddDoomGuyArmor(int Armor)
	{
		DoomGuyArmor += Armor;
		if (DoomGuyArmor < 0) DoomGuyArmor = 0;
	}

	int GetDoomGuyWeaponCount() { return DoomGuyWeaponCount; }
	void SetDoomGuyWeaponCount(int WeaponCount) { DoomGuyWeaponCount = WeaponCount; }
	void AddDoomGuyWeaponCount(int WeaponCount)
	{
		DoomGuyWeaponCount += WeaponCount;
		if (DoomGuyWeaponCount < 0) DoomGuyWeaponCount = 0;
		else if (DoomGuyWeaponCount > 2) DoomGuyWeaponCount = 2;
	}

protected:

private:
	int DoomGuyHealth = 100;
	int DoomGuyBullet = 50;
	int DoomGuyShell = 20;
	int DoomGuyRocket = 0;
	int DoomGuyCell = 0;
	int DoomGuyArmor = 0;
	int DoomGuyWeaponCount = 1;
};

