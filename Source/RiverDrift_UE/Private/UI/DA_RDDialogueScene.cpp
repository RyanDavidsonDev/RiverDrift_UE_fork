// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DA_RDDialogueScene.h"

FRDDialogueResponseOption::FRDDialogueResponseOption()
{
	UE_LOG(LogTemp, Log, TEXT("dialogue response option default constr called"))
	//ButtonText = FText::FromString("[continue]");
}

FRDDialogueResponseOption::FRDDialogueResponseOption(FText _ButtonText)
{
	ButtonText = _ButtonText;
}

FRDDialogueSlide::FRDDialogueSlide() 
{
	UE_LOG(LogTemp, Log, TEXT("dialogue slide default constr called"))
		LeftResponse = FRDDialogueResponseOption();
	RightResponse = FRDDialogueResponseOption(FText::FromString("[continue]"));

}