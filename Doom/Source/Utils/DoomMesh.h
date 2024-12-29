#pragma once
#include "DoomVertexBuffer.h"
#include "DoomIndexBuffer.h"

/**
 *	DoomMesh File class
 */
class UDoomMesh
{
public:
	/** 생성자, 소멸자 */
	UDoomMesh();
	~UDoomMesh();

	/** 객체 값 복사방지 */
	UDoomMesh(const UDoomMesh& Other) = delete;
	UDoomMesh(UDoomMesh&& Other) noexcept = delete;
	UDoomMesh& operator=(const UDoomMesh& Other) = delete;
	UDoomMesh& operator=(UDoomMesh&& Other) noexcept = delete;

	/** 클래스 메소드 */
	void CreateMesh(DoomVertex* VertexData);

	/** 겟, 셋 메소드 */

protected:


private:
	std::shared_ptr<FDoomVertexBuffer> VertexBuffer;
	std::shared_ptr<FDoomIndexBuffer> IndexBuffer;

	size_t IndexSize = 0;
};

