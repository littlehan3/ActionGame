#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner(); // 애니메이션 인스턴스의 소유자 Pawn 가져옴
	if (!IsValid(Pawn)) return; // 유효성 검사

	OwnerCharacter = Cast<AMainCharacter>(Pawn); // 캐스팅하여 AMainCharacter 참조 저장
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter)) return; // 캐릭터 참조 유효성 검사

	Speed = OwnerCharacter->GetSpeed(); // 캐릭터의 현재 속도 가져옴
	Direction = OwnerCharacter->GetDirection(); // 캐릭터의 이동 방향 가져옴
	TurnRate = OwnerCharacter->GetTurnRate(); // 제자리 턴 판별용 Yaw 값 변화량 가져옴

	bIsMoving = Speed > 3.0f; // 이동 중인지 여부 판별

	// 캐릭터의 bShouldTurn과 YawDelta 방향으로 턴 방향 결정
	bool bShouldTurn = OwnerCharacter->GetShouldTurn();
	float YawDelta = OwnerCharacter->GetYawDeltaToController();
	bIsRunning = OwnerCharacter->GetRunning();
	bHasMovementInput = OwnerCharacter->GetHasMovementInput();

	bIsTurningLeft = bShouldTurn && YawDelta < 0.0f; // 왼쪽 턴 (카메라가 캐릭터 왼쪽)
	bIsTurningRight = bShouldTurn && YawDelta > 0.0f; // 오른쪽 턴 (카메라가 캐릭터 오른쪽)
}

void UMainCharacterAnimInstance::OnTurnFinished()
{
	if (!IsValid(OwnerCharacter)) return; // 유효성 검사

	OwnerCharacter->OnTurnAnimationFinished(); // 캐릭터에 턴 애니메이션 종료 함수 호출
}
