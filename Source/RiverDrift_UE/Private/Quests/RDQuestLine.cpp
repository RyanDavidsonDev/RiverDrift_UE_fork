// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestLine.h"
#include "Hexes/TileData.h"

FRDProgressionCondition::FRDProgressionCondition()
{
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//EConditionType ConditionType;

	////A Unique ID/name passed from the 'prompting object' and used to lookup whether the prompting object is the same as in any of our active quests
	////corresponds to:
	//	//row name for landmarks
	//	//?? for dialogue
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FName OtherID;
}

FRDQuestObjective::FRDQuestObjective()
{

	////unique name for backend
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FName QuestID;

	////player facing name
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//FText QuestTitle;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//EConditionType ConditionType;

	////(optional) a dialogue scene to play when the player STARTS this quest Objective
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")
	//TObjectPtr<UDA_RDDialogueScene> InitializationScene;
}

void URDQuestLine::ProgressQuestline_Implementation()
{
}

FRDQuestObjective URDQuestLine::GetCurrentQuestObjective()
{
	return AllObjectives[CurrentObjectiveIndex];
}

int URDQuestLine::GetCurrentQuestObjectiveIndex()
{
	return CurrentObjectiveIndex;
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
