// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "NotifyKillEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CG2_API UNotifyKillEnemy : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// Notiy Function
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation);
	
	
};
