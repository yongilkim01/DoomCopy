#include "Light.hlsli"
#include "Transform.hlsli"

struct EngineVertex
{
    float4 POSITION : POSITION; // ������ ��ġ ����
    float4 TEXCOORD : TEXCOORD; // UV ��ǥ ���� (�ؽ�ó ���ο�)
    float4 COLOR : COLOR; // ������ ���� ����
    float4 NORMAL : NORMAL; // ������ ���� ����
    float4 BINORMAL : BINORMAL; // ������ ���̳�� ���� (TBN ��� ������)
    float4 TANGENT : TANGENT; // ������ ź��Ʈ ���� (TBN ��� ������)
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION; // Ŭ���� ������ ��ġ (���� ��� ��ǥ)
    float4 TEXCOORD : TEXCOORD; // �ؽ�ó ��ǥ
    float4 COLOR : COLOR; // ������ ���� ����

    float4 VIEWPOS : POSITION; // �� ���������� ��ġ
    float4 NORMAL : NORMAL; // �� ���������� ���� ����
    float4 BINORMAL : BINORMAL; // �� ���������� ���̳�� ����
    float4 TANGENT : TANGENT; // �� ���������� ź��Ʈ ����
};

VertexShaderOutPut MeshLight_VS(EngineVertex Vertex)
{
    VertexShaderOutPut OutPut;

    // ������ ��ġ�� ����-��-�������� ��ȯ�� �����Ͽ� Ŭ���� ���� ��ǥ�� ��ȯ
    OutPut.SVPOSITION = mul(Vertex.POSITION, WVP);
    
    // �ؽ�ó ��ǥ�� ������ �״�� ����
    OutPut.TEXCOORD = Vertex.TEXCOORD;
    OutPut.COLOR = Vertex.COLOR;

    // ����-�� ��ȯ�� �����Ͽ� �� ���������� ���� ��ġ ���
    OutPut.VIEWPOS = mul(Vertex.POSITION, World * View);

    // ����, ���̳��, ź��Ʈ ������ w ���� 0���� �����Ͽ� ���� ���ͷ� ���
    Vertex.NORMAL.w = 0.0f;
    OutPut.NORMAL = mul(Vertex.NORMAL, World * View);

    Vertex.BINORMAL.w = 0.0f;
    OutPut.BINORMAL = mul(Vertex.BINORMAL, World * View);

    Vertex.TANGENT.w = 0.0f;
    OutPut.TANGENT = mul(Vertex.TANGENT, World * View);

    return OutPut;
}

cbuffer MeshColor : register(b0)
{
    float4 SettingColor; // ������ �⺻ ����
};

float4 MeshLight_PS(VertexShaderOutPut Vertex) : SV_Target0
{
    float4 ResultColor;
    ResultColor = SettingColor; // �⺻ ���� �ʱ�ȭ

    // Ȯ�� ����
    float4 DiffuseColor;
    // ����ŧ�� ����
    float4 SpacularLight;
    
    float4 AmbiantLight = (0.1f, 0.1f, 0.1f, 0.1f);

    for (int i = 0; i < LightCount; ++i)
    {
        DiffuseColor += CalDiffusLight(Vertex.NORMAL, LightArr[i]);
        SpacularLight += CalSpacularLight(Vertex.VIEWPOS, Vertex.NORMAL, LightArr[i]);
    }
	
    ResultColor.xyz *= (DiffuseColor.xyz + SpacularLight.xyz + AmbiantLight.xyz);
    
    ResultColor.a = 1.0f; // ���İ� ����

    return ResultColor;
}
