// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestManagerSubsystem.h"
#include "../RiverDrift_UE.h"
#include "Quests/RDQuestLine.h"



URDQuestManagerSubsystem::URDQuestManagerSubsystem()
{

}


void URDQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp init start"))

    ActiveConditions.Add(EConditionType::CE_Dialogue, TMap<FGuid, TObjectPtr<URDQuestLine>>());
    ActiveConditions.Add(EConditionType::CE_NewLandmark, TMap<FGuid, TObjectPtr<URDQuestLine>>());
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
        FRDQuestObjective FirstObjective = QuestLine->AllObjectives[0];
        TMap<FGuid, TObjectPtr<URDQuestLine >> *Map = ActiveConditions.Find(FirstObjective.ProgressionConditionType);
        if (Map != nullptr) {
            //Map->Add(FirstObjective.ProgressionOtherObject.QuestID, QuestLine);
            Map->Add(FirstObjective.RowHandle.GetRow<FQuestLookup>(TEXT("START QUESTLINE"))->QuestID, QuestLine );
                //FirstObjective.ProgressionOtherObject, QuestLine);

            //UE_LOG(LogTemp, Warning, TEXT("SUCCESS"))
            UE_LOG(QuestLog, Log, TEXT("Start Questline successful? size of actives is% d, actives[questline].size() is% d "),
                ActiveConditions.Num(),
                Map->Num())
        }
        else {

            UE_LOG(LogTemp, Error, TEXT("map not valid"))
        }
            


    }
    else {
        UE_LOG(LogTemp, Error, TEXT("attempted to start a questline that has no Objectives in it"))
    }
    UE_LOG(LogTemp, Log, TEXT("rcs: cpp sq end"))
}

void URDQuestManagerSubsystem::ProgressQuestline_Implementation(FName Questline)
{

}


void URDQuestManagerSubsystem::CheckProgression_Implementation(EConditionType ConditionType, FGuid OtherID)
{
    UE_LOG(QuestLog, Log, TEXT("CheckProgression called for condition %s, OtherID is %s"), *UEnum::GetValueAsString(ConditionType), *OtherID.ToString())
    TMap<FGuid, TObjectPtr<URDQuestLine>>* Map = ActiveConditions.Find(ConditionType);
    if (TObjectPtr< URDQuestLine> QuestLine = *Map->Find(OtherID)) {

        UE_LOG(QuestLog, Log, TEXT("successfully found the row"))

    }



}

void URDQuestManagerSubsystem::InitializeSubsystem_Implementation()
{
}


void URDQuestManagerSubsystem::DeinitializeSubsystem_Implementation()
{
}

