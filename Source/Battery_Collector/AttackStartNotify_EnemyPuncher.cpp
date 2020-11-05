// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackStartNotify_EnemyPuncher.h"
#include "EnemyPuncher.h"
#include "Engine.h"

void UAttackStartNotify_EnemyPuncher::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Cyan, __FUNCTION__);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		AEnemyPuncher* Player = Cast<AEnemyPuncher>(MeshComp->GetOwner());
		if (Player != NULL) {
			Player->AttackStart();
		}
	}
}

void UAttackStartNotify_EnemyPuncher::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Cyan, __FUNCTION__);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		AEnemyPuncher* Player = Cast<AEnemyPuncher>(MeshComp->GetOwner());
		if (Player != NULL) {
			Player->AttackStop();
		}
	}
}