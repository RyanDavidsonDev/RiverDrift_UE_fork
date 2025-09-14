// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../HexLibrary.h"
#include "../Hexes/SpawnableTile.h"
#include "TileManager.generated.h"


class ASpawnableTile;
struct FHex;
struct FOffsetCoord;
struct FLandmarkKey;
//enum TileType;
class ARD_GameMode;
struct FLandmarkData;
struct FTableRowBase;
struct FTileData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileGeneratedSignature, FTileData, NextTile);

UCLASS()
class RIVERDRIFT_UE_API ATileManager : public AActor
{
	GENERATED_BODY()


	// --  VARS ---

public:	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> TileDataTable;


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> LandmarkDataTable;

	UPROPERTY(VisibleAnywhere)
	//TMap<FLandmarkKey, FLandmarkData > LandmarkHashMap;    
	TMap<FLandmarkKey, FName> LandmarkHashMap;


	static ASpawnableTile* dummy_tile;


	UPROPERTY(BlueprintAssignable)
	FOnTileGeneratedSignature D_OnTileGeneratedDelegate;


protected:
	UPROPERTY(VisibleInstanceOnly)
	FTileData NextTileToPlace; 

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FVector3f, ASpawnableTile* > RD_TileMap; //I think this is actually unneeded?


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpawnableTile> DefaultSpawnableTileBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARDSpawnableLandmark> DefaultSpawnableLandmarkBP;

	int fTotalRowsWeight;

private:
	TObjectPtr<ARD_GameMode> GameMode;
	

	// --  FUNCS ---


public:

	// Sets default values for this actor's properties
	ATileManager();

	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//UDA_TileBase* TileFormat;

	UFUNCTION(BlueprintCallable)
	bool TileIsFilled(FHex hex, ASpawnableTile*& tile); //expose - change to isFilled
	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTileData SelectRandomTileType( /*bool& valid*/);//change to selectRandomTileFormat
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	//returns the tile that the tilemanager is currently going to return next. defaults to generating a new one first
	FTileData GetNextTileToPlace(bool generateNew = true);

	void UpgradeTile(FTileData format, ASpawnableTile* tile);

	FTileData LookupTileType(ETileType tileType, FString contextMessage);

	UFUNCTION()
	void SetTileWeights();

	UFUNCTION()
	void InitializeLandmarkMap();

protected:
	//variables
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BuildGrid(); //implemented in c++ as BuildGrid_Implementation - able to be overriden by blueprints - largely placeholder until player input is working

	//private backend to initialize tile
	ASpawnableTile* SpawnTile(FHex hexCoord, FTileData format);

	UFUNCTION(BlueprintCallable) 
	ASpawnableTile* PlaceTile_XY(FOffsetCoord offsetCoord, FTileData format);
	UFUNCTION(BlueprintCallable)
	ASpawnableTile* PlaceTile_QRS(FHex hexCoord, FTileData format);

	//arguably unecessary wrapper function, just calls spawnTile and looks up blank format
	ASpawnableTile* CreateBlankTile(FHex hexCoord);

	void PlaceNeighbors(ASpawnableTile* tile);
	
	static FString LandmarkKeyToString(TArray<ETileType> arr);

	//static FTableRowBase* LookupTableByName(UDataTable* table, FName name, FString& contextMessage);
	//T* Lookup()shou


	template<typename FTableRowChild, typename = std::enable_if_t<std::is_base_of_v<FTableRowBase, FTableRowChild>>>
	static FTableRowChild* LookupTableByName(UDataTable* Table, FName RowName, const FString& contextMessage)
	{
		if (!IsValid(Table)) {
			UE_LOG(LogTemp, Warning, TEXT("LookupTableByName failed: Table is null. Context: %s"), *contextMessage);
			return nullptr;

		} 

		FTableRowChild* FoundRow = Table->FindRow<FTableRowChild>(RowName, contextMessage);
		if (!(FoundRow)) {
			UE_LOG(LogTemp, Warning, TEXT("LookupTableByName failed: Row '%s' not found in table. Context: %s"), *RowName.ToString(), *contextMessage);
		} 

		return FoundRow;
	}
	ARDSpawnableLandmark* CreatePotentialLandmark(FName name, FLandmarkData data, TArray<ASpawnableTile*> ComposingTiles);

	TArray<ARDSpawnableLandmark*> SpawnPotentialLandmarks(ASpawnableTile* tile);

	//static FTableRowChild* LookupTableByName(UDataTable* Table, FName RowName, const FString& ContextString)
	//{
	//	if (!Table)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("LookupTableByName failed: Table is null. Context: %s"), *ContextString);
	//		return nullptr;
	//	}

	//	FTableRowChild* FoundRow = Table->FindRow<FTableRowChild>(RowName, ContextString);
	//	if (!FoundRow)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("LookupTableByName failed: Row '%s' not found. Context: %s"), *RowName.ToString(), *ContextString);
	//	}

	//	return FoundRow;
	//}
};
