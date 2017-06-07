// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class CG2_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	/** Arrow Component for player's takedown position **/
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* TakedownPosition;

	UPROPERTY(EditDefaultsOnly)
	float SightRange;

	UPROPERTY(EditDefaultsOnly)
	float LoseSightRange;

	UPROPERTY(EditDefaultsOnly)
	float SightAngle;

	bool bSeesPlayer;

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
};
