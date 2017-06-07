// Fill out your copyright notice in the Description page of Project Settings.

#include "CG2.h"
#include "Interactable.h"
#include "Blueprint/UserWidget.h"
#include "EnemyController.h"
#include "Plr.h"


// Sets default values
APlr::APlr()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

	GetMesh()->DestroyComponent();

}

// Called when the game starts or when spawned
void APlr::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Stats
	bIsCrouching = false;
	Reach = 100.0f;
	EquippedWeapon = NONE;
	bCanInteract = true;

	// Set up Text Widget for Interactions
	TextWidget = CreateWidget<UTextWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), TextWidgetClass);
	TextWidget->GetInteractText()->SetVisibility(ESlateVisibility::Hidden);
	TextWidget->AddToViewport();

}

// Called every frame
void APlr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanInteract)
	{
		ReachCheck();
	}
	
	if (bAttacking)
	{
		AttackMovement();
	}
}

// Called to bind functionality to input
void APlr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlr::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlr::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlr::ToggleCrouch);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlr::OnInteract);
	PlayerInputComponent->BindAction("TakeDown", IE_Pressed, this, &APlr::Takedown);

}

void APlr::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void APlr::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void APlr::TurnRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlr::LookRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlr::ToggleCrouch()
{
	if (CanCrouch() == true)
	{
		Crouch();
		bIsCrouching = true;

		UE_LOG(LogTemp, Warning, TEXT("Crouch"));
		
	}
	else
	{
		UnCrouch();
		bIsCrouching = false;

		UE_LOG(LogTemp, Warning, TEXT("UnCrouch"));
	}
}

FVector APlr::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector APlr::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

void APlr::ReachCheck()
{
	/// ** If Player can Interact, This Section of code creates a line trace to simulate the player's reach range to see if he can interact or attack enemy.
	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// check if there is an interactable item in range, sets text on screen accordingly
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetReachLineStart(), GetReachLineEnd(), COLLISION_INTERACTABLES, TraceParams))
	{ 
		if (!TextWidget->GetInteractText()->IsVisible())
		{
			TextWidget->GetInteractText()->SetText(FText::FromString("E"));
			TextWidget->GetInteractText()->SetVisibility(ESlateVisibility::Visible);
		}
	} // check if there is an enemy in range, sets text on screen accordingly
	else if (GetWorld()->LineTraceSingleByChannel(Hit, GetReachLineStart(), GetReachLineEnd(), COLLISION_ENEMY, TraceParams))
	{
		if (EquippedWeapon != NONE && !bIsSeen)
		{
			if (!TextWidget->GetInteractText()->IsVisible())
			{
				TextWidget->GetInteractText()->SetText(FText::FromString("TakeDown"));
				TextWidget->GetInteractText()->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else  // if nothing on range, make sure text is hidden
	{
		if (TextWidget->GetInteractText()->IsVisible())
		{
			TextWidget->GetInteractText()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlr::OnInteract()
{

	// Create Line trace to make sure you are looking at an item to interact with

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, GetReachLineStart(), GetReachLineEnd(), COLLISION_INTERACTABLES, TraceParams))
	{
		AInteractable* InteractingItem = Cast<AInteractable>(Hit.GetActor());

		if (InteractingItem)
		{
			// Call the interact function on this item
			InteractingItem->Interact(this);
		}
	}
}

void APlr::Takedown()
{

	// Create line trace to make sure you are looking at enemy

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, GetReachLineStart(), GetReachLineEnd(), COLLISION_ENEMY, TraceParams))
	{
		// Check to see if player is currently carrying a weapon, and is not seen
		if (EquippedWeapon != NONE && !bIsSeen)
		{
			// Get reference to that enemy to use within code
			KillingEnemy = Cast<AEnemy>(Hit.GetActor());

			if (KillingEnemy)
			{
				UE_LOG(LogTemp, Warning, TEXT("TAKEDOWN ENEMY!!"));

				// Get Reference to enemy's controller to access behavior tree and blackboard
				AEnemyController* EnemyController = Cast<AEnemyController>(KillingEnemy->GetController());

				// Remove collision on enemy so animations work smoothly
				KillingEnemy->SetActorEnableCollision(false);

				// If player is crouching, make him stand for animations
				if (bIsCrouching)
				{
					ToggleCrouch();
				}

				// Call StartAttack function and input the location of player and enemy's takedown position for smooth movement
				StartAttack(GetActorLocation(), KillingEnemy->TakedownPosition->GetComponentLocation(), 0.5f);


				// Set Takedown on player and enemy to true so animation will play
				bTakeDown = true;
				EnemyController->BlackboardComponent->SetValue<UBlackboardKeyType_Bool>(TEXT("bTakeDown"), true);
				EnemyController->BlackboardComponent->SetValue<UBlackboardKeyType_Enum>(TEXT("PlayerWeapon"), EquippedWeapon);

				// Set TakeDownMove from a random number (1 or 2) to play a certain takedown, Making sure the enemy plays the corresponding takedown move animation aswell
				TakeDownMove = FMath::RandRange(1, 2);
				EnemyController->BlackboardComponent->SetValue<UBlackboardKeyType_Int>(TEXT("TakeDownMove"), TakeDownMove);

				// Stop movement and input from player during takedown to let the animation play correctly
				GetController()->StopMovement();
				DisableInput(GetWorld()->GetFirstPlayerController());

			}
		}
	}
}

void APlr::StartAttack(FVector StartLoc, FVector EndLoc, float Duration)
{
	// Set Attacking to true to make the movement happen through tick function
	bAttacking = true;

	// Set the start time and end time of the movement
	StartTime = GetWorld()->TimeSeconds;
	EndTime = Duration;

	// Set the start and end location of the movment
	StartPosition = StartLoc;
	EndPosition = EndLoc;

}

void APlr::AttackMovement()
{
	// Set so player can not interact with items during attack
	bCanInteract = false;

	// Find out the percentage of the completed movement
	float Time = GetWorld()->TimeSeconds - StartTime;
	float Percent = Time / EndTime;

	// Use Vector Interpololation to smoothly move player behind enemy 
	SetActorLocation(FMath::VInterpTo(StartPosition, EndPosition, Percent, AttackSpeed));

	// Get Player's rotation, and use rotator Interpolation to smoothly rotate player so he is looking at enemy
	FRotator PlayerRot = FRotationMatrix::MakeFromX(KillingEnemy->GetActorLocation() - GetActorLocation()).Rotator();
	GetController()->SetControlRotation(FMath::RInterpTo(GetActorRotation(), PlayerRot, Percent, AttackSpeed));
 
}

void APlr::KillEnemy()
{
	// Destroy enemy and remove reference to it
	KillingEnemy->Destroy();
	KillingEnemy = NULL;

	// Enable input back to player
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Double check to make sure attacking is false and player can interact again
	bAttacking = false;
	bCanInteract = true;
}



