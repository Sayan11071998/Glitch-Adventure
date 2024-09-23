// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PickupItem.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackTimeout = 1.5f;
	TimeSinceLastStrike = 0.f;
	CanAttack = true;
	AttackStarted = false;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.0f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

	BulletLaunchImpulse = 5000.f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastStrike += DeltaTime;

	if(TimeSinceLastStrike > AttackTimeout && !CanAttack)
	{
		CanAttack = true;
		TimeSinceLastStrike = 0;
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Yaw", this, &APlayerCharacter::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::Pitch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::OnAttack);

}

void APlayerCharacter::MoveForward(float Amount)
{
	if(Controller && Amount)
	{
		IsWalking = true;
		FVector fwd = GetActorForwardVector();
		AddMovementInput(fwd, Amount);
	}

	if(Amount == 0)
		IsWalking = false;
}

void APlayerCharacter::MoveRight(float Amount)
{
	if(Controller && Amount)
	{
		FVector right = GetActorRightVector();
		AddMovementInput(right, Amount);
	}
}

void APlayerCharacter::Yaw(float Amount)
{
	AddControllerYawInput(200.f * Amount * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Pitch(float Amount)
{
	AddControllerPitchInput(200.f * Amount * GetWorld()->GetDeltaSeconds());
}

float APlayerCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	HP -= Damage;

	if(HP <= 0)
	{
		HP = 0;
		Dead = true;
		UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()), false);
	}

	return Damage;
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<APickupItem>(OtherActor))
	{
		if(OtherActor->ActorHasTag("Key"))
		{
			Score += 300;
			HasKey = true;
		}
		else
		{
			Score += 100;
		}

		OtherActor->Destroy();
	}
}

void APlayerCharacter::OnAttack()
{
	AttackStarted = true;

	if(CanAttack)
	{
		FVector fwd = GetActorForwardVector();
		FVector nozzle = GetMesh()->GetBoneLocation("ArmR3");

		nozzle += fwd * 55;

		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(BPFireball, nozzle, RootComponent->GetComponentRotation());

		CanAttack = false;

		if(bullet)
		{
			bullet->CollisionSphere->AddImpulse(fwd * BulletLaunchImpulse);
		}
	}
}

