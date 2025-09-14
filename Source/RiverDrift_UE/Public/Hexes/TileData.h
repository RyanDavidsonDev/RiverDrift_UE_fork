#pragma once

#include "CoreMinimal.h"
//#include "../HexLibrary.h"
#include "RDBillboardGroupBase.h"
#include "TileData.generated.h"
/**
 *
 */

class UPaperSprite;
class ARDBillboardGroupBase;

UENUM(BlueprintType)
enum class ETileType : uint8
{
	TE_Blank UMETA(DisplayName = "Blank"),
	TE_River UMETA(DisplayName = "river"),
	TE_Field UMETA(DisplayName = "field"),
	TE_Forest UMETA(DisplayName = "forest"),
	TE_Mountain UMETA(DisplayName = "mountain"),
	TE_Town UMETA(DisplayName = "town"),
	TE_Mystery UMETA(DisplayName = "mystery"),
	TE_Beach UMETA(DisplayName = "beach"),
};

USTRUCT()
struct RIVERDRIFT_UE_API FTileData : public FTableRowBase
{
	GENERATED_BODY()


public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	UPaperSprite* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TSubclassOf<ARDBillboardGroupBase> Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	ETileType ETileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	bool bIsPlayerTraversible;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cellSize = 256.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float weight = 16.0f; //can we use non-whole ints?

	float cellWidth = cellSize;
	float cellHeight = cellSize;

};

USTRUCT(BlueprintType)
struct RIVERDRIFT_UE_API FLandmarkKey
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<ETileType> Key;


	FLandmarkKey()
		: FLandmarkKey(TArray<ETileType>{ETileType::TE_Blank, ETileType::TE_Blank, ETileType::TE_Blank})
	{}

	FLandmarkKey(TArray<ETileType> Key)
		: Key(Key)
	{}

	FLandmarkKey(const FLandmarkKey& Other)
		: FLandmarkKey(Other.Key)
	{}
	

	bool operator==(const FLandmarkKey& Other) const
	{
		return Equals(Other);
	}


	bool Equals(const FLandmarkKey& Other) const
	{
		TArray<ETileType> SortedOther = Other.Key;
		SortedOther.Sort();

		TArray<ETileType> SortedKey = Key;
		SortedKey.Sort();
		for (int i = 0; i < Key.Num(); i++) {
			if (SortedOther[i] != SortedKey[i]) {
				return false;
			}
		}
		return true;
	}

};
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FLandmarkKey& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FLandmarkKey& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FLandmarkKey));
	return Hash;
}
#endif


USTRUCT(BlueprintType)
struct RIVERDRIFT_UE_API FLandmarkData : public FTableRowBase
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	FLandmarkKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
	UPaperSprite* Sprite;


};

