// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Bullet.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 20.f;
	Health = 100.f;
	AttackTimeout = 1.5f;
	TimeSinceLastAttack = 0.f;
	BulletLaunchImpulse = 2000.f;

	SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
	SightSphere->SetupAttachment(RootComponent);

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerREF = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate Player Location
	FVector PlayerPos = PlayerREF->GetActorLocation();
	FVector DistanceDifference = PlayerPos - GetActorLocation();
	float DistanceToPlayer = DistanceDifference.Size();

	FRotator ToPlayerRotation = DistanceDifference.Rotation();
	ToPlayerRotation.Pitch = 0;

	if(!IsDead)
	{
		RootComponent->SetWorldRotation(ToPlayerRotation);
	}

	if(DistanceToPlayer > SightSphere->GetScaledSphereRadius())
	{
		IsAttacking = false;
		return;
	}

	if(IsInAttackRange(DistanceToPlayer) && !IsDead)
	{
		if(TimeSinceLastAttack == 0)
		{
			Attack(PlayerREF);
		}
		TimeSinceLastAttack += DeltaTime;
		if(TimeSinceLastAttack > AttackTimeout)
		{
			TimeSinceLastAttack = 0;
		}
		return;
	}

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Attack(AActor* AttackTarget)
{
	if(BPBullet)
	{
		IsAttacking = true;

		FVector fwd = GetActorForwardVector();
		FVector nozzle = GetMesh()->GetBoneLocation("Neck1");
		nozzle += fwd * 55;

		FVector ToOpponent = AttackTarget->GetActorLocation() - nozzle;
		ToOpponent.Normalize();

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(BPBullet, nozzle, RootComponent->GetComponentRotation());

		if(bullet)
		{
			bullet->CollisionSphere->AddImpulse(fwd * BulletLaunchImpulse);
		}
	}
}

float AEnemyCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= Damage;
	if(Health <= 0)
	{
		IsDead = true;
	}

	return 0.f;
}

