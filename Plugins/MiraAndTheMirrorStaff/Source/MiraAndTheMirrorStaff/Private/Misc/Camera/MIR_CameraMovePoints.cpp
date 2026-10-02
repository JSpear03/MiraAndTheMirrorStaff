// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/Camera/MIR_CameraMovePoints.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Camera/MIR_Camera.h"

/** Static-variable initializations. */
// The base length of a room on the x-axis.
const float AMIR_CameraMovePoints::BaseRoomExtentX = 600.0f;

// The base length of a room on the y-axis.
const float AMIR_CameraMovePoints::BaseRoomExtentY = 1100.0f;

// The base length of a room on the z-axis.
const float AMIR_CameraMovePoints::BaseRoomExtentZ = 500.0f;


// Sets default values
AMIR_CameraMovePoints::AMIR_CameraMovePoints()
{
 	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create default subobjects.
	CameraMoveTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraMoveTrigger"));
	Max_X = CreateDefaultSubobject<UBillboardComponent>(TEXT("Max_X"));
	Min_X = CreateDefaultSubobject<UBillboardComponent>(TEXT("Min_X"));
	Max_Y = CreateDefaultSubobject<UBillboardComponent>(TEXT("Max_Y"));
	Min_Y = CreateDefaultSubobject<UBillboardComponent>(TEXT("Min_Y"));
	Northeast = CreateDefaultSubobject<UBillboardComponent>(TEXT("Northeast"));
	Southeast = CreateDefaultSubobject<UBillboardComponent>(TEXT("Southeast"));
	Northwest = CreateDefaultSubobject<UBillboardComponent>(TEXT("Northwest"));
	Southwest = CreateDefaultSubobject<UBillboardComponent>(TEXT("Southwest"));

	// Initialize `BoundaryArray`.
	ConstructBoundaryArray();

	// Parent subobjects appropriately.
	CameraMoveTrigger->SetupAttachment(RootComponent);
	Max_X->SetupAttachment(CameraMoveTrigger);
	Min_X->SetupAttachment(CameraMoveTrigger);
	Max_Y->SetupAttachment(CameraMoveTrigger);
	Min_Y->SetupAttachment(CameraMoveTrigger);
	Northeast->SetupAttachment(CameraMoveTrigger);
	Southeast->SetupAttachment(CameraMoveTrigger);
	Northwest->SetupAttachment(CameraMoveTrigger);
	Southwest->SetupAttachment(CameraMoveTrigger);

	// Set mobility to stationary (allows in-editor changes).
	CameraMoveTrigger->SetMobility(EComponentMobility::Stationary);

	// Set up default 1x1 room.
	RoomConstructor();

	// Function binding(s).
	CameraMoveTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this,
		&AMIR_CameraMovePoints::OverlapBegins);
}

// LEAVE UNCOMMENTED; WILL CRASH EDITOR IF EXECUTED IN A CHILD BLUEPRINT.
//#if WITH_EDITOR
//void AMIR_CameraMovePoints::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	// The name of the changed property.
//	const FName PropertyName = PropertyChangedEvent.Property
//		? PropertyChangedEvent.Property->GetFName()
//		: NAME_None;
//
//	// Update `CameraMoveTrigger` and all boundaries if `ERoomDeterminer` is changed.
//	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMIR_CameraMovePoints, ERoomDeterminer))
//	{
//		RoomConstructor();
//	}
//}
//#endif

// Called when the game starts or when spawned
void AMIR_CameraMovePoints::BeginPlay()
{
	Super::BeginPlay();

	ConstructBoundaryLocationsArray();
	
}

// Called every frame
void AMIR_CameraMovePoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/************************************************************************************
 * OVERLAP
 ***********************************************************************************/

void AMIR_CameraMovePoints::OverlapBegins(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor) { return; }

	// The currently possessed player character.
	AMIR_PlayerChar* PlayerChar = Cast<AMIR_PlayerChar>(OtherActor);

	if (!PlayerChar) { return; }

	// Assign self for access in BP_MIR_Camera.
	PlayerChar->CurrentCMP = this;

	// Determine closest boundary to player.
	DetermineToPanBoundary(PlayerChar);
}

/************************************************************************************
 * CONSTRUCTION
 ***********************************************************************************/

void AMIR_CameraMovePoints::ConstructBoundaryArray()
{
	BoundaryArray.Add(Max_X);
	BoundaryArray.Add(Min_X);
	BoundaryArray.Add(Max_Y);
	BoundaryArray.Add(Min_Y);
	BoundaryArray.Add(Northeast);
	BoundaryArray.Add(Southeast);
	BoundaryArray.Add(Northwest);
	BoundaryArray.Add(Southwest);
}

void AMIR_CameraMovePoints::ConstructBoundaryLocationsArray()
{
	for (UBillboardComponent* CurrentBoundary : BoundaryArray)
	{
		if (!CurrentBoundary) { continue; }

		BoundaryLocationsArray.Add(CurrentBoundary->GetComponentLocation());
	}
}

void AMIR_CameraMovePoints::RoomConstructor()
{
	DetermineAppearance();
}

void AMIR_CameraMovePoints::DetermineAppearance()
{
	// Switch to determine the data for the actor.
	switch (ERoomDeterminer)
	{
	case ERoomSizes::OneByOne:
	{
		MakeRoomStruct();
		break;
	}
	case ERoomSizes::OneByTwo:
	{
		// X and Y are swapped intentionally; this is how rooms are (mistakenly)
		// set up in-game.
		MakeRoomStruct(2, 1);
		break;
	}
	case ERoomSizes::TwoByOne:
	{
		// X and Y are swapped intentionally; this is how rooms are (mistakenly)
		// set up in-game.
		MakeRoomStruct(1, 2);
		break;
	}
	case ERoomSizes::TwoByTwo:
	{
		MakeRoomStruct(2);
		break;
	}
	case ERoomSizes::ThreeByThree:
	{
		MakeRoomStruct(3);
		break;
	}
	default:
	{
		// Do nothing.
		break;
	}
	}
}

void AMIR_CameraMovePoints::MakeRoomStruct(int32 RoomSizeX, int32 RoomSizeY)
{
	// Box-extent formula:
	// - BoxX: (600.0 * RoomSizeX) + (100.0 * (RoomSizeX - 1))
	// - BoxY: (1100.0 * RoomSizeY) + (100.0 * (RoomSizeY - 1))

	// Boundary-pos formula:
	// - XVector: BoxX - 600.0
	// - YVector: BoxY - 1100.0

	float BoxX = CalculateBoxExtentOnAxis(BaseRoomExtentX, RoomSizeX);
	float BoxY = CalculateBoxExtentOnAxis(BaseRoomExtentY, RoomSizeY);
	FVector BoxExtent = FVector(BoxX, BoxY, BaseRoomExtentZ);

	float MaxPosX = BoxX - BaseRoomExtentX;
	float MaxPosY = BoxY - BaseRoomExtentY;

	FVector MaxXBoundary = FVector(MaxPosX, 0.0, 0.0);
	FVector MaxYBoundary = FVector(0.0, MaxPosY, 0.0);

	// The relative locations of all eight Billboard Components.
	TArray<FVector> BoundaryPos = { MaxXBoundary, -MaxXBoundary, MaxYBoundary, -MaxYBoundary,
									MaxXBoundary + MaxYBoundary, -MaxXBoundary + MaxYBoundary,
									MaxXBoundary - MaxYBoundary, -MaxXBoundary - MaxYBoundary };

	// Determine whether to use boundary; box extent and boundary locations are ready.
	bool bUseBoundary = false;

	if (RoomSizeX <= 1 && RoomSizeY <= 1)
	{
		bUseBoundary = true;
	}

	PrepareAppearance(bUseBoundary, BoxExtent, BoundaryPos);

}

void AMIR_CameraMovePoints::MakeRoomStruct(int32 RoomSizeXY)
{
	MakeRoomStruct(RoomSizeXY, RoomSizeXY);
}

float AMIR_CameraMovePoints::CalculateBoxExtentOnAxis(const float BaseRoomExtent,
	const float RoomSize)
{
	return (BaseRoomExtent * RoomSize) + (100.0f * (RoomSize - 1));
}

void AMIR_CameraMovePoints::PrepareAppearance(const bool bUseBoundary, const FVector& NewExtent,
	const TArray<FVector>& BoundaryLocations)
{
	bUseBoundaryOnLerp = bUseBoundary;
	CameraMoveTrigger->SetBoxExtent(NewExtent);

	// Update the relative locations of each boundary accordingly.
	/* The current index of the for loop. */
	for (int32 Index = 0; Index < BoundaryArray.Num(); Index++)
	{
		if (!BoundaryArray[Index] || !BoundaryArray.IsValidIndex(Index)) { continue; }

		BoundaryArray[Index]->SetRelativeLocation(BoundaryLocations[Index]);
	}
}

/************************************************************************************
 * PANNING
 ***********************************************************************************/

void AMIR_CameraMovePoints::DetermineToPanBoundary(AMIR_PlayerChar* PlayerCharRef)
{
	// The boundary to be panned to; it is closest to the player.
	FVector ToPanBoundary = FVector::ZeroVector;

	CalculatePlayerBoundaryDistance(PlayerCharRef, ToPanBoundary);

	// The first found MIR_Camera instance in the World.
	AMIR_Camera* CameraRef =
		Cast<AMIR_Camera>(UGameplayStatics::GetActorOfClass(GetWorld(), AMIR_Camera::StaticClass()));

	if (!CameraRef) { return; }

	// Stop the camera from following the player (a separate action from panning).
	CameraRef->CloseFollowGate();

	// New max and min boundaries for the MIR_Camera instance to adhere to.
	TArray<FVector2D> NewBoundaries =
		{ FVector2D(Max_X->GetComponentLocation().X, Max_Y->GetComponentLocation().Y),
		  FVector2D(Min_X->GetComponentLocation().X, Min_Y->GetComponentLocation().Y) };

	// Make the camera pan to the room enveloped by this CMP.
	CameraRef->PanToNewRoomBoundary(ToPanBoundary, NewBoundaries, bUseBoundaryOnLerp,
									ERoomDeterminer, GetActorLocation());
}

void AMIR_CameraMovePoints::CalculatePlayerBoundaryDistance(AMIR_PlayerChar* PlayerCharRef,
	FVector& ToPanBoundary)
{
	// The boundary closest to the player.
	FVector ClosestBoundary = FVector::ZeroVector;

	// The previous-best distance between the player and a given boundary.
	// Must be set to an arbitrarily high float so that a newly determined
	// distance may overwrite it.
	float PreviousDistance = 5000.0f;

	// Determine the closest boundary.
	/* The relative location of the currently evaluated boundary in the loop. */
	for (const FVector& CurrentBoundaryLoc : BoundaryLocationsArray)
	{
		// The distance between the player and `CurrentBoundaryLoc`.
		float CurrentDistance = FVector::Dist(PlayerCharRef->GetActorLocation(),
											  CurrentBoundaryLoc);

		// Update record only if the current distance is less than the previous one
		// (because this means it's closer to the player).
		if (CurrentDistance < PreviousDistance)
		{
			ClosestBoundary = CurrentBoundaryLoc;
			PreviousDistance = CurrentDistance;
		}
	}

	// The boundary to pan to must be the closest one.
	ToPanBoundary = ClosestBoundary;
}
