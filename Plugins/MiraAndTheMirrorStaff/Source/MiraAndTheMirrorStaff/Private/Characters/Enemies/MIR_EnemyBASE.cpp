#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Magic/MIR_MagicBASE.h"
#include "Components/CapsuleComponent.h"
#include "Components/MIR_Combat_Component.h"
#include "Kismet/GameplayStatics.h"


// Sets default values.
AMIR_EnemyBASE::AMIR_EnemyBASE()
{
	CopiableComponent = CreateDefaultSubobject<UMIR_CopiableComponent>(TEXT("Copiable Component"));
	CombatComponent = CreateDefaultSubobject<UMIR_Combat_Component>(TEXT("Combat Component"));

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
	// The currently possessed player character.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(UGameplayStatics::
															 GetPlayerCharacter(GetWorld(), 0));

	if (!OtherActor || !PlayerCharacter)  // Are NEITHER valid?
	{
		return;
	}

	bool bIsChildOfMagic = OtherActor->GetClass()->IsChildOf<AMIR_MagicBASE>();

	if (!bIsChildOfMagic)  // Is NOT valid?
	{
		return;
	}

	// Prevent friendly fire and self-damage:
	// Ignore magic if it was cast by this enemy, owned by this enemy, or cast by another enemy.
	AActor* MagicInstigator = OtherActor->GetInstigator();
	AActor* MagicOwner = OtherActor->GetOwner();

	if (MagicInstigator == this || MagicOwner == this ||
		(MagicInstigator && MagicInstigator->IsA<AMIR_EnemyBASE>()) ||
		(MagicOwner && MagicOwner->IsA<AMIR_EnemyBASE>()))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("Is Magic: %s"), 
									 *OtherActor->GetClass()->GetName()));		// TODO: Delete

	AMIR_MagicBASE* IncomingMagic = Cast<AMIR_MagicBASE>(OtherActor);
	if (!IncomingMagic)
	{
		return;
	}

	// Prevent the same magic instance from affecting this enemy more than once
	// (e.g. dual-capsule overlaps, AoE expansion, melee swing re-entry).
	// A single spell can still hit multiple different enemies.
	if (!IncomingMagic->TryRegisterHit(this))
	{
		return;
	}

	if (IncomingMagic->AttackTypeComponent && IncomingMagic->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Do Copy"));
		// This enemy is the copy source, so it is the interface receiver. Execute_CopyMagic is a
		// static function: the object it dispatches on is the first argument, not the "->" prefix.
		IIMIR_Copiable::Execute_CopyMagic(this, PlayerCharacter, GetStoredMagic());
	}
	// Incoming magic is a real attack — apply damage using the element/attack-type system.
	else if (IncomingMagic->ElementComponent && IncomingMagic->AttackTypeComponent)
	{
		// The `IncomingMagic`'s Element Name.
		EElements AttackerElementName = IncomingMagic->ElementComponent->GetElementName();

		// The receiver's Element Name.
		EElements ReceiverElementName = ElementComponent->GetElementName();

		// The `IncomingMagic`'s Damage.
		int32 AttackerDamage = IncomingMagic->AttackTypeComponent->GetDamage();

		CalculateDamage(AttackerElementName, ReceiverElementName, AttackerDamage);
		CheckHealth();
	}
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

void AMIR_EnemyBASE::CheckHealth()
{
	if (HealthComponent->GetHealth() == 0 && bCanRunBroadcast)
	{
		bCanRunBroadcast = false;

		// Notify the Combat Component so active BT tasks (Attack Player, etc.)
		// break immediately instead of freezing in their tick loop.
		if (CombatComponent)
		{
			CombatComponent->SetDeactivated(true);
		}

		OnEnemyDied.Broadcast(this);
		SetNewMaterial();
	}
}

// Called every frame.
void AMIR_EnemyBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
