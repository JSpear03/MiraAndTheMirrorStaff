// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_OnOffButton.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_OnOffButton::AMIR_OnOffButton()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
}

// Called when the game starts or when spawned
void AMIR_OnOffButton::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMIR_OnOffButton::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMIR_OnOffButton::OnEndOverlap);
}

void AMIR_OnOffButton::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
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

	ActivateDirect(PlayerCharacter);
}

void AMIR_OnOffButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
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

	DeactivateDirect(PlayerCharacter);
}

// Called every frame
void AMIR_OnOffButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
