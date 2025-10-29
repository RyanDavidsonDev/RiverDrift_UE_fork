// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Internationalization/Text.h"
#include "DA_RDDialogueScene.generated.h"

struct FRDDialogueResponseOption;
struct FRDDialogueSlide;
struct FDialogueQuestLookup;

static const FString TablePath = TEXT("/Game/Data/cutscenes/DT_DialogueQuestLookup.DT_DialogueQuestLookup");

USTRUCT(BlueprintType)
struct FRDDialogueResponseOption
{
	GENERATED_BODY()
public:
	FRDDialogueResponseOption();
	FRDDialogueResponseOption(FText _ButtonText);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ButtonText;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText NextIntro;

};

USTRUCT(BlueprintType)
struct  FRDDialogueSlide
{
	GENERATED_BODY()
public:
	FRDDialogueSlide();

	FText DialogueIntro;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueMainBody;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText DialogueOutro;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRDDialogueResponseOption RightResponse;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRDDialogueResponseOption LeftResponse;


};


/**
 * 
 */
UCLASS(Blueprintable)
class RIVERDRIFT_UE_API UDA_RDDialogueScene : public UDataAsset
{
	GENERATED_BODY()

public:
	UDA_RDDialogueScene(); 
	//UDA_RDDialogueScene(const UDA_RDDialogueScene& other)

	//UDA_RDDialogueScene();
#if WITH_EDITOR
	virtual void PostInitProperties() override;
#endif

	virtual void PostLoad() override;

	void RegisterDataTable();

	UFUNCTION(BlueprintCallable)
	void CloseDialogueScene();

	
	TObjectPtr<FDialogueQuestLookup> QuestLookupRow;


	UPROPERTY(VisibleAnywhere,BlueprintReadonly)
	UDataTable* QuestLookupDataTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* CharacterPortrait;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FRDDialogueSlide> Slides;

private:


	
};

//static const FString TablePath = TEXT("/Game/Data/Dialogue/DT_DialogueQuestLookup.DT_DialogueQuestLookup");
