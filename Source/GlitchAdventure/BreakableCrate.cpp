// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableCrate.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PickupItem.h"

// Sets default values
ABreakableCrate::ABreakableCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	Explosion = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion"));
	Explosion->AttachToComponent(CollisionBox, FAttachmentTransformRules::KeepRelativeTransform, FName(""));

	Health = 100.f;

}

// Called when the game starts or when spawned
void ABreakableCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableCrate::LootDrop()
{
	if(FMath::RandRange(0, 10) > 5)
	{
		FVector Up = GetActorUpVector();
		FVector Offset = CollisionBox->GetComponentLocation();
		Offset += Up;

		APickupItem* Pickup = GetWorld()->SpawnActor<APickupItem>(BPPickupItem, Offset, RootComponent->GetComponentRotation());
	}
}

float ABreakableCrate::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= Damage;
	
	if(Health <= 0)
	{
		LootDrop();

		Explosion->ToggleActive();
		Mesh->ToggleVisibility(false);
		SetLifeSpan(0.1);
	}

	return 0.f;
}

