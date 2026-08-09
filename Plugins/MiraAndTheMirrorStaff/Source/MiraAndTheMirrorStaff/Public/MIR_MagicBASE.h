// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MIR_MagicBASE.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MagicBASE : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Magic|Mesh")
        USkeletalMeshComponent* Mesh;

public:
    // Sets default values for this actor's properties
    AMIR_MagicBASE();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	// Creates the Attack Type Component for the spell.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Components")
	    class UMIR_AttackTypeComponent* AttackTypeComponent;

    // Creates the Element Component for the spell.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic|Components")
	    class UMIR_ElementComponent* ElementComponent;

	// Creates the overridable Collision Component for the spell.
	UPROPERTY(EditAnywhere, Category = "Magic|Collision")
        class USphereComponent* CollisionComponent;

    // Creates the overridable Collision Component for the ice freeze hitbox on spells.
    UPROPERTY(EditAnywhere, Category = "Magic|Collision")
	    class USphereComponent* IceCollisionComponent;
};