#include "NPCPathFinder.h"

#include "NavigationPoint.h"
#include "Kismet/GameplayStatics.h"


UNPCPathFinder::UNPCPathFinder()
{
}

TArray<AActor*> UNPCPathFinder::FindNavigationPointsWithFilter(TFunction<bool(const ANavigationPoint*)> Filter) const
{
	TArray<AActor*> allNavigationPoints = GetAllNavigationPoints();

	TArray<AActor*> filteredPoints;
	for (AActor* Actor : allNavigationPoints)
	{
		ANavigationPoint* Point = Cast<ANavigationPoint>(Actor);
		if (Point && Filter(Point))
		{
			filteredPoints.Add(Point);
		}
	}
	return filteredPoints;
}

AActor* UNPCPathFinder::GetBestIntersectionPoint(AActor* SpawningPoint) const
{
	AActor* IntersectionPointChosen = nullptr;
	float DistanceBetweenPoints = 0;
	float BestDistance = 0;

	auto IntersectionPoints = GetIntersectionNpcsPoints();

	for (auto IntersectionPoint : IntersectionPoints)
	{
		if (ArePointsAligned(SpawningPoint, IntersectionPoint))
		{
			DistanceBetweenPoints = FVector::Distance(SpawningPoint->GetActorLocation(), IntersectionPoint->GetActorLocation());

			if (DistanceBetweenPoints > BestDistance)
			{
				BestDistance = DistanceBetweenPoints;
				IntersectionPointChosen = IntersectionPoint;
			}
		}
	}
	return IntersectionPointChosen;
}

TArray<AActor*> UNPCPathFinder::GetNavigationPath(AActor* StartPoint) const
{
	auto IntersectionPoint = GetBestIntersectionPoint(StartPoint);
	auto FinalPoint = GetRandomValidFinalPoint(StartPoint, IntersectionPoint);

	TArray<AActor*> PathPoints = {IntersectionPoint, FinalPoint};
	return PathPoints;
}

/*
 * Calculates the final point for the NPC Path, selecting randomly between the 2 best choices:
 * Either retrieves a point to complete a straight path end to end
 * Or retrieves a point to generate an L-Shaped path
 */
AActor* UNPCPathFinder::GetRandomValidFinalPoint(const AActor* StartingPoint, const AActor* ChosenIntersectionPoint) const
{
	auto allExtremePoints = GetExtremePoints();
	AActor* ExtremePointStraightPath = nullptr;
	AActor* ExtremePointLShapedPath = nullptr;
	float DistanceBetweenPoints = 0;
	float BestDistance = 0;

	for (auto extremePoint : allExtremePoints)
	{
		if (StartingPoint == extremePoint)
			continue;
		//get the extreme aligned with spawning point and intersectionPoint (Straight path, end to end)
		if (ArePointsAligned(extremePoint, ChosenIntersectionPoint) && ArePointsAligned(extremePoint, StartingPoint))
		{
			ExtremePointStraightPath = extremePoint;
		}
		//get the furthest extreme aligned to the chosen intersection point (L-shaped path)
		if (ArePointsAligned(extremePoint, ChosenIntersectionPoint))
		{
			DistanceBetweenPoints = FVector::Distance(extremePoint->GetActorLocation(), ChosenIntersectionPoint->GetActorLocation());
			if (DistanceBetweenPoints > BestDistance)
			{
				BestDistance = DistanceBetweenPoints;
				ExtremePointLShapedPath = extremePoint;
			}
		}
	}

	TArray<AActor*> ValidExtremePoints = {ExtremePointStraightPath, ExtremePointLShapedPath};
	return ValidExtremePoints[FMath::RandRange(0, ValidExtremePoints.Num() - 1)];
}


TArray<AActor*> UNPCPathFinder::GetExtremePoints() const
{
	return FindNavigationPointsWithFilter([](const ANavigationPoint* Point) {
		return Point->IsExtreme();
	});
}

TArray<AActor*> UNPCPathFinder::GetIntersectionNpcsPoints() const
{
	return FindNavigationPointsWithFilter([](const ANavigationPoint* Point) {
	  return !Point->IsExtreme();
  });
}

TArray<AActor*> UNPCPathFinder::GetAllNavigationPoints() const
{
	TArray<AActor*> allNavigationPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigationPoint::StaticClass(), allNavigationPoints);
	return allNavigationPoints;
}

AActor* UNPCPathFinder::GetRandomSpawningPoint()
{
	TArray<AActor*> spawningPoints = GetExtremePoints();
	return spawningPoints[FMath::RandRange(0, spawningPoints.Num() - 1)];
}

bool UNPCPathFinder::ArePointsAligned(const AActor* SpawninPoint, const AActor* IntersectionPoint) const
{
	return
		(int)SpawninPoint->GetActorLocation().X == (int)IntersectionPoint->GetActorLocation().X
		||
		(int)SpawninPoint->GetActorLocation().Y == (int)IntersectionPoint->GetActorLocation().Y;
}
