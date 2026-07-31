// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "IMIR_Button.h"
#include "MIR_OnOffButton.generated.h"

UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_OnOffButton : public AActor, public IIMIR_Button
{
	GENERATED_BODY()

	// Collision for On/Off button.
	UPROPERTY(EditAnywhere, Category = "CollisionComponent")
		USphereComponent* CollisionComponent;

	// Mesh for On/Off button. 
	UPROPERTY(EditAnywhere, Category = "Mesh")
		USkeletalMeshComponent* Mesh;
	
	// Checks if button is turned on. 
	UPROPERTY(EditAnywhere, Category = "Button")
		bool bDidTurnOn = false;

	// Linked actor for On/Off button.
	UPROPERTY(EditAnywhere, Category = "Link")
		AActor* LinkedActor = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AMIR_OnOffButton();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	// Called when player collides 
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Turns on button. 
	virtual void TurnOnButton_Implementation(AMIR_PlayerChar* PlayerCharacter) override;

	// Turns off button. 
	virtual void TurnOffButton_Implementation(AMIR_PlayerChar* PlayerCharacter) override;
};
