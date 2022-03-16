// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ASGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;

	/*Spawning the projectile in blueprint allows us to pass the instantiated actor
	the instigator controller. This allows players to receieve a score for killing enemies with
	projectiles */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnProjectile(FRotator EyeRotation);

};
