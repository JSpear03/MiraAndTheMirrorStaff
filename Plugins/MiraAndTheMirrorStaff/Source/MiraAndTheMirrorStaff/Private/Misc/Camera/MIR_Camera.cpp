// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/Camera/MIR_Camera.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMath.h"
#include "Misc/Camera/MIR_CameraMovePoints.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values
AMIR_Camera::AMIR_Camera()
{
 	// Set this actor to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create default subobjects.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PanningAnimTimeline =
		CreateDefaultSubobject<UTimelineComponent>(TEXT("PanningAnimTimeline"));

	// Parent subobjects appropriately.
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	// Configure display.
	SpringArm->SocketOffset = FVector(-97.0, 0.0, -294.0);
	Camera->AspectRatio = 16.0f / 9.0f;
	Camera->bConstrainAspectRatio = true;
}

// Called when the game starts or when spawned
void AMIR_Camera::BeginPlay()
{
	Super::BeginPlay();

	// The first currently possessed player character.
	AMIR_PlayerChar* PlayerChar =
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerChar) { return; }

	SetStartingPos(PlayerChar);

	// Camera must be able to move toward the player's respawn point.
	PlayerChar->OnRespawn.AddUniqueDynamic(this, &AMIR_Camera::MoveTowardRespawnPoint);

	// The first player controller in the World.
	APlayerController* PlayerCont = (GetWorld()->GetFirstPlayerController());

	if (!PlayerCont) { return; }

	PlayerCont->SetViewTargetWithBlend(this);

	SetUpTimeline();

	OpenFollowGate();
}

// Called when the actor leaves play or is destroyed.
void AMIR_Camera::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// The first currently possessed player character.
	AMIR_PlayerChar* PlayerChar = GetMIRPlayerChar();

	if (PlayerChar)
	{
		PlayerChar->OnRespawn.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMIR_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnterFollowGate();
}

/************************************************************************************
 * GATE
 ***********************************************************************************/

void AMIR_Camera::EnterFollowGate()
{
	if (bFollowGateOpen)
	{
		CheckPlayerPos();
	}
}

void AMIR_Camera::OpenFollowGate()
{
	bFollowGateOpen = true;
}

void AMIR_Camera::CloseFollowGate()
{
	bFollowGateOpen = false;
}

void AMIR_Camera::ToggleFollowGate()
{
	bFollowGateOpen = !bFollowGateOpen;
}

/************************************************************************************
 * TIMELINE
 ***********************************************************************************/

void AMIR_Camera::PanningAnimUpdate(float Alpha)
{
	// The world location to pan to.
	FVector PanningLocation =
		CalculatePanningAxes(Alpha, TargetBoundary.X, TargetBoundary.Y,
			bUseBoundaryDuringLerp, CurrentRoomSize, CMPWorldLoc);

	SetActorLocation(PanningLocation);
}

void AMIR_Camera::PanningAnimComplete()
{
	MaxPos = UpdatedBoundaries[0];
	MinPos = UpdatedBoundaries[1];
	bPanOnX = true;

	OpenFollowGate();
}

/************************************************************************************
 * CAMERA POSITIONING
 ***********************************************************************************/

void AMIR_Camera::SetStartingPos(AMIR_PlayerChar*& PlayerRef)
{
	// An array of all CMPs in the current World.
	TArray<AActor*> ExistingCMPArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMIR_CameraMovePoints::StaticClass(),
										  ExistingCMPArray);

	// Don't continue if there are no CMP instances.
	if (ExistingCMPArray.Num() == 0) { return; }

	// The CMP closest to the player.
	FVector NearestLocation = FVector::ZeroVector;
	AMIR_CameraMovePoints* NearestCMP = nullptr;

	// The previous-best distance between the player and a given CMP.
	// Must be set to an arbitrarily high float so that a newly determined
	// distance may overwrite it.
	float NearestDistance = 100000.0f;

	// Determine the closest CMP.
	/* The currently evaluated CMP in the loop. */
	for (AActor* CurrentActor : ExistingCMPArray)
	{
		AMIR_CameraMovePoints* CurrentCMP = Cast<AMIR_CameraMovePoints>(CurrentActor);
		if (!CurrentCMP) { continue; }

		// The distance between the player and `CurrentCMP`.
		float CurrentDistance =
			FVector::Dist(PlayerRef->GetActorLocation(), CurrentCMP->GetActorLocation());

		// Update record only if the current distance is less than the previous one
		// (because this means it's closer to the player).
		if (CurrentDistance < NearestDistance)
		{
			NearestCMP = CurrentCMP;
			NearestLocation = CurrentCMP->GetActorLocation();
			NearestDistance = CurrentDistance;
		}
	}

	// Update location with calculated values, effectively bringing the camera to the player.
	SetActorLocation(FVector(NearestLocation.X, NearestLocation.Y, GetActorLocation().Z));

	if (NearestCMP)
	{
		PlayerRef->CurrentCMP = NearestCMP;
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMIR_SaveSubsystem* SaveSubsystem = GI->GetSubsystem<UMIR_SaveSubsystem>())
			{
				const FName RoomID = NearestCMP->GetCleanRoomID();
				if (!RoomID.IsNone())
				{
					SaveSubsystem->MarkRoomVisited(RoomID);
				}
			}
		}
	}
}

void AMIR_Camera::CheckPlayerPos()
{
	// The first currently possessed character.
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!PlayerChar) { return; }

	// Whether the camera should move on the x-axis with respect to the player's location.
	bool MoveOnX = DeterminePosInBoundaries(PlayerChar->GetActorLocation().X, WorldOrigin.X,
											MinPos.X, MaxPos.X);

	if (MoveOnX)
	{
		MoveCameraPos(true, PlayerChar);
	}

	// Whether the camera should move on the y-axis with respect to the player's location.
	bool MoveOnY = DeterminePosInBoundaries(PlayerChar->GetActorLocation().Y, WorldOrigin.Y,
											MinPos.Y, MaxPos.Y);

	if (MoveOnY)
	{
		MoveCameraPos(false, PlayerChar);
	}

}

bool AMIR_Camera::DeterminePosInBoundaries(const float PlayerLocation,
	const float InWorldOrigin, const float InMinPos, const float InMaxPos) const
{
	if (PlayerLocation > (InWorldOrigin + InMinPos) &&
		PlayerLocation < (InWorldOrigin + InMaxPos))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AMIR_Camera::MoveCameraPos(bool bIsX, ACharacter*& PlayerRef)
{
	if (!PlayerRef) { return; }

	// A target location for the vinterp call.
	FVector WorkingVector = FVector::ZeroVector;

	if (bIsX)
	{
		WorkingVector =
			{ PlayerRef->GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z };
	}
	else
	{
		WorkingVector =
			{ GetActorLocation().X, PlayerRef->GetActorLocation().Y, GetActorLocation().Z };
	}

	// Contains coordinates that will be passed to the actor's world location.
	FVector NewCameraLoc = 
		FMath::VInterpTo(GetActorLocation(), WorkingVector,
						 UGameplayStatics::GetWorldDeltaSeconds(this), 0.0);

	SetActorLocation(NewCameraLoc);

}

/************************************************************************************
 * CAMERA PANNING
 ***********************************************************************************/

void AMIR_Camera::PanToNewRoomBoundary(const FVector& ToPanBoundary,
	const TArray<FVector2D>& NewBoundaries, const bool bUseBoundaryOnLerp,
	const ERoomSizes RoomSize, const FVector& CMPWorldPos)
{
	DeterminePanningAxis(ToPanBoundary.X, ToPanBoundary.Y);

	// TODO: Remove applicable function parameters from other functions
	// due to these new member variables; allocating memory for such
	// parameters is wasteful when they can already be accessed globally.
	TargetBoundary = ToPanBoundary;
	UpdatedBoundaries = NewBoundaries;
	bUseBoundaryDuringLerp = bUseBoundaryOnLerp;
	CurrentRoomSize = RoomSize;
	CMPWorldLoc = CMPWorldPos;

	PanningAnimTimeline->PlayFromStart();
}

void AMIR_Camera::DeterminePanningAxis(const float TargetBoundaryX,
	const float TargetBoundaryY)
{
	// The world location of the Camera component.
	FVector CameraWorldPos = Camera->GetComponentLocation();

	// Determine whether the player is more distant on the x-axis than the y-axis.
	if (FMath::Abs(CameraWorldPos.X - TargetBoundaryX) >
		FMath::Abs(CameraWorldPos.Y - TargetBoundaryY))
	{
		return;
	}
	else
	{
		// The player is more distant on the y-, so allow panning on the y-axis only.
		bPanOnX = false;
	}
}

FVector AMIR_Camera::CalculatePanningAxes(const float Alpha, const float ToPanBoundaryX,
	const float ToPanBoundaryY, const bool bUseBoundaryOnLerp, const ERoomSizes RoomSize,
	const FVector& CMPWorldPos)
{
	if (bPanOnX)
	{
		// Contains data related to possible lerping restrictions on the y-axis.
		bool YRestriction = HandlePanningRestrictionY(ToPanBoundaryY, RoomSize, CMPWorldPos);

		// XYZ coords to use for x-axis lerping where applicable.
		FVector XLerp = XAxisLerping(Alpha, ToPanBoundaryX, ToPanBoundaryY, bUseBoundaryOnLerp);

		if (YRestriction)
		{
			return FVector(XLerp.X, ToPanBoundaryY, XLerp.Z);
		}
		else
		{
			return XLerp;
		}
	}
	else
	{
		// Contains data related to possible lerping restrictions on the x-axis.
		TTuple<float, bool> XRestriction =
			HandlePanningRestrictionX(ToPanBoundaryX, RoomSize, CMPWorldPos);

		// Switch on RoomSize to determine Bool to pass to the function below.
		bool bUseBoundary = false;

		switch (RoomSize)
		{
		case ERoomSizes::OneByOne:
			bUseBoundary = bUseBoundaryOnLerp;
			break;
		default:
			bUseBoundary = !bUseBoundaryOnLerp;
			break;
		}

		// XYZ coords to use for y-axis lerping where applicable.
		FVector YLerp = YAxisLerping(Alpha, ToPanBoundaryX, ToPanBoundaryY, bUseBoundary);

		if (XRestriction.Get<1>())
		{
			return FVector(XRestriction.Get<0>(), YLerp.Y, YLerp.Z);
		}
		else
		{
			return YLerp;
		}
	}
}

FVector AMIR_Camera::XAxisLerping(const float Alpha, const float BoundaryX,
	const float BoundaryY, const bool bUseBoundary)
{
	// The x-axis lerp target.
	float TargetX = FMath::Lerp(GetActorLocation().X, BoundaryX, Alpha);

	// The y-axis lerp target.
	float TargetY = bUseBoundary ? BoundaryY : GetPlayerWorldLocation().Y;
	
	return FVector(TargetX, TargetY, GetActorLocation().Z);
}

bool AMIR_Camera::HandlePanningRestrictionY(const float ToPanBoundaryY,
	const ERoomSizes RoomSize, const FVector& CMPWorldPos)
{
	switch (RoomSize)
	{
	case ERoomSizes::OneByOne:
		// Exit because 1x1 rooms do not need special panning.
		return false;
		break;

	default:

		// If true, the to-pan boundary is on the left.
		if (ToPanBoundaryY < CMPWorldPos.Y)
		{
			// Represents whether the player is to the left of the boundary.
			// If the condition is false, the above is true and `ToPanBoundaryY`
			// shouldn't be used.
			bool bUseIdealY = ToPanBoundaryY < GetPlayerWorldLocation().Y ? false : true;

			return bUseIdealY;

			//if (ToPanBoundaryY < GetPlayerWorldLocation().Y)
			//{
			//	// Player is to the left of `ToPanBoundaryY`.
			//	return TTuple<float, bool>(ToPanBoundaryY, false);
			//	break;
			//}
			//else
			//{
			//	// Player is not to the left of `ToPanBoundaryY`.
			//	return TTuple<float, bool>(ToPanBoundaryY, true);
			//	break;
			//}
		}
		else
		{
			if (ToPanBoundaryY > GetPlayerWorldLocation().Y)
			{
				// Player is not to the right of the boundary.

				// Represents whether the player is to the right of the boundary.
				// If its value is false, the above is true and `ToPanBoundaryY`
				// shouldn't be used.
				bool bUseIdealY = DetermineIdealCoord(RoomSize);

				return bUseIdealY;
			}
			else
			{
				// Player is to the right of the boundary.
				return true;
			}
		}
	}
}

FVector AMIR_Camera::YAxisLerping(const float Alpha, const float BoundaryX,
	const float BoundaryY, const bool bUseBoundary)
{
	// The x-axis lerp target.
	float TargetX = bUseBoundary ? BoundaryX : GetPlayerWorldLocation().X;

	// The y-axis lerp target.
	float TargetY = FMath::Lerp(GetActorLocation().Y, BoundaryY, Alpha);

	return FVector(TargetX, TargetY, GetActorLocation().Z);
}

TTuple<float, bool> AMIR_Camera::HandlePanningRestrictionX(const float ToPanBoundaryX,
	const ERoomSizes RoomSize, const FVector& CMPWorldPos)
{
	switch (RoomSize)
	{
	case ERoomSizes::OneByOne:
		// Exit because 1x1 rooms do not need special panning.
		return TTuple<float, bool>(0.0, false);
		break;

	default:

		// If true, the to-pan boundary is to the south.
		if (ToPanBoundaryX < CMPWorldPos.X)
		{
			if (ToPanBoundaryX < GetPlayerWorldLocation().X)
			{
				// Player is to the south of the boundary.

				// Represents whether the player is to the south of the boundary.
				// If its value is false, the above is true and `ToPanBoundaryX`
				// shouldn't be used.
				bool bUseIdealX = DetermineIdealCoord(RoomSize);

				return TTuple<float, bool>(GetPlayerWorldLocation().X, bUseIdealX);
			}
			else
			{
				// Player is not to the south of the boundary.
				return TTuple<float, bool>(ToPanBoundaryX, true);
			}
		}
		else
		{
			if (ToPanBoundaryX > GetPlayerWorldLocation().X)
			{
				// Player is to the north of the boundary.

				// Represents whether the player is to the north of the boundary.
				// If its value is false, the above is true and `ToPanBoundaryX`
				// shouldn't be used.
				bool bUseIdealX = DetermineIdealCoord(RoomSize);

				return TTuple<float, bool>(GetPlayerWorldLocation().X, bUseIdealX);
			}
			else
			{
				// Player is not to the north of the boundary.
				return TTuple<float, bool>(ToPanBoundaryX, false);
			}
		}
	}
}

void AMIR_Camera::MoveTowardRespawnPoint()
{
	// The first currently possessed player character.
	AMIR_PlayerChar* PlayerChar = GetMIRPlayerChar();

	if (!PlayerChar || !PlayerChar->CurrentCMP) { return; }

	PlayerChar->CurrentCMP->DetermineToPanBoundary(PlayerChar);
}

/************************************************************************************
 * HELPERS
 ***********************************************************************************/

AMIR_PlayerChar* AMIR_Camera::GetMIRPlayerChar()
{
	// The first currently possessed player character in the World.
	AMIR_PlayerChar* PlayerChar =
		Cast<AMIR_PlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerChar) { return nullptr; }
	else { return PlayerChar; }
}

FVector AMIR_Camera::GetPlayerWorldLocation()
{
	// The first currently possessed character in the World.
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!PlayerChar) { return FVector::ZeroVector; }
	else { return PlayerChar->GetActorLocation(); }
}

bool AMIR_Camera::DetermineIdealCoord(ERoomSizes RoomSize)
{
	switch (RoomSize)
	{
	case ERoomSizes::OneByOne:
		return false;
		break;
	case ERoomSizes::OneByTwo:
		return true;
		break;
	case ERoomSizes::TwoByOne:
		return false;
		break;
	case ERoomSizes::TwoByTwo:
		return true;
		break;
	case ERoomSizes::ThreeByThree:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void AMIR_Camera::SetUpTimeline()
{
	// Bind function signatures.
	PanningValue.BindUFunction(this, FName("PanningAnimUpdate"));
	TimelineCompleteEvent.BindUFunction(this, FName("PanningAnimComplete"));

	// Set up `PanningCurve`.
	SetUpCurve();

	// Link timeline events.
	PanningAnimTimeline->AddInterpFloat(PanningCurve, PanningValue);
	PanningAnimTimeline->SetTimelineFinishedFunc(TimelineCompleteEvent);
}

void AMIR_Camera::SetUpCurve()
{
	// If a curve is already assigned in the editor, don't continue;
	// the following code sets up a native curve.
	if (PanningCurve) { return; }

	// Set up `PanningCurve`.
	PanningCurve = NewObject<UCurveFloat>();

	// Add the two keys.
	PanningCurve->FloatCurve.AddKey(0.0f, 0.0f);  // Time = 0;   Value = 0
	PanningCurve->FloatCurve.AddKey(0.5f, 1.0f);  // Time = 0.5; Value = 1

	// Set the interpolation mode on said keys.
	/* The current key in `PanningCurve`. */
	for (FRichCurveKey& Key : PanningCurve->FloatCurve.Keys)
	{
		Key.InterpMode = RCIM_Linear;
	}
}
