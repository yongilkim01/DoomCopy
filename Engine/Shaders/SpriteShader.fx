struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 COLOR : COLOR;
};

// ���ؽ� ���̴��� ������ ���ϰ��� �־�� �մϴ�.
// ��ǲ�����2�� �ѱ� ���� ����������ϴµ�.
// �̶��� ��Ģ�� �ֽ��ϴ�.

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION; // ����Ʈ ����� �������� �������Դϴ�.
    float4 NEWPOSITION : TEXCOORD0; // ����� �ǹ̷�
    float4 COLOR : COLOR;
};

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

// ���ؽ� ���̴��� �� �������.
VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    // CPU���� ����� ���� ���̴����� �ѱ�� ����� �˾ƾ� �ϴµ�
    // ������۶�� �θ��ϴ�.
    // ���߿��� ���� �⺻���� ���� ������۸� 

    // float4x4 WVP;

    VertexShaderOutPut OutPut;
    // _Vertex 0.5, 0.5
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
    OutPut.NEWPOSITION = _Vertex.POSITION;
    //OutPut.SVPOSITION *= Projection;
    OutPut.COLOR = _Vertex.COLOR;
    return OutPut;
}

// �̹����� ���ø��ؼ� �̹����� ���̰� �����
float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
    if (_Vertex.NEWPOSITION.x < 0)
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        return float4(0.0f, 1.0f, 0.0f, 1.0f);
    }

    return _Vertex.COLOR;
}
