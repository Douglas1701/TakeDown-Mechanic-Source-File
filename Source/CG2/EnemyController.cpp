// Fill out your copyright notice in the Description page of Project Settings.

#include "CG2.h"
#include "Plr.h"
#include "EnemyController.h"

AEnemyController::AEnemyController(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Create all the components in controller

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	SetPerceptionComponent(*ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("Perception")));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	GetAIPerceptionComponent()->ConfigureSense(*Sight);
	GetAIPerceptionComponent()->SetDominantSense(Sight->GetSenseImplementation());

	// Bind OnPerceptionUpdate to a custom function in controller
	GetAIPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::UpdatePerception);
}

void AEnemyController::Possess(APawn* PawnRef)
{
	Super::Possess(PawnRef);

	// Set up Behavior Tree and Blackboard 
	if (PawnRef)
	{
		EnemyRef = Cast<AEnemy>(PawnRef);

		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);


		// Set Up AI Perception (Sight)
		Sight->SightRadius = EnemyRef->SightRange;
		Sight->LoseSightRadius = EnemyRef->LoseSightRange;
		Sight->PeripheralVisionAngleDegrees = EnemyRef->SightAngle;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = true;

		GetAIPerceptionComponent()->ConfigureSense(*Sight);

		UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, Sight->GetSenseImplementation(), EnemyRef);
	}
}

void AEnemyController::UpdatePerception(TArray<AActor*> SensedActors)
{
	// Look to see if the player gets into enemy's sight range

	for (AActor* Actor : SensedActors)
	{
		APlr* Player = Cast<APlr>(Actor);

		if (Player)
		{
			FActorPerceptionBlueprintInfo Info;

			GetAIPerceptionComponent()->GetActorsPerception(Actor, Info);

			if (Info.LastSensedStimuli.Num() > 0)
			{
				const FAIStimulus Stimulus = Info.LastSensedStimuli[0];

				// If Enemy see's player in range
				if (Stimulus.WasSuccessfullySensed())
				{
					Player->bIsSeen = true;
				}
				else // If Enemy loses sight of player
				{
					Player->bIsSeen = false;
				}
			}
		}
	}
}

AActor* AEnemyController::GetSeeingPawn()
{

	//Return the seeing pawn
	UObject* object = BlackboardComponent->GetValueAsObject(TEXT("TargetActor"));

	return object ? Cast<AActor>(object) : nullptr;
}






