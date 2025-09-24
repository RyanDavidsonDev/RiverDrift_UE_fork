// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestManagerSubsystem.h"
#include "Quests/RDQuestLine.h"


URDQuestManagerSubsystem::URDQuestManagerSubsystem()
{

}


void URDQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("HELLO WORLD!"))
    InitializeSubsystem();
    
    //ActiveConditions.Add(EConditionType::CE_Dialogue, TMap<FName, TObjectPtr<URDQuestLine>>());
    //ActiveConditions.Add(EConditionType::CE_NewLandmark, TMap<FName, TObjectPtr<URDQuestLine>>());
}


void URDQuestManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
    DeinitializeSubsystem();
}

void URDQuestManagerSubsystem::StartQuestline_Implementation(URDQuestLine* QuestLine)
{
    ActiveQuestLines.Add(QuestLine);
    UE_LOG(LogTemp, Error, TEXT("uniqueCall"))


    //if (QuestLine->AllObjectives.Num() >= 1) {
    //    FRDProgressionCondition FirstObjective = QuestLine->AllObjectives[0].ProgressionCondition;
    //    ActiveConditions.Find(QuestLine->AllObjectives[0].ProgressionCondition.ConditionType)
    //        ->Add(QuestLine->AllObjectives[0].ProgressionCondition.OtherID, QuestLine);
    //}
    //else {
    //    UE_LOG(LogTemp, Error, TEXT("attempted to start a questline that has no Objectives in it"))
    //}
}

void URDQuestManagerSubsystem::ProgressQuestline_Implementation(FName Questline)
{
}


void URDQuestManagerSubsystem::CheckProgression_Implementation(EConditionType ConditionType, FName OtherID)
{


}

void URDQuestManagerSubsystem::InitializeSubsystem_Implementation()
{
}


void URDQuestManagerSubsystem::DeinitializeSubsystem_Implementation()
{
}
