// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Characters/Player/MIR_PlayerChar.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Hazards/MIR_RespawnPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Save/MIR_SaveSubsystem.h"
#include "Misc/Camera/MIR_CameraMovePoints.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values.
AMIR_PlayerChar::AMIR_PlayerChar()
{
	// Create default subobects.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	ManaComponent = CreateDefaultSubobject<UMIR_ManaComponent>(TEXT("Mana Component"));
	MeleeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Melee Arrow"));
	RangedArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Ranged Arrow"));
	AOEArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("AOE Arrow"));

	// Parent subobjects appropriately.
	SpringArm->SetupAttachment(RootComponent); 
	MeleeArrow->SetupAttachment(RootComponent);
	RangedArrow->SetupAttachment(RootComponent);
	AOEArrow->SetupAttachment(RootComponent);

	// Puts arrows at the front of the player.
	MeleeArrow->SetWorldLocation(FVector(50.0, 0.0, 30.0));
	RangedArrow->SetWorldLocation(FVector(50.0, 0.0, 30.0));

	// Arrow colors.
	MeleeArrow->ArrowColor  = FColor::Red;
	RangedArrow->ArrowColor = FColor::Green;
	AOEArrow->ArrowColor    = FColor::Blue;

	// Allows character to rotate toward the direction of movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Attaches camera to the end of the spring arm. 
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 

	// Allows the player's yaw to match the camera yaw. 
	// Must be turned off since the point of view is third person.
	bUseControllerRotationYaw = false;

	SpringArm->bUsePawnControlRotation = true;  // Allows rotation of the spring arm. 
}

// Called when game starts.
void AMIR_PlayerChar::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AMIR_PlayerChar::HitEvent);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMIR_PlayerChar::OnOverlapBegin);

	// Periodically track active room and discover it in SaveSubsystem
	GetWorldTimerManager().SetTimer(RoomCheckTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		UpdateCurrentRoom();
	}), 0.25f, true, 0.05f);
}

// Called every frame.
void AMIR_PlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Push movement speed directly to the AnimInstance so BlendSpace (Idle/Run) always updates
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			const float GroundSpeed = GetVelocity().Size2D();

			const FName SpeedPropNames[] = {
				FName("VelocityLengthXY"),
				FName("Velocity"),
				FName("Speed"),
				FName("CurrentVelocity")
			};

			for (const FName& PropName : SpeedPropNames)
			{
				if (FProperty* Prop = AnimInst->GetClass()->FindPropertyByName(PropName))
				{
					if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Prop))
					{
						FloatProp->SetPropertyValue_InContainer(AnimInst, GroundSpeed);
					}
					else if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Prop))
					{
						DoubleProp->SetPropertyValue_InContainer(AnimInst, static_cast<double>(GroundSpeed));
					}
				}
			}
		}
	}
}


/************************************************************************************
 * SUSTAINING DAMAGE
 ***********************************************************************************/

void AMIR_PlayerChar::HitEvent(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, 
	const FHitResult& Hit)
{
	bool bIsChildOfEnemy = OtherActor->GetClass()->IsChildOf<AMIR_EnemyBASE>();

	// Reduces health by one upon collision with an enemy.
	if (bIsChildOfEnemy && bCanGetHit)
	{
		AMIR_EnemyBASE* CurrentEnemy = Cast<AMIR_EnemyBASE>(OtherActor);

		SetCanGetHit(false);

		HealthComponent->HealthReduction(CurrentEnemy->ContactDamage);

		// Check for game over and start i-frames.
		LoadingGameOver();
		StartCooldownTimer();
	}
}

void AMIR_PlayerChar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || !bCanGetHit) { return; }

	// Represents whether the `OtherActor` is a child of AMIR_MagicBASE.
	bool bIsChildOfMagic = OtherActor->GetClass()->IsChildOf<AMIR_MagicBASE>();

	if (!bIsChildOfMagic) { return; }

	// Ignore player-summoned magic.
	AActor* MagicInstigator = OtherActor->GetInstigator();  // Controller that initiated the summon.
	AActor* MagicOwner = OtherActor->GetOwner();			// The actor that summoned the magic.

	if (MagicInstigator == this || MagicOwner == this) { return; }

	// ========== HANDLE MAGIC OVERLAP =======================================================

	AMIR_MagicBASE* IncomingMagic = Cast<AMIR_MagicBASE>(OtherActor);

	if (!IncomingMagic || (IncomingMagic && !IncomingMagic->ElementComponent) 
		|| (IncomingMagic && !IncomingMagic->AttackTypeComponent))
	{
		return;
	}

	// Ignore copy-mode magic (it's for copying, not damaging)
	if (IncomingMagic->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
	{
		return;
	}

	SetCanGetHit(false);

	// The `IncomingMagic`'s Element Name.
	EElements AttackerElementName = IncomingMagic->ElementComponent->GetElementName();

	if (AttackerElementName == EElements::None)
	{
		AttackerElementName = IncomingMagic->ElementComponent->GetCurrentElement();
	}

	if (AttackerElementName == EElements::None) 
	{ 
		AttackerElementName = GetAttackElement(IncomingMagic); 
	}

	// The receiver's Element Name.
	EElements ReceiverElementName = ElementComponent ? ElementComponent->GetElementName() : EElements::None;

	// The `IncomingMagic`'s Damage.
	int32 AttackerDamage = IncomingMagic->AttackTypeComponent->GetDamage();

	if (AttackerDamage <= 0) { AttackerDamage = 1; }

	// Handle damage calculations, check alive status, and start i-frames.
	CalculateDamage(AttackerElementName, ReceiverElementName, AttackerDamage);
	LoadingGameOver();
	StartCooldownTimer();

	// Destroy the projectile upon impact
	IncomingMagic->Destroy();
}

void AMIR_PlayerChar::SetStoredMagic(TSubclassOf<class AMIR_MagicBASE> MagicType)
{
	Super::SetStoredMagic(MagicType);
	CopyElement();
}

void AMIR_PlayerChar::ApplyHazardDamage(int32 Damage)
{
	// Hazards share the same invincibility window as enemy contact,
	// so standing on spikes can't drain health every frame.
	if (!bCanGetHit)
	{
		return;
	}

	SetCanGetHit(false);
	HealthComponent->HealthReduction(Damage);
	LoadingGameOver();
	StartCooldownTimer();
}


EElements AMIR_PlayerChar::GetAttackElement(AMIR_MagicBASE* IncomingMagic)
{
	const FString MagicClassName = IncomingMagic->GetClass()->GetName();

	if (MagicClassName.Contains(TEXT("Flame"), ESearchCase::IgnoreCase) ||
		MagicClassName.Contains(TEXT("Fire"), ESearchCase::IgnoreCase))
	{
		return EElements::Flame;
	}
	else if (MagicClassName.Contains(TEXT("Ice"), ESearchCase::IgnoreCase) ||
		MagicClassName.Contains(TEXT("Frost"), ESearchCase::IgnoreCase))
	{
		return EElements::Ice;
	}
	else if (MagicClassName.Contains(TEXT("Thunder"), ESearchCase::IgnoreCase) ||
		MagicClassName.Contains(TEXT("Lightning"), ESearchCase::IgnoreCase))
	{
		return EElements::Thunder;
	}

	return EElements::None;  // Might not be used in the program, but all cases need a return statement.
}

/************************************************************************************
 * INVINCIBILITY FRAMES
 ***********************************************************************************/

void AMIR_PlayerChar::StartCooldownTimer()
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(InvincibilityTimerHandle);

		const FTimerDelegate CooldownTimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			SetCanGetHit(true);  // Disable i-frames.
		});

		TimerManager.SetTimer(InvincibilityTimerHandle, CooldownTimerDelegate, CooldownSeconds, false);
	}
}

void AMIR_PlayerChar::SetCanGetHit(bool bGetHit)
{
	bCanGetHit = bGetHit;
}

/************************************************************************************
 * OBTAINING KEY ITEMS
 ***********************************************************************************/

void AMIR_PlayerChar::GrantKeyItem(EMIR_KeyItem KeyItem)
{
	UMIR_SaveSubsystem* SaveSubsystem = nullptr;

	if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		SaveSubsystem = GameInstance->GetSubsystem<UMIR_SaveSubsystem>();
	}

	switch (KeyItem)
	{
	case EMIR_KeyItem::MirrorStaff:
		bHasMirrorStaff = true;
		// Swap from Magic Wand (RangedNothing) to Mirror Staff (defaults to MeleeCopy)
		SetStoredMagic(nullptr);
		if (ElementComponent)
		{
			ElementComponent->SetElementDetails(EElements::None);
		}

		CopyElement();

		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasMirrorStaff(true);
		}
		break;
	case EMIR_KeyItem::SkyboundShoes:
		bHasSkyboundShoes = true;

		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasSkyboundShoes(true);
		}
		break;
	case EMIR_KeyItem::SpiritStole:
		bHasSpiritStole = true;

		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasSpiritStole(true);
		}
		break;
	case EMIR_KeyItem::KeyGemstoneRed:
		bIsGemRed = true;
		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasRedKeystone(true);
		}
		break;
	case EMIR_KeyItem::KeyGemstoneBlue:
		bIsGemBlue = true;
		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasBlueKeystone(true);
		}
		break;
	case EMIR_KeyItem::KeyGemstoneGreen:
		bIsGemGreen = true;
		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasGreenKeystone(true);
		}
		break;
	case EMIR_KeyItem::KeyGemstoneYellow:
		bIsGemYellow = true;
		if (SaveSubsystem)
		{
			SaveSubsystem->SetHasYellowKeystone(true);
		}
		break;
	default:
		break;
	}
}

/************************************************************************************
 * RESPAWNING
 ***********************************************************************************/

void AMIR_PlayerChar::SetLastRespawnPoint(AMIR_RespawnPoint* RespawnPoint)
{
	LastRespawnPoint = RespawnPoint;
}

void AMIR_PlayerChar::RespawnAtLastPoint()
{
	if (!LastRespawnPoint)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("RespawnAtLastPoint: no respawn point set. Place an AMIR_RespawnPoint trigger at the room entrance."));
		return;
	}

	// Falling momentum would otherwise carry through the teleport and
	// slide the player straight back into the hazard.
	GetCharacterMovement()->StopMovementImmediately();
	SetActorLocation(LastRespawnPoint->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	OnRespawn.Broadcast();
}

/************************************************************************************
 * COMBAT ANIMATIONS
 ***********************************************************************************/

UAnimSequenceBase* AMIR_PlayerChar::GetAttackAnimation(EAttackTypes AttackType) const
{
	switch (AttackType)
	{
	case EAttackTypes::Melee:
		return MeleeAttackAnim;
	case EAttackTypes::Ranged:
		return RangedAttackAnim;
	case EAttackTypes::AoE:
		return AOEAttackAnim;
	default:
		return MeleeAttackAnim;
	}
}

UAnimSequenceBase* AMIR_PlayerChar::GetSubAttackAnimation() const
{
	return DaggerAttackAnim ? DaggerAttackAnim.Get() : MeleeAttackAnim.Get();
}

void AMIR_PlayerChar::UpdateCurrentRoom()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 1. Check overlapping CMPs via capsule
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AMIR_CameraMovePoints::StaticClass());

	AMIR_CameraMovePoints* FoundCMP = nullptr;
	if (OverlappingActors.Num() > 0)
	{
		FoundCMP = Cast<AMIR_CameraMovePoints>(OverlappingActors[0]);
	}

	// 2. If no direct capsule overlap (e.g. boundary seam / preloaded spawn), query all CMPs for box containment
	if (!FoundCMP)
	{
		TArray<AActor*> AllCMPs;
		UGameplayStatics::GetAllActorsOfClass(World, AMIR_CameraMovePoints::StaticClass(), AllCMPs);
		float ClosestDistSq = MAX_flt;
		const FVector PlayerLoc = GetActorLocation();

		for (AActor* Actor : AllCMPs)
		{
			if (AMIR_CameraMovePoints* CMP = Cast<AMIR_CameraMovePoints>(Actor))
			{
				if (UBoxComponent* Box = CMP->GetCameraMoveTrigger())
				{
					if (Box->Bounds.GetBox().IsInsideOrOn(PlayerLoc))
					{
						FoundCMP = CMP;
						break;
					}
				}

				float DistSq = FVector::DistSquared(PlayerLoc, CMP->GetActorLocation());
				if (DistSq < ClosestDistSq)
				{
					ClosestDistSq = DistSq;
					FoundCMP = CMP;
				}
			}
		}
	}

	if (FoundCMP)
	{
		CurrentCMP = FoundCMP;
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
			{
				const FName RoomID = FoundCMP->GetCleanRoomID();
				if (!RoomID.IsNone())
				{
					SaveSubsystem->MarkRoomVisited(RoomID);
				}
			}
		}
	}
}

