// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexes/RDSpawnableLandmark.h"
#include "Kismet/KismetMathLibrary.h"
#include "Hexes/SpawnableTile.h"
#include "PaperSpriteComponent.h"

// Sets default values
ARDSpawnableLandmark::ARDSpawnableLandmark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	Sprite = this->CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundPaperSpriteComponent"));
	Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void ARDSpawnableLandmark::InitializeLandmark(TArray<ASpawnableTile*> _ComposingTiles, FLandmarkData _LandmarkData, bool _bIsPotential)
{
	this->ComposingTiles = _ComposingTiles;
	LandmarkData = _LandmarkData;

	

	bIsPotential = _bIsPotential;

	//set the location to the average of each other location (i.e. the middle point between each vec)
	TArray<FVector> locations;
	for (ASpawnableTile* tile : ComposingTiles) {
		locations.Add(tile->GetActorLocation());
	}
	SetActorLocation(UKismetMathLibrary::GetVectorArrayAverage(locations));

	Sprite->SetSprite(LandmarkData.Sprite);
	if (_bIsPotential) {
		SetSpriteAlpha(Sprite, .75);
	}
}

void ARDSpawnableLandmark::SetIsPotential(bool b)
{
	if (!b) {
		for (ASpawnableTile* tile : ComposingTiles) {
			tile->Landmark = this;
			//if you change how this ref gets set such that it's set when bIsPotential is NOT true, then you'll
			//need to refactor the logic in tileManager::spawnPotentialLandmarks. specifically the if statement
			// in the first for loop it currently doesn't account for that possiblity.
		}
	}

	SetSpriteAlpha(Sprite, 1);
	bIsPotential = b;

	ClearAdjacentPotentialLandmarks();
	UE_LOGFMT(LogTemp, Log, "set potential to {0}", b ? "true" : "false");
}

// Called when the game starts or when spawned
void ARDSpawnableLandmark::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARDSpawnableLandmark::SetSpriteAlpha(UPaperSpriteComponent* _Sprite, float alpha)
{
	FLinearColor NewColor = Sprite->GetSpriteColor();
	NewColor.A = alpha;
	_Sprite->SetSpriteColor(NewColor);
}

// Called every frame
void ARDSpawnableLandmark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//base call, calls recursively then destroys everything contained within
void ARDSpawnableLandmark::ClearAdjacentPotentialLandmarks()
{
	UE_LOGFMT(LogTemp, Log, "clearadjacentpotential() called on {0}", *this->GetName());
	TSet<ARDSpawnableLandmark*> LandmarksToSetForDestruction;
	for (ASpawnableTile* tile : ComposingTiles) {
		for (ARDSpawnableLandmark* Landmark : tile->PotentialLandmarks) {

			ClearAdjacentPotentialLandmarks(LandmarksToSetForDestruction);//recursively check every adjacent landmark 


		}
	}
	UE_LOGFMT(LogTemp, Log, "found {0} pot landmark to clear out ", LandmarksToSetForDestruction.Num());
	for (ARDSpawnableLandmark* Landmark : LandmarksToSetForDestruction) {
		Landmark->Destroy();
	}
}

void ARDSpawnableLandmark::ClearAdjacentPotentialLandmarks(TSet<ARDSpawnableLandmark*>& LandmarksSetForDestruction)
{
	UE_LOGFMT(LogTemp, Log, "clearadjacentpotential(landmarks) called on {0}", *this->GetName());
	UE_LOGFMT(LogTemp, Log, "found {0} pot landmark to clear out ", LandmarksSetForDestruction.Num());
	for (ASpawnableTile* tile : ComposingTiles) {
		for (ARDSpawnableLandmark* Landmark : tile->PotentialLandmarks) {
			if (!LandmarksSetForDestruction.Contains(Landmark)) {//memoize the destroyed landmarks to avoid re-iterating on any that we've already marked
				if (Landmark->bIsPotential) {
					UE_LOGFMT(LogTemp, Log, "found pot landmark to clear out {0}", *Landmark->GetName());

					//don't destroy actual landmarks
					LandmarksSetForDestruction.Add(Landmark);
					Landmark->ClearAdjacentPotentialLandmarks(LandmarksSetForDestruction);
						
				}
			}
		}
	}
}


