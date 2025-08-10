#include "BaseNPC.h"

#include "Kismet/GameplayStatics.h"

void ABaseNPC::BroadcastNavigationCompleted(AActor* lastPointVisited)
{
UE_LOG(LogTemp, Warning, TEXT("1111111111111111111111111111111111111111") );
	
	// Get all NPC managers in the world (or use a different approach to find your manager)
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
