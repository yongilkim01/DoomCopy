#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Interfaces/IContentsCore.h"
#include <memory>

class ULevel;

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
	 *	���� ���� �޼ҵ�
	 *  @param Name: ������ ���� �̸�, ���� �̸��� �ߺ� �Ұ���
	 */
	template<typename GameModeType, typename MainPawnType>
	static class std::shared_ptr<ULevel> CreateLevel(std::string_view Name)
	{
		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(Name);

		return NewLevel;
	}

protected:

private:
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
	 *	������ ���� �� ����Ǵ� �޼ҵ�
	 */
	static void EngineEnd();
	/**
	 *	���� ���� ������ �޼ҵ�
	 *  @param Name: ������ ���� �̸�
	 */
	ENGINE_API static std::shared_ptr<ULevel> NewLevelCreate(std::string_view Name);

	static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;
	static std::map<std::string, std::shared_ptr<ULevel>> Levels;

};

