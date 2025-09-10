// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TDS_IGameActor.h"
#include "TDS_StateEffect.h"
#include "TDS_EnvironmentStructure.generated.h"

UCLASS()
class TDS_API ATDS_EnvironmentStructure : public AActor, public ITDS_IGameActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATDS_EnvironmentStructure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	EPhysicalSurface GetSurfuceType() override;

	TArray<UTDS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTDS_StateEffect* RemoveEffect)override;
	void AddEffect(UTDS_StateEffect* newEffect)override;

	//Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<UTDS_StateEffect*> Effects;
};
