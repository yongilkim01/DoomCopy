#include "Transform.hlsli"

struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

VertexShaderOutPut VertexToWorld_VS(EngineVertex VertexData)
{
    VertexShaderOutPut OutPut;
	
	
    OutPut.SVPOSITION = mul(VertexData.POSITION, WVP);
	
    OutPut.UV.x = VertexData.UV.x;
    OutPut.UV.y = VertexData.UV.y;
    
    OutPut.COLOR = VertexData.COLOR;
    
    return OutPut;
}

Texture2D ImageTexture : register(t0);
SamplerState ImageSampler : register(s0);

float4 PixelToWorld_PS(VertexShaderOutPut InVertex) : SV_Target0
{
    float4 Color = ImageTexture.Sample(ImageSampler, InVertex.UV);
    
    if (Color.a == 0)
    {
        clip(-1);
    }
    
    return Color;
}