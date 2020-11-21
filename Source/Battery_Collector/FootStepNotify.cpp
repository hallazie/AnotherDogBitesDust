// Fill out your copyright notice in the Description page of Project Settings.


#include "FootStepNotify.h"

#include "BatteryMan.h"
#include "Engine.h"

void UFootStepNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		ABatteryMan* Player = Cast<ABatteryMan>(MeshComp->GetOwner());
		if (Player != NULL) {
			Player->TriggerFootStep();
		}
	}
}

void UFootStepNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

}