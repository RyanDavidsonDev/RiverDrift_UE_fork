// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hexes/TileData.h"
#include "Quests/QuestLookup.h"
#include "RDQuestLine.generated.h"

class UDA_RDDialogueScene;
//struct FLandmarkData;

static const FString BasicProgressionScenePath = TEXT("/Game/Data/cutscenes/DA_DS_BasicQuestAdvancement.DA_DS_BasicQuestAdvancement");

UENUM(BlueprintType)
enum class EConditionType : uint8
{
	UNSET UMETA(DisplayName = "Unset"),
	CE_NewLandmark UMETA(DisplayName = "New Landmark"),
	CE_Dialogue UMETA(DisplayName = "Dialogue"),
};


USTRUCT(BlueprintType)
struct  FRDProgressionCondition
{
	GENERATED_BODY()
public:
	FRDProgressionCondition();
	virtual ~FRDProgressionCondition() {} ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	EConditionType ConditionType;

	//A Unique ID/name passed from the 'prompting object' and used to lookup whether the prompting object is the same as in any of our active quests
	//corresponds to:
		//row name for landmarks
		//?? for dialogue
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FGuid OtherID;
	//FTableRowBase OtherObject;

	//FGuid GetGuid();
};

//USTRUCT(BlueprintType)
//struct  FRDProgressionCondition_Landmark : public FRDProgressionCondition
//{
//	GENERATED_BODY()
//public:
//	FRDProgressionCondition_Landmark();
//
//	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
//	//EConditionType ConditionType = EConditionType::CE_NewLandmark;
//
//	//A Unique ID/name passed from the 'prompting object' and used to lookup whether the prompting object is the same as in any of our active quests
//	//corresponds to:
//		//row name for landmarks
//		//?? for dialogue
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
//	FLandmarkData Landmark;
//
//	virtual FGuid GetGuid() override;
//	
//};

USTRUCT(BlueprintType)
struct FRDQuestObjective
{
	GENERATED_BODY()
public:
	FRDQuestObjective();

	////unique name for backend
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FName ObjectiveID;

	//player facing name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FText ObjectiveTitle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	EConditionType ProgressionConditionType;


	//Used to set the reference to what other object is associated with this quest. 
	// When the player "interacts" with this object, the quest will progress
	// - Table: needs to be set to the landmarks data table or the dialogue quest lookup table
	// - Row: the row within that table that will identify the object
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FDataTableRowHandle ProgressionOtherObjectLookup;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FQuestLookup ProgressionOtherObject;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	FGuid OtherObjectQuestID;


	//Used to set the reference to what other object is associated with this quest. 
	// When the player "interacts" with this object, the quest will progress
	// - Table: needs to be set to the landmarks data table or the dialogue quest lookup table
	// - Row: the row within that table that will identify the object
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FDataTableRowHandle OtherObjectRowHandle;
		
	//(optional) a dialogue scene to play when the player FINISHES this quest Objective
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	TObjectPtr<UDA_RDDialogueScene> CompletionScene;

	//if true, will play a basic progression scene BEFORE other completion scene
	// scene will basically be "you completed the objective, here's your next one"
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	bool PlayDefaultProgressionScene;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
	TObjectPtr<UDA_RDDialogueScene> DefaultProgressionScene;
	//UDA_RDDialogueScene InitializationScene;


	
};
//
//USTRUCT(BlueprintType)
//struct  FRDQuestLine
//{
//	GENERATED_BODY()
//public:
//	FRDQuestObjective();
//
//};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RIVERDRIFT_UE_API URDQuestLine : public UDataAsset
{
	GENERATED_BODY()

// --- Variables ---

public:
	


	//player facing name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FText QuestTitle;

	//backend unique name
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quests")
	FGuid QuestID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	TArray<FRDQuestObjective> AllObjectives;

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quests")
	FRDQuestObjective GetCurrentQuestObjective();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quests")
	void GetCurrentQuestObjective_BP(UPARAM(DisplayName = "Objective") FRDQuestObjective& Objective);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quests")
	void GetCurrentQuestObjectiveIndex(UPARAM(DisplayName = "Index") int &Index);
	 
	void InitializeFromRowHandle();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

protected:
	UPROPERTY(VisibleAnywhere, Category = "Quests")
	int CurrentObjectiveIndex =0;



private:


	// --- Functions ---
public:
#if WITH_EDITOR
	virtual void PostInitProperties() override;

	virtual void PostLoad() override;

	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
#endif

	UFUNCTION(BlueprintNativeEvent)
	bool ProgressQuestline();

	void ResetIndex() { CurrentObjectiveIndex = 0; };
};
