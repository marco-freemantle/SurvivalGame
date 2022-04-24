// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Online/CoreOnline.h"
#include "Net/UnrealNetwork.h"


void ASGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void ASGameState::OnRep_CurrentRound(int32 PreviousRound)
{
	CurrentRoundChanged(CurrentRound, PreviousRound);
}

//Replicate the CurrentWeapon variable
void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
	DOREPLIFETIME(ASGameState, CurrentRound);
}

void ASGameState::SetWaveState(EWaveState NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		EWaveState OldState = WaveState;

		WaveState = NewState;
		OnRep_WaveState(OldState);
	}
}
