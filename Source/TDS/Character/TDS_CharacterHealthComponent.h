// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSHealthComponent.h"
#include "TDS_CharacterHealthComponent.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API UTDS_CharacterHealthComponent : public UTDSHealthComponent
{
	GENERATED_BODY()

public:

	
	void ChangeCurrentHealth(float ChangeValue) override;
	
};
