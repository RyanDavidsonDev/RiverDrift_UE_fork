// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "../HexLibrary.h"
//#include ""
#include "RD_PlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class ASpawnableTile;
class ATileManager;
class ARD_GameMode;
class ARD_PlayerPawn;

/**
 * 
 */
UCLASS()
class RIVERDRIFT_UE_API ARD_PlayerController : public APlayerController
{
	GENERATED_BODY()

	// --- VARIABLES ---

public:

	UPROPERTY(BlueprintReadOnly)
	bool bOverrideWater;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Tiles")
	ASpawnableTile* CurrentSelectedTile;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectTileClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectTileTouchAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OverrideWaterAction;


	///TODO: add input actions for move to tile and place tile
	///** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* SelectTileClickAction;

	///** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* SelectTileTouchAction;

	///** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* SelectTileClickAction;

	///** Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* SelectTileTouchAction;

	UPROPERTY(visibleAnywhere)
	TArray<TObjectPtr< ASpawnableTile>> TilesInRange;

	// --- FUNCTIONS
public:
	ARD_PlayerController();


	UFUNCTION()
	void BeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	//uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSelectTileTriggered();
	void OnTouchTriggered();
	void OnTouchReleased();

	void OnOverrideWaterTriggered();
	void OnOverrideWaterReleased();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable )
	bool CheckIfTileInRange(ASpawnableTile* tile);// only very basic (always true) c++ implementation, job for designers 



	void ActivateTile();

	TObjectPtr<ARD_GameMode> GameMode;
	//TObjectPtr<ATileManager> TileManager;
	

private:

	bool bPlayerIsTouchingScreen; // Is it a touch device

	ARD_PlayerPawn* RDPlayerPawn;

};
