// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexes/TileManager.h"
#include "HexLibrary.h"
#include "Hexes/SpawnableTile.h"
#include "Hexes/TileData.h"
#include "Hexes/RDSpawnableLandmark.h"
#include "Core/RD_GameMode.h"
#include "Core/DA_RDPrototypeAsset.h"
#include "Rendering/RenderingSpatialHash.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Math/MathFwd.h"
//#include ""
#include "Logging/StructuredLog.h"


FString ATileManager::LandmarkKeyToString(TArray<ETileType> arr)
{
	FString string = "{";
	for (int i = 0; i < arr.Num(); i++) {
		string += UEnum::GetValueAsString(arr[i]);
		string += ", ";
	}
	string += " )";
	return string;
}

ARDSpawnableLandmark* ATileManager::CreatePotentialLandmark(FName name, FLandmarkData data, TArray<ASpawnableTile*> ComposingTiles)
{
	ARDSpawnableLandmark* PotLandmark = GetWorld()->SpawnActor<ARDSpawnableLandmark>();
	
	
	return nullptr;
}



// Sets default values
ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOGFMT(LogTemp, Log, "tilemanager begin play started from cpp class size of map is {0}", RD_HexMap.IsEmpty());

	BuildGrid();
	UE_LOGFMT(LogTemp, Log, "tilemanager begin play called from cpp class");
	//GetNextTileToPlace();
	NextTileToPlace = SelectRandomTileType();
	GameMode = Cast<ARD_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->TileManager = this;

	GameMode->OnGameModeInitializedDelegate.AddUObject(this, &ATileManager::SetTileWeights);


	TArray<ETileType> test = { ETileType::TE_River, ETileType::TE_Mountain, ETileType::TE_Mountain };

	//FString ps = LandmarkKeyToString(test);
	UE_LOGFMT(LogTemp, Log, "presorted array: {0}", LandmarkKeyToString(test));

	test.Sort();
	UE_LOGFMT(LogTemp, Log, "postsorted array: {0}", LandmarkKeyToString(test));

	InitializeLandmarkMap();


}


void ATileManager::SetTileWeights()
{
	//will we need to be able to change the weights during runtime?
	fTotalRowsWeight = 0;
	for (FName name : TileDataTable->GetRowNames()) {
		FTileData* tile = TileDataTable->FindRow<FTileData>(name, "defaultRiver");
		//GameMode = Cast<ARD_GameMode>(GetWorld()->GetAuthGameMode());
		if (tile->ETileType!= ETileType::TE_River || GameMode->PrototypingAsset->bAllowWaterGeneration) {
			fTotalRowsWeight += tile->weight;
		}
	}
	UE_LOGFMT(LogTemp, Log, "setting tile weight, total is {0}", fTotalRowsWeight);
}

void ATileManager::InitializeLandmarkMap()
{

	for (FName name : LandmarkDataTable->GetRowNames()) {
		FLandmarkData* landmark = LandmarkDataTable->FindRow<FLandmarkData>(name, "initializing landmark map");
		UE_LOGFMT(LogTemp, Log, "presorted key array is {0}", LandmarkKeyToString(landmark->Key.Key));

		landmark->Key.Key.Sort();
		UE_LOGFMT(LogTemp, Log, "presorted key array is {0}", LandmarkKeyToString(landmark->Key.Key));


		LandmarkHashMap.Add(landmark->Key, name);
	}
}

//bool ATileManager::tileExists(FHex hex, ASpawnableTile* tile)
bool ATileManager::TileIsFilled(FHex hex, ASpawnableTile* &tile)
{
	FVector3f vector = FVector3f(hex.q, hex.r, hex.s);
	tile = RD_TileMap.FindRef(vector);

	return !(tile == nullptr);
}



ASpawnableTile* ATileManager::PlaceTile_XY(FOffsetCoord offsetCoord, FTileData format) {

	UE_LOGFMT(LogTemp, Log, "placetile_XY called || offset coords x {0} y {1} ", offsetCoord.x, offsetCoord.y);
	//FTileData tileData = format.GetRow<FTileData>("");
	FHex cubicCoord = UHexLibrary::offset_to_cube(offsetCoord); //static instantiation of FHex? Dunaganq

	return ATileManager::PlaceTile_QRS(cubicCoord, format);

	//ASpawnableTile* tile = ASpawnableTile::CreateTile(offsetCoord, format, this);
}


//'ultimate' call
ASpawnableTile* ATileManager::PlaceTile_QRS(FHex hexCoord, FTileData format)
{
	ASpawnableTile* tile;
	if (!TileIsFilled(hexCoord, tile)) {
		UE_LOGFMT(LogTemp, Warning, "tilemanager is trying to place a tile at a coordinate that doesn't yet exist. we likely only want to allow this when the tile man is placing them directly (rather than from input by player) {0} ", *hexCoord.ToString());
		//TODO: fill out
		//should only 


		tile= SpawnTile(hexCoord, format);
		//tile->AttachToActor(this);

		//tile = ASpawnableTile::InitializeTile(hexCoord, format, this);
	}
	else {
		//it already exists, so upgrade it.
		tile->UpgradeTile(format);
		ASpawnableTile* temp;



		if (TileIsFilled(tile->HexCoord, temp)) {//sort of redundant check, just needed to pull a ref to the tileMan's stored tile
			//intended for checking whether tileMan is properly receiving updates to tiles.
			if (tile->TileType.ETileType != temp->TileType.ETileType) {
				UE_LOGFMT(LogTemp, Error, 
					"ERROR, we've upgraded the tile but the tilemanager's storage doesn't seem to have gotten the memo. Thislikely indicates a deeper issue with memory management");

			}
		}
	}

	UE_LOGFMT(LogTemp, Log, "placetile_QRS called ||  cubic coords q {0} r {1} s {2} ", hexCoord.q, hexCoord.r, hexCoord.s);

	PlaceNeighbors(tile); 
	
	return tile;
}


//'ultimate' call
ASpawnableTile* ATileManager::SpawnTile(FHex hexCoord, FTileData format) {
	//ASpawnableTile* tile = GetWorld()->SpawnActor<ASpawnableTile>(DefaultSpawnableTileBP->StaticClass());
	ASpawnableTile* tile = GetWorld()->SpawnActor<ASpawnableTile>(DefaultSpawnableTileBP);

	if (IsValid(tile)) {
		tile->InitializeTile(hexCoord, format, this);
	
		//insert it into map, has to be converted to vec3 as FMaps can't really do custom structs
		RD_TileMap.Add(FVector3f(hexCoord.q, hexCoord.r, hexCoord.s), tile);

		TArray<FLandmarkData> potentialLandmarks;
		UE_LOGFMT(LogTemp, Log, "starting");

		for (int i = 0; i < tile->Neighbors.Num(); i++) {
			//set our key the tile type of {our tile, neighbors at i, neighbors at i+1
			TArray<ETileType> keyCheck = {
				tile->TileType.ETileType, 
				tile->Neighbors[i]->TileType.ETileType, 
				tile->Neighbors[(i + 1) % tile->Neighbors.Num()]->TileType.ETileType };//modulo operation for when we loop back around


			FLandmarkData* data = LookupTableByName<FLandmarkData>(LandmarkDataTable, *LandmarkHashMap.Find(keyCheck), "in tileMan.spawnTile(), checking whether the tiles exist in the hashmap");

			if (data != nullptr) {
				UE_LOGFMT(LogTemp, Log, "we found a match!");

			}
			else {
				UE_LOGFMT(LogTemp, Log, "nope");

			}

				//keyCheck 
			//keyCheck = ;
		}

		//tile->Neighbors

	}
	else {
		UE_LOGFMT(LogTemp, Fatal, "Tile not valid");
	}

	

	return tile;

}

ASpawnableTile* ATileManager::CreateBlankTile(FHex hexCoord)
{
	UE_LOGFMT(LogTemp, Log, "creating blank tile at coords: q {0} r {1} s {2} ", hexCoord.q, hexCoord.r, hexCoord.s);

	FTileData BlankTile = *this->TileDataTable->FindRow<FTileData>("Blank", "defaultBlank");//is this a dynamic instance of FTileData because it's returning a pointer? Dunaganq


	return SpawnTile(hexCoord, BlankTile);


	//ASpawnableTile* tile = GetWorld()->SpawnActor<ASpawnableTile>(SpawnableTileBP.GetDefaultObject()->StaticClass());
	//tile->InitializeTile(hexCoord,*this->TileDataTable->FindRow<FTileData>("Blank", "blank placement in PlaceNeighbors()"), this);
	
}


void ATileManager::PlaceNeighbors(ASpawnableTile* tile) {
	UE_LOGFMT(LogTemp, Log, " ");

	UE_LOGFMT(LogTemp, Log, "placing neighbors around coords: {0} ", tile->HexCoord.ToString());

	for (FHex direction : UHexLibrary::hex_directions) {
		//starting directly to the right ("cardinal right" or "cr", moves counterlockwise

		//check if tile exists
		ASpawnableTile* neighbor = nullptr;
		FHex neighborCoord = UHexLibrary::hex_add(tile->HexCoord, direction);
		UE_LOGFMT(LogTemp, Log, "direction : {0} neighbor's coords: {1} ", *direction.ToString(), *neighborCoord.ToString());
		if(!TileIsFilled(neighborCoord, neighbor)) {//if it doesn't exist, then we need to add it
			
			neighbor = CreateBlankTile(neighborCoord);
		} else  //if it already exists, I don't think we need to do anything
		{
			UE_LOGFMT(LogTemp, Log, "tile already exists, not creating a blank tile but instead pulling from map. " );

		}
		if (IsValid(neighbor)) {//shouldn't be necessary, as theoretically we're setting it by now. but just in case
			tile->Neighbors.Add(neighbor);
		} else {
			UE_LOGFMT(LogTemp, Error, "ERROR: neighbor reference in PlaceNeighbord() is somehow still null. double check your logic");
		}
	}
}


void ATileManager::BuildGrid_Implementation()
{
	UE_LOGFMT(LogTemp, Log, "tilemanager build grid called from cpp class");
	
	if (this->TileDataTable) {


		FTileData RiverTile = *this->TileDataTable->FindRow<FTileData>("River", "defaultRiver");//is this a dynamic instance of FTileData because it's returning a pointer? Dunaganq

		this->PlaceTile_XY(FOffsetCoord(0, 0), RiverTile);//make sure player starts on river tile
		


		for (int i = 1; i >=0; i--) {

			bool bTileIsValid = false;
			FTileData f;
			f = this->SelectRandomTileType();

			this->PlaceTile_XY(FOffsetCoord(1, i), f);//two tiles to right and up right are random




			//if (isempty(f)) {
			//}
			//else {
			//	UE_LOGFMT(LogTemp, Error, "bTileIsValid false, didn't place");

			//}

			//for (int j = -9; j < 10; j++) {

			//	bool bTileIsValid = false;
			//	FTileData f = this->SelectRandomTile(&bTileIsValid);

			//	if (bTileIsValid) {
			//		this->PlaceTile_XY(FOffsetCoord(i,j), f);
			//	}
			//	else {
			//		UE_LOGFMT(LogTemp, Log, "bTileIsValid false, didn't place");

			//	}
			//}

			UE_LOGFMT(LogTemp, Display, " buildgrid iter one tile --------------- \n");
			UE_LOGFMT(LogTemp, Display, "  \n");

		}

	}
	else {
		UE_LOGFMT(LogTemp, Log, "tile data table ref not set");

	}
}


FTileData ATileManager::SelectRandomTileType_Implementation(/*bool& valid*/)
{


	FTileData foundTile;
	if (IsValid(this->TileDataTable)) {

		int CurrentWeight = FMath::RandRange(0, fTotalRowsWeight - 1);

		for (FName rowName : TileDataTable->GetRowNames()) {
			if (rowName != "River" || GameMode->PrototypingAsset->bAllowWaterGeneration) {
				FTileData* tile = TileDataTable->FindRow<FTileData>(rowName, "Random weight calcs");
				CurrentWeight -= tile->weight;
				if (CurrentWeight < 0) {
					FString name = UEnum::GetValueAsString(tile->ETileType);
					UE_LOGFMT(LogTemp, Log, "select random called, result was {0}", name);
					D_OnTileGeneratedDelegate.Broadcast(*tile);
					return *tile;
				}
			}
		}

		UE_LOGFMT(LogTemp, Error, "You somehow managed to get over every row without getting negative. defaulting to fully equal weights I guess");


		FPermissionListOwners names = this->TileDataTable->GetRowNames();
		int rand = FMath::RandRange(1, names.Num() - 1);//excluding first row to keep blank tiles separate
		FName name = names[rand];

		UE_LOGFMT(LogTemp, Log, "randomly selected tile from row {0} ", name);

		foundTile = *this->TileDataTable->FindRow<FTileData>(name, "select random");//is this a dynamic instance of FTileData because it's returning a pointer? Dunaganq
		//D_OnTileGeneratedDelegate.Broadcast(foundTile);

	}
	else {
		UE_LOGFMT(LogTemp, Fatal, "ERROR: TileManager.TileDataTable ref not valid, make sure you set it");
	}
	return foundTile;
	
}


//void ATileManager::PlaceTile_XY_Implementation(FPoint point)
//{
//	UE_LOGFMT(LogTemp, Log, "placing tile with xy coords x {0} y {1}", point.x, point.y);
//
//}
//
//
//
//void ATileManager::PlaceTile_QRS_Implementation(FVector3f hexCoord)
//{
//	UE_LOGFMT(LogTemp, Log, "placing tile with hex coords q {0} r {1} s {2} ", hexCoord.X, hexCoord.Y, hexCoord.Z);
//}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTileData ATileManager::GetNextTileToPlace(bool generateNew)
{
	FTileData lastTile = NextTileToPlace;
	if (generateNew) {
		NextTileToPlace = SelectRandomTileType();
	}
	return lastTile;

	//if (IsValid(NextTileToPlace)) {

	//}
	//FTileData lastTile = NextTileToPlace;
	//if (pullNext) {
	//	NextTileToPlace =  SelectRandomTileType();//pull the value, convert it to pointer, and store the ref
	//}

	//return lastTile;
}

void ATileManager::UpgradeTile(FTileData format , ASpawnableTile* tile)
{
	tile->UpgradeTile(format);
	PlaceNeighbors(tile);



	TArray<ARDSpawnableLandmark*> PotentialLandmarks = SpawnPotentialLandmarks(tile) ;
	
	if (PotentialLandmarks.Num() > 0) {//only bother doing anything if we found at least one

		//for now we'll just spawn the first one, but we'll need to figure out UI for multiple options
		if (PotentialLandmarks.Num() == 1) {
			//if there's only one, then we can just finalize it immediately.
			PotentialLandmarks[0]->SetIsPotential(false);
			


			//ARDSpawnableLandmark* landmark = GetWorld()->SpawnActor<ARDSpawnableLandmark>(DefaultSpawnableLandmarkBP);
			//landmark->InitializeLandmark(PotentialLandmarks[0]);
			//landmark->InitializeLandmark()
			UE_LOGFMT(LogTemp, Log, "setting one landmark");

		}
		else {
			UE_LOGFMT(LogTemp, Error, "there are multiple potential landmarks but we don't know which one to spawn;");

		}
	}
}


TArray<ARDSpawnableLandmark*> ATileManager::SpawnPotentialLandmarks(ASpawnableTile* tile )
{
	//TMap<FName*, FLandmarkData*> ValidLandmarks;
	TArray<ARDSpawnableLandmark*> ValidLandmarks;


	//for each neighbor 
	for (int i = 0; i < tile->Neighbors.Num(); i++) {
		const int iplus1 = (i + 1) % tile->Neighbors.Num(); //uses modulo operation to wrap array back to start when at end. setting as var for readability

		//before checking the two tiles, make sure they're not already part of another confirmed landmark
		if ((tile->Neighbors[i]->Landmark == nullptr) && (tile->Neighbors[iplus1]->Landmark == nullptr ) ){
		//depending on any later changes made to how the landmark ref gets set, the above logic may need to also check whether the landmark is set AND IS NOT only potential
		//^that's not necessary right this moment as we only set them when they become confirmed, but if that changes the above logic also needs to be updated

			//look up the current key in the landmark data table

			//set our key the tile type of {our tile, neighbors at i, neighbors at i+1
			TArray<ETileType> keyCheck = {
				tile->TileType.ETileType,
				tile->Neighbors[i]->TileType.ETileType,
				tile->Neighbors[iplus1]->TileType.ETileType };//modulo operation for when we loop back around

			if (!IsValid(LandmarkDataTable)) {
				UE_LOGFMT(LogTemp, Fatal, "LandmarkDataTable not valid");
			}
			FName* RowName = LandmarkHashMap.Find(keyCheck);



			if (RowName != nullptr) {//we found a match

				FLandmarkData* data = LookupTableByName<FLandmarkData>(LandmarkDataTable, *RowName, "in tileMan.spawnTile(), checking whether the tiles exist in the hashmap");
				UE_LOGFMT(LogTemp, Log, "we found a match! landmark name is {0}, data is {1}", RowName->ToString(), data->Sprite->GetName());

				ARDSpawnableLandmark* PotLandmark = GetWorld()->SpawnActor<ARDSpawnableLandmark>(DefaultSpawnableLandmarkBP);

				TArray<ASpawnableTile*> tiles = { tile, tile->Neighbors[i], tile->Neighbors[iplus1] };

				if (data == nullptr) {
					UE_LOGFMT(LogTemp, Fatal, "unable to find row in LandmarkDataTable given rowName {0}", *RowName->ToString());

				}
				if (&tiles == NULL) {
					UE_LOGFMT(LogTemp, Fatal, "unable to initialize an array of {tile, tile.neighbors[i], and t.n[i+1]} array's ptr was null");

				}
				PotLandmark->InitializeLandmark(tiles, *data);


				//PotLandmark->ComposingTiles = tiles;
				//UE_LOGFMT(LogTemp, Fatal, "2");
				//PotLandmark->LandmarkData = *data;
				////UE_LOGFMT(LogTemp, Fatal, "3");

				//PotLandmark->Sprite->SetSprite(data->Sprite);

				////UE_LOGFMT(LogTemp, Fatal, "4");
				ValidLandmarks.Add(PotLandmark);
				tile->PotentialLandmarks.Add(PotLandmark);

				//ValidLandmarks.Add(RowName, data);

			}
			else {
				UE_LOGFMT(LogTemp, Log, "nope");

			}
		
		}


		
	}
	UE_LOGFMT(LogTemp, Log, "we found {0} valid landmarks that can be placed", ValidLandmarks.Num());

	return ValidLandmarks;
	//return TArray<ARDSpawnableLandmark*>();
}



FTileData ATileManager::LookupTileType(ETileType tileType, FString contextMessage = "context not specified")
{
	FName name=  UEnum::GetValueAsName(tileType);
	
	FText text;
	//UEnum::getdispla
	FTileData td;
	
	name = FName(UEnum::GetValueAsString(tileType).RightChop(14));

	UEnum::GetDisplayValueAsText(tileType, text);
	//FString s = text.ToString();
	FString string = text.ToString().RightChop(14);
	//name = FName(string.RightChop(7));

	UE_LOGFMT(LogTemp, Log, "looking up tile type in tileMan, name is {0}", name );

	return *this->TileDataTable->FindRow<FTileData>(name, contextMessage);//is this a dynamic instance of FTileData because it's returning a pointer? Dunaganq

}
//
//FTableRowBase* ATileManager::lookupTableByName(UDataTable table, FName name, FString contextMessage = "context not specified")
//{
//
//	return table.FindRow<FTableRowBase>(name, contextMessage);
//}

