// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	TrailEffect->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	
}

// Called when the game starts or when spawned

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// This is something called a delegate, it adds it to some other function which says
	// hey I need to know when this is called, basically an array of listeners.
	// ask Alex for more info :3
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation(), GetActorRotation(), 1, 1, 1);
}

// Called every frame

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* HitActor, UPrimitiveComponent* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	auto Insigator = GetOwner();
	auto Controller = Insigator->GetInstigatorController();
	if (Controller == nullptr) return;
	auto DamageType = UDamageType::StaticClass();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();	

	if (OtherActor && HitActor != this && HitActor != Insigator)
	{
		UGameplayStatics::ApplyDamage(HitActor, Damage, Controller, Insigator, DamageType);
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticle, GetActorLocation(), GetActorRotation(), 1);
	PlayerController->ClientStartCameraShake(CameraShake);
	ProjectileMesh->DestroyComponent();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AProjectile::DestoryActor, 0.5f, false);
	UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation(), GetActorRotation(), 1, 1, 1);
	//GetWorld()->DestroyActor(this);
	// As an alternative to using this, we can also use the Destroy Function
	//Destroy();
}

void AProjectile::DestoryActor()
{
	Destroy();
}