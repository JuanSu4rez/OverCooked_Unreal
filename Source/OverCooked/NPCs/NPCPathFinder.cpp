#include "NPCPathFinder.h"

#include "NavigationPoint.h"
#include "Kismet/GameplayStatics.h"


UNPCPathFinder::UNPCPathFinder()
{
}

TArray<AActor*> UNPCPathFinder::FindNPCSpawnPoints() const
{
	TArray<AActor*> allNavigationPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigationPoint::StaticClass(), allNavigationPoints);
	
	TArray<AActor*> allExtremes;
	for (AActor* Actor : allNavigationPoints)
	{
		ANavigationPoint* Point = Cast<ANavigationPoint>(Actor);
		if (Point && Point->IsExtreme())  // Using your IsExtreme() function
		{
			allExtremes.Add(Point);
		}
	}
	return allExtremes;
	
}

TArray<AActor*> UNPCPathFinder::FindAllNavigationPoints()
{
	TArray<AActor*> allNavigationPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigationPoint::StaticClass(), allNavigationPoints);
	return allNavigationPoints;
}

