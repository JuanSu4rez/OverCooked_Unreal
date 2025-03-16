// Fill out your copyright notice in the Description page of Project Settings.


#include "OrderManager.h"

#include "EDishes.h"
#include "HAL/Platform.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOrderManager::AOrderManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dishes = {EDishes::SimpleBurger, EDishes::CheeseBurger, EDishes::CompleteBurger, EDishes::Salad};
	
}

// Called when the game starts or when spawned
void AOrderManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay -> Dishes count: %d"), Dishes.Num());
}

// Called every frame
void AOrderManager::Tick(float DeltaTime)
{
	if (GetWorld()->TimeSeconds > LastOrderSpawnedAt + OrderSpawnInterval)
	{
		SpawnOrder();
		LastOrderSpawnedAt = GetWorld()->TimeSeconds;		
	}
}

void AOrderManager::SpawnOrder()
{		
	if (Dishes.Num() == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No dishes available to spawn!"));
		return;
	}	
	
	int32 RandomIndex = FMath::RandRange(0, Dishes.Num() - 1);	
	EDishes RandomDish = Dishes[RandomIndex];

	FText DishNameText = UEnum::GetDisplayValueAsText(RandomDish);
		
	UE_LOG(LogTemp, Warning, TEXT("SPAWNING ORDER: %s"), *DishNameText.ToString());

	// Trigger the event with the randomly selected dish
	OnOrderCreated.Broadcast(RandomDish);
}



bool AOrderManager::CheckOrder(const TArray<AActor*>& DeliveredIngredients)
{
	UE_LOG(LogTemp, Warning, TEXT("Checking order"));
	// OnOrderCreated.Broadcast(EDishes::Burger);
	return true;
}