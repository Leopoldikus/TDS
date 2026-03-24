// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileDefault_Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"


int32 DebugExplodeShow = 0;
FAutoConsoleVariableRef CVAREExplodeShow(TEXT("TDS.DebugExplode"), DebugExplodeShow, TEXT("Draw Debug for Explode"), ECVF_Cheat);


void AProjectileDefault_Grenade::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileDefault_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
}

void AProjectileDefault_Grenade::TimerExplose(float DeltaTime)
{
	if (TimerEnable)
	{
		if (TimerToExplose > TimeToExplose)
		{
			Explose();
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefault_Grenade::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!TimerEnable)
	{
		Explose();
	}
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileDefault_Grenade::ImpactProjectile()
{
	TimerEnable = true;
}

void AProjectileDefault_Grenade::Explose()
{

	if (DebugExplodeShow)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMinRadiusDamage, 12, FColor::Green, false, 12.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 12, FColor::Red, false, 12.f);

	}
	TimerEnable = false;
	if (ProjectileSetting.ExploseFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));
	}

	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
	}

	
	TArray<AActor*> FinalIgnoreList = ActorsToIgnore;
	FinalIgnoreList.Add(this);
	if (GetInstigator())
	{
		FinalIgnoreList.AddUnique(GetInstigator());
	}

	// Логируем для уверенности перед самым выстрелом функции урона
	UE_LOG(LogTemp, Warning, TEXT("=== ВЗРЫВ: Игнорируем %d акторов ==="), FinalIgnoreList.Num());

	// Используем ApplyRadialDamageWithFalloff для более точной настройки
	// Если ApplyRadialDamage не работает, попробуем этот метод.

	bool bApplied = UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		ProjectileSetting.ExploseMaxDamage, 10.0f, GetActorLocation(), 100.0f, 500.0f, 1.0f, UDamageType::StaticClass(), FinalIgnoreList, 
		this,  GetInstigatorController(),          
		ECC_Visibility                
	);


	
	//TArray<AActor*> IgnoreActor;
	//UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ProjectileSetting.ExploseMaxDamage, ProjectileSetting.ExploseMaxDamage * 0.2f,
		//GetActorLocation(), 1000.0f, 2000.0f, 5, NULL, IgnoreActor, this, nullptr);

	this->Destroy();
}







