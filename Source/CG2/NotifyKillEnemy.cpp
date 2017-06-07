// Fill out your copyright notice in the Description page of Project Settings.

#include "CG2.h"
#include "Plr.h"
#include "NotifyKillEnemy.h"




void UNotifyKillEnemy::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// Get reference to player and call the KillEnemy function.
	APlr* Player = Cast<APlr>(MeshComp->GetOwner());

	if (Player)
	{
		Player->KillEnemy();
	}
}
