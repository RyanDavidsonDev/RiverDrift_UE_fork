// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RD_GameMode.h"
#include "Player/RD_PlayerController.h"
#include "Hexes/TileManager.h"
#include "UObject/ConstructorHelpers.h"

ARD_GameMode::ARD_GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARD_PlayerController::StaticClass();

	 //set default pawn class to our Blueprinted character
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_RDPlayerPawn")); //TODO
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Player/BP_RDPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}


}

void ARD_GameMode::BeginPlay()
{
	Super::BeginPlay();

		//UE_LOG(LogTemp, Warning, TEXT("ctf hi"))
	
	OnGameModeInitializedDelegate.Broadcast();

	//// Load the package first
	//UPackage* Package = LoadPackage(nullptr, TEXT("/Script/RiverDrift_UE"), LOAD_None);
	//if (!Package)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load /Script/RiverDrift_UE package!"));
	//	return;
	//}

	//// Now force the struct to be registered
	//(void)FDialogueQuestLookup::StaticStruct();

	//// Test
	//if (const UScriptStruct* Struct = FindObject<UScriptStruct>(ANY_PACKAGE, TEXT("FDialogueQuestLookup")))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ctf Found struct: %s"), *Struct->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ctf Struct still not found!"));
	//}

	//PrototypingManagerInstance =  GetWorld()->SpawnActor<ARDPrototypingManager>(PrototypingManagerFormat);
}


