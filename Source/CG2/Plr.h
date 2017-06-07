// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TextWidget.h"
#include "GameData.h"
#include "Enemy.h"
#include "Plr.generated.h"




UCLASS()
class CG2_API APlr : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	USkeletalMeshComponent* ArmMesh;

public:
	// Sets default values for this character's properties
	APlr();


private:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	

	/// ** REFERENCE VARIABLES ** ///

	// Reference to check if player is crouching
	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching;

	// Reference to check if player can interact
	UPROPERTY(BlueprintReadWrite)
	bool bCanInteract;

	// Reference to check if player is attacking enemy
	UPROPERTY(BlueprintReadWrite)
	bool bTakeDown;

	// Reference to check which takedown move player is going to perform
	UPROPERTY(BlueprintReadWrite)
	int TakeDownMove;

	// Reference to check if player is seen by enemy
	bool bIsSeen;

	//Function to Kill enemy **Called from Anim Notifier**
	void KillEnemy();

private:

	// Reference to enemy that player is attacking
	AEnemy* KillingEnemy;

	// Start position of player for takedown movement (player's location)
	FVector StartPosition;

	// End position of player for takedown movement (Enemy's takedown position)
	FVector EndPosition;

	// Start rotation of player for takedown movement (player's rotation)
	FRotator StartRotation;

	// End rotation of player for takedown movement (Enemy's takedown rotation)
	FRotator EndRotation;

	// Time player started attack
	float StartTime;

	// Time player ends attack
	float EndTime;

	// Reference to check if player is attacking
	bool bAttacking;

	// Function that returns the start of the player's reach
	FVector GetReachLineStart();

	// Function that returns the end of the player's reach
	FVector GetReachLineEnd();

	// Function to Check if anything is in range to interact with
	void ReachCheck();

	// Interacting function
	void OnInteract();

	// Function that handles player's takedown logic
	void Takedown();

	// Function that handles player's movement to takedown
	void StartAttack(FVector StartLoc, FVector EndLoc, float Duration);

	// Function that handles logic for smooth movement to behind enemy
	void AttackMovement();

public:

	/// ** OBJECT REFERENCES ** ///
	
	// Current weapon player has equipped
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWeapons> EquippedWeapon;

	// References for the text widget class for interaction text
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextWidget> TextWidgetClass;

	UTextWidget* TextWidget;

	/// **Public Functions**//

	USkeletalMeshComponent* GetArms() { return ArmMesh; }




	/// ** VARIABLES FOR MOVEMENT ** ///

	// Turning rate for controllers
	UPROPERTY(EditAnywhere, Category = Movement)
		float BaseTurnRate;

	// Lookup rate for controllers
	UPROPERTY(EditAnywhere, Category = Movement)
		float BaseLookUpRate;

	// Base Movement speed for player
	UPROPERTY(EditAnywhere, Category = Movement)
		float BaseSpeed;

	// Reach distance for player
	UPROPERTY(EditAnywhere, Category = Movement)
		float Reach;

	// Speed for Player to move to Enemy on takedown
	UPROPERTY(EditAnywhere, Category = Movement)
		float AttackSpeed;


private:

	/// ** MOVEMENT FUNCTIONS ** ///

	// Moving forward function
	void MoveForward(float Val);

	// Moving right function
	void MoveRight(float Val);
	
	// Turning function
	void TurnRate(float Rate);

	// Looking up and down function
	void LookRate(float Rate);

	// Crouching function
	void ToggleCrouch();







	
};
