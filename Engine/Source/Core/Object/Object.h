#pragma once
#include <string>

#include "EngineDefine.h"

/**
 *	엔진 최상위 오브젝트 클래스 
 */
class UObject
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UObject();
	ENGINE_API ~UObject();

	/** 객체 값 복사 방지 */
	UObject(const UObject& Other) = delete;
	UObject(UObject&& Other) noexcept = delete;
	UObject& operator=(const UObject& Other) = delete;
	UObject& operator=(UObject&& Other) noexcept = delete;

	/** 겟, 셋 메소드 */
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