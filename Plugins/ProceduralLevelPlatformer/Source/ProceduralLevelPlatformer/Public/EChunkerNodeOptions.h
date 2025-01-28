#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

UENUM(BlueprintType)
enum class EChunkerNodeOptions :uint8
{
	SetStaticMeshesManually											= 0b0000 UMETA(DisplayName="SetStaticMeshesManually"),
	MirrorFloorAsCeilingOnly										= 0b0001 UMETA(DisplayName="MirrorFloorAsCeilingOnly"),
	MirrorFloorAsWallsOnly											= 0b0010 UMETA(DisplayName="MirrorFloorAsWallsOnly"),
	MirrorFloorAsCeilingAndWalls									= 0b0100 UMETA(DisplayName="MirrorFloorAsCeilingAndWalls"),
	MirrorFloorAsCeilingOnlyAndSetAutomaticCeilingHeight			= 0b1000 UMETA(DisplayName="MirrorFloorAsCeilingOnlyAndSetAutomaticCeilingHeight"),
	MirrorFloorAsWallsOnlyAndSetAutomaticCeilingHeight				= 0b0011 UMETA(DisplayName="MirrorFloorAsWallsOnlyAndSetAutomaticCeilingHeight"),
	MirrorFloorAsCeilingAndWallsAndSetAutomaticCeilingHeight		= 0b0111 UMETA(DisplayName="MirrorFloorAsCeilingAndWallsAndSetAutomaticCeilingHeight"),
};

class PROCEDURALLEVELPLATFORMER_API RandomEChunkerNodeOptionsEnum
{
public:
	static EChunkerNodeOptions GetRandomChunkerNodeOption();
};