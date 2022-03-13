#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBlackHole.generated.h"

UCLASS()
class SURVIVAL_API ASBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackHole();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* InnerSphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* OuterSphereComp;

	UFUNCTION()
	void OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
