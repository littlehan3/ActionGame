#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h" // 스프링암 컴포넌트 포함
#include "Camera/CameraComponent.h" // 카메라 컴포넌트 포함
#include "GameFramework/CharacterMovementComponent.h" // 캐릭터 무브먼트 컴포넌트 포함
#include "EnhancedInputComponent.h" // Enhanced Input 컴포넌트 포함
#include "EnhancedInputSubsystems.h" // Enhanced Input 서브시스템 포함
#include "KismetAnimationLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 함수 활성화

	// 스프링암 컴포넌트 생성 및 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent); // 루트 컴포넌트에 부착
	SpringArm->TargetArmLength = 300.0f; // 카메라와 캐릭터 사이의 거리
	SpringArm->bUsePawnControlRotation = true; // 캐릭터의 회전에 따라 스프링암이 회전하도록 설정

	// 카메라 컴포넌트 생성 및 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm); // 스프링암에 부착
	Camera->bUsePawnControlRotation = false; // 카메라 캐릭터의 회전에 따라 회전하지 않도록 설정

	// 캐릭터 회전 설정
	bUseControllerRotationYaw = false; // 컨트롤러의 Yaw 회전에 따라 캐릭터가 회전하지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향에 따라 캐릭터가 회전하지 않도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 회전 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 300.0f; // 걷기 속도
	GetCharacterMovement()->BrakingDecelerationWalking = 173.0f; // 감속: ~1.73초에 걸쳐 정지 (WalkStop 애니메이션길이와 동기화)
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PreviousYaw = GetActorRotation().Yaw; // 초기 Yaw 값 저장

	APlayerController* PC = Cast<APlayerController>(GetController()); // PlayerController 가져오기
	if (!IsValid(PC)) return; // 유효성 검사

	// Enhanced Input 서브시스템 가져옴
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!IsValid(Subsystem)) return; // 유효성 검사

	Subsystem->AddMappingContext(InputMappingContext, 0); // 입력 매핑 컨텍스트 등록 (우선순위 0)
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Speed: 현재 수평 속력
	Speed = GetVelocity().Size2D();

	// Direction: 이동 방향과 캐릭터 정면의 각도 차
	if (Speed > 0.0f) // 이동 중인 경우
	{
		Direction = UKismetAnimationLibrary::CalculateDirection(GetVelocity(), GetActorRotation()); // 이동 벡터와 캐릭터의 회전값의 각도 계산
	}
	else // 정지 상태인 경우
	{
		Direction = 0.0f; // 이동 방향이 없으므로 0으로 설정
	}

	// 컨트롤러 Yaw와 캐릭터 Yaw의 각도 차 계산
	float ControllerYaw = GetControlRotation().Yaw;
	float CharacterYaw = GetActorRotation().Yaw;
	YawDeltaToController = FMath::FindDeltaAngleDegrees(CharacterYaw, ControllerYaw);

	// 이동 중인 경우 컨트롤러 방향으로 즉시 회전
	if (Speed > 3.0f)
	{
		FRotator TargetRotation(0.0f, ControllerYaw, 0.0f);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f));
		bShouldTurn = false;
	}
	// 턴 애니메이션 재생 중: 목표 방향으로 선형 보간 회전
	else if (bShouldTurn)
	{
		FRotator TargetRotation(0.0f, TurnTargetYaw, 0.0f);
		SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, 90.0f));
		YawDeltaToController = TurnDirectionSign * 90.0f; // AnimInstance에서 정확한 턴 방향 판별용

		// 캡슐이 목표 각도에 도달하면 추가 턴 또는 종료 (AnimNotify 미호출 대비)
		if (FMath::Abs(FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, TurnTargetYaw)) < 1.0f)
		{
			float RemainingDelta = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, ControllerYaw); // 턴 후에도 컨트롤러와의 각도 차 계산
			if (FMath::Abs(RemainingDelta) > TurnTriggerAngle * 0.5f) // 45° 이상 남으면 추가 턴
			{
				TurnDirectionSign = (RemainingDelta > 0.0f) ? 1.0f : -1.0f; // 턴 방향 결정
				TurnTargetYaw = GetActorRotation().Yaw + TurnDirectionSign * 90.0f; // 다음 턴 목표 각도 설정
			}
			else // 턴 종료
			{
				bShouldTurn = false; // 턴 종료 플래그 해제
				SetActorRotation(FRotator(0.0f, ControllerYaw, 0.0f)); // 컨트롤러 방향으로 스냅
			}
		}
	}
	// 제자리 대기: 각도 차가 임계값 초과 시 턴 시작
	else if (FMath::Abs(YawDeltaToController) >= TurnTriggerAngle) // 턴이 필요한 각도 차이 이상이면
	{
		TurnDirectionSign = (YawDeltaToController > 0.0f) ? 1.0f : -1.0f; // 턴 방향 결정
		bShouldTurn = true; // 턴 시작 플래그 설정
		TurnTargetYaw = CharacterYaw + TurnDirectionSign * 90.0f; // 턴 목표 각도 설정
	}

	// AnimInstance에서 사용할 TurnRate 값 계산
	float CurrentYaw = GetActorRotation().Yaw; // 현재 Yaw 값 가져오기
	float YawDelta = FMath::FindDeltaAngleDegrees(PreviousYaw, CurrentYaw); // 이전 Yaw와 현재 Yaw의 각도 차 계산
	TurnRate = (DeltaTime > 0.0f) ? YawDelta / DeltaTime : 0.0f; // TurnRate 계산
	PreviousYaw = CurrentYaw; // 현재 Yaw 값을 이전 프레임의 값으로 저장
}

void AMainCharacter::OnTurnAnimationFinished()
{
	if (!bShouldTurn) return;

	float ControllerYaw = GetControlRotation().Yaw;
	float CharacterYaw = GetActorRotation().Yaw;
	float RemainingDelta = FMath::FindDeltaAngleDegrees(CharacterYaw, ControllerYaw);

	if (FMath::Abs(RemainingDelta) > TurnTriggerAngle * 0.5f) // 남은 각도가 절반(45°) 이상이면 추가 턴
	{
		TurnDirectionSign = (RemainingDelta > 0.0f) ? 1.0f : -1.0f;
		TurnTargetYaw = CharacterYaw + TurnDirectionSign * 90.0f;
	}
	else // 턴 완전 종료
	{
		bShouldTurn = false;
		SetActorRotation(FRotator(0.0f, ControllerYaw, 0.0f)); // 컨트롤러 방향으로 스냅
	}

	PreviousYaw = GetActorRotation().Yaw;
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 컴포넌트를 Enhanced Input 컴포넌트로 캐스팅
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(EnhancedInputComponent)) return; // 유효성 검사

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move); // Move 액션 바인딩
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look); // Look 액션 바인딩
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>(); // 입력 데이터 FVector2D 형태로 가져옴

	// 카메라 기준 방향으로 이동
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f); // 카메라의 Yaw 회전값 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // 카메라 기준 전방 방향 계산
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // 카메라 기준 우측 방향 계산

	AddMovementInput(ForwardDirection, Input.Y); // 전방 방향으로 이동 입력 추가
	AddMovementInput(RightDirection, Input.X); // 우측 방향으로 이동 입력 추가
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>(); // 입력 데이터 FVector2D 형태로 가져옴
	AddControllerYawInput(Input.X * LookSensitivity); // 좌우 시선 방향 입력
	AddControllerPitchInput(-Input.Y * LookSensitivity); // 상하 시선 방향 입력 (반전)
}
