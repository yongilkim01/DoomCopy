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
	float4 TEXCOORD; // UV���̶�� �Ҹ��� ����� �ؽ�ó�� ���εǴ� ������ ����
	float4 COLOR;

	float4 NORMAL; // ���� ���� ���͸� ���� ��
	float4 BINORMAL; // ���� ������ ���� �����Ͱ� ź��Ʈ ������ ����� ���� ��
	float4 TANGENT; // ���� ������ ���� �����Ͱ� ź��Ʈ ������ ����� ���� ��
};