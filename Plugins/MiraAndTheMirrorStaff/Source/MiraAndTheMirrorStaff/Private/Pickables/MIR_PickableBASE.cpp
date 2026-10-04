// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickables/MIR_PickableBASE.h"
#include "Save/MIR_SaveSubsystem.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"

// Sets default values
AMIR_PickableBASE::AMIR_PickableBASE()
{
 	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create subobjects.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Set up attachments.
	RootComponent = CollisionComponent;
	Mesh->SetupAttachment(RootComponent);

	// Disable mesh collision and can-step-on setting.
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void AMIR_PickableBASE::BeginPlay()
{
	Super::BeginPlay();

	FName EffectiveID = GetEffectivePickupID();
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
		{
			const bool bCollected = SaveSubsystem->IsPickupCollected(EffectiveID);
			UE_LOG(LogTemp, Log, TEXT("AMIR_PickableBASE::BeginPlay - Pickup [%s] collected: %d"),
				*EffectiveID.ToString(), bCollected ? 1 : 0);

			if (bCollected)
			{
				SetActorHiddenInGame(true);
				SetActorEnableCollision(false);
				Destroy();
				return;
			}
		}
	}
}

FName AMIR_PickableBASE::GetEffectivePickupID() const
{
	if (!PickupID.IsNone())
	{
		return PickupID;
	}

	// 1. Get the package name of the level this actor actually belongs to (handles streaming sublevels)
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

	// 3. ArcadeLoadStreamLevel appends a 32-character hex FGuid to the dynamic level instance name:
	// e.g. "/MiraAndTheMirrorStaff/Levels/MajorAreas/VGrotto/Rooms/1x2/L_MIR_VGRoomD3D454A6F1D2E0B34548B7AC62569C5C5EE7"
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

	// Extract the short clean level asset name (e.g. "L_MIR_VGRoomD3D4")
	FString CleanLevelName = FPackageName::GetShortName(LevelPackageName);

	// 4. Use the clean class name (e.g. "BP_MIR_ManaUp") so runtime instance suffixes (_C_1, _C_2) don't break persistence
	FString ClassName = GetClass()->GetName();
	ClassName.RemoveFromEnd(TEXT("_C"));

	return FName(*FString::Printf(TEXT("%s_%s"), *CleanLevelName, *ClassName));
}

// Called every frame
void AMIR_PickableBASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

