/*
 * @Author: Xiao Shanghua
 * @Date: 2020-10-14 22:05:43
 * @LastEditTime: 2020-10-20 23:24:20
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
};
