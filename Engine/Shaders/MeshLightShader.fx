struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD; // UV값이라고 불리는 존재로 텍스처가 매핑되는 비율을 지정해줍니다.
    float4 COLOR : COLOR;
    float4 NORMAL : NORMAL; // 면의 법선 백터가 필요하다.
    float4 BINORMAL : BINORMAL; // 범프 매핑을 위한 데이터가 탄젠트 공간을 만들기 위한것.
    float4 TANGENT : TANGENT; // 범프 매핑을 위한 데이터가 탄젠트 공간을 만들기 위한것.
};
// 버텍스 쉐이더는 무조건 리턴값이 있어야 합니다.
// 인풋어셈블러2로 넘길 값을 리턴해줘야하는데.
// 이때도 규칙이 있습니다.
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
// 상수버퍼를 사용하겠다.
// 상수버퍼는 용량제한이 있습니다.
cbuffer FTransform : register(b0)
{
	// transformupdate는 
	// 아래의 값들을 다 적용해서
	// WVP를 만들어내는 함수이다.
	// 변환용 벨류
    float4 Scale;
    float4 Rotation;
    float4 Qut;
    float4 Location;
	// 릴리에티브 로컬
    float4 RelativeScale;
    float4 RelativeRotation;
    float4 RelativeQut;
    float4 RelativeLocation;
	// 월드
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
    float4 LightPos; // 빛의 위치
    float4 LightDir; // 빛의 Forward
    float4 LightRevDir; // 빛의 반대방향이 < L!!!!! <= 
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
	// 빛은 일반적으로 뷰공간에서 계산합니다.
    OutPut.NORMAL = mul(_Vertex.POSITION, World * View);
    OutPut.BINORMAL = mul(_Vertex.POSITION, World * View);
    OutPut.TANGENT = mul(_Vertex.POSITION, World * View);
	
	// 빛 계산공식을 여기에서 해버리고 넘기면 고로쉐이딩이라고 한다.
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