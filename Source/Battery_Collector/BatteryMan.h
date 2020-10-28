/*
 * @Author: Xiao Shanghua
 * @Date: 2020-10-11 19:19:02
 * @LastEditTime: 2020-10-25 16:41:14
 * @LastEditors: Xiao Shanghua
 * @Description: 
 * @FilePath: \Battery_Collector\Source\Battery_Collector\BatteryMan.h
 */
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "string.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
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

	int ComboLoop;

	// String movingStatus; // run / sprint / idle

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DefaultMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Movement")
		float SprintMultiplier;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power_Threshold;

	//UPROPERTY(VisibleAnywhere, Category="Animation")
	//	class UAnimSequence* MeleeFistAttack; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
		class UAnimMontage* MeleeFistAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* DanceMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* CombatAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimMontage* JumpMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* LeftFistCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* RightFistCollisionBox;

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

	void ResumeMovingStatus();

	void SprintHold();
	void SprintStart();
	void SprintStop();
	void DanceStart();
	void DanceStop();
	void DanceLoop();
	void AttackStart();
	void AttackStop();
	void JumpStart();
	void JumpEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
