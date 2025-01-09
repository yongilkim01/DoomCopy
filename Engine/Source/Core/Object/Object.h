#pragma once
#include <string>
#include <memory>

#include "EngineDefine.h"

/**
 *	���� �ֻ��� ������Ʈ Ŭ���� 
 */
class UObject : public std::enable_shared_from_this<UObject>
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UObject();
	ENGINE_API virtual ~UObject();

	/** ��ü �� ���� ���� */
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
	 *	������Ʈ ��ü�� �� �� �ڿ� �ı��Ǵ� �޼ҵ�
	 *	@param Time: ������Ʈ ��ü�� �ı��Ǵ� ������ Ÿ��
	 */
	void Destroy(float Time = 0.0f);
	/**
	 *	������Ʈ ��ü�� �ı��Ǳ���� �ð��� üũ�ϴ� �޼ҵ�
	 *	@param DeltaTime: ���� ��ŸŸ��
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