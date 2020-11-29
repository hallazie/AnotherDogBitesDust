// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDeadNotify.h"


void UEnemyDeadNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		AActor* Actor = MeshComp->GetOwner();
		Actor->Destroy();
	}
}

void UEnemyDeadNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {

}