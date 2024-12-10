// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "ToonTanksPlayerController.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
	StartGame();
}

void AToonTanksGameMode::HandleDeath(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(true);
	}
	else if(ATurret* Turret = Cast<ATurret>(DeadActor))
	{
		Turret->HandleDestruction();
		TowerCount--;
		if (TowerCount == 0)
		{
			GameOver(false);
		}
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	TowerCount = GetTowerCount();
	
	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle TimerHandle;
		FTimerDelegate EnabledTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(TimerHandle, EnabledTimerDelegate, 5.0f, false);
	}
	
}

int32 AToonTanksGameMode::GetTowerCount()
{
	TArray<AActor*> TurretArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurret::StaticClass(), TurretArray);
	int32 Num = TurretArray.Num();

	return Num;
}