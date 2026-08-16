// Fill out your copyright notice in the Description page of Project Settings.

#include "MIR_LevelStreamActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ArcadeFunctions.h"

// Sets default values
AMIR_LevelStreamActor::AMIR_LevelStreamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMIR_LevelStreamActor::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &AMIR_LevelStreamActor::OverlapEnds);
}

// Called when the game starts or when spawned
void AMIR_LevelStreamActor::BeginPlay()
{
	Super::BeginPlay();

	if (!LevelToLoad.IsNull())
	{
		if (LevelToLoad.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level is validated!"));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level isn't null!"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("A Level is null."));
	}
	
}

void AMIR_LevelStreamActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// The currently possessed player character.
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor == MyCharacter && !LevelToLoad.IsNull())
	{
		// Austin's code.
		FSoftObjectPath levelloadpath = UWorld::RemovePIEPrefix(LevelToLoad.ToSoftObjectPath().GetLongPackageName());  // The filepath of the level to load.
		FString levelPath = levelloadpath.ToString();  // Current level path as a string.
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("Level will load! %s"), *levelPath));
		LoadedLevel = UArcadeFunctions::ArcadeLoadStreamLevel(this, TSoftObjectPtr<UWorld>(levelloadpath), this->GetActorLocation());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Level will NOT load!"));
	}
}

void AMIR_LevelStreamActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// The currently possessed player character.
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor == MyCharacter && !LevelToLoad.IsNull())
	{
		UArcadeFunctions::ArcadeUnloadStreamLevel(this, LoadedLevel);
		LoadedLevel = nullptr;
	}
}

// Called every frame
void AMIR_LevelStreamActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

