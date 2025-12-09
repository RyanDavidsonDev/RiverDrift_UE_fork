// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RD_PlayerPawn.h"
#include "Player/RD_PlayerController.h"
#include "Hexes/SpawnableTile.h"
#include "Hexes/TileManager.h"
#include "Core/RD_GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values
ARD_PlayerPawn::ARD_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//create root component, player model, and collider
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	PlayerModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	InteractColliderCircle = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerInteractCollider_C"));
	//sphere = CreateDefaultSubobject<USphereComponent>(TEXT("sphere"));
	
	// Create a camera boom...
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	
	//CameraBoom->AttachToComponent(rootcom);
	//CameraBoom->SetupAttachment(RootComponent);

	PlayerModel->SetupAttachment(RootComponent);
	InteractColliderCircle->SetupAttachment(RootComponent);


	
	SpringArmComp->SetupAttachment(PlayerModel);
	TopDownCameraComponent->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//SpringArmComp->SetRelativeRotation(FRotator(0.0f, 0.f, 0.f));


	//SpringArmComp->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	//SpringArmComp->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

				//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 65.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;


	//SpringArmComp->TargetArmLength = 800.f;
	// Create a camera...
	//TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


}

// Called when the game starts or when spawned
void ARD_PlayerPawn::BeginPlay()
{
	FTransform StartTransform = this->GetTransform();
	//FRotator CameraStartRotation = this->

	//this->RootComponent->SetWorldTransform(ZeroTransform);
	//TopDownCameraComponent->SetWorldTransform(StartTransform);


	PlayerController = Cast<ARD_PlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController)) {
		InteractColliderCircle->OnComponentBeginOverlap.AddUniqueDynamic(PlayerController, &ARD_PlayerController::BeginOverlapCallback);
		InteractColliderCircle->OnComponentEndOverlap.AddUniqueDynamic(PlayerController, &ARD_PlayerController::EndOverlapCallback);
		
		TArray<AActor*> OverlappingActors;
		InteractColliderCircle->GetOverlappingActors(OverlappingActors, ASpawnableTile::StaticClass());
		for (AActor* a : OverlappingActors) {
			ASpawnableTile* tile = Cast<ASpawnableTile>(a);
			if (IsValid(tile)) {
				PlayerController->TilesInRange.Add(tile);
			}
			else {
				UE_LOGFMT(LogTemp, Fatal, "cast from overlapping actor to tile failed PlayerPawn::Beginplay, the fact that this even happened means ryan misunderstood something about how the class param of GetOverlappingActors worked");
			}
		}



	}
	else {
		UE_LOGFMT(LogTemp, Error, "player controller is not valid");
	}

	GameMode = Cast<ARD_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode->TileManager)) {
		

		//may cause issues if player doesn't start at origin, or if out variables don't stay valid
		GameMode->TileManager->TileIsFilled(FHex(0, 0, 0), CurrentTileLocation);

	}
	else {
		UE_LOGFMT(LogTemp, Fatal, "playerpawn.beginPlay tried retrieving a reference to GameMode->TileManager and it wasn't valid");
	}

	Super::BeginPlay();
	
}


// Called every frame
void ARD_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ARD_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARD_PlayerPawn::MoveToTile(ASpawnableTile* tile)
{
	if (CurrentTileLocation->Neighbors.Contains(tile)) {
		SetActorLocation(tile->GetActorLocation());
		CurrentTileLocation = tile;
	}
	else {
		UE_LOGFMT(LogTemp, Log, "Player tried moving to a river tile they're not adjacent to");
	}
	
}

