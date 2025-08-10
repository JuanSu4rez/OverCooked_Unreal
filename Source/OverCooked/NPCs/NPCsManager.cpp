#include "NPCsManager.h"

#include "NavigationPoint.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BaseNPC.h"
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

	OnNavigationCompleted.AddDynamic(this, &ANPCsManager::HandleNavigationCompleted);

	Pedestrians = {};
	
	PathFinder = NewObject<UNPCPathFinder>(this);
	auto result = PathFinder->GetExtremePoints();
	CreateNPC();

	FTimerHandle NPCSpawnTimerHandle;
	
	// Proper timer setup for UE 5.4
	GetWorld()->GetTimerManager().SetTimer(
		NPCSpawnTimerHandle,  // Even if you don't plan to use it, the parameter is required
		this,
		&ANPCsManager::CheckAndSpawnNPC,
		5.0f,    // Interval in seconds
		true      // Loop
	);
}

void ANPCsManager::CheckAndSpawnNPC()
{
	if (CurrentNPCCount < MaxNPCCount)
	{
		CreateNPC();
		CurrentNPCCount++; // Ensure you increment this when spawning succeeds
	}
}

void ANPCsManager::CreateNPC()
{
	if (NPCUnityType)
	{
		auto randomSpawningPoint = PathFinder->GetRandomSpawningPoint();
		// auto bestIntersectionPoint = PathFinder-> GetBestIntersectionPoint(randomSpawningPoint);

		// UE_LOG(LogTemp, Warning, TEXT("randomSpawningPoint :"), randomSpawningPoint->GetActorLocation());
		// UE_LOG(LogTemp, Warning, TEXT("bestIntersectionPoint :"), bestIntersectionPoint->GetActorLocation());
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FTransform SpawnTransform = FTransform(randomSpawningPoint->GetActorLocation());
		ABaseNPC* SpawnedNPC = GetWorld()->SpawnActor<ABaseNPC>(NPCUnityType, SpawnTransform, SpawnParams);
		
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

void ANPCsManager::HandleNavigationCompleted(AActor* Npc, AActor* NavigationPoint)
{
	if (Npc && NavigationPoint)
	{
		ResetNpcPath(Npc, NavigationPoint);
        
		// Optional debug log
		UE_LOG(LogTemp, Log, TEXT("Resetting path for NPC %s at point %s"), 
			*Npc->GetName(), 
			*NavigationPoint->GetName());
	}
}


void ANPCsManager::ResetNpcPath(AActor* NPC, AActor* InitialPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("RESET ---------------------------- :") );
	if (!NPC) return;
	
	UE_LOG(LogTemp, Warning, TEXT("TEST :") );
	AddPathToNPC(Cast<ACharacter>(NPC), PathFinder->GetNavigationPath(InitialPoint));
}
