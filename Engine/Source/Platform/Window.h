#pragma once
// Os Header
#include <Windows.h>

// std Header
#include <map>
#include <string>
#include <functional>

// user header
#include "Core/Math/EngineMath.h"


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
	ENGINE_API void SetWindowPosAndScale(FVector Position, FVector Scale);
	ENGINE_API FVector GetMousePos();
	ENGINE_API static void SetCustomProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> _CustomProc);

	ENGINE_API inline FVector GetWindowSize() const
	{
		return WindowSize;
	}
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}
	HWND GetWindowHandle() const
	{
		return WindowHandle;
	}
	ENGINE_API static bool IsApplicationOn()
	{
		return LoopActive;
	}

protected:

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ENGINE_API static HINSTANCE hInstance;
	ENGINE_API static std::map<std::string, WNDCLASSEXA> WindowClasses;
	ENGINE_API static std::function<bool(HWND, UINT, WPARAM, LPARAM)> CustomProc;

	inline static bool LoopActive = true;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
};


