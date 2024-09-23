// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;

UCLASS()
class GLITCHADVENTURE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerCharacter* PlayerREF;

	UPROPERTY(EditAnywhere)
		float Speed;
	UPROPERTY(EditAnywhere)
		float Health;
	UPROPERTY(EditAnywhere)
		float AttackDamage;
	UPROPERTY(EditAnywhere)
		float AttackTimeout;
	UPROPERTY(EditAnywhere)
		float TimeSinceLastAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead;
	UPROPERTY(EditAnywhere)
		UClass* BPBullet;
	UPROPERTY(EditAnywhere)
		float BulletLaunchImpulse;
	
	void Attack(AActor* AttackTarget);

	UPROPERTY(EditAnywhere)
		USphereComponent* SightSphere;
	UPROPERTY(EditAnywhere)
		USphereComponent* AttackRangeSphere;

	bool IsInAttackRange(float Distance)
	{
		return Distance < AttackRangeSphere->GetScaledSphereRadius();
	}

	bool IsInSightRange(float Distance)
	{
		return Distance < SightSphere->GetScaledSphereRadius();
	}

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
};
