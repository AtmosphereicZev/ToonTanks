// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpZone.h"

#include "MaterialHLSLTree.h"
#include "Tank.h"
#include "Components/BoxComponent.h"

// Sets default values
AJumpZone::AJumpZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JumpZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Zone"));
	JumpZoneBox->SetupAttachment(RootComponent);

	TeleportLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportLocation"));
	TeleportLocation->SetupAttachment(JumpZoneBox);

	EndLocation = CreateDefaultSubobject<USceneComponent>(TEXT("EndLocation"));
	EndLocation->SetupAttachment(JumpZoneBox);

	BoostLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BoostLocation"));
	BoostLocation->SetupAttachment(JumpZoneBox);
	
}

// Called when the game starts or when spawned
void AJumpZone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AJumpZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GlobalDeltaTime = DeltaTime;

	if (isInJumpZone)
	{
		Tank->inJumpZone = true;
		if (Tank->Jumping)
		{
			APlayerController* Controller = Cast<APlayerController>(Tank->GetController());
			Tank->DisableInput(Controller);
			FVector TeleportVector = {TeleportLocation->GetComponentLocation().X, TeleportLocation->GetComponentLocation().Y, Tank->GetActorLocation().Z};
			Tank->SetActorLocation(TeleportVector);
			Tank->SetActorRotation(TeleportLocation->GetComponentRotation());
			FTimerHandle TimerHandle;
			FTimerHandle TimerHandle2;
			FTimerHandle TimerHandle3;
			if (AllowAnimation)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AJumpZone::AnimationFunction, 1, false);
			}
			if (AllowAnimation2)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AJumpZone::jumpFunction, 4, false);
			}
			if (AllowAnimation3)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle3, this, &AJumpZone::BoostFunction, 5, false);
			}
		}
	}
	
}

void AJumpZone::onOverlap(AActor* OverlappedActor)
{
	if (OverlappedActor && Cast<ATank>(OverlappedActor))
	{
		Tank = Cast<ATank>(OverlappedActor);
		if (Tank)
		{
			isInJumpZone = true;
			AllowAnimation = true;
			AllowAnimation2 = true;
			AllowAnimation3 = true;
		}
	}	
}

void AJumpZone::onOverlapEnd(AActor* OverlappedActor)
{
	if (OverlappedActor == Tank)
	{
		isInJumpZone = false;
		Tank->inJumpZone = false;
		Tank->Jumping = false;
		Tank = nullptr;
	}
}

void AJumpZone::AnimationFunction()
{
	AllowAnimation = false;
	FVector ActorScale = Tank->GetActorScale();
	FVector ActorEndScale = {1.000000, 0.690537, 0.850079};
	FVector Scale = FMath::VInterpTo(ActorScale, ActorEndScale, GlobalDeltaTime, 5);
	Tank->SetActorScale3D(Scale);
}

void AJumpZone::jumpFunction()
{
	AllowAnimation2 = false;
	FVector ActorScale = Tank->GetActorScale();
	FVector ActorEndScale = {1, 1, 1};
	FVector Scale = FMath::VInterpTo(ActorScale, ActorEndScale, GlobalDeltaTime, 5);
	Tank->SetActorScale3D(Scale);

	FVector ActorLocation = Tank->GetActorLocation();
	FVector ActorEndLocation = EndLocation->GetComponentLocation();
	FVector Interp = FMath::VInterpTo(ActorLocation, ActorEndLocation, GlobalDeltaTime, 10);
	Tank->SetActorLocation(Interp);
	
}

void AJumpZone::BoostFunction()
{
	AllowAnimation3 = false;
	Tank->BoostForward(BoostLocation->GetComponentLocation());
}
