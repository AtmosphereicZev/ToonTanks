// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    RootComponent = CapsuleCom;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleCom);

	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(Turret);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::RotateTurretFunc(FVector LookLocation)
{
	FVector EndPoint = LookLocation - Turret->GetComponentLocation();
	FRotator Rotation = FRotator(0, EndPoint.Rotation().Yaw, 0);
	Turret->SetWorldRotation(FMath::RInterpTo(Turret->GetComponentRotation(), Rotation, UGameplayStatics::GetWorldDeltaSeconds(this), 25.0f));
}

void ABasePawn::FireTurretFunc()
{
	FVector Loc = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rot = ProjectileSpawnPoint->GetComponentRotation();
	auto SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile, Loc, Rot);
	SpawnedProjectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, GetActorLocation(), GetActorRotation());
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShake);
	bIsAlive = false;
}