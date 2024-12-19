struct EngineVertex
{
    float4 COLOR : COLOR;
    float4 POSITION : POSITION;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 COLOR : COLOR;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    VertexShaderOutPut OutPut;
	
	// _Vertex 0.5, 0.5
    OutPut.SVPOSITION = _Vertex.POSITION;
    OutPut.COLOR = _Vertex.COLOR;
	
    return OutPut;
}