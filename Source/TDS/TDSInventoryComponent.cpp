// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSInventoryComponent.h"
#include "TDS/Interface/TDS_IGameActor.h"
#include "TDS/Game/TDSGameInstance.h"
#pragma optimize ("", off)


// Sets default values for this component's properties
UTDSInventoryComponent::UTDSInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTDSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Find init weaponsSlots Init weapon
	for (int8 i = 0; i < WeaponSlots.Num(); i++)
	{
		UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
		if (myGI)
		{
			if (!WeaponSlots[i].NameItem.IsNone())
			{
				FWeaponInfo Info;
				if (myGI->GetWeaponInfoByName(WeaponSlots[i].NameItem, Info))
				{
					WeaponSlots[i].AdditionalInfo.Round = Info.MaxRound;
				}
			}
		}	
	}
	MaxSlotsWeapon = WeaponSlots.Num();

	if (WeaponSlots.IsValidIndex(0))
	{
		if (!WeaponSlots[0].NameItem.IsNone())
		{
			OnSwitchWeapon.Broadcast(WeaponSlots[0].NameItem, WeaponSlots[0].AdditionalInfo, 0);
		}
	}
}

// Called every frame
void UTDSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTDSInventoryComponent::SwitchWeaponToIndexByNextPreviousIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward)
{
	bool bIsSuccess = false;
	int8 CorrectIndex = ChangeToIndex;
	if (ChangeToIndex > WeaponSlots.Num() - 1)
	{
		CorrectIndex = 0;
	}
	else
	{
		if (ChangeToIndex < 0)
		{
			CorrectIndex = WeaponSlots.Num() - 1;
		}
	}

	FName NewIdWeapon;
	FAdditionalWeaponInfo NewAdditionalInfo;
	int32 NewCurrentIndex = 0;

	if (WeaponSlots.IsValidIndex(CorrectIndex))

	{
		if (!WeaponSlots[CorrectIndex].NameItem.IsNone())
		{
			if (WeaponSlots[CorrectIndex].AdditionalInfo.Round > 0)
			{
				//good weapon have start change
				bIsSuccess = true;
			}
			else
			{
				UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
				if (myGI)
				{
					//check ammoslots for this weapon
					FWeaponInfo myInfo;
					myGI->GetWeaponInfoByName(WeaponSlots[CorrectIndex].NameItem, myInfo);
					bool bIsFind = false;
					int8 j = 0;
					while (j < AmmoSlots.Num() && !bIsFind)
					{
						if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
						{
							//good waepon have ammo start change
							bIsSuccess = true;
							bIsFind = true;
						}
						j++;
					}
				}
			}
			if (bIsSuccess)
			{
				NewCurrentIndex = CorrectIndex;
				NewIdWeapon = WeaponSlots[CorrectIndex].NameItem;
				NewAdditionalInfo = WeaponSlots[CorrectIndex].AdditionalInfo;
			}
		}
	}
	if (!bIsSuccess)
	{
		int8 Iteration = 0;
		int8 SecondIteration = 0;
		int8 tmpIndex = 0;
		while (Iteration < WeaponSlots.Num() && !bIsSuccess)
		{

			Iteration++;
			if (bIsForward)
			{
				//SecondIterartion=0;
				tmpIndex = ChangeToIndex + Iteration;
			}
			else
			{
				SecondIteration = WeaponSlots.Num() - 1;
				tmpIndex = ChangeToIndex - Iteration;
			}
			if (WeaponSlots.IsValidIndex(tmpIndex))
			{
				if (!WeaponSlots[tmpIndex].NameItem.IsNone())
				{
					if (WeaponSlots[tmpIndex].AdditionalInfo.Round > 0)
					{
						//WeaponGood
						bIsSuccess = true;
						NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
						NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
						NewCurrentIndex = tmpIndex;
					}
					else
					{
						FWeaponInfo myInfo;
						UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
						myGI->GetWeaponInfoByName(WeaponSlots[tmpIndex].NameItem, myInfo);
						bool bIsFind = false;
						int8 j = 0;
						while (j < AmmoSlots.Num() && !bIsFind)
						{
							if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
							{
								//weapongood
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
								NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
								NewCurrentIndex = tmpIndex;
								bIsFind = true;
							}
							j++;
						}
					}
				}
			}
			else
			{
				//go to end of left array weapon slots
				if (OldIndex != SecondIteration)
				{
					if (WeaponSlots.IsValidIndex(SecondIteration))
					{
						if (!WeaponSlots[SecondIteration].NameItem.IsNone())
						{
							if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
							{
								//WeaponGood
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
								NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
								NewCurrentIndex = SecondIteration;								
							}
							else
							{
								FWeaponInfo myInfo;
								UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
								myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
								bool bIsFind = false;
								int8 j = 0;

								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
									{
										//WeaponGood
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
										NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
										NewCurrentIndex = SecondIteration;
										bIsFind = true;							
									}
									j++;
								}
							}
						}
					}
				}
				else
				{
					//go to same weapon when start
					if (WeaponSlots.IsValidIndex(SecondIteration))
					{
						if (!WeaponSlots[SecondIteration].NameItem.IsNone())
						{
							if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
							{
								//WeaponGood, it's the same weapon, do nothing
							}
							else
							{
								FWeaponInfo myInfo;
								UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
								myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
								bool bIsFind = false;
								int8 j = 0;
								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType)
									{
										if (AmmoSlots[j].Cout > 0)
										{
											//Weapon good, it's the same weapon, do thing
										}
										else
										{
											//Not find weapon with ammo, need init Pistol with infinite ammo
											UE_LOG(LogTemp, Error, TEXT("UTDSInventoruComponent::SwitchWeaponToIndex - Init PISTOL - NEED"));
										}
									}
									j++;
								}
							}
						}
					}
				}
				if (bIsForward)
				{
					SecondIteration++;
				}
				else
				{
					SecondIteration--;
				}
			}
		}
	}
	if (bIsSuccess)
	{
		SetAdditionalInfoWeapon(OldIndex, OldInfo);
		OnSwitchWeapon.Broadcast(NewIdWeapon, NewAdditionalInfo, NewCurrentIndex);
	}
	return bIsSuccess;
}

bool UTDSInventoryComponent::SwitchWeaponByIndex(int32 IndexWeaponToChange, int32 PreviousIndex, FAdditionalWeaponInfo PreviousWeaponInfo)
{
	bool bIsSuccess = false;
	FName ToSwitchIdWeapon;
	FAdditionalWeaponInfo ToSwitchAdditionalInfo;
	ToSwitchIdWeapon = GetWeaponNameBySlotIndex(IndexWeaponToChange);
	ToSwitchAdditionalInfo = GetAdditionalInfoWeapon(IndexWeaponToChange);
	if (!ToSwitchIdWeapon.IsNone())
	{
		SetAdditionalInfoWeapon(PreviousIndex, PreviousWeaponInfo);
		OnSwitchWeapon.Broadcast(ToSwitchIdWeapon, ToSwitchAdditionalInfo, IndexWeaponToChange);
		//check ammo slot for event to player
		EWeaponType ToSwitchWeaponType;
		if (GetWeaponTypeByNameWeapon(ToSwitchIdWeapon, ToSwitchWeaponType))
		{
			int8 AvailableAmmoForWeapon = -1;
			if (CheckAmmoForWeapon(ToSwitchWeaponType, AvailableAmmoForWeapon))
			{

			}
		}
		bIsSuccess = true;
	}
	return
		bIsSuccess;
}
FAdditionalWeaponInfo UTDSInventoryComponent::GetAdditionalInfoWeapon(int32 IndexWeapon)
{
	FAdditionalWeaponInfo result;
	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				result = WeaponSlots[i].AdditionalInfo;
				bIsFind = true;
			}
			i++;
		}
		if (!bIsFind)
		{
			UE_LOG(LogTemp, Warning, TEXT("UTDSInventoryComponent::SetAdditioanlInfoWeapon - Not Found Weapon with index -%d"), IndexWeapon);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTDSInventoryComponent::SetAdditioanlInfoWeapon - Not Correct index Weapon -%d"), IndexWeapon);
	}

	return result;
}

/*bool UTDSInventoryComponent::SwitchWeaponToIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward)
{
	bool bIsSuccess = false;//keep it
	int8 CorrectIndex = ChangeToIndex;
	if (ChangeToIndex > WeaponSlots.Num() - 1)
	{
		CorrectIndex = 0;
	}
	else
	
		if (ChangeToIndex < 0)
		{
			CorrectIndex = WeaponSlots.Num() - 1;
		}

		FName NewIdWeapon;
		FAdditionalWeaponInfo NewAdditionalInfo;
		int32 NewCurrentIndex = 0;
	
		if (WeaponSlots.IsValidIndex(CorrectIndex))
		{
			if (!WeaponSlots[CorrectIndex].NameItem.IsNone())
			{
				if (WeaponSlots[CorrectIndex].AdditionalInfo.Round > 0)
				{
					bIsSuccess = true;
				}
				else
				{
					UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
					if (myGI)
					{
						FWeaponInfo myInfo;
						myGI->GetWeaponInfoByName(WeaponSlots[CorrectIndex].NameItem, myInfo);
						bool bIsFind = false;
						int8 j = 0;
						while (j < AmmoSlots.Num() && !bIsFind)
						{
							if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
							{
								bIsSuccess = true;
								bIsFind = true;
								//break;  // Exit loop immediately after finding ammo
							}
							j++;
						}
					}
				}

				if (bIsSuccess)  // Ensure it has a valid value
				{
					NewCurrentIndex = CorrectIndex;
					NewIdWeapon = WeaponSlots[CorrectIndex].NameItem;
					NewAdditionalInfo = WeaponSlots[CorrectIndex].AdditionalInfo;
				}
			}
		}

		if (!bIsSuccess)
		{
			if (bIsForward)
			{
				int8 Iteration = 0;
				int8 SecondIteration = 0;
				while (Iteration < WeaponSlots.Num() && !bIsSuccess)
				{
					Iteration++;
					int8 tmpIndex = ChangeToIndex + Iteration;
					if (WeaponSlots.IsValidIndex(tmpIndex))
					{
						if (!WeaponSlots[tmpIndex].NameItem.IsNone())
						{
							if (WeaponSlots[tmpIndex].AdditionalInfo.Round > 0)
							{
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
								NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
								NewCurrentIndex = tmpIndex;
							}
							else
							{
								FWeaponInfo myInfo;
								UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
								myGI->GetWeaponInfoByName(WeaponSlots[tmpIndex].NameItem, myInfo);
								bool bIsFind = false;
								int8 j = 0;
								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
									{
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
										NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
										NewCurrentIndex = tmpIndex;
										bIsFind = true;
									}
									j++;
								}
							}
						}
					}
					else
					{
						if (OldIndex != SecondIteration)
						{
							if (WeaponSlots.IsValidIndex(SecondIteration))
							{
								if (!WeaponSlots[SecondIteration].NameItem.IsNone())
								{
									if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
									{
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
										NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
										NewCurrentIndex = SecondIteration;
									}
									else
									{
										FWeaponInfo myInfo;
										UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
										myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
										bool bIsFind = false;
										int8 j = 0;
										while (j < AmmoSlots.Num() && !bIsFind)
										{
											if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
											{
												bIsSuccess = true;
												NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
												NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
												NewCurrentIndex = SecondIteration;
												bIsFind = true;
											}
											j++;
										}
									}
								}
							}
						}
						else
						{
							if (WeaponSlots.IsValidIndex(SecondIteration))
							{
								if (!WeaponSlots[SecondIteration].NameItem.IsNone())
								{
									if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
									{
										//WeaponGood, it is same weapon do nothing
									}
									else
									{
										FWeaponInfo myInfo;
										UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
										myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
										bool bIsFind = false;
										int8 j = 0;
										while (j < AmmoSlots.Num() && !bIsFind)
										{
											if (AmmoSlots[j].WeaponType == myInfo.WeaponType)
											{
												if (AmmoSlots[j].Cout > 0)
												{
													//WeaponGood, it is same weapon do nothing
												}
												else
												{
													//Nor find weapon with ammo need init Pistol with infinitive ammo
													UE_LOG(LogTemp, Error, TEXT("UTDSInventoryComponent::SwitchWeaponToIndex - Init Pistol - NEED"));
												}
											}
											j++;
										}
									}
								}
							}
						}
						SecondIteration++;
					}
				}
			}
			else
			{
				int8 Iteration = 0;
				int8 SecondIteration = WeaponSlots.Num() - 1;
				while (Iteration < WeaponSlots.Num() && !bIsSuccess)
				{
					Iteration++;
					int8 tmpIndex = ChangeToIndex - Iteration;
					if (WeaponSlots.IsValidIndex(tmpIndex))
					{
						if (!WeaponSlots[tmpIndex].NameItem.IsNone())
						{
							if (WeaponSlots[tmpIndex].AdditionalInfo.Round > 0)
							{
								//Weapon Good
								bIsSuccess = true;
								NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
								NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
								NewCurrentIndex = tmpIndex;
							}
							else
							{
								FWeaponInfo myInfo;
								UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
								myGI->GetWeaponInfoByName(WeaponSlots[tmpIndex].NameItem, myInfo);
								bool bIsFind = false;
								int8 j = 0;
								while (j < AmmoSlots.Num() && !bIsFind)
								{
									if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
									{
										//WeaponGood
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[tmpIndex].NameItem;
										NewAdditionalInfo = WeaponSlots[tmpIndex].AdditionalInfo;
										NewCurrentIndex = tmpIndex;
										bIsFind = true;
									}
									j++;
								}
							}
						}
					}
					else
					{
						//go to end of LEFT of array wepon slots
						if (OldIndex != SecondIteration)
						{
							if (!WeaponSlots.IsValidIndex(SecondIteration))
							{
								if (!WeaponSlots[SecondIteration].NameItem.IsNone())
								{
									if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
									{
										//WeaponGood
										bIsSuccess = true;
										NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
										NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
										NewCurrentIndex = SecondIteration;
									}
									else
									{
										FWeaponInfo myInfo;
										UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
										myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
										bool bIsFind = false;
										int8 j = 0;
										while (j < AmmoSlots.Num() && !bIsFind)
										{
											if (AmmoSlots[j].WeaponType == myInfo.WeaponType && AmmoSlots[j].Cout > 0)
											{
												//WeaponGood
												bIsSuccess = true;
												NewIdWeapon = WeaponSlots[SecondIteration].NameItem;
												NewAdditionalInfo = WeaponSlots[SecondIteration].AdditionalInfo;
												NewCurrentIndex = SecondIteration;
												bIsFind = true;
											}
											j++;
										}
									}
								}
							}
						}
						else
							//go to same weapon when start
							if (WeaponSlots.IsValidIndex(SecondIteration))
							{
								if (!WeaponSlots[SecondIteration].NameItem.IsNone())
								{
									if (WeaponSlots[SecondIteration].AdditionalInfo.Round > 0)
									{
										//Weapongood, it is same weapon do nothing
									}
									else
									{
										FWeaponInfo myInfo;
										UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
										myGI->GetWeaponInfoByName(WeaponSlots[SecondIteration].NameItem, myInfo);
										bool bIsFind = false;
										int8 j = 0;
										while (j < AmmoSlots.Num() && !bIsFind)
										{
											if (AmmoSlots[j].WeaponType == myInfo.WeaponType)
											{
												if (AmmoSlots[j].Cout > 0)
												{
													//WeaponGood it's the same weapon do nothing
												}
												else
												{
													//Not find weapon with amm need Pistol with inf ammo
													UE_LOG(LogTemp, Error, TEXT("UTDSInventoryComponent::SwitchWeaponToIndex - Init PISTIL - NEED"));
												}
											}
											j++;
										}
									}
								}
							}
					}
					SecondIteration--;
				}
			}
		}
		if (bIsSuccess)
		{
			SetAdditionalInfoWeapon(OldIndex, OldInfo);
			OnSwitchWeapon.Broadcast(NewIdWeapon, NewAdditionalInfo, NewCurrentIndex);
			//OnWeaponAmmoAviable.Broadcast();
		}
		return bIsSuccess;
}
*/

int32 UTDSInventoryComponent::GetWeaponIndexSlotByname(FName IdWeaponName)
{
	int32 result = -1;
	int8 i = 0;
	bool bIsFind = false;
	while (i < WeaponSlots.Num() && !bIsFind)
	{
		if (WeaponSlots[i].NameItem == IdWeaponName)
		{
			bIsFind = true;
			result = i;
		}
		i++;
	}
	return result;
}

FName UTDSInventoryComponent::GetWeaponNameBySlotIndex(int32 indexSlot)
{
	FName result;
	if (WeaponSlots.IsValidIndex(indexSlot))
	{
		result = WeaponSlots[indexSlot].NameItem;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTDSInventoryComponent::GetWeaponNameBySlotIndex - Not Correct index Weapon -%d"), indexSlot);
	}
	return result;
}

bool UTDSInventoryComponent::GetWeaponTypeByIndexSlot(int32 IndexSlot, EWeaponType& WeaponType)
{
	bool bIsFind = false;
	FWeaponInfo OutInfo;
	WeaponType = EWeaponType::RifleType;
	UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
	if (myGI)
	{
		if (WeaponSlots.IsValidIndex(IndexSlot))
		{
			myGI->GetWeaponInfoByName(WeaponSlots[IndexSlot].NameItem, OutInfo);
			WeaponType = OutInfo.WeaponType;
			bIsFind = true;
		}
	}
	return bIsFind;
}

bool UTDSInventoryComponent::GetWeaponTypeByNameWeapon(FName IdWeaponName, EWeaponType& WeaponType)
{
	bool bIsFind = false;
	FWeaponInfo OutInfo;

	WeaponType = EWeaponType::RifleType;
	UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
	if (myGI)

	{
		myGI->GetWeaponInfoByName(IdWeaponName, OutInfo);
		WeaponType = OutInfo.WeaponType;
		bIsFind = true;
	}
	return bIsFind;
}

void UTDSInventoryComponent::SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo)
{
	if (WeaponSlots.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlots.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				WeaponSlots[i].AdditionalInfo = NewInfo;
				bIsFind = true;
				OnWeaponAdditionalInfoChange.Broadcast(IndexWeapon, NewInfo);
			}
			i++;
		}
		if (!bIsFind)
		{
			UE_LOG(LogTemp, Warning, TEXT("UTDSInventoryComponent::SetAdditioanlInfoWeapon - Not Found Weapon with index -%d"), IndexWeapon);
		}
	}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UTDSInventoryComponent::SetAdditioanlInfoWeapon - Not Correct index Weapon -%d"), IndexWeapon);
		}
}

void UTDSInventoryComponent::AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo)
{
	bool bIsFind = false;
	int8 i = 0;
	while (i < AmmoSlots.Num() && !bIsFind)
	{
		if (AmmoSlots[i].WeaponType == TypeWeapon)
		{
			AmmoSlots[i].Cout += CoutChangeAmmo;
			if (AmmoSlots[i].Cout > AmmoSlots[i].MaxCout)
			{
				AmmoSlots[i].Cout = AmmoSlots[i].MaxCout;
			}
			OnAmmoChange.Broadcast(AmmoSlots[i].WeaponType, AmmoSlots[i].Cout);
			bIsFind = true;
		}
		i++;
	}
}

bool UTDSInventoryComponent::CheckAmmoForWeapon(EWeaponType TypeWeapon, int8& AvailableAmmoForWeapon)
{
	AvailableAmmoForWeapon = 0;
	bool bIsFind = false;
	int8 i = 0;
	while (i < AmmoSlots.Num() && !bIsFind)
	{
		if (AmmoSlots[i].WeaponType == TypeWeapon)
		{
			bIsFind = true;
			AvailableAmmoForWeapon = AmmoSlots[i].Cout;
			if (AmmoSlots[i].Cout > 0)
			{
				return true;
			}
		}
		i++;
	}
	if (AvailableAmmoForWeapon <= 0)
	{
	    OnWeaponAmmoEmpty.Broadcast(TypeWeapon);
	}
	else
	{
		OnWeaponAmmoAvailable.Broadcast(TypeWeapon);
	}
	return false;
}

bool UTDSInventoryComponent::CheckCanTakeAmmo(EWeaponType AmmoType)
{
	bool result = false;
	int8 i = 0;
	while (i < AmmoSlots.Num() && !result)
	{
		if (AmmoSlots[i].WeaponType == AmmoType && AmmoSlots[i].Cout < AmmoSlots[i].MaxCout)
		{
			result = true;
		}
		i++;
	}
	return result;
}

bool UTDSInventoryComponent::CheckCanTakeWeapon(int32& FreeSlot)
{
	bool bIsFreeSlot = false;
	int8 i = 0;
	while (i < WeaponSlots.Num() && !bIsFreeSlot)
	{
		if (WeaponSlots[i].NameItem.IsNone())
		{
			bIsFreeSlot = true;
			FreeSlot = i;
		}
		i++;
	}
	return bIsFreeSlot;
}

bool UTDSInventoryComponent::SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem& DropItemInfo)
{
	bool result = false;
	if (WeaponSlots.IsValidIndex(IndexSlot) && GetDropItemInfoFromInventory(IndexSlot, DropItemInfo))
	{
		WeaponSlots[IndexSlot] = NewWeapon;
		SwitchWeaponToIndexByNextPreviousIndex(CurrentIndexWeaponChar, -1, NewWeapon.AdditionalInfo, true);
		OnUpdateWeaponSlots.Broadcast(IndexSlot, NewWeapon);
		result = true;
	}
	return result;
}

bool UTDSInventoryComponent::TryGetWeaponToInventory(FWeaponSlot NewWeapon)
{
	int32 indexSlot = -1;
	if (CheckCanTakeWeapon(indexSlot))
	{
		if (WeaponSlots.IsValidIndex(indexSlot))
		{
			WeaponSlots[indexSlot] = NewWeapon;
			OnUpdateWeaponSlots.Broadcast(indexSlot, NewWeapon);
			return true;
		}
	}
	return false;
}

void UTDSInventoryComponent::DropWeaponByIndex(int32 ByIndex, FDropItem& DropItemInfo)
{
	FWeaponSlot EmptyWeaponSlot;
	bool bIsCanDrop = false;
	int8 i = 0;
	int8 AvailableWeaponNum = 0;
	while (i < WeaponSlots.Num() && !bIsCanDrop)
	{
		if (!WeaponSlots[i].NameItem.IsNone())
		{
			AvailableWeaponNum++;
			if (AvailableWeaponNum > 1)
			{
				bIsCanDrop = true;
			}
		}
		i++;
		if (bIsCanDrop && WeaponSlots.IsValidIndex(ByIndex) && GetDropItemInfoFromInventory(ByIndex, DropItemInfo))
		{
			GetDropItemInfoFromInventory(ByIndex, DropItemInfo);
			//switch weapon to valid slot weapon from start weapon slots array
			bool bIsFindWeapon = false;
			int8 j = 0;

			while (j < WeaponSlots.Num() && !bIsFindWeapon)
			{
				if (!WeaponSlots[j].NameItem.IsNone())
				{
					OnSwitchWeapon.Broadcast(WeaponSlots[j].NameItem, WeaponSlots[j].AdditionalInfo, j);
				}
				j++;
			}
			WeaponSlots[ByIndex] = EmptyWeaponSlot;
			if (GetOwner()->GetClass()->ImplementsInterface(UTDS_IGameActor::StaticClass()))
			{
				ITDS_IGameActor::Execute_DropWeaponToWorld(GetOwner(), DropItemInfo);
			}
			OnUpdateWeaponSlots.Broadcast(ByIndex, EmptyWeaponSlot);
		}
	}
}

bool UTDSInventoryComponent::GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem &DropItemInfo)
{
	bool result = false;
	FName DropItemName = GetWeaponNameBySlotIndex(IndexSlot);
	UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetWorld()->GetGameInstance());
	if (myGI)
	{
		result = myGI->GetDropItemInfoByWeaponName(DropItemName, DropItemInfo);
		if (WeaponSlots.IsValidIndex(IndexSlot))
		{
			DropItemInfo.WeaponInfo.AdditionalInfo = WeaponSlots[IndexSlot].AdditionalInfo;
		}
	}
	return result;
}

#pragma optimize ("", on)

