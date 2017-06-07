// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateCore.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "TextWidget.generated.h"

/**
 * 
 */
UCLASS()
class CG2_API UTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:


	/** Reference to the Interact text **/
	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* GetInteractText() const;
	
	
};
