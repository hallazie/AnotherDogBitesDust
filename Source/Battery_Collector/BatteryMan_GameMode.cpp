/*
 * @Author: Xiao Shanghua
 * @Date: 2020-10-14 22:05:43
 * @LastEditTime: 2020-10-22 02:13:16
 * @LastEditors: Xiao Shanghua
 * @Description: 
 * @FilePath: \Battery_Collector\Source\Battery_Collector\BatteryMan_GameMode.cpp
 */
// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryMan_GameMode.h"
#include "GameFramework/Actor.h"


ABatteryMan_GameMode::ABatteryMan_GameMode(){

    PrimaryActorTick.bCanEverTick = true;

}

void ABatteryMan_GameMode::BeginPlay(){

    Super::BeginPlay();

    FTimerHandle UnusedHandle;
    GetWorldTimerManager().SetTimer(
        UnusedHandle, this, &ABatteryMan_GameMode::SpawnPlayerRecharge, FMath::RandRange(2, 5), true
    );

}

void ABatteryMan_GameMode::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
}

void ABatteryMan_GameMode::SpawnPlayerRecharge(){

    float RandomX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
    float RandomY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

    FVector SpawnPosition = FVector(RandomX, RandomY, Spawn_Z);
    FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

    GetWorld()->SpawnActor(PlayerRecharge, &SpawnPosition, &SpawnRotation);

}
