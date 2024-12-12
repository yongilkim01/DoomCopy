#pragma once
#include "EngineDefine.h"

// Ό³Έν :
class UEngineCore
{
public:
	// constrcuter destructer
	ENGINE_API UEngineCore();
	ENGINE_API ~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

protected:

private:

};

