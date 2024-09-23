// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsActivatable)
	{
		FVector ActorLocation = GetActorLocation();
		ActorLocation.Y += SpeedOfPlatform;

		if(IsDiagonal)
			ActorLocation.Z += SpeedOfPlatform;

		SetActorLocation(ActorLocation);

		if(ActorLocation.Y >= FirstEnd)
			SpeedOfPlatform *= -1.0f;

		if(ActorLocation.Y <= SecondEnd)
			SpeedOfPlatform *= -1.0f;
	}
	else if(IsActivatable && Activated)
	{
		FVector ActorLocation = GetActorLocation();
		ActorLocation.Y += SpeedOfPlatform;
		SetActorLocation(ActorLocation);

		if(ActorLocation.Y >= FirstEnd)
			SpeedOfPlatform *= -1.0f;

		if(ActorLocation.Y <= SecondEnd)
			SpeedOfPlatform *= -1.0f;
	}

}

void AMovingPlatform::TogglePlatform()
{
	IsActivatable = false;
}

