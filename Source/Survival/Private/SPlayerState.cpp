// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::AddScore(float ScoreDelta)
{
	float MyScore;
	MyScore = GetScore();
	MyScore += ScoreDelta;
	SetScore(MyScore);
}

void ASPlayerState::TakeAwayScore(float ScoreDelta)
{
	float MyScore;
	MyScore = GetScore();
	MyScore -= ScoreDelta;
	SetScore(MyScore);
}
