struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD; // UV���̶�� �Ҹ��� ����� �ؽ�ó�� ���εǴ� ������ �������ݴϴ�.
    float4 COLOR : COLOR;
    float4 NORMAL : NORMAL; // ���� ���� ���Ͱ� �ʿ��ϴ�.
    float4 BINORMAL : BINORMAL; // ���� ������ ���� �����Ͱ� ź��Ʈ ������ ����� ���Ѱ�.
    float4 TANGENT : TANGENT; // ���� ������ ���� �����Ͱ� ź��Ʈ ������ ����� ���Ѱ�.
};
// ���ؽ� ���̴��� ������ ���ϰ��� �־�� �մϴ�.
// ��ǲ�����2�� �ѱ� ���� ����������ϴµ�.
// �̶��� ��Ģ�� �ֽ��ϴ�.
struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 COLOR : COLOR;
    float4 NORMAL : NORMAL;
    float4 BINORMAL : BINORMAL;
    float4 TANGENT : TANGENT;
	
    float4 LIGHTCOLOR : LIGHTCOLOR;
};
// ������۸� ����ϰڴ�.
// ������۴� �뷮������ �ֽ��ϴ�.
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
//struct FTransform 
//{
//	// transformupdate�� 
//	// �Ʒ��� ������ �� �����ؼ�
//	// WVP�� ������ �Լ��̴�.
//	// ��ȯ�� ����
//	float4 Scale;
//	float4 Rotation;
//	float4 Qut;
//	float4 Location;
//	// ������Ƽ�� ����
//	float4 RelativeScale;
//	float4 RelativeRotation;
//	float4 RelativeQut;
//	float4 RelativeLocation;
//	// ����
//	float4 WorldScale;
//	float4 WorldRotation;
//	float4 WorldQuat;
//	float4 WorldLocation;
//	float4x4 ScaleMat;
//	float4x4 RotationMat;
//	float4x4 LocationMat;
//	float4x4 RevolveMat;
//	float4x4 ParentMat;
//	float4x4 LocalWorld;
//	float4x4 World;
//	float4x4 View;
//	float4x4 Projection;
//	float4x4 WVP;
//};
// ���ο� ���ҽ��� �ϳ��� ����̴ϴ�.
// StructuredBuffer<FTransform> �̳༮�� �ؽ�ó ����Դϴ�.
// ������۴� �ƹ��͵� ���������� ������ �⺻���� 0���� ä�����ϴ�.
cbuffer FSpriteData : register(b1)
{
    float4 CuttingPos;
    float4 CuttingSize;
    float4 Pivot; // 0.5 0.0f
};
// ���ؽ����̴��� �� �������.
// �ѹ��� 100���� �׸��ٸ�. _DataIndex �̳༮�� 100���� �׷����� �ֵ��� 5��° Ŭ������ ����� ������� �ֽ��ϴ�.
// 0~99 
VertexShaderOutPut TileMap_VS(EngineVertex _Vertex /*, int _DataIndex*/)
{
    VertexShaderOutPut OutPut;
	
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
    OutPut.TEXCOORD = _Vertex.TEXCOORD;
    OutPut.COLOR = _Vertex.COLOR;
	// ���� �Ϲ������� ��������� ����մϴ�.
    OutPut.NORMAL = mul(_Vertex.POSITION, World * View);
    OutPut.BINORMAL = mul(_Vertex.POSITION, World * View);
    OutPut.TANGENT = mul(_Vertex.POSITION, World * View);
	
	// �� �������� ���⿡�� �ع����� �ѱ�� ��ν��̵��̶�� �Ѵ�.
	// OutPut.LIGHTCOLOR = 
	
    return OutPut;
}
Texture2D TileMapTex : register(t0);
// ���÷� 1���� �ʿ��մϴ�.
SamplerState ImageSampler : register(s0);
// ���̴������� ������� �ε��� ���ĵ� ��� ����.
// ex) ���ؽ����̴����� 0�� ������۸� ��
// ex) �ȼ����̴������� 0�� �� ���� �ִ�.
cbuffer ResultColor : register(b0)
{
    float4 PlusColor;
    float4 MulColor;
};
// �̹����� ���ø��ؼ� �̹����� ���̰� �����
float4 TileMap_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
    float4 Color = TileMapTex.Sample(ImageSampler, _Vertex.TEXCOORD.xy);
	
    if (0.0f >= Color.a)
    {
		// �ȼ����̴����� �ƿ�ǲ ������ �ѱ��� �ʴ´�.
        clip(-1);
    }
	
    Color += PlusColor;
    Color *= MulColor;
    return Color;
};