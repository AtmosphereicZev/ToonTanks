// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

class AToonTanksPlayerController;
/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

	class ATank* Tank;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bGameOverCond);
	
private:
	//APlayerController* Controller;
	AToonTanksPlayerController* ToonTanksPlayerController;

	void HandleGameStart();

	int32 TowerCount;
	int32 GetTowerCount();
	
public:
	void HandleDeath(AActor* DeadActor);
};
