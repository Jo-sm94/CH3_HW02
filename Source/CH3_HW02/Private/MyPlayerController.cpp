#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	: IMC(nullptr),
	MoveAction(nullptr),
	FlyAction(nullptr),
	LookAction(nullptr)
{

}

void AMyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC)
			{
				SubSystem->AddMappingContext(IMC, 0);
			}
		}
	}
}