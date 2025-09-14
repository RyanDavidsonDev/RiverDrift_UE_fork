#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileData.h"
#include "../HexLibrary.h"
#include "SpawnableTile.generated.h"

class ATileManager;
class ARDSpawnableLandmark;
class UPaperSpriteComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class UMaterialInstance;
class ARDSpawnableLandmark;
struct FTileData;


UCLASS()
class ASpawnableTile : public AActor
{

	GENERATED_BODY()

// --- VARS ---

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tiles")
	TArray<TObjectPtr<ASpawnableTile>> Neighbors;//do we need to keep this as an array of objects? storing coords might take less memory, and looking up into hexmap should(?) be a trivial expense

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tiles")
	FHex HexCoord;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tiles")
	FOffsetCoord offsetCoord;

	TObjectPtr<ATileManager> tileManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTileData TileType;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* BackgroundSpriteComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Landmarks")
	TObjectPtr<ARDSpawnableLandmark> Landmark;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Landmarks")
	TSet<ARDSpawnableLandmark*> PotentialLandmarks;


protected:

	UPROPERTY(EditDefaultsOnly, Category="Tiles")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Tiles")
	UMaterialInterface* HighlightedMaterial;

	UMaterialInstanceDynamic* DynamicMaterial;

	UMaterialInstanceDynamic* DefaultMaterialInstance;
	UMaterialInstanceDynamic* HighlightedMaterialInstance;


// --- FUNCS ---

public:
	ASpawnableTile();

	UFUNCTION()
	ASpawnableTile* InitializeTile(const FHex& h, FTileData prefab, AActor* _owner);

	ASpawnableTile* InitializeTile(FOffsetCoord c, FTileData prefab, AActor* _owner);

	void UpgradeTile(FTileData NewType);

	void UpdateAppearance(FTileData NewType);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SelectTile();
	void DeselectTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




};

