// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageArea.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
ADamageArea::ADamageArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	RootComponent = DamageBox;

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageArea::OnBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(DamageBox);

}

// Called when the game starts or when spawned
void ADamageArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageArea::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, this);OtherActor->TakeDamage(Damage, FDamageEvent(), NULL, this);
}