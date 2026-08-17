// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAssets/Direct/MIR_PuzzleAssetDirectBASE.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_PuzzleAssetDirectBASE::AMIR_PuzzleAssetDirectBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	RootComponent = CollisionComponent;

	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetDirectBASE::BeginPlay()
{
	Super::BeginPlay();

	OnDirectActivated.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::FindDirectBase);
}

// Called every frame
void AMIR_PuzzleAssetDirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
