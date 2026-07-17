// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Image.h"
#include "MIR_ElementComponent.generated.h"

USTRUCT(BlueprintType)  
struct FElementMap
{
	GENERATED_BODY()

	// Name of the element. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementName = "Nothing";

	// Sets the color of the element.
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		UMaterial* MainColor = nullptr;

	// What represents the element. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		UTexture2D* ElementSymbol;

	// What the current element is strong against (x2 multiplier). 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementStrength = "Nothing";

	// What the current element is weak against (x0 multiplier). 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementWeakness = "Nothing";

	// List of elements that the element name is neutral against. (x1 multiplier)
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		TArray<FString> ElementalNeutralities = {"Nothing", "Flame", "Ice", "Thunder"};

	// The type of action the element does. 
	UPROPERTY(EditAnywhere, Category = "ElementMap")
		FString ElementProperty = "None";

};

USTRUCT(BlueprintType)
struct FFlameElement : public FElementMap 
{
	GENERATED_BODY()

};

USTRUCT(BlueprintType)
struct FIceElement : public FElementMap
{
	GENERATED_BODY()

};


USTRUCT(BlueprintType)
struct FThunderElement : public FElementMap
{
	GENERATED_BODY()

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ElementComponent : public UActorComponent
{
	GENERATED_BODY()

	// List of elements. 
	UPROPERTY(EditAnywhere, Category = "Elements")
		TArray<FElementMap> ElementMap;

public:	
	// No Element
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elements")
		FElementMap NothingElement;

	// Flame Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elements")
		FFlameElement FlameElement;

	// Ice Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elements")
		FIceElement IceElement;

	// Thunder Element.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elements")
		FThunderElement ThunderElement;

	// Sets default values for this component's properties
	UMIR_ElementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Adds new element to the map. 
	UFUNCTION(BlueprintCallable)
		void AddStructToElement(FElementMap NewElementType);

	// Gets the current element map. 
	UFUNCTION(BlueprintCallable)
		TArray<FElementMap> GetElementMap();
};
