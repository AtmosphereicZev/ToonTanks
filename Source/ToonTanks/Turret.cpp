// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"

ATurret::ATurret()
{
	
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TurretTimer, this, &ATurret::CheckFireCondition, 2, true);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Tank != nullptr)
	{
		float Distance = FVector::Dist(this->GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= 1000.0f && Tank->bIsAlive == true)
		{
			RotateTurretFunc(Tank->GetActorLocation());
		}
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tank is NULL"));
	}
	
}

void ATurret::CheckFireCondition()
{
	if (Tank != nullptr && Tank->bIsAlive)
	{
		float Distance = FVector::Dist(this->GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= 1000.0f)
		{
			FireTurretFunc();
		}
	}
}

void ATurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}