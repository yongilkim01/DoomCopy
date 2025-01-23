// 빛 계산에 사용되는 상수 버퍼 구조체
// 각 조명에 대한 정보를 저장하는 구조체
struct FLightData
{
    float4 LightPos; // 빛의 위치 (월드 좌표)
    float4 LightDir; // 빛의 전방 방향 벡터 (빛의 방향)
    float4 LightRevDir; // 빛의 반대 방향 벡터 (빛의 반대 방향)
    float4 LightColor; // 빛의 색상 (RGB 값)
    float4 AmbientLight; // 주변광(Ambient Light)의 색상 (RGB 값)
    float4 ViewLightPos; // 카메라 뷰 공간에서의 빛의 위치
    float4 ViewLightDir; // 카메라 뷰 공간에서의 빛의 전방 방향 벡터
    float4 ViewLightRevDir; // 카메라 뷰 공간에서의 빛의 반대 방향 벡터
    float4 CameraPosition; // 카메라의 위치 (월드 좌표)
};

// 빛 데이터를 저장하는 상수 버퍼 정의
// Count는 빛의 총 개수를 나타내며, LightArr는 최대 256개의 빛 데이터를 저장
cbuffer FLightDatas : register(b11)
{
    int Count; // 현재 처리할 빛의 개수
    FLightData LightArr[256]; // 최대 256개의 빛 데이터를 저장할 배열
};

// HLSL에서는 전역 함수를 정의
// HLSL에서는 레퍼런스나 포인터 개념이 없으며, 모든 값은 값 복사 방식으로 처리


// 스페큘러 조명(Specular Lighting)을 계산하는 함수
// 스페큘러 조명은 빛의 반사가 관찰자에게 직접 보일 때 발생하는 밝은 하이라이트를 의미
float4 CalSpacularLight(float4 ViewPosition, float4 ViewNormal, FLightData LightData)
{
    // 결과로 반환할 스페큘러 조명의 강도
    float ResultLight = 0.0f;
    
    // 법선 벡터를 정규화 (Normalize the normal vector)
    float4 N = normalize(ViewNormal);
    
    // 빛의 반대 방향 벡터를 정규화 (Normalize the light reverse direction vector)
    float4 L = normalize(LightData.ViewLightRevDir);
    
    // 반사 벡터를 계산 (Reflection vector calculation)
    // 반사 벡터 = 2 * (N · L) * N - L
    float3 Reflection = normalize(2.0f * N.xyz * dot(L.xyz, N.xyz) - L.xyz);
    
    // 카메라(눈) 방향 벡터를 정규화 (Normalize the eye direction vector)
    float3 Eye = normalize(ViewPosition.xyz - LightData.CameraPosition.xyz);
    
    // 반사 벡터와 카메라(눈) 방향 벡터의 내적을 계산 (Dot product of reflection and eye direction)
    // 이 값은 스페큘러 조명의 강도를 결정
    ResultLight = max(0.0f, dot(Reflection.xyz, Eye.xyz));
    
    // 스페큘러 조명을 더 강렬하게 만들기 위해 지수 승을 적용 (Exponent to control the shininess)
    ResultLight = pow(ResultLight, 30.0f);

    // 최종 스페큘러 조명 값을 반환
    return ResultLight;
}


// 확산 조명(Diffuse Lighting)을 계산하는 함수
// 확산 조명은 물체의 표면이 빛에 의해 고르게 밝아지는 효과를 의미
float4 CalDiffusLight(float4 ViewNormal, FLightData LightData)
{
    // 법선 벡터를 정규화 (Normalize the normal vector)
    float4 N = normalize(ViewNormal);
    
    // 빛의 반대 방향 벡터를 정규화 (Normalize the light reverse direction vector)
    float4 L = normalize(LightData.ViewLightRevDir);
    // 이 벡터는 뷰 공간(View Space)에서 정의되어야 공식이 올바르게 동작

    // 확산 조명의 초기 강도 값
    float ResultLight;

    // 법선 벡터와 빛 방향 벡터의 내적을 계산 (Dot product of normal and light vectors)
    ResultLight = dot(N, L);

    // 내적 결과가 음수일 경우 최소값을 0으로 설정 (Clamp the result to a minimum of 0)
    ResultLight = max(0.0f, ResultLight);

    // 확산 조명 강도를 조정하기 위한 스케일링 (Scale the diffuse lighting intensity)
    return ResultLight * 0.5f;
}