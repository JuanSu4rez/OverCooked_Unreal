#pragma once
#include "NPCsManager.h"
#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"

UCLASS()
class OVERCOOKED_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void BroadcastNavigationCompleted(AActor* lastPointVisited);
	
};

