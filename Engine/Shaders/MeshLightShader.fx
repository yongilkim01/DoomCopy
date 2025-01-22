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

struct FLightData
{
    float4 LightPos; // ���� ��ġ
    float4 LightDir; // ���� Forward
    float4 LightRevDir; // ���� �ݴ������ < L!!!!! <= 
    float4 LightColor;
    float4 AmbientLight;
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
    float4 CameraPosition;
};

cbuffer FLightDatas : register(b11)
{
    int Count;
    FLightData Arr[256];
};

VertexShaderOutPut MeshLight_VS(EngineVertex _Vertex /*, int _DataIndex*/)
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

cbuffer MeshColor : register(b0)
{
    float4 Color;
};

float4 MeshLight_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
    return Color;
};