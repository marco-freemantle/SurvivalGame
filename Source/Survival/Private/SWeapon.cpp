// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Survival.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";

	BaseDamage = 20.0f;
	BulletSpread = 2.0f;

	RateOfFire = 600;

	BaseAmmo = 30;
	AmmoInClip = BaseAmmo;
	bCanShoot = true;

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;

}

void ASWeapon::Fire()
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

			FVector ShotDirection = EyeRotation.Vector();

			//Bullet spread
			float HalfRad = FMath::DegreesToRadians(BulletSpread);
			ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

			FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial = true;

			FVector TracerEndPoint = TraceEnd;

			EPhysicalSurface SurfaceType = SurfaceType_Default;

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
			{
				AActor* HitActor = Hit.GetActor();

				SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				float ActualDamage = BaseDamage;
				if (SurfaceType == SURFACE_FLESHVULNERABLE)
				{
					ActualDamage *= 4;
				}

				UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

				PlayImpactEffect(SurfaceType, Hit.ImpactPoint);

				TracerEndPoint = Hit.ImpactPoint;
			}

			PlayFireEffects(TracerEndPoint);

			if (GetLocalRole() == ROLE_Authority)
			{
				HitScanTrace.TraceTo = TracerEndPoint;
				HitScanTrace.SurfaceType = SurfaceType;
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

void ASWeapon::OnRep_HitScanTrace()
{
	//Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::PlayFireEffects(FVector TraceEnd)
{
	AActor* MyOwner = GetOwner();

	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation("MuzzleSocket");

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
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
}

void ASWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;

	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::ServerReload_Implementation()
{
	StartReloadTimer();
}

bool ASWeapon::ServerReload_Validate()
{
	return true;
}

void ASWeapon::StartFire()
{
	FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ASWeapon::StartReloadTimer()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerReload();
	}

	if (AmmoInClip < BaseAmmo)
	{
		bCanShoot = false;
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &ASWeapon::Reload, TimeToReload, false);
	}
}

void ASWeapon::Reload()
{
	AmmoInClip = BaseAmmo;;
	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadTime);
	bCanShoot = true;
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}

