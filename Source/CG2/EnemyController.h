// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CG2_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyController(const class FObjectInitializer& ObjectInitializer);

	void Possess(APawn* PawnRef);

	/** Reference to the behavior tree */
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	/** Behavior tree components*/
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTreeComponent* BehaviorTreeComponent;

	/** Blackboard components */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlackboardComponent* BlackboardComponent; 

private:

	UAISenseConfig_Sight* Sight;

	AEnemy* EnemyRef;

	UFUNCTION()
	void UpdatePerception(TArray<AActor*> SensedActors);

	AActor* GetSeeingPawn();
	
	
	
};
