// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tiledata.h"
#include "RDSpawnableLandmark.generated.h"

struct FLandmarkData;
class UPaperSpriteComponent;
class ASpawnableTile;

UCLASS()
class RIVERDRIFT_UE_API ARDSpawnableLandmark : public AActor
{
	GENERATED_BODY()
	// --- VARS ---


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landmarks")
	TArray<ASpawnableTile*> ComposingTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landmarks")
	UPaperSpriteComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landmarks")
	FLandmarkData LandmarkData;


protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Landmarks")
	bool bIsPotential = true;
	//
	//
	//
	//public:

		// --- FUNCS

public:
	// Sets default values for this actor's properties
	ARDSpawnableLandmark();

	void InitializeLandmark(TArray<ASpawnableTile*> _ComposingTiles, FLandmarkData _LandmarkData, bool _bIsPotential = true);

	UFUNCTION(BlueprintGetter, Category="Landmarks")
	bool GetIsPotential() const {return bIsPotential; } 
	void SetIsPotential(bool b);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ClearAdjacentPotentialLandmarks(TSet<ARDSpawnableLandmark*>& LandmarksSetForDestruction);
	void ClearAdjacentPotentialLandmarks();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SetSpriteAlpha(UPaperSpriteComponent* _Sprite, float alpha);

};
