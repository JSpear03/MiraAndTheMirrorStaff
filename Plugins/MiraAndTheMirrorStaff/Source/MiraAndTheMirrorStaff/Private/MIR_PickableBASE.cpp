// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PickableBASE.h"

// Sets default values
AMIR_PickableBASE::AMIR_PickableBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = CollisionComponent;

	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_PickableBASE::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMIR_PickableBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

