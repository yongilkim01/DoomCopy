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
	}
};

EngineVertexInit InitObject;