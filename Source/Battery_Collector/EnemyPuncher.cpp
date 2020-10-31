// Copyright Epic Games, Inc. All Rights Reserved.


#include "EnemyPuncher.h"

// Sets default values
AEnemyPuncher::AEnemyPuncher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

