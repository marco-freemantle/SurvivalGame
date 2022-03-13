// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenadeLauncher.h"
#include "Kismet/GameplayStatics.h"

void ASGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

//Overriding the SWeapon fire function
void ASGrenadeLauncher::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		if (AmmoInClip > 0 && bCanShoot)
		{
			AmmoInClip--;

			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (GetLocalRole() == ROLE_Authority)
			{
				GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
			}

			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			if (MyOwner)
			{
				APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
				if (PC)
				{
					PC->ClientStartCameraShake(FireCamShake);
				}
			}

			LastFireTime = GetWorld()->TimeSeconds;
		}
		else
		{
			if (bCanShoot)
			{
				if (GetLocalRole() < ROLE_Authority)
				{
					ServerReload();
				}
				StartReloadTimer();
			}
		}
	}
}