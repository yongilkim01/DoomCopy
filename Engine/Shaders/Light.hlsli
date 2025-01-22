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

// N (일반적으로 법선 벡터)과 L (빛의 반대 방향 벡터) 간의 내적을 계산
// 확산 조명(Diffuse Lighting)을 계산하는 함수
// 이 함수는 빛의 반사 정도를 계산하는 데 사용
float4 CalDiffusLight(float4 _Normal, FLightData _Data)
{
    // 법선 벡터와 빛의 반대 방향 벡터의 내적을 구한 후,
    // 결과 값에 해당하는 빛 색상 값(RGB)을 반환
    float4 ResultLight = dot(_Normal, _Data.LightRevDir);

    // 빛 색상 반환 (여기서는 단순히 LightColor만 반환)
    return _Data.LightColor;
}
