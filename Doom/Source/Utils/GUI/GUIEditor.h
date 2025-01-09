#pragma once
#include <Tools/DebugGUI/EngineGUIWindow.h>

/**
 *	설명
 */
class UGUIEditor : public UEngineGUIWindow
{
public:
	/** 생성자, 소멸자 */
	UGUIEditor();
	~UGUIEditor();

	/** 객체 값 복사 방지 */
	UGUIEditor(const UGUIEditor& Other) = delete;
	UGUIEditor(UGUIEditor&& Other) noexcept = delete;
	UGUIEditor& operator=(const UGUIEditor& Other) = delete;
	UGUIEditor& operator=(UGUIEditor&& Other) noexcept = delete;

protected:
	virtual void OnGUI() override;

private:

};

