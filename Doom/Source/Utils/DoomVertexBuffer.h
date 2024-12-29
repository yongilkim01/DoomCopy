#pragma once
#include "Core/Math/EngineMath.h"

struct DoomVertex
{
	float4 POSITION;
};

/**
 *	DoomVertexBuffer File class
 */
class FDoomVertexBuffer
{
public:
	/** ������, �Ҹ��� */
	FDoomVertexBuffer();
	~FDoomVertexBuffer();

	/** ��ü �� ������� */
	FDoomVertexBuffer(const FDoomVertexBuffer& Other) = delete;
	FDoomVertexBuffer(FDoomVertexBuffer&& Other) noexcept = delete;
	FDoomVertexBuffer& operator=(const FDoomVertexBuffer& Other) = delete;
	FDoomVertexBuffer& operator=(FDoomVertexBuffer&& Other) noexcept = delete;

	/** ��, �� �޼ҵ� */

protected:


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	D3D11_BUFFER_DESC VertexBufferDesc = { 0 };
	int VertexSize = 0;
	int VertexCount = 0;
};

