// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameData.h"
#include "Weapon.generated.h"

UCLASS()
class CG2_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AInteractable> InteractableClass;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EWeapons> WeaponType;


protected:

	/** Static Mesh for Weapon */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;


};
