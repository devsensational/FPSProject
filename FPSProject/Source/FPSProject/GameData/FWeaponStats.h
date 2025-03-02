#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FWeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Accuracy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Magazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float RPM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 Price;
};
