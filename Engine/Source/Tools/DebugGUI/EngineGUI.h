#pragma once
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"

class UEngineGUIWindow;

/**
 *	����
 */
class UEngineGUI
{
public:
	/** ������, �Ҹ��� */
	UEngineGUI();
	~UEngineGUI();

	/** ��ü �� ���� ���� */
	UEngineGUI(const UEngineGUI& Other) = delete;
	UEngineGUI(UEngineGUI&& Other) noexcept = delete;
	UEngineGUI& operator=(const UEngineGUI& Other) = delete;
	UEngineGUI& operator=(UEngineGUI&& Other) noexcept = delete;

	static void Init();
	static void Release();
	static void StartGUIRender();
	static void EndGUIRender();

	ENGINE_API static void PushGUIWindow(std::shared_ptr<UEngineGUIWindow> GUIWindow);

	template<typename WindowType>
	static std::shared_ptr<WindowType> CreateGUIWindow(std::string _Text)
	{
		std::shared_ptr<WindowType> Window = std::make_shared<WindowType>();
		Window->SetName(_Text);
		PushGUIWindow(Window);
		return Window;
	}
	static void GUIRender(ULevel* _Level);

protected:

private:
	static inline std::map<std::string, std::shared_ptr<class UEngineGUIWindow>> GUIWindows;

};

