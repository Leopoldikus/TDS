// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDS/FunctionLibrary/Types.h"
#include "TDS/WeaponDefault.h"
#include "TDS/TDSInventoryComponent.h"
#include "TDSCharacter.generated.h"



// USTRUCT(BlueprintType)
// struct FCharacterSpeedInfo
// {
// GENERATED_BODY()
// };

UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;


public:
	ATDSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTDSInventoryComponent* InventoryComponent;
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UDecalComponent* CursorToWorld;

public:
	//Cursor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	//Movement
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

	//Weapon
	AWeaponDefault* CurrentWeapon = nullptr;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Demo")
			
	UDecalComponent* CurrentCursor = nullptr;
		

	//Inputs
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	float AxisX = 0.0f;
	float AxisY = 0.0f;
	// Tick function
	UFUNCTION()
	void MovementTick(float DeltaTime);

	//Func
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();

	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
	void RemoveCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSucces, int32 AmmoSafe);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);

	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);


	UFUNCTION(BlueprintCallable)
	 UDecalComponent* GetCursorToWorld();	

	 //Inventory Func
	 void TrySwitchNextWeapon();
	 void TrySwitchPreviosWeapon();

	 UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	 int32 CurrentIndexWeapon = 0;

};

