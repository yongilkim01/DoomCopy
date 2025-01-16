#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

#include "ThirdParty/FW1FontWrapper/Inc/FW1FontWrapper.h"

/**
 *	설명
 */
class UFont : public UResource
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UFont();
	ENGINE_API ~UFont();

	/** 객체 값 복사 방지 */
	UFont(const UFont& Other) = delete;
	UFont(UFont&& Other) noexcept = delete;
	UFont& operator=(const UFont& Other) = delete;
	UFont& operator=(UFont&& Other) noexcept = delete;

protected:

private:

};

