// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	USGameInstance(const FObjectInitializer& ObjectInitializer);

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

};
