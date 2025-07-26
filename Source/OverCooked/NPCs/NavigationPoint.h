#pragma once

#include "CoreMinimal.h"
#include "EPatrolPointType.h"
#include "NavigationPoint.generated.h"

USTRUCT(BlueprintType)
struct OVERCOOKED_API FNavigationPoint
{
	GENERATED_BODY()

	FNavigationPoint()
	   : Location(FVector::ZeroVector)
	   , Type(EPatrolPointType::Extreme) {}

	FNavigationPoint(FVector Location, EPatrolPointType Type)
		: Location(Location), Type(Type){}
	
	// Point in space (x,y)
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;

	// Intersection or Extreme (Extreme will be the start of the path)
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPatrolPointType Type;

		bool IsExtreme() const
		{
			return Type == EPatrolPointType::Extreme;
		}
};


