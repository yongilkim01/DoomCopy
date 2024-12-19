#include "pch.h"
#include "Renderer.h"

#include "Classes/Engine/Level.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::BeginPlay()
{
	SetOrder(0);
}

void URenderer::Render(float DeltaTime)
{
	// TODO: Rendering pipeline
}

void URenderer::InputAssembler1Init()
{
	std::vector<EngineVertex> Vertexes;
	Vertexes.resize(6);

	Vertexes[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.5f), {} };
	Vertexes[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.5f), {} };
	Vertexes[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.5f), {} };
	Vertexes[3] = EngineVertex{ FVector(0.5f, 0.5f, -0.5f), {} };
	Vertexes[4] = EngineVertex{ FVector(0.5f, -0.5f, -0.5f), {} };
	Vertexes[5] = EngineVertex{ FVector(-0.5f, -0.5f, -0.5f), {} };

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeof(EngineVertex) * Vertexes.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&desc, &Data, &VertexBuffer))
	{
		MSGASSERT("버텍스 버퍼 생성 실패");
		return;
	}

}

void URenderer::InputAssembler1Setting()
{
}

void URenderer::SetOrder(int NewOrder)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(PrevOrder, RendererPtr);
}