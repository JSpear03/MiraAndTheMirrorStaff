#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "Magic/MIR_MagicBASE.h"
#include "Components/CapsuleComponent.h"
#include "Components/MIR_Combat_Component.h"
#include "Kismet/GameplayStatics.h"
#include "Save/MIR_SaveSubsystem.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"


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

	if (bIsBossVariant)
	{
		FName EffectiveID = GetEffectiveBossID();
		if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
		{
			if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
			{
				const bool bDefeated = SaveSubsystem->IsBossDefeated(EffectiveID);
				UE_LOG(LogTemp, Log, TEXT("AMIR_EnemyBASE::BeginPlay - Boss [%s] defeated: %d"),
					*EffectiveID.ToString(), bDefeated ? 1 : 0);

				if (bDefeated)
				{
					SetActorHiddenInGame(true);
					SetActorEnableCollision(false);
					Destroy();
					return;
				}
			}
		}
	}

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
		if (AttackerElementName == EElements::None)
		{
			AttackerElementName = IncomingMagic->ElementComponent->GetCurrentElement();
		}
		if (AttackerElementName == EElements::None)
		{
			const FString MagicClassName = IncomingMagic->GetClass()->GetName();
			if (MagicClassName.Contains(TEXT("Flame"), ESearchCase::IgnoreCase) || MagicClassName.Contains(TEXT("Fire"), ESearchCase::IgnoreCase))
			{
				AttackerElementName = EElements::Flame;
			}
			else if (MagicClassName.Contains(TEXT("Ice"), ESearchCase::IgnoreCase) || MagicClassName.Contains(TEXT("Frost"), ESearchCase::IgnoreCase))
			{
				AttackerElementName = EElements::Ice;
			}
			else if (MagicClassName.Contains(TEXT("Thunder"), ESearchCase::IgnoreCase) || MagicClassName.Contains(TEXT("Lightning"), ESearchCase::IgnoreCase))
			{
				AttackerElementName = EElements::Thunder;
			}
		}

		// The receiver's Element Name.
		EElements ReceiverElementName = ElementComponent ? ElementComponent->GetElementName() : EElements::None;

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

		if (bIsBossVariant)
		{
			if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
			{
				if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
				{
					SaveSubsystem->MarkBossDefeated(GetEffectiveBossID());
					UE_LOG(LogTemp, Log, TEXT("AMIR_EnemyBASE - Marked boss defeated: %s"),
						*GetEffectiveBossID().ToString());
				}
			}
		}

		OnEnemyDied.Broadcast(this);
		SetNewMaterial();
	}
}

FName AMIR_EnemyBASE::GetEffectiveBossID() const
{
	if (!BossID.IsNone())
	{
		return BossID;
	}

	// 1. Get the package name of the level this boss belongs to (handles streaming sublevels)
	FString LevelPackageName;
	if (ULevel* Level = GetLevel())
	{
		if (UPackage* Package = Level->GetPackage())
		{
			LevelPackageName = Package->GetName();
		}
	}

	if (LevelPackageName.IsEmpty() && GetWorld())
	{
		LevelPackageName = GetWorld()->GetMapName();
	}

	// 2. Strip PIE prefix if present (e.g. UEDPIE_0_)
	int32 PieIndex = LevelPackageName.Find(TEXT("UEDPIE_"));
	if (PieIndex != INDEX_NONE)
	{
		int32 SlashAfter = LevelPackageName.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromStart, PieIndex);
		if (SlashAfter != INDEX_NONE)
		{
			LevelPackageName.RemoveAt(PieIndex, SlashAfter - PieIndex + 1);
		}
		else
		{
			int32 NextUnderscore = LevelPackageName.Find(TEXT("_"), ESearchCase::IgnoreCase, ESearchDir::FromStart, PieIndex + 7);
			int32 CharsToRemove = (NextUnderscore != INDEX_NONE) ? (NextUnderscore - PieIndex + 1) : 9;
			LevelPackageName.RemoveAt(PieIndex, CharsToRemove);
		}
	}

	// 3. ArcadeLoadStreamLevel appends a 32-character hex FGuid to dynamic level instance names
	if (LevelPackageName.Len() > 32)
	{
		bool bEndsWithGuid = true;
		int32 SuffixStart = LevelPackageName.Len() - 32;
		for (int32 i = SuffixStart; i < LevelPackageName.Len(); ++i)
		{
			TCHAR c = LevelPackageName[i];
			if (!FChar::IsHexDigit(c))
			{
				bEndsWithGuid = false;
				break;
			}
		}

		if (bEndsWithGuid)
		{
			LevelPackageName.LeftInline(SuffixStart);
		}
	}

	// Extract the short clean level asset name (e.g. "L_MIR_VGRoomD6")
	FString CleanLevelName = FPackageName::GetShortName(LevelPackageName);

	// 4. Use the clean class name (e.g. "BP_MIR_BigSlimeFlame")
	FString ClassName = GetClass()->GetName();
	ClassName.RemoveFromEnd(TEXT("_C"));

	return FName(*FString::Printf(TEXT("%s_%s"), *CleanLevelName, *ClassName));
}

// Called every frame.
void AMIR_EnemyBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
