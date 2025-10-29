// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/RDQuestManagerSubsystem.h"
#include "../RiverDrift_UE.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RD_PlayerController.h"
#include "Quests/RDQuestLine.h"



URDQuestManagerSubsystem::URDQuestManagerSubsystem()
{

}

void URDQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UE_LOG(LogTemp, Warning, TEXT("rcs: cpp init start"))

    ActiveConditions.Add(EConditionType::CE_Dialogue, TMap<FGuid,  TArray<TObjectPtr<URDQuestLine>>>());
    ActiveConditions.Add(EConditionType::CE_NewLandmark, TMap<FGuid, TArray<TObjectPtr<URDQuestLine>>>());
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
        TMap<FGuid, TArray<TObjectPtr<URDQuestLine>>> *Map = ActiveConditions.Find(FirstObjective.ProgressionConditionType);
        if (Map != nullptr) {

            TArray<TObjectPtr<URDQuestLine>>* Objectives = Map->Find(FirstObjective.OtherObjectQuestID);
            if (Objectives) {
                //if we're already storing a reference to this quest object, then the array already exists and we need to inser
                Objectives->Add(QuestLine);
            }
            else {
                //if not, make a new one
                TArray<TObjectPtr<URDQuestLine>> arr;
                arr.Add(QuestLine);
                Map->Add(FirstObjective.OtherObjectQuestID, arr);
            }


            //Map->Add(FirstObjective.ProgressionOtherObject.QuestID, QuestLine);
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

        //check to see if active conditions contains any conditions of the given type
        if (TMap<FGuid, TArray<TObjectPtr<URDQuestLine>>>* ConditionsOfGivenProgressionType
            = ActiveConditions.Find(ConditionType))
        {

            TArray<TObjectPtr< URDQuestLine>> QuestLinesLinkedToGivenID;
            //if it does, check to see whether any of those conditions match to the provided ID
            if (ConditionsOfGivenProgressionType->RemoveAndCopyValue(ObjectiveObjectID, QuestLinesLinkedToGivenID))
            {
                //removes it from the array automatically

                //this'll return an array, for each questline contained within that array, progress it
                for (TObjectPtr < URDQuestLine> Quest : QuestLinesLinkedToGivenID) {
                    ProgressQuestline(Quest);
                    //CTODO: step back over other maps to see if they also contain the 'given questline(s) to allow for multiple ways to advance the question

                }

                UE_LOG(QuestLog, Log, TEXT("successfully found the row. pretty sure that means the objective is completed"))
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

    //following lines are deprecated, unsure why I was removing them in this function, but keeping them commented in case there was some reason
    ////try to look up the 'nested map' and remove the condition
    //if (TMap<FGuid, TArray<TObjectPtr<URDQuestLine>>>* ConditionsOfProgressionType
    //    = ActiveConditions.Find(PreviousObjective.ProgressionConditionType)) {
    //    

    //    
    //    if (ActiveConditionOfProgressionType->Contains(
    //            PreviousObjective.OtherObjectQuestID)) {
    //        //if we find the value



    //        ActiveConditionOfProgressionType->Remove(
    //            PreviousObjective.OtherObjectQuestID);
    //        //UE_LOG(QuestLog, Log, TEXT("removing prev objective, po is %s"), *PreviousObjective.CompletionScene->GetName())


    //    }
    //    else {
    //        UE_LOG(QuestLog, Warning, TEXT("not found"))
    //    }

    //}
    //else {
    //    UE_LOG(QuestLog, Error, TEXT("progress questline failed to grab the nested map"))

    //}

    if (QuestLine->ProgressQuestline()) {//always progresses questline, returns true if questline completed

        PlayProgressionScene(PreviousObjective, QuestLine, true);
        UE_LOG(QuestLog, Log, TEXT("IN FACT, YOU COMPLETED THE WHOLE QUESTLINE!! BIG CONGRATS!! \n - \n -"))
    }
    else {

        FRDQuestObjective NewObjective = QuestLine->GetCurrentQuestObjective();
        PlayProgressionScene(PreviousObjective, QuestLine, false);


        //ctodo: do this as a function for both here and startquestline
        if (TMap<FGuid, TArray<TObjectPtr<URDQuestLine>>>* MapOfIDsByConditionType
            = ActiveConditions.Find(NewObjective.ProgressionConditionType) ) {
            //Get the map of IDs of the givenCondition type

            //then, check whether that map currently contains an array matched to the other's quest ID
            if (TArray<TObjectPtr<URDQuestLine>>* QuestLines 
                = MapOfIDsByConditionType->Find(NewObjective.OtherObjectQuestID)) {
                //if it does, just add our quest to the array
                QuestLines->Add(QuestLine);
            }
            else {
                //if not, make a new one
                TArray<TObjectPtr<URDQuestLine>> arr;
                arr.Add(QuestLine);
                MapOfIDsByConditionType->Add(NewObjective.OtherObjectQuestID, arr);
            }
        }

            
        UE_LOG(QuestLog, Log, TEXT("your next objective is %s"), *NewObjective.ObjectiveTitle.ToString())
    }

}


void URDQuestManagerSubsystem::InitializeSubsystem_Implementation()
{
}


void URDQuestManagerSubsystem::DeinitializeSubsystem_Implementation()
{
}

void URDQuestManagerSubsystem::PlayProgressionScene_Implementation(FRDQuestObjective PrevObjective, URDQuestLine* Quest, bool Completed)
{
    //breakpoint
    UE_LOG(QuestLog, Log, TEXT("playprogscene_implementation called"))
    if (!IsValid(PrevObjective.CompletionScene)) {
        //do we want to do anything if the scene isn't set? 
        // My inclination is that that's the way for designers to avoid having anything pop up.
        // alternatively we could just play it with a call to the basic scene. 
        // I think ideally the designers would set a bool for whether or not they want to 
        // play that scene, but that's not necessary for this demo so I'm moving on
        
        //Cast< ARD_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->StartDialogueScene(PrevObjective.CompletionScene, TEXT("congrats"));
        //return;
        UE_LOG(QuestLog, Log, TEXT("wasNotValid"))

    }
    //UE_LOG(QuestLog, Log, TEXT("prevobj is %s, cpl scene is %s"), * PrevObjective.ObjectiveTitle.ToString(), *PrevObjective.CompletionScene->GetName())

    Cast< ARD_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->BeginDialogueScene(PrevObjective.CompletionScene);



    FText Intro;
    if (PrevObjective.PlayDefaultProgressionScene) {
        if (!IsValid(PrevObjective.DefaultProgressionScene)) {

            UE_LOG(QuestLog, Error, TEXT("default progression scene isn't valid"))
                return;
        }
        UDA_RDDialogueScene* CopiedDefault = DuplicateObject<UDA_RDDialogueScene>(PrevObjective.DefaultProgressionScene, GetTransientPackage());

        UE_LOG(QuestLog, Log, TEXT("2"))
            if (Completed) {
                Intro = FText::Format(FText::FromString(
                    TEXT("You completed the {0} quest. congrats!")), Quest->QuestTitle);
                UE_LOG(QuestLog, Log, TEXT("3"))
            }
            else {
                Intro = FText::Format(FText::FromString(
                    TEXT("You completed the {0} step of the {1} quest. your next objective is to {2}.")),
                    PrevObjective.ObjectiveTitle,
                    Quest->QuestTitle,
                    Quest->GetCurrentQuestObjective().ObjectiveTitle
            );

            UE_LOG(QuestLog, Log, TEXT("4"))
            }

        CopiedDefault->Slides[0].DialogueMainBody = Intro;

        //DefaultProgressionsc
        UE_LOG(QuestLog, Log, TEXT("prevobj is &s, scene is"))


        Cast< ARD_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->BeginDialogueScene(CopiedDefault);

    }

    UE_LOG(QuestLog, Log, TEXT("5"))

    

}


