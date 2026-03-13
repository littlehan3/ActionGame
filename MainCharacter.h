#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent; // 스프링암 컴포넌트
class UCameraComponent; // 카메라 컴포넌트
class UInputMappingContext; // Enhanced Input의 입력 매핑 컨텍스트
class UInputAction; // Enhanced Input의 입력 액션
struct FInputActionValue; // Enhanced Input의 입력 액션 값

UCLASS()
class ACTIONGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	// AnimInstance에서 캐스팅해서 읽어올 값에 대한 Getter 함수
	float GetSpeed() const { return Speed; } // 캐릭터의 현재 속도 반환
	float GetDirection() const { return Direction; } // 캐릭터의 이동 방향 반환
	float GetTurnRate() const { return TurnRate; } // 캐릭터의 제자리 턴 판별용 Yaw 값 변화량 반환
	bool GetShouldTurn() const { return bShouldTurn; } // 턴 필요 여부 반환
	float GetYawDeltaToController() const { return YawDeltaToController; } // 컨트롤러와의 각도 차 반환

	// AnimInstance에서 턴 애니메이션 종료 시 호출
	void OnTurnAnimationFinished();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; // 틱 함수 선언
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 입력 컴포넌트 설정 함수 선언

private:
	// 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USpringArmComponent> SpringArm; // 스프링암 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UCameraComponent> Camera; // 카메라 컴포넌트

	// 입력
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputMappingContext> InputMappingContext; // 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> MoveAction; // 이동 입력 액션
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UInputAction> LookAction; // 시선 방향 입력 액션

	void Move(const FInputActionValue& Value); // 이동 함수 선언
	void Look(const FInputActionValue& Value); // 시선 방향 함수 선언

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float LookSensitivity = 0.5f; // 마우스 감도

	// AnimInstance에서 캐스팅해서 읽어올 값
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"));
	float Speed = 0.0f; // 캐릭터의 현재 속도
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (ClampMin = "-180.0", ClampMax = "180.0", AllowPrivateAccess = "true"));
	float Direction = 0.0f; // 캐릭터의 이동 방향
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"));
	float TurnRate = 0.0f; // 제자리 턴 판별용 Yaw 값 변화량

	float PreviousYaw = 0.0f; // 이전 프레임의 Yaw 값
	float YawDeltaToController = 0.0f; // 컨트롤러와 캐릭터의 Yaw 값의 각도 차
	bool bShouldTurn = false; // 턴 필요 여부
	float TurnTargetYaw = 0.0f; // 턴 시작 시 고정된 목표 Yaw 값
	float TurnDirectionSign = 0.0f; // 턴 방향 (+1: 우, -1: 좌)

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnTriggerAngle = 90.0f; // 턴이 필요한 각도 차이 임계값
};
