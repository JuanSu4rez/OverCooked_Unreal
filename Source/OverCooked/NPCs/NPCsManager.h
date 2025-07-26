#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCsManager.generated.h"

UCLASS(Blueprintable)
class OVERCOOKED_API ANPCsManager : public AActor
{

	GENERATED_BODY()

public:

	ANPCsManager();	

	//EditAnywhere -> writable
	//VisibleAnywhere -> read-only
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs Management")
	int32 CurrentNPCCount;

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void CreateNPC();
	
};
