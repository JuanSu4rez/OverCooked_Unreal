#include "BaseNPC.h"

#include "Kismet/GameplayStatics.h"

/*
 * Notify the NPCsManager that this NPC has completed its navigation
 * and provide the last point visited.
 */
void ABaseNPC::BroadcastNavigationCompleted(AActor* lastPointVisited)
{
// UE_LOG(LogTemp, Warning, TEXT("1111111111111111111111111111111111111111") );
	
	// Get all NPC managers in the world (should be only one)
	
	//TODO - Manager should be a singleton , a reference to it should be stored in the NPC
	// to avoid searching for it every time an NPC completes its navigation
	// This is a temporary solution
	
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCsManager::StaticClass(), FoundManagers);
    
	for (AActor* Actor : FoundManagers)
	{
		if (ANPCsManager* Manager = Cast<ANPCsManager>(Actor))
		{
			Manager->OnNavigationCompleted.Broadcast(this, lastPointVisited);
		}
	}
}
