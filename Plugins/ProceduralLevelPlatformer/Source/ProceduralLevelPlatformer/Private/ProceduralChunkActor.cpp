// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralChunkActor.h"

void AProceduralChunkActor::InitDefaultValues()
{
	PrimaryActorTick.bCanEverTick = true;

	LevelChunkMeshFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Chunk Floor"));
	if(FloorMesh)
	{
		LevelChunkMeshFloor->SetStaticMesh(FloorMesh);
	}
	PlatformHeightOffset = AActor::GetComponentsBoundingBox().GetExtent().Z;
	LevelChunkMeshCeiling = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Chunk Ceiling"));
	RightWallStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Chunk Wall Right"));
	LeftWallStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Chunk Wall Left"));
	LevelChunkMeshCeiling->SetupAttachment(LevelChunkMeshFloor);
	RightWallStaticMesh->SetupAttachment(LevelChunkMeshFloor);
	LeftWallStaticMesh->SetupAttachment(LevelChunkMeshFloor);
	
	ActorStartOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("StartOverlapComponent"));
	ActorStartOverlap->SetupAttachment(LevelChunkMeshFloor);
	ActorStartOverlap->ShapeColor.Green;
	ActorStartOverlap->SetLineThickness(8.0f);
	ActorEndOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("EndOverlapComponent"));
	ActorEndOverlap->SetupAttachment(LevelChunkMeshFloor);
	ActorEndOverlap->ShapeColor.Red;
	ActorEndOverlap->SetLineThickness(8.0f);

	NodeEndConnector = CreateDefaultSubobject<UArrowComponent>(TEXT("Used to connect other nodes at end of Actor specified by the Procedural Chunk Manager"));
	NodeEndConnector->SetupAttachment(LevelChunkMeshFloor);

	NodeStartConnector = CreateDefaultSubobject<UArrowComponent>(TEXT("Used to connect other nodes at Start of Actor specified by the Procedural Chunk Manager"));
	NodeStartConnector->SetupAttachment(LevelChunkMeshFloor);

	SpawnInAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Spawn In Timeline Animation"));
	
}

// Sets default values
AProceduralChunkActor::AProceduralChunkActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	InitDefaultValues();

	//Bind event on overlap
	ActorStartOverlap->OnComponentBeginOverlap.AddDynamic(this, &AProceduralChunkActor::OnActorBeginOverlap);
	ActorEndOverlap->OnComponentBeginOverlap.AddDynamic(this, &AProceduralChunkActor::OnActorBeginOverlap);
	
}

void AProceduralChunkActor::SetFloorAsCeilings()
{
	if(LevelChunkMeshFloor->GetStaticMesh() && MirrorFloorAsCeiling)
	{
		LevelChunkMeshCeiling->SetStaticMesh(LevelChunkMeshFloor->GetStaticMesh());

		if(AutoMaticCelingHeightIfMirroredFromFloor)
		{
			// MeshThicknessOffset = MeshThicknessOffset;
			CeilingHeightIfFloorIsMirroredAndNotAutomatic = GetComponentsBoundingBox().GetSize().Y - PlatformHeightOffset;
			LevelChunkMeshCeiling->SetWorldLocation(FVector(LevelChunkMeshFloor->GetRelativeLocation().X, LevelChunkMeshFloor->GetRelativeLocation().Y, LevelChunkMeshFloor->GetRelativeLocation().Z + CeilingHeightIfFloorIsMirroredAndNotAutomatic));
		}
		else
		{
			LevelChunkMeshCeiling->SetWorldLocation(FVector(LevelChunkMeshFloor->GetRelativeLocation().X, LevelChunkMeshFloor->GetRelativeLocation().Y, LevelChunkMeshFloor->GetRelativeLocation().Z + CeilingHeightIfFloorIsMirroredAndNotAutomatic));
		}
	} else if(LevelChunkMeshFloor->GetStaticMesh() && CeilingMesh)
	{
		LevelChunkMeshCeiling->SetStaticMesh(CeilingMesh);
			if(AutoMaticCelingHeightIfMirroredFromFloor)
			{
				CeilingHeightIfFloorIsMirroredAndNotAutomatic = GetComponentsBoundingBox().GetSize().Y - PlatformHeightOffset;
			}
			LevelChunkMeshCeiling->SetWorldLocation(FVector(LevelChunkMeshFloor->GetRelativeLocation().X, LevelChunkMeshFloor->GetRelativeLocation().Y, LevelChunkMeshFloor->GetRelativeLocation().Z + CeilingHeightIfFloorIsMirroredAndNotAutomatic));
		
	}
}

void AProceduralChunkActor::SetMirrorFloorsAsWalls()
{
	float WorldSpaceSizeY = GetComponentsBoundingBox().GetExtent().Y;
	float WorldSpaceSizeZ = GetComponentsBoundingBox().GetExtent().Z;
	WorldSpaceCenterAxisX = GetComponentsBoundingBox().GetCenter().X;
	
	if(LevelChunkMeshFloor->GetStaticMesh() && MirrorFloorAsWalls)
	{
		WorldSpaceSizeY = GetComponentsBoundingBox().GetExtent().Y;
		WorldSpaceSizeZ = GetComponentsBoundingBox().GetExtent().Z;
		WorldSpaceCenterAxisX = GetComponentsBoundingBox().GetCenter().X;

		if(!bRemoveAllChunkWallsOnRightSide)
		{
			RightWallStaticMesh->SetStaticMesh(LevelChunkMeshFloor->GetStaticMesh());
			RightWallStaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
			RightWallStaticMesh->SetRelativeLocation(FVector(0.0f, WorldSpaceSizeY - (PlatformHeightOffset / 2), WorldSpaceSizeZ - PlatformHeightOffset ));
		}
		
		if(!bRemoveAllChunkWallsOnLeftSide)
		{
			LeftWallStaticMesh->SetStaticMesh(LevelChunkMeshFloor->GetStaticMesh());
			LeftWallStaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
			LeftWallStaticMesh->SetRelativeLocation(FVector(0.0f, (WorldSpaceSizeY * -1.0f) + PlatformHeightOffset / 2, WorldSpaceSizeZ - PlatformHeightOffset ));
		}
		
	} if(RightWallMesh && !bRemoveAllChunkWallsOnRightSide)
	{
		WorldSpaceSizeY = GetComponentsBoundingBox().GetExtent().Y;
		RightWallStaticMesh->SetStaticMesh(RightWallMesh);
		WorldSpaceCenterAxisX = RightWallStaticMesh->GetStaticMesh()->GetBoundingBox().GetExtent().X;
		WorldSpaceSizeZ = RightWallStaticMesh->GetStaticMesh()->GetBoundingBox().GetExtent().Z ;
		RightWallStaticMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		RightWallStaticMesh->SetRelativeLocation(FVector(0.0f, WorldSpaceSizeY - (PlatformHeightOffset / 2), WorldSpaceSizeZ + (PlatformHeightOffset * 0.5f) ));
	} if(LeftWallMesh && !bRemoveAllChunkWallsOnLeftSide)
	{
		WorldSpaceSizeY = GetComponentsBoundingBox().GetExtent().Y;
		LeftWallStaticMesh->SetStaticMesh(LeftWallMesh);
		WorldSpaceCenterAxisX = LeftWallStaticMesh->GetStaticMesh()->GetBoundingBox().GetExtent().X;
		WorldSpaceSizeZ = LeftWallStaticMesh->GetStaticMesh()->GetBoundingBox().GetExtent().Z;
		LeftWallStaticMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		LeftWallStaticMesh->SetRelativeLocation(FVector(0.0f, (WorldSpaceSizeY * -1.0f) + PlatformHeightOffset / 2, WorldSpaceSizeZ + (PlatformHeightOffset * 0.5f)  ));
	}
}

void AProceduralChunkActor::SetEdgeConnectorLocations()
{
	if(LevelChunkMeshFloor->GetStaticMesh())
	{
		NodeEndConnector->SetRelativeLocation(FVector(AActor::GetComponentsBoundingBox().GetExtent().X, 0.0f, 0.0f));
		NodeStartConnector->SetRelativeLocation(FVector(AActor::GetComponentsBoundingBox().GetExtent().X * -1.0f, 0.0f, 0.0f));
	}
}

void AProceduralChunkActor::SetActorRotationAfterWallsAndCeilingIsBuilt()
{
	if(FullPlatformNodeRotation != FRotator(0.0f, 0.0f, 0.0f))
	{
		SetActorRotation(FullPlatformNodeRotation);
	}
}

void AProceduralChunkActor::BuildStartAndEndOverlapCollisions(int32 Index)
{
	WorldSpaceVertexLocationEnd = GetActorLocation() + GetTransform().TransformVector(UE::Math::TVector<double>(VertexBuffer->VertexPosition(Index)));
				
	WorldSpaceCenterAxisX = GetComponentsBoundingBox().GetCenter().X;
	WorldSpaceCenterAxisY = GetComponentsBoundingBox().GetCenter().Y;
	WorldSpaceCenterAxisZ = GetComponentsBoundingBox().GetCenter().Z;
	WorldSpaceExtentX = GetComponentsBoundingBox().GetExtent().X;
	WorldSpaceExtentY = GetComponentsBoundingBox().GetExtent().Y;
	WorldSpaceExtentZ = GetComponentsBoundingBox().GetExtent().Z;
				
	float OverlapXAxisSize = 1.0;
	const FVector OverlapBoxSizesWhenCeilingMeshIsSet(OverlapXAxisSize ,WorldSpaceExtentY, WorldSpaceExtentZ);
	const FVector OverlapBoxSizesWhenCeilingMeshIsNotSet(OverlapXAxisSize ,WorldSpaceExtentY, WorldSpaceExtentY);
	WorldSpaceVertexLocationStart = GetActorLocation() + GetTransform().TransformVector(UE::Math::TVector<double>(VertexBuffer->VertexPosition(0)));
	FVector StartingOverlapLocationWhenCeilingMeshIsSet(WorldSpaceVertexLocationStart.X + OverlapXAxisSize, WorldSpaceCenterAxisY, GetComponentsBoundingBox().GetCenter().Z);
	FVector StartingOverlapLocationWhenCeilingMeshIsNotSet(WorldSpaceVertexLocationStart.X + OverlapXAxisSize, WorldSpaceCenterAxisY,  WorldSpaceVertexLocationEnd.Z + WorldSpaceExtentY);
				
	FVector EndingOverlapLocationWhenCeilingMeshIsSet(WorldSpaceVertexLocationEnd.X - OverlapXAxisSize, WorldSpaceCenterAxisY, GetComponentsBoundingBox().GetCenter().Z);
	FVector EndingOverlapLocationWhenCeilingMeshIsNotSet(WorldSpaceVertexLocationEnd.X - OverlapXAxisSize, WorldSpaceCenterAxisY, WorldSpaceVertexLocationEnd.Z + WorldSpaceExtentY);


	ActorStartOverlap->SetWorldLocation(MirrorFloorAsCeiling ? StartingOverlapLocationWhenCeilingMeshIsSet : StartingOverlapLocationWhenCeilingMeshIsNotSet);
	ActorStartOverlap->SetBoxExtent(MirrorFloorAsCeiling ? OverlapBoxSizesWhenCeilingMeshIsSet : OverlapBoxSizesWhenCeilingMeshIsNotSet);

	ActorEndOverlap->SetWorldLocation(MirrorFloorAsCeiling ? EndingOverlapLocationWhenCeilingMeshIsSet : EndingOverlapLocationWhenCeilingMeshIsNotSet);
	ActorEndOverlap->SetBoxExtent(MirrorFloorAsCeiling ? OverlapBoxSizesWhenCeilingMeshIsSet : OverlapBoxSizesWhenCeilingMeshIsNotSet);
}

void AProceduralChunkActor::SetActorScaleAfterWallsAndCeilingIsBuilt()
{
	if(ActorScaleOverride != FVector(1.0f, 1.0f, 1.0f))
	{
		SetActorScale3D(ActorScaleOverride);
	}
}

void AProceduralChunkActor::SetActorLocationOffsetIfSet()
{
	if(LevelChunkMeshFloor->GetStaticMesh())
	{
		float XLocation = GetActorLocation().X;
		float XOffset = ActorLocationOffsetOverride.X;
		float YLocation = GetActorLocation().Y;
		float YOffset = ActorLocationOffsetOverride.Y;
		float ZLocation = GetActorLocation().Z;
		float ZOffset = ActorLocationOffsetOverride.Z;
		SetActorLocation(FVector(XLocation + XOffset, YLocation + YOffset, ZLocation + ZOffset));
	}
}

void AProceduralChunkActor::BuildNodeByParameters()
{
	if (LevelChunkMeshFloor->GetStaticMesh()->GetRenderData()->LODResources.Num() > 0)
	{
		VertexBuffer = &LevelChunkMeshFloor->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for (int32 Index = 0; Index < VertexCount; Index++)
			{
			BuildStartAndEndOverlapCollisions(Index);
			}
			SetMirrorFloorsAsWalls();
			SetEdgeConnectorLocations();
			SetActorLocationOffsetIfSet();
			SetActorRotationAfterWallsAndCeilingIsBuilt();
			SetActorScaleAfterWallsAndCeilingIsBuilt();
		}
	}
}

void AProceduralChunkActor::ApplyRelativeTransformsToWallsAndCeilings()
{
	if(RightWallStaticMesh->GetStaticMesh() && bUseTransformOverrides)
	{
		float XLocation = RightWallStaticMesh->GetRelativeLocation().X;
		float XOffset = RightWallTransformOffset.GetLocation().X;
		float YLocation = RightWallStaticMesh->GetRelativeLocation().Y;
		float YOffset = RightWallTransformOffset.GetLocation().Y;
		float ZLocation = RightWallStaticMesh->GetRelativeLocation().Z;
		float ZOffset = RightWallTransformOffset.GetLocation().Z;
		RightWallStaticMesh->SetRelativeLocation(FVector(XLocation + XOffset, YLocation + YOffset, ZLocation + ZOffset));

		float XRot = RightWallStaticMesh->GetRelativeRotation().Pitch;
		float XRotOffset = RightWallTransformOffset.GetRotation().Rotator().Pitch;
		float YRot = RightWallStaticMesh->GetRelativeRotation().Yaw;
		float YRotOffset = RightWallTransformOffset.GetRotation().Rotator().Yaw;
		float ZRot = RightWallStaticMesh->GetRelativeRotation().Roll;
		float ZRotOffset = RightWallTransformOffset.GetRotation().Rotator().Roll;
		UE_LOG(LogTemp, Warning, TEXT("Before Rotation: Pitch=%f, Yaw=%f, Roll=%f"), XRot, YRot, ZRot);
		RightWallStaticMesh->SetRelativeRotation(FRotator(XRot + XRotOffset, YRot + YRotOffset, ZRot + ZRotOffset));
		UE_LOG(LogTemp, Warning, TEXT("After Rotation: Pitch=%f, Yaw=%f, Roll=%f"), RightWallStaticMesh->GetRelativeRotation().Pitch, RightWallStaticMesh->GetRelativeRotation().Yaw, RightWallStaticMesh->GetRelativeRotation().Roll);
	}
	if(LeftWallStaticMesh->GetStaticMesh() && bUseTransformOverrides)
	{
		float XLocation = LeftWallStaticMesh->GetRelativeLocation().X;
		float XOffset = LeftWallTransformOffset.GetLocation().X;
		float YLocation = LeftWallStaticMesh->GetRelativeLocation().Y;
		float YOffset = LeftWallTransformOffset.GetLocation().Y;
		float ZLocation = LeftWallStaticMesh->GetRelativeLocation().Z;
		float ZOffset = LeftWallTransformOffset.GetLocation().Z;
		LeftWallStaticMesh->SetRelativeLocation(FVector(XLocation + XOffset, YLocation + YOffset, ZLocation + ZOffset));

		float XRot = LeftWallStaticMesh->GetRelativeRotation().Pitch;
		float XRotOffset = LeftWallTransformOffset.GetRotation().Rotator().Pitch;
		float YRot = LeftWallStaticMesh->GetRelativeRotation().Yaw;
		float YRotOffset = LeftWallTransformOffset.GetRotation().Rotator().Yaw;
		float ZRot = LeftWallStaticMesh->GetRelativeRotation().Roll;
		float ZRotOffset = LeftWallTransformOffset.GetRotation().Rotator().Roll;
		LeftWallStaticMesh->SetRelativeRotation(FRotator(XRot + XRotOffset, YRot + YRotOffset, ZRot + ZRotOffset));
	}
	if(LevelChunkMeshCeiling->GetStaticMesh() && bUseTransformOverrides)
	{
		float XLocation = LevelChunkMeshCeiling->GetRelativeLocation().X;
		float XOffset = CeilingTransformOffset.GetLocation().X;
		float YLocation = LevelChunkMeshCeiling->GetRelativeLocation().Y;
		float YOffset = CeilingTransformOffset.GetLocation().Y;
		float ZLocation = LevelChunkMeshCeiling->GetRelativeLocation().Z;
		float ZOffset = CeilingTransformOffset.GetLocation().Z;
		LevelChunkMeshCeiling->SetRelativeLocation(FVector(XLocation + XOffset, YLocation + YOffset, ZLocation + ZOffset));

		float XRot = LevelChunkMeshCeiling->GetRelativeRotation().Pitch;
		float XRotOffset = CeilingTransformOffset.GetRotation().Rotator().Pitch;
		float YRot = LevelChunkMeshCeiling->GetRelativeRotation().Yaw;
		float YRotOffset = CeilingTransformOffset.GetRotation().Rotator().Yaw;
		float ZRot = LevelChunkMeshCeiling->GetRelativeRotation().Roll;
		float ZRotOffset = CeilingTransformOffset.GetRotation().Rotator().Roll;
		LevelChunkMeshCeiling->SetRelativeRotation(FRotator(XRot + XRotOffset, YRot + YRotOffset, ZRot + ZRotOffset));
	}
}

void AProceduralChunkActor::ApplyMaterialsToStaticMeshesIfSet()
{
	if(FloorMaterial)
	{
		LevelChunkMeshFloor->SetMaterial(0, FloorMaterial);
	}
	if(RightWallMaterial && RightWallStaticMesh->GetStaticMesh())
	{
		RightWallStaticMesh->SetMaterial(0, RightWallMaterial);
	}
	if(LeftWallMaterial && LeftWallStaticMesh->GetStaticMesh())
	{
		LeftWallStaticMesh->SetMaterial(0, LeftWallMaterial);
	}
	if(CeilingMaterial && LevelChunkMeshCeiling->GetStaticMesh())
	{
		LevelChunkMeshCeiling->SetMaterial(0, CeilingMaterial);
	}
}

void AProceduralChunkActor::SpawnAnimationFloatReturn(float value)
{
	SetActorLocation(FMath::Lerp(TimelineStartLocation, TimelineEndLocation, value));
	UE_LOG(LogTemp, Warning, TEXT("value of timeline so far: %f"), value);

}

void AProceduralChunkActor::OnSpawnAnimationTimelineFinished()
{
	//Expose on BP
}

// Called when the game starts or when spawned
void AProceduralChunkActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("This node index is: %d"), ThisNodeIndex);
	UE_LOG(LogTemp, Warning, TEXT("I'm responsible for spawning and deleting nodes: %s"), IsResponsibleForSpawnAndDeletingProcChunkActors ? TEXT("true") : TEXT("false"));
	switch (NodeOptions)
	{
	case EChunkerNodeOptions::SetStaticMeshesManually:
		MirrorFloorAsCeiling = false;
		MirrorFloorAsWalls = false;
		AutoMaticCelingHeightIfMirroredFromFloor = false;
		break;
	case EChunkerNodeOptions::MirrorFloorAsCeilingOnly:
		MirrorFloorAsCeiling = true;
		MirrorFloorAsWalls = false;
		AutoMaticCelingHeightIfMirroredFromFloor = false;
		break;

	case EChunkerNodeOptions::MirrorFloorAsWallsOnly:
		MirrorFloorAsCeiling = false;
		MirrorFloorAsWalls = true;
		AutoMaticCelingHeightIfMirroredFromFloor = false;
		break;
	case EChunkerNodeOptions::MirrorFloorAsCeilingAndWalls:
		MirrorFloorAsCeiling = true;
		MirrorFloorAsWalls = true;
		AutoMaticCelingHeightIfMirroredFromFloor = false;
		break;
	case EChunkerNodeOptions::MirrorFloorAsCeilingOnlyAndSetAutomaticCeilingHeight:
		MirrorFloorAsCeiling = true;
		MirrorFloorAsWalls = false;
		AutoMaticCelingHeightIfMirroredFromFloor = false;
		break;
	case EChunkerNodeOptions::MirrorFloorAsWallsOnlyAndSetAutomaticCeilingHeight:
		MirrorFloorAsCeiling = false;
		MirrorFloorAsWalls = true;
		AutoMaticCelingHeightIfMirroredFromFloor = true;
		break;
	case EChunkerNodeOptions::MirrorFloorAsCeilingAndWallsAndSetAutomaticCeilingHeight:
		MirrorFloorAsCeiling = true;
		MirrorFloorAsWalls = true;
		AutoMaticCelingHeightIfMirroredFromFloor = true;
		break;

	default:
		MirrorFloorAsCeiling = true;
		MirrorFloorAsWalls = true;
		AutoMaticCelingHeightIfMirroredFromFloor = true;
		break;
	}
	SetFloorAsCeilings();
	BuildNodeByParameters();

	ApplyMaterialsToStaticMeshesIfSet();

	//Apply Vector offsets

	ApplyRelativeTransformsToWallsAndCeilings();


	//Timeline stuff
	// InterpolationFunction.BindUFunction(this, FName("SpawnAnimationFloatReturn"));
	// SpawnAnimationTimelineFinished.BindUFunction(this, FName("OnSpawnAnimationTimelineFinished"));

	InterpolationFunction.BindDynamic(this, &AProceduralChunkActor::SpawnAnimationFloatReturn);
	SpawnAnimationTimelineFinished.BindDynamic(this, &AProceduralChunkActor::OnSpawnAnimationTimelineFinished);
	if(AnimationTimelineFCurve && bAnimateNewChunkNodesOnSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("I made it to the timeline part here"));

		//This adds the float curve timeline to the delegate interpolationFunction. We make the Animation Timeline FCurve assignable by the manager cpp
		SpawnInAnimationTimeline->AddInterpFloat(AnimationTimelineFCurve, InterpolationFunction, FName("Alpha"));
		//This tells where the function finished will be
		SpawnInAnimationTimeline->SetTimelineFinishedFunc(SpawnAnimationTimelineFinished);

		TimelineEndLocation = GetActorLocation();

		float RandomFloat = FMath::RandRange( -5000.0f, 5000.0f);
		FVector StaticTimelineStartLocation(GetActorLocation().X + TimelineXYZOffset.X, GetActorLocation().Y + TimelineXYZOffset.Y, GetActorLocation().Z + TimelineXYZOffset.Z);
		FVector RandomTimelineStartLocation(GetActorLocation().X + RandomFloat, GetActorLocation().Y + RandomFloat, GetActorLocation().Z + RandomFloat);
		TimelineStartLocation = bRandomTimelineXYZOffset ? RandomTimelineStartLocation : StaticTimelineStartLocation;

		SpawnInAnimationTimeline->SetLooping(false);
		SpawnInAnimationTimeline->SetIgnoreTimeDilation(true);
		SpawnInAnimationTimeline->Play();
	}
}

// Called every frame
void AProceduralChunkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralChunkActor::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent,
												AActor* OtherActor,
												UPrimitiveComponent* OtherComp,
												int32 OtherBodyIndex,
												bool bFromSweep,
												const FHitResult& SweepResult)
{
	// Ensure we have a valid other actor and it's not this actor
	if (OtherActor && (OtherActor != this) && OverlappedComponent == ActorStartOverlap && IsResponsibleForSpawnAndDeletingProcChunkActors)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped with %s"), *OverlappedComponent->GetName(), *OtherActor->GetName());
		if(IsResponsibleForSpawnAndDeletingProcChunkActors && OtherActor->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped with %s and has Player Tag. This is the Main Chunk Actor Doing the dirty work"), *OverlappedComponent->GetName(), *OtherActor->GetName());
			if(ProceduralChunkManager && !HasExecutedGenerateProceduralChunkActors)
			{
				//TODO: call function on manager to spawn new nodes in
				ProceduralChunkManager->GenerateNewProceduralChunkActors(OtherActor);
				HasExecutedGenerateProceduralChunkActors = true;
			}
		}
	} else if(OtherActor && (OtherActor != this) && OverlappedComponent == ActorEndOverlap && IsResponsibleForSpawnAndDeletingProcChunkActors)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped with %s"), *OverlappedComponent->GetName(), *OtherActor->GetName());
		if(IsResponsibleForSpawnAndDeletingProcChunkActors && OtherActor->ActorHasTag("Player"))
		{
			UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped with %s and has Player Tag. This is the Main Chunk Actor Doing the dirty work"), *OverlappedComponent->GetName(), *OtherActor->GetName());
			if(ProceduralChunkManager && !HasExecutedDestroyPreviousProceduralChunkActors)
			{
				//TODO: call function on manager to destroy previous chunk actors and copy new array to old array
				UE_LOG(LogTemp, Display, TEXT("Actor %s overlapped with %s and has Player Tag. Call Manager to destroy previous nodes and copy new to old array"), *OverlappedComponent->GetName(), *OtherActor->GetName());
				HasExecutedDestroyPreviousProceduralChunkActors = true;
				ProceduralChunkManager->DestroyPreviousProceduralChunkActors(OtherActor);
			}
		}
	}
}