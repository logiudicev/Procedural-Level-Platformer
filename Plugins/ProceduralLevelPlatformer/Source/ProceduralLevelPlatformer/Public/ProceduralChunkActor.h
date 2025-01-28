// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EChunkerNodeOptions.h"
#include "ProceduralChunkerManager.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralChunkActor.generated.h"

class UDynamicMesh;

UCLASS()
class PROCEDURALLEVELPLATFORMER_API AProceduralChunkActor : public AActor
{
	GENERATED_BODY()

public:
	void InitDefaultValues();
	// Sets default values for this actor's properties
	AProceduralChunkActor();
	void SetFloorAsCeilings();
	void SetMirrorFloorsAsWalls();
	void SetEdgeConnectorLocations();
	void SetActorRotationAfterWallsAndCeilingIsBuilt();
	void BuildStartAndEndOverlapCollisions(int32 Index);
	void SetActorScaleAfterWallsAndCeilingIsBuilt();
	void SetActorLocationOffsetIfSet();
	void BuildNodeByParameters();
	void ApplyRelativeTransformsToWallsAndCeilings();
	void ApplyMaterialsToStaticMeshesIfSet();

	UFUNCTION()
	void SpawnAnimationFloatReturn(float value);
	UFUNCTION()
	void OnSpawnAnimationTimelineFinished();

	//Delegate that can be binded with the Float return for Spawning the Chunk Nodes
	
	FOnTimelineFloat InterpolationFunction{};
	//Delegate to bind with OnSpawnAnimationTimelineFinished
	FOnTimelineEvent SpawnAnimationTimelineFinished{};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timelines")
	UTimelineComponent* SpawnInAnimationTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines")
	UCurveFloat* AnimationTimelineFCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timelines")
	FVector TimelineStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timelines")
	FVector TimelineEndLocation;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines", meta = (ExposeOnSpawn="true"))
	FVector TimelineXYZOffset = FVector(5000.0f, 5000.0f, 5000.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timelines", meta = (ExposeOnSpawn="true"))
	bool bRandomTimelineXYZOffset = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor")
	UStaticMeshComponent* LevelChunkMeshFloor;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Ceiling")
	UStaticMeshComponent* LevelChunkMeshCeiling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Right Wall")
	UStaticMeshComponent* RightWallStaticMesh;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Left Wall")
	UStaticMeshComponent* LeftWallStaticMesh;

	UPROPERTY(Blueprintable ,BlueprintReadOnly, Category = "ActorStartOverlap")
	UBoxComponent* ActorStartOverlap;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "ActorEndOverlap")
	UBoxComponent* ActorEndOverlap;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Node Connector Reference")
	UArrowComponent* NodeEndConnector;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Node Connector Reference")
	UArrowComponent* NodeStartConnector;
	
	FPositionVertexBuffer* VertexBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Internal Actor Mesh Values")
	FVector WorldSpaceVertexLocationEnd;
	FVector WorldSpaceVertexLocationStart;
	FVector WorldSpaceVertexYAxisWallStart;
	float WorldSpaceCenterAxisX;
	float WorldSpaceCenterAxisY;
	float WorldSpaceCenterAxisZ;
	float WorldSpaceExtentX;
	float WorldSpaceExtentY;
	float WorldSpaceExtentZ;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	float CeilingHeightIfFloorIsMirroredAndNotAutomatic = 2000.0f;
	
	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters")
	bool MirrorFloorAsCeiling = false;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters")
	bool MirrorFloorAsWalls = false;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters")
	bool AutoMaticCelingHeightIfMirroredFromFloor = false;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	float PlatformHeightOffset = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	EChunkerNodeOptions NodeOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	FRotator FullPlatformNodeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool IsResponsibleForSpawnAndDeletingProcChunkActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	int32 ThisNodeIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	AProceduralChunkerManager* ProceduralChunkManager;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	UMaterial* FloorMaterial;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	UMaterial* RightWallMaterial;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	UMaterial* LeftWallMaterial;

	UPROPERTY(Blueprintable, BlueprintReadOnly, Category = "Chunk Parameters", meta = (ExposeOnSpawn="true"))
	UMaterial* CeilingMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FVector ActorLocationOffsetOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FVector ActorScaleOverride = FVector(1.0f,1.0f,1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform RightWallTransformOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform LeftWallTransformOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform CeilingTransformOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	bool bUseTransformOverrides = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bRemoveAllChunkWallsOnLeftSide = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bRemoveAllChunkWallsOnRightSide = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Chunk Parameters", meta = (ExposeOnSpawn="true"))
	bool bAnimateNewChunkNodesOnSpawn = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Used for Floor", meta = (ExposeOnSpawn="true"))
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Used for RightWall", meta = (ExposeOnSpawn="true"))
	UStaticMesh* RightWallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Used for Left Wall", meta = (ExposeOnSpawn="true"))
	UStaticMesh* LeftWallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Used for Ceiling", meta = (ExposeOnSpawn="true"))
	UStaticMesh* CeilingMesh;
private:
	bool HasExecutedGenerateProceduralChunkActors = false;
	bool HasExecutedDestroyPreviousProceduralChunkActors = false;
};
