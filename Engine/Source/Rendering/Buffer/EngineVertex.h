#pragma once
#include "Core/Math/EngineMath.h"
#include "Rendering/Buffer/EngineInputLayoutInfo.h"

class EngineVertexInit;

struct EngineVertex
{
	friend EngineVertexInit;

	ENGINE_API static UEngineInputLayoutInfo Info;

	ENGINE_API static UEngineInputLayoutInfo& GetInfo();

	float4 POSITION;
	float4 TEXCOORD; // UV값이라고 불리는 존재로 텍스처가 매핑되는 비율을 지정해줍니다.
	float4 COLOR;
};