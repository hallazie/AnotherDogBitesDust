// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackStartNotify.generated.h"

/**
 * 
 */
UCLASS()
class BATTERY_COLLECTOR_API UAttackStartNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
