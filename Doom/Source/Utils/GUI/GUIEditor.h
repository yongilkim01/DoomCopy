#pragma once
#include <Tools/DebugGUI/EngineGUIWindow.h>

/**
 *	����
 */
class UGUIEditor : public UEngineGUIWindow
{
public:
	/** ������, �Ҹ��� */
	UGUIEditor();
	~UGUIEditor();

	/** ��ü �� ���� ���� */
	UGUIEditor(const UGUIEditor& Other) = delete;
	UGUIEditor(UGUIEditor&& Other) noexcept = delete;
	UGUIEditor& operator=(const UGUIEditor& Other) = delete;
	UGUIEditor& operator=(UGUIEditor&& Other) noexcept = delete;

protected:
	virtual void OnGUI() override;

private:

};

