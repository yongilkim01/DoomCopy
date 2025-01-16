#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

#include "ThirdParty/FW1FontWrapper/Inc/FW1FontWrapper.h"

/**
 *	����
 */
class UFont : public UResource
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UFont();
	ENGINE_API ~UFont();

	/** ��ü �� ���� ���� */
	UFont(const UFont& Other) = delete;
	UFont(UFont&& Other) noexcept = delete;
	UFont& operator=(const UFont& Other) = delete;
	UFont& operator=(UFont&& Other) noexcept = delete;

protected:

private:

};

