// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPHasMesh.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPHasMesh : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSPROJECT_API IFPHasMesh
{
	GENERATED_BODY()

public:
	virtual UMeshComponent* GetMainMesh() = 0;
};
