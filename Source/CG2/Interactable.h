// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "Interactable.generated.h"

UCLASS()
class CG2_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	virtual bool Interact(class APlr* InteractingCharacter);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Static Mesh for Item */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* InteractCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
