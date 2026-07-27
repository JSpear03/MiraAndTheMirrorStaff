// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_PlayerChar.h"
#include "MIR_EnemyBASE.h"
#include "MIR_RespawnPoint.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values.
AMIR_PlayerChar::AMIR_PlayerChar()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	ManaComponent = CreateDefaultSubobject<UMIR_ManaComponent>(TEXT("Mana Component"));

	SpringArm->SetupAttachment(RootComponent); 

	// Attaches camera to the end of the spring arm. 
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 

	// Allows the player's yaw to match the camera yaw. 
	// Needs to be turned off, since the point of view might be third person.
	bUseControllerRotationYaw = false; 

	SpringArm->bUsePawnControlRotation = true;  // Allows rotation of the spring arm. 

	ElementComponent->ElementType.ElementName = "Ranged Nothing";
}

void AMIR_PlayerChar::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AMIR_PlayerChar::HitEvent);
}

void AMIR_PlayerChar::HitEvent(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, 
	const FHitResult& Hit)
{
	bool bIsChildOfEnemy = OtherActor->GetClass()->IsChildOf<AMIR_EnemyBASE>();

	// Reduces health by one, when collided with enemy. 
	if (bIsChildOfEnemy && bCanGetHit)
	{
		AMIR_EnemyBASE* CurrentEnemy = Cast<AMIR_EnemyBASE>(OtherActor);

		bCanGetHit = false;

		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("No Touchy!"));
		HealthComponent->HealthReduction(CurrentEnemy->ContactDamage);

		StartCooldownTimer();
	}
}

void AMIR_PlayerChar::StartCooldownTimer()
{
	FTimerHandle CooldownTimerHandle;
	const UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	const FTimerDelegate& CooldownTimerDelegate = FTimerDelegate::CreateLambda([this, &TimerManager, &CooldownTimerHandle]
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Player Cooldown"));

			TimerManager.ClearTimer(CooldownTimerHandle);

			SetCanGetHit(true);
		}
	);

	TimerManager.SetTimer(CooldownTimerHandle, CooldownTimerDelegate, CooldownSeconds, false);
}

void AMIR_PlayerChar::SetCanGetHit(bool bGetHit)
{
	bCanGetHit = bGetHit;
}

void AMIR_PlayerChar::SetStoredMagic(FString MagicType)
{
	StoredMagicType = MagicType;
}

FString AMIR_PlayerChar::GetStoredMagic()
{
	return StoredMagicType;
}

// Leave most of the cases blank for now.
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
	default:
		break;
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

	bCanGetHit = false;
	HealthComponent->HealthReduction(Damage);
	StartCooldownTimer();
}

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
