// Copyright Epic Games, Inc. All Rights Reserved.

#include "EveInventoryGameMode.h"
#include "EveInventoryPlayerController.h"
#include "EveInventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEveInventoryGameMode::AEveInventoryGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AEveInventoryPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}