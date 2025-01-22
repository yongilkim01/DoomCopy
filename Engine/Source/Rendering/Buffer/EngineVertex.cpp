#include "pch.h"
#include "EngineVertex.h"

UEngineInputLayoutInfo EngineVertex::Info;

class EngineVertexInit
{
public:
	EngineVertexInit()
	{
		EngineVertex::Info.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
		EngineVertex::Info.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
		EngineVertex::Info.AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
		EngineVertex::Info.AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
		EngineVertex::Info.AddInputLayout("BINORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
		EngineVertex::Info.AddInputLayout("TANGENT", DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
};

EngineVertexInit InitObject;

UEngineInputLayoutInfo& EngineVertex::GetInfo()
{
	return Info;
}
