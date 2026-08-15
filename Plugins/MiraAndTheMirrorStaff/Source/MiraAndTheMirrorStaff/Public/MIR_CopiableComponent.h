// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IMIR_Copiable.h"
#include "MIR_MagicBASE.h"
#include "MIR_CopiableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_CopiableComponent : public UActorComponent, public IIMIR_Copiable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMIR_CopiableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Sets new copied magic for player. 
	virtual void CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer, TSubclassOf<class AMIR_MagicBASE> MagicToCopy) override;
};
