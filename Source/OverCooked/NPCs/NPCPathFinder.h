#pragma once

#include "CoreMinimal.h"
#include "NPCPathFinder.generated.h"
class ANavigationPoint;


UCLASS()
class OVERCOOKED_API UNPCPathFinder : public UObject
{
	GENERATED_BODY()

public:
	
	UNPCPathFinder();

	// UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	UFUNCTION()
	TArray<AActor*> GetAllNavigationPoints() const;

	UFUNCTION()
	TArray<AActor*> GetExtremePoints() const;

	UFUNCTION()
	TArray<AActor*> GetIntersectionNpcsPoints() const;

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	AActor* GetRandomSpawningPoint();

	AActor* GetBestIntersectionPoint(AActor* SpawningPoint) const;

	TArray<AActor*> GetNavigationPath(AActor* StartPoint) const;

private:

	bool ArePointsAligned(const AActor* SpawninPoint, const AActor* IntersectionPoint) const;

	TArray<AActor*> FindNavigationPointsWithFilter(TFunction<bool(const ANavigationPoint*)> Filter) const;

	AActor* GetRandomValidFinalPoint(const AActor* StartingPoint, const AActor* ChosenIntersectionPoint) const;

};
