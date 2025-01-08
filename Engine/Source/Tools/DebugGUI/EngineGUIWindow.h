#pragma once
#include "Core/Object/Object.h"

class UEngineGUI;
class ULevel;

/**
 *	����
 */
class UEngineGUIWindow : public UObject
{
	friend class UEngineGUI;
public:
	/** ������, �Ҹ��� */
	ENGINE_API UEngineGUIWindow();
	ENGINE_API ~UEngineGUIWindow();

	/** ��ü �� ���� ���� */
	UEngineGUIWindow(const UEngineGUIWindow& Other) = delete;
	UEngineGUIWindow(UEngineGUIWindow&& Other) noexcept = delete;
	UEngineGUIWindow& operator=(const UEngineGUIWindow& Other) = delete;
	UEngineGUIWindow& operator=(UEngineGUIWindow&& Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void OnGUI() = 0;

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World = nullptr;

};

