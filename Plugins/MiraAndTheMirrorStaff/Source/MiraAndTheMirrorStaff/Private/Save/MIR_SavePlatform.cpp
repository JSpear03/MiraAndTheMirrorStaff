// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/MIR_SavePlatform.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_SavePlatform::AMIR_SavePlatform()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));

	RootComponent = PlatformMesh;

	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMIR_SavePlatform::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind `OnComponentBeginOverlap()` to user-defined function `OnSaveBeginOverlap()`.
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMIR_SavePlatform::OnSaveBeginOverlap);
}

void AMIR_SavePlatform::OnSaveBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// ========== VALIDATE PLAYER ===============================================================

	// The currently possessed player character, casted to MIR_PlayerChar.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(UGameplayStatics::
															 GetPlayerCharacter(GetWorld(), 0));

	if (!OtherActor || !PlayerCharacter)  // Is NEITHER valid?
	{
		return;
	}

	// ========== VALIDATE CLASS HEIRARCHY ======================================================

	// Represents whether the `OtherActor` is a child of `AMIR_PlayerChar`.
	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	if (!bIsChildOfPlayer)  // Is NOT valid?
	{
		return;
	}

	// ========== CALL UI =======================================================================
	
	// The Save-Room UI can be safely created; the validations above succeeded.
	CallSaveRoomMenu(PlayerCharacter);
}

// Called every frame
void AMIR_SavePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
