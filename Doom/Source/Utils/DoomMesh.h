#pragma once
#include "DoomVertexBuffer.h"
#include "DoomIndexBuffer.h"

/**
 *	DoomMesh File class
 */
class UDoomMesh
{
public:
	/** ������, �Ҹ��� */
	UDoomMesh();
	~UDoomMesh();

	/** ��ü �� ������� */
	UDoomMesh(const UDoomMesh& Other) = delete;
	UDoomMesh(UDoomMesh&& Other) noexcept = delete;
	UDoomMesh& operator=(const UDoomMesh& Other) = delete;
	UDoomMesh& operator=(UDoomMesh&& Other) noexcept = delete;

	/** Ŭ���� �޼ҵ� */
	void CreateMesh(DoomVertex* VertexData);

	/** ��, �� �޼ҵ� */

protected:


private:
	std::shared_ptr<FDoomVertexBuffer> VertexBuffer;
	std::shared_ptr<FDoomIndexBuffer> IndexBuffer;

	size_t IndexSize = 0;
};

