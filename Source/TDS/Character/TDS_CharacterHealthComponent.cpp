// Fill out your copyright notice in the Description page of Project Settings.


#include "TDS_CharacterHealthComponent.h"



void UTDS_CharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{
	float CurrentDamage = ChangeValue * CoefDamage;
	

	if (Shield > 0.0f && ChangeValue < 0.0f)
	{
		ChangeShieldValue(ChangeValue);

		if (Shield < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("UTDS_CharacterHealthComponent::ChangeHealthValue - Shield < 0"));

			//FX
		}
	}
	else
	{
		Super::ChangeHealthValue(ChangeValue);
	}
	
		
}

float UTDS_CharacterHealthComponent::GetCurrentShield()
{
	return Shield;
}

void UTDS_CharacterHealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;
	OnShieldChange.Broadcast(Shield, ChangeValue);

	if (Shield > 100.0f)
	{
		Shield = 100.0f;
	}
	else
	{
		if (Shield < 0.0f)
		{
			Shield = 0.0f;
		}
	}
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CoolDownShieldTimer, this, &UTDS_CharacterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecoveryTime, false);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}
}

void UTDS_CharacterHealthComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UTDS_CharacterHealthComponent::RecoveryShield, ShieldRecoveryRate, true);
	}
}

void UTDS_CharacterHealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoveryValue;
	if (tmp > 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
		}
	}
	else
	{
		Shield = tmp;
	}
	OnShieldChange.Broadcast(Shield, ShieldRecoveryValue);

}
	


