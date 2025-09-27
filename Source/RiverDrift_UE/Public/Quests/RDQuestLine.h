// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Hexes/TileData.h"
#include "RDQuestLine.generated.h"

class UDA_RDDialogueScene;
//struct FLandmarkData;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FQuestLookup ProgressionOtherObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	FDataTableRowHandle RowHandle;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FRDProgressionCondition ProgressionCondition;

	//(optional) a dialogue scene to play when the player STARTS this quest Objective
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	TObjectPtr<UDA_RDDialogueScene> InitializationScene;
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
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FGuid QuestID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	TArray<FRDQuestObjective> AllObjectives;

	UFUNCTION(BlueprintGetter, Category = "Quests")
	FRDQuestObjective GetCurrentQuestObjective();

	UFUNCTION(BlueprintGetter, Category = "Quests")
	int GetCurrentQuestObjectiveIndex();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Quests")
	int CurrentObjectiveIndex;



private:


	// --- Functions ---
public:

	UFUNCTION(BlueprintNativeEvent)
	void ProgressQuestline();


};
