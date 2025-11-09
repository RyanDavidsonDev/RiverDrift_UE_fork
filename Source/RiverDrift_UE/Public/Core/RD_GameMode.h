// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Quests/QuestLookup.h"
#include "RD_GameMode.generated.h"

//static const FString TablePath = TEXT("/Game/Data/cutscenes/DT_DialogueQuestLookup.DT_DialogueQuestLookup");

/**
 * 
 */
class ATileManager;
class ARDPrototypingManager;
class UDA_RDPrototypeAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameModeInitializedSignature);

UCLASS()
class RIVERDRIFT_UE_API ARD_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	// --- VARS ---

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TObjectPtr<ATileManager> TileManager;
	
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDA_RDPrototypeAsset> PrototypingAsset;

	TMap<FGuid, TObjectPtr<FDialogueQuestLookup>> QuestLookupMap;

	//void OnGameModeInitialized();

	UPROPERTY(BlueprintAssignable)
	FOnGameModeInitializedSignature OnGameModeInitializedDelegate;
	// --- FUNCTIONS ---

public:
	ARD_GameMode();


protected:
	virtual void BeginPlay() override;


};
