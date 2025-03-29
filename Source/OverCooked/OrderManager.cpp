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

	AvailableDishes = {EDishes::SimpleBurger, EDishes::CheeseBurger, EDishes::CompleteBurger, EDishes::Salad};
	
}

// Called when the game starts or when spawned
void AOrderManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay -> Dishes count: %d"), AvailableDishes.Num());
	SpawnInitialOrders();
}

void AOrderManager::SpawnInitialOrders()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		for (int i = 0; i < 2; i++)		
			SpawnOrder();		
	}, 2.0f, false);
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
	if (AvailableDishes.Num() == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No dishes available to spawn!"));
		return;
	}	
	
	if (OrdersQueue.Num() < 5)
	{
		int32 RandomIndex = FMath::RandRange(0, AvailableDishes.Num() - 1);	
		EDishes RandomDish = AvailableDishes[RandomIndex];

		FText DishNameText = UEnum::GetDisplayValueAsText(RandomDish);
		
		// UE_LOG(LogTemp, Warning, TEXT("SPAWNING ORDER: %s"), *DishNameText.ToString());

		OrdersQueue.Push(RandomDish);

		UE_LOG(LogTemp, Warning, TEXT("-------ORDERS IN QUEUE----------"));
		for (auto Order : OrdersQueue)
		{
			UE_LOG(LogTemp, Warning, TEXT("Order in queue: %s"), *UEnum::GetDisplayValueAsText(Order).ToString());
		}

		// for (auto Element : OrdersQueue)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Order in queue: %s"), *UEnum::GetDisplayValueAsText(Element).ToString());
		// }

		// Trigger the event with the randomly selected dish
		OnOrderCreated.Broadcast(OrdersQueue);
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting Order: %s"), *UEnum::GetDisplayValueAsText(RandomDish).ToString());
	}
}



bool AOrderManager::CheckOrder(const TArray<AActor*>& DeliveredIngredients)
{
	UE_LOG(LogTemp, Warning, TEXT("Checking order FROM C++"));	
	return true;
}