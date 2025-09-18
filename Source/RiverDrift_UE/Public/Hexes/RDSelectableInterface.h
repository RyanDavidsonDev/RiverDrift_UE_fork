// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RDSelectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URDSelectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RIVERDRIFT_UE_API IRDSelectableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SelectObject();
	virtual void DeselectObject();

protected:

	//UPROPERTY(EditDefaultsOnly, Category = "Tiles")
	//UMaterialInterface* HighlightedMaterial;

	//UMaterialInstanceDynamic* DynamicMaterial;

	//UMaterialInstanceDynamic* DefaultMaterialInstance;
	//UMaterialInstanceDynamic* HighlightedMaterialInstance;
};
