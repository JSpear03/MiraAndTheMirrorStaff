// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PuzzleAssetDirectBASE.h"
#include "Kismet/GameplayStatics.h"
#include "MIR_PlayerChar.h"

// Sets default values
AMIR_PuzzleAssetDirectBASE::AMIR_PuzzleAssetDirectBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	RootComponent = CollisionComponent;

	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_PuzzleAssetDirectBASE::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMIR_PuzzleAssetDirectBASE::OnEndOverlap);
}

void AMIR_PuzzleAssetDirectBASE::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	
	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}
	
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Actor Overlap"));
	
	if (!bIsChildOfPlayer)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Found player"));

	IIMIR_Indirect::Execute_ActivateIndirect(this, PlayerCharacter);
}

void AMIR_PuzzleAssetDirectBASE::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	
	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}
	
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();
	
	if (!bIsChildOfPlayer)
	{
		return;
	}
	
	IIMIR_Indirect::Execute_DeactivateIndirect(this, PlayerCharacter);
}

// Called every frame
void AMIR_PuzzleAssetDirectBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMIR_PuzzleAssetDirectBASE::ActivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}
		
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Executed turn on"));
		
	bDidTurnOn = true;
}

void AMIR_PuzzleAssetDirectBASE::DeactivateIndirect_Implementation(AMIR_PlayerChar* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Executed turn on"));

	bDidTurnOn = false;
}

