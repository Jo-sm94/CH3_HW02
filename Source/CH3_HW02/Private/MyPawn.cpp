#include "MyPawn.h"
#include "MyPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AMyPawn::AMyPawn()
{

	PrimaryActorTick.bCanEverTick = true;

	bIsFlying = false;
	Gravity = -980.0f;
	JumpingSpeed = 500.0f;
	MovingSpeed = 10.0f;
	RollSpeed = 30.0f;
	Yaw = 180.0f;
	Pitch = -30.f;
	Roll = 0.f;
	LookSensitivity = 1.5f;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	RootComp->SetCollisionProfileName(TEXT("BlockAll"));
	RootComp->SetSimulatePhysics(false);
	RootComp->SetSphereRadius(50.0f);


	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Batron/Meshes/SK_Batron.SK_Batron"));

	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(MeshAsset.Object);
		SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(SkeletalMeshComp);
	SpringArmComp->TargetArmLength = 500.0f;
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	SpringArmComp->SetRelativeRotation(FRotator(Pitch, 90.f, 0.f));
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

}


void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -RootComp->GetScaledSphereRadius()));
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsFlying)
	{
		Velocity.Z += Gravity * DeltaTime;
		AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);
	}

	/*
	if (Hit.IsValidBlockingHit())
	{
		Velocity.Z = 0.f;
		bIsFlying = false;
	}
	*/
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}

			if (PlayerController->FlyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FlyAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartFly
				);
			}

			if (PlayerController->FlyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FlyAction,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopFly
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}


void AMyPawn::Move(const FInputActionValue& value)
{

	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	const FVector RotZ = GetActorUpVector();
	FRotator ControlRot = FRotator(0.0, Controller->GetControlRotation().Yaw, 0.0f);

	const FVector ForwardDir = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
	const FVector RollDir = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Z);



	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(-ForwardDir * MoveInput.X * MovingSpeed, true);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(-RightDir * MoveInput.Y * MovingSpeed, true);
	}
		
}

void AMyPawn::StartFly(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		bIsFlying = true;
		Velocity.Z = JumpingSpeed;

	}
}

void AMyPawn::StopFly(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		bIsFlying = false;

	}
}

void AMyPawn::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	Yaw += LookInput.X * LookSensitivity;
	Pitch = FMath::Clamp(Pitch - LookInput.Y * LookSensitivity, -75.f, 75.f);

	if (!bIsFlying) 
	{
		SetActorRotation(FRotator(0.f, Yaw, 0.f));
		SpringArmComp->SetRelativeRotation(FRotator(Pitch, 90.f, 0.f));
	}
	else
	{
		SetActorRotation(FRotator(Pitch, Yaw, 0.f));
		
	}
}
