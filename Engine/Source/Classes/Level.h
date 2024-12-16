#pragma once

#include "Core/Object/Object.h"
#include "EngineDefine.h"

/**
 *	���� ���� Ŭ����
 */
class ULevel : public UObject
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API ULevel();
	ENGINE_API ~ULevel();

	/** ��ü �� ���� ���� */
	ULevel(const ULevel& Other) = delete;
	ULevel(ULevel&& Other) noexcept = delete;
	ULevel& operator=(const ULevel& Other) = delete;
	ULevel& operator=(ULevel&& Other) noexcept = delete;

protected:

private:

};