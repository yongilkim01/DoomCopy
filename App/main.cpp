#include <Core/Math/EngineMath.h>
#include <Core/EngineCore.h>

#pragma comment(lib, "Engine.lib")

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPWSTR    _lpCmdLine,
	_In_ int       _nCmdShow)
{
	UEngineCore::EngineStart(_hInstance, "Doom.dll");
	return 0;
}