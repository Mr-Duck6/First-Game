
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CarsParametrsDA.generated.h"


UENUM()
enum class ECarType
{
	BaseCar,
	BigCar,
	SmallCar,
};

UCLASS()
class MYPROJECT3_API UCarsParametrsDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 CarsType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 CarsOnTheRoad;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 DistancebetweenCars;

	
};
