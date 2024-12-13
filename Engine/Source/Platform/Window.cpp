#include "pch.h"
#include "Window.h"

#include "WorldPartition/DebugHelpers.h"

HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEX> UEngineWindow::WindowClasses;
int WindowCount = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        ++WindowCount;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: hdc를 사용한 그리기 코드 추가 예정
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        --WindowCount;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


ENGINE_API void UEngineWindow::EngineWindowInit(HINSTANCE Instance)
{
    hInstance = Instance;

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;

    CreateWindowClass(wcex);
}

ENGINE_API void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& WndClass)
{
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasses.end();
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasses.find(std::string(WndClass.lpszClassName));

    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(WndClass.lpszClassName) + "동일한 이름의 윈도우가 이미 존재합니다.");
        return;
    }
    
    RegisterClassExA(&WndClass);

    WindowClasses.insert(std::pair{ WndClass.lpszClassName, WndClass });

}

ENGINE_API int UEngineWindow::WindowMessageLoop(std::function<void()> StartFunction, 
                                                std::function<void()> FrameFunction, 
                                                std::function<void()> EndFunction)
{
    MSG msg = MSG();

    if (nullptr != StartFunction)
    {
        StartFunction();
    }

    if (nullptr == FrameFunction)
    {
        MSGASSERT("업데이트 함수가 바인드 되어 있지 않습니다.");
        return 0;
    }
    while (0 != WindowCount)
    {
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        FrameFunction();
    }

    if (nullptr != EndFunction)
    {
        EndFunction();
    }

    return (int)msg.wParam;
}

UEngineWindow::UEngineWindow()
{
}

UEngineWindow::~UEngineWindow()
{
    if (nullptr != WindowHandle)
    {
        DestroyWindow(WindowHandle);
        WindowHandle = nullptr;
    }
}

ENGINE_API void UEngineWindow::Create(std::string_view TitleName, std::string_view ClassName)
{
    if (nullptr != WindowHandle)
    {
        return;
    }

    if (false == WindowClasses.contains(ClassName.data()))
    {
        MSGASSERT(std::string(ClassName) + " 윈도우 클래스가 등록되어 있지 않습니다.");
        return;
    }

    WindowHandle = CreateWindowA(ClassName.data(), TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(TitleName) + " 윈도우 생성에 실패했습니다.");
        return;
    }

    HDC WindowMainDC = GetDC(WindowHandle);
}

ENGINE_API void UEngineWindow::Open(std::string_view TitleName)
{
    if (0 == WindowHandle)
    {
        Create(TitleName);
    }

    if (0 == WindowHandle)
    {
        return;
    }

    ShowWindow(WindowHandle, SW_SHOW);
    UpdateWindow(WindowHandle);
}

void UEngineWindow::SetWindowPosAndScale(FVector Position, FVector Scale)
{
    WindowSize = Scale;
    RECT Rc = { 0, 0, Scale.iX(), Scale.iY() };

    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);
    ::SetWindowPos(WindowHandle, nullptr, Position.iX(), Position.iY(), 
                   Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER);
}

FVector UEngineWindow::GetMousePos()
{
    POINT MousePoint;

    GetCursorPos(&MousePoint);

    ScreenToClient(WindowHandle, &MousePoint);

    return FVector(MousePoint.x, MousePoint.y);
}
