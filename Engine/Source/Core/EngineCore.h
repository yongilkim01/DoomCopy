#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Rendering/EngineGraphicDevice.h"
#include "Interfaces/IContentsCore.h"
#include "Classes/Engine/Level.h"
#include "Time/Timer.h"
#include "Core/Containers/EngineString.h"

#include <memory>

/**
 *	���� �ھ� Ŭ����
 */
class UEngineCore
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UEngineCore();
	ENGINE_API virtual ~UEngineCore() = 0;

	/**
	 *	���� ���� �޼ҵ�
	 *  @param Instance: ������ ������ HINSTANCE ������ ��� �ִ� ��ü
	 *  @param DllName: ������ ���� ���̺귯�� ���� �̸�
	 */
	ENGINE_API static void EngineStart(HINSTANCE Instance, std::string_view DllName);
	/**
	 *	������ ���� �޼ҵ�
	 *  @param LevelName: �����Ϸ��� ���� �̸�
	 */
	ENGINE_API static void OpenLevel(std::string_view LevelName);
	/**
	 *	���� ���� �޼ҵ�
	 *  @param Name: ������ ���� �̸�, ���� �̸��� �ߺ� �Ұ���
	 */
	template<typename GameModeType, typename MainPawnType>
	static class std::shared_ptr<ULevel> CreateLevel(std::string_view Name)
	{
		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(Name);

		std::shared_ptr<GameModeType> InitGameMode = NewLevel->SpawnActor<GameModeType>();
		std::shared_ptr<MainPawnType> InitPawn = NewLevel->SpawnActor<MainPawnType>();

		NewLevel->InitLevel(InitGameMode.get(), InitPawn.get());

		return NewLevel;
	}

	/** ��, �� �޼ҵ� */
	ENGINE_API static UEngineGraphicDevice& GetDevice();
	ENGINE_API static FVector GetSceenScale();
	ENGINE_API static UEngineWindow& GetMainWindow();

protected:

private:
	/**
	 *	���� ���� ������ �޼ҵ�
	 *  @param Name: ������ ���� �̸�
	 */
	ENGINE_API static std::shared_ptr<ULevel> NewLevelCreate(std::string_view Name);
	/**
	 *	������ â �ʱ�ȭ �޼ҵ�
	 *  @param Instance: ������ �ʱ�ȭ ������ ��� �ִ� HINSTANCE ��ü
	 */
	static void WindowInit(HINSTANCE Instance);
	/**
	 *	�������� ���� ���� �ʱ�ȭ ������ �����ϴ� �޼ҵ�
	 *  @param DllName: ������ ���� ���̺귯�� ���� �̸�
	 */
	static void LoadContents(std::string_view DllName);
	/**
	 *	���� ������ �޼ҵ�
	 */
	static void EngineFrame();
	/**
	 *	������ ���� �� ����Ǵ� �޼ҵ�
	 */
	static void EngineEnd();

	ENGINE_API static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;
	static UEngineInitData InitData;

	static UEngineTimer Timer;

	static std::map<std::string, std::shared_ptr<ULevel>> LevelMap;
	static std::shared_ptr<ULevel> CurLevel;
	static std::shared_ptr<ULevel> NextLevel;

	ENGINE_API static UEngineGraphicDevice Device;

};

