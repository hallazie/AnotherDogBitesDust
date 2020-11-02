// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPuncher.generated.h"

UCLASS()
class BATTERY_COLLECTOR_API AEnemyPuncher : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyPuncher();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* SimpleAttackMontage;

	UFUNCTION(BlueprintCallable, Category = "ControlFunction")
		void AttackStart();

	UFUNCTION(BlueprintCallable, Category = "ControlFunction")
		void AttackStop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
