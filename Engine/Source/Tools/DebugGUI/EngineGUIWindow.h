#pragma once
#include "Core/Object/Object.h"

/**
 *	설명
 */
class UEngineGUIWindow : public UObject
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UEngineGUIWindow();
	ENGINE_API ~UEngineGUIWindow();

	/** 객체 값 복사 방지 */
	UEngineGUIWindow(const UEngineGUIWindow& Other) = delete;
	UEngineGUIWindow(UEngineGUIWindow&& Other) noexcept = delete;
	UEngineGUIWindow& operator=(const UEngineGUIWindow& Other) = delete;
	UEngineGUIWindow& operator=(UEngineGUIWindow&& Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void OnGUI(ULevel* _Level) = 0;

protected:

private:

};

