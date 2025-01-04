struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION; // ����Ʈ����� �������� �������Դϴ�.
    float4 UV : TEXCOORD; // 
    float4 COLOR : COLOR;
};

cbuffer FTransform : register(b0)
{
	// transformupdate�� 
	// �Ʒ��� ������ �� �����ؼ�
	// WVP�� ������ �Լ��̴�.
	// ��ȯ�� ����
    float4 Scale;
    float4 Rotation;
    float4 Qut;
    float4 Location;

	// ������Ƽ�� ����
    float4 RelativeScale;
    float4 RelativeRotation;
    float4 RelativeQut;
    float4 RelativeLocation;

	// ����
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

VertexShaderOutPut VertexToWorld_VS(EngineVertex _Vertex)
{
	// CPU���� ����� ���� ���̴����� �ѱ�� ����� �˾ƾ� �ϴµ�
	// ������۶�� �θ��ϴ�.
	// ���߿��� ���� �⺻���� ���� ������۸� 
	
	// float4x4 WVP;
	
    VertexShaderOutPut OutPut;
	
	
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	
    OutPut.UV = _Vertex.UV;
    
    OutPut.COLOR = _Vertex.COLOR;
    
    return OutPut;
}

Texture2D ImageTexture : register(t0);
SamplerState ImageSampler : register(s0);

float4 PixelToWorld_PS(VertexShaderOutPut InVertex) : SV_Target0
{
    float4 Color = ImageTexture.Sample(ImageSampler, InVertex.UV);
    return Color;
}