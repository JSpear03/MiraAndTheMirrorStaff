// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Enums/EMIR_RoomSizes.h"
#include "MIR_Camera.generated.h"

class AMIR_PlayerChar;

/**
 * A camera made specifically for dungeon rooms.
 * By default, it will follow the player within its alloted boundaries (x- and y- only).
 * However, if a player overlaps with a Camera Move Points (CMP) instance, the CMP
 * will pass relevant room-transition data to this actor, allowing it to pan to the
 * CMP. This effectively allows the camera to transition from room to room.
 */
UCLASS(Abstract)
class MIRAANDTHEMIRRORSTAFF_API AMIR_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMIR_Camera();

public:
	
	// The SpringArm Component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Spring Arm",
			  meta = (ToolTip = "The SpringArm Component."))
		TObjectPtr<USpringArmComponent> SpringArm;

	// The Camera Component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Camera",
			  meta = (ToolTip = "The Camera Component."))
		TObjectPtr<UCameraComponent> Camera;

	// The Timeline Component for the panning animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Timeline",
			  meta = (ToolTip = "The Timeline Component for the panning animation."))
		TObjectPtr<UTimelineComponent> PanningAnimTimeline;

protected:
	// A curve for `PanningAnimTimeline`.
	// If no curve is assigned in the Unreal editor, one will be made natively.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Timeline|Curve")
		TObjectPtr<UCurveFloat> PanningCurve = nullptr;

private:

	// Represents the world origin.
	const FVector2D WorldOrigin = { 0.0, 0.0 };

	// Represents the boundaries on the positive x- and y-axes.
	// Thus, once the player leaves the initial room, its values should be positive.
	FVector2D MaxPos = { -310.0, 0.0 };

	// Represents the boundaries on the negative x- and y-axes.
	// Thus, once the player leaves the initial room, its values should be negative.
	FVector2D MinPos = { -310.0, 0.0 };

	/**
	 * Whether the camera is allowed to pan on the x-axis while lerping during a room
	 * transition.
	 * - If true, it pans on the x-axis.
	 * - If false, it pans on the y-axis.
	 */
	bool bPanOnX = true;

	// Whether the Follow Gate is in the Open state.
	// If it's false, then said gate is in the Closed state.
	bool bFollowGateOpen = false;

	// The alpha to be fed into `PanningAnimTimeline`.
	FOnTimelineFloat PanningValue;

	// A function signature for the Complete event of `PanningAnimTimeline`.
	FOnTimelineEvent TimelineCompleteEvent;

private:
	// The boundary to target during the lerp.
	FVector TargetBoundary = FVector::ZeroVector;

	// New boundaries for `MaxPos` and `MinPos`.
	TArray<FVector2D> UpdatedBoundaries = { FVector2D::ZeroVector,
										    FVector2D::ZeroVector };

	// Whether to use the boundary during the lerp.
	bool bUseBoundaryDuringLerp = false;

	// The room size of the calling CMP.
	ERoomSizes CurrentRoomSize = ERoomSizes::OneByOne;

	// The world location of the calling CMP.
	FVector CMPWorldLoc = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the actor leaves play or is destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ===== GATE =====

	/**
	 * Enters the Follow Gate. Execution will continue only if `bFollowGateOpen` is true.
	 */
	void EnterFollowGate();

	/**
	 * Opens the Follow Gate by setting `bFollowGateOpen` to true.
	 */
	void OpenFollowGate();

	/**
	 * Closes the Follow Gate by setting `bFollowGateOpen` to false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Gate")
	void CloseFollowGate();

	/**
	 * Toggles the Follow Gate's state by setting `bFollowGateOpen` to its opposite value.
	 */
	void ToggleFollowGate();

protected:

	// ===== TIMELINE =====

	/**
	 * Called when `PanningAnimTimeline` updates.
	 */
	UFUNCTION()
		void PanningAnimUpdate(float Alpha);

	/**
	 * Called once `PanningAnimTimeline` is complete.
	 */
	UFUNCTION()
		void PanningAnimComplete();

private:

	// ===== CAMERA POSITIONING =====

	/**
	 * Gets all Camera Move Points (CMP) instances in the world, determines the one closest
	 * to the player, and then moves toward it.
	 * This facilitates the camera adapting to the player's initial spawn point, which may
	 * vary by location.
	 * 
	 * @param PlayerRef A reference to a player character.
	 */
	void SetStartingPos(AMIR_PlayerChar*& PlayerRef);

	/**
	 * Checks the player's current position, determining whether the camera should move
	 * on either axis.
	 */
	void CheckPlayerPos();

	/**
	 * Determines whether the player lies within both positive and negative boundaries
	 * on a given axis.
	 * 
	 * @param PlayerLocation A player's location on a specified axis.
	 * @param WorldOrigin The world origin on a specified axis.
	 * @param MinPos A minimum boundary on a specified axis.
	 * @param MaxPos A maximum boundary on a specified axis.
	 * 
	 * @returns Whether the player lies within said boundaries on a given axis.
	 */
	bool DeterminePosInBoundaries(const float PlayerLocation, const float InWorldOrigin,
								  const float InMinPos, const float InMaxPos) const;

	/**
	 * Moves the camera toward the player character on a specified axis.
	 * 
	 * Currently, movement on the z-axis (which may happen in either case) does not occur.
	 * 
	 * @param bIsX Whether to move on the x-axis (true) or y-axis (false).
	 * @param PlayerRef A reference to a character.
	 */
	void MoveCameraPos(bool bIsX, ACharacter*& PlayerRef);

public:
	// ===== CAMERA PANNING =====

	/**
	 * After a CMP has calculated a target boundary, this function will use it to
	 * execute panning.
	 * 
	 * @param ToPanBoundary The boundary to pan to.
	 * @param NewBoundaries New boundaries for `MaxPos` and `MinPos`.
	 * @param bUseBoundaryOnLerp Whether the use the boundary when lerping.
	 * @param RoomSize The room size used in the CMP.
	 * @param CMPWorldPos The world location of the CMP which called this function.
	 */
	void PanToNewRoomBoundary(const FVector& ToPanBoundary,
							  const TArray<FVector2D>& NewBoundaries,
							  const bool bUseBoundaryOnLerp, const ERoomSizes RoomSize,
							  const FVector& CMPWorldPos);

private:
	/**
	 * Determines the preferred panning axis (x- or y-) based on the difference (absolute
	 * value) of the player's world location and the target boundaries.
	 * 
	 * If the player is more distant on the x-axis than the y-axis, the camera will pan
	 * on the x-axis; otherwise, the y-axis.
	 * 
	 * @param TargetBoundaryX The target boundary's x-axis location in world space.
	 * @param TargetBoundaryY The target boundary's y-axis location in world space.
	 */
	void DeterminePanningAxis(const float TargetBoundaryX, const float TargetBoundaryY);

	/**
	 * Calculates the values needed to pan on each axis during a room transition.
	 * This function is crucial to allowing the camera to pan freely on a given axis
	 * based on the primary axis determined.
	 * 
	 * For example, if the player is entering a room on the y-axis, the camera will be
	 * allowed to pan freely on the x-axis even during the transition (within the
	 * determined boundaries).
	 * 
	 * @param Alpha An alpha generated by a Timeline Component.
	 * @param ToPanBoundaryX The target boundary's x-axis location in world space.
	 * @param ToPanBoundaryY The target boundary's y-axis location in world space.
	 * @param bUseBoundaryOnLerp Whether to use the boundary when lerping.
	 * @param RoomSize The room size of the CMP.
	 * @param CMPWorldPos The world location of the CMP.
	 * 
	 * @returns The new world location for the actor.
	 */
	FVector CalculatePanningAxes(const float Alpha, const float ToPanBoundaryX,
								 const float ToPanBoundaryY, const bool bUseBoundaryOnLerp,
								 const ERoomSizes RoomSize, const FVector& CMPWorldPos);

	/**
	 * Handles lerping on the x-axis with a determined target y-axis.
	 * 
	 * In generic scenarios without evaluating the room size, this is what allows
	 * the camera to pan freely on the y-axis while transitioning on the x-.
	 * 
	 * @param Alpha An alpha generated by a Timeline Component.
	 * @param BoundaryX The target boundary's x-axis location in world space.
	 * @param BoundaryY The target boundary's y-axis location in world space.
	 * @param bUseBoundary Whether to use the boundary when lerping.
	 * 
	 * @returns Coordinates to use for x-axis lerping where applicable.
	 */
	FVector XAxisLerping(const float Alpha, const float BoundaryX, const float BoundaryY,
						 const bool bUseBoundary);

	/**
	 * Determines how to restrict panning on the y-axis based on the following criteria
	 * (in chronological order):
	 * 
	 * 1. A given boundary's location relative to its CMP reference.
	 * 
	 * 2. The player's location relative to the given boundary.
	 * 
	 * @param ToPanBoundaryY The target boundary's y-axis location in world space.
	 * @param RoomSize The room size of the CMP.
	 * @param CMPWorldPos The world location of the CMP.
	 * 
	 * @returns A flag determining whether that coordinate should be used for said lerping.
	 */
	bool HandlePanningRestrictionY(const float ToPanBoundaryY, const ERoomSizes RoomSize,
								   const FVector& CMPWorldPos);

	/**
	 * Handles lerping on the y-axis with a determined target x-axis.
	 *
	 * In generic scenarios without evaluating the room size, this is what allows
	 * the camera to pan freely on the x-axis while transitioning on the y-.
	 *
	 * @param Alpha An alpha generated by a Timeline Component.
	 * @param BoundaryX The target boundary's x-axis location in world space.
	 * @param BoundaryY The target boundary's y-axis location in world space.
	 * @param bUseBoundary Whether to use the boundary when lerping.
	 *
	 * @returns Coordinates to use for y-axis lerping where applicable.
	 */
	FVector YAxisLerping(const float Alpha, const float BoundaryX, const float BoundaryY,
						 const bool bUseBoundary);

	/**
	 * Determines how to restrict panning on the x-axis based on the following criteria
	 * (in chronological order):
	 *
	 * 1. A given boundary's location relative to its CMP reference.
	 *
	 * 2. The player's location relative to the given boundary.
	 *
	 * @param ToPanBoundaryX The target boundary's x-axis location in world space.
	 * @param RoomSize The room size of the CMP.
	 * @param CMPWorldPos The world location of the CMP.
	 *
	 * @returns A data container with the following:
	 *
	 * 1. An ideal x-axis coordinate for lerping.
	 *
	 * 2. A flag determining whether that coordinate should be used for said lerping.
	 */
	TTuple<float, bool> HandlePanningRestrictionX(const float ToPanBoundaryX,
		const ERoomSizes RoomSize, const FVector& CMPWorldPos);

	/**
	 * Moves the camera toward the player's respawn point by redetermining the closest
	 * boundary.
	 * Called when `OnRespawn` is dispatched by MIR_PlayerChar.
	 */
	UFUNCTION()
		void MoveTowardRespawnPoint();

	// ===== HELPERS =====

	/**
	 * Gets the first MIR_PlayerChar in the World.
	 * 
	 * @returns A player reference.
	 */
	AMIR_PlayerChar* GetMIRPlayerChar();

	/**
	 * Gets the world location of the first character in the world.
	 * 
	 * @returns The character's world location.
	 */
	FVector GetPlayerWorldLocation();

	/**
	 * Determines whether an ideal coordinate should be used.
	 * 
	 * @param RoomSize The room size of the CMP.
	 * 
	 * @returns Whether the ideal coordinate should be used.
	 * 
	 * @see HandlePanningRestrictionY()
	 * @see HandlePanningRestructionX()
	 */
	bool DetermineIdealCoord(ERoomSizes RoomSize);

	/**
	 * Sets up `PanningAnimTimeline`.
	 */
	void SetUpTimeline();

	/**
	 * Sets up `PanningCurve` if one is not assigned in the Unreal editor.
	 */
	void SetUpCurve();
};
