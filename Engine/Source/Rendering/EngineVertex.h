#pragma once
#include "Core/Math/EngineMath.h"
#include "Rendering/EngineInputLayoutInfo.h"

class EngineVertexInit;

struct EngineVertex
{
	friend EngineVertexInit;

	static UEngineInputLayoutInfo Info;

	float4 POSITION;
	float4 TEXCOORD; // UV���̶�� �Ҹ��� ����� �ؽ�ó�� ���εǴ� ������ �������ݴϴ�.
	float4 COLOR;
};