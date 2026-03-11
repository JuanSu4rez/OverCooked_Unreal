#include "NPCsManager.h"

#include "NavigationPoint.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BaseNPC.h"
#include "UPedestrianControllerInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"

ANPCsManager::ANPCsManager()
{
	CurrentNPCCount = 0;
	MaxNPCCount = 100;
}

//TODO - Manager should be a singleton
void ANPCsManager::BeginPlay()
{
	Super::BeginPlay();

	OnNavigationCompleted.AddDynamic(this, &ANPCsManager::HandleNavigationCompleted);

	Pedestrians = {};
	
	PathFinder = NewObject<UNPCPathFinder>(this);	
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

/*
 * Spawns an NPC at a random spawning point AND assigns it a random path and speed.
 * TODO - refactor, this function is doing too many things
 */

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

		float RandomSpeed = FMath::FRandRange(75.0f, 150.0f);		
		SetNPCDisplacementSpeed(Cast<ACharacter>(SpawnedNPC), RandomSpeed);

		FLinearColor SkinColor (
		FMath::FRand(),
		FMath::FRand(),
		FMath::FRand(),
		1.0f
		);
		SetSkinToNPC(Cast<ACharacter>(SpawnedNPC), SkinColor);
		
		AddPathToNPC(Cast<ACharacter>(SpawnedNPC), PathFinder->GetNavigationPath(randomSpawningPoint));
		
	}
}

void ANPCsManager::AddPathToNPC(ACharacter* NPCCharacter, TArray<AActor*> npcPath)
{
	if (!NPCCharacter) return;

	if (!NPCCharacter->GetController()) return;

	if (NPCCharacter->GetController()->Implements<UPedestrianControllerInterface>())
	{
		auto* PedestrianController = Cast<IPedestrianControllerInterface>(NPCCharacter->GetController());
		PedestrianController->Execute_SetPathToNavigate(NPCCharacter->GetController(), npcPath);
	}
}

//TODO - REPLACE FOR A PROPER MESH, THIS IS JUST FOR TESTING
// Generate a random color for the skin
void ANPCsManager::SetSkinToNPC(ACharacter* NPCCharacter, FLinearColor Color)
{
	if (!NPCCharacter) return;
	// Assuming the NPC has a mesh component
	TArray<UMeshComponent*> MeshComponents;
	NPCCharacter->GetComponents<UMeshComponent>(MeshComponents);
	
	if (MeshComponents.Num() > 0)
	{
		for (UMeshComponent* MeshComponent : MeshComponents)
		{
			if (MeshComponent)
			{
				UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
				if (DynamicMaterial)
				{
					DynamicMaterial->SetVectorParameterValue("Color", Color);
				}
			}
		}
		
	}
}

void ANPCsManager::SetNPCDisplacementSpeed(ACharacter* NPCCharacter, float DisplacementSpeed)
{
	if (NPCCharacter && NPCCharacter->GetCharacterMovement())
	{
		NPCCharacter->GetCharacterMovement()->MaxWalkSpeed = DisplacementSpeed;
	}
}

void ANPCsManager::HandleNavigationCompleted(AActor* Npc, AActor* NavigationPoint)
{
	if (Npc && NavigationPoint)
	{
		ResetNpcPath(Npc, NavigationPoint);

		FLinearColor SkinColor (
		FMath::FRand(),
		FMath::FRand(),
		FMath::FRand(),
		1.0f
		);
		SetSkinToNPC(Cast<ACharacter>(Npc), SkinColor);
        
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
