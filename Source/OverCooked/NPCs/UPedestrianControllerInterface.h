
#include "UPedestrianControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPedestrianControllerInterface : public UInterface
{
public:
	GENERATED_BODY()
};

class OVERCOOKED_API IPedestrianControllerInterface
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PedestrianControllerInterface")
	void SetPathToNavigate(const TArray<AActor*>& Path);
};
