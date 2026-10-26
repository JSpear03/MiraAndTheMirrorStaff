// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "LevelStreaming/MIR_LevelStreamActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ArcadeFunctions.h"

// Sets default values
AMIR_LevelStreamActor::AMIR_LevelStreamActor()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Default Subobjects and root component.
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	// Set mobility.
	RootComponent->SetMobility(EComponentMobility::Stationary);

	// Function bindings.
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMIR_LevelStreamActor::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &AMIR_LevelStreamActor::OverlapEnds);
}

// Called when the game starts or when spawned
void AMIR_LevelStreamActor::BeginPlay()
{
	Super::BeginPlay();

	// DEBUG STATEMENTS:
	//if (!LevelToLoad.IsNull())
	//{
	//	if (LevelToLoad.IsValid())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level is validated!"));
	//	}
	//	else {
	//		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level isn't null!"));
	//	}
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level is null."));
	//}
}

// Called every frame
void AMIR_LevelStreamActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*********************************************************************************************
* OVERLAP EVENTS
*********************************************************************************************/

void AMIR_LevelStreamActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Perform streaming only if the player began the overlap.
	if (ValidateOtherActorAsPlayer(OtherActor))
	{
		StreamLevel();
	}
}

void AMIR_LevelStreamActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Perform unstreaming only if the player ended the overlap.
	if (ValidateOtherActorAsPlayer(OtherActor))
	{
		UnstreamLevel();
	}
}

/*********************************************************************************************
* VALIDATION OF OVERLAPPING ACTOR
*********************************************************************************************/

const bool AMIR_LevelStreamActor::ValidateOtherActorAsPlayer(const AActor* OtherActor)
{
	// If `OtherActor` is equal to the first player character in the world,
	// return true; otherwise, false.
	return OtherActor == UGameplayStatics::GetPlayerCharacter(this, 0) ? true : false;
}

/*********************************************************************************************
* STREAMING & UNSTREAMING
*********************************************************************************************/

void AMIR_LevelStreamActor::StreamLevel()
{
	// Credit to Austin for helping out with implementing logic for streaming via file path.
	if (!LevelToLoad.IsNull())
	{
		// The filepath of the level to load.
		FSoftObjectPath levelLoadPath = UWorld::RemovePIEPrefix(LevelToLoad.ToSoftObjectPath().
																GetLongPackageName());

		LoadedLevel = UArcadeFunctions::ArcadeLoadStreamLevel(this,
															  TSoftObjectPtr<UWorld>(levelLoadPath),
															  this->GetActorLocation());
	}
}

void AMIR_LevelStreamActor::UnstreamLevel()
{
	if (!LevelToLoad.IsNull())
	{
		UArcadeFunctions::ArcadeUnloadStreamLevel(this, LoadedLevel);
		LoadedLevel = nullptr;
	}
}
