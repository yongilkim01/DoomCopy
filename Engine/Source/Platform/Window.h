#pragma once
// Os Header
#include <Windows.h>

// std Header
#include <map>
#include <string>
#include <functional>

// user header
#include "Math/EngineMath.h"


/** 
 *	������ â Ŭ����
 *  TODO: GLFW�� �̿��� �ٸ� �÷��������� â�� ��� �� �ֵ��� ���� ����
 */
class UEngineWindow
{
public:
	/** ���� ������ �ʱ�ȭ, ���� �޼ҵ� */
	ENGINE_API static void EngineWindowInit(HINSTANCE Instance);
	ENGINE_API static void CreateWindowClass(const WNDCLASSEXA& WndClass);
	ENGINE_API static int WindowMessageLoop(std::function<void()> StartFunction, 
											std::function<void()> FrameFunction, 
											std::function<void()> EndFunction = nullptr);

	/** ������, �Ҹ��� */
	ENGINE_API UEngineWindow();
	ENGINE_API ~UEngineWindow();

	/** ��ü �� ���� ���� */
	UEngineWindow(const UEngineWindow& Other) = delete;
	UEngineWindow(UEngineWindow&& Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& Other) noexcept = delete;

	/** ������ ����, ���� �޼ҵ� */
	ENGINE_API void Create(std::string_view TitleName, std::string_view ClassName = "Default");
	ENGINE_API void Open(std::string_view TitleName = "Window");

	/** ��, �� �޼ҵ� */
	inline FVector GetWindowSize() const
	{
		return WindowSize;
	}
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}
	void SetWindowPosAndScale(FVector Position, FVector Scale);
	FVector GetMousePos();

protected:

private:
	static HINSTANCE hInstance;
	static std::map<std::string, WNDCLASSEXA> WindowClasses;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
};


