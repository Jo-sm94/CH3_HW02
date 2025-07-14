#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
struct FInputActionValue;

UCLASS()
class CH3_HW02_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root");
	USphereComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh");
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;
	FHitResult Hit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed");
	float MovingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed");
	float JumpingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed");
	float RollSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly");
	bool bIsFlying;
	FVector Velocity;
	float Gravity;


	float Yaw;
	float Pitch;
	float Roll;
	float LookSensitivity;

	UFUNCTION()
	void Move(const FInputActionValue& value);  //const FInputActionValue& value 구조체이기 때문에 그냥 복사해오면 메모리가큼 &을 사용하여 찐 가져옴
	UFUNCTION()
	void StartFly(const FInputActionValue& value);
	UFUNCTION()
	void StopFly(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
};
