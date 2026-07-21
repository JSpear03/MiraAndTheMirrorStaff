// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Image.h"
#include "MIR_ElementComponent.generated.h"

USTRUCT(BlueprintType)  
struct FElement
{
	GENERATED_BODY()

	// Name of the element. 
	UPROPERTY(EditAnywhere, Category = "Element", meta = (GetOptions = "GetElementNames"))
		FString ElementName = "Nothing";

	// Sets the color of the element.
	UPROPERTY(EditAnywhere, Category = "Element")
		UMaterial* MainColor = nullptr;

	// What represents the element. 
	UPROPERTY(EditAnywhere, Category = "Element")
		UTexture2D* ElementSymbol;

	// What the current element is strong against (x2 multiplier). 
	UPROPERTY(EditAnywhere, Category = "Element", meta = (GetOptions = "GetElementNames"))
		FString ElementStrength = "Nothing";

	// What the current element is weak against (x0 multiplier). 
	UPROPERTY(EditAnywhere, Category = "Element", meta = (GetOptions = "GetElementNames"))
		FString ElementWeakness = "Nothing";

	// List of elements that the element name is neutral against. (x1 multiplier)
	UPROPERTY(EditAnywhere, Category = "Element", meta = (GetOptions = "GetElementNames"))
		TArray<FString> ElementalNeutralities;

	// The type of action the element does. 
	UPROPERTY(EditAnywhere, Category = "Element", meta = (GetOptions = "GetElementProperties"))
		FString ElementProperty = "None";

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ElementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element Type")
		FElement ElementType;

	// Sets default values for this component's properties
	UMIR_ElementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Returns a list of element names.
	// RangedNothing is only for the player.
	UFUNCTION()
		TArray<FString> GetElementNames();

	// Returns a list of properties for each element. 
	UFUNCTION()
		TArray<FString> GetElementProperties();

};
