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
    float4 Qut;
    float4 Location;

	// 릴리에티브 로컬
    float4 RelativeScale;
    float4 RelativeRotation;
    float4 RelativeQut;
    float4 RelativeLocation;

	// 월드
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

cbuffer FSpriteData : register(b1)
{
    float4 CuttingPos;
    float4 CuttingSize;
    float4 Pivot;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    VertexShaderOutPut OutPut;
    
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);

    OutPut.UV.x = (_Vertex.UV.x * CuttingSize.x) + CuttingPos.x;
    OutPut.UV.y = (_Vertex.UV.y * CuttingSize.y) + CuttingPos.y;
    OutPut.UV.z = 1.0f; // z 값을 초기화 
    OutPut.UV.w = 1.0f; // w 값을 초기화
    
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