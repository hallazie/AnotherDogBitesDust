// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryManDamageNotify.h"
#include "BatteryMan.h"
#include "Engine.h"
#include "Engine/World.h"


void UBatteryManDamageNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		ABatteryMan* Player = Cast<ABatteryMan>(MeshComp->GetOwner());
		if (Player != NULL) {
			FVector LeftFistLocation = MeshComp->GetSocketLocation(FName("LeftFistCollision"));
			FVector RightFistLocation = MeshComp->GetSocketLocation(FName("RightFistCollision"));
		}
	}
}

void UBatteryManDamageNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		ABatteryMan* Player = Cast<ABatteryMan>(MeshComp->GetOwner());
		if (Player != NULL) {

		}
	}
}