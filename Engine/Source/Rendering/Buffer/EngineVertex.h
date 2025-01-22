#pragma once
#include "Core/Public/Math/EngineMath.h"
#include "Rendering/Buffer/EngineInputLayoutInfo.h"

class EngineVertexInit;

struct EngineVertex
{
	friend EngineVertexInit;

	ENGINE_API static UEngineInputLayoutInfo Info;

	ENGINE_API static UEngineInputLayoutInfo& GetInfo();

	float4 POSITION;
	float4 TEXCOORD; // UV값이라고 불리는 존재로 텍스처가 매핑되는 비율을 지정
	float4 COLOR;

	float4 NORMAL; // 면의 법선 백터를 위한 값
	float4 BINORMAL; // 범프 매핑을 위한 데이터가 탄젠트 공간을 만들기 위한 값
	float4 TANGENT; // 범프 매핑을 위한 데이터가 탄젠트 공간을 만들기 위한 값
};