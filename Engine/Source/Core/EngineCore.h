#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Renderer/EngineGraphicDevice.h"
#include "Interfaces/IContentsCore.h"
#include "Classes/Engine/Level.h"

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

		NewLevel->SpawnActor<GameModeType>();
		NewLevel->SpawnActor<MainPawnType>();

		return NewLevel;
	}

	/** ��, �� �޼ҵ� */
	ENGINE_API static UEngineGraphicDevice Device;
	ENGINE_API static FVector GetSceenScale();

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

	static std::map<std::string, std::shared_ptr<ULevel>> LevelMap;
	static std::shared_ptr<ULevel> CurLevel;
	static std::shared_ptr<ULevel> NextLevel;

};

