// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Internationalization/Text.h"
#include "DA_RDDialogueScene.generated.h"

struct FRDDialogueResponseOption;
struct FRDDialogueSlide;

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* CharacterPortrait;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FRDDialogueSlide> Slides;


	
};
