#pragma once
#include "Core/Object/Object.h"

/**
 *	����
 */
class UEngineGUIWindow : public UObject
{
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
	virtual void OnGUI(ULevel* _Level) = 0;

protected:

private:

};

