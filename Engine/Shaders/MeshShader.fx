struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexOutput
{
    float4 SVPOSITION : SV_POSITION;
    float4 UV : TEXCOORD;
};

cbuffer FTransform : register(b0)
{
    float4 Scale;
    float4 Rotation;
    float4 Qut;
    float4 Location;

    float4 RelativeScale;
    float4 RelativeRotation;
    float4 RelativeQut;
    float4 RelativeLocation;

    float4 WorldScale;
    float4 WorldRotation;
    float4 WorldQuat;
    float4 WorldLocation;

    float4x4 ScaleMat;
    float4x4 RotationMat;
    float4x4 LocationMat;
    float4x4 RevolveMat;
    float4x4 ParentMat;
    float4x4 LocalWorld;
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WVP;
};

VertexOutput VS(EngineVertex InVertex)
{
    VertexOutput OutPut;
    
    OutPut.SVPOSITION = mul(InVertex.POSITION, WVP);
    OutPut.UV = InVertex.UV;
    
    return OutPut;
}

Texture2D ImageTexture : register(t0);
SamplerState ImageSampler : register(s0);

float4 PS(VertexOutput InVertex) : SV_Target0
{
    float4 Color = ImageTexture.Sample(ImageSampler, InVertex.UV.xy);
    return Color;
}