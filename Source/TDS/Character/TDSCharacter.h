// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDS/FunctionLibrary/Types.h"
#include "TDS/WeaponDefault.h"
#include "TDS/TDSInventoryComponent.h"
#include "TDS/Character/TDS_CharacterHealthComponent.h"
#include "TDS/Interface/TDS_IGameActor.h"
#include "TDS/TDS_StateEffect.h"
#include "TDSCharacter.generated.h"



UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter, public ITDS_IGameActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	//Inputs
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	
	
	//Inventory Inputs
	void TrySwitchNextWeapon();
	void TrySwitchPreviousWeapon();

	//Ability Inputsc
	UFUNCTION()
	void TryAbilityEnabled();

	template<int32 Id>
	void TKeyPressed()
	{
		TrySwitchWeaponToIndexByKeyInput(Id);
	}
	//inputs end


	//input flags
	float AxisX = 0.0f;
	float AxisY = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementSpeedInfo;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	bool SprintRunEnable = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnable = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnable = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	float Stamina = 80.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health")
	bool bIsAlive = true;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health")
	TArray<UAnimMontage*> DeadsAnim;
		
	AWeaponDefault* CurrentWeapon = nullptr;
	UDecalComponent* CurrentCursor = nullptr;
	TArray <UTDS_StateEffect*> Effects;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentIndexWeapon = 0;

	UFUNCTION()
	void CharDead();
	UFUNCTION()
	void EnableRagDoll();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



public:
	ATDSCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UTDSInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	class UTDS_CharacterHealthComponent* CharHealthComponent;

	//Cursor material on decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	//Default move rule and state character

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UTDS_StateEffect> AbilityEffect;


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	
public:
	
	// Tick function
	UFUNCTION()
	void MovementTick(float DeltaTime);

	//Func
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);

	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION(BlueprintCallable)
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSucces, int32 AmmoSafe);
	UFUNCTION(BlueprintCallable)
	bool TrySwitchWeaponToIndexByKeyInput(int32 ToIndex);
	UFUNCTION(BlueprintCallable)
	void DropCurrentWeapon();

	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);
	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDecalComponent* GetCursorToWorld();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMovementState GetMovementState();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTDS_StateEffect*> GetCurrentEffectsOnChar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentWeaponIndex();
	
	 
	 //Interface
	EPhysicalSurface GetSurfaceType() override;
	TArray<UTDS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTDS_StateEffect* RemoveEffect) override;
	void AddEffect(UTDS_StateEffect* newEffect) override;
	 //End Interface

	UFUNCTION(BlueprintNativeEvent)
	void CharDead_BP();
	
	

};

