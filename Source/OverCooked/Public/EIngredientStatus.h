#pragma once

#include "CoreMinimal.h"
#include "EDishes.generated.h"

UENUM(BlueprintType) 
enum class EIngredientStatus : uint8
{	
	Raw				UMETA(DisplayName = "Raw"),	
	Cooked			UMETA(DisplayName = "Cooked"),
	Burned			UMETA(DisplayName = "Burned"),
	OnFire			UMETA(DisplayName = "OnFire"),
	Ready			UMETA(DisplayName = "Ready")	
};