struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 UV : TEXCOORD; // 
    float4 COLOR : COLOR;
};

// 상수버퍼를 사용하겠다.
cbuffer FTransform : register(b0)
{
    float4 Scale;
    float4 Rotation;
    float4 Location;

    float4x4 ScaleMat;
    float4x4 RotationMat;
    float4x4 LocationMat;
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WVP;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    VertexShaderOutPut OutPut;
    
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
    OutPut.UV = _Vertex.UV;
    OutPut.COLOR = _Vertex.COLOR;
    
    return OutPut;
}

Texture2D ImageTexture : register(t0);
SamplerState ImageSampler : register(s0);

float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
    float4 Color = ImageTexture.Sample(ImageSampler, _Vertex.UV.xy);
    return Color;
}