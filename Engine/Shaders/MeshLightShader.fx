#include "Light.hlsli"
#include "Transform.hlsli"

struct EngineVertex
{
    float4 POSITION : POSITION; // 정점의 위치 정보
    float4 TEXCOORD : TEXCOORD; // UV 좌표 정보 (텍스처 맵핑용)
    float4 COLOR : COLOR; // 정점의 색상 정보
    float4 NORMAL : NORMAL; // 정점의 법선 벡터
    float4 BINORMAL : BINORMAL; // 정점의 바이노멀 벡터 (TBN 행렬 구성용)
    float4 TANGENT : TANGENT; // 정점의 탄젠트 벡터 (TBN 행렬 구성용)
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION; // 클리핑 공간의 위치 (최종 출력 좌표)
    float4 TEXCOORD : TEXCOORD; // 텍스처 좌표
    float4 COLOR : COLOR; // 정점의 색상 정보

    float4 VIEWPOS : POSITION; // 뷰 공간에서의 위치
    float4 NORMAL : NORMAL; // 뷰 공간에서의 법선 벡터
    float4 BINORMAL : BINORMAL; // 뷰 공간에서의 바이노멀 벡터
    float4 TANGENT : TANGENT; // 뷰 공간에서의 탄젠트 벡터
};

VertexShaderOutPut MeshLight_VS(EngineVertex Vertex)
{
    VertexShaderOutPut OutPut;

    // 정점의 위치를 월드-뷰-프로젝션 변환을 적용하여 클리핑 공간 좌표로 변환
    OutPut.SVPOSITION = mul(Vertex.POSITION, WVP);
    
    // 텍스처 좌표와 색상을 그대로 전달
    OutPut.TEXCOORD = Vertex.TEXCOORD;
    OutPut.COLOR = Vertex.COLOR;

    // 월드-뷰 변환을 적용하여 뷰 공간에서의 정점 위치 계산
    OutPut.VIEWPOS = mul(Vertex.POSITION, World * View);

    // 법선, 바이노멀, 탄젠트 벡터의 w 값을 0으로 설정하여 방향 벡터로 취급
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
    float4 SettingColor; // 설정된 기본 색상
};

float4 MeshLight_PS(VertexShaderOutPut Vertex) : SV_Target0
{
    float4 ResultColor;
    ResultColor = SettingColor; // 기본 색상 초기화

    // 확산 조명
    float4 DiffuseColor;
    // 스페큘러 조명
    float4 SpacularLight;
    
    float4 AmbiantLight = (0.1f, 0.1f, 0.1f, 0.1f);

    for (int i = 0; i < LightCount; ++i)
    {
        DiffuseColor += CalDiffusLight(Vertex.NORMAL, LightArr[i]);
        SpacularLight += CalSpacularLight(Vertex.VIEWPOS, Vertex.NORMAL, LightArr[i]);
    }
	
    ResultColor.xyz *= (DiffuseColor.xyz + SpacularLight.xyz + AmbiantLight.xyz);
    
    ResultColor.a = 1.0f; // 알파값 설정

    return ResultColor;
}
