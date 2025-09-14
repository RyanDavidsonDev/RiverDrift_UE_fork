// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RD_PlayerController.h"
#include "Player/RD_PlayerPawn.h"
#include "Core/DA_RDPrototypeAsset.h"
#include "Core/RD_GameMode.h"
#include "Hexes/SpawnableTile.h"
#include "Hexes/TileManager.h"
#include "Hexes/RDSpawnableLandmark.h"
#include "HexLibrary.h"
#include "Logging/StructuredLog.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"

ARD_PlayerController::ARD_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARD_PlayerController::BeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOGFMT(LogTemp, Log, "begining overlap callback");
	ASpawnableTile* tile = Cast<ASpawnableTile>(OtherActor);
	if (IsValid(tile)) {
		UE_LOGFMT(LogTemp, Log, "overlap is a tile");
		TilesInRange.Add(tile);

	}
}

void ARD_PlayerController::EndOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOGFMT(LogTemp, Log, "ending overlap callback");

	ASpawnableTile* tile = Cast<ASpawnableTile>(OtherActor);
	if (IsValid(tile)) {
		UE_LOGFMT(LogTemp, Log, " end overlap is a tile, removing from array");
		TilesInRange.Remove(tile);

	}
}

void ARD_PlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


	GameMode = Cast<ARD_GameMode>(GetWorld()->GetAuthGameMode());
	//TileManager = GameMode->TileManager;
	if (IsValid(GameMode)) {
		UE_LOGFMT(LogTemp, Log, "gamemode is properly set");
	}
	else {
		UE_LOGFMT(LogTemp, Warning, "gamemode is properly INVALID");

	}
	APawn* pawn = GetPawn();
	RDPlayerPawn = Cast<ARD_PlayerPawn>(pawn);
	if (!IsValid(RDPlayerPawn)) {
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn not set!!!"))
	}


	//if (IsValid(TileManager)) {
	//	UE_LOGFMT(LogTemp, Log, "TileManager is properly set");
	//}
	//else {
	//	UE_LOGFMT(LogTemp, Warning, "TileManager is INVALID");		
	//}
}

void ARD_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SelectTileClickAction, ETriggerEvent::Started, this, &ARD_PlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SelectTileClickAction, ETriggerEvent::Triggered, this, &ARD_PlayerController::OnSelectTileTriggered);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SelectTileTouchAction, ETriggerEvent::Started, this, &ARD_PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SelectTileTouchAction, ETriggerEvent::Triggered, this, &ARD_PlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SelectTileTouchAction, ETriggerEvent::Completed, this, &ARD_PlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SelectTileTouchAction, ETriggerEvent::Canceled, this, &ARD_PlayerController::OnTouchReleased);
		
		EnhancedInputComponent->BindAction(OverrideWaterAction, ETriggerEvent::Started, this, &ARD_PlayerController::OnOverrideWaterTriggered);
		EnhancedInputComponent->BindAction(OverrideWaterAction, ETriggerEvent::Completed, this, &ARD_PlayerController::OnOverrideWaterReleased);



	}
	else
	{
		UE_LOGFMT(LogTemp, Log, "{0} Failed to find an Enhanced Input Component!This template is built to use the Enhanced Input system.If you intend to use the legacy system, then you will need to update this C++ file.", * GetNameSafe(this));
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARD_PlayerController::OnInputStarted()
{
	OnSelectTileTriggered();
}

void ARD_PlayerController::OnSelectTileTriggered()
{

	UE_LOGFMT(LogTemp, Log, "Player input detected");

	//raytracing method, works but feels needlessly expensive
	ETraceTypeQuery traceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(traceChannel, false, HitResult);

	if (HitResult.bBlockingHit) {
		ASpawnableTile* tile;

		tile = Cast<ASpawnableTile>(HitResult.GetActor());
		if (IsValid(tile)) {//ensure that the actor we found was correctly cast to a tile - theoretically once I tweak trace channels this shouldn't ever return false, but I always cast on the side of caution
			UE_LOGFMT(LogTemp, Log, "we got a hit, pos is {0} channel was {1}", tile->HexCoord.ToString(), ECollisionChannel::ECC_GameTraceChannel2);

			if (!CheckIfTileInRange(tile)) {
				UE_LOGFMT(LogTemp, Log, "player tried selecting a tile that they're not in range for. exiting");
				return;
			}


			//TODO: check whether the tile is "in range" of player - will we still be using the interaction box?
			if (IsValid(CurrentSelectedTile)) {



				if (CurrentSelectedTile == tile) {
					UE_LOGFMT(LogTemp, Log, "player has clicked current tile twice over. activating that tile");
					
					ActivateTile();
				}
				else {
					UE_LOGFMT(LogTemp, Log, "player has clicked a tile other than the currently selected tile. selecting that tile");
					CurrentSelectedTile->DeselectTile();
					CurrentSelectedTile = tile;
					tile->SelectTile();
				}
			}
			else {
				UE_LOGFMT(LogTemp, Log, "player has clicked a new tile for the very first time. selecting that tile");
				CurrentSelectedTile = tile;
				tile->SelectTile();

			}
		}
		else {

			ARDSpawnableLandmark* Landmark;
			Landmark = Cast<ARDSpawnableLandmark>(HitResult.GetActor());
			if (IsValid(Landmark)) {
				UE_LOGFMT(LogTemp, Log, "Player clicked a landmark, time to upgrade it");


				Landmark->SetIsPotential(false);
			}
			else {
				UE_LOGFMT(LogTemp, Warning, "player tile raycast somehow returned an actor that isn't a tile. something is wrong with your collisions");
			}


		}
	}
	else {
		UE_LOGFMT(LogTemp, Log, "player has clicked into the void or somehow hit something that wasn't a tile. should we unset the currently selected tile?");

	}
}


void ARD_PlayerController::OnTouchTriggered()
{
	UE_LOGFMT(LogTemp, Log, "Player selected a tile through a screen touch rather than a click, do we need to do anything differently here?");
	OnSelectTileTriggered();
}

void ARD_PlayerController::OnTouchReleased()
{
}

void ARD_PlayerController::OnOverrideWaterTriggered()
{
	if (GameMode->PrototypingAsset->bAllowOverridingWater) {
		bOverrideWater = true;
		GameMode->TileManager->D_OnTileGeneratedDelegate.Broadcast(GameMode->TileManager->LookupTileType(ETileType::TE_River, "override water triggered"));
	}
}

void ARD_PlayerController::OnOverrideWaterReleased()
{
	UE_LOGFMT(LogTemp, Log, "overriding water false");

	if (GameMode->PrototypingAsset->bAllowOverridingWater) {
		bOverrideWater = false;
		GameMode->TileManager->D_OnTileGeneratedDelegate.Broadcast(GameMode->TileManager->GetNextTileToPlace(false));

	}
}

bool ARD_PlayerController::CheckIfTileInRange_Implementation(ASpawnableTile* tile)
{
	return TilesInRange.Contains(tile);
}


void ARD_PlayerController::ActivateTile()
{
	if (!CheckIfTileInRange(CurrentSelectedTile)) {
		UE_LOGFMT(LogTemp, Log, "player tried activating a tile that they're not in range for. exiting");
		return;
	}
	
	if (!IsValid(GameMode)) {
		GameMode = Cast<ARD_GameMode>(GetWorld()->GetAuthGameMode());
		UE_LOGFMT(LogTemp, Error, "Player controller's GameModeRef was not valid, double check the logic for retrieving it in beginPlay()");
	}

	FTileData format;
	
	switch (CurrentSelectedTile->TileType.ETileType) {
	case(ETileType::TE_Blank):
		if (bOverrideWater) {

			FName name = UEnum::GetValueAsName(ETileType::TE_River);

			UE_LOGFMT(LogTemp, Log, "looking up tile type, name is {0}", name);
			format = GameMode->TileManager->LookupTileType(ETileType::TE_River, TEXT("activate tile with river override active"));
			//GameMode->TileManager->UpgradeTile()
		}
		else {
			format = GameMode->TileManager->GetNextTileToPlace();

		}
		GameMode->TileManager->UpgradeTile(format, CurrentSelectedTile);


		//CurrentSelectedTile->UpgradeTile(format);
		UE_LOGFMT(LogTemp, Log, "player activated blank tile, time to upgrade it");
		break;
	case(ETileType::TE_River):
		//need to implement boat movement :)
		UE_LOGFMT(LogTemp, Log, "player activated River tile, move to tile");
		RDPlayerPawn->MoveToTile(CurrentSelectedTile);
		break;
	default:
		UE_LOGFMT(LogTemp, Log,
			"player activated a different tile (tiletype {0}, do we want to do something different here? should it be interactable?",
			UEnum::GetValueAsString(CurrentSelectedTile->TileType.ETileType));
		break;
	}


	CurrentSelectedTile->DeselectTile();
	CurrentSelectedTile = nullptr;

}
