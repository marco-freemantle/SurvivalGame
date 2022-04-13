#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class SURVIVAL_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	class USHealthComponent* HealthComp;

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	float DefaultFOV;

	void BeginZoom();

	void EndZoom();

	//Replicated weapon variable
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
	class ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<AActor> DeployableAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketname;

	void Reload();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* InHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Pawn died previously
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	class UAnimMontage* ReloadAnimation;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
	void UseAbility();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player")
	void ServerSwapWeapon(TSubclassOf<class ASWeapon> NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void DestroyActor(class AActor* ActorToDestroy);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player")
	void ServerDestroyActor(class AActor* ActorToDestroy);

};
