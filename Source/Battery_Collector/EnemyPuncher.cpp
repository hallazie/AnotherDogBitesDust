// Copyright Epic Games, Inc. All Rights Reserved.


#include "EnemyPuncher.h"

// Sets default values
AEnemyPuncher::AEnemyPuncher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SimpleAttackMontageObject(TEXT("AnimMontage'/Game/Characters/Enemy/BPM_EnemyPuncherAttack.BPM_EnemyPuncherAttack'"));
	if (SimpleAttackMontageObject.Succeeded()) {
		SimpleAttackMontage = SimpleAttackMontageObject.Object;
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