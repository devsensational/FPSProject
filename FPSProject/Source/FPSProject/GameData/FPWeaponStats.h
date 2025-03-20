#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon/FPWeaponBase.h"
#include "FPWeaponStats.generated.h"

enum class EFPWeaponType : uint8;

USTRUCT(BlueprintType)
struct FPWeaponStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPWeaponStats()
		: Name(NAME_None)
		, Type(EFPWeaponType::WT_None) 
		, Damage(0.0f)
		, Accuracy(0.0f)
		, RPM(0.0f)
		, Price(0)
		, ReloadTime(0.0f)
		, MaxAmmo(0)
		, MaxRemainingAmmo(0)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	EFPWeaponType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Accuracy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float RPM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float ReloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 MaxRemainingAmmo;

};
