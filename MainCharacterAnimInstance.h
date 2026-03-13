#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class ACTIONGAME_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // 애니메이션 업데이트 함수
	virtual void NativeInitializeAnimation() override; // 애니메이션 초기화 함수 

	// ABP에서 사용할 변수
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0.0f; // 캐릭터의 현재 속도

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction = 0.0f; // 캐릭터의 이동 방향 (전, 후, 좌, 우)

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float TurnRate = 0.0f; // 제자리 턴 판별용 Yaw 값 변화량

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false; // 캐릭터가 이동 중인지 여부

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsTurningLeft = false; // 캐릭터가 왼쪽으로 제자리 턴 중인지 여부

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsTurningRight = false; // 캐릭터가 오른쪽으로 제자리 턴 중인지 여부

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void OnTurnFinished(); // 턴 애니메이션 종료 시 캐릭터에 알리는 함수

private:
	UPROPERTY()
	TObjectPtr<class AMainCharacter> OwnerCharacter; // 캐릭터 참조
};
