#pragma once

#include "CoreMinimal.h"
#include "NPCPathFinder.h"
#include "NavigationPoint.h"
#include "GameFramework/Actor.h"
#include "NPCsManager.generated.h"

// // Delegate for broadcasting npc events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNpcEvent, AActor*, Npc, AActor*, NavigationPoint);

UCLASS(Blueprintable)
class OVERCOOKED_API ANPCsManager : public AActor
{

	GENERATED_BODY()

public:

	ANPCsManager();
	virtual void BeginPlay() override;
	void CheckAndSpawnNPC();


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

	UPROPERTY(BlueprintAssignable, Category = "NPCs Management Events")
	FOnNpcEvent OnNavigationCompleted; 
	
	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void CreateNPC();

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void AddPathToNPC(ACharacter* NPCCharacter, TArray<AActor*> npcPath);

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void SetSkinToNPC(ACharacter* NPCCharacter, FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void SetNPCDisplacementSpeed(ACharacter* NPCCharacter, float DisplacementSpeed);
	
	UFUNCTION()
	void HandleNavigationCompleted(AActor* Npc, AActor* NavigationPoint);

	UFUNCTION(BlueprintCallable, Category = "NPCs Management")
	void ResetNpcPath(AActor* NPC, AActor* InitialPoint);

private:
	
};
