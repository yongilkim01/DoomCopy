#pragma once
#include "Engine/Classes/Interfaces/IContentsCore.h"


/**
 *	설명
 */
class UDoomCore : public IContentsCore
{
public:
	/** 생성자, 소멸자 */
	UDoomCore();
	~UDoomCore();

	/** 객체 값 복사 방지 */
	UDoomCore(const UDoomCore& Other) = delete;
	UDoomCore(UDoomCore&& Other) noexcept = delete;
	UDoomCore& operator=(const UDoomCore& Other) = delete;
	UDoomCore& operator=(UDoomCore&& Other) noexcept = delete;

	/** 클래스 메소드 */
	void InitWindowSize(UEngineInitData& Data);
	void InitContentsInfo(UEngineInitData& Data);

	/** 겟, 셋 메소드 */

protected:
	/** IContentsCore 인터페이스 메소드 */
	void EngineStart(UEngineInitData& Data);
	void EngineAssetLoad();
	void EngineLevelStart();
	void EngineTick(float DeltaTime);
	void EngineEnd();

	void InitContentsRenderingResource();

private:
	const float WindowWidth = 1280;
	const float WindowHeight = 720;
	const std::string ContentsName = "Doom";
};