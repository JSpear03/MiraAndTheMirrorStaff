// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/SMIR_ElementData.h"
#include "MIR_ElementComponent.generated.h"

/**
 * Handles Element details for an actor, such as magic or a character.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ElementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UMIR_ElementComponent();

	// Default Structs (deprecated, but keep in case we make them static)
	//const FDefaultElementStruct DefaultElementStruct;  // Default struct for Elements.
	//const FNothingStruct NothingStruct;				   // Default struct for Nothing.
	//const FFlameStruct FlameStruct;					   // Default struct for Flame.
	//const FIceStruct IceStruct;						   // Default struct for Ice.
	//const FThunderStruct ThunderStruct;				   // Default struct for Thunder.

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The current Element of this magic. Set this value to dictate the working struct.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element Details",
			  meta = (KeyWords = "Current"))
		EElements CurrentElement = EElements::None;

	// Determines working data for the specified Element (via enum called `CurrentElement`).
	// Its contents will not update until BeginPlay() is called.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element Details",
			  meta = (KeyWords = "Struct"))
		FDefaultElementStruct ElementStruct;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the `AttackTypeName` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details", meta = (KeyWords = "Get Name",
			  ReturnDisplayName = "Name", ToolTip = "Returns the `ElementName` member variable."))
		EElements GetElementName() const { return (ElementStruct.ElementName != EElements::None) ? ElementStruct.ElementName : CurrentElement; }

	// Returns the `CurrentElement` member variable.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Element Details", meta = (KeyWords = "Get Current Element",
			  ReturnDisplayName = "CurrentElement", ToolTip = "Returns the `CurrentElement` member variable."))
		EElements GetCurrentElement() const { return CurrentElement; }

	// Returns the `MainColor` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details|Appearance", meta = (KeyWords = "Get Main Color",
			  ReturnDisplayName = "MainColor", ToolTip = "Returns the `MainColor` member variable."))
		const FLinearColor GetMainColor() const { return ElementStruct.MainColor; }

	// Returns the `SubColor` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details|Appearance", meta = (KeyWords = "Get Sub Color",
			  ReturnDisplayName = "SubColor", ToolTip = "Returns the `SubColor` member variable."))
		const FLinearColor GetSubColor() const { return ElementStruct.SubColor; }

	// Returns the `ElementStrength` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details|Matchups", meta = (KeyWords = "Get Strength",
			  ReturnDisplayName = "Strength", ToolTip = "Returns the `ElementStrength` member variable."))
		EElements GetElementStrength() const { return ElementStruct.ElementStrength; }

	// Returns the `ElementWeakness` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details|Matchups", meta = (KeyWords = "Get Weakness",
			  ReturnDisplayName = "Weakness", ToolTip = "Returns the `ElementWeakness` member variable."))
		EElements GetElementWeakness() const { return ElementStruct.ElementWeakness; }

	// Returns the `ElementNeutralities` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details|Matchups", meta = (KeyWords = "Get Neutralities",
			  ReturnDisplayName = "Neutralities", ToolTip = "Returns the `ElementNeutralities` member variable."))
		TArray<EElements> GetElementNeutralities() const { return ElementStruct.ElementNeutralities; }

	// Returns the `ElementProperty` member variable.
	UFUNCTION(BlueprintCallable, Category = "Element Details", meta = (KeyWords = "Get Property",
			  ReturnDisplayName = "Property", ToolTip = "Returns the `ElementProperty` member variable."))
		EElementProperties GetElementProperty() const { return ElementStruct.ElementProperty; }

	/**
	 * Passes the Enum parameter to a switch statement, which checks
	 * each case to then set the `ElementStruct` appropriately.
	 *
	 * @param ENewElement A specifier to determine the new struct for the `ElementStruct`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Element Details", meta = (KeyWords = "Set Element Details"))
		void SetElementDetails(EElements ENewElement = EElements::None);

	/**
	 * Sets the `CurrentElement` member variable.
	 * 
	 * @param ENewElement The value to be assigned to `CurrentElement`.
	 */
	void SetCurrentElement(EElements ENewElement = EElements::None);
};
