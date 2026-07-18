// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_MagicMelee.h"
#include "MIR_AttackTypeComponent.h"
#include "MIR_ElementComponent.h"
#include "Components/SphereComponent.h"

AMIR_MagicMelee::AMIR_MagicMelee()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set the init radius to something small for the melee spell as a placeholder
    // For actual implementation, it might be best to use a capsule attached to the end of Mira's staff/dagger
	// and have the collision component follow the staff's tip for melee attacks.
    CollisionComponent->InitSphereRadius(5.0f);

    // TODO: Use AttackTypeComponent to add the Melee struct object with the new method. 

}