// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLaunchPad.generated.h"

UCLASS()
class SURVIVAL_API ASLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLaunchPad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LaunchStrength;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LaunchPitchAngle;

};
