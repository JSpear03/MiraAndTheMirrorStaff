// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_EnemyBASE.h"
#include "MIR_MagicBASE.h"
#include "MIR_PlayerChar.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values.
AMIR_EnemyBASE::AMIR_EnemyBASE()
{
	CopiableComponent = CreateDefaultSubobject<UMIR_CopiableComponent>(TEXT("Copiable Component"));

	ElementComponent->ElementType.ElementalNeutralities = { "Nothing", "Flame", "Ice", "Thunder" };
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
	AMIR_MagicBASE* CurrentMagic;
	FString CopiedMagic;

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

	CurrentMagic = Cast<AMIR_MagicBASE>(OtherActor);
	CopiedMagic = CurrentMagic->ElementComponent->ElementType.ElementName;

	// This enemy is the copy source, so it is the interface receiver. Execute_CopyMagic is a
	// static function: the object it dispatches on is the first argument, not the "->" prefix.
	IIMIR_Copiable::Execute_CopyMagic(this, PlayerCharacter, CopiedMagic);
}

void AMIR_EnemyBASE::CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
	const FString& MagicToCopy)
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
