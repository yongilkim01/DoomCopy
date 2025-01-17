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
 *	엔진 코어 클래스
 */
class UGameEngine
{
public:
	/**
	 *	엔진 시작 메소드
	 *  @param Instance: 엔진을 생성할 HINSTANCE 정보를 담고 있는 객체
	 *  @param DllName: 콘텐츠 동적 라이브러리 파일 이름
	 */
	ENGINE_API static void EngineStart(HINSTANCE Instance, std::string_view DllName);
	/**
	 *	레벨을 여는 메소드
	 *  @param LevelName: 오픈하려는 레벨 이름
	 */
	ENGINE_API static void OpenLevel(std::string_view LevelName);
	/**
	 *	렌더 시작 메소드
	 */
	ENGINE_API static void StartRender();
	/**
	 *	렌더 마무리 메소드
	 */
	ENGINE_API static void EndRender();
	/**
	 *	그래픽 디바이스 백버퍼 렌더타겟 반환 메소드
	 */
	ENGINE_API static std::shared_ptr<URenderTarget> GetBackBufferRenderTarget();
	/**
	 *	레벨 생성 메소드
	 *  @param Name: 생성할 레벨 이름, 레벨 이름은 중복 불가능
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
	 *	게임 인스턴스 생성 메소드
	 *	언리얼 엔진에서는 에디터에서 생성
	 */
	template<typename GameInstanceType>
	ENGINE_API static void CreateGameInstance()
	{
		SetGameInstance(std::make_shared<GameInstanceType>());
	}

	/** 겟, 셋 메소드 */
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
	 *	레벨 생성 구현부 메소드
	 *  @param Name: 생성할 레벨 이름
	 */
	ENGINE_API static std::shared_ptr<ULevel> NewLevelCreate(std::string_view Name);
	/**
	 *	윈도우 창 초기화 메소드
	 *  @param Instance: 윈도우 초기화 정보를 담고 있는 HINSTANCE 객체
	 */
	static void WindowInit(HINSTANCE Instance);
	/**
	 *	콘텐츠에 따른 엔진 초기화 과정을 진행하는 메소드
	 *  @param DllName: 콘텐츠 동적 라이브러리 파일 이름
	 */
	static void LoadContents(std::string_view DllName);
	/**
	 *	엔진 프레임 메소드
	 */
	static void EngineFrame();
	/**
	 *	엔진이 끝날 때 실행되는 메소드
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
