#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"
#include "Interfaces/IContentsCore.h"
#include <memory>

class ULevel;

/**
 *	엔진 코어 클래스
 */
class UEngineCore
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UEngineCore();
	ENGINE_API virtual ~UEngineCore() = 0;

	/**
	 *	엔진 시작 메소드
	 *  @param Instance: 엔진을 생성할 HINSTANCE 정보를 담고 있는 객체
	 *  @param DllName: 콘텐츠 동적 라이브러리 파일 이름
	 */
	ENGINE_API static void EngineStart(HINSTANCE Instance, std::string_view DllName);
	/**
	 *	레벨 생성 메소드
	 *  @param Name: 생성할 레벨 이름, 레벨 이름은 중복 불가능
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
	 *	엔진이 끝날 때 실행되는 메소드
	 */
	static void EngineEnd();
	/**
	 *	레벨 생성 구현부 메소드
	 *  @param Name: 생성할 레벨 이름
	 */
	ENGINE_API static std::shared_ptr<ULevel> NewLevelCreate(std::string_view Name);

	static UEngineWindow MainWindow;
	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;
	static std::map<std::string, std::shared_ptr<ULevel>> Levels;

};

