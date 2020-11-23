// Copyright Epic Games, Inc. All Rights Reserved.


#include "EnemyPuncher.h"

// Sets default values
AEnemyPuncher::AEnemyPuncher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;

	bBeingHit = false;

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SimpleAttackMtgObj(TEXT("AnimMontage'/Game/Characters/Enemy/BPM_EnemyPuncherAttack.BPM_EnemyPuncherAttack'"));
	if (SimpleAttackMtgObj.Succeeded()) {
		SimpleAttackMontage = SimpleAttackMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactMtgObj(TEXT("AnimMontage'/Game/Characters/Enemy/BPM_HitReact.BPM_HitReact'"));
	if (HitReactMtgObj.Succeeded()) {
		HitReactMontage = HitReactMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMtgObj(TEXT("AnimMontage'/Game/Characters/Enemy/BPM_Dead.BPM_Dead'"));
	if (DeadMtgObj.Succeeded()) {
		DeadMontage = DeadMtgObj.Object;
	}

}

// Called when the game starts or when spawned
void AEnemyPuncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPuncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Health <= 0.0f) {
	//	PlayAnimMontage(DeadMontage);
	//}

}

// Called to bind functionality to input
void AEnemyPuncher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyPuncher::AttackStart() {
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	PlayAnimMontage(SimpleAttackMontage, 2.0f);
}

void AEnemyPuncher::AttackStop() {
	StopAnimMontage(SimpleAttackMontage);
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void AEnemyPuncher::BehaviourTreePatrol() {

}

void AEnemyPuncher::BehaviourTreeChasePlayer() {

}

void AEnemyPuncher::BehaviourTreeAttackPlayer() {
}

float AEnemyPuncher::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	FString StrDamageAmount = FString::SanitizeFloat(DamageAmount);
	//FString StrHealth = FString::SanitizeFloat(Health);

	Health -= DamageAmount;
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Red, *StrDamageAmount);
	PlayAnimMontage(HitReactMontage);

	return 0.0f;
}
