#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Rendering/EngineGraphicDevice.h"
#include "Interfaces/IContentsCore.h"
#include "Engine/Classes/Engine/Level.h"
#include "Core/Public/Time/Timer.h"
#include "Core/Public/Containers/EngineString.h"
#include <memory>

/**
 *	���� �ھ� Ŭ����
 */
class UEngineCore
{
public:
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
	template<typename GameModeType, typename MainPawnType, typename HUDType>
	static class std::shared_ptr<ULevel> CreateLevel(std::string_view Name)
	{
		std::string UpperLevelName = UEngineString::ToUpper(Name);

		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(UpperLevelName);

		std::shared_ptr<GameModeType> InitGameMode = NewLevel->SpawnActor<GameModeType>();
		std::shared_ptr<MainPawnType> InitPawn = NewLevel->SpawnActor<MainPawnType>();
		std::shared_ptr<HUDType> InitHUD = NewLevel->SpawnActor<HUDType>();

		NewLevel->InitLevel(InitGameMode.get(), InitPawn.get(), InitHUD.get());

		return NewLevel;
	}

	/** ��, �� �޼ҵ� */
	ENGINE_API static UEngineGraphicDevice& GetDevice();
	ENGINE_API static FVector GetSceenScale();
	ENGINE_API static UEngineWindow& GetMainWindow();
	ENGINE_API static std::map<std::string, std::shared_ptr<ULevel>> GetAllLevelMap();

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

	UEngineWindow MainWindow;
	UEngineGraphicDevice Device;

	HMODULE ContentsDLL;
	std::shared_ptr<IContentsCore> Core;
	UEngineInitData InitData;

	UEngineTimer Timer;

	std::map<std::string, std::shared_ptr<ULevel>> LevelMap;
	std::shared_ptr<ULevel> CurLevel;
	std::shared_ptr<ULevel> NextLevel;

	ENGINE_API UEngineCore();
	ENGINE_API virtual ~UEngineCore();
};

ENGINE_API extern class UEngineCore* GEngine;
