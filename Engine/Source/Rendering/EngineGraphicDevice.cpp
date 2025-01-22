#include "pch.h"
#include "EngineGraphicDevice.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/Buffer/VertexBuffer.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/EngineBlend.h"
#include "Rendering/Shader/EngineShader.h"
#include "Rendering/Buffer/DepthStencilState.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"

#include "Engine/Public/Materials/Material.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/Font.h"

#include "NavigationSystem/Public/NavigationSystem.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{

	Release();
}

void UEngineGraphicDevice::Release()
{
	MainAdapter = nullptr;
	SwapChain = nullptr;
	DeviceContext = nullptr;
	Device = nullptr;
}


void UEngineGraphicDevice::InitDefaultResources()
{
	InitDepthStencil();
	InitTexture();
	InitMesh();
	InitBlend();
	InitRasterizerState();
	InitShader();
	InitMaterial();
	InitFont();
}

void UEngineGraphicDevice::InitTexture()
{
	// Sampler �ʱ�ȭ �� ����
	{
		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };

		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		// SampInfo.MaxLOD = 0.0f;
		// SampInfo.MinLOD = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);
	}

	// ���� �⺻ �ؽ��� ����
	{
		FDirectoryHelper CurDir;

		CurDir.MoveEngineShaderDirectory();

		if (false == CurDir.MoveEngineShaderDirectory())
		{
			MSGASSERT("���� ���̴� ���� ã�⿡ �����߽��ϴ�");
			return;
		}

		std::vector<FFileHelper> ImageFiles = CurDir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

		for (int i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UTexture::Load(FilePath);
		}
	}
}

void UEngineGraphicDevice::InitMesh()
{
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(4);
		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		FVertexBuffer::Create("Rect", Vertexs);

		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		Indexs.push_back(1);
		Indexs.push_back(3);
		Indexs.push_back(2);

		FIndexBuffer::Create("Rect", Indexs);

		UMesh::Create("Rect");
	}
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(3);

		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(0.0f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };


		FVertexBuffer::Create("Triangle", Vertexs);

		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		FIndexBuffer::Create("Triangle", Indexs);

		UMesh::Create("Triangle");
	}
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(4);

		Vertexs[0] = EngineVertex{ FVector(-1.0f, 1.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(1.0f, 1.0f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-1.0f, -1.0f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(1.0f, -1.0f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };
		
		FVertexBuffer::Create("FullRect", Vertexs);

		UMesh::Create("FullRect", "FullRect", "Rect");
	}

	{
		std::vector<EngineVertex> Vertex;
		Vertex.resize(24);
		// �ո�
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		// �޸�
		Vertex[4] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(180) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[5] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(180)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[6] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(180) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[7] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(180), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		// ����
		Vertex[8] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[9] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[10] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[11] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		// ������
		Vertex[12] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[13] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[14] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[15] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		// ��
		Vertex[16] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[17] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[18] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[19] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		// �Ʒ�
		Vertex[20] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[21] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[22] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[23] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		
		FVertexBuffer::Create("Box", Vertex);

		std::vector<int> Index;
		// Index.resize(36);
		for (int i = 0; i < 6; i++)
		{
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 1);
			Index.push_back(i * 4 + 0);
			Index.push_back(i * 4 + 3);
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 0);
		}

		FIndexBuffer::Create("Box", Index);
		UMesh::Create("Box", "Box", "Box");
	}

	// Sphere
	{
		EngineVertex VertexData; // ������ �����ϱ� ���� ����ü
		std::vector<EngineVertex> VBVector; // ���� �����͸� �����ϴ� ����
		std::vector<UINT> IBVector; // �ε��� �����͸� �����ϴ� ����
		float Radius = 0.5f; // ���� ������

		// �ϱ��� (���� ���� ���κ�)�� ���� �����͸� �����մϴ�.
		VertexData.POSITION = float4(0.0f, Radius, 0.0f, 1.0f); // ��ġ ���� (y������ ��������ŭ ������)
		VertexData.TEXCOORD = float4(0.5f, 0.0f); // �ؽ�ó ��ǥ ���� (�߾� ����)

		// ���� ���͸� ���� (�鿡 �������� �� �ݻ縦 ����ϱ� ����)
		VertexData.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		VertexData.NORMAL.Normalize(); // ���͸� ����ȭ (���̸� 1�� ����)
		VertexData.NORMAL.W = 0.0f; // W ������ 0���� ���� (��ġ�� ����)

		// ���� ���Ϳ� ��븻 ���͸� ���� (������ ������ ���͵�� ǥ�� �������� ǥ���ϱ� ����)
		VertexData.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f); // ���� ����
		VertexData.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f); // ��븻 ����

		VBVector.push_back(VertexData); // ���� �����͸� ���Ϳ� �߰�

		// ���ð� �����̽��� ������ ���� (���� �����ϴ� ���� �� ���� ����)
		UINT iStackCount = 16; // ���� ���� ����
		UINT iSliceCount = 16; // ���� ���� ����

		// �� ���ð� �����̽� ������ ȸ�� ������ ���
		float yRotAngle = FMath::PI / (float)iStackCount; // ���� ���� ȸ�� ����
		float zRotAngle = FMath::PI2 / (float)iSliceCount; // ���� ���� ȸ�� ����

		// UV �ؽ�ó ��ǥ�� ������ ���
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iSliceCount;

		// �ϱؿ��� ���ر��� ���� �����͸� ����
		for (UINT y = 1; y < iStackCount; ++y) // �ϱذ� ���� ����
		{
			float phi = y * yRotAngle; // ���� ������ ���� ���� ����
			for (UINT z = 0; z < iSliceCount + 1; ++z) // �����̽� ������ŭ
			{
				float theta = z * zRotAngle; // ���� �����̽��� ���� ���� ����

				// ���� ��ġ�� ��� (���� ��ǥ)
				VertexData.POSITION = float4{
					Radius * sinf(phi) * cosf(theta),
					Radius * cosf(phi),
					Radius * sinf(phi) * sinf(theta),
					1.0f
				};

				// �ؽ�ó ��ǥ�� ����
				VertexData.TEXCOORD = float4(yUvRatio * z, zUvRatio * y);

				// ���� ���͸� ����ȭ
				VertexData.NORMAL = VertexData.POSITION.NormalizeReturn();
				VertexData.NORMAL.W = 0.0f;

				// ���� ���� ����
				VertexData.TANGENT.X = -Radius * sinf(phi) * sinf(theta);
				VertexData.TANGENT.Y = 0.0f;
				VertexData.TANGENT.Z = Radius * sinf(phi) * cosf(theta);
				VertexData.TANGENT = VertexData.TANGENT.NormalizeReturn();
				VertexData.TANGENT.W = 0.0f;

				// ��븻 ���� ����
				VertexData.BINORMAL = float4::Cross(VertexData.TANGENT, VertexData.NORMAL);
				VertexData.BINORMAL = VertexData.BINORMAL.NormalizeReturn();
				VertexData.BINORMAL.W = 0.0f;

				// ������ ������ ���Ϳ� �߰�
				VBVector.push_back(VertexData);
			}
		}

		// ������ (���� ���� �Ʒ� �κ�)�� ���� �����͸� ����
		VertexData.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f); // ��ġ ���� (y������ ��������ŭ �Ʒ�)
		VertexData.TEXCOORD = float4(0.5f, 1.0f); // �ؽ�ó ��ǥ ���� (�߾� �Ʒ���)
		VertexData.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		VertexData.NORMAL.Normalize();
		VertexData.NORMAL.W = 0.0f;
		VertexData.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		VertexData.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(VertexData);

		// �ε��� �����͸� �����Ͽ� �ﰢ�� ���� ����
		IBVector.clear();

		// �ϱ����� �������� �ﰢ�� ����
		for (UINT i = 0; i < iSliceCount; i++)
		{
			IBVector.push_back(0); // �ϱ���
			IBVector.push_back(i + 2); // ���� �����̽��� ��
			IBVector.push_back(i + 1); // ���� �����̽��� ��
		}

		// ���� ������ �簢�� ���� �� ���� �ﰢ������ ����
		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			}
		}

		// �������� �������� �ﰢ�� ����
		UINT iBotIndex = (UINT)VBVector.size() - 1; // �������� �ε���
		for (UINT i = 0; i < iSliceCount; i++)
		{
			IBVector.push_back(iBotIndex); // ������
			IBVector.push_back(iBotIndex - (i + 2)); // ���� �����̽��� ��
			IBVector.push_back(iBotIndex - (i + 1)); // ���� �����̽��� ��
		}

		// ������ ���� ���ۿ� �ε��� ���۸� Ȱ���� ���� ����
		FVertexBuffer::Create("Sphere", VBVector);
		FIndexBuffer::Create("Sphere", IBVector);

		UMesh::Create("Sphere");
	}


}

void UEngineGraphicDevice::InitBlend()
{
	D3D11_BLEND_DESC BlendDesc = { 0 };

	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = true;
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	UEngineBlend::Create("AlphaBlend", BlendDesc);
}

void UEngineGraphicDevice::InitShader()
{
	FDirectoryHelper CurDir;
	CurDir.MoveEngineShaderDirectory();

	std::vector<FFileHelper> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}
}

void UEngineGraphicDevice::InitMaterial()
{
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");

		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetBlend("AlphaBlend");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("WidgetMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetDepthStencilState("UIDepth");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("CollisionDebugShader.fx");
		Mat->SetPixelShader("CollisionDebugShader.fx");
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("RenderTargetMaterial");
		Mat->SetVertexShader("RenderTargetMergeShader.fx");
		Mat->SetPixelShader("RenderTargetMergeShader.fx");
		Mat->SetDepthStencilState("RenderTargetMergeDepth");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("LightMaterial");
		Mat->SetVertexShader("MeshLightShader.fx");
		Mat->SetPixelShader("MeshLightShader.fx");
	}
}

void UEngineGraphicDevice::InitRasterizerState()
{
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		UEngineRasterizerState::Create("EngineBase", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		UEngineRasterizerState::Create("CollisionDebugRas", Desc);
	}
}

void UEngineGraphicDevice::InitDepthStencil()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("BaseDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("UIDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("CollisionDebugDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		UDepthStencilState::Create("RenderTargetMergeDepth", Desc);
	}
}

void UEngineGraphicDevice::InitFont()
{
	UFont::Load("�ü�", "�ü�");
}

void UEngineGraphicDevice::RenderStart()
{
	BackBufferRenderTarget->Clear();
	BackBufferRenderTarget->Setting();
}

void UEngineGraphicDevice::RenderEnd()
{
	HRESULT hr = SwapChain->Present(0, 0);

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		MSGASSERT("�ػ� �����̳� ����̽� ���� ���� ��Ÿ�� ���� ����");
		return;
	}
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
	MainAdapter = GetHighPerformanceAdapter();

	int IFlag = 0;

#ifdef _DEBUG
	IFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL ResultLevl;

	HRESULT HR = D3D11CreateDevice(
		MainAdapter.Get(),
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,	// ���� �������� Adapter�� ����̽��� ����
		nullptr,									// Ư�� �ܰ�(�����Ͷ������̼�: ������ ���������� �Ϻ�)�� �ۼ��� DLL�� ��ü
		IFlag,
		nullptr,									// DirectX Feature Level ����
		0,											// �����ϴ� Feature Level ����
		D3D11_SDK_VERSION,
		&Device,
		&ResultLevl,
		&DeviceContext);

	if (nullptr == Device)
	{
		MSGASSERT("�׷��� ����̽� ������ �����߽��ϴ�.");
		return;
	}

	if (nullptr == DeviceContext)
	{
		MSGASSERT("�׷��� ���ؽ�Ʈ ����̽� ������ �����߽��ϴ�.");
		return;
	}

	HR = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (HR != S_OK)
	{
		MSGASSERT("��Ƽ������ ���� ����");
		return;
	}

	InitDefaultResources();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& EngineWindow)
{
	FVector Size = EngineWindow.GetWindowSize();

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 2;
	desc.BufferDesc.Width = Size.iX(); 
	desc.BufferDesc.Height = Size.iY();
	desc.OutputWindow = EngineWindow.GetWindowHandle();
	desc.Windowed = true;
	desc.BufferDesc.RefreshRate.Denominator = 1;									// �ֻ���
	desc.BufferDesc.RefreshRate.Numerator = 60;										// 1�ʿ� 60ȸ ������ �� �� ������ �ض�
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// ����� �� ����
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// ����ͳ� �����쿡 �ȼ��� ���ŵǴ� ������ ���� �����ɷ� ����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;	// BackBuffer�� ȭ�鿡 �׷����� �뵵�� ���̴����� ����� �� �ִ� �뵵�� ����
	desc.SampleDesc.Quality = 0;													// ���ø��� �ȼ� ���̴��� ����
	desc.SampleDesc.Count = 1;														// ���ø� �� ����
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;										// ���� ������ �ϴ� ����� ������ ������� �غ�Ǵ� ������ ����
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIFactory* PtrFactory = nullptr;

	MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&PtrFactory));

	PtrFactory->CreateSwapChain(Device.Get(), &desc, &SwapChain);
	PtrFactory->Release();
	MainAdapter->Release();

	if (nullptr == SwapChain)
	{
		MSGASSERT("����ü�� ���ۿ� �����߽��ϴ�.");
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBufferTexture = nullptr;
	
	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBufferTexture))
	{
		MSGASSERT("����� �ؽ�ó �ε� ����");
	}
	
	BackBufferRenderTarget = std::make_shared<URenderTarget>();
	BackBufferRenderTarget->CreateTarget(BackBufferTexture);
	BackBufferRenderTarget->CreateDepthStencil();

}

// ���� �����ս��� ���� �׷��� ��ġ �ϵ��� ã�� �޼ҵ�
IDXGIAdapter* UEngineGraphicDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* ResultAdapter = nullptr;

	unsigned __int64 MaxGraphicDeviceMemory = 0;

	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (nullptr == Factory)
	{
		MSGASSERT("�׷��� ī�� ���� ���丮 ���� ����");
		return nullptr;
	}

	for (int Index = 0; ; ++Index)
	{
		IDXGIAdapter* CurAdapter = nullptr;
		Factory->EnumAdapters(Index, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		// ����� ���� ����ü
		DXGI_ADAPTER_DESC Desc;
		CurAdapter->GetDesc(&Desc);

		// TODO: VRAM ũ�⿡ ���� �� ���� ���� ��ȯ
		if (MaxGraphicDeviceMemory <= Desc.DedicatedVideoMemory)
		{
			MaxGraphicDeviceMemory = Desc.DedicatedVideoMemory;

			if (nullptr != ResultAdapter)
			{
				ResultAdapter->Release();
			}

			ResultAdapter = CurAdapter;
			continue;
		}

		CurAdapter->Release();
	}

	if (nullptr != Factory)
	{
		Factory->Release();
	}

	if (nullptr == ResultAdapter)
	{
		MSGASSERT("�׷���ī�尡�� ã�� ���߽��ϴ�.");
		return nullptr;
	}

	return ResultAdapter;
}
