#pragma once

#include "CoreMinimal.h"
#include "EPatrolPointType.h"
#include "NavigationPoint.generated.h"


UCLASS(BlueprintType)
class OVERCOOKED_API ANavigationPoint: public AActor
{
	GENERATED_BODY()

public: 
	ANavigationPoint();
	virtual void BeginPlay() override;

	// Intersection or Extreme (Extreme will be the start of the path)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPatrolPointType Type;

	UFUNCTION(BlueprintCallable)
	bool IsExtreme() const;
};