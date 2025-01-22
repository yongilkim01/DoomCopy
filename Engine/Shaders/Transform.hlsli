// 상수 버퍼(cbuffer)를 사용하여 변환 데이터를 GPU에 전달
// 상수 버퍼는 메모리 용량 제한 존재
cbuffer FTransform : register(b0)
{
    // TransformUpdate 함수는 아래의 값들을 조합하여
    // WVP(World-View-Projection) 행렬을 생성

    // 변환에 필요한 값들
    float4 Scale; // 객체의 스케일링 값 (x, y, z 축 스케일)
    float4 Rotation; // 회전 정보 (Euler 각도 또는 Quaternion 변환 전 값)
    float4 Qut; // 회전에 사용되는 Quaternion 값
    float4 Location; // 객체의 위치 (월드 좌표)

    // 객체의 부모와 관련된 상대적(Local) 변환 정보
    float4 RelativeScale; // 부모 객체에 대한 상대적 스케일
    float4 RelativeRotation; // 부모 객체에 대한 상대적 회전 (Euler 각도)
    float4 RelativeQut; // 부모 객체에 대한 상대적 회전 (Quaternion 값)
    float4 RelativeLocation; // 부모 객체에 대한 상대적 위치

    // 월드 좌표계에서의 최종 변환 정보
    float4 WorldScale; // 월드 공간에서의 최종 스케일 값
    float4 WorldRotation; // 월드 공간에서의 최종 회전 값 (Euler 각도)
    float4 WorldQuat; // 월드 공간에서의 최종 회전 값 (Quaternion 값)
    float4 WorldLocation; // 월드 공간에서의 최종 위치 값

    // 변환 행렬들
    float4x4 ScaleMat; // 스케일 행렬
    float4x4 RotationMat; // 회전 행렬
    float4x4 LocationMat; // 위치 변환 행렬
    float4x4 RevolveMat; // 회전을 기반으로 한 보조 행렬
    float4x4 ParentMat; // 부모 객체의 변환 행렬

    // 로컬 공간과 월드 공간의 변환 결과
    float4x4 LocalWorld; // 로컬 공간에서 월드 공간으로의 변환 행렬
    float4x4 World; // 월드 공간에서의 최종 변환 행렬

    // 뷰와 프로젝션 행렬
    float4x4 View; // 카메라의 뷰 행렬
    float4x4 Projection; // 카메라의 프로젝션 행렬

    // WVP(World-View-Projection) 행렬
    float4x4 WVP; // 최종적으로 GPU에서 사용되는 변환 행렬
};

// GPU에서 사용하기 위한 구조화된 버퍼 정의
struct FTransformStruct
{
    // TransformUpdate 함수는 아래의 값들을 조합하여
    // WVP(World-View-Projection) 행렬을 생성

    // 변환에 필요한 값들
    float4 Scale; // 객체의 스케일링 값 (x, y, z 축 스케일)
    float4 Rotation; // 회전 정보 (Euler 각도 또는 Quaternion 변환 전 값)
    float4 Qut; // 회전에 사용되는 Quaternion 값
    float4 Location; // 객체의 위치 (월드 좌표)

    // 객체의 부모와 관련된 상대적(Local) 변환 정보
    float4 RelativeScale; // 부모 객체에 대한 상대적 스케일
    float4 RelativeRotation; // 부모 객체에 대한 상대적 회전 (Euler 각도)
    float4 RelativeQut; // 부모 객체에 대한 상대적 회전 (Quaternion 값)
    float4 RelativeLocation; // 부모 객체에 대한 상대적 위치

    // 월드 좌표계에서의 최종 변환 정보
    float4 WorldScale; // 월드 공간에서의 최종 스케일 값
    float4 WorldRotation; // 월드 공간에서의 최종 회전 값 (Euler 각도)
    float4 WorldQuat; // 월드 공간에서의 최종 회전 값 (Quaternion 값)
    float4 WorldLocation; // 월드 공간에서의 최종 위치 값

    // 변환 행렬들
    float4x4 ScaleMat; // 스케일 행렬
    float4x4 RotationMat; // 회전 행렬
    float4x4 LocationMat; // 위치 변환 행렬
    float4x4 RevolveMat; // 회전을 기반으로 한 보조 행렬
    float4x4 ParentMat; // 부모 객체의 변환 행렬

    // 로컬 공간과 월드 공간의 변환 결과
    float4x4 LocalWorld; // 로컬 공간에서 월드 공간으로의 변환 행렬
    float4x4 World; // 월드 공간에서의 최종 변환 행렬

    // 뷰와 프로젝션 행렬
    float4x4 View; // 카메라의 뷰 행렬
    float4x4 Projection; // 카메라의 프로젝션 행렬

    // WVP(World-View-Projection) 행렬
    float4x4 WVP; // 최종적으로 GPU에서 사용되는 변환 행렬
};

// 겹침 문제를 방지하기 위해 12번 슬롯으로 고정
StructuredBuffer<FTransformStruct> TransformBuffer : register(t0);
