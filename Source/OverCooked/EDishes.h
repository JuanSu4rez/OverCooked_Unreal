#pragma once

#include "CoreMinimal.h"
#include "EDishes.generated.h"

UENUM(BlueprintType) 
enum class EDishes : uint8
{
	Pizza			UMETA(DisplayName = "Pizza"),	
	Salad			UMETA(DisplayName = "Salad"),
	SimpleBurger	UMETA(DisplayName = "SimpleBurger"),
	CheeseBurger	UMETA(DisplayName = "CheeseBurger"),
	CompleteBurger	UMETA(DisplayName = "CompleteBurger"),
	TomatoSoup		UMETA(DisplayName = "TomatoSoup"),
	OnionSoup 		UMETA(DisplayName = "OnionSoup"),
	MushroomSoup	UMETA(DisplayName = "MushroomSoup")
};