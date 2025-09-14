#include "Hexes/SpawnableTile.h"
#include "Hexes/TileManager.h"
#include "Hexes/TileData.h"
#include "Hexes/RDSpawnableLandmark.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "RiverDrift_UE/RiverDrift_UE.h"
#include "Kismet//KismetMathLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Engine/World.h"


ASpawnableTile* ASpawnableTile::InitializeTile(const FHex& h, FTileData prefab, AActor* _owner)
{

	FOffsetCoord offset = UHexLibrary::offset_from_cube(h);

	ASpawnableTile* tile = InitializeTile(offset, prefab, _owner);

	return tile;
}

//should standardize your calls
ASpawnableTile* ASpawnableTile::InitializeTile(FOffsetCoord c, FTileData prefab, AActor* _owner)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = _owner;
	SpawnInfo.ObjectFlags |= RF_Transient;

	FVector worldPos;
	
	worldPos.X = (((c.x - abs(c.y) * 0.5f) + abs(c.y) / 2) * (prefab.cellSize * 2.0f)) ;//currently hard coded for even-r offsetting

	worldPos.Y = (c.y * (prefab.cellWidth * 1.5f));
	worldPos.Z = 0.0f;

	SetActorLocation(worldPos);

	this->SetOwner(_owner);
	
	TileType = prefab;

	UpdateAppearance(prefab);

	offsetCoord = FOffsetCoord::FOffsetCoord(c.x/*-c.y/2*/, c.y);//-c.y/2 from catlike

	HexCoord = UHexLibrary::offset_to_cube(offsetCoord);
	

	return this;
}


void ASpawnableTile::UpgradeTile(FTileData NewType)
{
	UE_LOGFMT(LogTemp, Log, "It should've called upgrade");
	this->TileType = NewType;

	UpdateAppearance(NewType);
}

void ASpawnableTile::UpdateAppearance(FTileData NewType)
{
	if (IsValid(this->BackgroundSpriteComponent)) {
		this->BackgroundSpriteComponent->SetSprite(NewType.Sprite);
		this->BackgroundSpriteComponent->SetWorldRotation(FRotator(0, 0, -90));

	}
	else {
		UE_LOGFMT(LogTemp, Error, "Sprite for the {0} Data Asset HAS NOT been set ", UEnum::GetValueAsString(NewType.ETileType));
	}

	if (NewType.Billboard != nullptr) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.ObjectFlags |= RF_Transient;


		GetWorld()->SpawnActor<AActor>(
			NewType.Billboard, this->GetTransform().GetLocation(), FRotator::ZeroRotator, SpawnInfo);
	}
	else {
		UE_LOGFMT(LogTemp, Log, "billboard reference for the {0} Data Asset HAS NOT been set, leaving a blank billboard", UEnum::GetValueAsString(NewType.ETileType));
	}
}


// Sets default values
ASpawnableTile::ASpawnableTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	BackgroundSpriteComponent = this->CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundPaperSpriteComponent"));
	BackgroundSpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
	//SpriteComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	BackgroundSpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	//SpriteComponent->setresponseto
	//ETraceTypeQuery type = 
	UE_LOGFMT(LogTemp, Log, "ASPawnableTile constr called, channel was {0}" , BackgroundSpriteComponent->GetCollisionObjectType());
	//SetDefaults();
	
	
	DefaultMaterialInstance = UMaterialInstanceDynamic::Create(DefaultMaterial, Owner);
	HighlightedMaterialInstance = UMaterialInstanceDynamic::Create(HighlightedMaterial, Owner);
	
	DynamicMaterial = DefaultMaterialInstance;

	BackgroundSpriteComponent->SetMaterial(0, DynamicMaterial);

	//UDataTable* table =  Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("Game/Tiles/TileTypes")));
	//	//LoadObject<FTileData>(nullptr, TEXT("Game/TileTypes"), );//will fail if we ever move the tiletypes table

	//
	//if (table) {
	//	UE_LOGFMT(LogTemp, Log, "table succesfully loaded ");
	//	
	//	//table->row

	//}
	//else {
	//	UE_LOGFMT(LogTemp, Warning, "could not load table");

	//}
}


// Called when the game starts or when spawned
void ASpawnableTile::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	//UE_LOGFMT(LogTemp, Log, "Yeah it gets called");

}

// Called every frame
void ASpawnableTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOGFMT(LogTemp, Log, "Yeah it gets called	");


}

void ASpawnableTile::SelectTile()
{
	UMaterialInterface* mat = BackgroundSpriteComponent->GetMaterial(0);

	UE_LOGFMT(LogTemp, Log, "select tile called in cpp, {0}", *mat->GetName());
	BackgroundSpriteComponent->SetMaterial(0, HighlightedMaterial );
	DynamicMaterial = HighlightedMaterialInstance;
	
	UE_LOGFMT(LogTemp, Log, "select tile called in cpp, {0}", *mat->GetName());
}

void ASpawnableTile::DeselectTile()
{
	BackgroundSpriteComponent->SetMaterial(0, DefaultMaterial );
}

