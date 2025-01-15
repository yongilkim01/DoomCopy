struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 COLOR : COLOR;
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

float crossProduct(float2 p0, float2 p1, float2 p2)
{
    return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

VertexShaderOutPut VertexToWorld_VS(EngineVertex _Vertex)
{
    VertexShaderOutPut OutPut;
    
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
    
    OutPut.COLOR = _Vertex.COLOR;
    
    return OutPut;
}

float4 PixelToWorld_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
  // �ﰢ���� �� ������ ����
    float2 v0 = float2(640.0f, 300.0f); // ù ��° ������
    float2 v1 = float2(570.0f, 420.0f); // �� ��° ������
    float2 v2 = float2(710.0f, 420.0f); // �� ��° ������

    float2 v3 = float2(640.0f, 440.0f); // ù ��° ������
    float2 v4 = float2(570.0f, 320.0f); // �� ��° ������
    float2 v5 = float2(710.0f, 320.0f); // �� ��° ������

// �ȼ� ��ġ
    float2 pixelPos = _Vertex.SVPOSITION.xy;

// �ﰢ���� �� ���� ���� ������ ����Ͽ� ����/�ܺ� �Ǵ�
    float cross1 = crossProduct(v0, v1, pixelPos);
    float cross2 = crossProduct(v1, v2, pixelPos);
    float cross3 = crossProduct(v2, v0, pixelPos);

    float cross4 = crossProduct(v3, v4, pixelPos);
    float cross5 = crossProduct(v4, v5, pixelPos);
    float cross6 = crossProduct(v5, v3, pixelPos);

// �ﰢ�� ���ο� �ִ��� Ȯ�� (�� ������ ��� ���� ��ȣ���� �ﰢ�� ����)
    if ((cross1 >= 0.0f && cross2 >= 0.0f && cross3 >= 0.0f)||
        (cross1 <= 0.0f && cross2 <= 0.0f && cross3 <= 0.0f)||
        (cross4 >= 0.0f && cross5 >= 0.0f && cross6 >= 0.0f)||
        (cross4 <= 0.0f && cross5 <= 0.0f && cross6 <= 0.0f))

    {
        return float4(1.0f - _Vertex.SVPOSITION.x / 1280.0f, 1.0f - _Vertex.SVPOSITION.x / 640.0f, 0.0f, 1.0f); // ������
    }
    else
    {
        return float4(1.0f - _Vertex.SVPOSITION.y / 640.0f, _Vertex.SVPOSITION.y / 640.0f, 0.0f, 1.0f); // �ʷϻ�
    }
}