#pragma once

#include "FChunkActorsStruct.generated.h"

USTRUCT(BlueprintType)
struct FChunkActorsStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	TSubclassOf<AActor> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	FVector Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	bool UsesNavMeshVolume = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	bool UseAdditionalInstancesOfSaidActor = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	int32 AdditionalInstancesOfSaidActor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeActors")
	FVector StaggerAdditionalInstanceOfSaidActorsByOffset;
};
