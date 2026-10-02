// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Enums/EMIR_RoomSizes.h"
//#include "Structs/SMIR_RoomPrep.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "MIR_CameraMovePoints.generated.h"

/**
 * A Camera Move Points (CMP) actor specifies eight boundaries for a
 * Camera actor to pan to. When a player character overlaps with it,
 * it will determine the closest boundary to the player via world space.
 * This closest boundary is then passed to the Camera actor, which
 * handles the panning.
 * 
 * Moreover, the CMP supports dynamic sizing via its `ERoomDeterminer`
 * Enum. In the Unreal editor, changing the value of the enum will
 * update the sizing of the overlap trigger and the positions of the
 * boundaries in relative space.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_CameraMovePoints : public AActor
{
	GENERATED_BODY()

private:
	// The base extent of a room on the x-axis.
	static const float BaseRoomExtentX;

	// The base extent of a room on the y-axis.
	static const float BaseRoomExtentY;

	// The base extent of a room on the z-axis.
	static const float BaseRoomExtentZ;
	
public:	
	// Sets default values for this actor's properties
	AMIR_CameraMovePoints();

// LEAVE UNCOMMENTED; WILL CRASH EDITOR IF EXECUTED IN A CHILD BLUEPRINT.
//#if WITH_EDITOR
//	/**
//	 * Called when a property is changed in the editor.
// 	 *
// 	 * CAUTION: WILL CRASH EDITOR IF EXECUTED IN A CHILD BLUEPRINT.
//	 *
//	 * @param PropertyChangedEvent Contains information about the changed property.
//	 */
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The box trigger which causes the room transition.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Trigger")
		TObjectPtr<UBoxComponent> CameraMoveTrigger;

	// Occupies the north position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Max_X;

	// Occupies the south position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Min_X;

	// Occupies the east position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Max_Y;

	// Occupies the west position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Min_Y;

	// Occupies the northeast position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Northeast;

	// Occupies the southeast position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Southeast;

	// Occupies the southwest position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Southwest;

	// Occupies the northwest position in the CameraMoveTrigger.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Boundaries")
		TObjectPtr<UBillboardComponent> Northwest;
		
	// Contains all eight Billboard Components.
	// Should be initialized in the order of Max_X, Min_X, Max_Y, Min_Y,
	// Northeast, Southeast, Southwest, Northwest.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boundaries")
		TArray<TObjectPtr<UBillboardComponent>> BoundaryArray;

	// Contains the world locations of all eight Billboard Components.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boundaries")
		TArray<FVector> BoundaryLocationsArray;

	/**
	 * Whether to use a boundary when lerping.
	 * Should be true for only 1x1 rooms because of their minimal size.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boundaries")
		bool bUseBoundaryOnLerp = true;

	// Determines room size, thereby dictating transition target and trigger extent.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Construction",
			  meta = (DisplayName = "Room Determiner"))
		ERoomSizes ERoomDeterminer = ERoomSizes::OneByOne;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// ===== OVERLAP =====

	/**
	 * Called when an actor overlaps with `CameraMoveTrigger`.
	 * 
	 * @param OtherActor That actor that initiated the overlap.
	 */
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Constructs the `BoundaryArray` with all eight boundaries.
	void ConstructBoundaryArray();

	/**
	 * Constructs the `BoundaryLocationsArray` with the world locations of
	 * all eight boundaries.
	 * 
	 * Usage of world locations is crucial because the Camera actor must pan
	 * to a world location rather than a relative one.
	 */
	void ConstructBoundaryLocationsArray();

private:

	// ===== CONSTRUCTION SCRIPT =====

	/**
	 * Constructs the appearance of the CMP such that it matches the extent
	 * and specifications of an in-game room, which may be of variable size.
	 */
	UFUNCTION(BlueprintCallable, Category = "Construction", meta = (KeyWords = "Constructor"))
		void RoomConstructor();

	/**
	 * Determines the room dimentions to pass to `MakeRoomStruct()`.
	 * 
	 * @see `MakeRoomStruct()`
	 * @see `PrepareAppearance()`.
	 */
	void DetermineAppearance();

	/**
	 * Determines and calculates data related to room setups (i.e., boundary usage,
	 * box extent, and the relative locations of all eight Billboard Components).
	 * It then passes this data to `PrepareAppearance()`.
	 * 
	 * @param RoomSizeX The size of the room on the x-axis.
	 * @param RoomSizeY The size of the room on the y-axis.
	 */
	void MakeRoomStruct(int32 RoomSizeX, int32 RoomSizeY);

	/**
	 * Determines and calculates data related to room setups (i.e., boundary usage,
	 * box extent, and the relative locations of all eight Billboard Components).
	 * It then passes this data to `PrepareAppearance()`.
	 *
	 * @param RoomSizeXY The size of the room on the xy-axes.
	 */
	void MakeRoomStruct(int32 RoomSizeXY = 1);

	/**
	 * Calculates the size of a room on a given axis, adjusted for scale
	 * as dictated by `ERoomDeterminer`.
	 * 
	 * @param RoomExtent The base extent of a room on a given axis.
	 * @param RoomSize The scale to apply to said room.
	 * 
	 * @returns A scale-adjusted box extent (intended for `CameraMoveTrigger`).
	 */
	float CalculateBoxExtentOnAxis(const float BaseRoomExtent, const float RoomSize);

	/**
	 * Handles room setup based on the ERoomDeterminer Enum:
	 * 
	 * - Sets `bUseBoundaryOnLerp`, which represents whether the use the boundary
	 * when lerping during a room transition.
	 * 
	 * - Sets the BoxExtent of the CameraMoveTrigger.
	 * 
	 * - Sets the relative positions of the Billboard Components.
	 * 
	 * @param bUseBoundary A Bool to be assigned to `bUseBoundaryOnLerp`.
	 * @param NewExtent A vector to be passed to the box extent of `CameraMoveTrigger`.
	 * @param BoundaryLocations An array of vectors to be accessed when setting the
	 * relative locations of each boundary.
	 * 
	 * @see `DetermineAppearance()`
	 */
	void PrepareAppearance(const bool bUseBoundary, const FVector& NewExtent,
		const TArray<FVector>& BoundaryLocations);

public:

	// ===== PANNING =====

	/**
	 * First determines the boundary closest to the player, and then passes
	 * the necessary values to the Camera based on the result.
	 * 
	 * @param PlayerCharRef A reference to the current player character.
	 */
	UFUNCTION(BlueprintCallable, Category = "Panning", meta = (KeyWords = "Pan Boundary"))
		void DetermineToPanBoundary(AMIR_PlayerChar* PlayerCharRef);

	/**
	 * Calculates the distance between the player and a given boundary, and then
	 * determines whether this distance is less than the previous distance (PreviousDistance).
	 * 
	 * The result of this logic is then assigned to the function's vector parameter.
	 * 
	 * @param PlayerCharRef A reference to the current player character.
	 * @param ToPanBoundary The boundary to pan to. Will be assigned with the result of
	 * the function's interior logic (NOT via a return statement).
	 */
	void CalculatePlayerBoundaryDistance(AMIR_PlayerChar* PlayerCharRef, FVector& ToPanBoundary);
};
