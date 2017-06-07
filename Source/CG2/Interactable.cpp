// Fill out your copyright notice in the Description page of Project Settings.

#include "CG2.h"
#include "Plr.h"
#include "Interactable.h"


// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a root component for the actor.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Creates a mesh and attaches it to root component.
	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Creates a collider and attaches it to root component.
	InteractCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	InteractCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 bool AInteractable::Interact(class APlr* InteractingCharacter)
{
	 // base function for this class, it is called when player is in range and presses interact button

	UE_LOG(LogTemp, Warning, TEXT("INTERACTWITHITEM"));

	FActorSpawnParameters SpawnParameters;
	AWeapon* PickupWeapon = GetWorld()->SpawnActor<class AWeapon>(WeaponClass, SpawnParameters);
	PickupWeapon->AttachRootComponentTo(InteractingCharacter->GetArms(), TEXT("bn_r_index1"));

	InteractingCharacter->EquippedWeapon = PickupWeapon->WeaponType;

	Destroy();

	 return true;
}

