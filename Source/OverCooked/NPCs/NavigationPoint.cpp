#include "NavigationPoint.h"

ANavigationPoint::ANavigationPoint()
{
	
}

void ANavigationPoint::BeginPlay()
{
	Super::BeginPlay();
    
	FColor DebugColor = IsExtreme() ? FColor::Green : FColor::Blue;
	float DebugSize = IsExtreme() ? 50.f : 25.f;
    
	DrawDebugSphere(
		GetWorld(), 
		GetActorLocation(), 
		DebugSize, 
		4, 
		DebugColor, 
		true,  // Persistent
		-1.f,  // Lifetime (negative = infinite)
		0,     // Depth priority
		2.f    // Thickness
	);
}

bool ANavigationPoint::IsExtreme() const
{	
   	return Type == EPatrolPointType::Extreme;   
}
