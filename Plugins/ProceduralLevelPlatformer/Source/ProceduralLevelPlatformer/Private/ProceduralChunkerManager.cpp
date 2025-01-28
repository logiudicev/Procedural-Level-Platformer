// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralChunkerManager.h"

#include "FChunkNodeOptionsStruct.h"
#include "Math/UnrealMathUtility.h"
#include "ProceduralChunkActor.h"

// Sets default values
AProceduralChunkerManager::AProceduralChunkerManager(): EndConnectorXAxisAdjustedLocation(0), FirstNode(false),
                                                        NewChunkXAxisCenterToStartOffset(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

float CalculateZOffset(float PitchAngle, float DistanceXAxis)
{
	// Convert the pitch angle from degrees to radians
	float YawAngleDegrees = PitchAngle;

	float YawAngleRadians = FMath::DegreesToRadians(YawAngleDegrees);

	float HalfWidth = DistanceXAxis * 0.5f;

	float ZOffset = FMath::Sin(YawAngleRadians) * HalfWidth;
	
	return ZOffset;
}

float CalculateXOffset(float PitchAngle, float DistanceXAxis)
{
	// Convert the pitch angle from degrees to radians
	float YawAngleDegrees = PitchAngle;

	float YawAngleRadians = FMath::DegreesToRadians(YawAngleDegrees);

	float HalfWidth = DistanceXAxis * 0.5f;

	float XOffset = FMath::Cos(YawAngleRadians) * HalfWidth;
	
	return XOffset;
}

void AProceduralChunkerManager::CalculateIncomingProceduralChunkActorTransform(FRotator PlatformRotation, float PlatformLengthOnXAxis, float PlatformHeightOffset, UStaticMesh* FloorStaticMesh, float XScale, float ZScale, bool CalculatePlatformLengthAndThinknessOffsetsAutomatically)
{
	if(CalculatePlatformLengthAndThinknessOffsetsAutomatically)
	{
		if(FloorStaticMesh)
		{
			PlatformLengthOnXAxis = ((FloorStaticMesh->GetBoundingBox().GetExtent().X) * 2) * XScale;
			PlatformHeightOffset = ((FloorStaticMesh->GetBoundingBox().GetExtent().Z) * 2) * ZScale;
			UE_LOG(LogTemp, Display, TEXT("Next platform length X on X axis will be: %f"), PlatformLengthOnXAxis);
			UE_LOG(LogTemp, Display, TEXT("Next platform Height Z on Z axis will be: %f"), PlatformHeightOffset);
		}
	}
	//Get the ZOffset for Platforms Rotated on Y Pitch Axis
	CalculatedZOffsetForSpawningNode = CalculateZOffset(PlatformRotation.Pitch, PlatformLengthOnXAxis);
	float ZOffSetForSpawningNode = CalculatedZOffsetForSpawningNode;
	CalculatedXOffsetForSpawningNode = CalculateXOffset(PlatformRotation.Pitch, PlatformLengthOnXAxis);
	float XOffsetForSpawningNode = CalculatedXOffsetForSpawningNode;
	
	//Get and set last node start connectors vectors
	LastNodeStartConnectorLocation = FVector(NewChunkXAxisCenterToStartOffset, NewChunkYAxisCenterToStartOffset, NewChunkZAxisCenterToStartOffset);
	EndConnectorXAxisAdjustedLocation = (LastNodeStartConnectorLocation.X + LastNodeEndConnectorLocation.X) + XOffsetForSpawningNode - CalculatedXOffsetForLastSpawnedNode;
	EndConnectorYAxisAdjustedLocation = (LastNodeStartConnectorLocation.Y + LastNodeEndConnectorLocation.Y);
	EndConnectorZAxisAdjustedLocation = (LastNodeStartConnectorLocation.Z + LastNodeEndConnectorLocation.Z) + ZOffSetForSpawningNode - CalculatedZOffsetForLastSpawnedNode;
	LastNodeEndConnectorLocation = FVector(EndConnectorXAxisAdjustedLocation, EndConnectorYAxisAdjustedLocation, EndConnectorZAxisAdjustedLocation);
	//We don't want to rotate immediately, only in the Procedural Chunk Actor after it's begin play. This is the final Transform
	NewNodeConnectorTransform = FTransform(FRotator(0.0,0.0,0.0), LastNodeEndConnectorLocation, LastNodeEndConnectorScale);
}

void AProceduralChunkerManager::SetupAndSpawnProceduralChunkActor(	EChunkerNodeOptions Option,
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
																	bool bRandomTimelineXYZOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("This node index is from manager is: %d"), ThisNodeIndex);

	bool IsResponsibleForSpawnAndDeletingProcChunkActors = NodeIndexForSpawnAndDeletingProcChunkActors == ThisNodeIndex;
	NewChunkActor->LevelChunkMeshFloor->SetStaticMesh(FloorStaticMesh);
	NewChunkActor->NodeOptions = bRandomEChunkerNodeOptions ? RandomEChunkerNodeOptionsEnum::GetRandomChunkerNodeOption() : Option;
	NewChunkActor->CeilingHeightIfFloorIsMirroredAndNotAutomatic = CeilingHeight;
	NewChunkActor->PlatformHeightOffset = PlatformHeightOffset;
	NewChunkActor->IsResponsibleForSpawnAndDeletingProcChunkActors = IsResponsibleForSpawnAndDeletingProcChunkActors;
	//Add Platform Rotation, AnimateNewNodes Bool, Additional Actors to Spawn, and Number of Them
	NewChunkActor->FullPlatformNodeRotation = PlatformRotation;
	NewChunkActor->ThisNodeIndex = ThisNodeIndex;

	NewChunkActor->ProceduralChunkManager = IsResponsibleForSpawnAndDeletingProcChunkActors ? this : nullptr;
	NewChunkActor->FloorMesh = FloorStaticMesh;
	NewChunkActor->FloorMaterial = FloorMaterial ? FloorMaterial : nullptr;
	NewChunkActor->RightWallMesh = RightWallStaticMesh ? RightWallStaticMesh : nullptr;
	NewChunkActor->RightWallMaterial = RightWallMaterial ? RightWallMaterial : nullptr;
	NewChunkActor->LeftWallMesh = LeftWallStaticMesh ? LeftWallStaticMesh : nullptr;
	NewChunkActor->LeftWallMaterial = LeftWallMaterial ? LeftWallMaterial : nullptr;
	NewChunkActor->CeilingMesh = CeilingStaticMesh ? CeilingStaticMesh : nullptr;
	NewChunkActor->CeilingMaterial = CeilingMaterial ? CeilingMaterial : nullptr;
	NewChunkActor->ActorLocationOffsetOverride = ActorLocationOffsetOverride;
	NewChunkActor->ActorScaleOverride = ActorScaleOffsetOverride;
	NewChunkActor->RightWallTransformOffset = RightWallTransform;
	NewChunkActor->LeftWallTransformOffset = LeftWallTransform;
	NewChunkActor->CeilingTransformOffset = CeilingTransform;
	NewChunkActor->bUseTransformOverrides = bUseTransformOverrides;
	NewChunkActor->bAnimateNewChunkNodesOnSpawn = bAnimateNewChunkNodesOnSpawn;
	NewChunkActor->AnimationTimelineFCurve = AnimationTimelineFCurve;
	NewChunkActor->TimelineXYZOffset = TimelineXYZOffset;
	NewChunkActor->bRandomTimelineXYZOffset = bRandomTimelineXYZOffset;

	NewChunkActor->bRemoveAllChunkWallsOnLeftSide = bRemoveAllChunkWallsOnLeftSide;
	NewChunkActor->bRemoveAllChunkWallsOnRightSide = bRemoveAllChunkWallsOnRightSide;
	
	//Finally Spawn Node
	NewChunkActor->FinishSpawning(NewNodeConnectorTransform);
	ForwardSpawnedProceduralChunkActors.AddUnique(NewChunkActor);
}

void AProceduralChunkerManager::SetPreviousProceduralChunkActorPropertiesUsedLaterWhenSpawningNewActors(FRotator PlatformRotation, AProceduralChunkActor* NewChunkActor, float PlatformLengthOnXAxis, float PlatformHeightOffset, bool CalculatePlatformLengthAndThinknessOffsetsAutomatically)
{
	if(CalculatePlatformLengthAndThinknessOffsetsAutomatically)
	{
		if(NewChunkActor->LevelChunkMeshFloor->GetStaticMesh())
		{
			PlatformLengthOnXAxis = ((NewChunkActor->LevelChunkMeshFloor->GetStaticMesh()->GetBoundingBox().GetExtent().X) * 2) * NewChunkActor->LevelChunkMeshFloor->GetRelativeScale3D().X;
			PlatformHeightOffset = ((NewChunkActor->LevelChunkMeshFloor->GetStaticMesh()->GetBoundingBox().GetExtent().Z) * 2) * NewChunkActor->LevelChunkMeshFloor->GetRelativeScale3D().Z;
			UE_LOG(LogTemp, Display, TEXT("Last platform length on X axis was: %f"), PlatformLengthOnXAxis);
			UE_LOG(LogTemp, Display, TEXT("Last platform Height on Z axis was: %f"), PlatformHeightOffset);
		}
	}
	NewChunkXAxisCenterToStartOffset = NewChunkActor->GetActorLocation().X - NewChunkActor->NodeStartConnector->GetComponentLocation().X;
	NewChunkYAxisCenterToStartOffset = NewChunkActor->GetActorLocation().Y - NewChunkActor->NodeStartConnector->GetComponentLocation().Y;
	NewChunkZAxisCenterToStartOffset = NewChunkActor->GetActorLocation().Z - NewChunkActor->NodeStartConnector->GetComponentLocation().Z;
	CalculatedZOffsetForLastSpawnedNode = CalculateZOffset(PlatformRotation.Pitch, PlatformLengthOnXAxis);
	CalculatedXOffsetForLastSpawnedNode = CalculateXOffset(PlatformRotation.Pitch, PlatformLengthOnXAxis);
	LastNodeEndConnectorLocation = NewChunkActor->NodeEndConnector->GetComponentLocation();
	FirstNode = false;
	LastNodeEndConnectorRotation = PlatformRotation;
}

void AProceduralChunkerManager::SpawnNextChunkNodeAndSetOptions(
	EChunkerNodeOptions Option,
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
	bool bRandomTimelineXYZOffset)
{
	//Instantiate new ChunkActor
	AProceduralChunkActor* NewChunkActor =	GetWorld()->SpawnActorDeferred<AProceduralChunkActor>
											(AProceduralChunkActor::StaticClass(), GetActorTransform());

	//Setup incoming Chunk Actor
	CalculateIncomingProceduralChunkActorTransform(PlatformRotation, PlatformLengthOnXAxis, PlatformHeightOffset, FloorStaticMesh, ActorScaleOffsetOverride.X, ActorScaleOffsetOverride.Z, CalculatePlatformLengthAndThinknessOffsetsAutomatically);

	//Setup and Spawn New Procedural Chunk Actor
	SetupAndSpawnProceduralChunkActor(	Option,
										CeilingHeight,
										PlatformHeightOffset,
										PlatformRotation,
										NewChunkActor,
										ThisNodeIndex,
										FloorStaticMesh,
										FloorMaterial,
										RightWallStaticMesh,
										RightWallMaterial,
										LeftWallStaticMesh,
										LeftWallMaterial,
										CeilingStaticMesh,
										CeilingMaterial,
										ActorLocationOffsetOverride,
										ActorScaleOffsetOverride,
										RightWallTransform,
										LeftWallTransform,
										CeilingTransform,
										bUseTransformOverrides,
										bAnimateNewChunkNodesOnSpawn,
										AnimationTimelineFCurve,
										TimelineXYZOffset,
										bRandomTimelineXYZOffset);
	
	//We want to remember some settings from the previous chunk. This is For Next Iteration of Spawns.
	SetPreviousProceduralChunkActorPropertiesUsedLaterWhenSpawningNewActors(PlatformRotation, NewChunkActor, PlatformLengthOnXAxis, PlatformHeightOffset, CalculatePlatformLengthAndThinknessOffsetsAutomatically);
	
}

// Called when the game starts or when spawned
void AProceduralChunkerManager::BeginPlay()
{
	Super::BeginPlay();
	LastNodeEndConnectorLocation = GetActorLocation();
	LastNodeEndConnectorRotation = GetActorRotation();
	LastNodeEndConnectorScale = GetActorScale3D();
	NewNodeConnectorTransform = GetActorTransform();
	LastNodeStartConnectorLocation = GetActorLocation();
	int32 CurrentNodeIndex = 0;
	
	//Want to make sure the node index is between the array size and 1.
	if(NodeIndexForSpawnAndDeletingProcChunkActors >= ChunkerNodeOptionsStructsArray.Num())
	{
		NodeIndexForSpawnAndDeletingProcChunkActors = ChunkerNodeOptionsStructsArray.Num() - 1;
	}
	
	if(NodeIndexForSpawnAndDeletingProcChunkActors <= 0)
	{
		NodeIndexForSpawnAndDeletingProcChunkActors = 1;
	}

	//This is used to make sure you don't fall off the platform when nodes are destroyed... And that the nodes are policed up on the next round of cleanup
	ChunkActorIndexToRemovePreviousPlatformsToTotalChunkActorOffset = (ChunkerNodeOptionsStructsArray.Num() - NodeIndexForSpawnAndDeletingProcChunkActors);
	
	if(ChunkerNodeOptionsStructsArray.Num() > 0)
	{
		
		EndConnectorXAxisAdjustedLocation = 0.0f;
		FirstNode = true;
		
		for(const FChunkNodeOptionsStruct& ChunkNode : ChunkerNodeOptionsStructsArray)
		{
			if(ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh)
			{
				
				SpawnNextChunkNodeAndSetOptions(
					ChunkNode.ChunkerNodeOptions,
					ChunkNode.CeilingHeightIfFloorIsMirroredAndNotAutomatic,
					ChunkNode.PlatformLengthOnXAxis,
					ChunkNode.PlatformHeightOnZAxisOffset,
					ChunkNode.PlatformRotationOverride,
					ChunkNode.bAnimateNewChunkNodesOnSpawn,
					ChunkNode.AdditionalActorsToSpawnIntoNode,
					ChunkNode.NumberOfEachAdditionalActorToSpawnIntoNode,
					CurrentNodeIndex,
					ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh,
					ChunkNode.FChunkMeshOptionsStruct.FloorMaterial,
					ChunkNode.FChunkMeshOptionsStruct.RightWallStaticMesh,
					ChunkNode.FChunkMeshOptionsStruct.RightWallMaterial,
					ChunkNode.FChunkMeshOptionsStruct.LeftWallStaticMesh,
					ChunkNode.FChunkMeshOptionsStruct.LeftWallMaterial,
					ChunkNode.FChunkMeshOptionsStruct.CeilingStaticMesh,
					ChunkNode.FChunkMeshOptionsStruct.CeilingMaterial,
					ChunkNode.ActorLocationOffsetOverride,
					ChunkNode.ActorScaleOverride,
					ChunkNode.FChunkMeshOptionsStruct.RightWallTransformOffsetOverride,
					ChunkNode.FChunkMeshOptionsStruct.LeftWallTransformOffsetOverride,
					ChunkNode.FChunkMeshOptionsStruct.CeilingTransformOffsetOverride,
					ChunkNode.FChunkMeshOptionsStruct.bUseOffsetOverrides,
					ChunkNode.CalculatePlatformLengthAndHeightXZOffsetsAutomatically,
					ChunkNode.AnimationTimelineFCurve,
					ChunkNode.TimelineXYZOffset,
					ChunkNode.bRandomTimelineXYZOffset);
				CurrentNodeIndex = CurrentNodeIndex + 1;
				UE_LOG(LogTemp, Warning, TEXT("Current node index is: %d"), CurrentNodeIndex);

			}
		}
		//When complete set value to enable running again (if Randomizer is enabled, reshuffle) otherwise run again for x Loops set
		//Should be in a function that gets called when last (specified node is overlapped)
	}
}

// Called every frame
void AProceduralChunkerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralChunkerManager::GenerateNewProceduralChunkActors(AActor* OtherActor)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped and called the new manager function successfully"), *OtherActor->GetName());
		if(RandomizeProceduralChunkActorOrder)
		{
			RandomizedNodeOptionsStructsArray = ChunkerNodeOptionsStructsArray;

			FRandomStream RandomStream(FMath::Rand());
			for (int32 i = RandomizedNodeOptionsStructsArray.Num() - 1; i > 0; i--)
			{
				int32 j = RandomStream.RandRange(0, i);
				RandomizedNodeOptionsStructsArray.Swap(i, j);
			}
			int32 CurrentNodeIndex = 0;
			PreviouslySpawnedProceduralChunkActors = ForwardSpawnedProceduralChunkActors;
			// ForwardSpawnedProceduralChunkActors.Empty();
			for(int32 i = 0; i < RandomizedNodeOptionsStructsArray.Num(); i++)
			{
				const FChunkNodeOptionsStruct& ChunkNode = RandomizedNodeOptionsStructsArray[i];
				if(ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh)
				{
					SpawnNextChunkNodeAndSetOptions(
						ChunkNode.ChunkerNodeOptions,
						ChunkNode.CeilingHeightIfFloorIsMirroredAndNotAutomatic,
						ChunkNode.PlatformLengthOnXAxis,
						ChunkNode.PlatformHeightOnZAxisOffset,
						ChunkNode.PlatformRotationOverride,
						ChunkNode.bAnimateNewChunkNodesOnSpawn,
						ChunkNode.AdditionalActorsToSpawnIntoNode,
						ChunkNode.NumberOfEachAdditionalActorToSpawnIntoNode,
						CurrentNodeIndex,
						ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.FloorMaterial,
						ChunkNode.FChunkMeshOptionsStruct.RightWallStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.RightWallMaterial,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallMaterial,
						ChunkNode.FChunkMeshOptionsStruct.CeilingStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.CeilingMaterial,
						ChunkNode.ActorLocationOffsetOverride,
						ChunkNode.ActorScaleOverride,
						ChunkNode.FChunkMeshOptionsStruct.RightWallTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.CeilingTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.bUseOffsetOverrides,
						ChunkNode.CalculatePlatformLengthAndHeightXZOffsetsAutomatically,
						ChunkNode.AnimationTimelineFCurve,
						ChunkNode.TimelineXYZOffset,
						ChunkNode.bRandomTimelineXYZOffset);
					CurrentNodeIndex = CurrentNodeIndex + 1;
					UE_LOG(LogTemp, Warning, TEXT("Current node index is: %d"), CurrentNodeIndex);

				}
			}
		}
		else
		{
			CopiedNodeOptionsStructsArray = ChunkerNodeOptionsStructsArray;

			int32 CurrentNodeIndex = 0;
			PreviouslySpawnedProceduralChunkActors = ForwardSpawnedProceduralChunkActors;
			// ForwardSpawnedProceduralChunkActors.Empty();
			for(int32 i = 0; i < CopiedNodeOptionsStructsArray.Num(); i++)
			{
				const FChunkNodeOptionsStruct& ChunkNode = CopiedNodeOptionsStructsArray[i];
				if(ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh)
				{
					SpawnNextChunkNodeAndSetOptions(
						ChunkNode.ChunkerNodeOptions,
						ChunkNode.CeilingHeightIfFloorIsMirroredAndNotAutomatic,
						ChunkNode.PlatformLengthOnXAxis,
						ChunkNode.PlatformHeightOnZAxisOffset,
						ChunkNode.PlatformRotationOverride,
						ChunkNode.bAnimateNewChunkNodesOnSpawn,
						ChunkNode.AdditionalActorsToSpawnIntoNode,
						ChunkNode.NumberOfEachAdditionalActorToSpawnIntoNode,
						CurrentNodeIndex,
						ChunkNode.FChunkMeshOptionsStruct.FloorStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.FloorMaterial,
						ChunkNode.FChunkMeshOptionsStruct.RightWallStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.RightWallMaterial,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallMaterial,
						ChunkNode.FChunkMeshOptionsStruct.CeilingStaticMesh,
						ChunkNode.FChunkMeshOptionsStruct.CeilingMaterial,
						ChunkNode.ActorLocationOffsetOverride,
						ChunkNode.ActorScaleOverride,
						ChunkNode.FChunkMeshOptionsStruct.RightWallTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.LeftWallTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.CeilingTransformOffsetOverride,
						ChunkNode.FChunkMeshOptionsStruct.bUseOffsetOverrides,
						ChunkNode.CalculatePlatformLengthAndHeightXZOffsetsAutomatically,
						ChunkNode.AnimationTimelineFCurve,
						ChunkNode.TimelineXYZOffset,
						ChunkNode.bRandomTimelineXYZOffset);
					CurrentNodeIndex = CurrentNodeIndex + 1;
					UE_LOG(LogTemp, Warning, TEXT("Current node index is: %d"), CurrentNodeIndex);

				}
			}
		}
	}
}

void AProceduralChunkerManager::DestroyPreviousProceduralChunkActors(AActor* OtherActor)
{
	AProceduralChunkActor* CurrentChunkActorToDestroy;
	if(OtherActor->ActorHasTag("Player") && DestroyPreviousProceduralChunkActorsWhenNewChunkActorsAreSpawned)
	{
		//how to add the .Num() to tracked array?
		// ChunkActorIndexToRemovePreviousPlatformsToTotalChunkActorOffset = (PreviouslySpawnedProceduralChunkActors.Num() - NodeIndexForSpawnAndDeletingProcChunkActors);
		for(int32 i = 0; i < PreviouslySpawnedProceduralChunkActors.Num() - ChunkActorIndexToRemovePreviousPlatformsToTotalChunkActorOffset; i++)
		{
			ForwardSpawnedProceduralChunkActors.Remove(PreviouslySpawnedProceduralChunkActors[i]);
			if(PreviouslySpawnedProceduralChunkActors.IsValidIndex(i) && !PreviouslySpawnedProceduralChunkActors[i]->IsActorBeingDestroyed() && !ForwardSpawnedProceduralChunkActors.Contains(PreviouslySpawnedProceduralChunkActors[i]))
			{
				CurrentChunkActorToDestroy = PreviouslySpawnedProceduralChunkActors[i];
				if(CurrentChunkActorToDestroy)
				{
					CurrentChunkActorToDestroy->Destroy();
				}
				
			}
		}
		//What is left of Prev Proc Actors?
		UE_LOG(LogTemp, Display, TEXT("There are %d Actors left in PreviouslySpawnedProceduralChunkActors"), PreviouslySpawnedProceduralChunkActors.Num());

	}
}

EChunkerNodeOptions RandomEChunkerNodeOptionsEnum::GetRandomChunkerNodeOption()
{
	//get the count of the options in EChunkerNodeOptions. This is used if RandomEChunkerNodeOptions is true
	int32 EChunkerNodeOptionsEnumCount = StaticEnum<EChunkerNodeOptions>()->NumEnums() - 1;
	int32 RandomIndex = FMath::RandRange( 0, EChunkerNodeOptionsEnumCount - 1);
	UE_LOG(LogTemp, Display, TEXT("Randomly selected option: %d"), static_cast<int32>(RandomIndex));
	return static_cast<EChunkerNodeOptions>(RandomIndex);
}
