#pragma once
// Os Header
#include <Windows.h>

// std Header
#include <map>
#include <string>
#include <functional>

// user header
#include "Core/Public/Math/EngineMath.h"


/** 
 *	윈도우 창 클래스
 *  TODO: GLFW를 이용한 다른 플랫폼에서도 창을 띄울 수 있도록 변경 예정
 */
class UEngineWindow
{
public:
	/** 엔진 윈도우 초기화, 루프 메소드 */
	ENGINE_API static void EngineWindowInit(HINSTANCE Instance);
	ENGINE_API static void CreateWindowClass(const WNDCLASSEXA& WndClass);
	ENGINE_API static int WindowMessageLoop(std::function<void()> StartFunction, 
											std::function<void()> FrameFunction, 
											std::function<void()> EndFunction = nullptr);

	/** 생성자, 소멸자 */
	ENGINE_API UEngineWindow();
	ENGINE_API ~UEngineWindow();

	/** 객체 값 복사 방지 */
	UEngineWindow(const UEngineWindow& Other) = delete;
	UEngineWindow(UEngineWindow&& Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& Other) noexcept = delete;

	/** 윈도우 생성, 열기 메소드 */
	ENGINE_API void Create(std::string_view TitleName, std::string_view ClassName = "Default");
	ENGINE_API void Open(std::string_view TitleName = "Window");

	/** 겟, 셋 메소드 */
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
	ENGINE_API bool IsFocus()
	{
		return bFocus;
	}

protected:

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ENGINE_API static HINSTANCE hInstance;
	ENGINE_API static std::map<std::string, WNDCLASSEXA> WindowClasses;
	ENGINE_API static std::function<bool(HWND, UINT, WPARAM, LPARAM)> CustomProc;
	ENGINE_API static std::map<HWND, UEngineWindow*> AllWindowMap;

	inline static bool LoopActive = true;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
	bool bFocus = false;
};


