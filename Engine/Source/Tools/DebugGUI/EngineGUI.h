#pragma once
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"
#include "ThirdParty/imgui/imgui_impl_win32.h"

#include "Core/Public/Containers/EngineString.h"

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
	static std::shared_ptr<WindowType> CreateGUIWindow(std::string_view GUIWindowName)
	{
		std::string UpperGUIWindowName = UEngineString::ToUpper(GUIWindowName);

		if (nullptr != FindGUIWindow(UpperGUIWindowName))
		{
			MSGASSERT("������ �̸��� GUIWindow�� �̹� �ֽ��ϴ�" + UpperGUIWindowName);
			return nullptr;
		}

		std::shared_ptr<WindowType> Window = std::make_shared<WindowType>();
		Window->SetName(UpperGUIWindowName);
		PushGUIWindow(Window);

		return Window;
	}
	template<typename WindowType>
	static std::shared_ptr<WindowType> FindGUIWindow(std::string_view GUIWindowName)
	{
		return std::dynamic_pointer_cast<WindowType>(FindGUIWindow(GUIWindowName));
	}
	ENGINE_API static std::shared_ptr<UEngineGUIWindow> FindGUIWindow(std::string_view GUIWindowName);

	static void GUIRender(ULevel* _Level);

	ENGINE_API static void OnAllWindow();
	ENGINE_API static void OffAllWindow();


protected:

private:
	static inline std::map<std::string, std::shared_ptr<class UEngineGUIWindow>> GUIWindows;

};

