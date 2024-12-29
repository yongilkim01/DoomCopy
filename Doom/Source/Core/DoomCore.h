#pragma once
#include "Interfaces/IContentsCore.h"


/**
 *	����
 */
class UDoomCore : public IContentsCore
{
public:
	/** ������, �Ҹ��� */
	UDoomCore();
	~UDoomCore();

	/** ��ü �� ���� ���� */
	UDoomCore(const UDoomCore& Other) = delete;
	UDoomCore(UDoomCore&& Other) noexcept = delete;
	UDoomCore& operator=(const UDoomCore& Other) = delete;
	UDoomCore& operator=(UDoomCore&& Other) noexcept = delete;

	/** Ŭ���� �޼ҵ� */
	void InitWindowSize(UEngineInitData& Data);

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