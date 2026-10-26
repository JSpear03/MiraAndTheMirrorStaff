// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Characters/Bosses/MIR_FinalBossDoor.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/MIR_PlayerChar.h"

// Sets default values
AMIR_FinalBossDoor::AMIR_FinalBossDoor()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));

	RootComponent = BoxCollision;
	DoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_FinalBossDoor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMIR_FinalBossDoor::OnFinalDoorOverlap);
}

void AMIR_FinalBossDoor::OnFinalDoorOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	if (!OtherActor || !bIsChildOfPlayer)
	{
		return;
	}

	// Open door only if all unique Key Gemstones are collected.
	if (PlayerCharacter->bIsGemRed && PlayerCharacter->bIsGemBlue && PlayerCharacter->bIsGemGreen &&
		PlayerCharacter->bIsGemYellow)
	{
		OpenDoor();
	}
}

// Called every frame
void AMIR_FinalBossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

