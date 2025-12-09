// Fill out your copyright notice in the Description page of Project Settings.


#include "Hexes/RDBillboardGroupBase.h"
#include "Components/MeshComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"

// Sets default values
ARDBillboardGroupBase::ARDBillboardGroupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	ReferenceTile = this->CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	ReferenceTile->SetupAttachment(this->GetRootComponent());
	//ReferenceTile->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	TArray<UPaperSpriteComponent*> paperSprites;
	//GetComponents**<UPaperSpriteComponent>** (paperSprites);
	GetComponents<UPaperSpriteComponent>(paperSprites);
	TArray<UPaperFlipbookComponent*> flipBooks;
	GetComponents<UPaperFlipbookComponent>(flipBooks);

	paperSprites.Remove(ReferenceTile);

	Billboards.Append(paperSprites);
	Billboards.Append(flipBooks);


}

// Called when the game starts or when spawned
void ARDBillboardGroupBase::BeginPlay()
{
	Super::BeginPlay();

	ReferenceTile->DestroyComponent();


	TArray<UPaperSpriteComponent*> paperSprites;
	//GetComponents**<UPaperSpriteComponent>** (paperSprites);
	GetComponents<UPaperSpriteComponent>(paperSprites);
	TArray<UPaperFlipbookComponent*> flipBooks;
	GetComponents<UPaperFlipbookComponent>(flipBooks);


	Billboards.Append(paperSprites);
	Billboards.Append(flipBooks);

	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;


	for (UMeshComponent* billboard : Billboards) {
		UPaperFlipbookComponent* flipbook = Cast<UPaperFlipbookComponent>(billboard);
		UPaperSpriteComponent* paperSprite = Cast<UPaperSpriteComponent>(billboard);
		if (!IsValid(flipbook) && !IsValid(paperSprite)) {
			UE_LOGFMT(LogTemp, Error, "ERROR not a vali");
		}
		billboard->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

// Called every frame
void ARDBillboardGroupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (UMeshComponent* billboard : Billboards) {
		
		FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(
			billboard->GetComponentLocation(), CameraManager->GetCameraLocation());
		FRotator LookRotation = FRotator(0, lookAtRot.Yaw - GetOwner()->GetActorRotation().Yaw, 0);
		LookRotation += FRotator(0,-90,0);

		//UE_LOGFMT(LogTemp, Log, "look at {0}, look rotation {1}", *lookAtRot.ToCompactString(), *LookRotation.ToCompactString());
		//SetWorldRotation(lookRotation);
		billboard->SetRelativeRotation(LookRotation);
	}

}

