// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include "CoreMinimal.h"
#include "EDishes.h"
#include "GameFramework/Actor.h"
#include "Delegates//DelegateCombinations.h"

#include "OrderManager.generated.h"

// // Delegate for broadcasting order events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrderCreated, const TArray<EDishes>&, OrdersQueue);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOrderCompleted);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrderChecked, bool, bIsOrderCorrect);

UCLASS()
class OVERCOOKED_API AOrderManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrderManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Manager")
	TArray<EDishes> AvailableDishes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order Manager")
	float OrderSpawnInterval;	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Order Manager")
	void SpawnOrder();

	UFUNCTION(BlueprintCallable)
	bool CheckOrder(const TArray<AActor*>& DeliveredIngredients);	
		
	// Event Dispatcher for when a new order is created
	UPROPERTY(BlueprintAssignable, Category = "Order Events")
	FOnOrderCreated OnOrderCreated;
	
	// Event Dispatcher for when an order is completed
	// UPROPERTY(BlueprintAssignable, Category = "Order Events")
	// FOnOrderCompleted OnOrderCompleted;
	
	// Event Dispatcher for order checking (used internally)
	// UPROPERTY(BlueprintAssignable, Category = "Order Events")
	// FOnOrderChecked OnOrderChecked;
	
private:
	TArray<EDishes> OrdersQueue = {};
	float LastOrderSpawnedAt = {0};
	void SpawnInitialOrders();
};
