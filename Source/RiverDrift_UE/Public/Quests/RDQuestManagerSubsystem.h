// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableUtils.h"
#include "Subsystems/WorldSubsystem.h"
//#include "Enumerators/QuestEnums.h"
#include "Quests/RDQuestline.h"
#include "Misc/Guid.h"
#include "RDQuestManagerSubsystem.generated.h"

//class URDQuestLine;

UCLASS(Blueprintable)
class RIVERDRIFT_UE_API URDQuestManagerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
    // --- variables ---

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quests")

    TMap<FGuid, TObjectPtr<URDQuestLine>> ActiveQuestLines;
    

protected:

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap < EConditionType, TMap<FGuid, TObjectPtr<URDQuestLine>>> ActiveConditions;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    //U

    // --- Functions ---
public:


    URDQuestManagerSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    //otherID follows conventions set by the progressionCondition struct. landmarks are the row name.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    //void CheckProgression(EConditionType ConditionType, FGuid ObjectiveObjectID);
    void CheckProgression(EConditionType ConditionType, FGuid ObjectiveObjectID);
    //UFUNCTION(BlueprintGetter)
    
    //TMap < EConditionType, TMap<FName, TObjectPtr<URDQuestLine>>> GetActiveConditions() { return ActiveConditions; };
    
protected:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void InitializeSubsystem();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void DeinitializeSubsystem();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void StartQuestline(URDQuestLine* QuestLine);

    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void ProgressQuestline(URDQuestLine* QuestLine);


    UFUNCTION(BlueprintNativeEvent)
    void PlayProgressionScene(FRDQuestObjective PrevObjective, URDQuestLine* Quest, bool Completed);

};