// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	
}

void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ATank::MoveFunction);
		EnhancedInputComponent->BindAction(Turn, ETriggerEvent::Triggered, this, &ATank::TurnFunction);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Started, this, &ATank::FireTurretFunc);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this , &ATank::JumpFunction);
	}
	
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	SecondaryController = Cast<APlayerController>(GetController());
	
	// if (Controller != nullptr)
	// {
	// 	FHitResult HitResult;
	// 	Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	// 	FVector Location = HitResult.Location;
	// 	DrawDebugSphere(GetWorld(), Location, 3, 10, FColor::Red, false, 3)
	// }else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Could not find Player Controller!"))
	// }
	
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (test)
	{
		FVector ActorLocation = GetActorLocation();
		FVector ActorEndLocation = {testlo.X, testlo.Y, testlo.Z};
		FVector Interp = FMath::VInterpTo(ActorLocation, ActorEndLocation, UGameplayStatics::GetWorldDeltaSeconds(this), 8);
		SetActorLocation(Interp);
	}
	
	if (Controller != nullptr)
	{
		FHitResult HitResult;
		SecondaryController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		FVector Location = HitResult.Location;
		RotateTurretFunc(Location);
		//DrawDebugSphere(GetWorld(), Location, 3, 10, FColor::Red, false, 0);
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Player Controller!"))
	}
}

void ATank::MoveFunction(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector Location(0.0f);
	Location.X = Input * Speed * DeltaTime;
	AddActorLocalOffset(Location, true);
}

void ATank::TurnFunction(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator Rotation(0.0f);
	Rotation.Yaw = Input * 200 * DeltaTime;
	AddActorLocalRotation(Rotation, true);
	
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

APlayerController* ATank::GetPlayerController()
{
	return SecondaryController;	
}

void ATank::JumpFunction(const FInputActionValue& Value)
{
	if (inJumpZone)
	{
		Jumping = true;
	}
}

void ATank::BoostForward(FVector Location)
{
	testlo = Location;
	test = true;
	// FVector ActorLocation = GetActorLocation();
	// FVector ActorEndLocation = {Location.X, Location.Y, GetActorLocation().Z};
	// FVector Interp = FMath::VInterpTo(ActorLocation, ActorEndLocation, UGameplayStatics::GetWorldDeltaSeconds(this), 8);
	// SetActorLocation(Interp);
}