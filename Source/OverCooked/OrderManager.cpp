// Fill out your copyright notice in the Description page of Project Settings.


#include "OrderManager.h"

#include "EDishes.h"
#include "HAL/Platform.h"
#include "OrderData.h"
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

		// UE_LOG(LogTemp, Warning, TEXT("-------ORDERS IN QUEUE----------"));
		// for (auto Order : OrdersQueue)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Order in queue: %s"), *UEnum::GetDisplayValueAsText(Order).ToString());
		// }

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
    // UE_LOG(LogTemp, Display, TEXT("=== ORDER CHECK STARTED ==="));
    if (DeliveredIngredients.IsEmpty())
    {
        // UE_LOG(LogTemp, Warning, TEXT("No ingredients delivered!"));
        return false;
    }
    
    if (!OrderDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ OrdersDataTable is NOT assigned in the Editor!"));
        return false;
    }

    // Print all delivered ingredients
    // UE_LOG(LogTemp, Display, TEXT("DELIVERED INGREDIENTS (%d):"), DeliveredIngredients.Num());
    TArray<FName> DeliveredIngredientNames;
    for (AActor* IngredientActor : DeliveredIngredients)
    {
        FName IngredientTag = NAME_None;
        if (IngredientActor && IngredientActor->Tags.Num() > 0)
        {
        	IngredientTag = IngredientActor->Tags[0];
            // UE_LOG(LogTemp, Display, TEXT(" - %s (Actor: %s)"), 
            //     *IngredientTag.ToString(), 
            //     *IngredientActor->GetName());
            DeliveredIngredientNames.Add(IngredientTag);
        }
    }

	for (int index = 0; index < OrdersQueue.Num(); index++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Order in queue: %s"), *UEnum::GetDisplayValueAsText(OrdersQueue[index]).ToString());
		const FName& OrderName = *UEnum::GetDisplayValueAsText(OrdersQueue[index]).ToString();		
		FOrderData* OrderDataRow = OrderDataTable->FindRow<FOrderData>(OrderName, TEXT("CheckingIngredients"));

		if (DeliveredIngredientNames.Num() != OrderDataRow->Ingredients.Num())
		{
			continue;
		}
		
		bool bAllIngredientsMatch = true;
		for (const FName& RequiredIngredient : OrderDataRow->Ingredients)
		{
			if (!DeliveredIngredientNames.Contains(RequiredIngredient))
			{
				UE_LOG(LogTemp, Display, TEXT("❌ Missing ingredient: %s"), *RequiredIngredient.ToString());
				bAllIngredientsMatch = false;
				break;
			}
		}

		if (bAllIngredientsMatch)
		{
			UE_LOG(LogTemp, Display, TEXT("✅ CORRECT ORDER MATCH, remove from queue: %s, index: %d"), *OrderName.ToString(), index);
			OrdersQueue.RemoveAt(index);
			return true;
		}
		
	}
	// UE_LOG(LogTemp, Display, TEXT("DELIVERED INGREDIENTS (%hs):"), "ACA");
	
	// OrdersQueue.Sort(DeliveredIngredientNames);

    // Get all row names from the DataTable
    // TArray<FName> RowNames = OrderDataTable->GetRowNames();
    // UE_LOG(LogTemp, Display, TEXT("Checking against %d possible orders..."), RowNames.Num());
    //
    // // Iterate through each row
    // for (const FName& RowName : RowNames)
    // {
    //     FOrderData* OrderDataRow = OrderDataTable->FindRow<FOrderData>(RowName, TEXT("CheckingIngredients"));
    //
    //     if (!OrderDataRow)
    //     {
    //         UE_LOG(LogTemp, Warning, TEXT("Skipping invalid row: %s"), *RowName.ToString());
    //         continue;
    //     }
    //
    //     // Print current order being checked
    //     // UE_LOG(LogTemp, Display, TEXT("\nChecking order: %s"), *RowName.ToString());
    //     // UE_LOG(LogTemp, Display, TEXT("REQUIRED INGREDIENTS (%d):"), OrderDataRow->Ingredients.Num());
    //     // for (const FName& Ingredient : OrderDataRow->Ingredients)
    //     // {
    //     //     UE_LOG(LogTemp, Display, TEXT(" - %s"), *Ingredient.ToString());
    //     // }
    //     
    //     if (DeliveredIngredientNames.Num() != OrderDataRow->Ingredients.Num())
    //     {
    //         // UE_LOG(LogTemp, Display, TEXT("❌ Ingredient count mismatch (Delivered: %d, Required: %d)"),
    //         //     DeliveredIngredientNames.Num(),
    //         //     OrderDataRow->Ingredients.Num());
    //         continue;
    //     }
    //
    //     // Check if all required ingredients are present
    //     bool bAllIngredientsMatch = true;
    //     for (const FName& RequiredIngredient : OrderDataRow->Ingredients)
    //     {
    //         if (!DeliveredIngredientNames.Contains(RequiredIngredient))
    //         {
    //             UE_LOG(LogTemp, Display, TEXT("❌ Missing ingredient: %s"), *RequiredIngredient.ToString());
    //             bAllIngredientsMatch = false;
    //             break;
    //         }
    //     }
    //
    //     if (bAllIngredientsMatch)
    //     {
    //         UE_LOG(LogTemp, Display, TEXT("✅ CORRECT ORDER MATCH, remove from queue: %s"), *RowName.ToString());        	
    //         return true;
    //     }
    // }

    UE_LOG(LogTemp, Warning, TEXT("=== NO MATCHING ORDER FOUND ==="));
    return false;
}