// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EChunkerNodeOptions.h"
#include "FChunkNodeOptionsStruct.h"
#include "GameFramework/Actor.h"
#include "ProceduralChunkerManager.generated.h"

class AProceduralChunkActor;

UCLASS()
class PROCEDURALLEVELPLATFORMER_API AProceduralChunkerManager : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Generate new Procedural Chunk Actors")
	void GenerateNewProceduralChunkActors(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Destroy Previous Procedural Chunk Actors")
	void DestroyPreviousProceduralChunkActors(AActor* OtherActor);
	
	// Sets default values for this actor's properties
	AProceduralChunkerManager();
	void CalculateIncomingProceduralChunkActorTransform(FRotator PlatformRotation, float PlatformLengthOnXAxis, float PlatformHeightOffset, UStaticMesh* FloorStaticMesh, float XScale, float ZScale, bool CalculatePlatformLengthAndThinknessOffsetsAutomatically);
	void SetupAndSpawnProceduralChunkActor(	EChunkerNodeOptions Option,
											float CeilingHeight,
											float PlatformHeightOffset,
											FRotator PlatformRotation,
											AProceduralChunkActor* NewChunkActor,
											int32 ThisNodeIndex,
											UStaticMesh* FloorStaticMesh,
											UMaterial* FloorMaterial,
											UStaticMesh* RightWallStaticMesh,
											UMaterial* RightWallMaterial,
											UStaticMesh* LeftWallStaticMesh,
											UMaterial* LeftWallMaterial,
											UStaticMesh* CeilingStaticMesh,
											UMaterial* CeilingMaterial,
											FVector ActorLocationOffsetOverride,
											FVector ActorScaleOffsetOverride,
											FTransform RightWallTransform,
											FTransform LeftWallTransform,
											FTransform CeilingTransform,
											bool bUseTransformOverrides,
											bool bAnimateNewChunkNodesOnSpawn,
											UCurveFloat* AnimationTimelineFCurve,
											FVector TimelineXYZOffset,
											bool bRandomTimelineXYZOffset);
	void SetPreviousProceduralChunkActorPropertiesUsedLaterWhenSpawningNewActors(
											FRotator PlatformRotation,
											AProceduralChunkActor* NewChunkActor,
											float PlatformLengthOnXAxis,
											float PlatformHeightOffset,
											bool CalculatePlatformLengthAndThinknessOffsetsAutomatically);
	void SpawnNextChunkNodeAndSetOptions(	EChunkerNodeOptions Option,
											float CeilingHeight,
											float PlatformLengthOnXAxis,
											float PlatformHeightOffset,
											FRotator PlatformRotation,
											bool bAnimateNewChunkNodesOnSpawn,
											TArray<FChunkActorsStruct> AdditionalActorsToSpawnIntoNode,
											float NumberOfEachAdditionalActorToSpawnIntoNode,
											int32 ThisNodeIndex,
											UStaticMesh* FloorStaticMesh,
											UMaterial* FloorMaterial,
											UStaticMesh* RightWallStaticMesh,
											UMaterial* RightWallMaterial,
											UStaticMesh* LeftWallStaticMesh,
											UMaterial* LeftWallMaterial,
											UStaticMesh* CeilingStaticMesh,
											UMaterial* CeilingMaterial,
											FVector ActorLocationOffsetOverride,
											FVector ActorScaleOffsetOverride,
											FTransform RightWallTransform,
											FTransform LeftWallTransform,
											FTransform CeilingTransform,
											bool bUseTransformOverrides,
											bool CalculatePlatformLengthAndThinknessOffsetsAutomatically,
											UCurveFloat* AnimationTimelineFCurve,
											FVector TimelineXYZOffset,
											bool bRandomTimelineXYZOffset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Set Properties for Each Node Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	TArray<FChunkNodeOptionsStruct> ChunkerNodeOptionsStructsArray;

	//Set Properties for All Node Actors Collectively
	//TODO: Add node index number that this manager sends to Chunk Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	//The index of the Procedural Node Actor that will spawn new Actors and delete Old ones on overlap
	int32 NodeIndexForSpawnAndDeletingProcChunkActors = 4;

	// int32 NodeIndexForDeletingStragglerProcChunkActors;
	
	//TODO: Add Optional Randomizer of configured nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool RandomizeProceduralChunkActorOrder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool DestroyPreviousProceduralChunkActorsWhenNewChunkActorsAreSpawned = true;
	
	//TODO: Add Optional Randomizer of configured nodes to have different Enum Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bRandomEChunkerNodeOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bRemoveAllChunkWallsOnLeftSide = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bRemoveAllChunkWallsOnRightSide = false;
	
	//TODO: Add optional Endless mode (default)
	//TODO: Add Optional loop counts to spawn again
	//TODO: Way to use Seed, Level Goal
	
	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters")
	FVector LastNodeEndConnectorLocation;
	FRotator LastNodeEndConnectorRotation;
	FVector LastNodeEndConnectorScale;
	FTransform NewNodeConnectorTransform;
	FVector LastNodeStartConnectorLocation;
	float EndConnectorXAxisAdjustedLocation;
	float EndConnectorYAxisAdjustedLocation;
	float EndConnectorZAxisAdjustedLocation;
	bool FirstNode;
	float NewChunkXAxisCenterToStartOffset;
	float NewChunkYAxisCenterToStartOffset;
	float NewChunkZAxisCenterToStartOffset;
	float CalculatedZOffsetForSpawningNode;
	float CalculatedXOffsetForSpawningNode;
	float CalculatedZOffsetForLastSpawnedNode;
	float CalculatedXOffsetForLastSpawnedNode;
	int32 ChunkActorIndexToRemovePreviousPlatformsToTotalChunkActorOffset;
	
	TArray<FChunkNodeOptionsStruct> RandomizedNodeOptionsStructsArray = ChunkerNodeOptionsStructsArray;
	TArray<FChunkNodeOptionsStruct> CopiedNodeOptionsStructsArray = ChunkerNodeOptionsStructsArray;

	TArray<AProceduralChunkActor*> ForwardSpawnedProceduralChunkActors;
	TArray<AProceduralChunkActor*> PreviouslySpawnedProceduralChunkActors;
	
};
