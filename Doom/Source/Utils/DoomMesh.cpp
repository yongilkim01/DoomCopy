#include "pch.h"
#include "DoomMesh.h"

#include "Utils/DoomVertexBuffer.h"
#include "Utils/DoomIndexBuffer.h"

UDoomMesh::UDoomMesh()
{
	VertexBuffer = std::make_shared<FDoomVertexBuffer>();
	IndexBuffer = std::make_shared<FDoomIndexBuffer>();
}

UDoomMesh::~UDoomMesh()
{

}

void UDoomMesh::CreateMesh(DoomVertex* VertexData)
{
}
