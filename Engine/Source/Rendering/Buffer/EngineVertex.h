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
	float4 TEXCOORD; // UV���̶�� �Ҹ��� ����� �ؽ�ó�� ���εǴ� ������ �������ݴϴ�.
	float4 COLOR;
};