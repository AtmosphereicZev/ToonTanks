// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpZone.generated.h"

class ATank;
class UBoxComponent;

UCLASS()
class TOONTANKS_API AJumpZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool isInJumpZone;

	bool AllowAnimation;
	bool AllowAnimation2;
	bool AllowAnimation3;

	ATank* Tank;

	float GlobalDeltaTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* JumpZoneBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* TeleportLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* EndLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BoostLocation;
	
	UFUNCTION(BlueprintCallable)
	void onOverlap(AActor* OverlappedActor);

	UFUNCTION(BlueprintCallable)
	void onOverlapEnd(AActor* OverlappedActor);

	void AnimationFunction();
	void jumpFunction();
	void BoostFunction();
	
};
