#pragma once

#include "CoreMinimal.h"
#include "EChunkerNodeOptions.h"
#include "FChunkActorsStruct.h"
#include "FChunkMeshOptionsStruct.h"
#include "FChunkNodeOptionsStruct.generated.h"

USTRUCT(BlueprintType)
struct FChunkNodeOptionsStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FChunkMeshOptionsStruct FChunkMeshOptionsStruct;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	EChunkerNodeOptions ChunkerNodeOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	float CeilingHeightIfFloorIsMirroredAndNotAutomatic = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	bool CalculatePlatformLengthAndHeightXZOffsetsAutomatically = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	float PlatformLengthOnXAxis = 4000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	float PlatformHeightOnZAxisOffset = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FVector ActorLocationOffsetOverride;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FRotator PlatformRotationOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FVector ActorScaleOverride = FVector(1.0f,1.0f,1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	bool bAnimateNewChunkNodesOnSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines")
	UCurveFloat* AnimationTimelineFCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines", meta = (ExposeOnSpawn="true"))
	FVector TimelineXYZOffset = FVector(5000.0f, 5000.0f, 5000.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines", meta = (ExposeOnSpawn="true"))
	bool bRandomTimelineXYZOffset = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	TArray<FChunkActorsStruct> AdditionalActorsToSpawnIntoNode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	float NumberOfEachAdditionalActorToSpawnIntoNode;
};
