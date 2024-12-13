#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Interfaces/IContentsCore.h"
#include <memory>

// Ό³Έν :
class UEngineCore
{
public:
	// constrcuter destructer
	ENGINE_API UEngineCore();
	ENGINE_API virtual ~UEngineCore() = 0;

	ENGINE_API static void EngineStart(HINSTANCE Instance, std::string_view DllName);

protected:

private:
	static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;

	static void WindowInit(HINSTANCE Instance);
	static void LoadContents(std::string_view DllName);

};

