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
    float4 SVPOSITION : SV_POSITION; // 뷰포트 행렬이 곱해지는 포지션입니다.
    float4 NEWPOSITION : TEXCOORD0; // 변경된 의미론
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

// 버텍스 쉐이더를 다 만들었다.
VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    // CPU에서 계산한 값을 쉐이더에게 넘기는 방법을 알아야 하는데
    // 상수버퍼라고 부릅니다.
    // 그중에서 가장 기본적인 것은 상수버퍼를 

    // float4x4 WVP;

    VertexShaderOutPut OutPut;
    // _Vertex 0.5, 0.5
    OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
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
