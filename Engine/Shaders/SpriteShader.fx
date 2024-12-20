struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 COLOR : COLOR;
};

// 버텍스 쉐이더는 무조건 리턴값이 있어야 합니다.
// 인풋어셈블러2로 넘길 값을 리턴해줘야하는데.
// 이때도 규칙이 있습니다.

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION; // 뷰포트행렬이 곱해지는 포지션입니다.
	float4 NEWPOSITION : POSITION; // 
	float4 COLOR : COLOR;
};

// 내가 원하는 크기로사각형들을 만든다. 
// 버텍스쉐이더를 다 만들었다.
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

// 이미지를 샘플링해서 이미지를 보이게 만들고
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