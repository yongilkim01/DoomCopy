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
	/** 생성자, 소멸자 */
	FDoomVertexBuffer();
	~FDoomVertexBuffer();

	/** 객체 값 복사방지 */
	FDoomVertexBuffer(const FDoomVertexBuffer& Other) = delete;
	FDoomVertexBuffer(FDoomVertexBuffer&& Other) noexcept = delete;
	FDoomVertexBuffer& operator=(const FDoomVertexBuffer& Other) = delete;
	FDoomVertexBuffer& operator=(FDoomVertexBuffer&& Other) noexcept = delete;

	/** 겟, 셋 메소드 */

protected:


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	D3D11_BUFFER_DESC VertexBufferDesc = { 0 };
	int VertexSize = 0;
	int VertexCount = 0;
};

