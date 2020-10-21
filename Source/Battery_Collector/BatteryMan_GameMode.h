/*
 * @Author: Xiao Shanghua
 * @Date: 2020-10-14 22:05:43
 * @LastEditTime: 2020-10-22 01:59:49
 * @LastEditors: Xiao Shanghua
 * @Description: 
 * @FilePath: \Battery_Collector\Source\Battery_Collector\BatteryMan_GameMode.h
 */
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BatteryMan_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTERY_COLLECTOR_API ABatteryMan_GameMode : public AGameMode
{
	GENERATED_BODY()

	ABatteryMan_GameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PlayerRecharge;

	float Spawn_Z = 500.0f;

	UPROPERTY(EditAnywhere)
	float Spawn_X_Min;
	UPROPERTY(EditAnywhere)
	float Spawn_X_Max;
	UPROPERTY(EditAnywhere)
	float Spawn_Y_Min;
	UPROPERTY(EditAnywhere)
	float Spawn_Y_Max;

	void SpawnPlayerRecharge();

};
