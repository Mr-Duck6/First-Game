
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorBaseStaticObject.generated.h"

UCLASS()
class MYPROJECT3_API AMyActorBaseStaticObject : public AActor
{
	GENERATED_BODY()
	
public:	

	AMyActorBaseStaticObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticObjectMesh;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;





};
