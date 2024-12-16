#pragma once
#include <string>

#include "EngineDefine.h"

/**
 *	���� �ֻ��� ������Ʈ Ŭ���� 
 */
class UObject
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UObject();
	ENGINE_API ~UObject();

	/** ��ü �� ���� ���� */
	UObject(const UObject& Other) = delete;
	UObject(UObject&& Other) noexcept = delete;
	UObject& operator=(const UObject& Other) = delete;
	UObject& operator=(UObject&& Other) noexcept = delete;

	/** ��, �� �޼ҵ� */
	std::string GetName() const
	{
		return Name;
	}
	std::string_view GetNameView() const
	{
		return Name.c_str();
	}
	virtual void SetName(std::string_view ChangeName)
	{
		Name = ChangeName;
	}
	virtual bool IsActive()
	{
		return IsActiveValue;
	}
	virtual bool IsDestroy()
	{
		return IsDestroyValue;
	}

protected:

private:
	std::string Name;

	bool IsActiveValue = false;
	bool IsDestroyValue = true;

};