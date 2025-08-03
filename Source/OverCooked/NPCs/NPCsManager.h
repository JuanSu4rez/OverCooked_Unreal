#pragma once

#include "CoreMinimal.h"
#include "NPCPathFinder.h"
#include "NavigationPoint.h"
#include "GameFramework/Actor.h"
#include "NPCsManager.generated.h"

UCLASS(Blueprintable)
class OVERCOOKED_API ANPCsManager : public AActor
{

	GENERATED_BODY()

public:

	ANPCsManager();
	virtual void BeginPlay() override;


	UPROPERTY()
	UNPCPathFinder* PathFinder;

	UPROPERTY()
	TArray<AActor*> Pedestrians;

	//EditAnywhere -> writable
	//VisibleAnywhere -> read-only
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NPCs Management")
	int32 CurrentNPCCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs Management")
	int32 MaxNPCCount;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCs Management")
	TSubclassOf<ACharacter> NPCUnityType;

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void CreateNPC();

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void AddPathToNPC(ACharacter* npcCharacter, TArray<AActor*> npcPath);

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void SetNPCTarget(ACharacter* NPC, const FVector& TargetLocation);

private:
	

	
};
