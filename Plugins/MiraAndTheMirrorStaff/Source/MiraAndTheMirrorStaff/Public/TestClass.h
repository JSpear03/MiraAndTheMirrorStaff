// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcadeFramework/Public/ArcadeGameMode.h"
#include "TestClass.generated.h"

/**
 * 
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API ATestClass : public AArcadeGameMode
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	ATestClass();
};
