#pragma once
#include <string>
#include <memory>

#include "EngineDefine.h"

/**
 *	엔진 최상위 오브젝트 클래스 
 */
class UObject : public std::enable_shared_from_this<UObject>
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UObject();
	ENGINE_API virtual ~UObject();

	/** 객체 값 복사 방지 */
	UObject(const UObject& Other) = delete;
	UObject(UObject&& Other) noexcept = delete;
	UObject& operator=(const UObject& Other) = delete;
	UObject& operator=(UObject&& Other) noexcept = delete;

	template<typename ChildPtrType>
	std::shared_ptr<ChildPtrType> GetThis()
	{
		return std::static_pointer_cast<ChildPtrType>(shared_from_this());
	}

	/**
	 *	오브젝트 객체가 몇 초 뒤에 파괴되는 메소드
	 *	@param Time: 오브젝트 객체가 파괴되는 딜레이 타임
	 */
	void Destroy(float Time = 0.0f);
	/**
	 *	오브젝트 객체가 파괴되기까지 시간을 체크하는 메소드
	 *	@param DeltaTime: 엔진 델타타임
	 */
	virtual void ReleaseTimeCheck(float DeltaTime)
	{
		if (false == IsDeathTimeCheck)
		{
			return;
		}

		CurDeathTime += DeltaTime;

		if (DeathTime <= CurDeathTime)
		{
			IsDestroyValue = true;
		}
	}
	virtual void ReleaseCheck(float DeltaTime)
	{

	}

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
	void SetActive(bool IsActive)
	{
		IsActiveValue = IsActive;
	}
	void SetActiveSwitch()
	{
		IsActiveValue = !IsActiveValue;
	}
	bool IsDebug()
	{
		return IsDebugValue;
	}
	void DebugOn()
	{
		IsDebugValue = true;
	}
	void DebugOff()
	{
		IsDebugValue = false;
	}
	void DebugSwitch()
	{
		IsDebugValue = !IsDebugValue;
	}
	int GetOrder()
	{
		return Order;
	}
	virtual void SetOrder(int NewOrder)
	{
		if (0 != GetOrder() && NewOrder == GetOrder())
		{
			return;
		}

		Order = NewOrder;
	}
	bool& IsActiveRef()
	{
		return IsActiveValue;
	}

protected:

private:
	std::string Name;

	bool IsActiveValue = true;
	bool IsDebugValue = false;
	bool IsDestroyValue = false;
	bool IsDeathTimeCheck = false;

	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

	int Order = 0;
};