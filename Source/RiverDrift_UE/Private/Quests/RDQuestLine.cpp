// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestLine.h"
#include "Hexes/TileData.h"

//URDQuestLine::URDQuestLine() {
//	QuestID = FGuid::NewGuid();
//}

FRDProgressionCondition::FRDProgressionCondition()
{
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	ConditionType = EConditionType::UNSET;

	//A Unique ID/name passed from the 'prompting object' and used to lookup whether the prompting object is the same as in any of our active quests
	//corresponds to:
		//row name for landmarks
		//?? for dialogue
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	OtherID = FGuid();
}

FRDQuestObjective::FRDQuestObjective()
{
	DefaultProgressionScene = LoadObject<UDA_RDDialogueScene>(nullptr, *BasicProgressionScenePath);
	//unique name for backend
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FName QuestID = FName();

	////player facing name
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FText QuestTitle = FText();

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//EConditionType ConditionType = EConditionType::UNSET;

	////(optional) a dialogue scene to play when the player STARTS this quest Objective
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//DefaultProgressionScene = LoadObject<UDA_RDDialogueScene>(nullptr, TEXT("Game/Data/Dialogue"));
}

bool URDQuestLine::ProgressQuestline_Implementation()
{
	if (AllObjectives.Num() <= CurrentObjectiveIndex + 1) {
		UE_LOG(QuestLog, Log, TEXT("questline completed"))
			return true;
	}
	else {
		UE_LOG(QuestLog, Log, TEXT("inc'ing objective"))
		CurrentObjectiveIndex++;

		return false;
	}
}

#if WITH_EDITOR
void URDQuestLine::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject) && !QuestID.IsValid()) {
		QuestID = FGuid::NewGuid();

		Modify();
		MarkPackageDirty();
	}
}

void URDQuestLine::PostLoad()
{
	Super::PostLoad();

	if (!QuestID.IsValid())
	{
		QuestID = FGuid::NewGuid();
		Modify();
		MarkPackageDirty();
	}

	
}

void URDQuestLine::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		QuestID = FGuid::NewGuid();
	}
}
#endif

FRDQuestObjective URDQuestLine::GetCurrentQuestObjective()
{
	return AllObjectives[CurrentObjectiveIndex];
}

void URDQuestLine::GetCurrentQuestObjective_BP(FRDQuestObjective &Objective)
{
	Objective = GetCurrentQuestObjective();
}

void URDQuestLine::GetCurrentQuestObjectiveIndex(int &Index)
{
	Index = CurrentObjectiveIndex;
}

//FRDProgressionCondition_Landmark::FRDProgressionCondition_Landmark()
//{
//	//FRDProgressionCondition();
//	ConditionType = EConditionType::CE_NewLandmark;
//}
//
//virtual FGuid FRDProgressionCondition_Landmark::GetGuid() override {
//	//Landmark
//	return Landmark.
//}

void URDQuestLine::InitializeFromRowHandle() {

	UE_LOG(QuestLog, Log, TEXT("initializing"))
	for (FRDQuestObjective& Objective : AllObjectives) {
		if (Objective.OtherObjectRowHandle.DataTable) {
			if (FQuestLookup* Row = Objective.OtherObjectRowHandle.GetRow<FQuestLookup>(TEXT("initialize from row handle"))) {
				Objective.OtherObjectQuestID= Row->QuestID;
			}
		}
	}
}

#if WITH_EDITOR
void URDQuestLine::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	UE_LOG(QuestLog, Log, TEXT("received edit, propname is %s"), *PropName.ToString())

	//if (PropName == GET_MEMBER_NAME_CHECKED(FRDQuestObjective, QuestRowHandle.RowName ) || PropName == GET_MEMBER_NAME_CHECKED(FRDQuestObjective, QuestRowHandle.TableName)) {
	if (PropName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName )) {
		InitializeFromRowHandle();
	}
}
#endif