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
	float4 SVPOSITION : SV_POSITION; // ����Ʈ����� �������� �������Դϴ�.
	float4 NEWPOSITION : POSITION; // 
	float4 COLOR : COLOR;
};

// ���� ���ϴ� ũ��λ簢������ �����. 
// ���ؽ����̴��� �� �������.
VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
	// float4x4 WVP;
	
	VertexShaderOutPut OutPut;
	// _Vertex 0.5, 0.5
	OutPut.SVPOSITION = _Vertex.POSITION;
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