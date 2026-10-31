// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "SMIR_RoomPrep.generated.h"

/**
 * Contains data with which a Camera Move Points (CMP) instance may update its variables.
 */
USTRUCT(meta = (Deprecated = "No longer used in MIR_CameraMovePoints."))
struct FRoomPrep
{
	GENERATED_BODY()

public:
	// Whether to use the boundary when lerping.
	bool BoundarySetting = false;

	// The box extent of the trigger.
	FVector BoxExtent = FVector::ZeroVector;

	// The local positions of each boundary.
	TArray<FVector> BoundaryLocalPos;

public:

	FRoomPrep() : FRoomPrep(false, FVector::ZeroVector, MakeZeroFilledVectorArray())
	{
	}

	FRoomPrep(bool bUseBoundary)
		: FRoomPrep(bUseBoundary, FVector::ZeroVector, MakeZeroFilledVectorArray())
	{
	}

	FRoomPrep(FVector NewBoxExtent)
		: FRoomPrep(false, NewBoxExtent, MakeZeroFilledVectorArray())
	{
	}

	FRoomPrep(TArray<FVector> BoundaryPos)
		: FRoomPrep(false, FVector::ZeroVector, BoundaryPos)
	{
	}

	FRoomPrep(bool bUseBoundary, FVector NewBoxExtent)
		: FRoomPrep(bUseBoundary, NewBoxExtent, MakeZeroFilledVectorArray())
	{
	}

	FRoomPrep(bool bUseBoundary, TArray<FVector> BoundaryPos)
		: FRoomPrep(bUseBoundary, FVector::ZeroVector, BoundaryPos)
	{
	}

	FRoomPrep(FVector NewBoxExtent, TArray<FVector> BoundaryPos)
		: FRoomPrep(false, NewBoxExtent, BoundaryPos)
	{
	}

	FRoomPrep(bool bUseBoundary, FVector NewBoxExtent, TArray<FVector> BoundaryPos)
	{
		BoundarySetting = bUseBoundary;
		BoxExtent = NewBoxExtent;
		BoundaryLocalPos = BoundaryPos;
	}

private:

	/**
	 * Makes an array of zero-filled vectors (eight indeces).
	 * 
	 * @returns ZeroFilledVectorArray
	 */
	TArray<FVector> MakeZeroFilledVectorArray()
	{
		// An array of zero-filled vectors.
		TArray<FVector> ZeroFilledVectorArray;

		// A loop index ranging from 0 to 7 (eight total indeces).
		for (int32 Index = 0; Index < 7; Index++)
		{
			ZeroFilledVectorArray.Add(FVector::ZeroVector);
		}

		return ZeroFilledVectorArray;
	}

};

/**
 *
 */
 //class MIRAANDTHEMIRRORSTAFF_API SMIR_RoomPrep
 //{
 //public:
 //	SMIR_RoomPrep();
 //	~SMIR_RoomPrep();
 //};
