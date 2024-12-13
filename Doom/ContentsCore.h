#pragma once
#include "Interfaces/IContentsCore.h"

/**
 *	설명
 */
class UContentsCore : public IContentsCore
{
public:
	/** 생성자, 소멸자 */
	UContentsCore();
	~UContentsCore();

	/** 객체 값 복사 방지 */
	UContentsCore(const UContentsCore& Other) = delete;
	UContentsCore(UContentsCore&& Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& Other) = delete;
	UContentsCore& operator=(UContentsCore&& Other) noexcept = delete;

	/** 클래스 메소드 */

	/** 겟, 셋 메소드 */

protected:
	/** IContentsCore 인터페이스 메소드 */
	void EngineStart(UEngineInitData& Data);
	void EngineTick(float DeltaTime);
	void EngineEnd();

private:
	const float WindowWidth = 1280;
	const float WindowHeight = 720;
};