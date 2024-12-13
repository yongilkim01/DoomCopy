#pragma once
#include "Interfaces/IContentsCore.h"

/**
 *	����
 */
class UContentsCore : public IContentsCore
{
public:
	/** ������, �Ҹ��� */
	UContentsCore();
	~UContentsCore();

	/** ��ü �� ���� ���� */
	UContentsCore(const UContentsCore& Other) = delete;
	UContentsCore(UContentsCore&& Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& Other) = delete;
	UContentsCore& operator=(UContentsCore&& Other) noexcept = delete;

	/** Ŭ���� �޼ҵ� */

	/** ��, �� �޼ҵ� */

protected:
	/** IContentsCore �������̽� �޼ҵ� */
	void EngineStart(UEngineInitData& Data);
	void EngineTick(float DeltaTime);
	void EngineEnd();

private:
	const float WindowWidth = 1280;
	const float WindowHeight = 720;
};