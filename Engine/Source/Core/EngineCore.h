#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Interfaces/IContentsCore.h"
#include <memory>

class ULevel;

// Ό³Έν :
class UEngineCore
{
public:
	// constrcuter destructer
	ENGINE_API UEngineCore();
	ENGINE_API virtual ~UEngineCore() = 0;

	ENGINE_API static void EngineStart(HINSTANCE Instance, std::string_view DllName);

	template<typename GameModeType, typename MainPawnType>
	static class std::shared_ptr<ULevel> CreateLevel(std::string_view Name)
	{
		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(Name);

		return NewLevel;

	}

protected:

private:
	static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;

	static void WindowInit(HINSTANCE Instance);
	static void LoadContents(std::string_view DllName);
	static void EngineEnd();
	ENGINE_API static std::shared_ptr<ULevel> NewLevelCreate(std::string_view Name);

	static std::map<std::string, std::shared_ptr<ULevel>> Levels;

};

