// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IMIR_Copiable.h"
#include "Magic/MIR_MagicBASE.h"
#include "MIR_CopiableComponent.generated.h"

/**
 * A Component that allows a player character to copy magic from enemies.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_CopiableComponent : public UActorComponent, public IIMIR_Copiable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties.
	UMIR_CopiableComponent();

protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Sets new copied magic for player. 
	virtual void CopyMagic_Implementation(AMIR_PlayerChar* CurrentPlayer,
		TSubclassOf<class AMIR_MagicBASE> MagicToCopy) override;
};
