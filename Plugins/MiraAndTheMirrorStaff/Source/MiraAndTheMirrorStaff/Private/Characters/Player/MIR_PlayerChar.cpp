// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/MIR_PlayerChar.h"
#include "Characters/Enemies/MIR_EnemyBASE.h"
#include "Hazards/MIR_RespawnPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MIR_AttackTypeComponent.h"
#include "Components/MIR_ElementComponent.h"
//#include "Input/MIR_ArcadeController.h"

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
}

// Called every frame.
void AMIR_PlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("No Touchy!"));
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
	if (!OtherActor || !bCanGetHit)  // Validate
	{
		return;
	}

	// Represents whether the `OtherActor` is a child of AMIR_MagicBASE.
	bool bIsChildOfMagic = OtherActor->GetClass()->IsChildOf<AMIR_MagicBASE>();

	if (!bIsChildOfMagic)  // Validate
	{
		return;
	}

	// Ignore player-summoned magic.
	AActor* MagicInstigator = OtherActor->GetInstigator();  // The Controller that initiated the summon.
	AActor* MagicOwner = OtherActor->GetOwner();			// The actor that summoned the magic.

	if (MagicInstigator == this || MagicOwner == this)
	{
		return;
	}

	// ========== HANDLE MAGIC OVERLAP =======================================================

	AMIR_MagicBASE* IncomingMagic = Cast<AMIR_MagicBASE>(OtherActor);
	if (IncomingMagic && IncomingMagic->ElementComponent && IncomingMagic->AttackTypeComponent)
	{
		// Ignore copy-mode magic (it's for copying, not damaging)
		if (IncomingMagic->AttackTypeComponent->GetAttackTypeProperty() == EAttackProperties::Copy)
		{
			return;
		}

		SetCanGetHit(false);

		// The `IncomingMagic`'s Element Name.
		EElements AttackerElementName = IncomingMagic->ElementComponent->GetElementName();

		// The receiver's Element Name.
		EElements ReceiverElementName = ElementComponent->GetElementName();

		// The `IncomingMagic`'s Damage.
		int32 AttackerDamage = IncomingMagic->AttackTypeComponent->GetDamage();

		if (AttackerDamage <= 0)
		{
			AttackerDamage = 1;
		}

		// Handle damage calculations, check alive status, and start i-frames.
		CalculateDamage(AttackerElementName, ReceiverElementName, AttackerDamage);
		LoadingGameOver();
		StartCooldownTimer();

		// Destroy the projectile upon impact
		IncomingMagic->Destroy();
	}
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

/************************************************************************************
 * INVINCIBILITY FRAMES
 ***********************************************************************************/

void AMIR_PlayerChar::StartCooldownTimer()
{
	FTimerHandle CooldownTimerHandle;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	const FTimerDelegate& CooldownTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager, &CooldownTimerHandle]
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Player Cooldown"));

			TimerManager.ClearTimer(CooldownTimerHandle);

			SetCanGetHit(true);  // Disable i-frames.
		}
	);

	TimerManager.SetTimer(CooldownTimerHandle, CooldownTimerDelegate, CooldownSeconds, false);
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
	switch (KeyItem)
	{
	case EMIR_KeyItem::MirrorStaff:
		bHasMirrorStaff = true;
		break;

	case EMIR_KeyItem::SkyboundShoes:
		bHasSkyboundShoes = true;
		break;

	case EMIR_KeyItem::SpiritStole:
		bHasSpiritStole = true;
		break;

	case EMIR_KeyItem::KeyGemstone:
		KeyGemstoneCount++;
		break;

	case EMIR_KeyItem::KeyGemstoneRed:
		bIsGemRed = true;
		break;

	case EMIR_KeyItem::KeyGemstoneBlue:
		bIsGemBlue = true;
		break;

	case EMIR_KeyItem::KeyGemstoneGreen:
		bIsGemGreen = true;
		break;

	case EMIR_KeyItem::KeyGemstoneYellow:
		bIsGemYellow = true;
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
}
