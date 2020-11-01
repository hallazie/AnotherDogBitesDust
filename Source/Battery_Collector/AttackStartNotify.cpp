// Copyright Epic Games, Inc. All Rights Reserved.


#include "AttackStartNotify.h"

#include "BatteryMan.h"
#include "Engine.h"

void UAttackStartNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Cyan, __FUNCTION__);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		ABatteryMan* Player = Cast<ABatteryMan>(MeshComp->GetOwner());
		if (Player != NULL) {
			Player->AttackStart();
		}
	}
}

void UAttackStartNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Cyan, __FUNCTION__);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		ABatteryMan* Player = Cast<ABatteryMan>(MeshComp->GetOwner());
		if (Player != NULL) {
			Player->AttackStop();
		}
	}
}
