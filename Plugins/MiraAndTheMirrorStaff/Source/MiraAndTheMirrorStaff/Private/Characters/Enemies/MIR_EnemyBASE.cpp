// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Magic/MIR_MagicBASE.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values.
AMIR_EnemyBASE::AMIR_EnemyBASE()
{
	CopiableComponent = CreateDefaultSubobject<UMIR_CopiableComponent>(TEXT("Copiable Component"));

	ElementComponent->SetCurrentElement(EElements::Nothing);
}

void AMIR_EnemyBASE::BeginPlay()
{
	Super::BeginPlay();

	OnEnemyDied.AddDynamic(this, &AMIR_EnemyBASE::EnemyDied);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMIR_EnemyBASE::OnOverlapBegin);
}

void AMIR_EnemyBASE::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	TSubclassOf<class AMIR_MagicBASE> CurrentMagic;

	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}

	bool bIsChildOfMagic = OtherActor->GetClass()->IsChildOf<AMIR_MagicBASE>();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Overlap found"));

	if (!bIsChildOfMagic)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Is Magic"));

	CurrentMagic = Cast<AMIR_MagicBASE>(OtherActor)->GetClass();

	// This enemy is the copy source, so it is the interface receiver. Execute_CopyMagic is a
	// static function: the object it dispatches on is the first argument, not the "->" prefix.
	IIMIR_Copiable::Execute_CopyMagic(this, PlayerCharacter, CurrentMagic);
}

void AMIR_EnemyBASE::CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
	TSubclassOf<class AMIR_MagicBASE> MagicToCopy)
{
	if (!CurrentPlayer || !CopiableComponent)
	{
		return;
	}

	IIMIR_Copiable::Execute_CopyMagic(CopiableComponent, CurrentPlayer, MagicToCopy);
}

// Called every frame.
void AMIR_EnemyBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent->GetHealth() == 0 && bCanRunBroadcast)
	{
		bCanRunBroadcast = false;
		
		OnEnemyDied.Broadcast();
	}
}
