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

	UFUNCTION()
	TArray<AActor*> FindNPCSpawnPoints() const;

	// UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	UFUNCTION()
	TArray<AActor*> FindAllNavigationPoints();	
	
};
