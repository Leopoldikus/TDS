// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSHealthComponent.h"
#include "TDS_CharacterHealthComponent.generated.h"



/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);

UCLASS()
class TDS_API UTDS_CharacterHealthComponent : public UTDSHealthComponent
{
	GENERATED_BODY()

protected:

	float Shield = 100.0f;


public:

	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnShieldChange OnShieldChange;
	FTimerHandle TimerHandle_CoolDownShieldTimer;
	FTimerHandle TimerHandle_ShieldRecoveryRateTimer;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CoolDownShieldRecoveryTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldRecoveryValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldRecoveryRate = 0.1f;

	void ChangeHealthValue(float ChangeValue) override;
	float GetCurrentShield();
	void ChangeShieldValue(float ChangeValue);
	void CoolDownShieldEnd();
	void RecoveryShield();
};
