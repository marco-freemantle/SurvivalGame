#include "SBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASBlackHole::ASBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComp->SetupAttachment(MeshComp);

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHole::OverlapInnerSphere);

	OuterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComp->SetupAttachment(MeshComp);
}

//When an an actor overlaps with this actor;s innersphere, destroy it
void ASBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}


// Called every frame
void ASBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get an array of all overlapping components
	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphereComp->GetOverlappingComponents(OverlappingComps);

	//For each overlapping component
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		//If the component is simulating physics
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = OuterSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			//Pull the actor towards to centre of the blackhole
			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

