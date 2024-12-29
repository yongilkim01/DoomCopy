struct VS_INPUT         
{
    float4 POSITION : POSITION;
    float4 COLOR : COLOR;
};

struct VS_OUTPUT
{
    float4 POSITION : SV_POSITION;
    float4 COLOR : COLOR;
};

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

VS_OUTPUT VS(VS_INPUT input)
{
    VS_INPUT output;
    
    output.POSITION = mul(input.POSITION, WVP);
    output.COLOR = float4(1.0f, 1.0f, 1.0f, 0.0f);
    
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = input.COLOR;
    
    return color;
}