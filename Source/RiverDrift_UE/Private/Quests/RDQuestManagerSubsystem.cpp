// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestManagerSubsystem.h"
#include "Quests/RDQuestLine.h"


URDQuestManagerSubsystem::URDQuestManagerSubsystem()
{

}


void URDQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp init start"))

    ActiveConditions.Add(EConditionType::CE_Dialogue, TMap<FName, TObjectPtr<URDQuestLine>>());
    ActiveConditions.Add(EConditionType::CE_NewLandmark, TMap<FName, TObjectPtr<URDQuestLine>>());
    Super::Initialize(Collection);
    InitializeSubsystem();

    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp init end"))

}


void URDQuestManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
    DeinitializeSubsystem();
}

void URDQuestManagerSubsystem::StartQuestline_Implementation(URDQuestLine* QuestLine)
{
    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp sq start"))
        ActiveQuestLines.Add(QuestLine);
    UE_LOG(LogTemp, Warning, TEXT("StartQuestline C++ version is being called") )


    if (QuestLine->AllObjectives.Num() >= 1) {
        FRDProgressionCondition FirstObjective = QuestLine->AllObjectives[0].ProgressionCondition;
        TMap < FName, TObjectPtr<URDQuestLine>>* map = ActiveConditions.Find(QuestLine->AllObjectives[0].ProgressionCondition.ConditionType);
        if (map != nullptr) {
            map->Add(QuestLine->AllObjectives[0].ProgressionCondition.OtherID, QuestLine);

            UE_LOG(LogTemp, Warning, TEXT("SUCCESS"))
        }
        else {

            UE_LOG(LogTemp, Error, TEXT("map not valid"))
        }
            

    }
    else {
        UE_LOG(LogTemp, Error, TEXT("attempted to start a questline that has no Objectives in it"))
    }
    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp sq end"))
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

