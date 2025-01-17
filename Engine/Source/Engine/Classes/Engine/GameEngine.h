#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Rendering/EngineGraphicDevice.h"
#include "Engine/Classes/Interfaces/IContentsCore.h"
#include "Engine/Classes/Engine/Level.h"
#include "Core/Public/Time/Timer.h"
#include "Core/Public/Containers/EngineString.h"

#include <memory>

class UGameInstance;

/**
 *	���� �ھ� Ŭ����
 */
class UGameEngine
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
	 */
	ENGINE_API static void StartRender();
	/**
	 *	���� ������ �޼ҵ�
	 */
	ENGINE_API static void EndRender();
	/**
	 *	�׷��� ����̽� ����� ����Ÿ�� ��ȯ �޼ҵ�
	 */
	ENGINE_API static std::shared_ptr<URenderTarget> GetBackBufferRenderTarget();
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
	/**
	 *	���� �ν��Ͻ� ���� �޼ҵ�
	 *	�𸮾� ���������� �����Ϳ��� ����
	 */
	template<typename GameInstanceType>
	ENGINE_API static void CreateGameInstance()
	{
		SetGameInstance(std::make_shared<GameInstanceType>());
	}

	/** ��, �� �޼ҵ� */
	ENGINE_API static UGameInstance* GetGameInstance();
	ENGINE_API static ID3D11Device* GetDevice();
	ENGINE_API static ID3D11DeviceContext* GetDeviceContext();
	ENGINE_API static FVector GetSceenScale();
	ENGINE_API static UEngineWindow& GetMainWindow();
	ENGINE_API static std::map<std::string, std::shared_ptr<ULevel>> GetAllLevelMap();
	ENGINE_API void SetContentsProjectName(std::string_view Value)
	{
		ContentsProjectName = Value;
	}
	ENGINE_API std::string GetContentsProjectName()
	{
		return ContentsProjectName;
	}

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
	std::shared_ptr<IContentsCore> ContentsCore;
	UEngineInitData InitData;

	UEngineTimer Timer;

	std::map<std::string, std::shared_ptr<ULevel>> LevelMap;
	std::shared_ptr<ULevel> CurLevel;
	std::shared_ptr<ULevel> NextLevel;

	std::string ContentsProjectName = "";

	std::shared_ptr<UGameInstance> GameInstance;

	ENGINE_API UGameEngine();
	ENGINE_API virtual ~UGameEngine();

	ENGINE_API static void SetGameInstance(std::shared_ptr<UGameInstance> Instance);
};

ENGINE_API extern class UGameEngine* GEngine;
