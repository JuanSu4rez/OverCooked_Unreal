#pragma once

#include "CoreMinimal.h"
#include "EDishes.generated.h"

UENUM(BlueprintType) 
enum class EIngredientTasks : uint8
{	
	Chopping		UMETA(DisplayName = "Chopping"),	
	Frying			UMETA(DisplayName = "Frying"),
	Cooking			UMETA(DisplayName = "Cooking"),
	Baking			UMETA(DisplayName = "Baking"),
	Mixing			UMETA(DisplayName = "Mixing"),
	Blending		UMETA(DisplayName = "Blending"),
	NoTask			UMETA(DisplayName = "NoTask")
};