#include "NPCsManager.h"

#include "NavigationPoint.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"

ANPCsManager::ANPCsManager()
{
	CurrentNPCCount = 0;	
}

void ANPCsManager::CreateNPC()
{
	if (NPCUnityType)
	{		
		UE_LOG(LogTemp, Warning, TEXT("================================= Creating NPC =================================") );
		auto spawningPoint = GetRandomSpawningPoint();
		FString PointType = spawningPoint->IsExtreme() ? TEXT("Extreme") : TEXT("Intersection");
		FVector Location = spawningPoint->GetActorLocation();
		UE_LOG(LogTemp, Warning, 
				TEXT("Spawning NPC at: %s (Type: %s, Location: X=%.2f, Y=%.2f, Z=%.2f)"), 
				*spawningPoint->GetName(), 
				*PointType,
				Location.X, Location.Y, Location.Z);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FTransform SpawnTransform = FTransform(Location);

		AActor* SpawnedNPC = GetWorld()->SpawnActor<AActor>(NPCUnityType, SpawnTransform, SpawnParams);
		if (SpawnedNPC)
		{
			ACharacter* SpawnedNPCCharacter = Cast<ACharacter>(SpawnedNPC);
			if (SpawnedNPCCharacter)
			{
				FVector TargetLocation = GetRandomSpawningPoint()->GetActorLocation();
				SetNPCTarget(SpawnedNPCCharacter, TargetLocation);
			}
		}
	}
		
}

void ANPCsManager::SetNPCTarget(ACharacter* NPC, const FVector& TargetLocation)
{
	if (!NPC) return;

	AAIController* AIController = Cast<AAIController>(NPC->GetController());
	if (AIController)
	{
		AIController->MoveToLocation(TargetLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NPC has no AI Controller!"));
	}
}

TArray<AActor*> ANPCsManager::FindNPCSpawnPoints()
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

TArray<AActor*> ANPCsManager::FindAllNavigationPoints()
{
	TArray<AActor*> allNavigationPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigationPoint::StaticClass(), allNavigationPoints);

	return allNavigationPoints;
}

ANavigationPoint* ANPCsManager::GetRandomSpawningPoint()
{
	TArray<AActor*> spawningPoints = FindNPCSpawnPoints();
	return Cast<ANavigationPoint>(spawningPoints[FMath::RandRange(0, spawningPoints.Num() - 1)]);	
}

