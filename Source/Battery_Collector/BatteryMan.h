// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "string.h"
#include <string>
#include "CoreMinimal.h"
#include "Engine.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"

#include "BatteryMan.generated.h"

UCLASS()
class BATTERY_COLLECTOR_API ABatteryMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABatteryMan();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool bDead;
	bool bAttacking;
	bool bDancing;
	bool bInAir;
	bool bActivateSprint;

	int ComboLoop;
	int DanceType;

	float LeftFistDamage;
	float RightFistDamage;

	class UAnimMontage* DanceMontage;

	// String movingStatus; // run / sprint / idle

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DefaultMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Movement")
		float SprintMultiplier;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float Energy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power_Threshold;

	//UPROPERTY(VisibleAnywhere, Category="Animation")
	//	class UAnimSequence* MeleeFistAttack; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
		class UAnimMontage* MeleeFistAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* DanceRumbaMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* DanceSillyMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* CombatAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* JumpMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* LeftFistCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* RightFistCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USoundWave* FootStepDirtSoundWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USoundWave* FootStepGrassSoundWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USoundWave* FootStepConcretSoundWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<AActor*> HitActorArrayCache;

	UFUNCTION()
		void OnBeginOverlap(
			class UPrimitiveComponent* HitComp, 
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult & SweepResult
			);

	UPROPERTY(EditAnywhere, Category="UI HUD")
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;

	UUserWidget* Player_Power_Widget;

	void RestartGame();

	void ResumeMovingStatus(bool);

	void SprintHold();
	void SprintStart();
	void SprintStop();
	void DanceStart();
	void DanceStop();
	void DanceLoop();
	void AttackInput();
	void AttackStart();
	void AttackStop();
	void JumpInput();
	void JumpStart();
	void JumpEnd();
	void TriggerFootStep();

	float GetCurrentSpeed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
