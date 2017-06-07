// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameData.generated.h"

UENUM(BlueprintType)
enum EWeapons
{
	NONE			UMETA(DisplayName = "None"),
	KITCHENKNIFE	UMETA(DisplayName = "Kitchen Knife")
};

UENUM(BlueprintType)
enum EInteractableTypes
{
	ENEMY			UMETA(DisplayName = "Enemy"),
	ITEM			UMETA(DisplayName = "Pickup Item"),
};
