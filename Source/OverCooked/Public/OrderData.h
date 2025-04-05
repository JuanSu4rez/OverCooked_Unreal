#pragma once

#include "CoreMinimal.h"
#include "EDishes.h"
#include "OrderData.generated.h"

USTRUCT(BlueprintType)
struct OVERCOOKED_API FOrderData: public FTableRowBase
{
	GENERATED_BODY()
	
	// Dish Type (enum)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDishes Dish;

	// Icon Texture
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// Ingredients List
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> Ingredients;
};
