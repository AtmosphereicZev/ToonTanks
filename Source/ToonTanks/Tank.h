// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Components/TimelineComponent.h"
#include "InputActionValue.h"
#include "Tank.generated.h"

class UHealthComponent;
class UInputAction;
class UInputMappingContext;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	void HandleDestruction();

	void BoostForward(FVector Location);

	bool inJumpZone = false;
	bool Jumping = false;

	APlayerController* GetPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Mapping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* Turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* RotateTurret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* JumpAction;

	void MoveFunction(const FInputActionValue& Value);
	void TurnFunction(const FInputActionValue& Value);
	void JumpFunction(const FInputActionValue& Value);
	//void FireFunction(const FInputActionValue& Value);

	UHealthComponent* HealthComp;

	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	bool test = false;
	FVector testlo;

	UPROPERTY(EditAnywhere)
	float Speed;

	APlayerController* SecondaryController;

public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;
	
};
