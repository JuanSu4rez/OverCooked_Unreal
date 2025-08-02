#include "NPCsManager.h"

#include "NavigationPoint.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "UPedestrianControllerInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"

ANPCsManager::ANPCsManager()
{
	CurrentNPCCount = 0;
	MaxNPCCount = 100;
}

void ANPCsManager::BeginPlay()
{
	Super::BeginPlay();

	PathFinder = NewObject<UNPCPathFinder>(this);
	auto result = PathFinder->GetExtremePoints();
	CreateNPC();
}

void ANPCsManager::CreateNPC()
{
	if (NPCUnityType)
	{
		auto randomSpawningPoint = PathFinder->GetRandomSpawningPoint();
		auto bestIntersectionPoint = PathFinder-> GetBestIntersectionPoint(randomSpawningPoint);

		UE_LOG(LogTemp, Warning, TEXT("randomSpawningPoint :"), randomSpawningPoint->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("bestIntersectionPoint :"), bestIntersectionPoint->GetActorLocation());
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FTransform SpawnTransform = FTransform(randomSpawningPoint->GetActorLocation());
		AActor* SpawnedNPC = GetWorld()->SpawnActor<AActor>(NPCUnityType, SpawnTransform, SpawnParams);
		AddPathToNPC(Cast<ACharacter>(SpawnedNPC), PathFinder->GetNavigationPath(randomSpawningPoint));
	}
}

void ANPCsManager::AddPathToNPC(ACharacter* npcCharacter, TArray<AActor*> npcPath)
{
	if (!npcCharacter) return;

	if (!npcCharacter->GetController()) return;

	if (npcCharacter->GetController()->Implements<UPedestrianControllerInterface>())
	{
		auto* pedestrianController = Cast<IPedestrianControllerInterface>(npcCharacter->GetController());
		pedestrianController->Execute_SetPathToNavigate(npcCharacter->GetController(), npcPath);
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