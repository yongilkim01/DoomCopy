#pragma once
#include "Engine/Classes/Interfaces/IContentsCore.h"


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
	void InitContentsInfo(UEngineInitData& Data);

	/** ��, �� �޼ҵ� */

protected:
	/** IContentsCore �������̽� �޼ҵ� */
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