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

};
