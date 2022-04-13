#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);


UCLASS()
class SURVIVAL_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	FTimerHandle TimerHandle_GameStart;

	//Bots to spawn in current wave
	int32 NrOfBotsToSpawn;

	UPROPERTY(BlueprintReadWrite, Category = "GameMode")
	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnBoss();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void CheckWaveAndPlayerState();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RespawnDeadPlayers();

public:

	ASGameMode();

	virtual void StartPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
};
