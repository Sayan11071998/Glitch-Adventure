// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GLITCHADVENTURE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly)
		bool IsWalking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HP;
	UPROPERTY(BlueprintReadOnly)
		int Score;
	UPROPERTY(BlueprintReadOnly)
		bool HasKey;
	UPROPERTY(BlueprintReadWrite)
		bool AttackStarted;
	UPROPERTY(BlueprintReadWrite)
		bool Dead;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* TriggerCapsule;
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Yaw(float Amount);
	void Pitch(float Amount);

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Attack Properties
	void OnAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackTimeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeSinceLastStrike;

	bool CanAttack;

	// Bullet
	UPROPERTY(EditAnywhere)
		UClass* BPFireball;
	UPROPERTY(EditAnywhere)
		float BulletLaunchImpulse;

};
