#pragma once
#include "Classes/Engine/RenderAsset.h"

/**
 *	설명
 */
class UEngineRasterizerState : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	UEngineRasterizerState();
	~UEngineRasterizerState();

	/** 객체 값 복사 방지 */
	UEngineRasterizerState(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState(UEngineRasterizerState&& Other) noexcept = delete;
	UEngineRasterizerState& operator=(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState& operator=(UEngineRasterizerState&& Other) noexcept = delete;

protected:

private:

};

