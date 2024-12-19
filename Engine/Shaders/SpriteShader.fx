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
	float4 SVPOSITION : SV_POSITION;
	float4 COLOR : COLOR;
};

// ���ؽ����̴��� �� �������.
VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	// _Vertex 0.5, 0.5
	OutPut.SVPOSITION = _Vertex.POSITION;
	OutPut.COLOR = _Vertex.COLOR;
	return OutPut;
}

float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
    float a = _Vertex.SVPOSITION.x - 640.0f;
    float b = _Vertex.SVPOSITION.y - 360.0f;
    float r = 50.0f;
    if (a * a + b * b <= r * r)
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    return float4(0.0f, 1.0f, 0.0f, 1.0f);

}