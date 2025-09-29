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
    ActiveQuestLines.Add(QuestLine->QuestID, QuestLine);
    //QuestLineProgressionIndex.Add(QuestLine, 0);
    QuestLine->ResetIndex();

    if (QuestLine->AllObjectives.Num() >= 1) {
        FRDQuestObjective FirstObjective = QuestLine->AllObjectives[0];
        TMap<FGuid, TObjectPtr<URDQuestLine >> *Map = ActiveConditions.Find(FirstObjective.ProgressionConditionType);
        if (Map != nullptr) {
            //Map->Add(FirstObjective.ProgressionOtherObject.QuestID, QuestLine);
            Map->Add(FirstObjective.ProgressionOtherObject.QuestID, QuestLine );
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
}

void URDQuestManagerSubsystem::CheckProgression_Implementation(EConditionType ConditionType, FGuid ObjectiveObjectID)
{
    UE_LOG(QuestLog, Log, TEXT("CheckProgression called for condition %s, OtherID is %s"), *UEnum::GetValueAsString(ConditionType), *ObjectiveObjectID.ToString())
        if (TMap<FGuid, TObjectPtr<URDQuestLine>>* Map = ActiveConditions.Find(ConditionType))
        {
            if (TObjectPtr< URDQuestLine>* QuestLine = Map->Find(ObjectiveObjectID))
            {
                UE_LOG(QuestLog, Log, TEXT("successfully found the row. pretty sure that means the objective is completed"))

                    ProgressQuestline((*QuestLine));
            }
            else {
                UE_LOG(QuestLog, Log, TEXT("failed to find the row. Objective ID is %s, "), *ObjectiveObjectID.ToString())
            }
        }
        else {
            UE_LOG(QuestLog, Log, TEXT("2"))
        }
}

void URDQuestManagerSubsystem::ProgressQuestline_Implementation(URDQuestLine* QuestLine)
{
    UE_LOG(QuestLog, Log,
        TEXT("pqic"))

    if (!ActiveQuestLines.Find(QuestLine->QuestID)) {
        UE_LOG(QuestLog, Error, 
            TEXT("progress questline was passed a questline that isn't active (supposedly). Either it was passed an incorrect ql, or activeQL's is borkedquest ID is %s"), *QuestLine->QuestID.ToString())
    }

    UE_LOG(QuestLog, Log, TEXT("congratulations! you completed the %s objective in the %s questline, you should probably be seeing a proper UI element instead of (or at least in addition to) this debug log"),
        *QuestLine->GetCurrentQuestObjective().ObjectiveTitle.ToString(), *QuestLine->QuestTitle.ToString())

    FRDQuestObjective PreviousObjective = QuestLine->GetCurrentQuestObjective();
    //FRDQuestObjective PreviousObjective = QuestLine->AllObjectives[*QuestLineProgressionIndex.Find(QuestLine)];

    //try to look up the 'nested map' and remove the condition
    if (TMap<FGuid, TObjectPtr<URDQuestLine>>* ActiveConditionOfProgressionType 
            = ActiveConditions.Find(PreviousObjective.ProgressionConditionType)) {
        if (ActiveConditionOfProgressionType->Contains(
                PreviousObjective.ProgressionOtherObject.QuestID)) {
            ActiveConditionOfProgressionType->Remove(
                PreviousObjective.ProgressionOtherObject.QuestID);

        }
        else {
            UE_LOG(QuestLog, Warning, TEXT("not found"))
        }

    }
    else {
        UE_LOG(QuestLog, Error, TEXT("progress questline failed to grab the nested map"))

    }

    if (QuestLine->ProgressQuestline()) {
        UE_LOG(QuestLog, Log, TEXT("IN FACT, YOU COMPLETED THE WHOLE QUESTLINE!! BIG CONGRATS!! \n - \n -"))
    }
    else {
        FRDQuestObjective NewObjective = QuestLine->GetCurrentQuestObjective();
            
        ActiveConditions.Find(NewObjective.ProgressionConditionType)->Add(
            NewObjective.ProgressionOtherObject.QuestID, QuestLine);
            
        UE_LOG(QuestLog, Log, TEXT("your next objective is %s"), *NewObjective.ObjectiveTitle.ToString())
    }

}


void URDQuestManagerSubsystem::InitializeSubsystem_Implementation()
{
}


void URDQuestManagerSubsystem::DeinitializeSubsystem_Implementation()
{
}

