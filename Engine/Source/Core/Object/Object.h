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

	/**
	 *	������Ʈ ��ü�� �� �� �ڿ� �ı��Ǵ� �޼ҵ�
	 *	@param Time: ������Ʈ ��ü�� �ı��Ǵ� ������ Ÿ��
	 */
	void Destroy(float Time = 0.0f);
	/**
	 *	������Ʈ ��ü�� �ı��Ǳ���� �ð��� üũ�ϴ� �޼ҵ�
	 *	@param DeltaTime: ���� ��ŸŸ��
	 */
	virtual void ReleaseTimeCheck(float DeltaTime);
	virtual void ReleaseCheck(float DeltaTime);

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

protected:

private:
	std::string Name;

	bool IsDebugValue = false;
	bool IsActiveValue = false;
	bool IsDestroyValue = true;
	bool IsDeathTimeCheck = false;

	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

};